// BCGPDurationCtrl.cpp: implementation of the CBCGPDurationCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgcbpro.h"
#include "bcgglobals.h"
#include "BCGPDurationCtrl.h"

#include "BCGPVisualManager.h"
#include "BCGPToolbarComboBoxButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static const int iSpinWidth = 15;
static const int iSpinID = 1;

const UINT CBCGPDurationCtrl::DRTN_DAYS =  1u;
const UINT CBCGPDurationCtrl::DRTN_HOURS_MINS = 2u;
const UINT CBCGPDurationCtrl::DRTN_SPIN =  8u;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPDurationCtrl::CBCGPDurationCtrl()
{
	m_spinButton = TRUE;
	m_lDays = 0;
	m_lHours = 0;
	m_lMinutes = 0;

	m_strDaysLabel = _T("Day(s) ");
	m_strHoursLabel = _T(":");
	m_strMinutesLabel = _T("");

	m_bShowDays = TRUE;
	m_bShowHoursMinutes = TRUE;

	m_iSelectedPart = UNDEFINED_PART;

	m_iPrevDigit = -1;
}

CBCGPDurationCtrl::~CBCGPDurationCtrl()
{

}

BEGIN_MESSAGE_MAP(CBCGPDurationCtrl, CButton)
	//{{AFX_MSG_MAP(CBCGPDurationCtrl)
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


