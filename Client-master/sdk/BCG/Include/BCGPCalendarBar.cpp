// BCGCalendarBar.cpp : implementation file
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//

#include "stdafx.h"
#include <math.h>
#include <locale.h>

#include "BCGCBPro.h"
#include "BCGPCalendarBar.h"
#include "BCGPCalendarMenuButton.h"
#include "BCGPPopupMenu.h"
#include "BCGPContextMenuManager.h"
#include "BCGPDateTimeCtrl.h"
#include "trackmouse.h"
#include "BCGPVisualManager.h"
#include "BCGPWorkspace.h"
#include "BCGProRes.h"
#include "BCGPLocalResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DISABLE_MONTH_POPUP

static const int SEPARATOR_SIZE = 2;
static const int nHoldMouseTimerID = 1;

UINT BCGM_CALENDAR_ON_SELCHANGED = ::RegisterWindowMessage (_T("BCGM_CALENDAR_ON_SELCHANGED"));

/////////////////////////////////////////////////////////////////////////////

class CCalendarWeekButton : public CBCGPCalendarButton  
{
	friend class CBCGPCalendarBar;
	friend class CBCGPCalendarMenuButton;

	DECLARE_SERIAL(CCalendarWeekButton)

protected:
	virtual void SetDate(const COleDateTime& date)
	{
		if (date != m_Calendar)
		{
			CBCGPDefaultLocale dl;

			m_Calendar = date;
			m_strText = m_Calendar.Format(_T("%a"));
			m_pParentBar->InvalidateRect(m_rect);
		}
	}

	virtual BOOL IsActualDay () const
	{
		return FALSE;
	}
};

IMPLEMENT_SERIAL(CCalendarWeekButton, CBCGPCalendarButton, 1)

class CCalendarWeekNumButton : public CBCGPCalendarButton  
{
	friend class CBCGPCalendarBar;
	friend class CBCGPCalendarMenuButton;

	DECLARE_SERIAL(CCalendarWeekNumButton)

protected:
	virtual void SetDate(const COleDateTime& date)
	{
		if (date != m_Calendar)
		{
			CBCGPDefaultLocale dl;

			m_Calendar = date;
			m_strText.Format(_T("%02d"), CBCGPCalendarBar::GetWeekNo(m_Calendar, m_pParentBar->m_nWeekStart)); 
			m_pParentBar->InvalidateRect(m_rect);
		}
	}

	virtual COLORREF GetTextColor ()
	{
		ASSERT_VALID (m_pParentBar);
		return IsCurrMonth () ? 
			m_pParentBar->m_clrWeekText : m_pParentBar->m_clrWeekTextDisabled;
	}

	virtual BOOL IsActualDay () const
	{
		return FALSE;
	}
};

IMPLEMENT_SERIAL(CCalendarWeekNumButton, CBCGPCalendarButton, 1)

class CCalendarTodayButton : public CBCGPCalendarButton  
{
	friend class CBCGPCalendarBar;
	friend class CBCGPCalendarMenuButton;

	DECLARE_SERIAL(CCalendarTodayButton)

protected:
	CCalendarTodayButton(int iImage = -1)
	{
		m_iImage = iImage;
	}

	virtual BOOL OnToolHitTest(const CWnd* /*pWnd*/, TOOLINFO* pTI)
	{
		if (m_nStyle & TBBS_DISABLED)
		{
			return FALSE;
		}

		if (!CBCGPToolBar::GetShowTooltips () || pTI == NULL)
		{
			return FALSE;
		}

		CString str = _T("Today");

		pTI->lpszText = (LPTSTR) ::calloc ((str.GetLength () + 1), sizeof (TCHAR));
		_tcscpy (pTI->lpszText, str);

		return TRUE;
	}

	virtual void SetDate(const COleDateTime& date)
	{
		if (date != m_Calendar)
		{
			m_Calendar = date;
			m_pParentBar->InvalidateRect(m_rect);
		}
	}
	
	virtual BOOL IsActualDay () const
	{
		return FALSE;
	}
};

IMPLEMENT_SERIAL(CCalendarTodayButton, CBCGPCalendarButton, 1)

//*******************************************************
class CCalendarNavigateButton : public CBCGPCalendarButton  
{
	friend class CBCGPCalendarBar;
	friend class CBCGPCalendarMenuButton;

	DECLARE_SERIAL(CCalendarNavigateButton)

protected:
	CCalendarNavigateButton(int iImage = -1)
	{
		m_iImage = iImage;
	}

	virtual BOOL OnToolHitTest(const CWnd* /*pWnd*/, TOOLINFO* pTI)
	{
		if (m_nStyle & TBBS_DISABLED)
		{
			return FALSE;
		}

		if (!CBCGPToolBar::GetShowTooltips () || pTI == NULL)
		{
			return FALSE;
		}

		CString str;

		switch (m_iImage)
		{
		case 0:
			str = _T("Prev. year");
			break;

		case 1:
			str = _T("Prev. month");
			break;

		case 2:
			str = _T("Next month");
			break;

		case 3:
			str = _T("Next year");
			break;
		}

		pTI->lpszText = (LPTSTR) ::calloc ((str.GetLength () + 1), sizeof (TCHAR));
		_tcscpy (pTI->lpszText, str);

		return TRUE;
	}

	virtual void SetDate(const COleDateTime& date)
	{
		if (date != m_Calendar)
		{
			m_Calendar = date;
			m_pParentBar->InvalidateRect(m_rect);
		}
	}

	virtual BOOL IsActualDay () const
	{
		return FALSE;
	}
};

IMPLEMENT_SERIAL(CCalendarNavigateButton, CBCGPCalendarButton, 1)

//*******************************************************
class CCalendarMonthButton : public CBCGPToolbarButton  
{
	friend class CBCGPCalendarBar;
	friend class CBCGPCalendarMenuButton;

	DECLARE_SERIAL(CCalendarMonthButton)

	
protected:

	CCalendarMonthButton()
	{
		m_pParentBar = NULL;
	}

	virtual BOOL OnToolHitTest(const CWnd* /*pWnd*/, TOOLINFO* pTI)
	{
		if (m_nStyle & TBBS_DISABLED)
		{
			return FALSE;
		}

		if (!CBCGPToolBar::GetShowTooltips () || pTI == NULL)
		{
			return FALSE;
		}

		CString str = _T("Change month");

		pTI->lpszText = (LPTSTR) ::calloc ((str.GetLength () + 1), sizeof (TCHAR));
		_tcscpy (pTI->lpszText, str);

		return TRUE;
	}

	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
						BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
						BOOL bHighlight = FALSE,
						BOOL bDrawBorder = TRUE,
						BOOL bGrayDisabledButtons = TRUE);
	
	virtual void OnChangeParentWnd (CWnd* pWndParent)
	{
		CBCGPToolbarButton::OnChangeParentWnd (pWndParent);
		m_pParentBar = DYNAMIC_DOWNCAST (CBCGPCalendarBar, pWndParent);
	}

	CSize ContextSize (CWnd* pWnd)
	{
		ASSERT (pWnd->GetSafeHwnd () != NULL);

		CClientDC dc (pWnd);
		CFont* pOldFont = m_pParentBar->SelectDefaultFont (&dc);
		
		CSize btnSize = dc.GetTextExtent(m_strText);

		dc.SelectObject (pOldFont);
		return btnSize;
	}

	CBCGPCalendarBar*	m_pParentBar;
	virtual BOOL IsActualDay () const
	{
		return FALSE;
	}
};

void CCalendarMonthButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* /*pImages*/,
								BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,
								BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParentBar);

	CBCGPToolbarButton::OnDraw (pDC, rect, NULL,
								bHorz, bCustomizeMode, bHighlight,
								bDrawBorder, bGrayDisabledButtons);
	
	int nOldBkMode = pDC->SetBkMode (TRANSPARENT);
	COLORREF clrOldText = pDC->SetTextColor (m_pParentBar->m_clrNavButton);

	CFont* pOldFont = m_pParentBar->SelectDefaultFont (pDC);
	CRect rectWnd = rect;

	if (m_nStyle ^ TBBS_DISABLED)
	{	
		rectWnd.DeflateRect (1, 1, rect.Height(), 1);
		// Draw menu triangle:
		CRect rectArrow = rectWnd;
		rectArrow.left = rect.right;

		int iXMiddle = rectArrow.left + rectArrow.Width () / 2;

		rectArrow.DeflateRect (0, rectArrow.Height () / 3);
		rectArrow.DeflateRect (rectArrow.Height () / 3, rectArrow.Height () / 3);
		rectArrow.left = iXMiddle - rectArrow.Height () - 1;
		rectArrow.right = iXMiddle + rectArrow.Height () + 1;

		int iHalfWidth =	(rectArrow.Width () % 2 != 0) ?
							(rectArrow.Width () - 1) / 2 :
							rectArrow.Width () / 2;

		CPoint pts [3];
		pts[0].x = rectArrow.left;
		pts[0].y = rectArrow.top;
		pts[1].x = rectArrow.right;
		pts[1].y = rectArrow.top;
		pts[2].x = rectArrow.left + iHalfWidth;
		pts[2].y = rectArrow.bottom + 1;

		CBrush brArrow (pDC->GetTextColor ());

		CPen* pOldPen = (CPen*) pDC->SelectStockObject (NULL_PEN);
		CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brArrow);

		pDC->SetPolyFillMode (WINDING);
		pDC->Polygon (pts, 3);

		pDC->SelectObject (pOldBrush);
		pDC->SelectObject (pOldPen);
		pDC->DrawText (m_strText, rectWnd, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
	}
	else
	{
		pDC->DrawText (m_strText, rectWnd, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	pDC->SelectObject (pOldFont);
	pDC->SetTextColor (clrOldText);
	pDC->SetBkMode (nOldBkMode);
	
}
IMPLEMENT_SERIAL(CCalendarMonthButton, CBCGPToolbarButton, 1)


/////////////////////////////////////////////////////////////////////////////
// CBCGPCalendarBar

const UINT CBCGPCalendarBar::CBR_WEEKDAYSEL = 0x1;
const UINT CBCGPCalendarBar::CBR_WEEKNUMBER = 0x2;
const UINT CBCGPCalendarBar::CBR_MULTISELECTION = 0x4;
const UINT CBCGPCalendarBar::CBR_ENABLED = 0x8; // still has not effect - 6.2
const UINT CBCGPCalendarBar::CBR_NAVIGATION_BUTTONS = 0x10;


CString CBCGPCalendarBar::m_strMonthNames[12];

IMPLEMENT_SERIAL(CBCGPCalendarBar, CBCGPPopupMenuBar, 1)

CBCGPCalendarBar::CBCGPCalendarBar()
{
	CommonInit ();

    m_dateSelected = COleDateTime::GetCurrentTime();
	m_nCommandID = 0;
	m_bIsTearOff = TRUE;
	m_pParentBtn = NULL;
}
//**************************************************************************************
CBCGPCalendarBar::CBCGPCalendarBar (const COleDateTime& month, 
				UINT nCommandID, CBCGPDateTimeCtrl* pParentBtn) :
	m_dateSelected (month),
	m_bIsTearOff (FALSE),
	m_nCommandID (nCommandID),
	m_pParentBtn (pParentBtn)
{
	CommonInit ();
}
//**************************************************************************************
CBCGPCalendarBar::CBCGPCalendarBar (CBCGPCalendarBar& src, UINT uiCommandID, BOOL enableSelection) :
		m_dateSelected (src.m_dateSelected),
		m_bIsTearOff (TRUE),
		m_nCommandID (uiCommandID),
		m_pParentBtn (NULL)

{
	CommonInit ();

	if (enableSelection) 
	{
		m_styleFlags |= CBR_MULTISELECTION;
	}
	else				
	{
		m_styleFlags &= (~CBR_MULTISELECTION);
	}
}
//**************************************************************************************
void CBCGPCalendarBar::CommonInit ()
{
	m_bLocked = TRUE;
	m_nWeekStart = 1;
    m_iButtonCapture = -1;
	m_bIsEnabled = TRUE;
	m_nVertMargin = 4;
	m_nHorzMargin = 4;
	m_bInternal = FALSE;
	m_selMode  = Select_BYNONE;
	m_styleFlags = (CBR_WEEKDAYSEL | CBR_WEEKNUMBER | CBR_ENABLED | CBR_NAVIGATION_BUTTONS)
					& (~CBR_MULTISELECTION);
	m_sizeBox = CSize (0, 0);
	m_sizeBoxMin = CSize (0, 0);
	m_nDaysHorzOffset = 0;
	m_bIsCtrl = FALSE;
}
//**************************************************************************************
CBCGPCalendarBar::~CBCGPCalendarBar()
{
}
//*****************************************************************************************
void CBCGPCalendarBar::SetState(UINT flags, UINT mask)
{
	m_styleFlags &= ~mask;
	m_styleFlags |= (flags & mask);

	if (!(m_styleFlags & CBR_MULTISELECTION))
	{
		ClearSelectedDays();
	}

	if (GetSafeHwnd () != NULL)
	{
		Rebuild ();
		AdjustLocations ();

		Invalidate();
		UpdateWindow();
	}
}
//*************************************************************************************
void CBCGPCalendarBar::AdjustLocations ()
{
	ASSERT_VALID(this);

	if (GetSafeHwnd () == NULL || !::IsWindow (m_hWnd))
	{
		return;
	}

	m_sizeBox.cx = max (m_sizeBox.cx, m_sizeBoxMin.cx);
	m_sizeBox.cy = max (m_sizeBox.cy, m_sizeBoxMin.cy);

	CClientDC dc (this);

	CFont* pPrevFont = SelectDefaultFont (&dc);
	ASSERT (pPrevFont != NULL);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	m_iTitleHeight = tm.tmHeight + m_nVertMargin * 2;

    CRect rectClient;
	GetClientRect (&rectClient);

	int nXNavButtonOffeset = rectClient.left;
	int nYNavButtonOffeset = rectClient.top;

	int nWeekBarWidth = (m_styleFlags & CBCGPCalendarBar::CBR_WEEKNUMBER) ?
		CBCGPToolBar::GetMenuImageSize ().cx + 
		2 * CBCGPVisualManager::GetInstance ()->GetMenuImageMargin () : 0;

	int xDaysStart = rectClient.left + nWeekBarWidth + m_nHorzMargin + m_nDaysHorzOffset;

	rectClient.DeflateRect (m_nHorzMargin, m_nVertMargin);

	int x = rectClient.left;
	int y = rectClient.top;
	rectClient.top += m_iTitleHeight;

	int i = 0;

	int iWeekNo = 0;
	int iWeekNum = 0;
	int iDayNum = 0;

	BOOL bPrevSeparator = FALSE;

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL; i++)
	{
		CRect rectButton (0, 0, 0, 0);

		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		ASSERT_VALID (pButton);
		
		if (pButton->m_nStyle & TBBS_SEPARATOR)
		{
			if (bPrevSeparator)
			{
				rectButton.SetRectEmpty ();
			}
			else
			{
				// Next line
				x = xDaysStart;
				y = rectClient.top + m_sizeBox.cy;

				rectButton = CRect (
					CPoint (x, y),
					CSize (rectClient.right - x, SEPARATOR_SIZE));
			}

			bPrevSeparator = TRUE;
		}
		else
		{
			if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarWeekNumButton)))
			{
				CCalendarWeekNumButton* pCalendarButton = DYNAMIC_DOWNCAST (CCalendarWeekNumButton, pButton);
				if (pCalendarButton == NULL)
				{
					continue;
				}

				ASSERT_VALID (pCalendarButton);

				x = m_nHorzMargin / 2;
				y = rectClient.top  + SEPARATOR_SIZE + m_sizeBox.cy * (iWeekNo + 1) ;

                rectButton = CRect (CPoint (x, y), CSize (nWeekBarWidth - m_nHorzMargin, m_sizeBox.cy));

				bPrevSeparator = FALSE;

				iWeekNo ++;
			}

			else if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarTodayButton)) ||
					pButton->IsKindOf (RUNTIME_CLASS (CCalendarNavigateButton)))
			{
				rectButton = CRect (
					CPoint (nXNavButtonOffeset, nYNavButtonOffeset),
					CSize (m_iTitleHeight - 1, m_iTitleHeight - 1));
				rectButton.DeflateRect(1, 1);

				nXNavButtonOffeset = rectButton.right;
				bPrevSeparator = FALSE;
			}
			else if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarWeekButton)))
			{
				CCalendarWeekButton* pCalendarButton = DYNAMIC_DOWNCAST (CCalendarWeekButton, pButton);
				
				if (pCalendarButton == NULL)
				{
					continue;
				}

				ASSERT_VALID (pCalendarButton);
				{

					x = xDaysStart + m_sizeBox.cx * (iWeekNum % 7);
					y = rectClient.top;

					rectButton = CRect (CPoint (x, y), m_sizeBox);
					bPrevSeparator = FALSE;
				}
				iWeekNum ++;
			}
			else if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarMonthButton)))
			{
				const CString strYear = _T("0000");
				int nYearWidth = dc.GetTextExtent(strYear).cx;

				int nMonthWidth = 
					(m_styleFlags & CBCGPCalendarBar::CBR_NAVIGATION_BUTTONS) ?
						rectClient.Width () + 2 * m_nHorzMargin - 5 * (m_iTitleHeight - 1) - nYearWidth :
						rectClient.Width () - nYearWidth;

				pButton->m_strText = GetMonthName (m_dateSelected.GetMonth ());

				rectButton = CRect (
					CPoint (nXNavButtonOffeset, nYNavButtonOffeset),
					CSize (nMonthWidth, m_iTitleHeight - 1));

				rectButton.DeflateRect (0, 1);

				nXNavButtonOffeset = rectButton.right;
				bPrevSeparator = FALSE;
			}			
			else if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPCalendarButton)))
			{
				CBCGPCalendarButton* pCalendarButton = DYNAMIC_DOWNCAST (CBCGPCalendarButton, pButton);
				
				if (pCalendarButton == NULL)
				{
					continue;
				}

				ASSERT_VALID (pCalendarButton);

				x = xDaysStart + m_sizeBox.cx * (iDayNum % 7);
				y = rectClient.top  + SEPARATOR_SIZE + m_sizeBox.cy * (iDayNum / 7 + 1) ;

				rectButton = CRect (
					CPoint (x, y), m_sizeBox);

				bPrevSeparator = FALSE;

				iDayNum ++;
			}
		}

		pButton->SetRect (rectButton);
	}
	
	dc.SelectObject (pPrevFont);
	UpdateTooltips ();
}
//***************************************************************************************
CSize CBCGPCalendarBar::CalcSize (BOOL /*bVertDock*/)
{
	// Calculate caption width:
	CClientDC dc (this);
	CFont* pOldFont = SelectDefaultFont (&dc);

	int nMonthMaxWidth = 0;
	for(int i = 0; i < 12; i ++)
	{
		CString strMonth = GetMonthName (i + 1);
		nMonthMaxWidth = max (dc.GetTextExtent(strMonth).cx + 2 * m_nHorzMargin, nMonthMaxWidth);
	}

	CString strYear = _T("0000");
	int nYearWidth = dc.GetTextExtent(strYear).cx;

	dc.SelectObject (pOldFont);

	int nDaysWidth = (m_styleFlags & CBCGPCalendarBar::CBR_WEEKNUMBER) ?
		8 * m_sizeBox.cx : 7 * m_sizeBox.cx;
	nDaysWidth +=  + 2 * m_nHorzMargin;

	int nNavBarWidth = (m_styleFlags & CBCGPCalendarBar::CBR_NAVIGATION_BUTTONS) ?
		nMonthMaxWidth + 5 * m_sizeBox.cx + nYearWidth :
		nMonthMaxWidth + nYearWidth;

	m_nDaysHorzOffset = nNavBarWidth > nDaysWidth ?
		max (0, (nNavBarWidth - nDaysWidth) / 2) : 0;
		
	return CSize(
			max (nNavBarWidth, nDaysWidth), 
			7 * (m_sizeBox.cy + 2) + 2 * m_nVertMargin + 
			SEPARATOR_SIZE + m_iTitleHeight);
}
//*************************************************************************************
CSize CBCGPCalendarBar::GetCalendarGridSize (BOOL /*bVertDock*/) const
//
// Calculate number of columns and rows in the grid
//
{
	int nNumColumns = 7;
	int nNumRows = 7;

	return CSize (nNumColumns, nNumRows);
}

BEGIN_MESSAGE_MAP(CBCGPCalendarBar, CBCGPPopupMenuBar)
	//{{AFX_MSG_MAP(CBCGPCalendarBar)
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPCalendarBar message handlers

void CBCGPCalendarBar::ContextToSize ()
{
	ASSERT (GetSafeHwnd () != NULL);

	int nHorzOffset = GetMenuImageSize ().cx + 
			2 * CBCGPVisualManager::GetInstance ()->GetMenuImageMargin () + 1;
	
	CRect rectClient;
	GetClientRect (rectClient);

	rectClient.top += m_iTitleHeight;
	rectClient.left += nHorzOffset;

	m_sizeBox.cx = rectClient.Width () / 7 - 1;
	m_sizeBox.cy = (rectClient.Height () - SEPARATOR_SIZE) / 7 - 1;

	m_sizeBox.cx = max (m_sizeBox.cx, m_sizeBoxMin.cx);
	m_sizeBox.cy = max (m_sizeBox.cy, m_sizeBoxMin.cy);

	AdjustLocations ();

	SetWindowPos (&wndTop, -1, -1, -1, -1,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}
//**************************************************************************************
void CBCGPCalendarBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (m_bIsCtrl)
	{
		if (GetStyle() & WS_BORDER) 
		{
			lpncsp->rgrc[0].left++; 
			lpncsp->rgrc[0].top++ ;
			lpncsp->rgrc[0].right--;
			lpncsp->rgrc[0].bottom--;
		}

		return;
	}

	if (m_bIsTearOff)
	{
		CBCGPToolBar::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
	else
	{
		CBCGPPopupMenuBar::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
//**************************************************************************************
void CBCGPCalendarBar::OnNcPaint() 
{
	if (m_bIsCtrl)
	{
		if (GetStyle () & WS_BORDER)
		{
			CBCGPVisualManager::GetInstance ()->OnDrawControlBorder (this);
		}

		return;
	}

	if (m_bIsTearOff)
	{
		CBCGPToolBar::OnNcPaint();
	}
	else
	{
		CBCGPPopupMenuBar::OnNcPaint();
	}
}
//**************************************************************************************
int CBCGPCalendarBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPPopupMenuBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	const CSize sizeNavImage (10, 10);
	const int nImageMargin = 2;

	// Load navigation images:
	{
		SetLockedSizes (sizeNavImage + CSize (nImageMargin, nImageMargin), sizeNavImage);

		CBCGPLocalResource	lr;
		BOOL bIsLoaded = LoadBitmap (IDB_BCGBARRES_CALENDAR, 0, 0, TRUE);
		ASSERT (bIsLoaded);
	}

	m_sizeBox = sizeNavImage;
	m_bLeaveFocus = (lpCreateStruct->style & WS_TABSTOP) == 0;

	CClientDC dc (this);

	CFont* pOldFont = SelectDefaultFont (&dc);
	ASSERT (pOldFont != NULL);

	int year = m_dateSelected.GetYear();
	int month = m_dateSelected.GetMonth();
	
	CBCGPDefaultLocale dl;

	CString strDay = _T(" 99");
	CSize textSize = dc.GetTextExtent(strDay);

	m_sizeBox.cx = max (textSize.cx, m_sizeBox.cx);
	m_sizeBox.cy = max (textSize.cy, m_sizeBox.cy);

	for (int i = 1; i <= 7; i ++)
	{
		COleDateTime dt(year, month, i, 0, 0, 0);
		CString strWeekday = dt.Format(_T("%a"));

		textSize = dc.GetTextExtent(strWeekday);

		m_sizeBox.cx = max (textSize.cx, m_sizeBox.cx);
		m_sizeBox.cy = max (textSize.cy, m_sizeBox.cy);
	}

	if (pOldFont != NULL)
	{
		dc.SelectObject (pOldFont);
	}

	m_sizeBox.cx += nImageMargin;
	m_sizeBox.cy += nImageMargin;

	m_sizeBoxMin = m_sizeBox;

	Rebuild ();
	AdjustLocations();

	if (m_pParentBtn != NULL)
	{
		SetCapture ();
		SelectDay(m_dateSelected, FALSE, FALSE);
	}
	else
	{
		m_LastAddedDate = COleDateTime::GetCurrentTime();
	}

	return 0;
}
//*************************************************************************************
void CBCGPCalendarBar::Rebuild ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	RemoveAllButtons ();

	if (m_styleFlags & CBCGPCalendarBar::CBR_NAVIGATION_BUTTONS)
	{
		InsertButton (new CCalendarTodayButton(4));
		InsertButton (new CCalendarNavigateButton (0));
		InsertButton (new CCalendarNavigateButton (1));
	}

	InsertButton (
		CCalendarMonthButton());

	if (m_styleFlags & CBCGPCalendarBar::CBR_NAVIGATION_BUTTONS)
	{
		InsertButton (new CCalendarNavigateButton (2));
		InsertButton (new CCalendarNavigateButton (3));
	}

	for (int i = 0; i < 7; i ++)
	{
		InsertButton (
			new CCalendarWeekButton ());
	}

	if (m_styleFlags & CBCGPCalendarBar::CBR_WEEKNUMBER)
		for (i = 0; i < 6; i ++)
		{
			InsertButton (
				new CCalendarWeekNumButton ());
		}

	InsertSeparator ();
	
	for (i = 0; i < 42; i ++)
	{
		InsertButton (
			new CBCGPCalendarButton ());
	}

	SetDate (m_dateSelected);
}

class CBCGPCalendarCCmdUI : public CCmdUI
{
public:
	CBCGPCalendarCCmdUI();

public: // re-implementations only
	virtual void Enable(BOOL bOn);
	BOOL m_bEnabled;
};

CBCGPCalendarCCmdUI::CBCGPCalendarCCmdUI()
{
	m_bEnabled = TRUE;  // assume it is enabled
}
//*************************************************************************************
void CBCGPCalendarCCmdUI::Enable(BOOL bOn)
{
	m_bEnabled = bOn;
	m_bEnableChanged = TRUE;
}
//*************************************************************************************
void CBCGPCalendarBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);

	if (m_nCommandID == 0 || m_nCommandID == (UINT)-1)
	{
		CBCGPPopupMenuBar::OnUpdateCmdUI (pTarget, bDisableIfNoHndler);
		return;
	}

	CBCGPCalendarCCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = m_nCommandID;

	BOOL bIsEnabled = FALSE;
	if (pTarget->OnCmdMsg (m_nCommandID, CN_UPDATE_COMMAND_UI, &state, NULL))
	{
		bIsEnabled = state.m_bEnabled;
	}
	else if (bDisableIfNoHndler && !state.m_bEnableChanged)
	{
		AFX_CMDHANDLERINFO info;
		info.pTarget = NULL;

		bIsEnabled = pTarget->OnCmdMsg (m_nCommandID, CN_COMMAND, &state, &info);
	}

	if (bIsEnabled != m_bIsEnabled)
	{
		m_bIsEnabled = bIsEnabled;

		for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPCalendarButton* pCalendarButton = DYNAMIC_DOWNCAST (CBCGPCalendarButton, m_Buttons.GetNext (pos));
			if (pCalendarButton != NULL)
			{
				pCalendarButton->m_nStyle &= ~TBBS_DISABLED;
				if (!bIsEnabled)
				{
					pCalendarButton->m_nStyle |= TBBS_DISABLED;
				}
			}
		}

		Invalidate ();
		UpdateWindow ();
	}

	CBCGPPopupMenuBar::OnUpdateCmdUI (pTarget, bDisableIfNoHndler);
}
//*************************************************************************************
void  CBCGPCalendarBar::OnFillBackground (CDC* pDC)
{
	ASSERT_VALID (pDC);

	CRect rectClient;
	GetClientRect (rectClient);

	if (m_styleFlags & CBCGPCalendarBar::CBR_WEEKNUMBER)
	{
		CRect rectWeekColumn = rectClient;

		rectWeekColumn.top += m_iTitleHeight;
		rectWeekColumn.right = rectWeekColumn.left + CBCGPToolBar::GetMenuImageSize ().cx + 
					2 * CBCGPVisualManager::GetInstance ()->GetMenuImageMargin () + 1;

		CBCGPVisualManager::GetInstance ()->OnFillCalendarBarWeekColumn (
			pDC, rectWeekColumn, m_clrWeekText, m_clrWeekTextDisabled);
	}

	CRect rectNavRow = rectClient;
//	rectNavRow.bottom = rectClient.top + m_iTitleHeight;

	rectNavRow.DeflateRect(0, 0, 0, rectNavRow.Height () - m_iTitleHeight + 1);

	m_clrNavButton = CBCGPVisualManager::GetInstance ()->OnFillCalendarBarNavigationRow
		(pDC, rectNavRow);

	int nOldBkMode = pDC->SetBkMode (TRANSPARENT);
	COLORREF clrOldText = pDC->SetTextColor (m_clrNavButton);

	CFont* pOldFont = SelectDefaultFont (pDC);

	CString strYear = m_dateSelected.Format(_T("%Y"));
	rectNavRow.DeflateRect (m_nHorzMargin, 0);
	pDC->DrawText (strYear, rectNavRow, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);

	pDC->SelectObject (pOldFont);
	pDC->SetTextColor (clrOldText);
	pDC->SetBkMode (nOldBkMode);
}
//*************************************************************************************
BOOL CBCGPCalendarBar::OnSendCommand (const CBCGPToolbarButton* pButton)
{
	BOOL  isCtrlPressed =  (0x8000 & GetKeyState(VK_CONTROL)) != 0;
	BOOL  isShiftPressed = (0x8000 & GetKeyState(VK_SHIFT)) != 0;
	
    COleDateTime date = COleDateTime::GetCurrentTime();
	CBCGPCalendarMenuButton* pCalendarMenuButton = NULL;

	CBCGPPopupMenu* pParentMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, GetParent ());
	if (pParentMenu != NULL)
	{
		pCalendarMenuButton = DYNAMIC_DOWNCAST (CBCGPCalendarMenuButton, pParentMenu->GetParentButton ());
	}

	LPARAM buttonKind = Click_NOWHERE;
	CBCGPCalendarButton* pCalendarButton;

	pCalendarButton = DYNAMIC_DOWNCAST (CCalendarNavigateButton, pButton);
	if (pCalendarButton)
	{
		if (m_bIsTearOff)
			SetDate(pCalendarButton->m_Calendar);
		else
		{
			int iY = pCalendarButton->m_Calendar.GetYear();
			int iM = pCalendarButton->m_Calendar.GetMonth();
			int iD = m_dateSelected.GetDay ();
			COleDateTime dateToSelect(iY, iM, 1, 0, 0, 0);
			int iMaxDay = GetMaxMonthDay(dateToSelect);
			if (iD > iMaxDay)
				iD = iMaxDay;
			SetDate(COleDateTime (iY, iM, iD, 0, 0, 0));
		}
		return TRUE;
	}
	else
	{
		pCalendarButton = DYNAMIC_DOWNCAST (CCalendarWeekButton, pButton);

		if (pCalendarButton)
		{
			if (m_bIsTearOff 
				&& (m_styleFlags & CBCGPCalendarBar::CBR_MULTISELECTION) != 0 
				&& (m_styleFlags & CBCGPCalendarBar::CBR_WEEKDAYSEL) != 0)
			{
				buttonKind = Click_WEEKDAY;
				SelectWeekday(pCalendarButton->m_Calendar, isCtrlPressed, isShiftPressed);
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			pCalendarButton = DYNAMIC_DOWNCAST (CCalendarWeekNumButton, pButton);
			if (pCalendarButton)
			{
			if (m_bIsTearOff 
				&& (m_styleFlags & CBCGPCalendarBar::CBR_MULTISELECTION) != 0 
				&& (m_styleFlags & CBCGPCalendarBar::CBR_WEEKNUMBER) != 0)
				{
					buttonKind = Click_WEEKNO;
					CString str = pCalendarButton->m_Calendar.Format ();
					SelectWeek(pCalendarButton->m_Calendar, isCtrlPressed, isShiftPressed);
				}
				else
				{
					return TRUE;
				}
			}
			else
			{
				pCalendarButton = DYNAMIC_DOWNCAST (CBCGPCalendarButton, pButton);
				if (!isCtrlPressed && !isShiftPressed)
					buttonKind = Click_DAY;
				if (pCalendarButton != NULL)
					SelectDay(pCalendarButton->m_Calendar, isCtrlPressed, isShiftPressed);
			}
		}
	}
	
	if (pCalendarButton != NULL)
	{
		date = pCalendarButton->m_Calendar;
	}

	if (pCalendarMenuButton != NULL)
	{
		pCalendarMenuButton->SetDate (date);
		InvokeMenuCommand (pCalendarMenuButton->m_nID, pCalendarMenuButton);
	}
	else if (m_pParentBtn != NULL)
	{
		m_pParentBtn->SetDate (date);
		GetParent ()->PostMessage (WM_CLOSE);
	}
	else if (Click_DAY == buttonKind)
	{
		ASSERT (m_nCommandID != 0);

		SetDate (date);

		CObList listButtons;
		if (CBCGPToolBar::GetCommandButtons (m_nCommandID, listButtons) > 0)
		{
			for (POSITION pos = listButtons.GetHeadPosition (); pos != NULL;)
			{
				CBCGPCalendarMenuButton* pButton = 
					DYNAMIC_DOWNCAST (CBCGPCalendarMenuButton, listButtons.GetNext (pos));
				if (pButton != NULL)
				{
					pButton->SetDate (date, FALSE);
				}
			}
		}

		CBCGPCalendarMenuButton::SetCalendarByCmdID (m_nCommandID, date);
		GetOwner()->SendMessage (WM_COMMAND, m_nCommandID);    // send command

		if (BCGCBProGetTopLevelFrame (this) != NULL)
		{
			BCGCBProGetTopLevelFrame (this)->SetFocus ();
		}
	}

	return TRUE;
}
//*************************************************************************************
BOOL CBCGPCalendarBar::Create(
			CWnd*		pParentWnd,
			DWORD		dwStyle,
			UINT		nID,
			BOOL		enableSelection)
{
	if (enableSelection)
	{
		m_styleFlags |= CBR_MULTISELECTION;
	}
	else
	{
		m_styleFlags &= (~CBR_MULTISELECTION);
	}

	//TODO: calc from max(textheight and menuimageHeight)
	//m_iTitleHeight = GetSystemMetrics(SM_CYCAPTION);
	
	
	return CBCGPPopupMenuBar::Create (pParentWnd, dwStyle, nID);
}
//************************************************************************************
BOOL CBCGPCalendarBar::CreateControl (
				CWnd*			pParentWnd,
				const CRect&	rect,
				UINT			nID,
				BOOL			enableSelection,
				DWORD			dwStyle)
{
	ASSERT_VALID (pParentWnd);

	EnableLargeIcons (FALSE);

	if (!Create (pParentWnd, dwStyle | CBRS_ALIGN_TOP, nID, enableSelection))
	{
		return FALSE;
	}

	//----------------------------
	// Remove borders and gripper:
	//----------------------------
	SetBarStyle (
		GetBarStyle ()
        & ~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	CRect rectWnd = rect;
	MoveWindow (rectWnd);
	
	ContextToSize ();

	SetOwner (pParentWnd);
	SetCommandID (nID);

	m_bIsCtrl = TRUE;

	// All commands will be routed via this dialog, not via the parent frame:
	SetRouteCommandsViaFrame (FALSE);
	return TRUE;
}
//*************************************************************************************
void CBCGPCalendarBar::Serialize (CArchive& ar)
{
	CBCGPPopupMenuBar::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_bIsTearOff;
		ar >> m_nCommandID;

		Rebuild ();
		AdjustLocations ();
	}
	else
	{
		ar << m_bIsTearOff;
		ar << m_nCommandID;
	}
}
//*************************************************************************************
void CBCGPCalendarBar::ShowCommandMessageString (UINT /*uiCmdId*/)
{
	GetOwner()->SendMessage (WM_SETMESSAGESTRING,
		m_nCommandID == (UINT) -1 ? AFX_IDS_IDLEMESSAGE : (WPARAM) m_nCommandID);
}
//*************************************************************************************
void CBCGPCalendarBar::SetDate (const COleDateTime& date)
{
	int iOldYear = m_dateSelected.GetYear();
	int iOldMonth = m_dateSelected.GetMonth();
	m_dateSelected = date;

	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	COleDateTime firstDate = date;
	m_LastAddedDate = date;
	firstDate -= COleDateTimeSpan(firstDate.GetDay () - 1, 0, 0, 0);
	
	int iYear = m_dateSelected.GetYear();
	int iMonth = m_dateSelected.GetMonth();

	CRect capRect;
	GetClientRect (capRect);
	capRect.bottom = capRect.top + m_iTitleHeight;
	InvalidateRect (capRect);

	int dDelta = (firstDate.GetDayOfWeek () - m_nWeekStart + 7) % 7;

	firstDate -= COleDateTimeSpan(dDelta, 0, 0, 0);
	COleDateTime weekDay = firstDate;
	weekDay += COleDateTimeSpan(7, 0, 0, 0);
	
	COleDateTime weekNo = firstDate;
	weekNo += COleDateTimeSpan(6, 0, 0, 0);

	m_iHighlighted = -1;
	int iButton = 0;
	int iNavBtn = 0;
	COleDateTimeSpan oneDay(1,0,0,0);

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL; iButton++)
	{
		CRect rectButton;

		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		if (pButton->m_nStyle & TBBS_SEPARATOR)
		{
			continue;
		}

		COleDateTime dateToSet;
		
		if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarMonthButton)))
		{
			pButton->m_strText = GetMonthName(date.GetMonth ());

#ifndef DISABLE_MONTH_POPUP

			if (!m_bIsTearOff || (m_styleFlags & CBCGPCalendarBar::CBR_NAVIGATION_BUTTONS) == 0)
			{
				pButton->m_nStyle |= TBBS_DISABLED;
			}
			else
			{
				pButton->m_nStyle &= ~TBBS_DISABLED;
			}
#else
			pButton->m_nStyle |= TBBS_DISABLED;
#endif
			InvalidateRect (pButton->Rect());
			continue;
		}
		
		if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarWeekNumButton)))
		{
			dateToSet = weekNo;
			weekNo += COleDateTimeSpan(7, 0, 0, 0);
			if (!(m_styleFlags & CBR_MULTISELECTION) 
				|| !m_bIsTearOff
				|| !(m_styleFlags & CBR_WEEKNUMBER))
				pButton->m_nStyle |= TBBS_DISABLED;
			else
				pButton->m_nStyle &= ~TBBS_DISABLED;

		}
		else if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarTodayButton)))
		{
			dateToSet = COleDateTime::GetCurrentTime();
			pButton->m_nStyle &= ~TBBS_DISABLED;
		}
		else if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarNavigateButton)))
		{
			iNavBtn ++;
			int iToYear = iYear;
			int iToMonth = iMonth;
			switch(iNavBtn)
			{
			case 1:
				{
					iToYear --;
				}
				break;
			case 2:
				{
					iToMonth --;
					if (!iToMonth)
					{
						iToMonth = 12;
						iToYear --;
					}
				}
				break;
			case 3:
				{
					iToMonth ++;
					if (iToMonth > 12)
					{
						iToMonth = 1;
						iToYear ++;
					}
				}
				break;
			case 4:
				{
					iToYear ++;
				}
				break;
			}

			dateToSet = COleDateTime(iToYear, iToMonth, 1, 0, 0, 0);
			pButton->m_nStyle &= ~TBBS_DISABLED;
		}
		else if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarWeekButton)))
		{
			dateToSet = weekDay;
			weekDay += oneDay;

			
			if (!(m_styleFlags & CBR_MULTISELECTION) 
				|| m_pParentBtn != NULL
				|| !(m_styleFlags & CBR_WEEKDAYSEL))
				pButton->m_nStyle |= TBBS_DISABLED;
			else
				pButton->m_nStyle &= ~TBBS_DISABLED;
		}
		else if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPCalendarButton)))
		{
			dateToSet = firstDate;
			firstDate += oneDay;
		}
		else 
		{
			continue;
		}
		
		CBCGPCalendarButton* pCalendarButton = DYNAMIC_DOWNCAST (CBCGPCalendarButton, pButton);
		if (pCalendarButton == NULL)
		{
			continue;
		}

		ASSERT_VALID (pCalendarButton);

		pCalendarButton->SetDate (dateToSet);
		
		if (pCalendarButton->m_bHighlight)
		{
			pCalendarButton->m_bHighlight = FALSE;
			CRect r = pCalendarButton->Rect ();
			r.InflateRect (2, 2);
			InvalidateRect (r);
		}
	}

	if (iOldMonth != m_dateSelected.GetMonth() || 
		iOldYear != m_dateSelected.GetYear())
	{
		OnSelectionChanged ();
	}
}
//***************************************************************************************
void CBCGPCalendarBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!IsCustomizeMode () || m_bInternal)
	{
		CBCGPToolBar::OnMouseMove(nFlags, point);
	}

	if (m_nHoldTimerID && GetCapture() == this)
	{
		int iHit = HitTest (point);
		if (iHit  == -1)
		{
			KillTimer(m_nHoldTimerID);
			m_nHoldTimerID = 0;
		}
	}
}
//***************************************************************************************
void CBCGPCalendarBar::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != nHoldMouseTimerID)
		return;

	if (m_nElapseInterval > 100)
	{
		m_nHoldTimerID = SetTimer (nHoldMouseTimerID, m_nElapseInterval -= 50, NULL);
	}

	CBCGPToolbarButton* pBtnHit = GetButton (m_iButtonCapture);
	CCalendarNavigateButton* pNavi = DYNAMIC_DOWNCAST (CCalendarNavigateButton, pBtnHit);
	if (pNavi)
	{
		SetDate(pNavi ->m_Calendar);
	}
}
//***************************************************************************************
void CBCGPCalendarBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//TODO: chech that hit-point is over navigate btn, start timer if so and each timer tick set new navi date
	// maybe it is necessary to call SetCapture
	if (IsCustomizeMode () && !m_bInternal)
	{
		return;
	}
	int iHit = HitTest (point);

	CBCGPToolBar::OnLButtonDown(nFlags, point);
	SetFocus ();

	CBCGPToolbarButton* pBtnHit = GetButton (iHit);
	CCalendarNavigateButton* pNavi = DYNAMIC_DOWNCAST (CCalendarNavigateButton, pBtnHit);
	if (pNavi)
	{
		m_iButtonCapture = iHit;
		SetCapture ();
		CRect rect;
		GetButtonRect (m_iButtonCapture, rect);
		InvalidateRect(rect);
		m_nHoldTimerID = SetTimer (nHoldMouseTimerID, m_nElapseInterval = 500, NULL);
	}
	else
	{
		CCalendarMonthButton* pmonth = DYNAMIC_DOWNCAST (CCalendarMonthButton, pBtnHit);

		if (pmonth != NULL)
		{
			CRect rect = pmonth->Rect();
			OnSelectMonth(pmonth->Rect());
            OnChangeHot (-1);
			InvalidateRect( rect );
			InvalidateRect( pmonth->Rect() );
    		UpdateWindow(); // immediate feedback
		}
	}
}
//***************************************************************************************
void CBCGPCalendarBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//Reset capturing if navi and finish timer
		
	if (!IsCustomizeMode () || m_bInternal)
	{
		if (GetCapture() == this)
		{
			KillTimer(m_nHoldTimerID);
			m_nHoldTimerID = 0;
		}
		
		HWND hwndSaved = m_hWnd;

		CBCGPToolBar::OnLButtonUp(nFlags, point);
        m_iButtonCapture = -1;

    	if (::IsWindow (hwndSaved) &&				// "This" may be destoyed now!
	    	m_bIsTearOff)	
        {
            int iPB = m_iHighlighted;
            OnChangeHot (-1);
            m_iHighlighted = m_iHot;
            InvalidateButton(iPB);
            if (m_iHighlighted >= 0)
                InvalidateButton(m_iHighlighted);
        }
	}
}
//***************************************************************************************
void CBCGPCalendarBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!IsCustomizeMode () || m_bInternal)
	{
		CBCGPToolBar::OnLButtonDblClk(nFlags, point);
	}
}
//***************************************************************************************
BOOL CBCGPCalendarBar::PreTranslateMessage(MSG* pMsg)
{
	if ((m_pParentBtn != NULL ) && !m_bInCommand)
	{
		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			{
				CRect rect;
				GetClientRect (rect);

				CPoint pt (BCG_GET_X_LPARAM (pMsg->lParam), BCG_GET_Y_LPARAM (pMsg->lParam));
				if (!rect.PtInRect (pt))
				{
					GetParent ()->SendMessage (WM_CLOSE);
					return TRUE;
				}
			}
			break;

		case WM_SYSKEYDOWN:
		case WM_CONTEXTMENU:
			GetParent ()->SendMessage (WM_CLOSE);
			return TRUE;

		case WM_KEYDOWN:
			return OnKey ((UINT) pMsg->wParam);
		}
	}
    else if (m_bIsTearOff)
    {
		switch (pMsg->message)
        {
    		case WM_KEYDOWN:
                return OnKey ((UINT) pMsg->wParam);
        }
    }

	return CBCGPPopupMenuBar::PreTranslateMessage(pMsg);
}
//*************************************************************************************
void CBCGPCalendarBar::OnDestroy() 
{
	if (m_pParentBtn != NULL)
	{
		m_pParentBtn->m_pPopup = NULL;
		m_pParentBtn->SetFocus ();
		m_pParentBtn->OnHideCalendarPopup();
		ReleaseCapture ();
	}

	CBCGPPopupMenuBar::OnDestroy();
}
//****************************************************************************************
BOOL CBCGPCalendarBar::OnKey(UINT nChar)
{
	BOOL  isCtrlPressed =  (0x8000 & GetKeyState(VK_CONTROL)) != 0;
	BOOL  isShiftPressed = (0x8000 & GetKeyState(VK_SHIFT)) != 0;

    switch (nChar)
	{

    case VK_HOME:
        {
            SetDate (COleDateTime::GetCurrentTime());
			SelectDay(m_dateSelected, FALSE, FALSE);
            Invalidate();
            UpdateWindow();
			return TRUE;
        }

    case VK_ESCAPE:
        if (m_pParentBtn != NULL)
	    {
		    GetParent ()->SendMessage (WM_CLOSE);
		    return TRUE;
	    }
		break;
	case VK_RETURN:
        if (m_pParentBtn != NULL)
	        {
		        m_pParentBtn->SetDate (m_LastAddedDate);
		        GetParent ()->SendMessage (WM_CLOSE);
                return TRUE;
	        }
		break;

    case VK_PRIOR:
		if (!isCtrlPressed)
        {
            int mon = m_dateSelected.GetMonth();
            mon --;

            if (mon < 1)
                mon = 12;

            int days = GetMaxMonthDay(COleDateTime(m_dateSelected.GetYear (), mon, 
                                m_dateSelected.GetDay(), 0, 0, 0));

            SetDate (m_LastAddedDate - COleDateTimeSpan(days, 0, 0, 0));
            Invalidate();
            UpdateWindow();
			return TRUE;
        }
        else
        {
            SetDate (COleDateTime(m_dateSelected.GetYear () - 1, m_dateSelected.GetMonth(), 
                                m_dateSelected.GetDay(), 0, 0, 0));
            Invalidate();
            UpdateWindow();
			return TRUE;
        }

	case VK_UP:
		if (!isCtrlPressed)
        {
			SelectDay(m_LastAddedDate + COleDateTimeSpan(-7, 0, 0, 0), FALSE, isShiftPressed);
            SetDate (m_LastAddedDate);
            Invalidate();
            UpdateWindow();
		}
		return TRUE;

    case VK_NEXT:
		if (!isCtrlPressed)
        {
            SetDate (m_LastAddedDate + COleDateTimeSpan(GetMaxMonthDay (m_dateSelected), 0, 0, 0));
            Invalidate();
            UpdateWindow();
			return TRUE;
        }
        else
        {
            SetDate (COleDateTime(m_dateSelected.GetYear () + 1, m_dateSelected.GetMonth(), 
                                m_dateSelected.GetDay(), 0, 0, 0));
            Invalidate();
            UpdateWindow();
			return TRUE;
        }

	case VK_DOWN:
		if (!isCtrlPressed)
		{
			SelectDay(m_LastAddedDate + COleDateTimeSpan(7, 0, 0, 0), FALSE, isShiftPressed);
            SetDate (m_LastAddedDate);
            Invalidate();
            UpdateWindow();
		}
		return TRUE;

	case VK_LEFT:
		if (!isCtrlPressed)
		{
			SelectDay(m_LastAddedDate + COleDateTimeSpan(-1, 0, 0, 0), FALSE, isShiftPressed);
            SetDate (m_LastAddedDate);
            Invalidate();
            UpdateWindow();
			return TRUE;
		}
	case VK_RIGHT:
		if (!isCtrlPressed)
		{
			SelectDay(m_LastAddedDate + COleDateTimeSpan(1, 0, 0, 0), FALSE, isShiftPressed);
            SetDate (m_LastAddedDate);
            Invalidate();
            UpdateWindow();
			return TRUE;
		}
	}

	return CBCGPPopupMenuBar::OnKey (nChar);
}
//****************************************************************************************
void CBCGPCalendarBar::OnSelectMonth(LPCRECT /*lpRect*/)
{
#ifndef DISABLE_MONTH_POPUP

	if ((m_styleFlags & CBCGPCalendarBar::CBR_NAVIGATION_BUTTONS) && m_bIsTearOff)
	{
		CBCGPWorkspace* pWS = GetWorkspace();

		if (!pWS)
			return;

		CMenu menu;

		menu.CreatePopupMenu();
		for(int i = 0; i < 12; i ++)
		{
			menu.AppendMenu(MF_STRING, BCGM_SETMONTHFIRST + i, GetMonthName(i + 1));
		}
		CRect capRect(lpRect);
		ClientToScreen(capRect);
		int x = capRect.left;
		int y = capRect.bottom;

		CBCGPPopupMenu monthPopup;

		CWnd* pParent = GetParent();
		if (pParent)
			pParent = pParent->GetParent();

		if (!pParent)
			pParent = this;

		
		UINT retID = pWS->GetContextMenuManager()->TrackPopupMenu (menu.Detach (), x, y, pParent);

		if (retID)
			OnMonthMenu(retID);	
		else
			InvalidateRect(lpRect);
	}

#endif
}