UINT CBCGPDurationCtrl::OnGetDlgCode() 
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}
//**************************************************************************************
void CBCGPDurationCtrl::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CClientDC dc (this);

	CFont* pPrevFont = (CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT (pPrevFont != NULL);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	m_iControlHeight = tm.tmHeight + 6;

	dc.SelectObject (pPrevFont);

	int x = 1;
	for (int i = 0; i < PARTS_NUM; i ++)
	{
		CSize size (0, 0);
		CString strText;

		switch (i)
		{
		case DAYS:
			if (m_bShowDays)
			{
				strText = _T("000");
			}
			break;

		case DAYS_LABEL:
			if (m_bShowDays)
			{
				strText = m_strDaysLabel;
			}
			break;

		case HOURS:
		case MINUTES:
			if (m_bShowHoursMinutes)
			{
				strText = _T("00");
			}
			break;

		case HOURS_LABEL:
			if (m_bShowHoursMinutes)
			{
				strText = m_strHoursLabel;
			}
			break;

		case MINUTES_LABEL:
			if (m_bShowHoursMinutes)
			{
				strText = m_strMinutesLabel;
			}
			break;

		default:
			ASSERT (FALSE);
		}

		GetPartSize (strText, size);

		m_rectParts [i] = CRect (x, 1,
								x + size.cx, m_iControlHeight - 3);
		if (size.cx > 0)
		{
			x += size.cx + 2;
		}
	}

	m_iControlWidth = m_rectParts [MINUTES_LABEL].right + iSpinWidth + 2;

	SetWindowPos (NULL, -1, -1, m_iControlWidth, m_iControlHeight,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	CRect rectClient;
	GetClientRect (&rectClient);

	// Adjust spin button:
	if (m_spinButton)
	{
		if (m_wndSpin.GetSafeHwnd () == NULL)
		{
			CRect rectSpin (0, 0, 0, 0);
			m_wndSpin.Create (WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_AUTOBUDDY,
								rectSpin, this, iSpinID);
		}

		m_wndSpin.SetWindowPos (NULL, 
				rectClient.right - iSpinWidth, 0,
				iSpinWidth, rectClient.Height (),
				SWP_NOZORDER | SWP_NOACTIVATE);
		m_wndSpin.ShowWindow (SW_SHOW);

		m_wndSpin.EnableWindow (IsWindowEnabled ());
	}
	else
	{
		if (m_wndSpin.GetSafeHwnd () != NULL)
		{
			m_wndSpin.SetWindowPos (NULL, 
					0, 0,
					0, 0,
					SWP_NOZORDER | SWP_NOACTIVATE);
			m_wndSpin.ShowWindow (SW_HIDE);
		}
	}
}
//***************************************************************************************
void CBCGPDurationCtrl::GetPartSize (const CString& strText, CSize& size)
{
	if (strText.IsEmpty ())
	{
		size = CSize (0, 0);
		return;
	}

	CClientDC dc (this);

	CRect rectClient;
	GetClientRect (&rectClient);

	CFont* pPrevFont = (CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT (pPrevFont != NULL);

	dc.DrawText (strText, rectClient, DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);

	dc.SelectObject (pPrevFont);
	size = rectClient.Size ();
}
//**************************************************************************************
void CBCGPDurationCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDown(nFlags, point);

	SetFocus ();

	int iNewSel = UNDEFINED_PART;

	for (int i = 0; i < PARTS_NUM; i ++)
	{
		if (m_rectParts [i].PtInRect (point))
		{
			iNewSel = i;
			break;
		}
	}

	SelectPart (iNewSel);

	if (m_wndSpin.GetSafeHwnd () != NULL)
	{
		m_wndSpin.RedrawWindow ();
	}
}
//**************************************************************************************
void CBCGPDurationCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (isdigit (nChar))
	{
		PushDigit (nChar - '0');
		return;
	}

	if (nChar >= VK_NUMPAD0 && nChar <= VK_NUMPAD9)
	{
		PushDigit (nChar - VK_NUMPAD0);
		return;
	}

	m_iPrevDigit = -1;

	switch (nChar)
	{
	case VK_DOWN:
		ScrollCurrPart (-1);
		break;

	case VK_UP:
		ScrollCurrPart (1);
		break;

	case VK_RIGHT:
		SelectNext ();
		break;

	case VK_LEFT:
		SelectPrev ();
		break;

	case VK_HOME:
		SelectPart (m_bShowDays ? DAYS : HOURS);
		break;

	case VK_END:
		if (m_bShowHoursMinutes)
		{
			SelectPart (MINUTES);
		}
		break;
	}
	
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
//*****************************************************************************************
void CBCGPDurationCtrl::SelectNext ()
{
	int iNewSel = m_iSelectedPart;

	switch (m_iSelectedPart)
	{
	case DAYS:
		if (m_bShowHoursMinutes)
		{
			iNewSel = HOURS;
		}
		break;

	case HOURS:
		iNewSel = MINUTES;
		break;

	case MINUTES:
		iNewSel = m_bShowDays ? DAYS : HOURS;
		break;
	}

	SelectPart (iNewSel);
}
//*****************************************************************************************
void CBCGPDurationCtrl::SelectPrev ()
{
	int iNewSel = m_iSelectedPart;

	switch (m_iSelectedPart)
	{
	case DAYS:
		if (m_bShowHoursMinutes)
		{
			iNewSel = MINUTES;
		}
		break;

	case HOURS:
		iNewSel = m_bShowDays ? DAYS : MINUTES;
		break;

	case MINUTES:
		iNewSel = HOURS;
		break;
	}

	SelectPart (iNewSel);
}
//**************************************************************************************
void CBCGPDurationCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	if (m_iSelectedPart == UNDEFINED_PART)
	{
		SelectPart (m_bShowDays ? DAYS : HOURS);
	}
	else
	{
		if (m_iSelectedPart != UNDEFINED_PART)
		{
			RedrawWindow (m_rectParts [m_iSelectedPart]);
		}
	}
}
//**************************************************************************************
void CBCGPDurationCtrl::SelectPart (int iNewSel)
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	int iOldSel = m_iSelectedPart;

	switch (iNewSel)
	{
	case DAYS:
	case HOURS:
	case MINUTES:
		m_iSelectedPart = iNewSel;
		break;

	case DAYS_LABEL:
		m_iSelectedPart = DAYS;
		break;

	case HOURS_LABEL:
		m_iSelectedPart = HOURS;
		break;

	case MINUTES_LABEL:
		m_iSelectedPart = MINUTES;
		break;
	}

	if (m_iSelectedPart == iOldSel)
	{
		return;
	}

	if (iOldSel != UNDEFINED_PART)
	{
		RedrawWindow (m_rectParts [iOldSel]);
	}

	if (m_iSelectedPart != UNDEFINED_PART)
	{
		RedrawWindow (m_rectParts [m_iSelectedPart]);
	}
}
//***************************************************************************************
void CBCGPDurationCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
	if (m_iSelectedPart != UNDEFINED_PART)
	{
		RedrawWindow (m_rectParts [m_iSelectedPart]);
	}
}
//*****************************************************************************************
void CBCGPDurationCtrl::PushDigit (int iDigit)
{
	int iNumber;
	if (m_iPrevDigit == -1)
	{
		iNumber = iDigit;
	}
	else
	{
		iNumber = m_iPrevDigit * 10 + iDigit;
	}

	switch (m_iSelectedPart)
	{
	case DAYS:
		if (m_iPrevDigit == -1)
		{
			m_lDays = iDigit;
		}
		else
		{
			m_lDays = m_lDays * 10 + iDigit;
		}
		break;

	case HOURS:
		if (iNumber >= 24)
		{
			return;
		}

		m_lHours = iNumber;
		break;

	case MINUTES:
		if (iNumber >= 60)
		{
			return;
		}

		m_lMinutes = iNumber;
		break;

	default:
		return;
	}

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow (m_rectParts [m_iSelectedPart]);
	}

	if (m_iPrevDigit == -1)	// First push
	{
		m_iPrevDigit = iDigit;
	}
	else
	{
		if (m_iSelectedPart != DAYS || m_lDays >= 100)
		{
			m_iPrevDigit = -1;
		}

		if (m_iSelectedPart != MINUTES &&
			(m_iSelectedPart != DAYS || m_lDays >= 100))
		{
			SelectNext ();
		}
	}

	OnDurationChanged ();
}
//***************************************************************************************
COleDateTimeSpan CBCGPDurationCtrl::GetDuration() const
{
	return COleDateTimeSpan(m_lDays, m_lHours, m_lMinutes, 0);
}
//****************************************************************************************
void CBCGPDurationCtrl::SetDuration(const COleDateTimeSpan& timeSpan)
{
	m_lDays = timeSpan.GetDays ();
	m_lHours = timeSpan.GetHours ();
	m_lMinutes = timeSpan.GetMinutes ();

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//***************************************************************************************
void CBCGPDurationCtrl::ScrollCurrPart (int iDelta)
{
	switch (m_iSelectedPart)
	{
	case DAYS:
		if (m_lDays + iDelta < 0 || m_lDays + iDelta >= 1000)
		{
			return;
		}

		m_lDays += iDelta;
		break;

	case HOURS:
		m_lHours += iDelta;
		if (m_lHours < 0)
		{
			m_lHours = 23;
		}
		else if (m_lHours > 23)
		{
			m_lHours = 0;
		}
		break;

	case MINUTES:
		m_lMinutes += iDelta;
		if (m_lMinutes < 0)
		{
			m_lMinutes = 59;
		}
		else if (m_lMinutes > 59)
		{
			m_lMinutes = 0;
		}
		break;

	default:
		return;
	}

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow (m_rectParts [m_iSelectedPart]);
	}

	OnDurationChanged ();
}
//**************************************************************************************
BOOL CBCGPDurationCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == iSpinID)
	{
		NM_UPDOWN* pNM = (NM_UPDOWN*) lParam;
		ASSERT (pNM != NULL);

		if (pNM->hdr.code == UDN_DELTAPOS)
		{
			ScrollCurrPart (pNM->iDelta < 0 ? 1 : -1);
		}

		SetFocus ();
	}
	
	return CButton::OnNotify(wParam, lParam, pResult);
}
//***************************************************************************************
void CBCGPDurationCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonUp(nFlags, point);

	if (m_wndSpin.GetSafeHwnd () != NULL)
	{
		m_wndSpin.RedrawWindow ();
	}
}
//***************************************************************************************
BOOL CBCGPDurationCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	cs.style &= ~BS_DEFPUSHBUTTON;

	return CButton::PreCreateWindow(cs);
}