void CBCGPCalendarBar::OnMonthMenu(UINT /*id*/)
{
#ifndef DISABLE_MONTH_POPUP
	COleDateTime dt = m_dateSelected;
	dt.SetDate (dt.GetYear(), id - BCGM_SETMONTHFIRST + 1, dt.GetDay());
	SetDate(dt);
#endif
}

afx_msg LRESULT CBCGPCalendarBar::OnMouseLeave(WPARAM /*wp*/,LPARAM /*lp*/)
{
	if (m_hookMouseHelp != NULL || 
		(m_bMenuMode && !IsCustomizeMode () && GetDroppedDownMenu () != NULL))
	{
		return 0;
	}

	m_bTracked = FALSE;
	m_ptLastMouse = CPoint (-1, -1);

	if (m_iHighlighted >= 0)
	{
		int iButton = m_iHighlighted;
		m_iHighlighted = -1;

		OnChangeHot (m_iHighlighted);

		InvalidateButton (iButton);
		UpdateWindow(); // immediate feedback

		GetOwner()->SendMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	}

	return 0;
}
//****************************************************************************************
void CBCGPCalendarBar::SetVertMargin (int nVertMargin)
{
	ASSERT_VALID (this);

	m_nVertMargin = nVertMargin;
	AdjustLayout ();
}
//*****************************************************************************************
void CBCGPCalendarBar::SetHorzMargin (int nHorzMargin)
{
	ASSERT_VALID (this);

	m_nHorzMargin = nHorzMargin;
	AdjustLayout ();
}
//************************************************************************************
LPCTSTR CBCGPCalendarBar::GetMonthName(int index)
{
	ASSERT(index > 0 && index <= 12);

	if (m_strMonthNames[0].IsEmpty())
	{
		CBCGPDefaultLocale dl;
		COleDateTime today = COleDateTime::GetCurrentTime();

		for (int i = 0; i < 12; i ++)
		{
			today.SetDate (today.GetYear(), i + 1, 1);
			m_strMonthNames[i] = today.Format (_T("%B"));
		}
	}
	
	if (index > 0 && index <= 12)
	{
		return m_strMonthNames[index - 1];
	}

	return NULL;
}

//****************************************************************************************
void CBCGPCalendarBar::SelectWeekday(const COleDateTime& date, BOOL isCtrlPressed, BOOL isShiftPressed)
{
	if (!(m_styleFlags & CBR_MULTISELECTION))
		return;
	
	int iMonth = m_dateSelected.GetMonth ();
	int iYear = m_dateSelected.GetYear ();
	int iDaysInMonth = GetMaxMonthDay (m_dateSelected);
	int iWeekDay = date.GetDayOfWeek ();
	
	if (!isCtrlPressed && !isShiftPressed)
	{
		ClearSelectedDays();
	}

	for (int iDay = 1; iDay <= iDaysInMonth; iDay ++)
	{
		COleDateTime date (iYear, iMonth, iDay, 0, 0, 0);
		if (date.GetDayOfWeek () == iWeekDay)
		{
			if (!isShiftPressed && IsDaySelected (date))
			{
				RemoveSelectedDay (iDay);
			}
			else
			{
				m_LastAddedDate = date;
				AddSelectedDay (iDay);
			}
		}
	}

	Invalidate ();
	UpdateWindow ();

	OnSelectionChanged ();
}
//****************************************************************************************
void CBCGPCalendarBar::SelectWeek(const COleDateTime& date, BOOL isCtrlPressed, BOOL isShiftPressed)
{
	if (!(m_styleFlags & CBR_MULTISELECTION))
		return;

	int iMonth = m_dateSelected.GetMonth ();
	int iYear = m_dateSelected.GetYear ();
	int iDaysInMonth = GetMaxMonthDay (m_dateSelected);

	int iWeekNo = CBCGPCalendarBar::GetWeekNo(date, m_nWeekStart);

	if (!isCtrlPressed && !isShiftPressed)
	{
		ClearSelectedDays();
	}

	for (int iDay = 1; iDay <= iDaysInMonth; iDay ++)
	{
		COleDateTime date (iYear, iMonth, iDay, 0, 0, 0);

		if (	iWeekNo == CBCGPCalendarBar::GetWeekNo(date, m_nWeekStart))
		{
			if (!isShiftPressed && IsDaySelected (date))
			{
				RemoveSelectedDay (iDay);
			}
			else
			{
				m_LastAddedDate = date;
				AddSelectedDay (iDay);
			}
		}
	}

	Invalidate ();
	UpdateWindow ();

	OnSelectionChanged ();
}
//****************************************************************************************
void CBCGPCalendarBar::SelectDaySequence(const COleDateTime& date)
{
	int iFirst = 0, iLast = 0;
	int d = date.GetDay ();
	for (POSITION pos = m_lstSelectedDays.GetHeadPosition (); pos != NULL; )
	{
		int i = m_lstSelectedDays.GetNext (pos);
		
		if (!i)
			continue;

		if (iFirst == 0)
			iFirst = i;

		if (iLast == 0)
			iLast = i;

		if (iFirst > i)
			iFirst = i;

		if (iLast < i)
			iLast = i;
	}

	if (d < iFirst)
	{
		iFirst = d;
	}
	else 
	{
		iLast = d;
	}

	ASSERT(iFirst <= iLast && iFirst > 0);
	
	ClearSelectedDays();

	for (int i = iFirst; i <= iLast; i ++)
	{
		m_LastAddedDate = date;
		AddSelectedDay(i);
	}
}
//****************************************************************************************
void CBCGPCalendarBar::SelectDay(const COleDateTime& date, BOOL isCtrlPressed, BOOL isShiftPressed)
{
	if (!(m_styleFlags & CBR_MULTISELECTION) || !isCtrlPressed && !isShiftPressed)
	{
		ClearSelectedDays();
		m_LastAddedDate = date;
		AddSelectedDay (date.GetDay());
	}

    if (!m_bIsTearOff)
    {
        m_dateSelected = date;
    }

	if (date.GetMonth () == m_dateSelected.GetMonth())
		//return;
	{
		if (isCtrlPressed)
		{
			if (!IsDaySelected(date))
			{
				m_LastAddedDate = date;
				AddSelectedDay (date.GetDay());
			}
			else
				RemoveSelectedDay(date.GetDay());
		}
		else if (isShiftPressed)
		{
			SelectDaySequence(date);
		}
	}

	OnSelectionChanged();
}
//****************************************************************************************
void CBCGPCalendarBar::TranslateSelectionMode(SelectionMODE newMode)
{
	if (m_selMode == newMode)
		return;

	CList<int,int> dayList;

	int year = m_dateSelected.GetYear ();
	int mon = m_dateSelected.GetMonth ();

	//dayList results in Select_BYDAYS mode
	switch (m_selMode)
	{
	case Select_BYDAYS:
		{
			for (POSITION pos = m_lstSelectedDays.GetHeadPosition (); pos != NULL; )
			{
				int iDay = m_lstSelectedDays.GetNext (pos);
				dayList.AddTail (iDay);
			}
		}
		break;
	case Select_BYWEEKS:
		{
			int maxDays = GetMaxMonthDay(m_dateSelected);
			for (int iDay = 1; iDay <= maxDays; iDay ++)
			{
				int weekNo = (COleDateTime(year, mon, iDay, 0, 0, 0).GetDayOfYear () / 7);
				POSITION pos = m_lstSelectedDays.Find(weekNo);
				if (pos)
					dayList.AddTail (iDay);
			}
		}
		break;
	case Select_BYWEEKDAYS:
		{
			int maxDays = GetMaxMonthDay(m_dateSelected);
			for (int iDay = 1; iDay <= maxDays; iDay ++)
			{
				int weekNo = (COleDateTime(year, mon, iDay, 0, 0, 0).GetDayOfWeek());
				POSITION pos = m_lstSelectedDays.Find(weekNo);
				if (pos)
					dayList.AddTail (iDay);
			}
		}
		break;
	}

	m_lstSelectedDays.RemoveAll();

	for (POSITION pos = dayList.GetHeadPosition (); pos != NULL; )
	{
		int iDay = dayList.GetNext (pos);
		switch (newMode)
		{
		case Select_BYDAYS:
			{
				m_lstSelectedDays.AddTail (iDay);
			}
		break;
		case Select_BYWEEKS:
			{
				int weekNo = (COleDateTime(year, mon, iDay, 0, 0, 0).GetDayOfYear () / 7);
				POSITION p = m_lstSelectedDays.Find (weekNo);
				if (!p)
				{
					m_lstSelectedDays.AddTail (weekNo);
				}
			}
		break;
		case Select_BYWEEKDAYS:
			{
				int weekNo = (COleDateTime(year, mon, iDay, 0, 0, 0).GetDayOfWeek ());
				POSITION p = m_lstSelectedDays.Find (weekNo);
				if (!p)
				{
					m_lstSelectedDays.AddTail (weekNo);
				}
			}
		break;
		}
	}
	
	m_selMode = newMode;
}