void CBCGPDurationCtrl::PreSubclassWindow() 
{
	ModifyStyle (BS_DEFPUSHBUTTON, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CBCGPDurationCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	lpncsp->rgrc[0].left++; 
	lpncsp->rgrc[0].top++ ;
	lpncsp->rgrc[0].right--;
	lpncsp->rgrc[0].bottom--;

	CButton::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CBCGPDurationCtrl::OnNcPaint() 
{
	CBCGPVisualManager::GetInstance ()->OnDrawControlBorder (this);
}

BOOL CBCGPDurationCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CBCGPDurationCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT (lpDIS != NULL);
	ASSERT (lpDIS->CtlType == ODT_BUTTON);

	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rectClient = lpDIS->rcItem;

	pDC->FillSolidRect (&rectClient, globalData.clrWindow);

	pDC->SetBkMode (TRANSPARENT);
	pDC->SelectStockObject (DEFAULT_GUI_FONT);

	for (int i = 0; i < PARTS_NUM; i ++)
	{
		if (m_rectParts [i].Width () == 0)
		{
			continue;
		}

		CString strText;

		switch (i)
		{
		case DAYS:
			strText.Format (_T("%d"), m_lDays);
			break;

		case DAYS_LABEL:
			strText = m_strDaysLabel;
			break;

		case HOURS:
			strText.Format (_T("%02d"), m_lHours);
			break;

		case MINUTES:
			strText.Format (_T("%02d"), m_lMinutes);
			break;

		case HOURS_LABEL:
			strText = m_strHoursLabel;
			break;

		case MINUTES_LABEL:
			strText = m_strMinutesLabel;
			break;

		default:
			ASSERT (FALSE);
		}

		CRect rect = m_rectParts [i];

		if (IsWindowEnabled () && GetFocus () == this && i == m_iSelectedPart)	// Selected part
		{
			pDC->FillSolidRect (rect, globalData.clrHilite);
			pDC->SetTextColor (globalData.clrTextHilite);
		}
		else
		{
			pDC->SetTextColor (	IsWindowEnabled () ?
								globalData.clrWindowText :
								globalData.clrGrayedText);
		}

		pDC->DrawText (strText, rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	}
	if (::IsWindow (m_wndSpin.GetSafeHwnd ()))
	{
		m_wndSpin.RedrawWindow ();
	}
}
//*************************************************************************************
void CBCGPDurationCtrl::OnDurationChanged ()
{
	CWnd* pParent = GetParent ();
	if (pParent != NULL)
	{
		pParent->SendMessage (	WM_COMMAND,
								MAKEWPARAM (GetDlgCtrlID (), BN_CLICKED),
								(LPARAM) m_hWnd);
	}
}
//*****************************************************************************************
UINT CBCGPDurationCtrl::GetState () const
{
	UINT flags = 0;

	if (m_bShowDays)
	{
		flags |= DRTN_DAYS;
	}

	if (m_bShowHoursMinutes)
	{
		flags |= DRTN_HOURS_MINS;
	}

	if (m_spinButton)
	{
		flags |= DRTN_SPIN;
	}

	return flags;
}
//*****************************************************************************************
void CBCGPDurationCtrl::SetState (UINT flags, UINT mask)
{
	if(! ( flags & (~DRTN_SPIN) ) )
	{
		flags |= DRTN_DAYS | DRTN_HOURS_MINS;
		mask |= DRTN_DAYS | DRTN_HOURS_MINS;
	}

	m_bShowDays = (flags & DRTN_DAYS);
	m_bShowHoursMinutes = (flags & DRTN_HOURS_MINS);
	m_spinButton = (flags & DRTN_SPIN);

    m_iSelectedPart = UNDEFINED_PART;

	AdjustLayout ();
}