//****************************************************************************************
void CBCGPCalendarBar::ClearSelectedDays ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	while (!m_lstSelectedDays.IsEmpty ())
	{
		int iDay = m_lstSelectedDays.RemoveHead ();
		ASSERT (iDay >= 1 && iDay <= GetMaxMonthDay(m_dateSelected));

		CRect rect;
		if (GetButtonRect(iDay, rect))
		{
			rect.InflateRect (2, 2);
			InvalidateRect (rect);
		}
	}

	UpdateWindow ();
}
//****************************************************************************************
void CBCGPCalendarBar::AddSelectedDay (int iDay)
{
	ASSERT (iDay >= 1 && iDay <= 31);// GetMaxMonthDay(m_dateSelected));

	BOOL bInserted = FALSE;

	for (POSITION pos = m_lstSelectedDays.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;
		int iSelDay = m_lstSelectedDays.GetNext (pos);

		if (iSelDay == iDay)	// Already in list,
		{						// do nothing!
			return;
		}

		if (iSelDay > iDay)
		{
			m_lstSelectedDays.InsertBefore (posSave, iDay);
			bInserted = TRUE;
			break;
		}
	}

	if (!bInserted)
	{
		m_lstSelectedDays.AddTail (iDay);
	}

	CRect rect;
	if (GetButtonRect(iDay, rect))
	{
		rect.InflateRect (2, 2);
		InvalidateRect (rect);
	}
}
//****************************************************************************************
void CBCGPCalendarBar::RemoveSelectedDay (int iDay)
{
	ASSERT (iDay >= 1 && iDay <=  GetMaxMonthDay(m_dateSelected));

	for (POSITION pos = m_lstSelectedDays.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;
		if (m_lstSelectedDays.GetNext (pos) == iDay)
		{
			m_lstSelectedDays.RemoveAt (posSave);

			CRect rect;
			if (GetButtonRect(iDay, rect))
			{
				rect.InflateRect(2, 2);
				InvalidateRect (rect);
			}
			return;
		}
	}
}
//****************************************************************************************
BOOL CBCGPCalendarBar::IsDaySelected (const COleDateTime& date) const
{
	int iDay = date.GetDay ();
	if (date.GetMonth() !=  m_dateSelected.GetMonth())
	{
		return FALSE;
	}

	if (!m_bIsTearOff)
	{
		return (date == m_dateSelected);
	}

	for (POSITION pos = m_lstSelectedDays.GetHeadPosition (); pos != NULL;)
	{
		if (m_lstSelectedDays.GetNext (pos) == iDay)
		{
			return TRUE;
		}
	}

	return FALSE;
}
//****************************************************************************************
CBCGPCalendarButton* CBCGPCalendarBar::GetDayButton(int iID) const
{
	BOOL isStarted = FALSE;
	
	CRuntimeClass* dayRT = RUNTIME_CLASS(CBCGPCalendarButton);
	CString strRT = dayRT->m_lpszClassName;

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL; )
	{
		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		if (pButton->IsKindOf (RUNTIME_CLASS (CCalendarWeekNumButton))
			|| (pButton->IsKindOf (RUNTIME_CLASS (CCalendarWeekButton))))
			continue;
		if (pButton->m_nStyle & TBBS_SEPARATOR)
			continue;

		CRuntimeClass* pRunTime = pButton->GetRuntimeClass ();
		if (pRunTime && strRT == pRunTime->m_lpszClassName)
		{
			CBCGPCalendarButton* pCalendarButton = DYNAMIC_DOWNCAST (CBCGPCalendarButton, pButton);	
			int day = pCalendarButton->m_Calendar.GetDay();
			if (day == 1)
			{
				if (!isStarted)
					isStarted = TRUE;
				else break;
			}

			if (isStarted && day == iID)
			{
				return pCalendarButton;
			}
		}
	}

	return NULL;
}
//****************************************************************************************
BOOL CBCGPCalendarBar::GetButtonRect (int iID, CRect& rect, BOOL /*isHitTest*/) const
{
	CBCGPCalendarButton* pCalendarButton = GetDayButton(iID);
	if (!pCalendarButton)
		return FALSE;
	rect = pCalendarButton->m_rect;
	return TRUE;
}
//****************************************************************************************
int CBCGPCalendarBar::GetWeekNo(const COleDateTime& date, int weekStart)
{
	COleDateTime Jan1 (date.GetYear(), 1, 1, 0, 0, 0);
	int Jan1wd = Jan1.GetDayOfWeek ();
	Jan1wd = (Jan1wd + 7 - weekStart) % 7 + 1;
	int datewd = date.GetDayOfWeek ();
	datewd = (datewd + 7 - weekStart) % 7 + 1;
	int dd = datewd - Jan1wd;
		//- weekStart + 1);

	int dateyd = date.GetDayOfYear () - dd;
	return dateyd / 7 + 1;
}

//****************************************************************************************
int CBCGPCalendarBar::GetMaxMonthDay(COleDateTime date)
{ 
	//for(date)	
	int y = date.GetYear ();
	int m = date.GetMonth();
	for(int i = 31; i > 0; i --)
	{
		date.SetDate (y, m, i);
		if (date.GetStatus() == COleDateTime::valid)
			return i;
	}	
	return 0;
}
//***************************************************************************************
void CBCGPCalendarBar::OnSelectionChanged ()
{
	if (!m_bIsTearOff)
	{
		return;
	}

	CWnd* pOwner = GetOwner ();
	ASSERT_VALID (pOwner);

	pOwner->SendMessage (BCGM_CALENDAR_ON_SELCHANGED,
		0, (LPARAM) GetSafeHwnd ());
}
//***************************************************************************************
void CBCGPCalendarBar::SetFirstDayOfWeek (int nDay)
{
	ASSERT_VALID (this);
	ASSERT (nDay >= 0 && nDay < 7);

	m_nWeekStart = nDay + 1;
	SetDate (m_dateSelected);
}
//*************************************************************************************
int CBCGPCalendarBar::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return CBCGPBaseToolBar::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
//*************************************************************************************
void CBCGPCalendarBar::SelectDays (const CList<int,int>& lstDays,
									 BOOL bRedraw)
{
	if (!(m_styleFlags & CBR_MULTISELECTION))
	{
		ASSERT (FALSE);
		return;
	}

	m_lstSelectedDays.RemoveAll ();
	m_lstSelectedDays.AddTail ((CList<int, int>*) &lstDays);

	if (GetSafeHwnd () != NULL && bRedraw)
	{
		RedrawWindow ();
	}
}
//*************************************************************************************
void CBCGPCalendarBar::MarkDays (const CList<int,int>& lstDays,
								 BOOL bRedraw)
{
	m_lstMarkedDays.RemoveAll ();
	m_lstMarkedDays.AddTail ((CList<int, int>*) &lstDays);

	if (GetSafeHwnd () != NULL && bRedraw)
	{
		RedrawWindow ();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPCalendarButton

IMPLEMENT_SERIAL(CBCGPCalendarButton, CBCGPToolbarButton, 1)

CBCGPCalendarButton::CBCGPCalendarButton(const COleDateTime& date, BOOL bHighlight)
{
	m_Calendar = date;
	m_bHighlight = bHighlight;
	m_strText.Format(_T("%d"), date.GetDay ());
	m_pParentBar = NULL;
}

CBCGPCalendarButton::CBCGPCalendarButton()
{
    m_Calendar = COleDateTime::GetCurrentTime();
	m_bHighlight = FALSE;
	m_strText.Format(_T("%d"), m_Calendar.GetDay ());
	m_pParentBar = NULL;
}

BOOL CBCGPCalendarButton::OnToolHitTest(const CWnd* /*pWnd*/, TOOLINFO* /*pTI*/)
{
	return FALSE;
}

void CBCGPCalendarButton::OnChangeParentWnd (CWnd* pWndParent)
{
	CBCGPToolbarButton::OnChangeParentWnd (pWndParent);
	m_pParentBar = DYNAMIC_DOWNCAST (CBCGPCalendarBar, pWndParent);
}

void CBCGPCalendarButton::SetDate (const COleDateTime& date)
{
	if (date != m_Calendar)
	{
		m_Calendar = date;
		m_strText.Format(_T("%d"), date.GetDay ());
		CRect invRect = m_rect;
		invRect.InflateRect (2, 2);
		m_pParentBar->InvalidateRect(m_rect);
	}
}

void CBCGPCalendarButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
								BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,
								BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParentBar);

	BOOL isSelected = IsSelected();
	UINT nStyle = m_nStyle;

	if (m_bHighlight)
	{
		m_nStyle |= TBBS_CHECKED;
	}

	if (isSelected)
		m_nStyle |= TBBS_CHECKED;
	else
		m_nStyle &= ~TBBS_CHECKED;
	
	CBCGPToolbarButton::OnDraw (pDC, rect, pImages,
			bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

	//------------------
	// Draw today frame:
	//------------------
	COleDateTime today = COleDateTime::GetCurrentTime();
	if (IsActualDay () && today.GetMonth () == m_Calendar.GetMonth () &&
			today.GetDay () == m_Calendar.GetDay () &&
			today.GetYear () == m_Calendar.GetYear ())
	{
		CRect rectToday = rect;
		rectToday.DeflateRect (afxData.cxBorder2 - 1, afxData.cyBorder2 -  1);
		pDC->Draw3dRect (rectToday, globalData.clrBarShadow,
									globalData.clrBarShadow);
		rectToday.bottom = rectToday.top + 2;
		rectToday.top -= 1;
		pDC->FillSolidRect (rectToday, globalData.clrBarShadow);
	}

	//------------
	// Draw label:
	//------------
	if (m_iImage < 0)
	{
		CFont* pOldFont = NULL;

		if (IsCurrMonth () && IsActualDay () &&
			m_pParentBar->m_lstMarkedDays.Find (m_Calendar.GetDay()) != NULL)
		{
			pOldFont = pDC->SelectObject (
				m_pParentBar->IsDialogControl () ? &globalData.fontDefaultGUIBold : &globalData.fontBold);
			ASSERT (pOldFont != NULL);
		}

		CBCGPVisualManager::BCGBUTTON_STATE state = CBCGPVisualManager::ButtonsIsRegular;

		if (bHighlight)
		{
			state = CBCGPVisualManager::ButtonsIsHighlighted;
		}
		else if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
		{
			//-----------------------
			// Pressed in or checked:
			//-----------------------
			state = CBCGPVisualManager::ButtonsIsPressed;
		}

		COLORREF clrText = CBCGPVisualManager::GetInstance ()->GetToolbarButtonTextColor (
			this, state);

		pDC->SetTextColor (clrText);

		CRect rectText = rect;
		pDC->DrawText (m_strText, &rectText, 
			DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS);

		if (pOldFont != NULL)
		{
			pDC->SelectObject (pOldFont);
		}
	}

	m_nStyle = nStyle;
}
//*************************************************************************************
BOOL CBCGPCalendarButton::IsCurrMonth () const
{
	ASSERT_VALID (m_pParentBar);
	return (m_pParentBar->m_dateSelected.GetMonth () == m_Calendar.GetMonth ());
}
//********************************************************************************
void CBCGPCalendarBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPPopupMenuBar::OnSize(nType, cx, cy);
	
	ContextToSize ();
	RedrawWindow ();
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPDefaultLocale

CBCGPDefaultLocale::CBCGPDefaultLocale ()
{
	m_strLanguage = CString (_tsetlocale (LC_TIME, NULL));
	
	int nLen = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE , NULL, 0);
	
	TCHAR* pzBuffer = new TCHAR [nLen + 1];
	pzBuffer[nLen] = NULL;

	nLen = GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, pzBuffer, nLen);
	CString strLocale = CString (pzBuffer) + _T("_");
	delete [] pzBuffer;
	
	nLen = GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, NULL, 0);
	
	pzBuffer = new TCHAR [nLen + 1];
	pzBuffer [nLen] = NULL;

	nLen = GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, pzBuffer, nLen);
	strLocale += pzBuffer;
	delete [] pzBuffer;
	
	_tsetlocale (LC_TIME, strLocale);
}
//**************************************************************************************
CBCGPDefaultLocale::~CBCGPDefaultLocale ()
{
	_tsetlocale(LC_TIME, m_strLanguage);
}
