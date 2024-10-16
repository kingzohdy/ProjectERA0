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
// BCGPGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "multimon.h"
#include "trackmouse.h"
#include "BCGPReportCtrl.h"
#include "BCGPGridCtrl.h"

#ifndef BCGP_EXCLUDE_GRID_CTRL

#include "BCGPLocalResource.h"
#include "BCGPGlobalUtils.h"

#ifndef _BCGPGRID_STANDALONE
#ifndef _BCGSUITE_
	#include "BCGPColorBar.h"
	#include "BCGPWorkspace.h"
	#include "BCGPToolbarComboBoxButton.h"
	#include "BCGPRegistry.h"
	#include "BCGPPopupMenu.h"
	#include "BCGPToolBar.h"
	#include "BCGPMaskEdit.h"
	#include "RegPath.h"
#endif
	#include "BCGPVisualManager.h"
	#include "MenuImages.h"
	#include "bcgprores.h"
#else
	#include "BCGPGridVisualManager.h"
	#include "resource.h"
#endif

#include "BCGPDrawManager.h"
#include "BCGPSpinButtonCtrl.h"
#include "BCGPGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CBCGPGridCtrl::m_bEnableAssertValidInDebug = FALSE;

#ifdef _DEBUG
#undef ASSERT_VALID
#define ASSERT_VALID(pOb) !CBCGPGridCtrl::m_bEnableAssertValidInDebug ? ((void)0) : (::AfxAssertValidObject(pOb, THIS_FILE, __LINE__))
#endif

#define ID_HEADER		1
#define ID_SCROLL_VERT	2
#define ID_SCROLL_HORZ	3
#define STRETCH_DELTA	2

#define TEXT_MARGIN		3
#define TEXT_VMARGIN	2

#define	BCGPGRIDCTRL_ID_INPLACE	1

#ifndef _BCGPGRID_STANDALONE
#ifndef _BCGSUITE_
	extern CBCGPWorkspace*		g_pWorkspace;
	#define visualManagerMFC	CBCGPVisualManager::GetInstance ()
#else
	#define visualManagerMFC	CMFCVisualManager::GetInstance ()
#endif

	#define visualManager		CBCGPVisualManager::GetInstance ()
#else
	#define visualManager		CBCGPGridVisualManager::GetInstance ()
	#define visualManagerMFC	CBCGPGridVisualManager::GetInstance ()
#endif

static const CString strGridsProfile = _T ("BCGPGrids");

#define REG_SECTION_FMT	_T("%sBCGPGrid-%d")

/////////////////////////////////////////////////////////////////////////////
// Grid sort helper class

typedef int (__cdecl *GENERICCOMPAREFN)(const void * elem1, const void * elem2);
typedef int (__cdecl *STRINGCOMPAREFN)(const CBCGPGridRow** pElem1, const CBCGPGridRow** pElem2);

typedef CArray<CBCGPGridRow*, CBCGPGridRow*> CBCGPGridRowArray;

class CBCGPGridCtrl;

class CBCGPSortableArray : public CBCGPGridRowArray
{
public:
	CBCGPSortableArray()
	{
	}

	virtual ~CBCGPSortableArray()
	{
	}

public:
	void Sort(STRINGCOMPAREFN pfnCompare = Compare)
	{
		CBCGPGridRow** prgstr = GetData();
		qsort(prgstr, GetSize(), sizeof(CBCGPGridRow*),(GENERICCOMPAREFN)pfnCompare);
	}

protected:
	static int __cdecl Compare(const CBCGPGridRow** pElem1, const CBCGPGridRow** pElem2)
	{
		ASSERT(pElem1);
		ASSERT(pElem2);

		CBCGPGridRow* pRow1 = * ( CBCGPGridRow** ) pElem1;
		CBCGPGridRow* pRow2 = * ( CBCGPGridRow** ) pElem2;
		ASSERT_VALID (pRow1);
		ASSERT_VALID (pRow2);

		CBCGPGridCtrl* pGrid = pRow1->GetOwnerList ();
		ASSERT_VALID (pGrid);
		if (!pGrid)
		{
			TRACE0 ("\nCBCGPSortableArray::Compare: Owner grid - Null");
			return 0;
		}

		return pGrid->DoMultiColumnCompare (pRow1, pRow2);
	}
};

/////////////////////////////////////////////////////////////////////////////
// Grid custom colors

BOOL BCGP_GRID_COLOR_DATA::ColorData::Draw (CDC* pDC, CRect rect, BOOL bNoBorder)
{
	if (m_clrBackground == (COLORREF)-1)
	{
		return FALSE;
	}

	if (m_clrGradient == (COLORREF)-1)
	{
		CBrush br (m_clrBackground);
		pDC->FillRect (rect, &br);
	}
	else
	{
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient2 (rect, 
			m_clrBackground, 
			m_clrGradient,
			m_nGradientAngle);
	}

	if (m_clrBorder != (COLORREF)-1 && !bNoBorder)
	{
		pDC->Draw3dRect (rect, m_clrBorder, m_clrBorder);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColumnListBox window

class CBCGPGridColumnListBox : public CListBox
{
// Construction
public:
	CBCGPGridColumnListBox(CBCGPGridColumnsInfo& columns);

// Attributes
public:
	CBCGPGridColumnsInfo& m_Columns;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPGridColumnListBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPGridColumnListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPGridColumnListBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColumnChooser frame

class CBCGPGridColumnChooser : public CMiniFrameWnd
{
	DECLARE_DYNAMIC(CBCGPGridColumnChooser)

public:
	CBCGPGridColumnChooser(CBCGPGridColumnsInfo& columns);

// Attributes
public:
	CBCGPGridColumnListBox	m_wndList;
	CBCGPGridCtrl*			m_pOwnerGrid;

protected:
	BOOL					m_bIsEmpty;

// Operations
public:
	void UpdateList ();
	int GetColumnWidth () const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPGridColumnChooser)
	public:
	virtual BOOL Create(CBCGPGridCtrl* pOwnerGrid, const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBCGPGridColumnChooser();

	// Generated message map functions
	//{{AFX_MSG(CBCGPGridColumnChooser)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridItem

IMPLEMENT_DYNCREATE(CBCGPGridItem, CObject)

CBCGPGridItem::CBCGPGridItem ():
	m_varValue (),
	m_strLabel (_T("")),
	m_strEditMask (_T("")),
	m_strEditTempl (_T("")),
	m_strValidChars (_T("")),
	m_dwData (0)
{
	Init ();
	SetFlags ();
}
//****************************************************************************************
CBCGPGridItem::CBCGPGridItem(const _variant_t& varValue, DWORD_PTR dwData,
		LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate,
		LPCTSTR lpszValidChars) :
	m_varValue (varValue),
	m_strEditMask (lpszEditMask == NULL ? _T("") : lpszEditMask),
	m_strEditTempl (lpszEditTemplate == NULL ? _T("") : lpszEditTemplate),
	m_strValidChars (lpszValidChars == NULL ? _T("") : lpszValidChars),
	m_dwData (dwData)
{
	Init ();
	SetFlags ();

	m_bIsChanged = TRUE;

	if (m_varValue.vt == VT_BOOL)
	{
		m_bAllowEdit = FALSE;
	}
}
//****************************************************************************************
void CBCGPGridItem::SetFlags ()
{
	m_dwFlags = 0;

	switch (m_varValue.vt)
	{
	case VT_BSTR:
	case VT_R4:
	case VT_R8:
	case VT_UI1:
	case VT_I2:
	case VT_I4:
	case VT_INT:
	case VT_UINT:
	case VT_UI2:
	case VT_UI4:
		break;

	case VT_DATE:
		break;

    case VT_BOOL:
		m_dwFlags = BCGP_GRID_ITEM_HAS_LIST;
		break;

	default:
		break;
	}
}
//******************************************************************************************
void CBCGPGridItem::Init ()
{
	m_pGridRow = NULL;
	m_nIdColumn = -1;

	m_bEnabled = TRUE;
	m_bAllowEdit = TRUE;
	m_bSelected = FALSE;

	m_pWndInPlace = NULL;
	m_pWndCombo = NULL;
	m_pWndSpin = NULL;
	m_bInPlaceEdit = FALSE;
	m_bButtonIsDown = FALSE;
	m_bValueIsTrancated = FALSE;

	m_Rect.SetRectEmpty ();
	m_rectButton.SetRectEmpty ();

	m_nMinValue = 0;
	m_nMaxValue = 0;

	m_clrBackground = (COLORREF)-1;
	m_clrText = (COLORREF)-1;

	m_iImage = -1;

	m_pMerged = NULL;

	m_bIsChanged = FALSE;
}
//******************************************************************************************
CBCGPGridItem::~CBCGPGridItem()
{
	if (m_pMerged != NULL)
	{
		m_pMerged->Release ();
	}

	OnDestroyWindow ();
}
//******************************************************************************************
void CBCGPGridItem::OnDestroyWindow ()
{
	if (m_pWndCombo != NULL)
	{
		m_pWndCombo->DestroyWindow ();
		delete m_pWndCombo;
		m_pWndCombo = NULL;
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->DestroyWindow ();
		delete m_pWndInPlace;
		m_pWndInPlace = NULL;
	}

	if (m_pWndSpin != NULL)
	{
		m_pWndSpin->DestroyWindow ();
		delete m_pWndSpin;
		m_pWndSpin = NULL;
	}

	if (m_varValue.vt == VT_BOOL)
	{
		m_lstOptions.RemoveAll ();
	}
}
//****************************************************************************************
BOOL CBCGPGridItem::HasButton () const
{
	return	(m_dwFlags & BCGP_GRID_ITEM_HAS_LIST) ||
			(m_dwFlags & BCGP_GRID_ITEM_HAS_BUTTON);
}
//*******************************************************************************************
BOOL CBCGPGridItem::AddOption (LPCTSTR lpszOption, BOOL bInsertUnique/* = TRUE*/)
{
	ASSERT_VALID (this);
	ASSERT (lpszOption != NULL);

	if (bInsertUnique)
	{
 		if (m_lstOptions.Find (lpszOption) != NULL)
		{
			return FALSE;
		}
	}

	m_lstOptions.AddTail (lpszOption);
	m_dwFlags = BCGP_GRID_ITEM_HAS_LIST;

	return TRUE;
}
//****************************************************************************************
void CBCGPGridItem::RemoveAllOptions ()
{
	ASSERT_VALID (this);

	m_lstOptions.RemoveAll ();
	m_dwFlags = 0;
}
//****************************************************************************************
int CBCGPGridItem::GetOptionCount () const
{
	ASSERT_VALID (this);
	return (int) m_lstOptions.GetCount ();
}
//****************************************************************************************
LPCTSTR CBCGPGridItem::GetOption (int nIndex) const
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= m_lstOptions.GetCount ())
	{
		ASSERT (FALSE);
		return NULL;
	}

	POSITION pos = m_lstOptions.FindIndex (nIndex);
	if (pos == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	return m_lstOptions.GetAt (pos);
}
//*******************************************************************************************
CBCGPGridItem* CBCGPGridItem::HitTest (CPoint point, CBCGPGridRow::ClickArea* pnArea)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);
	
	CRect rect = m_Rect;
	rect.right++;

	if (rect.PtInRect (point))
	{
		if (pnArea != NULL)
		{
			*pnArea = CBCGPGridRow::ClickValue;
		}

		return this;
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPGridItem::Redraw ()
{
	ASSERT_VALID (this);

	CBCGPGridCtrl* pWndList = NULL;
	if (m_pGridRow != NULL)
	{
		ASSERT_VALID (m_pGridRow);
		pWndList = m_pGridRow->m_pWndList;
	}

	if (pWndList != NULL && pWndList->GetSafeHwnd () != NULL)
	{
		ASSERT_VALID (pWndList);

		pWndList->InvalidateRect (m_Rect);

		if (!pWndList->m_bNoUpdateWindow)
		{
			pWndList->UpdateWindow ();
		}
	}
}
//*******************************************************************************************
void CBCGPGridItem::EnableSpinControl (BOOL bEnable, int nMin, int nMax)
{
	ASSERT_VALID (this);

	switch (m_varValue.vt)
	{
	case VT_INT:
	case VT_UINT:
    case VT_I2:
	case VT_I4:
    case VT_UI2:
	case VT_UI4:
		break;

	default:
		ASSERT (FALSE);
		return;
	}

	m_nMinValue = nMin;
	m_nMaxValue = nMax;

	if (bEnable)
	{
		m_dwFlags |= BCGP_GRID_ITEM_HAS_SPIN;
	}
	else
	{
		m_dwFlags &= ~BCGP_GRID_ITEM_HAS_SPIN;
	}
}
//*******************************************************************************************
void CBCGPGridItem::Select (BOOL bSelect)
{
	m_bSelected = bSelect;

	if (bSelect)
	{
		if (HasButton ())
		{
			AdjustButtonRect ();
		}
	}
	else
	{
		m_rectButton.SetRectEmpty ();
	}
}
//*******************************************************************************************
BOOL CBCGPGridItem::IsSelected () const
{
	return m_bSelected;
}
//******************************************************************************************
void CBCGPGridItem::SetValue (const _variant_t& varValue, BOOL bRedraw)
{
	ASSERT_VALID (this);

	if (m_varValue.vt != VT_EMPTY && m_varValue.vt != varValue.vt)
	{
		ASSERT (FALSE);
		return;
	}

	BOOL bInPlaceEdit = m_bInPlaceEdit;
	if (bInPlaceEdit)
	{
		OnEndEdit ();
	}

	m_varValue = varValue;
	m_bIsChanged = TRUE;

	if (bRedraw)
	{
		Redraw ();
	}

	if (bInPlaceEdit)
	{
		ASSERT_VALID (m_pGridRow);

		CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
		ASSERT_VALID (m_pGridRow);

		CPoint pt = m_Rect.TopLeft ();
		pWndList->EditItem (m_pGridRow, &pt);
	}
}
//*******************************************************************************************
void CBCGPGridItem::SetImage (int iImage, BOOL bRedraw/* = TRUE*/)
{
	ASSERT_VALID (this);

	m_iImage = iImage;

	if (bRedraw)
	{
		Redraw ();
	}
}
//*******************************************************************************************
void CBCGPGridItem::Enable (BOOL bEnable)
{
	ASSERT_VALID (this);

	if (m_bEnabled != bEnable)
	{
		m_bEnabled = bEnable;

		CBCGPGridCtrl* pWndList = NULL;
		if (m_pGridRow != NULL)
		{
			ASSERT_VALID (m_pGridRow);
			pWndList = m_pGridRow->m_pWndList;
		}

		if (pWndList != NULL && pWndList->GetSafeHwnd () != NULL)
		{
			ASSERT_VALID (pWndList);
			pWndList->InvalidateRect (m_Rect);
		}
	}
}
//*******************************************************************************************
void CBCGPGridItem::SetOwnerRow (CBCGPGridRow* pRow)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRow);

	m_pGridRow = pRow;
}
//*****************************************************************************************
CString CBCGPGridItem::FormatItem ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	// If merged, refer to main item
	CBCGPGridItem* pMergedItem = GetMergedMainItem ();

	const _variant_t& var = (pMergedItem != NULL) ? pMergedItem->GetValue () : m_varValue;

	CString strVal;

	switch (var.vt)
	{
	case VT_BSTR:
		strVal = (LPCTSTR)(_bstr_t)var;
		break;

	case VT_DECIMAL:
		{
			DECIMAL num = (DECIMAL) var;
			globalUtils.StringFromDecimal (strVal, num);
		}
		break;

	case VT_CY:
		{
			CY cy = (CY) var;
			globalUtils.StringFromCy (strVal, cy);
		}
		break;

    case VT_I2:
		strVal.Format (CBCGPGridRow::m_strFormatShort, (short) var);
		break;

	case VT_I4:
	case VT_INT:
		strVal.Format (CBCGPGridRow::m_strFormatLong, (long) var);
		break;

	case VT_UI1:
		strVal.Format (CBCGPGridRow::m_strFormatUShort, (BYTE) var);
		break;

    case VT_UI2:
		strVal.Format (CBCGPGridRow::m_strFormatUShort, var.uiVal);
		break;

	case VT_UINT:
	case VT_UI4:
		strVal.Format (CBCGPGridRow::m_strFormatULong, var.ulVal);
		break;

    case VT_R4:
		strVal.Format (CBCGPGridRow::m_strFormatFloat, (float) var);
		break;

    case VT_R8:
		strVal.Format (CBCGPGridRow::m_strFormatDouble, (double) var);
		break;

    case VT_BOOL:
		{
			bool bVal = (bool) var;
			strVal = bVal ? pWndList->m_strTrue : pWndList->m_strFalse;
		}
		break;

	case VT_DATE:
		{
			COleDateTime date = (DATE) var;
			strVal = date.Format ();
		}
		break;

	default:
		break;
	}

	return strVal;
}
//******************************************************************************************
void CBCGPGridItem::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);
	
	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	COLORREF clrTextOld = (COLORREF)-1;
	COLORREF clrText = OnFillBackground (pDC, rect);

	if (clrText != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor (clrText);
	}

	//-----------
	// Draw icon:
	//-----------
	OnDrawIcon (pDC, rect);

	// -----------
	// Draw value:
	// -----------
	rect.DeflateRect (TEXT_MARGIN, TEXT_VMARGIN);

	// If merged, refer to main item
	CBCGPGridMergedCells* pMerged = GetMergedCells ();
	const int nColumn = (pMerged != NULL) ? pMerged->GetMainItemID ().m_nColumn : GetGridItemID ().m_nColumn;

	int nTextAlign = pWndList->GetColumnAlign (nColumn);
	UINT uiTextFlags = DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;

	if (nTextAlign & HDF_CENTER)
	{
		uiTextFlags |= DT_CENTER;
	}
	else if (nTextAlign & HDF_RIGHT)
	{
		uiTextFlags |= DT_RIGHT;
	}
	else // nTextAlign & HDF_LEFT
	{
		uiTextFlags |= DT_LEFT;
	}

	if (m_dwFlags & BCGP_GRID_ITEM_VCENTER)
	{
		uiTextFlags |= DT_VCENTER;
	}
	else if (m_dwFlags & BCGP_GRID_ITEM_VBOTTOM)
	{
		uiTextFlags |= DT_BOTTOM;
	}
	else // m_dwFlags & BCGP_GRID_ITEM_VTOP
	{
		uiTextFlags |= DT_TOP;
	}

	// If merged, refer to main item
	CBCGPGridItem* pMergedItem = GetMergedMainItem ();
	const CString& strText = (pMergedItem != NULL) ? pMergedItem->GetLabel () : GetLabel ();

	pDC->DrawText (strText, rect, uiTextFlags);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}

	m_bValueIsTrancated = pDC->GetTextExtent (GetLabel ()).cx > rect.Width ();
}
//******************************************************************************************
void CBCGPGridItem::OnDrawIcon (CDC* pDC, CRect& rect)
{
	if (m_iImage < 0)
	{
		return;
	}

	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	if (pWndList->GetImageList () == NULL)
	{
		return;
	}

	int cx = 0;
	int cy = 0;

	VERIFY (::ImageList_GetIconSize (*pWndList->m_pImages, &cx, &cy));

	if (rect.left + cx > rect.right)
	{
		return;
	}
	
	CPoint pt = rect.TopLeft ();

	pt.x++;
	pt.y = (rect.top + 1 + rect.bottom - cy) / 2;

	VERIFY (pWndList->m_pImages->Draw (pDC, m_iImage, pt, ILD_NORMAL));

	rect.left += cx;
}
//******************************************************************************************
COLORREF CBCGPGridItem::OnFillBackground (CDC* pDC, CRect rect)
{
	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	COLORREF clrText = m_clrText != (COLORREF)-1 ? m_clrText : pWndList->m_ColorData.m_clrText;

	CBCGPGridItem* pCurSelItem = pWndList->GetCurSelItem ();

	BOOL bActiveItem =	pCurSelItem == this && 
						pWndList->AllowInPlaceEdit ();

	if (GetMergedCells () != NULL && pCurSelItem != NULL)
	{
		if (pCurSelItem->GetMergedCells () == GetMergedCells () &&
			pWndList->AllowInPlaceEdit ())
		{
			bActiveItem = TRUE;
		}
	}

	BOOL bNoHighlightActiveItem = !pWndList->IsHighlightActiveItem () || 
		pWndList->IsSelectionBorderEnabled ();
	BOOL bSelectionBorderActiveItem = pWndList->IsSelectionBorderForActiveItem () && 
		!pWndList->IsSelectionBorderEnabled ();

	CRect rectFill = rect;
	rectFill.top++;

	BOOL bSelected = 
		(pWndList->m_bSingleSel && pWndList->m_bWholeRowSel) ?
			m_pGridRow->IsSelected () :
			IsSelected ();

	if (bSelected && 
		m_pGridRow->HasValueField () && 
		!(bActiveItem && bNoHighlightActiveItem || m_bInPlaceEdit))
	{
		if (pWndList->m_bFocused && pWndList->IsWholeRowSel ())
		{
			rectFill.right++;
		}
		clrText = pWndList->OnFillSelItem (pDC, rectFill, this);
	}
	else if (bActiveItem && bNoHighlightActiveItem)
	{
		if (!m_rectButton.IsRectEmpty ())
		{
			rectFill.right = m_rectButton.left;
		}

		if (bSelectionBorderActiveItem || pWndList->IsSelectionBorderEnabled () &&
			(pWndList->m_lstSel.GetCount () > 1 || pWndList->IsGrouping ()))
		{
			pDC->FillRect (rectFill, &globalData.brBlack);

			rectFill.DeflateRect (1, 1);
		}

		if (pWndList->m_brBackground.GetSafeHandle () != NULL)
		{
			pDC->FillRect (rectFill, &pWndList->m_brBackground);
		}
		else
		{
			COLORREF clr = visualManager->OnFillGridItem (
				pWndList, pDC, rectFill, bSelected, bActiveItem, FALSE);
			if (clrText == (COLORREF)-1)
			{
				clrText = clr;
			}
		}
	}
	else 
	{
		CBCGPGridItemID id = pWndList->GetGridItemID (this);
		BOOL bCustomColors = FALSE;

		// Item has own color - first priority
		if (m_clrBackground != (COLORREF)-1)
		{
			CBrush br (m_clrBackground);
			pDC->FillRect (rectFill, &br);
			bCustomColors = TRUE;
		}

		// Use m_ColorData to get colors
		else if (!id.IsNull ())
		{
			if (id.m_nRow % 2 == 0)
			{
				bCustomColors = pWndList->m_ColorData.m_EvenColors.Draw (pDC, rectFill);
				if (m_clrText == (COLORREF)-1 &&
					pWndList->m_ColorData.m_EvenColors.m_clrText != (COLORREF)-1)
				{
					clrText = pWndList->m_ColorData.m_EvenColors.m_clrText;
				}
			}
			else
			{
				bCustomColors = pWndList->m_ColorData.m_OddColors.Draw (pDC, rectFill);
				if (m_clrText == (COLORREF)-1 &&
					pWndList->m_ColorData.m_OddColors.m_clrText != (COLORREF)-1)
				{
					clrText = pWndList->m_ColorData.m_OddColors.m_clrText;
				}
			}
		}

		if (!bCustomColors)
		{
			// If the column of this item is sorted
			BOOL bSortedColumn = (pWndList->m_bMarkSortedColumn &&
				!id.IsNull () &&
				(pWndList->m_Columns.GetColumnState (id.m_nColumn) != 0));

			COLORREF clr = visualManager->OnFillGridItem (
				pWndList, pDC, rectFill, bSelected, bActiveItem && bNoHighlightActiveItem, bSortedColumn);
			if (clrText == (COLORREF)-1)
			{
				clrText = clr;
			}
		}
	}

	return clrText;
}
//******************************************************************************************
void CBCGPGridItem::OnDrawButton (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	CBCGPToolbarComboBoxButton button;

	pDC->FillRect (rect, pWndList->m_bControlBarColors ?
		&globalData.brBarFace : &globalData.brBtnFace);

	if (m_dwFlags & BCGP_GRID_ITEM_HAS_LIST)
	{
		visualManagerMFC->OnDrawComboDropButton (pDC,
			rect, !m_bEnabled, m_bButtonIsDown, m_bButtonIsDown, &button);
		return;
	}

	CString str = _T("...");
	pDC->DrawText (str, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

#ifndef _BCGPGRID_STANDALONE
#ifndef _BCGSUITE_
	CBCGPVisualManager::BCGBUTTON_STATE state = 
					m_bButtonIsDown ?
					CBCGPVisualManager::ButtonsIsPressed :
					CBCGPVisualManager::ButtonsIsRegular;
#else
	CMFCVisualManager::AFX_BUTTON_STATE state = 
					m_bButtonIsDown ?
					CMFCVisualManager::ButtonsIsPressed :
					CMFCVisualManager::ButtonsIsRegular;
#endif
	visualManagerMFC->OnDrawButtonBorder (pDC, &button, rect, state);
#else
	COLORREF colorBorder = globalData.IsHighContastMode () ?
		globalData.clrBtnDkShadow : globalData.clrHilite;
	pDC->Draw3dRect (rect, colorBorder, colorBorder);
#endif
}
//******************************************************************************************
void CBCGPGridItem::OnDrawBorders (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	GRID_BORDERS borders;
	OnGetBorders (borders);

	if (borders.top != GRID_BORDERSTYLE_EMPTY)
	{
	}

	if (borders.left != GRID_BORDERSTYLE_EMPTY)
	{
	}

	if (borders.bottom != GRID_BORDERSTYLE_EMPTY)
	{
		pDC->MoveTo (rect.left, rect.bottom);
		pDC->LineTo (rect.right + 1, rect.bottom);
	}

	if (borders.right != GRID_BORDERSTYLE_EMPTY)
	{
		pDC->MoveTo (rect.right, rect.top);
		pDC->LineTo (rect.right, rect.bottom);
	}
}
//******************************************************************************************
void CBCGPGridItem::OnPrintValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);
	ASSERT (pWndList->m_bIsPrinting);

	// map to printer metrics
	HDC hDCFrom = ::GetDC(NULL);
	int nXMul = pDC->GetDeviceCaps(LOGPIXELSX); // pixels in print dc
	int nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX); // pixels in screen dc
	::ReleaseDC(NULL, hDCFrom);

	const int CALCULATED_TEXT_MARGIN = ::MulDiv (TEXT_MARGIN, nXMul, nXDiv);
	const CRect& rectClip = pWndList->m_PrintParams.m_pageInfo.m_rectPageItems;

	// -----------
	// Draw value:
	// -----------
	CString strVal = FormatItem ();

	COLORREF clrTextOld = pDC->SetTextColor (pWndList->m_clrPrintText);

	CRect rectText = rect;
	rectText.DeflateRect (CALCULATED_TEXT_MARGIN, 0);

	CRect rectClipText = rectText;
	rectClipText.NormalizeRect ();
	if (rectClipText.IntersectRect (&rectClipText, &rectClip))
	{
		// Draw text vertically centered
		ASSERT_VALID (pWndList->m_pPrintDC);

		TEXTMETRIC tm;
		pWndList->m_pPrintDC->GetTextMetrics (&tm);
		int nDescent = tm.tmDescent;
		int nVCenterOffset = (rectText.Height () - pDC->GetTextExtent (strVal).cy + nDescent) / 2;

		pDC->SetTextAlign (TA_LEFT | TA_TOP);
		pDC->ExtTextOut (rectText.left, rectText.top + nVCenterOffset, ETO_CLIPPED, &rectClipText, strVal, NULL);
	}

	pDC->SetTextColor (clrTextOld);
}
//******************************************************************************************
BOOL CBCGPGridItem::CanUpdateData () const
{
	if (m_pGridRow == NULL || m_pGridRow->m_pWndList == NULL)
	{
		return TRUE;
	}
	return m_pGridRow->m_pWndList->m_bUpdateItemData;
}
//******************************************************************************************
BOOL CBCGPGridItem::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	CString strText;
	m_pWndInPlace->GetWindowText (strText);

	// If merged, refer to main item
	CBCGPGridItem* pMergedItem = GetMergedMainItem ();
	CBCGPGridItem* pUpdateItem = (pMergedItem != NULL) ? pMergedItem : this;
	ASSERT_VALID (pUpdateItem);

	BOOL bRes = TRUE;
	BOOL bIsChanged = FormatItem () != strText;
	BOOL bUpdateData = bIsChanged && pUpdateItem->CanUpdateData ();

	if (bUpdateData)
	{
		bRes = pUpdateItem->TextToVar (strText);
	}

	if (bRes && bUpdateData)
	{
		pUpdateItem->SetItemChanged ();
	}

	return bRes;
}
//******************************************************************************************
void CBCGPGridItem::SetItemChanged ()
{
	ASSERT_VALID (m_pGridRow);

		CBCGPGridItemID id = GetGridItemID ();
		m_pGridRow->OnItemChanged (this, id.m_nRow, id.m_nColumn);
		m_bIsChanged = TRUE;
}
//******************************************************************************************
BOOL CBCGPGridItem::TextToVar (const CString& strText)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	switch (m_varValue.vt)
	{
	case VT_BSTR:
		m_varValue = (LPCTSTR) strText;
		return TRUE;

	case VT_UI1:
		{
			int nVal = _ttoi (strText);
			if (nVal < 0 || nVal > 255)
			{
				return FALSE;
			}

			m_varValue = (BYTE) nVal;
		}
		return TRUE;

	case VT_DECIMAL:
		{
			DECIMAL num;
			if (globalUtils.DecimalFromString (num, strText))
			{
				m_varValue = num;
				return TRUE;
			}

			return FALSE;
		}
		break;

	case VT_CY:
		{
			CY cy;
			if (globalUtils.CyFromString (cy, strText))
			{
				m_varValue = cy;
				return TRUE;
			}

			return FALSE;
		}

	case VT_I2:
		m_varValue = (short) _ttoi (strText);
		return TRUE;

	case VT_INT:
	case VT_I4:
		m_varValue = _ttol (strText);
		return TRUE;

	case VT_UI2:
		m_varValue.uiVal = unsigned short (_ttoi (strText));
		return TRUE;

	case VT_UINT:
	case VT_UI4:
		m_varValue.ulVal = unsigned long (_ttol (strText));
		return TRUE;

	case VT_R4:
		{
			float fVal = 0.;
			if (!strText.IsEmpty ())
			{
#if _MSC_VER < 1400
				_stscanf (strText, CBCGPGridRow::m_strFormatFloat, &fVal);
#else
				_stscanf_s (strText, CBCGPGridRow::m_strFormatFloat, &fVal);
#endif
			}

			m_varValue = fVal;
		}
		return TRUE;

	case VT_R8:
		{
			double dblVal = 0.;
			if (!strText.IsEmpty ())
			{
#if _MSC_VER < 1400
				_stscanf (strText, CBCGPGridRow::m_strFormatDouble, &dblVal);
#else
				_stscanf_s (strText, CBCGPGridRow::m_strFormatDouble, &dblVal);
#endif
			}

			m_varValue = dblVal;
		}
		return TRUE;

	case VT_BOOL:
		m_varValue = (bool) (strText == pWndList->m_strTrue);
		return TRUE;
	}

	return FALSE;
}
//*****************************************************************************
BOOL CBCGPGridItem::OnEdit (LPPOINT)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	if (!m_bAllowEdit)
	{
		return FALSE;
	}

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	m_pWndInPlace = NULL;

	CRect rectEdit;
	CRect rectSpin;

	AdjustInPlaceEditRect (rectEdit, rectSpin);

	BOOL bDefaultFormat = FALSE;
	m_pWndInPlace = CreateInPlaceEdit (rectEdit, bDefaultFormat);

	if (m_pWndInPlace != NULL)
	{
		if (bDefaultFormat)
		{
			m_pWndInPlace->SetWindowText (FormatItem ());
		}

		if (m_dwFlags & BCGP_GRID_ITEM_HAS_LIST)
		{
			CRect rectCombo = m_Rect;
			rectCombo.left = rectEdit.left - 4;

			m_pWndCombo = CreateCombo (pWndList, rectCombo);
			ASSERT_VALID (m_pWndCombo);

			m_pWndCombo->SetFont (pWndList->GetFont ());

			//-------------------------------------------------------------------
			// Synchronize bottom edge of the combobox with the item bottom edge:
			//-------------------------------------------------------------------
			m_pWndCombo->GetWindowRect (rectCombo);
			pWndList->ScreenToClient (&rectCombo);

			int dy = rectCombo.Height () - m_Rect.Height ();

			m_pWndCombo->SetWindowPos (NULL, rectCombo.left,
				rectCombo.top - dy + 1, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

			if (m_varValue.vt == VT_BOOL)
			{
				m_lstOptions.AddTail (pWndList->m_strTrue);
				m_lstOptions.AddTail (pWndList->m_strFalse);
			}

			for (POSITION pos = m_lstOptions.GetHeadPosition (); pos != NULL;)
			{
				m_pWndCombo->AddString (m_lstOptions.GetNext (pos));
			}
		}

		if (m_dwFlags & BCGP_GRID_ITEM_HAS_SPIN)
		{
			m_pWndSpin = CreateSpinControl (rectSpin);
		}

		m_pWndInPlace->SetFont (pWndList->GetFont ());
		m_pWndInPlace->SetFocus ();

		if (!m_bAllowEdit)
		{
			m_pWndInPlace->HideCaret ();
		}

		m_bInPlaceEdit = TRUE;
		return TRUE;
	}

	return FALSE;
}
//******************************************************************************************
void CBCGPGridItem::AdjustButtonRect ()
{
	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	if (pWndList->AllowInPlaceEdit ())
	{
		m_rectButton = m_Rect;
		m_rectButton.left = m_rectButton.right - m_rectButton.Height () + 3;
		m_rectButton.top ++;
	}
	else
	{
		m_rectButton.SetRectEmpty ();
	}
}
//******************************************************************************************
void CBCGPGridItem::AdjustInPlaceEditRect (CRect& rectEdit, CRect& rectSpin)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);
	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	if (GetMergedCells () != NULL)
	{
		rectEdit = GetMergedRect ();
	}
	else
	{
		rectEdit = m_Rect;
	}

	rectEdit.DeflateRect (0, 2);

	if (m_iImage >= 0 && pWndList->GetImageList () != NULL)
	{
		int cx = 0;
		int cy = 0;

		VERIFY (::ImageList_GetIconSize (*pWndList->m_pImages, &cx, &cy));

		rectEdit.left += cx;
	}

	int nMargin = pWndList->m_nEditLeftMargin;

	rectEdit.left += TEXT_MARGIN - nMargin;

	if (HasButton ())
	{
		AdjustButtonRect ();
		rectEdit.right = m_rectButton.left - 1;
	}
	else
	{
		rectEdit.right--;
	}

	if (m_dwFlags & BCGP_GRID_ITEM_HAS_SPIN)
	{
		rectSpin = m_Rect;
		rectSpin.right = rectEdit.right;
		rectSpin.left = rectSpin.right - rectSpin.Height ();
		rectSpin.top ++;
		rectEdit.right = rectSpin.left;
		rectSpin.DeflateRect (0, 1);
	}
	else
	{
		rectSpin.SetRectEmpty ();
	}
}
//******************************************************************************************
CWnd* CBCGPGridItem::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	switch (m_varValue.vt)
	{
	case VT_EMPTY:
		return NULL;
		
	case VT_BSTR:
	case VT_R4:
	case VT_R8:
	case VT_UI1:
	case VT_I2:
	case VT_INT:
	case VT_UINT:
	case VT_I4:
	case VT_UI2:
	case VT_UI4:
	case VT_BOOL:
		break;

	}

	CEdit* pWndEdit = NULL;

	if (!m_strEditMask.IsEmpty () || !m_strEditTempl.IsEmpty () ||
		!m_strValidChars.IsEmpty ())
	{
		CBCGPMaskEdit* pWndEditMask = new CBCGPMaskEdit;

		if (!m_strEditMask.IsEmpty () && !m_strEditTempl.IsEmpty ())
		{
			pWndEditMask->EnableMask (m_strEditMask, m_strEditTempl, _T(' '));
		}

		if (!m_strValidChars.IsEmpty ())
		{
			pWndEditMask->SetValidChars (m_strValidChars);
		}
		pWndEditMask->EnableSetMaskedCharsOnly (FALSE);
		pWndEditMask->EnableGetMaskedCharsOnly (FALSE);

		pWndEdit = pWndEditMask;
	}
	else
	{
		pWndEdit = new CEdit;
	}

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL;

	if (IsMultiline ())
	{
		dwStyle |= ES_MULTILINE;
	}

	if (!IsWordWrap ())
	{
		dwStyle |= ES_AUTOHSCROLL; // no wrapping, use scrolling instead
	}

	if (!m_bEnabled || !m_bAllowEdit)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->Create (dwStyle, rectEdit, pWndList, BCGPGRIDCTRL_ID_INPLACE);

	bDefaultFormat = TRUE;
	return pWndEdit;
}
//*****************************************************************************
CSpinButtonCtrl* CBCGPGridItem::CreateSpinControl (CRect rectSpin)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	CSpinButtonCtrl* pWndSpin = new CBCGPSpinButtonCtrl;

	if (!pWndSpin->Create (
		WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_SETBUDDYINT | UDS_NOTHOUSANDS,
		rectSpin, pWndList, BCGPGRIDCTRL_ID_INPLACE))
	{
		return NULL;
	}

	pWndSpin->SetBuddy (m_pWndInPlace);

	if (m_nMinValue != 0 || m_nMaxValue != 0)
	{
		pWndSpin->SetRange32 (m_nMinValue, m_nMaxValue);
	}

	return pWndSpin;
}
//*****************************************************************************
BOOL CBCGPGridItem::OnEndEdit ()
{
	ASSERT_VALID (this);

	m_bInPlaceEdit = FALSE;
	OnDestroyWindow ();
	return TRUE;
}
//*****************************************************************************************
CComboBox* CBCGPGridItem::CreateCombo (CWnd* pWndParent, CRect rect)
{
	ASSERT_VALID (this);

	rect.bottom = rect.top + 400;

	CComboBox* pWndCombo = new CComboBox;
	if (!pWndCombo->Create (WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL, 
		rect, pWndParent, BCGPGRIDCTRL_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}

	return pWndCombo;
}
//****************************************************************************************
void CBCGPGridItem::DoClickButton (CPoint point)
{
	ASSERT_VALID (this);

	CString strPrevVal = FormatItem ();

	CWaitCursor wait;
	OnClickButton (point);

	if (strPrevVal != FormatItem ())
	{
		SetItemChanged ();
	}
}
//****************************************************************************************
void CBCGPGridItem::OnClickButton (CPoint)
{
	ASSERT_VALID (this);

	if (m_pWndCombo != NULL)
	{
		m_bButtonIsDown = TRUE;
		Redraw ();

		CString str;
		m_pWndInPlace->GetWindowText (str);

		m_pWndCombo->SetCurSel (m_pWndCombo->FindStringExact (-1, str));

		m_pWndCombo->SetFocus ();
		m_pWndCombo->ShowDropDown ();
	}
}
//****************************************************************************************
BOOL CBCGPGridItem::OnClickValue (UINT uiMsg, CPoint point)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (m_pWndInPlace);

	if (m_pWndSpin != NULL)
	{
		ASSERT_VALID (m_pWndSpin);
		ASSERT (m_pWndSpin->GetSafeHwnd () != NULL);

		CRect rectSpin;
		m_pWndSpin->GetClientRect (rectSpin);
		m_pWndSpin->MapWindowPoints (pWndList, rectSpin);

		if (rectSpin.PtInRect (point))
		{
			pWndList->MapWindowPoints (m_pWndSpin, &point, 1); 

			m_pWndSpin->SendMessage (uiMsg, 0, MAKELPARAM (point.x, point.y));
			return TRUE;
		}
	}

	CPoint ptEdit = point;
	::MapWindowPoints (	pWndList->GetSafeHwnd (), 
						m_pWndInPlace->GetSafeHwnd (), &ptEdit, 1);

	m_pWndInPlace->SendMessage (uiMsg, 0, MAKELPARAM (ptEdit.x, ptEdit.y));
	return TRUE;
}
//****************************************************************************************
BOOL CBCGPGridItem::OnDblClick (CPoint)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	if (m_lstOptions.GetCount () > 1)
	{
		CString strText;
		m_pWndInPlace->GetWindowText (strText);

		POSITION pos = m_lstOptions.Find (strText);
		if (pos == NULL)
		{
			return FALSE;
		}

		m_lstOptions.GetNext (pos);
		if (pos == NULL)
		{
			pos = m_lstOptions.GetHeadPosition ();
		}

		ASSERT (pos != NULL);
		strText = m_lstOptions.GetAt (pos);

		m_pWndInPlace->SetWindowText (strText);
		OnUpdateValue ();

		return TRUE;
	}

	if (m_dwFlags & BCGP_GRID_ITEM_HAS_LIST)
	{
		CWaitCursor wait;

		CString strPrevVal = GetLabel ();

		OnClickButton (CPoint (-1, -1));

		if (strPrevVal != FormatItem ())
		{
			SetItemChanged ();
		}

		return TRUE;
	}

	return FALSE;
}
//*****************************************************************************************
void CBCGPGridItem::OnSelectCombo ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndCombo);
	ASSERT_VALID (m_pWndInPlace);

	int iSelIndex = m_pWndCombo->GetCurSel ();
	if (iSelIndex >= 0)
	{
		CString str;
		m_pWndCombo->GetLBText (iSelIndex, str);
		m_pWndInPlace->SetWindowText (str);
		OnUpdateValue ();
	}
}
//****************************************************************************************
void CBCGPGridItem::OnCloseCombo ()
{
	ASSERT_VALID (this);

	m_bButtonIsDown = FALSE;
	Redraw ();

	ASSERT_VALID (m_pWndInPlace);
	m_pWndInPlace->SetFocus ();
}
//****************************************************************************************
BOOL CBCGPGridItem::OnSetCursor () const
{
	if (m_bInPlaceEdit)
	{
		return FALSE;
	}

	ASSERT_VALID (m_pGridRow);
	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	BOOL bActiveItem =	pWndList->GetCurSelItem () == this;

	if (pWndList->AllowInPlaceEdit () && IsAllowEdit () &&
		(bActiveItem || pWndList->IsEditFirstClick ()))
	{
		switch (m_varValue.vt)
		{
		case VT_BSTR:
		case VT_R4:
		case VT_R8:
		case VT_UI1:
		case VT_I2:
		case VT_INT:
		case VT_UINT:
		case VT_I4:
		case VT_UI2:
		case VT_UI4:
			SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_IBEAM));
			return TRUE;
		}
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPGridItem::PushChar (UINT nChar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);
	ASSERT (m_pGridRow->IsSelected ());//ASSERT (m_pWndList->m_pSel == this);
	ASSERT_VALID (m_pWndInPlace);

	switch (m_varValue.vt)
	{
	case VT_BSTR:
    case VT_R4:
    case VT_R8:
    case VT_UI1:
    case VT_I2:
	case VT_INT:
	case VT_UINT:
	case VT_I4:
    case VT_UI2:
	case VT_UI4:
		if (m_bEnabled && m_bAllowEdit)
		{
			m_pWndInPlace->SetWindowText (_T(""));
			m_pWndInPlace->SendMessage (WM_CHAR, (WPARAM) nChar);
			return TRUE;
		}
	}

	if (!m_bAllowEdit)
	{
		if (nChar == VK_SPACE)
		{
			OnDblClick (CPoint (-1, -1));
		}
		else if (m_lstOptions.GetCount () > 1)
		{
			CString strText;
			m_pWndInPlace->GetWindowText (strText);

			POSITION pos = m_lstOptions.Find (strText);
			if (pos == NULL)
			{
				return FALSE;
			}

			POSITION posSave = pos;
			CString strChar;
			strChar += (TCHAR) nChar;
			strChar.MakeUpper ();

			m_lstOptions.GetNext (pos);

			while (pos != posSave)
			{
				if (pos == NULL)
				{
					pos = m_lstOptions.GetHeadPosition ();
				}

				if (pos == posSave)
				{
					break;
				}

				strText = m_lstOptions.GetAt (pos);
				
				CString strUpper = strText;
				strUpper.MakeUpper ();

				if (strUpper.Left (1) == strChar)
				{
					m_pWndInPlace->SetWindowText (strText);
					OnUpdateValue ();
					break;
				}

				m_lstOptions.GetNext (pos);
			}
		}
	}

	OnEndEdit ();

	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	if (::GetCapture () == pWndList->GetSafeHwnd ())
	{
		ReleaseCapture ();
	}

	return FALSE;
}
//*******************************************************************************************
CString CBCGPGridItem::GetValueTooltip ()
{
	ASSERT_VALID (this);
	return m_bValueIsTrancated ? FormatItem () : _T("");
}
//*******************************************************************************************
void CBCGPGridItem::OnPosSizeChanged (CRect /*rectOld*/)
{
	CBCGPGridMergedCells* pMerged = GetMergedCells ();
	if (pMerged != NULL)
	{
		CRect rectCurr = CBCGPGridItem::GetRect ();
		if (!rectCurr.IsRectEmpty ())
		{
			// Calculate merged rectangle by the first visible item of the merged range
			CBCGPGridCtrl* pWndList = GetOwnerList ();
			ASSERT_VALID (pWndList);

			// save id of the first visible item in range
			pWndList->MarkMergedItemChanged (rectCurr, this);
		}

		// If the first visible item is not set yet - clear merged rectangle
		if (pMerged->GetVisibleItemID () == GetGridItemID () && !pMerged->IsChanged ())
		{
			pMerged->SetRectEmpty ();
		}
	}
}
//*******************************************************************************************
void CBCGPGridItem::Merge (CBCGPGridMergedCells* pMergedCells)
{
	if (m_pMerged != NULL)
	{
		m_pMerged->Release ();
		m_pMerged = NULL;
	}

	if (pMergedCells != NULL)
	{
		pMergedCells->AddRef ();
		m_pMerged = pMergedCells;
	}

	m_bIsChanged = TRUE;
}
//*******************************************************************************************
CBCGPGridMergedCells* CBCGPGridItem::GetMergedCells ()
{
	return m_pMerged;
}
//*******************************************************************************************
CRect CBCGPGridItem::GetMergedRect ()
{
	CBCGPGridMergedCells* pMerged = GetMergedCells ();
	if (pMerged != NULL)
	{
		return pMerged->GetRect ();
	}

	CRect rect (0, 0, 0, 0);
	return rect;
}
//*******************************************************************************************
BOOL CBCGPGridItem::GetMergedRange (CBCGPGridRange& range)
{
	CBCGPGridMergedCells* pMerged = GetMergedCells ();
	if (pMerged != NULL)
	{
		range = pMerged->GetRange ();
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
CBCGPGridItem* CBCGPGridItem::GetMergedMainItem () const
{
	if (m_pMerged != NULL)
	{
		ASSERT_VALID (m_pMerged);

		CBCGPGridItemID id = m_pMerged->GetMainItemID ();
		CBCGPGridCtrl* pGrid = GetOwnerList ();
		if (pGrid != NULL)
		{
			ASSERT_VALID (pGrid);

			CBCGPGridRow* pRow = pGrid->GetRow (id.m_nRow);
			if (pRow != NULL)
			{
				ASSERT_VALID (pRow);

				CBCGPGridItem* pItem = pRow->GetItem (id.m_nColumn);
				if (pItem != NULL)
				{
					ASSERT_VALID (pItem);

					return pItem;
				}
			}
		}
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPGridItem::OnGetBorders (GRID_BORDERS& borders)
{
	borders.top = GRID_BORDERSTYLE_EMPTY;
	borders.left = GRID_BORDERSTYLE_EMPTY;
	borders.bottom = GRID_BORDERSTYLE_DEFAULT;
	borders.right = GRID_BORDERSTYLE_DEFAULT;
}
//*******************************************************************************************
void CBCGPGridItem::OnGetBorders (CRect& rect)
{
	rect.SetRectEmpty ();

	GRID_BORDERS borders;
	OnGetBorders (borders);

	if (borders.left != GRID_BORDERSTYLE_EMPTY)
	{
		rect.left = 1;
	}
	if (borders.top != GRID_BORDERSTYLE_EMPTY)
	{
		rect.top = 1;
	}
	if (borders.right != GRID_BORDERSTYLE_EMPTY)
	{
		rect.right = 1;
	}
	if (borders.bottom != GRID_BORDERSTYLE_EMPTY)
	{
		rect.bottom = 1;
	}
}
//*******************************************************************************************
HBRUSH CBCGPGridItem::OnCtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->m_pWndList;
	ASSERT_VALID (pWndList);

	if (pWndList->m_ColorData.m_clrText != (COLORREF)-1)
	{
		pDC->SetTextColor (pWndList->m_ColorData.m_clrText);
	}

	if (pWndList->m_brBackground.GetSafeHandle () != NULL)
	{
		if (pWndList->m_ColorData.m_clrBackground != -1)
		{
			pDC->SetBkColor (pWndList->m_ColorData.m_clrBackground);
		}

		return (HBRUSH) pWndList->m_brBackground.GetSafeHandle ();
	}

	switch (m_varValue.vt)
	{
	case VT_BSTR:
    case VT_R4:
    case VT_R8:
    case VT_UI1:
    case VT_I2:
	case VT_I4:
	case VT_INT:
	case VT_UINT:
    case VT_UI2:
	case VT_UI4:
	case VT_BOOL:
		if (!m_bEnabled || !m_bAllowEdit)
		{
			pDC->SetBkColor (globalData.clrWindow);
			return (HBRUSH) globalData.brWindow.GetSafeHandle ();
		}
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPGridItem::SetBackgroundColor (COLORREF color, BOOL bRedraw)
{
	m_clrBackground = color;
	if (bRedraw)
	{
		Redraw ();
	}
}
//*******************************************************************************************
void CBCGPGridItem::SetTextColor (COLORREF color, BOOL bRedraw)
{
	m_clrText = color;
	if (bRedraw)
	{
		Redraw ();
	}
}
//*******************************************************************************************
BOOL CBCGPGridItem::IsWordWrap () const
{
	BOOL bWordWrap = IsMultiline () && ((m_dwFlags & BCGP_GRID_ITEM_WORDWRAP) != 0);

	// If merged then enable word-wrapping
	if (m_pMerged != NULL)
	{
		bWordWrap = TRUE;
	}

	return bWordWrap;
}

#ifndef _BCGPGRID_STANDALONE

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColorItem

IMPLEMENT_DYNCREATE(CBCGPGridColorItem, CBCGPGridItem)

CBCGPGridColorItem::CBCGPGridColorItem () :
	CBCGPGridItem (_variant_t(), 0),
	m_Color ((COLORREF) -1)
{
	m_varValue = (LONG) (COLORREF) -1;
	m_dwFlags = BCGP_GRID_ITEM_HAS_LIST;

	m_pPopup = NULL;
	m_bStdColorDlg = FALSE;
	m_ColorAutomatic = RGB (0, 0, 0);
	m_nColumnsNumber = 5;
}
//*****************************************************************************************
CBCGPGridColorItem::CBCGPGridColorItem(const COLORREF& color, 
							   CPalette* /*pPalette*/, DWORD_PTR dwData) :
	CBCGPGridItem (_variant_t(), dwData),
	m_Color (color)
{
	m_varValue = (LONG) color ;
	m_dwFlags = BCGP_GRID_ITEM_HAS_LIST;

	m_pPopup = NULL;
	m_bStdColorDlg = FALSE;
	m_ColorAutomatic = RGB (0, 0, 0);
	m_nColumnsNumber = 5;
}
//*****************************************************************************************
CBCGPGridColorItem::~CBCGPGridColorItem()
{
}
//*******************************************************************************************
void CBCGPGridColorItem::OnDrawValue (CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height ();
	CBCGPGridItem::OnDrawValue (pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height ();
	rectColor.DeflateRect (1, 1);
	rectColor.top++;
	rectColor.left++;

	CBrush br (m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color);
	pDC->FillRect (rectColor, &br);
	pDC->Draw3dRect (rectColor, 0, 0);
}
//****************************************************************************************
void CBCGPGridColorItem::OnPrintValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);
	ASSERT (pWndList->m_bIsPrinting);

	CRect rectColor = rect;

	rect.left += rect.Height ();
	CBCGPGridItem::OnPrintValue (pDC, rect);

	const CRect& rectClip = pWndList->m_PrintParams.m_pageInfo.m_rectPageItems;
	const CSize& szOne = pWndList->m_PrintParams.m_pageInfo.m_szOne;

	rectColor.right = rectColor.left + rectColor.Height ();
	rectColor.DeflateRect (szOne.cx, szOne.cy);
	rectColor.top += szOne.cy;
	rectColor.left += szOne.cx;

	rectColor.NormalizeRect ();
	if (rectColor.IntersectRect (&rectColor, &rectClip))
	{

		CBrush br (m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color);
		pDC->FillRect (rectColor, &br);

		CPen* pOldPen = pDC->SelectObject (&pWndList->m_penVLine);

		pDC->MoveTo (rectColor.TopLeft ());
		pDC->LineTo (rectColor.right, rectColor.top);
		pDC->LineTo (rectColor.BottomRight ());
		pDC->LineTo (rectColor.left, rectColor.bottom);
		pDC->LineTo (rectColor.TopLeft ());

		pDC->SelectObject (pOldPen);
	}
}
//****************************************************************************************
void CBCGPGridColorItem::OnClickButton (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	m_bButtonIsDown = TRUE;
	Redraw ();

	CList<COLORREF,COLORREF> lstDocColors;

	m_pPopup = new CColorPopup (NULL, m_Colors, m_Color,
		NULL, NULL, NULL, lstDocColors,
		m_nColumnsNumber, m_ColorAutomatic);

#ifndef _BCGSUITE_
	m_pPopup->SetParentGrid (pWndList);
#endif

	if (!m_strOtherColor.IsEmpty ())	// Other color button
	{
		((CBCGPColorBar*)m_pPopup->GetMenuBar ())->EnableOtherButton (m_strOtherColor, !m_bStdColorDlg);
	}

	if (!m_strAutoColor.IsEmpty ())	// Automatic color button
	{
		((CBCGPColorBar*)m_pPopup->GetMenuBar ())->EnableAutomaticButton (m_strAutoColor, m_ColorAutomatic);
	}

	CPoint pt (
		m_Rect.left + 1, 
		m_rectButton.bottom + 1);
	pWndList->ClientToScreen (&pt);

	if (!m_pPopup->Create (pWndList, pt.x, pt.y, NULL, FALSE))
	{
		ASSERT (FALSE);
		m_pPopup = NULL;
	}
	else
	{
		m_pPopup->GetMenuBar()->SetFocus ();
	}
}
//******************************************************************************************
BOOL CBCGPGridColorItem::OnEdit (LPPOINT /*lptClick*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	m_pWndInPlace = NULL;

	CRect rectEdit;
	CRect rectSpin;

	AdjustInPlaceEditRect (rectEdit, rectSpin);

	CBCGPMaskEdit* pWndEdit = new CBCGPMaskEdit;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;

	if (!m_bEnabled)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->EnableMask(
		_T("AAAAAA"), 
		_T("______"), 
		_T(' ')); 
	pWndEdit->SetValidChars(_T("01234567890ABCDEFabcdef"));

	pWndEdit->Create (dwStyle, rectEdit, pWndList, BCGPGRIDCTRL_ID_INPLACE);
	m_pWndInPlace = pWndEdit;

	m_pWndInPlace->SetWindowText (FormatItem ());

	m_pWndInPlace->SetFont (pWndList->GetFont ());
	m_pWndInPlace->SetFocus ();

	m_bInPlaceEdit = TRUE;
	return TRUE;
}
//****************************************************************************************
void CBCGPGridColorItem::AdjustInPlaceEditRect (CRect& rectEdit, CRect& rectSpin)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);
	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	rectSpin.SetRectEmpty ();

	rectEdit = m_Rect;
	rectEdit.DeflateRect (0, 2);

	int nMargin = pWndList->m_nEditLeftMargin;

	rectEdit.left = rectEdit.left + m_Rect.Height () + TEXT_MARGIN - nMargin + 1;

	AdjustButtonRect ();
	rectEdit.right = m_rectButton.left - 1;
}
//****************************************************************************************
CString CBCGPGridColorItem::FormatItem ()
{
	ASSERT_VALID (this);

	CString str;
	str.Format (_T("%02x%02x%02x"),
		GetRValue (m_Color), GetGValue (m_Color), GetBValue (m_Color));

	return str;
}
//******************************************************************************************
void CBCGPGridColorItem::SetColor (COLORREF color)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);

	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	m_Color = color;
	m_varValue = (LONG) color;

	if (::IsWindow (pWndList->GetSafeHwnd())) 
	{
		CRect rect = m_Rect;
		rect.DeflateRect (0, 1);

		pWndList->InvalidateRect (rect);
		pWndList->UpdateWindow ();
	}

	m_bIsChanged = TRUE;

	if (m_pWndInPlace != NULL)
	{
		ASSERT_VALID (m_pWndInPlace);
		m_pWndInPlace->SetWindowText (GetLabel ());
	}
}
//********************************************************************************
void CBCGPGridColorItem::SetColumnsNumber (int nColumnsNumber)
{
	ASSERT_VALID (this);
	ASSERT (nColumnsNumber > 0);

	m_nColumnsNumber = nColumnsNumber;
}
//*************************************************************************************
void CBCGPGridColorItem::EnableAutomaticButton (LPCTSTR lpszLabel, COLORREF colorAutomatic, BOOL bEnable)
{
	ASSERT_VALID (this);

	m_ColorAutomatic = colorAutomatic;
	m_strAutoColor = (!bEnable || lpszLabel == NULL) ? _T("") : lpszLabel;
}
//*************************************************************************************
void CBCGPGridColorItem::EnableOtherButton (LPCTSTR lpszLabel, BOOL bAltColorDlg, BOOL bEnable)
{
	ASSERT_VALID (this);

	m_bStdColorDlg = !bAltColorDlg;
	m_strOtherColor = (!bEnable || lpszLabel == NULL) ? _T("") : lpszLabel;
}
//*****************************************************************************************
BOOL CBCGPGridColorItem::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));
	ASSERT_VALID (m_pGridRow);

	CString strText;
	m_pWndInPlace->GetWindowText (strText);

	COLORREF colorCurr = m_Color;
	UINT nR = 0, nG = 0, nB = 0;
#if _MSC_VER < 1400
	_stscanf (strText, _T("%2x%2x%2x"), &nR, &nG, &nB);
#else
	_stscanf_s (strText, _T("%2x%2x%2x"), &nR, &nG, &nB);
#endif
	if (CanUpdateData ())
	{
		SetColor (RGB (nR, nG, nB));
	}

	if (colorCurr != m_Color)
	{
		SetItemChanged ();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridDateTimeItem

IMPLEMENT_DYNCREATE(CBCGPGridDateTimeItem, CBCGPGridItem)

CBCGPGridDateTimeItem::CBCGPGridDateTimeItem() :
	CBCGPGridItem (_variant_t(COleDateTime (), VT_DATE), 0)
{
	m_nFlags = 0;
	m_dwFlags = BCGP_GRID_ITEM_VCENTER;
}
//*****************************************************************************************
CBCGPGridDateTimeItem::CBCGPGridDateTimeItem(const COleDateTime& date, DWORD_PTR dwData/* = 0*/,
		UINT nFlags/* = CBCGPDateTimeCtrl::DTM_DATE | CBCGPDateTimeCtrl::DTM_TIME*/) :
	CBCGPGridItem (_variant_t(date, VT_DATE), dwData)
{
	m_nFlags = nFlags;
	m_dwFlags = BCGP_GRID_ITEM_VCENTER;
}
//*****************************************************************************************
CBCGPGridDateTimeItem::~CBCGPGridDateTimeItem()
{
}
//*****************************************************************************************
void CBCGPGridDateTimeItem::SetDateTime (const COleDateTime& date, DWORD_PTR dwData,
	UINT nFlags, BOOL bRedraw)
{
	m_nFlags = nFlags;
	SetData (dwData);
	SetValue (_variant_t(date, VT_DATE), bRedraw);
}
//*****************************************************************************************
void CBCGPGridDateTimeItem::OnDrawValue (CDC* pDC, CRect rect)
{
	CBCGPGridItem::OnDrawValue (pDC, rect);
}
//*****************************************************************************************
CWnd* CBCGPGridDateTimeItem::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	ASSERT_VALID (m_pGridRow);
	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	CBCGPDateTimeCtrl* pDateTime = new CBCGPDateTimeCtrl;
	ASSERT_VALID (pDateTime);

	pDateTime->SetAutoResize (FALSE);

	CRect rectSpin;
	AdjustInPlaceEditRect (rectEdit, rectSpin);

	pDateTime->Create (_T(""), WS_CHILD | WS_VISIBLE, rectEdit, 
		pWndList, BCGPGRIDCTRL_ID_INPLACE);
	pDateTime->SetFont (pWndList->GetFont ());

	CString strFormat;
	::GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, strFormat.GetBuffer (_MAX_PATH), _MAX_PATH);
	strFormat.ReleaseBuffer ();

	if (strFormat.Find (_T("MMMM")) != -1)
	{
		pDateTime->m_monthFormat = 1;
	}
	else if (strFormat.Find (_T("MMM")) != -1)
	{
		pDateTime->m_monthFormat = 0;
	}
	else
	{
		pDateTime->m_monthFormat = 2;
	}

	SetState (*pDateTime);
	pDateTime->SetDate (GetDate ());

	pDateTime->SetTextColor (pWndList->GetTextColor (), FALSE);
	pDateTime->SetBackgroundColor (pWndList->GetBkColor (), FALSE);

	bDefaultFormat = FALSE;

	return pDateTime;
}
//*****************************************************************************************
BOOL CBCGPGridDateTimeItem::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));
	ASSERT_VALID (m_pGridRow);

	CBCGPDateTimeCtrl* pDateTime = DYNAMIC_DOWNCAST (CBCGPDateTimeCtrl, m_pWndInPlace);
	ASSERT_VALID (pDateTime);

	COleDateTime dateOld = GetDate ();
	COleDateTime dateNew = pDateTime->GetDate ();
	
	if (CanUpdateData ())
	{
		m_varValue = _variant_t (dateNew, VT_DATE);
	}

	if (dateOld != dateNew)
	{
		SetItemChanged ();
	}

	return TRUE;
}
//****************************************************************************************
CString CBCGPGridDateTimeItem::FormatItem ()
{
	ASSERT_VALID (this);

	COleDateTime date = (DATE) m_varValue;

	SYSTEMTIME st;
	date.GetAsSystemTime (st);

	CString str;

	CString strDate;
	::GetDateFormat (LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, strDate.GetBuffer (_MAX_PATH), _MAX_PATH);
	strDate.ReleaseBuffer ();

	CString strTime;
	::GetTimeFormat (LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, strTime.GetBuffer (_MAX_PATH), _MAX_PATH);
	strTime.ReleaseBuffer ();

	if (m_nFlags == CBCGPDateTimeCtrl::DTM_DATE)
	{
		str = strDate;
	}
	else if (m_nFlags == CBCGPDateTimeCtrl::DTM_TIME)
	{
		str = strTime;
	}
	else
	{
		if (!strDate.IsEmpty ())
		{
			str = strDate;
		}

		if (!strTime.IsEmpty ())
		{
			if (!str.IsEmpty ())
			{
				str += _T(" ");
			}

			str += strTime;
		}
	}

	return str;
}
//*****************************************************************************************
void CBCGPGridDateTimeItem::OnSetSelection (CBCGPGridItem*)
{
	Redraw ();
}
//*****************************************************************************************
void CBCGPGridDateTimeItem::OnKillSelection (CBCGPGridItem*)
{
	Redraw ();
}
//*****************************************************************************************
BOOL CBCGPGridDateTimeItem::PushChar (UINT nChar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);
	ASSERT (m_pGridRow->IsSelected ());
	ASSERT_VALID (m_pWndInPlace);

	if (m_bEnabled && m_bAllowEdit)
	{
		CString str;
		str += (TCHAR) nChar;
		str.MakeUpper ();

		m_pWndInPlace->SendMessage (WM_KEYDOWN, (WPARAM) str [0]);
		return TRUE;
	}

	OnEndEdit ();

	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	if (::GetCapture () == pWndList->GetSafeHwnd ())
	{
		ReleaseCapture ();
	}

	return FALSE;
}
//*****************************************************************************************
void CBCGPGridDateTimeItem::SetDate (COleDateTime date)
{
	ASSERT_VALID (this);
	SetValue (_variant_t (date, VT_DATE));
}
//*****************************************************************************************
void CBCGPGridDateTimeItem::SetState (CBCGPDateTimeCtrl& wnd)
{
	ASSERT (wnd.GetSafeHwnd () != NULL);

	UINT nFlags = m_nFlags;
	if(!(nFlags & CBCGPDateTimeCtrl::DTM_DATE) && !(nFlags & CBCGPDateTimeCtrl::DTM_TIME))
	{
		nFlags |= (CBCGPDateTimeCtrl::DTM_DATE | CBCGPDateTimeCtrl::DTM_TIME);
	}

	UINT stateFlags = 0;

	if (nFlags & (CBCGPDateTimeCtrl::DTM_DATE))
	{
		stateFlags |= (CBCGPDateTimeCtrl::DTM_DATE | CBCGPDateTimeCtrl::DTM_DROPCALENDAR);
	}
	
	if (nFlags & (CBCGPDateTimeCtrl::DTM_TIME))
	{
		stateFlags |= (CBCGPDateTimeCtrl::DTM_TIME | CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE);
	}

	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;

	wnd.SetState (stateFlags, stateMask);
}
//*****************************************************************************************
void CBCGPGridDateTimeItem::AdjustInPlaceEditRect (CRect& rectEdit, CRect& rectSpin)
{
	CBCGPGridItem::AdjustInPlaceEditRect (rectEdit, rectSpin);

	rectEdit.bottom++;
}

#endif // _BCGPGRID_STANDALONE

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridCheckItem

IMPLEMENT_DYNCREATE (CBCGPGridCheckItem, CBCGPGridItem)

CBCGPGridCheckItem::CBCGPGridCheckItem () :
	CBCGPGridItem (_variant_t (false), 0)
{
	m_bAllowEdit = FALSE;
	m_dwFlags = m_dwFlags & ~BCGP_GRID_ITEM_HAS_LIST;
}
//*****************************************************************************************
CBCGPGridCheckItem::CBCGPGridCheckItem (bool bVal, DWORD_PTR dwData/* = 0*/) :
	CBCGPGridItem (_variant_t (bVal), dwData)
{
	m_bAllowEdit = FALSE;
	m_dwFlags = m_dwFlags & ~BCGP_GRID_ITEM_HAS_LIST;
}
//*****************************************************************************************
CBCGPGridCheckItem::~CBCGPGridCheckItem()
{
}
//*****************************************************************************************
void CBCGPGridCheckItem::OnDrawValue (CDC* pDC, CRect rect)
{
	CBCGPGridItem::OnDrawValue (pDC, rect);

	CRect rectCheck = rect;
	
	rectCheck.DeflateRect (0, 2);

	int nWidth = rectCheck.Height ();
	rectCheck.left = rectCheck.CenterPoint ().x - nWidth / 2;
	rectCheck.right = rectCheck.left + nWidth;

	COLORREF clrTextCold = pDC->GetTextColor ();

	visualManagerMFC->OnDrawCheckBox (pDC, rectCheck, FALSE,
		(bool) m_varValue, TRUE);

	pDC->SetTextColor (clrTextCold);
}
//*****************************************************************************************
BOOL CBCGPGridCheckItem::PushChar (UINT nChar)
{
	ASSERT_VALID (m_pGridRow);

	if (nChar == VK_SPACE)
	{
		BOOL bOldValue = (bool)GetValue ();
		SetValue (!bOldValue);

		SetItemChanged ();
		return TRUE;
	}
	
	return FALSE;
}
//*****************************************************************************************
BOOL CBCGPGridCheckItem::OnClickValue (UINT uiMsg, CPoint point)
{
	ASSERT_VALID (m_pGridRow);
	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	if ((uiMsg == WM_LBUTTONDOWN || uiMsg == WM_RBUTTONDOWN) &&
		m_pWndInPlace == NULL && pWndList->AllowInPlaceEdit ())
	{
		BOOL bOldValue = (bool)GetValue ();
		SetValue (!bOldValue);

		SetItemChanged ();
		return TRUE;
	}

	return CBCGPGridItem::OnClickValue (uiMsg, point);
}
//*****************************************************************************************
BOOL CBCGPGridCheckItem::OnDblClick (CPoint point)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pGridRow);
	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	if (m_pWndInPlace == NULL && pWndList->AllowInPlaceEdit ())
	{
		BOOL bOldValue = (bool)GetValue ();
		SetValue (!bOldValue);

		SetItemChanged ();
		return TRUE;
	}

	return CBCGPGridItem::OnDblClick (point);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridURLItem

IMPLEMENT_DYNCREATE (CBCGPGridURLItem, CBCGPGridItem)

CBCGPGridURLItem::CBCGPGridURLItem () :
	CBCGPGridItem (_variant_t ((LPCTSTR) _T("www.bcgsoft.com")), 0),
	m_strURL (_T("http://www.bcgsoft.com"))
{
}
//*****************************************************************************************
CBCGPGridURLItem::CBCGPGridURLItem (CString str, CString strURL, DWORD_PTR dwData/* = 0*/) :
	CBCGPGridItem (_variant_t ((LPCTSTR) str), dwData),
	m_strURL (strURL)
{
}
//*****************************************************************************************
CBCGPGridURLItem::~CBCGPGridURLItem()
{
}
//*****************************************************************************************
void CBCGPGridURLItem::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (m_pGridRow);

	COLORREF clrText = OnFillBackground (pDC, rect);

	//-----------
	// Draw icon:
	//-----------
	OnDrawIcon (pDC, rect);

	//-----------
	// Draw link:
	//-----------

#ifndef _BCGSUITE_
	COLORREF clrLink = globalData.clrHotText;
#else
	COLORREF clrLink = globalData.clrHotLinkNormalText;
#endif

	COLORREF clrTextOld = 
		pDC->SetTextColor (clrText == (COLORREF)-1 ? clrLink : clrText);

	CFont* pOldFont = pDC->SelectObject (CBCGPGridCtrl::m_bUseSystemFont ? &globalData.fontDefaultGUIUnderline : &globalData.fontUnderline);
	ASSERT (pOldFont != NULL);

	CString str = (LPCTSTR)(_bstr_t) m_varValue;

	rect.DeflateRect (TEXT_MARGIN, TEXT_VMARGIN);

	UINT uiTextFlags = DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;

	if (m_dwFlags & BCGP_GRID_ITEM_VCENTER)
	{
		uiTextFlags |= DT_VCENTER;
	}
	else if (m_dwFlags & BCGP_GRID_ITEM_VBOTTOM)
	{
		uiTextFlags |= DT_BOTTOM;
	}
	else // m_dwFlags & BCGP_GRID_ITEM_VTOP
	{
		uiTextFlags |= DT_TOP;
	}

	pDC->DrawText (str, rect, uiTextFlags);

	m_bValueIsTrancated = pDC->GetTextExtent (str).cx > rect.Width ();

	pDC->SelectObject (pOldFont);
	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}
}
//*****************************************************************************************
BOOL CBCGPGridURLItem::OnSetCursor () const
{
#ifndef _BCGPGRID_STANDALONE
	CBCGPGridCtrl* pWndList = GetOwnerList ();
	ASSERT_VALID (pWndList);

	if (pWndList->AllowInPlaceEdit () && IsAllowEdit ())
	{
		return CBCGPGridItem::OnSetCursor ();
	}
	
	::SetCursor (globalData.GetHandCursor ());
	return TRUE;
#else
	return FALSE;
#endif
}
//*****************************************************************************************
BOOL CBCGPGridURLItem::OnClickValue (UINT uiMsg, CPoint point)
{
	if (uiMsg == WM_LBUTTONDOWN && m_pWndInPlace == NULL)
	{
		CWaitCursor wait;

		CString strURL = m_strURL;

		if (strURL.IsEmpty ())
		{
			strURL = _T("http:\\\\");
			strURL += (LPCTSTR)(_bstr_t) m_varValue;
		}

		if (::ShellExecute (NULL, NULL, strURL, NULL, NULL, NULL) < (HINSTANCE) 32)
		{
			TRACE(_T("Can't open URL: %s\n"), m_strURL);
			return FALSE;
		}

		return TRUE;
	}

	return CBCGPGridItem::OnClickValue (uiMsg, point);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridMergedCells

CBCGPGridMergedCells::CBCGPGridMergedCells (BOOL bAutoDelete/* = TRUE*/)
{
	m_rectMerged.SetRectEmpty ();
	m_bChanged = FALSE;
	m_idFirstVisible.SetNull ();

	m_nRefCount = 1;
	m_bAutoDelete = bAutoDelete;
}
//*****************************************************************************
CBCGPGridMergedCells::~CBCGPGridMergedCells ()
{
	if (m_nRefCount > 0)
	{
		TRACE0 ("CBCGPGridMergedCells: refCount != 0\n");
	}
}
//*****************************************************************************
const CBCGPGridItemID CBCGPGridMergedCells::GetMainItemID () const
{
	CBCGPGridItemID id (m_range.m_nTop, m_range.m_nLeft);
	return id;
}
//*****************************************************************************
const CBCGPGridItemID CBCGPGridMergedCells::GetVisibleItemID () const
{
	return m_idFirstVisible;
}
//*****************************************************************************
void CBCGPGridMergedCells::SetRect (const CRect& rect)
{
	m_rectMerged = rect;
}
//*****************************************************************************
void CBCGPGridMergedCells::SetRectEmpty ()
{
	m_rectMerged.SetRectEmpty ();
}
//*****************************************************************************
void CBCGPGridMergedCells::SetRange (const CBCGPGridRange& r)
{
	m_range.Set (r);
}
//*****************************************************************************
CRect& CBCGPGridMergedCells::GetRect ()
{
	return m_rectMerged;
}
//*****************************************************************************
const CRect& CBCGPGridMergedCells::GetRect () const
{
	return m_rectMerged;
}
//*****************************************************************************
CBCGPGridRange& CBCGPGridMergedCells::GetRange ()
{
	return m_range;
}
//*****************************************************************************
const CBCGPGridRange& CBCGPGridMergedCells::GetRange () const
{
	return m_range;
}
//*****************************************************************************
void CBCGPGridMergedCells::MarkChanged (const CRect& rectNew, const CBCGPGridItemID& id)
{
	ASSERT (m_range.IsInRange (id));
	if (!rectNew.IsRectEmpty () && !id.IsNull ())
	{
		if (!m_bChanged)
		//if (idFirstVisible.IsNull ())
		{
			m_idFirstVisible = id;
		}
		m_bChanged = TRUE;
	}
}
//*****************************************************************************
void CBCGPGridMergedCells::MarkUpdated ()
{
	m_bChanged = FALSE;
}
//*****************************************************************************
void CBCGPGridMergedCells::AddRef ()
{
	m_nRefCount++;
}
//*****************************************************************************
void CBCGPGridMergedCells::Release ()
{
	--m_nRefCount;
	if (m_bAutoDelete && m_nRefCount <= 0)
	{
		delete this;
	}
}
//*****************************************************************************
void CBCGPGridMergedCells::SetAutoDelete (BOOL bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridRow

IMPLEMENT_DYNAMIC(CBCGPGridRow, CObject)

CString CBCGPGridRow::m_strFormatChar = _T("%c");
CString CBCGPGridRow::m_strFormatShort = _T("%d");
CString CBCGPGridRow::m_strFormatLong = _T("%ld");
CString CBCGPGridRow::m_strFormatUShort = _T("%u");
CString CBCGPGridRow::m_strFormatULong = _T("%u");
CString CBCGPGridRow::m_strFormatFloat = _T("%f");
CString CBCGPGridRow::m_strFormatDouble = _T("%lf");

CBCGPGridRow::CBCGPGridRow (const CString& /*strGroupName*/, DWORD_PTR dwData, int nBlockSize) :
	m_dwData (dwData), m_lstSubItems (nBlockSize)
{
	m_bGroup = TRUE;
	m_bExpanded = TRUE;

	m_nIdRow = -1;

	Init ();
	SetFlags ();
}
//******************************************************************************************
CBCGPGridRow::CBCGPGridRow (int /*nColumnsNum*/, DWORD_PTR dwData, int nBlockSize) :
	m_dwData (dwData), m_lstSubItems (nBlockSize)
{
	m_bGroup = FALSE;
	m_bExpanded = TRUE;

	m_nIdRow = -1;

	Init ();
	SetFlags ();
}
//******************************************************************************************
void CBCGPGridRow::SetFlags ()
{
	m_dwFlags = 0;
}
//******************************************************************************************
void CBCGPGridRow::Init ()
{
	m_pWndList = NULL;
	m_bSelected = FALSE;
	m_bEnabled = TRUE;
	m_bInPlaceEdit = FALSE;
	m_bAllowEdit = TRUE;
	m_bNameIsTrancated = FALSE;
	m_pParent = NULL;

	m_Rect.SetRectEmpty ();
}
//*******************************************************************************************
CBCGPGridRow::~CBCGPGridRow()
{
	while (!m_lstSubItems.IsEmpty ())
	{
		/*TODO delete */m_lstSubItems.RemoveHead ();
	}

	for (int i = (int) m_arrRowItems.GetUpperBound (); i >= 0; i--)
	{
		delete m_arrRowItems [i];
	}
	m_arrRowItems.RemoveAll ();
}
//******************************************************************************************
CBCGPGridItem* CBCGPGridRow::CreateItem (int nRow, int nColumn)
{
	if (m_pWndList == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	ASSERT_VALID (m_pWndList);
	return m_pWndList->CreateItem (nRow, nColumn);
}
//******************************************************************************************
BOOL CBCGPGridRow::ReplaceItem (int nColumn, CBCGPGridItem* pNewItem, BOOL bRedraw, BOOL bRepos)
{
	ASSERT_VALID (m_pWndList);

	if (nColumn >= 0 && nColumn < m_arrRowItems.GetSize ())
	{
		CBCGPGridItem* pOldItem = m_arrRowItems [nColumn];
		pNewItem->SetOwnerRow (this);
		pNewItem->m_nIdColumn = nColumn;
		m_arrRowItems [nColumn] = pNewItem;
		delete pOldItem;

		int y = m_Rect.top;
		m_pWndList->m_idCur.m_nRow = GetRowId ();

		if (bRepos)
		{
			Repos (y);
		}

		if (bRedraw)
		{
			pNewItem->Redraw ();
		}

		return TRUE;
	}

	return FALSE;
}
//******************************************************************************************
void CBCGPGridRow::AllowSubItems (BOOL bGroup)
{
	m_bGroup = bGroup;
}
//******************************************************************************************
void CBCGPGridRow::AddItem (CBCGPGridItem* pItem)
{
	pItem->SetOwnerRow (this);
	int nIndex = (int) m_arrRowItems.Add (pItem);
	pItem->m_nIdColumn = nIndex;
}
//******************************************************************************************
void CBCGPGridRow::SetItemRTC (int nColumn, CRuntimeClass* pRuntimeClass)
{
	if (m_pWndList != NULL)
	{
		m_pWndList->SetItemRTC (nColumn, pRuntimeClass);
	}
}
//******************************************************************************************
void CBCGPGridRow::SetDefaultItemRTC (CRuntimeClass* pRTC)
{
	if (m_pWndList != NULL)
	{
		SetDefaultItemRTC (pRTC);
	}
}
//******************************************************************************************
BOOL CBCGPGridRow::AddSubItem (CBCGPGridRow* pItem, BOOL bRedraw)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	if (!IsGroup ())
	{
		ASSERT(FALSE);
		return FALSE;
	}

#ifdef _DEBUG	
	if (pItem->m_pWndList != NULL)
	{
		ASSERT_VALID (pItem->m_pWndList);

		for (POSITION pos = pItem->m_pWndList->m_lstItems.GetHeadPosition (); pos != NULL;)
		{
			CBCGPGridRow* pListItem = pItem->m_pWndList->m_lstItems.GetNext (pos);
			ASSERT_VALID (pListItem);

			if (pListItem == pItem || pListItem->IsSubItem (pItem))
			{
				// Can't add the same item twice
				ASSERT (FALSE);
				return FALSE;
			}
		}
	}
#endif // _DEBUG

	pItem->m_pParent = this;

	int nPosParent = m_nIdRow;
	int nSubItemsCount = GetSubItemsCount (TRUE);
	int nPosInsertAfter = nPosParent + nSubItemsCount;
	m_pWndList->InsertRowAfter (nPosInsertAfter, pItem, bRedraw);

	m_lstSubItems.AddTail (pItem);
	pItem->m_pWndList = m_pWndList;

	return TRUE;
}
//*******************************************************************************************
int CBCGPGridRow::GetSubItemsCount (BOOL bRecursive) const
{
	ASSERT_VALID (this);

	if (!bRecursive)
	{
		return (int) m_lstSubItems.GetCount ();
	}

	int nCount = 0;

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pItem);

		nCount += pItem->GetSubItemsCount (TRUE) + 1;
	}

	return nCount;
}
//*******************************************************************************************
void CBCGPGridRow::GetSubItems (CList<CBCGPGridRow*, CBCGPGridRow*>& lst,
								BOOL bRecursive)
{
	ASSERT_VALID (this);

	if (!bRecursive)
	{
		lst.AddTail (&m_lstSubItems);
		return;
	}

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pItem);

		lst.AddTail (pItem);
		pItem->GetSubItems (lst, TRUE);
	}
}
//*******************************************************************************************
CBCGPGridRow* CBCGPGridRow::HitTest (CPoint point, int &iColumn, 
									 CBCGPGridItem*& pGridItem,
									 CBCGPGridRow::ClickArea* pnArea)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (m_Rect.PtInRect (point))
	{
		int dx = m_pWndList->IsSortingMode () && !m_pWndList->IsGrouping () ? 0 : 
			GetHierarchyLevel () * m_pWndList->m_nRowHeight;

		CRect rectExpand = m_Rect;
		rectExpand.DeflateRect (dx, 0, 0, 0);
		rectExpand.right = min (rectExpand.left + m_pWndList->m_nRowHeight, rectExpand.right);
		
		if (IsGroup () && (!m_pWndList->IsSortingMode () || m_pWndList->IsGrouping ()) 
			&& rectExpand.PtInRect (point))
		{
			if (pnArea != NULL)
			{
				*pnArea = ClickExpandBox;
			}
		}
		else
		{
			if (HasValueField ())
			{
				for (int i = 0; i < m_arrRowItems.GetSize (); i++)
				{
					CBCGPGridItem* pItem = m_arrRowItems [i];
					ASSERT_VALID (pItem);

					CBCGPGridItem* pHit = pItem->HitTest (point, pnArea);
					if (pHit != NULL)
					{
						iColumn = i;
						pGridItem = pHit;
						return this;
					}
				}
			}

			if (pnArea != NULL)
			{
				*pnArea = ClickName;
			}
		}

		iColumn = -1;
		pGridItem = NULL;
		return this;
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPGridRow::Expand (BOOL bExpand)
{
	ASSERT_VALID (this);
	ASSERT (IsGroup ());

	if (m_bExpanded == bExpand ||
		m_lstSubItems.IsEmpty ())
	{
		return;
	}

	CWaitCursor wait;

	OnExpand (bExpand);

	m_bExpanded = bExpand;

	if (m_pWndList != NULL && m_pWndList->GetSafeHwnd () != NULL)
	{
		ASSERT_VALID (m_pWndList);
		BOOL bRebuildTerminalItemsOld = m_pWndList->m_bRebuildTerminalItems;
		m_pWndList->m_bRebuildTerminalItems = FALSE;

		m_pWndList->AdjustLayout ();

		if (!m_pWndList->IsVirtualMode () && m_pWndList->IsRowExtraHeightAllowed ())
		{
			// Recalc extra height for row with specific height
			m_pWndList->AdjustLayout ();
		}

		m_pWndList->m_bRebuildTerminalItems = bRebuildTerminalItemsOld;

		CRect rectRedraw = m_pWndList->m_rectList;
		rectRedraw.top = m_Rect.top;

		m_pWndList->RedrawWindow (rectRedraw);
	}
}
//*******************************************************************************************
void CBCGPGridRow::ExpandDeep (BOOL bExpand)
{
	ASSERT_VALID (this);

	m_bExpanded = bExpand;

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pItem);

		pItem->ExpandDeep (bExpand);
	}
}
//*******************************************************************************************
BOOL CBCGPGridRow::IsItemFiltered () const
{
	return m_pWndList->FilterItem (this);
}
//*******************************************************************************************
void CBCGPGridRow::Redraw ()
{
	ASSERT_VALID (this);

	if (m_pWndList != NULL)
	{
		ASSERT_VALID (m_pWndList);
		m_pWndList->InvalidateRect (m_Rect);

		for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
		{
			CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
			ASSERT_VALID (pItem);

			m_pWndList->InvalidateRect (pItem->m_Rect);
		}

		if (!m_pWndList->m_bNoUpdateWindow)
		{
			m_pWndList->UpdateWindow ();
		}
	}
}
//*****************************************************************************************
void CBCGPGridRow::AdjustButtonRect ()
{
	ASSERT_VALID (m_pWndList);
	// adjust all buttons in the row
	for (int i = 0; i < m_arrRowItems.GetSize (); i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		if (!pItem->m_rectButton.IsRectEmpty () && pItem->HasButton ())
		{
			pItem->AdjustButtonRect ();
			m_pWndList->InvalidateRect (pItem->m_rectButton);
		}
	}
}
//*******************************************************************************************
void CBCGPGridRow::Select (BOOL bSelect)
{
	m_bSelected = bSelect;
}
//*******************************************************************************************
BOOL CBCGPGridRow::IsSelected () const
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	return m_pWndList->GetCurSel () == this || m_bSelected;
}
//*****************************************************************************************
BOOL CBCGPGridRow::IsParentExpanded () const
{
	ASSERT_VALID (this);

	for (CBCGPGridRow* pItem = m_pParent; pItem != NULL;)
	{
		ASSERT_VALID (pItem);

		if (!pItem->IsExpanded ())
		{
			return FALSE;
		}

		pItem = pItem->m_pParent;
	}

	return TRUE;
}
//******************************************************************************************
int CBCGPGridRow::GetHierarchyLevel () const
{
	ASSERT_VALID (this);

	int nLevel = 0;
	for (CBCGPGridRow* pParent = m_pParent; pParent != NULL;
		pParent = pParent->m_pParent)
	{
		nLevel++;
	}

	return nLevel;
}
//*******************************************************************************************
CBCGPGridRow* CBCGPGridRow::GetSubItem (int nIndex) const
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= m_lstSubItems.GetCount ())
	{
		ASSERT (FALSE);
		return NULL;
	}

	return m_lstSubItems.GetAt (m_lstSubItems.FindIndex (nIndex));
}
//*******************************************************************************************
CWnd* CBCGPGridRow::GetInPlaceWnd () const
{
	if (!m_bInPlaceEdit)
	{
		return NULL;
	}

	const CArray<CBCGPGridItem*, CBCGPGridItem*>& arr = m_arrRowItems;

	for (int i = 0; i < arr.GetSize (); i++)
	{
		CBCGPGridItem* pItem = arr [i];
		ASSERT_VALID (pItem);

		if (pItem->m_pWndInPlace != NULL)
		{
			ASSERT_VALID (pItem->m_pWndInPlace);
			return pItem->m_pWndInPlace;
		}
	}

	ASSERT (FALSE);
	return NULL;
}
//*******************************************************************************************
CComboBox* CBCGPGridRow::GetComboWnd () const
{
	if (!m_bInPlaceEdit)
	{
		return NULL;
	}

	const CArray<CBCGPGridItem*, CBCGPGridItem*>& arr = m_arrRowItems;

	for (int i = 0; i < arr.GetSize (); i++)
	{
		CBCGPGridItem* pItem = arr [i];
		ASSERT_VALID (pItem);

		if (pItem->m_pWndCombo != NULL)
		{
			ASSERT_VALID (pItem->m_pWndCombo);
			return pItem->m_pWndCombo;
		}
	}

	return NULL;
}
//*******************************************************************************************
CSpinButtonCtrl* CBCGPGridRow::GetSpinWnd () const
{
	if (!m_bInPlaceEdit)
	{
		return NULL;
	}

	const CArray<CBCGPGridItem*, CBCGPGridItem*>& arr = m_arrRowItems;

	for (int i = 0; i < arr.GetSize (); i++)
	{
		CBCGPGridItem* pItem = arr [i];
		ASSERT_VALID (pItem);

		if (pItem->m_pWndSpin != NULL)
		{
			ASSERT_VALID (pItem->m_pWndSpin);
			return pItem->m_pWndSpin;
		}
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPGridRow::Enable (BOOL bEnable/* = TRUE*/)
{
	ASSERT_VALID (this);

	if (m_bEnabled != bEnable)
	{
		m_bEnabled = bEnable;

		if (m_pWndList->GetSafeHwnd () != NULL)
		{
			ASSERT_VALID (m_pWndList);
			m_pWndList->InvalidateRect (m_Rect);
		}
	}
}
//*******************************************************************************************
void CBCGPGridRow::SetOwnerList (CBCGPGridCtrl* pWndList)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pWndList);

	m_pWndList = pWndList;

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pItem);

		pItem->SetOwnerList (m_pWndList);
	}
}
//*******************************************************************************************
void CBCGPGridRow::Repos (int& y)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (m_pWndList->m_bIsPrinting)
	{
		m_nIdRow = m_pWndList->m_PrintParams.m_idCur.m_nRow;
		m_pWndList->m_PrintParams.m_idCur.m_nRow ++;
	}
	else
	{
		m_nIdRow = m_pWndList->m_idCur.m_nRow;
		m_pWndList->m_idCur.m_nRow ++;
	}

	CRect rectOld = m_Rect;

	BOOL bShowAllItems = (m_pWndList->IsSortingMode () && !m_pWndList->IsGrouping ());
	BOOL bShowItem = bShowAllItems ? !IsItemFiltered () : IsItemVisible ();
	if (bShowItem)
	{
		int nXMul = 1, nXDiv = 1;
		if (m_pWndList->m_bIsPrinting)
		{
			// map to printer metrics
			ASSERT_VALID (m_pWndList->m_pPrintDC);
			HDC hDCFrom = ::GetDC(NULL);

			nXMul = m_pWndList->m_pPrintDC->GetDeviceCaps(LOGPIXELSX); // pixels in print dc
			nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX); // pixels in screen dc

			::ReleaseDC(NULL, hDCFrom);
		}

		int nHorzScrollOffset = m_pWndList->m_nHorzScrollOffset;
		if (m_pWndList->m_bIsPrinting)
		{
			nHorzScrollOffset = m_pWndList->m_PrintParams.m_nHorzScrollOffset;
		}

		int nRowHeight = m_pWndList->m_bIsPrinting ? m_pWndList->m_PrintParams.m_nRowHeight : m_pWndList->m_nRowHeight;
		int nLargeRowHeight = m_pWndList->m_bIsPrinting ? m_pWndList->m_PrintParams.m_nLargeRowHeight : m_pWndList->m_nLargeRowHeight;
		CRect& rectList = m_pWndList->m_bIsPrinting ? m_pWndList->m_PrintParams.m_rectList: m_pWndList->m_rectList;


		int dx = m_pWndList->IsSortingMode () && !m_pWndList->IsGrouping () ? 0 :
			GetHierarchyLevel () * nRowHeight;

		int nRowLeft = rectList.left - nHorzScrollOffset;
		int nRowWidth = m_pWndList->m_Columns.GetTotalWidth ();
		
		if (m_pWndList->m_bIsPrinting)
		{
			// map to printer metrics
			nRowWidth = ::MulDiv(nRowWidth, nXMul, nXDiv);
		}

		CRect rectRow (
			nRowLeft,
			y,
			nRowLeft + nRowWidth,
			y + (IsGroup () ? nLargeRowHeight : nRowHeight));
		OnMeasureGridRowRect (rectRow);
		m_Rect = rectRow;

		// -----------------
		// Repos grid items:
		// -----------------

		for (int i = 0; i < m_arrRowItems.GetSize (); i++)
		{
			CBCGPGridItem* pItem = m_arrRowItems [i];
			ASSERT_VALID (pItem);

			pItem->m_Rect.SetRectEmpty ();
			pItem->m_rectButton.SetRectEmpty ();
		}
			
		CSize szOne = m_pWndList->m_bIsPrinting ? 
			m_pWndList->m_PrintParams.m_pageInfo.m_szOne : CSize (1, 1);
		int nXLeft = nRowLeft;
		int nCount = 0;

		int nPos = m_pWndList->m_Columns.Begin ();
		while (nPos != m_pWndList->m_Columns.End ())
		{
			int iColumn = m_pWndList->m_Columns.Next (nPos);
			if (iColumn == -1)
			{
				break; // no more visible columns
			}

			ASSERT (iColumn >= 0);
			if (m_arrRowItems.GetSize () <= iColumn)
			{
				continue; // row has no item for this column
			}

			CBCGPGridItem* pItem = m_arrRowItems [iColumn];
			ASSERT_VALID (pItem);

			CRect rectItemOld = pItem->m_Rect;

			BOOL bIsTreeColumn = (m_pWndList->m_nTreeColumn == -1) ? (nCount == 0):
				(m_pWndList->m_nTreeColumn == iColumn);

			int nWidth = m_pWndList->m_Columns.GetColumnWidth (iColumn);
			if (m_pWndList->m_bIsPrinting)
			{
				// map to printer metrics
				nWidth = ::MulDiv(nWidth, nXMul, nXDiv);
			}

			int nTreeOffset = bIsTreeColumn ? m_pWndList->GetExtraHierarchyOffset () * szOne.cx + dx : 0; 
			if (bIsTreeColumn)
			{
				nWidth += m_pWndList->GetExtraHierarchyOffset () * szOne.cx +
					m_pWndList->GetHierarchyOffset () * szOne.cx;
			}

			BOOL bShowColumn = (nWidth > 0);

			if (bShowItem && bShowColumn)
			{
				CRect rectItem (
					nXLeft + nTreeOffset,
					y, 
					min (nXLeft + nWidth - 1, m_Rect.right),
					y + (IsGroup () ? nLargeRowHeight : nRowHeight));
				OnMeasureGridItemRect (rectItem, pItem);
				pItem->m_Rect = rectItem;

				if (!pItem->m_rectButton.IsRectEmpty ())
				{
					pItem->m_rectButton.top = pItem->m_Rect.top + 1;
					pItem->m_rectButton.bottom = pItem->m_Rect.bottom;
				}
			}
			else
			{
				pItem->m_Rect.SetRectEmpty ();
				pItem->m_rectButton.SetRectEmpty ();
			}

			pItem->OnPosSizeChanged (rectItemOld);

			nXLeft += nWidth;
			nCount ++;
		}

		y += m_Rect.Height ();
	}
	else
	{
		m_Rect.SetRectEmpty ();
	}

	OnPosSizeChanged (rectOld);
}
//*******************************************************************************************
void CBCGPGridRow::Shift (int dx, int dy)
{
	ASSERT_VALID (this);

	for (int iColumn = 0; iColumn < m_arrRowItems.GetSize (); iColumn++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [iColumn];
		ASSERT_VALID (pItem);

		if (!pItem->m_Rect.IsRectEmpty ())
		{
			CRect rectItemOld = pItem->m_Rect;
			
			pItem->m_Rect.OffsetRect (dx, dy);
			pItem->OnPosSizeChanged (rectItemOld);
		}
	}

	if (!m_Rect.IsRectEmpty ())
	{
		CRect rectOld = m_Rect;
		m_Rect.OffsetRect (dx, dy);

		OnPosSizeChanged (rectOld);
	}
}
//******************************************************************************************
void CBCGPGridRow::AddTerminalItem (CList<CBCGPGridRow*, CBCGPGridRow*>& lstItems)
{
	ASSERT_VALID (this);
	ASSERT (!m_pWndList->IsGrouping ());

	if (!m_bGroup || HasValueField ())
	{
		// ---------------------------------
		// Simple sorting (one sort column):
		// ---------------------------------
		if (!m_pWndList->IsMultipleSort ())
		{
			// Insert sorted:
			BOOL bInserted = FALSE;
			for (POSITION pos = lstItems.GetHeadPosition (); !bInserted && pos != NULL;)
			{
				POSITION posSave = pos;

				CBCGPGridRow* pItem = lstItems.GetNext (pos);

				int iSortedColumn = m_pWndList->GetSortColumn ();
				if (m_pWndList->CompareItems (pItem, this, iSortedColumn) > 0)
				{
					lstItems.InsertBefore (posSave, this);
					bInserted = TRUE;
				}
			}

			if (!bInserted)
			{
				lstItems.AddTail (this);
			}
		}

		// -------------
		// MultiSorting:
		// -------------
		else
		{
			// Get sort order:
			const int nSortCount = m_pWndList->m_Columns.GetGroupColumnCount () + m_pWndList->m_Columns.GetSortColumnCount ();
			int* aSortOrder = new int [nSortCount];
			memset (aSortOrder, 0, nSortCount * sizeof (int));

			if (!m_pWndList->m_Columns.GetGroupingColumnOrderArray ((LPINT) aSortOrder, nSortCount))
			{
				ASSERT (FALSE);	// error getting sort order

				lstItems.AddTail (this);
				delete [] aSortOrder;
				return;
			}

			// Insert sorted:
			BOOL bInserted = FALSE;
			for (POSITION pos = lstItems.GetHeadPosition (); !bInserted && pos != NULL;)
			{
				POSITION posSave = pos;

				CBCGPGridRow* pItem = lstItems.GetNext (pos);

				BOOL bTryNextSortedColumn = TRUE;
				int iLevel = 0;			// 0, ..., nSortCount - 1
				while (!bInserted && bTryNextSortedColumn && iLevel < nSortCount)
				{
					int iSortedColumn = aSortOrder [iLevel];
					
					int nCompare = m_pWndList->CompareItems (pItem, this, iSortedColumn);
					if (nCompare > 0)
					{
						lstItems.InsertBefore (posSave, this);
						bInserted = TRUE;
						bTryNextSortedColumn = FALSE;
					}
					else if (nCompare == 0)
					{
						iLevel++;
					}
					else
					{
						bTryNextSortedColumn = FALSE;
					}
				} // while
			}

			delete [] aSortOrder;

			if (!bInserted)
			{
				lstItems.AddTail (this);
			}
		}
	}
	else
	{
		m_Rect.SetRectEmpty ();
	}
}
//****************************************************************************************
void CBCGPGridRow::AddGroupedItem (CList<CBCGPGridRow*, CBCGPGridRow*>& lstItems)
{
	ASSERT_VALID (this);
	ASSERT (m_pWndList->IsGrouping ());

	if (!m_bGroup || HasValueField ())
	{
		CList <int, int> & lstGroupingColumns = m_pWndList->m_Columns.m_lstGroupingColumns;
		const int nGroupingColumnsNum = (int) lstGroupingColumns.GetCount ();

		// Get sort order:
		const int nSortCount = m_pWndList->m_Columns.GetGroupColumnCount () + m_pWndList->m_Columns.GetSortColumnCount ();
		int* aSortOrder = new int [nSortCount];
		memset (aSortOrder, 0, nSortCount * sizeof (int));

		if (!m_pWndList->m_Columns.GetGroupingColumnOrderArray ((LPINT) aSortOrder, nSortCount))
		{
			ASSERT (FALSE);	// error getting sort order

			lstItems.AddTail (this);
			delete [] aSortOrder;
			return;
		}

		CArray <POSITION, POSITION> arrPositions;
		arrPositions.SetSize (nGroupingColumnsNum);
		for (int i = 0; i < arrPositions.GetSize (); i++)
		{
			arrPositions [i] = NULL;
		}

		POSITION posGroup = NULL;

		// Insert sorted:
		BOOL bInserted = FALSE;
		for (POSITION pos = lstItems.GetHeadPosition (); !bInserted && pos != NULL;)
		{
			POSITION posSave = pos;

			CBCGPGridRow* pItem = lstItems.GetNext (pos);
			ASSERT_VALID (pItem);

			BOOL bIsAutoGroup = pItem->IsGroup () &&
				(pItem->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
			if (bIsAutoGroup)
			{
				continue; // skip autogroup item
			}

			BOOL bTryNextSortedColumn = TRUE;
			int iLevel = 0;			// 0, ..., nGroupingColumnsNum - 1
			while (!bInserted && bTryNextSortedColumn && iLevel < nSortCount)
			{
				int iSortedColumn = aSortOrder [iLevel];

				int nCompare = 0;
				if (iLevel < nGroupingColumnsNum) // grouping
				{
					nCompare = m_pWndList->CompareGroup (pItem, this, iSortedColumn);
				}
				else	// compare items inside same group
				{
					nCompare = m_pWndList->CompareItems (pItem, this, iSortedColumn);
				}

				if (nCompare > 0)
				{
					POSITION posInserted = NULL;
					if (iLevel < nGroupingColumnsNum)
					{
						POSITION posFirstItemInGroup = NULL;
						int nParentLevel = -1;
						for (int i = 0; i < nGroupingColumnsNum; i++)
						{
							if (arrPositions [i] != NULL)
							{
								nParentLevel = i;
								posFirstItemInGroup = arrPositions [i];
							}
						}

						POSITION posInsertBefore = posSave;
						posGroup = InsertAutoGroupBefore (lstItems, posInsertBefore, posFirstItemInGroup, nParentLevel, this);
						posInserted = lstItems.InsertBefore (posInsertBefore, this);
					}
					else
					{
						ASSERT (iLevel >= nGroupingColumnsNum);
						POSITION posFirstItemInGroup = arrPositions [nGroupingColumnsNum - 1];
						POSITION posInsertBefore = posSave;
						posGroup = InsertAutoGroupBefore (lstItems, posInsertBefore, posFirstItemInGroup, nGroupingColumnsNum - 1, this);

						posInserted = lstItems.InsertBefore (posSave, this);
					}
					bInserted = TRUE;
					bTryNextSortedColumn = FALSE;

					for (int i = 0; i < arrPositions.GetSize (); i++)
					{
						if (posSave == arrPositions [i])
						{
							arrPositions [i] = posInserted;
						}
					}
				}
				else if (nCompare == 0)
				{
					if (iLevel < nGroupingColumnsNum && 
						arrPositions [iLevel] == NULL)
					{
						arrPositions [iLevel] = posSave;
					}
					iLevel++;
				}
				else
				{
					bTryNextSortedColumn = FALSE;
				}
			} // while
		}

		if (!bInserted)
		{
			POSITION posFirstItemInGroup = NULL;
			int nParentLevel = -1;
			for (int i = 0; i < nGroupingColumnsNum; i++)
			{
				if (arrPositions [i] != NULL)
				{
					nParentLevel = i;
					posFirstItemInGroup = arrPositions [i];
				}
			}

			POSITION posInsertBefore = NULL;
			posGroup = InsertAutoGroupBefore (lstItems, posInsertBefore, posFirstItemInGroup, nParentLevel, this);
			lstItems.AddTail (this);
		}

		//POSITION posGroup = arrPositions [nGroupingColumnsNum - 1];
		// Add item as subitem for the group
		if (posGroup != NULL)
		{
			CBCGPGridRow* pGroup = lstItems.GetAt (posGroup);
			BOOL bIsAutoGroup = pGroup != NULL && pGroup->IsGroup () && (pGroup->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
			if (bIsAutoGroup)
			{
				ASSERT_VALID (pGroup);

				// Add item as subitem for the group
				pGroup->m_lstSubItems.AddTail (this);
				m_pParent = pGroup;
			}
		}

		delete [] aSortOrder;
	}
	else
	{
		m_Rect.SetRectEmpty ();
	}
}
//****************************************************************************************
POSITION CBCGPGridRow::InsertAutoGroupBefore (
    CList<CBCGPGridRow*, CBCGPGridRow*>& lstItems, 
    POSITION& posInsertBefore, POSITION posFirstItemInGroup, int nParentLevel,
	CBCGPGridRow* pGroupedItem)
/*	if posInsertBefore - NULL, add last
	else use nParentLevel to skip not parent autogroups
		nParentLevel (0 <= nParentLevel <= ColumnNum-1)
		nParentLevel - -1, skip all autogroups (ColumnNum autogroups)
		nParentLevel - 0, skip ColumnNum-1 autogroups
		nParentLevel - ColumnNum-1, skip 0 autogroup
	if posFirstItemInGroup - NULL, create full hierarchy (all autogroups), nParentLevel is ignored
	else uses nParentLevel to search parent autogroup
		nParentLevel (0 <= nParentLevel <= ColumnNum-1)
		nParentLevel - -1, create full hierarchy (nParentLevel autogroups), do not search parent
		nParentLevel - 0, if parent found then create nParentLevel-1 autogroups, else create full hierarchy
		nParentLevel - ColumnNum-1, if parent found then do not create autogroup, else create full hierarchy

	nParentLevel - search parent autogroup at this level
	if nParentLevel - -1, 
*/
{
    ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);
    ASSERT (nParentLevel >= -1);
	ASSERT_VALID (pGroupedItem);

	const int nGroupingColumnsNum = 
		(int) m_pWndList->m_Columns.m_lstGroupingColumns.GetCount ();
    CBCGPGridRow* pParentGroup = NULL;
    POSITION posParentGroup = NULL;

    // Find parent group:
	if (posFirstItemInGroup != NULL)
	{
		POSITION pos = posFirstItemInGroup;
		POSITION posSave = posFirstItemInGroup;

        CBCGPGridRow* pItem = lstItems.GetPrev (pos);
        ASSERT_VALID (pItem);

		CBCGPGridRow* pGroup = NULL;
        if (pos != NULL)
        {
			posSave = pos;

            pGroup = lstItems.GetPrev (pos);
            ASSERT_VALID (pGroup);
        }

		int iLevel = nGroupingColumnsNum - 1;
		do
		{
			BOOL bIsAutoGroup = pGroup != NULL && pGroup->IsGroup () && (pGroup->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
			if (bIsAutoGroup && pItem != NULL && pGroup->IsSubItem (pItem))
			{
				if (iLevel == nParentLevel && iLevel >= 0)
				{
					pParentGroup = pGroup;
					posParentGroup = posSave;
				}

				// go level up
				pItem = pGroup;
				if (pos != NULL)
				{
					posSave = pos;

					pGroup = lstItems.GetPrev (pos);
					ASSERT_VALID (pGroup);
				}
				else
				{
					pGroup = NULL;
				}
			}

			// go level up
			iLevel--;
		}
		while (iLevel >= nParentLevel);
	}

	// Skip autogroups
    if (posInsertBefore != NULL)
    {
		POSITION pos = posInsertBefore;
		POSITION posSave = posInsertBefore;

        CBCGPGridRow* pItem = lstItems.GetPrev (pos);
        ASSERT_VALID (pItem);

		CBCGPGridRow* pGroup = NULL;
        if (pos != NULL)
        {
			posSave = pos;

            pGroup = lstItems.GetPrev (pos);
            ASSERT_VALID (pGroup);
        }

		BOOL bSkipAllLevels = FALSE;
		int iLevel = nGroupingColumnsNum - 1;
		do
		{
			BOOL bIsAutoGroup = pGroup != NULL && pGroup->IsGroup () && (pGroup->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
			if (bIsAutoGroup && pItem != NULL && pGroup->IsSubItem (pItem))
			{
				if (iLevel == nParentLevel && iLevel >= 0)
				{
					if (pParentGroup == NULL)
					{
						pParentGroup = pGroup;
					}
					bSkipAllLevels = pParentGroup != pGroup;
				}

				if (iLevel > nParentLevel || bSkipAllLevels)
				{
					posInsertBefore = posSave;	// insert before not parent autogroup
					ASSERT (posInsertBefore != NULL);
				}

				// go level up
				pItem = pGroup;
				if (pos != NULL)
				{
					posSave = pos;

					pGroup = lstItems.GetPrev (pos);
					ASSERT_VALID (pGroup);
				}
				else
				{
					pGroup = NULL;
				}
			}

			// go level up
			iLevel--;
		}
		while (iLevel >= nParentLevel || bSkipAllLevels && iLevel >= 0);
    }

	if (posParentGroup != NULL)
	{
		// parent group found - skip one level
		nParentLevel++;
	}
	else
	{
		// parent group not found - create full hierarchy
		nParentLevel = 0;
	}

	if (nParentLevel >= nGroupingColumnsNum)
	{
		return posParentGroup;
	}

    POSITION posGroup = NULL;
    for (int iLevel = nParentLevel; iLevel < nGroupingColumnsNum; iLevel++)
    {
	    // Create auto group:
	    CString strGroup;

		int nGroupCol = m_pWndList->m_Columns.GetGroupColumn (iLevel);
		int nGroupNumber = m_pWndList->OnGetGroupNumber (pGroupedItem, nGroupCol);
		if (nGroupNumber != -1)
		{
			strGroup = m_pWndList->OnGetGroupText (nGroupNumber, nGroupCol);
		}
		else
		{
			CBCGPGridItem* pItem = pGroupedItem->GetItem (nGroupCol);
			if (pItem != NULL)
			{
				ASSERT_VALID (pItem);
				strGroup = m_pWndList->GetGroupName (nGroupCol, pItem);
			}
		}

		CBCGPGridRow* pGroup = m_pWndList->CreateRow (strGroup);
		ASSERT_VALID (pGroup);

		pGroup->m_dwFlags |= BCGP_GRID_ITEM_AUTOGROUP;
		pGroup->m_bExpanded = IsAutoGroupExpanded (pGroupedItem, nGroupingColumnsNum - 1 - iLevel);
		m_pWndList->m_lstAutoGroups.AddTail (pGroup);

        if (posInsertBefore != NULL)
        {
	        posGroup = lstItems.InsertBefore (posInsertBefore, pGroup);
        }
        else
        {
            lstItems.AddTail (pGroup);
            posGroup = lstItems.GetTailPosition ();
        }
        ASSERT (posGroup != NULL);
	    pGroup->SetOwnerList (m_pWndList);

	    // Add group as subitem for the parent group:
	    if (pParentGroup != NULL)
        {
			pParentGroup->m_lstSubItems.AddTail (pGroup);
			pGroup->m_pParent = pParentGroup;
        }

        pParentGroup = pGroup;
    }

    return posGroup;
}
//****************************************************************************************
BOOL CBCGPGridRow::IsAutoGroupExpanded (CBCGPGridRow* pGroupedItem, int iLevel) const
{
	ASSERT_VALID (pGroupedItem);
	ASSERT (iLevel >= 0);
	ASSERT_VALID (m_pWndList);

	CList<CBCGPGridRow*, CBCGPGridRow*>& lstOldAutoGroups = 
		m_pWndList->m_lstOldAutoGroups;

	CBCGPGridRow* pParentAutoGroup = NULL;

	// Find direct parent:
	for (POSITION pos = lstOldAutoGroups.GetHeadPosition (); 
		pos != NULL && pParentAutoGroup == NULL; )
	{
		CBCGPGridRow* pAutoGroup = lstOldAutoGroups.GetNext (pos);
		ASSERT_VALID (pAutoGroup);

		for (POSITION posSub = pAutoGroup->m_lstSubItems.GetHeadPosition (); 
			posSub != NULL && pParentAutoGroup == NULL;)
		{
			CBCGPGridRow* pItem = pAutoGroup->m_lstSubItems.GetNext (posSub);
			ASSERT_VALID (pItem);
			
			if (pGroupedItem == pItem)
			{
				pParentAutoGroup = pAutoGroup;
			}
		}
	}

	// Find parent for specified level:
	if (pParentAutoGroup != NULL)
	{
		int nLevelCount = iLevel;
		while (nLevelCount > 0)
		{
			CBCGPGridRow* pGroup = pParentAutoGroup->m_pParent;
			BOOL bIsAutoGroup = pGroup != NULL && pGroup->IsGroup () && (pGroup->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
			if (bIsAutoGroup)
			{
				pParentAutoGroup = pGroup;
				ASSERT_VALID (pParentAutoGroup);
			}
			else // can't find parent group at iLevel
			{
				return TRUE;
			}

			nLevelCount--;
		}

		return pParentAutoGroup->m_bExpanded;
	}

	return TRUE;
}
//****************************************************************************************
BOOL CBCGPGridRow::IsSubItem (CBCGPGridRow* pSubItem) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pSubItem);

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pSubItem == pItem || pItem->IsSubItem (pSubItem))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//******************************************************************************************
CBCGPGridRow* CBCGPGridRow::FindSubItemByData (DWORD_PTR dwData) const
{
	ASSERT_VALID (this);
	
	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
        ASSERT_VALID (pItem);
		
        if (pItem->m_dwData == dwData)
        {
			return pItem;
		}
		
		pItem = pItem->FindSubItemByData (dwData);
		
		if (pItem != NULL)
		{
			return pItem;
        }
    }
	
	return NULL;
}
//*****************************************************************************************
CBCGPGridRow* CBCGPGridRow::FindSubItemById (int nIndex) const
{
	ASSERT_VALID (this);
	
	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = m_lstSubItems.GetNext (pos);
        ASSERT_VALID (pItem);
		
        if (pItem->m_nIdRow == nIndex)
        {
			return pItem;
		}
		
		pItem = pItem->FindSubItemById (nIndex);
		
		if (pItem != NULL)
		{
			return pItem;
        }
    }
	
	return NULL;
}
//*****************************************************************************************
CString CBCGPGridRow::FormatItem ()
{
	ASSERT_VALID (this);

	CString strVal;

	if (HasValueField ())
	{
		for (int i = 0; i < m_arrRowItems.GetSize (); i++)
		{
			CBCGPGridItem* pItem = m_arrRowItems [i];
			ASSERT_VALID (pItem);

			strVal += pItem->FormatItem ();

			if (i < m_arrRowItems.GetUpperBound ())
			{
				strVal += m_pWndList->m_cListDelimeter;
				strVal += _T(' ');
			}
		}
		
		if (m_arrRowItems.GetSize () > 0)
		{
			return strVal;
		}
	}

	strVal = _T("There is no items in this row.");
	return strVal;
}
//****************************************************************************************
CString CBCGPGridRow::GetName ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	BOOL bIsAutoGroup = IsGroup () && (m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;

	CString strName;
	if (!bIsAutoGroup || !m_pWndList->GetRowName (this, strName))
	{
		if (IsGroup ())
		{
			strName = _T("Group");//m_strName;
		}
	}

	return strName;
}
//****************************************************************************************
void CBCGPGridRow::OnDrawName (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);

	COLORREF clrTextOld = (COLORREF)-1;

	BOOL bIsAutoGroup = IsGroup () && (m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
	
	if (!HasValueField () || bIsAutoGroup)
	{
		COLORREF clrText = m_pWndList->GetGroupTextColor ();

		if (!m_pWndList->m_bHighlightGroups && IsSelected ())
		{
			CRect rectFill = rect;
			rectFill.top++;
			BOOL bGroupUnderline = !m_pWndList->m_bHighlightGroups && !HasValueField ();
			if (bGroupUnderline && IsGroup ())
			{
				rectFill.DeflateRect (0, 0, 0, 1);
			}
			
#ifndef _BCGSUITE_
	COLORREF clrHighlight = globalData.clrHotText;
#else
	COLORREF clrHighlight = globalData.clrHotLinkNormalText;
#endif

			COLORREF clrTextDefault = !m_pWndList->IsFocused () ?
					clrHighlight : globalData.clrTextHilite;
			if (!m_pWndList->m_ColorData.m_GroupSelColors.Draw (pDC, rectFill))
			{
				clrTextDefault = visualManager->OnFillGridRowBackground (
					m_pWndList, pDC, rectFill, IsSelected ());
			}
			
			if (m_pWndList->m_ColorData.m_GroupSelColors.m_clrText != (COLORREF)-1)
			{
				clrText = m_pWndList->m_ColorData.m_GroupSelColors.m_clrText;
			}
			else
			{
				clrText = clrTextDefault;
			}
		}

		if (clrTextOld == (COLORREF)-1)
		{
			clrTextOld = pDC->SetTextColor (clrText);
		}

		rect.DeflateRect (TEXT_MARGIN, 0);
		
		CString strName = GetName ();
		
		int nTextHeight = pDC->DrawText (strName, rect, 
			DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

		if (m_pWndList->m_bHighlightGroups && IsSelected ())
		{
			CRect rectFocus = rect;
			rectFocus.top = rectFocus.CenterPoint ().y - nTextHeight / 2;
			rectFocus.bottom = rectFocus.top + nTextHeight;
			rectFocus.right = rect.right;
			rectFocus.right = 
				min (rect.right, rectFocus.left + pDC->GetTextExtent (strName).cx);
			rectFocus.InflateRect (2, 0);

			COLORREF clrShadow = 
				(m_pWndList->m_ColorData.m_GroupColors.m_clrText != (COLORREF)-1) ?
				m_pWndList->m_ColorData.m_GroupColors.m_clrText :
				(m_pWndList->m_bControlBarColors ?
				globalData.clrBarShadow : globalData.clrBtnShadow);

			pDC->Draw3dRect (rectFocus, clrShadow, clrShadow);
		}

		m_bNameIsTrancated = pDC->GetTextExtent (strName).cx > rect.Width ();
	}
	else
	{
		if (m_pWndList->IsPreviewRowEnabled ())
		{
			OnDrawPreview (pDC, rect);
		}
	}

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}

}
//******************************************************************************************
void CBCGPGridRow::OnDrawPreview (CDC* /*pDC*/, CRect /*rect*/)
{
}
//******************************************************************************************
void CBCGPGridRow::OnDrawItems (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	rect.NormalizeRect ();

	for (int i = 0; i < m_arrRowItems.GetSize (); i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		CRect rectValue = pItem->GetRect ();
		if (!rectValue.IsRectEmpty ())
		{
			rectValue.NormalizeRect ();

			CRect rectClipItem = rectValue;
			rectClipItem.IntersectRect (rectClipItem, rect);

			CRect rectBordersSize (0, 0, 0, 0);
			if (m_pWndList->m_bGridItemBorders)
			{
				pItem->OnGetBorders (rectBordersSize);
				rectClipItem.InflateRect (rectBordersSize);
			}

			m_pWndList->m_rgnClipItem.CreateRectRgnIndirect (&rectClipItem);
			pDC->SelectClipRgn (&m_pWndList->m_rgnClipItem);

			if (pItem->GetMergedCells () != NULL)
			{
				// Item is a part of merged item - Expand item rect to whole merged item.
				rectValue = pItem->GetMergedRect ();
			}

			pItem->OnDrawValue (pDC, rectValue);

			if (m_pWndList->m_bGridItemBorders)
			{
				pItem->OnDrawBorders (pDC, rectValue);
			}

			if (!pItem->m_rectButton.IsRectEmpty ())
			{
				pItem->OnDrawButton (pDC, pItem->m_rectButton);
			}

			pDC->SelectClipRgn (&m_pWndList->m_rgnClipRow);
			m_pWndList->m_rgnClipItem.DeleteObject ();
		}
	}
}
//******************************************************************************************
void CBCGPGridRow::OnPrintName (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (!HasValueField ())
	{
		ASSERT_VALID (m_pWndList);
		ASSERT (m_pWndList->m_bIsPrinting);

		// map to printer metrics
		HDC hDCFrom = ::GetDC(NULL);
		int nXMul = pDC->GetDeviceCaps(LOGPIXELSX);			// pixels in print dc
		int nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX);	// pixels in screen dc
		::ReleaseDC(NULL, hDCFrom);

		const int CALCULATED_TEXT_MARGIN = ::MulDiv (TEXT_MARGIN, nXMul, nXDiv);
		const CRect& rectClip = m_pWndList->m_PrintParams.m_pageInfo.m_rectPageItems;

		CRect rectText = rect;
		rectText.DeflateRect (CALCULATED_TEXT_MARGIN, 0);
		CString strName = GetName ();

		CRect rectClipText = rectText;
		rectClipText.NormalizeRect ();
		if (rectClipText.IntersectRect (&rectClipText, &rectClip))
		{
			COLORREF clrTextOld = pDC->SetTextColor (m_pWndList->m_clrPrintText);

			// Draw text vertically centered
			ASSERT_VALID (m_pWndList->m_pPrintDC);

			TEXTMETRIC tm;
			m_pWndList->m_pPrintDC->GetTextMetrics (&tm);
			int nDescent = tm.tmDescent;
			int nVCenterOffset = (rectText.Height () - pDC->GetTextExtent (strName).cy + nDescent) / 2;

			pDC->SetTextAlign (TA_LEFT | TA_TOP);
			pDC->ExtTextOut (rectText.left, rectText.top + nVCenterOffset, ETO_CLIPPED, &rectClipText, strName, NULL);

			pDC->SetTextColor (clrTextOld);
		}
	}
}
//******************************************************************************************
void CBCGPGridRow::OnPrintItems (CDC* pDC, CRect /*rectItems*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	ASSERT_VALID (m_pWndList);
	const CRect rectClipPage = m_pWndList->m_PrintParams.m_pageInfo.m_rectPageItems;

	for (int i = 0; i < m_arrRowItems.GetSize (); i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		CRect rectClipRect = pItem->m_Rect;
		if (!rectClipRect.IsRectEmpty ())
		{
			rectClipRect.NormalizeRect ();
			if (rectClipRect.IntersectRect (&rectClipRect, &rectClipPage))
			{
				pItem->OnPrintValue (pDC, pItem->m_Rect);
			}
		}
	}
}
//****************************************************************************************
void CBCGPGridRow::OnDrawExpandBox (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);
	ASSERT (IsGroup ());

	BOOL bIsOpened = m_bExpanded && !m_lstSubItems.IsEmpty ();

	if (m_pWndList->m_bIsPrinting)
	{
		// map to printer metrics
		ASSERT_VALID (m_pWndList->m_pPrintDC);
		CSize szOne = m_pWndList->m_PrintParams.m_pageInfo.m_szOne;

		CPoint ptCenter = rect.CenterPoint ();
		// align to integral logical point
		ptCenter.Offset (-(ptCenter.x % szOne.cx), -(ptCenter.y % szOne.cy));

		int nMinSize = (szOne.cx != 0 && szOne.cy != 0) ?
			min (rect.Width () / szOne.cx, rect.Height () / szOne.cy) : 0;
		int nBoxSize = min (9, nMinSize); // in logical units

		rect = CRect (ptCenter, szOne);
		rect.InflateRect ((nBoxSize / 2) * szOne.cx, (nBoxSize / 2) * szOne.cy);

		CPen penLine (PS_SOLID, 1, m_pWndList->m_clrPrintText);
		CPen* pOldPen = pDC->SelectObject (&penLine);

		pDC->MoveTo (rect.TopLeft ());
		pDC->LineTo (rect.right, rect.top);
		pDC->LineTo (rect.BottomRight ());
		pDC->LineTo (rect.left, rect.bottom);
		pDC->LineTo (rect.TopLeft ());

		rect.DeflateRect (2 * szOne.cx, 2 * szOne.cy);

		pDC->MoveTo (rect.left, ptCenter.y + szOne.cy / 2);
		pDC->LineTo (rect.right, ptCenter.y + szOne.cy / 2);

		if (!bIsOpened)
		{
			pDC->MoveTo (ptCenter.x + szOne.cx / 2, rect.top);
			pDC->LineTo (ptCenter.x + szOne.cx / 2, rect.bottom);
		}

		pDC->SelectObject (pOldPen);
	}
	else
	{
		CPoint ptCenter = rect.CenterPoint ();

		int nBoxSize = min (9, rect.Width ());

		rect = CRect (ptCenter, CSize (1, 1));
		rect.InflateRect (nBoxSize / 2, nBoxSize / 2);

		COLORREF clrText = m_pWndList->m_bControlBarColors ? 
			globalData.clrBarText : globalData.clrBtnText;

		if (m_pWndList->m_bControlBarColors)
		{
			visualManager->OnDrawGridExpandingBox (pDC, rect, 
				bIsOpened, clrText);
		}
		else
		{
			visualManagerMFC->OnDrawExpandingBox (pDC, rect,
				bIsOpened, clrText);
		}
	}
}
//******************************************************************************************
void CBCGPGridRow::OnDrawRowMarker (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);
	ASSERT (!IsGroup ());

	if (m_pWndList->m_bIsPrinting)
	{
		return;
	}

	CPoint ptCenter = rect.CenterPoint ();

	int nBoxSize = min (9, rect.Width ());

	rect = CRect (ptCenter, CSize (1, 1));
	rect.InflateRect (nBoxSize / 2, nBoxSize / 2);

#ifndef _BCGPGRID_STANDALONE
	COLORREF clrTextOld = pDC->GetTextColor ();

	CBCGPMenuImages::Draw (pDC, CBCGPMenuImages::IdArowRightTab3d,
		rect.TopLeft ());

	pDC->SetTextColor (clrTextOld);
#else
	visualManager->OnDrawRowMarker (pDC, rect);
#endif
}
//******************************************************************************************
BOOL CBCGPGridRow::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (m_bInPlaceEdit)
	{
		// ---------------------------------------
		// Find inplace edited item and update it:
		// ---------------------------------------
		const CArray<CBCGPGridItem*, CBCGPGridItem*>& arr = m_arrRowItems;

		for (int i = 0; i < arr.GetSize (); i++)
		{
			CBCGPGridItem* pItem = (CBCGPGridItem*) arr [i];
			ASSERT_VALID (pItem);

			if (pItem->m_pWndInPlace != NULL)
			{
				ASSERT_VALID (pItem->m_pWndInPlace);
				return pItem->OnUpdateValue ();
			}
		}
	}
	else
	{
		CBCGPGridItem* pSelItem = m_pWndList->GetCurSelItem (this);
		if (pSelItem != NULL)
		{
			CBCGPGridItemID id = pSelItem->GetGridItemID ();
			m_pWndList->OnItemChanged (pSelItem, id.m_nRow, id.m_nColumn);
		}
	}

	return TRUE;
}
//*****************************************************************************
void CBCGPGridRow::OnItemChanged (CBCGPGridItem* pItem, int nRow, int nColumn)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);
	ASSERT_VALID (m_pWndList);

	m_pWndList->OnItemChanged (pItem, nRow, nColumn);
}
//*****************************************************************************
BOOL CBCGPGridRow::IsItemVisible () const
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	BOOL bIsVisible = IsParentExpanded ();

	if (bIsVisible)
	{
		return !IsItemFiltered (); // visible if not filtered
	}

	return bIsVisible;
}
//*****************************************************************************
BOOL CBCGPGridRow::OnEdit (LPPOINT lptClick)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (!HasValueField () || !m_pWndList->AllowInPlaceEdit ())
	{
		return FALSE;
	}

	if (lptClick == NULL)
	{
		CBCGPGridItem* pItem = m_pWndList->GetCurSelItem (this);
		if (pItem == NULL)
		{
			return FALSE;
		}

		_variant_t varValue = pItem->GetValue();
		if (varValue.vt == VT_BOOL)
		{
			return TRUE;
		}

		if (!pItem->IsAllowEdit ())
		{
			return FALSE;
		}

		m_bInPlaceEdit = pItem->OnEdit (lptClick);
		return m_bInPlaceEdit;
	}

	for (int i = 0; i < m_arrRowItems.GetSize (); i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		if (pItem->m_Rect.PtInRect (*lptClick) && pItem->m_bEnabled)
		{
			m_bInPlaceEdit = pItem->OnEdit (lptClick);

			if (m_bInPlaceEdit)
			{
				break;
			}
		}
	}

	return m_bInPlaceEdit;
}
//*****************************************************************************
BOOL CBCGPGridRow::OnEndEdit ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arrRowItems.GetSize () && m_bInPlaceEdit; i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		if (pItem->m_bInPlaceEdit)
		{
			pItem->OnEndEdit ();
		}
	}

	m_bInPlaceEdit = FALSE;
	return TRUE;
}
//****************************************************************************************
BOOL CBCGPGridRow::OnKillFocus (CWnd* pNewWnd)
{
	ASSERT_VALID (m_pWndList);
	
	CBCGPGridItem* pItem = m_pWndList->GetCurSelItem (this);
	if (pItem != NULL)
	{
		return pItem->OnKillFocus (pNewWnd);
	}

	return TRUE;
}
//****************************************************************************************
BOOL CBCGPGridRow::OnEditKillFocus ()
{
	ASSERT_VALID (m_pWndList);

	CBCGPGridItem* pItem = m_pWndList->GetCurSelItem (this);
	if (pItem != NULL && pItem->m_bInPlaceEdit && pItem->m_pWndInPlace != NULL)
	{
		return pItem->OnEditKillFocus ();
	}

	return TRUE;
}
//****************************************************************************************
BOOL CBCGPGridRow::OnClickValue (UINT uiMsg, CPoint point)
{
	ASSERT_VALID (this);

	if (!m_bInPlaceEdit)
	{
		return FALSE;
	}

	for (int i = 0; i < m_arrRowItems.GetSize (); i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		if (pItem->m_Rect.PtInRect (point))
		{
			return pItem->OnClickValue (uiMsg, point);
		}
	}

	return FALSE;
}
//****************************************************************************************
BOOL CBCGPGridRow::OnDblClick (CPoint point)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	CBCGPGridItem* pClickedItem = NULL;

	for (int i = 0; i < m_arrRowItems.GetSize (); i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		if (pItem->m_Rect.PtInRect (point))
		{
			pClickedItem = pItem;
			break;
		}
	}

	if (m_pWndList->GetOwner ()->SendMessage (
			BCGM_GRID_ITEM_DBLCLICK, 0, (LPARAM) pClickedItem) != 0)
	{
		return FALSE;
	}

	if (pClickedItem != NULL)
	{
		return pClickedItem->OnDblClick (point);
	}

	return FALSE;
}
//****************************************************************************************
void CBCGPGridRow::OnRClickValue (CPoint point, BOOL bSelChanged)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arrRowItems.GetSize () && !m_bInPlaceEdit; i++)
	{
		CBCGPGridItem* pItem = m_arrRowItems [i];
		ASSERT_VALID (pItem);

		if (pItem->m_Rect.PtInRect (point))
		{
			pItem->OnRClickValue (point, bSelChanged);
			break;;
		}
	}
}
//*****************************************************************************************
void CBCGPGridRow::OnSelectCombo ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	CBCGPGridItem* pItem = m_pWndList->GetCurSelItem (this);
	if (pItem != NULL && pItem->m_bInPlaceEdit && pItem->m_pWndCombo != NULL)
	{
		pItem->OnSelectCombo ();
	}
}
//****************************************************************************************
void CBCGPGridRow::OnCloseCombo ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	CBCGPGridItem* pItem = m_pWndList->GetCurSelItem (this);
	if (pItem != NULL && pItem->m_bInPlaceEdit)
	{
		pItem->OnCloseCombo ();
	}
}
//****************************************************************************************
BOOL CBCGPGridRow::PushChar (UINT nChar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	CBCGPGridItem* pItem = m_pWndList->GetCurSelItem (this);
	if (pItem != NULL)
	{
		ASSERT_VALID (pItem);

		if (pItem->m_pGridRow == this)
		{
			return pItem->PushChar (nChar);
		}
	}

	return FALSE;
}
//*******************************************************************************************
CString CBCGPGridRow::GetNameTooltip ()
{
	ASSERT_VALID (this);
	return m_bNameIsTrancated ? GetName () : _T("");
}
//*******************************************************************************************
CRect CBCGPGridRow::GetNameTooltipRect ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (!m_Rect.IsRectEmpty ())
	{
		CRect rectName = GetRect ();

		// --------------------
		// space for expandbox:
		// --------------------
		if (IsGroup () && (!m_pWndList->IsSortingMode () || m_pWndList->IsGrouping ()) &&
			!m_lstSubItems.IsEmpty ())
		{
			int dx = GetHierarchyLevel () * m_pWndList->GetRowHeight ();
			rectName.left += m_pWndList->GetRowHeight () + dx;
		}

		if (rectName.right > rectName.left)
		{
			return rectName;
		}
	}

	CRect rect (0, 0, 0, 0);
	return rect;
}
//*******************************************************************************************
HBRUSH CBCGPGridRow::OnCtlColor(CDC* pDC, UINT nCtlColor)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (m_pWndList->m_ColorData.m_clrText != (COLORREF)-1)
	{
		pDC->SetTextColor (m_pWndList->m_ColorData.m_clrText);
	}

	if (IsGroup () && m_pWndList->m_brGroupBackground.GetSafeHandle () != NULL)
	{
		if (m_pWndList->m_ColorData.m_GroupColors.m_clrBackground != -1)
		{
			pDC->SetBkColor (m_pWndList->m_ColorData.m_GroupColors.m_clrBackground);
		}

		return (HBRUSH) m_pWndList->m_brGroupBackground.GetSafeHandle ();
	}
	else if (m_pWndList->m_brBackground.GetSafeHandle () != NULL)
	{
		if (m_pWndList->m_ColorData.m_clrBackground != -1)
		{
			pDC->SetBkColor (m_pWndList->m_ColorData.m_clrBackground);
		}

		return (HBRUSH) m_pWndList->m_brBackground.GetSafeHandle ();
	}

	// Use the first cell in the row
	if (m_arrRowItems.GetSize () > 0)
	{
		CBCGPGridItem* pSelItem = m_arrRowItems [0];
		ASSERT_VALID (pSelItem);

		return pSelItem->OnCtlColor(pDC, nCtlColor);
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPGridRow::GetPreviewText (CString& str) const
{
	str.Empty ();
}
//*******************************************************************************************
void CBCGPGridRow::OnMeasureGridRowRect (CRect& /*rect*/)
{
}
//*******************************************************************************************
void CBCGPGridRow::OnMeasureGridItemRect (CRect& /*rect*/, CBCGPGridItem* /*pItem*/)
{
}


/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColumnsInfo object

CBCGPGridColumnsInfo::CBCGPGridColumnsInfo () :
	m_bAutoSize (FALSE), m_nTotalWidth (0), m_bMultipleSort (FALSE),
	m_bDrawingDraggedColumn (FALSE), m_nHighlightedItem (-1), m_nHighlightedItemBtn (-1), m_pWndList (NULL), m_bInvertPressedColumn (TRUE)
{
}
//*******************************************************************************************
CBCGPGridColumnsInfo::~CBCGPGridColumnsInfo ()
{
	DeleteAllColumns ();
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::DrawColumn (CDC* pDC, int nCol, CRect rect,
									   int nTextMargin, int nArrowMargin,
									   BOOL bIsPrinting,
									   BOOL bNoSortArrow,
									   BOOL bIsGroupBox)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);

	CRect rectColumn = rect;
	m_bInvertPressedColumn = TRUE;

	//-------------
	// Draw border:
	//-------------
	if (bIsPrinting)
	{
		CSize szOne = m_pWndList->m_PrintParams.m_pageInfo.m_szOne;
		CRect rectBorder = rect;
		rectBorder.bottom -= szOne.cy;

		pDC->MoveTo (rect.TopLeft ());
		pDC->LineTo (rect.right, rect.top);
		pDC->LineTo (rect.BottomRight ());
		pDC->MoveTo (rect.TopLeft ());
		pDC->LineTo (rect.left, rect.bottom);
		pDC->LineTo (rect.BottomRight ());
	}
	else
	{
		if (bIsGroupBox)
		{
			visualManager->OnDrawGridGroupByBoxItemBorder (m_pWndList, pDC, rect);
		}
		else
		{
			m_pWndList->OnDrawHeaderItemBorder (pDC, rect, nCol);
		}
	}

	if (nCol >= m_lstColumns.GetCount ())
	{
		return; // last
	}

	if (!bIsGroupBox && !bIsPrinting)
	{
		int bFirstVisibleColumn = FALSE;
		int nPos = Begin ();
		if (nPos != End ())
		{
			int nFirstColumn = Next (nPos);
			bFirstVisibleColumn = (nFirstColumn != -1 && nFirstColumn == nCol);
		}

		if (bFirstVisibleColumn)
		{
			int nHierarchyOffset = m_pWndList->GetHierarchyOffset () + 
				m_pWndList->GetExtraHierarchyOffset ();
			rect.DeflateRect (nHierarchyOffset, 0, 0, 0);
		}
	}

	BOOL bNotDrawButton = (nCol == m_pWndList->m_nDraggedColumn || bIsGroupBox);

	int nSortVal = GetColumnState (nCol);

	//------------
	// Draw image:
	//------------
	int nImage = GetColumnImage (nCol);

	if (nImage >= 0 && !(bIsGroupBox && 
		// ignore bIsGroupBox parameter when dragging from header
		(!m_bDrawingDraggedColumn ||
		m_pWndList->m_bDragGroupItem ||
		m_pWndList->m_bDragFromChooser) ) ) 
	{
		//---------------------------------------
		// The column has a image from imagelist:
		//---------------------------------------
		if (m_pWndList->m_pImagesHeader != NULL)
		{			
			int cx = 0;
			int cy = 0;

			VERIFY (::ImageList_GetIconSize (*m_pWndList->m_pImagesHeader, &cx, &cy));

			CPoint pt = rect.TopLeft ();
			pt.x ++;
			pt.y = (rect.top + rect.bottom - cy) / 2;

			int nArrow = 0; // width of the arrow or the button

			if (nSortVal != 0 && !bNoSortArrow)
			{
				CRect rectArrow (rect);
				rectArrow.DeflateRect (nArrowMargin, nArrowMargin);
				if (rectArrow.Width () >= (rectArrow.Height () + cx))
				{
					nArrow = rectArrow.Height () + nArrowMargin * 2;
				}
			}

			int nBtnWidth = m_pWndList->GetHeaderMenuButtonRect (rect, nCol).Width ();
			if (nBtnWidth > 0 && !bNotDrawButton)
			{
				nArrow = nBtnWidth;
			}

			if (rect.Width () > cx + nArrow)
			{
				if (!m_bDrawingDraggedColumn &&
					rect.Width () > cx)
				{
					int nAlign = GetHeaderAlign (nCol);

					if (nAlign & HDF_CENTER)
					{
						pt.x += (rect.Width () - nArrow - cx) / 2;
					}
					else if (nAlign & HDF_RIGHT)
					{
						pt.x = rect.right - nArrow - cx - 1;
					}

					rect.left = pt.x;
				}

				VERIFY (m_pWndList->m_pImagesHeader->Draw (pDC, nImage, pt, ILD_NORMAL));

				rect.left += cx;
			}
		}
	}

	if (m_pWndList->IsHeaderMenuButtonEnabled (nCol) && !bNotDrawButton)
	{
		CRect rectBtn = m_pWndList->GetHeaderMenuButtonRect (rect, nCol);
		m_pWndList->OnDrawHeaderMenuButton (pDC, rectBtn, nCol, !bNoSortArrow);
		rect.right = rectBtn.left - 1;
	}
	else if (nSortVal != 0 && !bNoSortArrow)
	{
		//-----------------
		// Draw sort arrow:
		//-----------------
		CRect rectArrow = rect;
		rectArrow.DeflateRect (nArrowMargin, nArrowMargin);
		if (rectArrow.Width () >= rectArrow.Height ())
		{
			rectArrow.left = rectArrow.right - rectArrow.Height ();
			rect.right = rectArrow.left - 1;

			int dy2 = (int) (.134 * rectArrow.Width ());
			rectArrow.DeflateRect (0, dy2);

			m_pWndList->OnDrawSortArrow (pDC, rectArrow, nSortVal > 0);
		}
	}

	CRect rectLabel = rect;
	rectLabel.DeflateRect (nTextMargin, 0);


	//-----------
	// Draw text:
	//-----------
	if (nImage < 0 || bIsGroupBox)
	{
		COLORREF clrText = m_pWndList->m_ColorData.m_HeaderColors.m_clrText;
		COLORREF clrTextOld = (COLORREF)-1;
		if (clrText != (COLORREF)-1 && !bIsPrinting)
		{
			clrTextOld = pDC->SetTextColor (clrText);
		}

		CRect rectLabel = rect;
		rectLabel.DeflateRect (nTextMargin, 0);

		CString strLabel = GetColumnName (nCol);
		int nTextAlign = GetHeaderAlign (nCol);

		UINT uiTextFlags = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
		if (!bIsGroupBox)	// ignore align flags in groupbox
		{
			if (nTextAlign & HDF_CENTER)
			{
				uiTextFlags |= DT_CENTER;
			}
			else if (nTextAlign & HDF_RIGHT)
			{
				uiTextFlags |= DT_RIGHT;
			}
		}

		pDC->DrawText (strLabel, rectLabel, uiTextFlags);

		if (clrTextOld != (COLORREF)-1)
		{
			pDC->SetTextColor (clrTextOld);
		}
	}

	if (m_bInvertPressedColumn &&
		nCol == m_pWndList->m_nDraggedColumn && !bIsGroupBox &&
		!m_pWndList->m_bDragGroupItem && !m_pWndList->m_bDragFromChooser)
	{
		pDC->InvertRect (rectColumn);
	}
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::SetHighlightColumn (int nColumn)
{
	m_nHighlightedItem = nColumn;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetHighlightColumn () const
{
	return m_nHighlightedItem;
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::SetHighlightColumnBtn (int nColumn)
{
	m_nHighlightedItemBtn = nColumn;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetHighlightColumnBtn () const
{
	return m_nHighlightedItemBtn;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::IsTextColumn (int nColumn) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nColumn);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumn);

		return pColumn->m_bText;
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetTextColumn (int nCol, BOOL bText)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		pColumnsItem->m_bText = bText;
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetLeftTextOffset () const
{
	ASSERT_VALID (this);

	int x = 0;

	int nPos = Begin ();
	while (nPos != End ())
	{
		int iColumn = Next (nPos);
		if (iColumn == -1)
		{
			return 0; // column not found - hidden column
		}

		ASSERT (iColumn >= 0);
		ASSERT (iColumn < GetColumnCount ());

		if (IsTextColumn (iColumn))
		{
			return x;
		}

		x += GetColumnWidth (iColumn);
	}

	return 0;
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::SetOwnerList (CBCGPGridCtrl* pWndList)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pWndList);

	m_pWndList = pWndList;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::InsertColumn (int nPos, LPCTSTR lpszColumn, int nWidth, int iImage)
{
	ASSERT_VALID (this);

	//------------------
	// Insert new column
	//------------------
	// Create new item:
	CBCGPGridColumnsItem* pColumnsItem = new CBCGPGridColumnsItem (lpszColumn, nWidth, iImage);
	ASSERT_VALID (pColumnsItem);

	// Insert the item at the specified index:
	if (m_lstColumns.GetCount () == nPos)
	{
		m_lstColumns.AddTail (pColumnsItem);
		OnInsertColumn (nPos);
		return nPos;
	}

	POSITION pos = m_lstColumns.FindIndex (nPos);
	if (pos != NULL)
	{
		m_lstColumns.InsertBefore (pos, pColumnsItem);
		OnInsertColumn (nPos);
		return nPos;
	}

	ASSERT (FALSE);
	return -1;
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::OnInsertColumn (int nPos)
{
	//----------------------------------------------------
	// Shift all column indexes in the internal containers
	//----------------------------------------------------
	//m_mapSortColumn;
	int i;
	for (i = (int) m_lstColumns.GetCount () - 2; i >= 0 && i >= nPos; i--)
	{
		int nState = 0;
		if (m_mapSortColumn.Lookup (i, nState))
		{
			m_mapSortColumn.RemoveKey (i);
			m_mapSortColumn.SetAt (i + 1, nState);
		}
	}
	//m_lstGroupingColumns;
	for (POSITION posGroup = m_lstGroupingColumns.GetHeadPosition (); posGroup != NULL;)
	{
		int iCol = m_lstGroupingColumns.GetAt (posGroup);
		if (iCol >= nPos)
		{
			iCol++;
			m_lstGroupingColumns.GetAt (posGroup) = iCol;
		}

		m_lstGroupingColumns.GetNext (posGroup);
	}
	//m_arrColumnOrder;
	for (i = nPos; i < m_arrColumnOrder.GetSize (); i++)
	{
		m_arrColumnOrder [i]++;
	}

	//-----------------------------------------
	// Add new column in the column order array
	//-----------------------------------------
	if (m_arrColumnOrder.GetSize () > 0)
	{
		m_arrColumnOrder.Add (nPos);

		ASSERT (m_arrColumnOrder.GetSize () == GetColumnCount (TRUE));
	}
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::DeleteColumn (int nPos)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nPos);
	if (pos != NULL)
	{
		RemoveSortColumn (nPos);
		RemoveGroupColumn (nPos);
		m_arrColumnOrder.RemoveAll ();

		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		m_lstColumns.RemoveAt (pos);
		delete pColumnsItem;

		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::InsertColumns (int nColumnsNum, int nDefaultWidth)
{
	ASSERT_VALID (this);

	for (int i = 0; i < nColumnsNum; i++)
	{
		CBCGPGridColumnsItem* pColumnsItem = new CBCGPGridColumnsItem (NULL, nDefaultWidth);
		ASSERT_VALID (pColumnsItem);

		m_lstColumns.AddTail (pColumnsItem);
	}
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::DeleteAllColumns ()
{
	ASSERT_VALID (this);

	while (!m_lstColumns.IsEmpty ())
	{
		delete m_lstColumns.RemoveTail ();
	}

	m_mapSortColumn.RemoveAll ();
	m_lstGroupingColumns.RemoveAll ();
	m_arrColumnOrder.RemoveAll ();
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetColumnCount (BOOL bCalcVisibleOnly) const
{
	ASSERT_VALID (this);

	if (bCalcVisibleOnly)
	{
		int nCount = 0;

		POSITION pos = m_lstColumns.GetHeadPosition ();
		while (pos != NULL)
		{
			CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetNext (pos);
			ASSERT_VALID (pColumnsItem);

			if (pColumnsItem->m_bVisible)
			{
				nCount++;
			}
		}

		return nCount;
	}

	return (int) m_lstColumns.GetCount ();
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetColumnWidth (int nColumn) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nColumn);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumn);

		if (m_bAutoSize)
		{
			return pColumn->m_nAutoSize;
		}

		int nSize = 0;
		if (pColumn->m_bVisible)
		{
			if (pColumn->m_bFixedSize)
			{
				// Specific size
				nSize = pColumn->m_nDefaultSize;
			}
			else
			{
				// Best fit
				nSize = BestFitColumn (nColumn);
			}
		}
		
		return nSize;
	}

	ASSERT (FALSE);
	return 0;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetColumnWidth(int nCol, int nWidth)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		pColumnsItem->m_nDefaultSize = nWidth;

		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
LPCTSTR CBCGPGridColumnsInfo::GetColumnName(int nCol) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		return pColumnsItem->m_strName;
	}

	ASSERT (FALSE);
	return NULL;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetColumnName(int nCol, LPCTSTR lpszColumn)
{
	ASSERT_VALID (this);
	ASSERT (lpszColumn != NULL);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		pColumnsItem->m_strName = lpszColumn;

		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetColumnAlign (int nCol) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		return pColumnsItem->m_nAlignment;
	}

	ASSERT (FALSE);
	return HDF_LEFT;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetColumnAlign (int nCol, int nAlign)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		pColumnsItem->m_nAlignment = nAlign;

		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetHeaderAlign (int nCol) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		return pColumnsItem->m_nHeaderAlignment;
	}

	ASSERT (FALSE);
	return HDF_LEFT;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetHeaderAlign (int nCol, int nAlign)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		pColumnsItem->m_nHeaderAlignment = nAlign;

		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetColumnImage (int nCol) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		return pColumnsItem->m_iImage;
	}

	ASSERT (FALSE);
	return HDF_LEFT;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetColumnImage (int nCol, int nImage)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		pColumnsItem->m_iImage = nImage;
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetColumnOrderArray (int iCount, LPINT piArray)
{
	ASSERT_VALID (this);
	ASSERT (iCount <= GetColumnCount ());
	ASSERT (AfxIsValidAddress (piArray, iCount * sizeof(int), FALSE)); //for readonly

	// -----------------------
	// Set all columns hidden:
	// -----------------------
	for (POSITION pos = m_lstColumns.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridColumnsItem* pColumn = m_lstColumns.GetNext (pos);
		ASSERT_VALID (pColumn);

		pColumn->m_bVisible = FALSE;
	}

	m_arrColumnOrder.SetSize (iCount);
	for (int i = 0; i < iCount; i++)
	{
		m_arrColumnOrder [i] = piArray [i];

		// ------------------------------------------------------------
		// Set visible the items which are specified in the order array
		// ------------------------------------------------------------
		POSITION pos = m_lstColumns.FindIndex (piArray [i]);
		if (pos != NULL)
		{
			CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (pos);
			ASSERT_VALID (pColumn);

			pColumn->m_bVisible = TRUE;
		}
	}

	return TRUE;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetColumnOrderArray (LPINT piArray, int iCount) const
{
	ASSERT_VALID (this);

	// if -1 was passed, find the count ourselves
	int nCount = iCount;
	if (nCount == -1)
	{
		nCount = GetColumnCount ();

		if (nCount == -1)
			return 0;
	}

	ASSERT (AfxIsValidAddress (piArray, nCount * sizeof(int)));
	ASSERT (nCount <= GetColumnCount ());

	// copy column indexes skipping hidden columns
	int i = 0;
	int iCol = 0;

	for (iCol = 0; iCol < m_arrColumnOrder.GetSize () && i < nCount; iCol++)
	{
		if (GetColumnVisible (m_arrColumnOrder [iCol])) // visible
		{
			int nSortCol = m_arrColumnOrder [iCol];
			piArray [i] = nSortCol;
			i++;
		}
	}
	if (m_arrColumnOrder.GetSize () == 0)
	{
		for (int nCol = 0; nCol < GetColumnCount () && i < nCount; nCol++)
		{
			if (GetColumnVisible (nCol)) // visible
			{
				int nSortCol = nCol;
				piArray [i] = nSortCol;
				i++;
			}
		}
	}

	return i;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetGroupingColumnOrderArray (LPINT piArray, int iCount) const
{
	ASSERT_VALID (this);

	// if -1 was passed, find the count ourselves
	int nCount = iCount;
	if (nCount == -1)
	{
		nCount = GetGroupColumnCount () + GetSortColumnCount ();

		if (nCount == -1)
			return 0;
	}

	//ASSERT (AfxIsValidAddress (piArray, nCount * sizeof(int), FALSE)); //for readonly
	ASSERT (AfxIsValidAddress (piArray, nCount * sizeof(int)));
	ASSERT (nCount <= GetGroupColumnCount () + GetSortColumnCount ());

	// Copy list of sorted columns to ensure every one will be recorded:
	CMap<int, int, int, int> mapNotReferenced;
	int nColSave, nStateSave;
	for (POSITION pos = m_mapSortColumn.GetStartPosition (); pos != NULL; )
	{
		m_mapSortColumn.GetNextAssoc (pos, nColSave, nStateSave);
		mapNotReferenced.SetAt (nColSave, nStateSave);
	}

	int i = 0;
	// first grouped columns
	POSITION posGroup = m_lstGroupingColumns.GetHeadPosition ();
	for (; posGroup != NULL && i < nCount; i++)
	{
		int nGroupCol = m_lstGroupingColumns.GetNext (posGroup);
		piArray [i] = nGroupCol;
	}
	// then sorted columns
	for (int iCol = 0; iCol < m_arrColumnOrder.GetSize () && i < nCount; iCol++)
	{
		if (GetColumnState (m_arrColumnOrder [iCol]) != 0) // the column is sorted
		{
			int nSortCol = m_arrColumnOrder [iCol];
			piArray [i] = nSortCol;
			i++;
			mapNotReferenced.RemoveKey (nSortCol);
		}
	}
	if (m_arrColumnOrder.GetSize () == 0)
	{
		for (int nCol = 0; nCol < GetColumnCount () && i < nCount; nCol++)
		{
			if (GetColumnState (nCol) != 0) // the column is sorted
			{
				int nSortCol = nCol;
				piArray [i] = nSortCol;
				i++;
			}
		}
	}
	else
	{
		// then sorted columns which are not referenced before (hidden sorted columns)
		for (int nCol = 0; nCol < GetColumnCount () && i < nCount; nCol++)
		{
			int nState;
			if (!mapNotReferenced.Lookup (nCol, nState) && nState != 0) // the column is sorted
			{
				int nSortCol = nCol;
				piArray [i] = nSortCol;
				i++;
			}
		}
	}

	mapNotReferenced.RemoveAll ();

	return i;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetGroupColumnCount () const
{
	return (int) m_lstGroupingColumns.GetCount ();
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetSortColumnCount () const
{
	int nCount = 0;

	int nColumn, nState;
	for (POSITION pos = m_mapSortColumn.GetStartPosition (); pos != NULL; )
	{
		m_mapSortColumn.GetNextAssoc (pos, nColumn, nState);

		if (nState != 0) // column is sorted
		{
			nCount++;
		}
	}

	return nCount;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::IsGroupColumn (int nCol) const
{
	return (m_lstGroupingColumns.Find (nCol) != NULL);
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetColumnState (int nCol) const
	// Returns: 0 - not not sorted, -1 - descending, 1 - ascending
{
	int nState = 0;
	if (!m_mapSortColumn.Lookup (nCol, nState))
	{
		return 0;
	}

	return nState;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::InsertGroupColumn (int nPos, int nColumn)
{
	ASSERT_VALID (this);
	
	POSITION posFound = m_lstGroupingColumns.Find (nColumn);

	if (m_lstGroupingColumns.GetCount () == nPos)
	{
		m_lstGroupingColumns.AddTail (nColumn);
		if (posFound != NULL)
		{
			m_lstGroupingColumns.RemoveAt (posFound);
		}
		return nPos;
	}

	POSITION pos = m_lstGroupingColumns.FindIndex (nPos);
	if (pos != NULL)
	{
		m_lstGroupingColumns.InsertBefore (pos, nColumn);
		if (posFound != NULL)
		{
			m_lstGroupingColumns.RemoveAt (posFound);
		}
		return nPos;
	}

	ASSERT (FALSE);
	return -1;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::RemoveGroupColumn (int nPos)
{
	POSITION pos = m_lstGroupingColumns.FindIndex (nPos);
	if (pos == NULL)
	{
		return FALSE;
	}

	m_lstGroupingColumns.RemoveAt (pos);
	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::RemoveGroupColumnByVal (int nColumn)
{
	POSITION pos = m_lstGroupingColumns.Find (nColumn);
	if (pos == NULL)
	{
		return FALSE;
	}

	m_lstGroupingColumns.RemoveAt (pos);
	return TRUE;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetGroupColumn (int nPos) const
{
	POSITION pos = m_lstGroupingColumns.FindIndex (nPos);
	if (pos != NULL)
	{
		return m_lstGroupingColumns.GetAt (pos);
	}

	return -1;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetGroupColumnPos (int nColumn)
{
	int nPos = 0;
	for (POSITION pos = m_lstGroupingColumns.GetHeadPosition (); pos != NULL; nPos++)
	{
		int nGroupColumn = m_lstGroupingColumns.GetNext (pos);

		if (nGroupColumn == nColumn)
		{
			return nPos;
		}
	}

	return -1;
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::SetSortColumn (int nColumn, BOOL bAscening, BOOL bAdd)
{
	if (bAdd)
	{
		if (!m_bMultipleSort)
		{
			ASSERT (FALSE);
			bAdd = FALSE;
		}
	}

	if (!bAdd)
	{
		m_mapSortColumn.RemoveAll ();
	}

	m_mapSortColumn.SetAt (nColumn, bAscening ? 1 : -1);
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::RemoveSortColumn (int nColumn)
{
	return m_mapSortColumn.RemoveKey (nColumn);
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::RemoveAllSortColumns ()
{
	m_mapSortColumn.RemoveAll ();
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetSortColumn () const
{
	ASSERT_VALID (this);

	if (m_bMultipleSort)
	{
		TRACE0("Call CBCGPGridColumnsInfo::GetColumnState for muliple sort\n");
		ASSERT (FALSE);
		return -1;
	}

	POSITION pos = m_mapSortColumn.GetStartPosition ();
	if (pos != NULL)
	{
		int nColumn, nState;
		m_mapSortColumn.GetNextAssoc (pos, nColumn, nState);

		if (nState != 0) // column is sorted
		{
			return nColumn;
		}
	}

	return -1;
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::EnableMultipleSort (BOOL bEnable)
{
	ASSERT_VALID (this);

	if (m_bMultipleSort == bEnable)
	{
		return;
	}

	m_bMultipleSort = bEnable;

	if (!m_bMultipleSort)
	{
		m_mapSortColumn.RemoveAll ();
	}
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::EnableAutoSize (BOOL bEnable)
{
	m_bAutoSize = bEnable;
}
//*******************************************************************************************
void CBCGPGridColumnsInfo::Resize (int nTotalWidth, int nStartColumn)
{
	ASSERT_VALID (this);
	ASSERT (nStartColumn >= -1);
	
	int nPosStartColumn = (nStartColumn != -1) ? IndexToOrder (nStartColumn) : -1;

	int nSum = 0;
	int nSumMin = 0;
	int nSumLocked = 0;

	POSITION posFirstSizingColumn = NULL;
	POSITION posLastSizingColumn = NULL;
	
	int nCount = 0; // count visible columns
	int nPos = Begin ();
	while (nPos != End ())
	{
		int nColumn = Next (nPos);
		if (nColumn == -1)
		{
			break; // no more visible columns
		}

		ASSERT (nColumn >= 0);
		ASSERT (nColumn < m_lstColumns.GetCount ());

		POSITION posSave = m_lstColumns.FindIndex (nColumn);
		ASSERT (posSave != NULL);

		CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (posSave);
		ASSERT_VALID (pColumn);

		if (pColumn->m_bVisible)
		{
			if (pColumn->m_bFixedSize)
			{
				// Specific size
				nSum += pColumn->m_nDefaultSize;
			}
			else
			{
				// Best fit
				nSum += BestFitColumn (nColumn);
			}
			
			if (CanChangeWidth (nColumn) && nCount > nPosStartColumn)
			{
				// Can sizing
				nSumMin += GetMinWidth (nColumn);

				if (posFirstSizingColumn == NULL)
				{
					posFirstSizingColumn = posSave;
				}
				posLastSizingColumn = posSave;
			}
			else
			{
				// Locked size
				nSumMin += pColumn->m_nDefaultSize;
				nSumLocked += pColumn->m_nDefaultSize;
			}
			
		}
		
		nCount++;
	}
	
	int nHierarchyOffset = 0;
	if (m_pWndList != NULL)
	{
		ASSERT_VALID (m_pWndList);
		nHierarchyOffset = 
			m_pWndList->GetHierarchyOffset () + 
			m_pWndList->GetExtraHierarchyOffset ();
	}

	const int nDefaultWidth = nSum;
	const int nMinWidth = nSumMin;
	const int nLockedWidth = nSumLocked;

	ASSERT (nDefaultWidth >= nLockedWidth);
	
	if (m_bAutoSize)
	{
		m_nTotalWidth = nTotalWidth - nHierarchyOffset;
		
		if (nMinWidth >= m_nTotalWidth ||
			nMinWidth >= nDefaultWidth)
		{
			// use minimal width for all columns
			nCount = 0;	// count visible columns
			int nPos = Begin ();
			while (nPos != End ())
			{
				int nColumn = Next (nPos);
				if (nColumn == -1)
				{
					break; // no more visible columns
				}

				ASSERT (nColumn >= 0);
				ASSERT (nColumn < m_lstColumns.GetCount ());

				POSITION posSave = m_lstColumns.FindIndex (nColumn);
				ASSERT (posSave != NULL);

				CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (posSave);
				ASSERT_VALID (pColumn);
	
				if (pColumn->m_bVisible)
				{
					if (CanChangeWidth (nColumn) && nCount > nPosStartColumn)
					{
						pColumn->m_nAutoSize = GetMinWidth (nColumn);
					}
					else
					{
						pColumn->m_nAutoSize = pColumn->m_nDefaultSize;
					}
				}

				nCount++;
			}
			
		}
		
		else if (m_nTotalWidth >= nDefaultWidth)
		{
			// enlarge columns in proporsion

			ASSERT (nLockedWidth >= 0);
			ASSERT (nDefaultWidth > nLockedWidth);
			ASSERT (m_nTotalWidth > nLockedWidth);
			float k = ((float) (m_nTotalWidth - nLockedWidth)) / 
				(nDefaultWidth - nLockedWidth);
			ASSERT (k >= 1);


			nSum = 0;
			
			nCount = 0;	// count visible columns
			int nPos = Begin ();
			while (nPos != End ())
			{
				int nColumn = Next (nPos);
				if (nColumn == -1)
				{
					break; // no more visible columns
				}

				ASSERT (nColumn >= 0);
				ASSERT (nColumn < m_lstColumns.GetCount ());

				POSITION posSave = m_lstColumns.FindIndex (nColumn);
				ASSERT (posSave != NULL);

				CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (posSave);
				ASSERT_VALID (pColumn);

				if (pColumn->m_bVisible)
				{
					if (CanChangeWidth (nColumn) && nCount > nPosStartColumn)
					{
						// Can sizing
						int nSize = 0;
						if (pColumn->m_bFixedSize)
						{
							// Specific size
							nSize = pColumn->m_nDefaultSize;
						}
						else
						{
							// Best fit
							nSize = BestFitColumn (nColumn);
						}
						
						int nMin = GetMinWidth (nColumn);
						nSize = max (nSize, nMin);
						
						pColumn->m_nAutoSize = (int) (k * nSize);
						pColumn->m_nAutoSize = max (GetMinWidth (nColumn), pColumn->m_nAutoSize);
					}
					else
					{
						// Locked size
						pColumn->m_nAutoSize = pColumn->m_nDefaultSize;
					}
				}
				
				nSum += pColumn->m_nAutoSize;
			
				nCount++;
			}
			
			int nDx = m_nTotalWidth - nSum;
			if (nDx > 0 && posLastSizingColumn != NULL)
			{
				CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (posLastSizingColumn);
				ASSERT_VALID (pColumn);
				
				pColumn->m_nAutoSize += nDx;
			}
			
		}
		
		else if (m_nTotalWidth < nDefaultWidth)
		{
			// compact columns in proporsion
			
			ASSERT (nLockedWidth >= 0);
			ASSERT (nDefaultWidth > nLockedWidth);
			ASSERT (m_nTotalWidth > nLockedWidth);
			float k = ((float) (m_nTotalWidth - nLockedWidth)) / 
				(nDefaultWidth - nLockedWidth);
			ASSERT (k < 1);

	
			nSum = 0;
	
			nCount = 0;	// count visible columns
			int nPos = Begin ();
			while (nPos != End ())
			{
				int nColumn = Next (nPos);
				if (nColumn == -1)
				{
					break; // no more visible columns
				}

				ASSERT (nColumn >= 0);
				ASSERT (nColumn < m_lstColumns.GetCount ());

				POSITION posSave = m_lstColumns.FindIndex (nColumn);
				ASSERT (posSave != NULL);

				CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (posSave);
				ASSERT_VALID (pColumn);

				if (pColumn->m_bVisible)
				{
					if (CanChangeWidth (nColumn) && nCount > nPosStartColumn)
					{
						// Can sizing
						int nSize = 0;
						if (pColumn->m_bFixedSize)
						{
							// Specific size
							nSize = pColumn->m_nDefaultSize;
						}
						else
						{
							// Best fit
							nSize = BestFitColumn (nColumn);
						}
						
						int nMin = GetMinWidth (nColumn);
						nSize = max (nSize, nMin);
						
						pColumn->m_nAutoSize = (int) (k * nSize);
						pColumn->m_nAutoSize = max (GetMinWidth (nColumn), pColumn->m_nAutoSize);
					}
					else
					{
						// Locked size
						pColumn->m_nAutoSize = pColumn->m_nDefaultSize;
					}
				}
				
				nSum += pColumn->m_nAutoSize;
			
				nCount++;
			}
			
			int nDx = m_nTotalWidth - nSum;
			if (nDx > 0 && posFirstSizingColumn != NULL)
			{
				CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (posFirstSizingColumn);
				ASSERT_VALID (pColumn);
				
				pColumn->m_nAutoSize += nDx;
			}
			
		}

		m_nTotalWidth += nHierarchyOffset;
	}
	else
	{
		// No auto size - ignore nTotalWidth param
		m_nTotalWidth = nDefaultWidth + nHierarchyOffset;
	}

}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::OnChangeColumnWidth (int nColumn, int &nWidth) const
	// returns FALSE, if width is not changing
{
	ASSERT_VALID (this);
	
	if (m_bAutoSize)
	{
		int nMin = nWidth;
		int nMax = nWidth;

		if (!GetColumnWidthMinMax (nColumn, nMin, nMax))
		{
			// column can't be resized
			return FALSE;
		}

		nWidth = max (nMin, min (nMax, nWidth));
	}
	else
	{
		int nMin = GetMinWidth (nColumn);
		nWidth = max (nMin, nWidth);
	}

	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::ResizeColumn (int nColumn, int nWidth)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nColumn);
	if (pos == NULL)
	{
		return FALSE;
	}

	CBCGPGridColumnsItem* pResizeColumn = m_lstColumns.GetAt (pos);
	ASSERT_VALID (pResizeColumn);

	if (m_bAutoSize)
	{
		for (pos = m_lstColumns.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridColumnsItem* pColumn = m_lstColumns.GetNext (pos);
			ASSERT_VALID (pColumn);

			if (pColumn->m_bVisible)
			{
				pColumn->m_nDefaultSize = pColumn->m_nAutoSize;
			}
		}

		pResizeColumn->m_nDefaultSize = nWidth;
		if (pResizeColumn->m_bVisible)
		{
			Resize (m_nTotalWidth, nColumn);
		}
		
		for (pos = m_lstColumns.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridColumnsItem* pColumn = m_lstColumns.GetNext (pos);
			ASSERT_VALID (pColumn);

			if (pColumn->m_bVisible)
			{
				pColumn->m_nDefaultSize = pColumn->m_nAutoSize;
			}
		}
	}
	else
	{
		pResizeColumn->m_nDefaultSize = nWidth;
	}
	
	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::GetColumnWidthMinMax (int nColumn, int& nMin, int& nMax) const
	// returns FALSE, if column's not found or column can't be resized
{
	ASSERT_VALID (this);
	
	if (!m_bAutoSize)
	{
		return FALSE;
	}

	POSITION posCol = m_lstColumns.FindIndex (nColumn);
	if (posCol == NULL)
	{
		return FALSE;
	}
	
	CBCGPGridColumnsItem* pColumnTest = m_lstColumns.GetAt (posCol);
	ASSERT_VALID (pColumnTest);
	
	if (!CanChangeWidth (nColumn))
	{
		nMin = nMax = pColumnTest->m_nDefaultSize;
		return FALSE;
	}

	int nSumMin = 0;

	int i = 0;
	for (POSITION pos = m_lstColumns.GetHeadPosition (); pos != NULL; i++)
	{
		CBCGPGridColumnsItem* pColumn = m_lstColumns.GetNext (pos);
		ASSERT_VALID (pColumn);
		
		if (pColumn->m_bVisible)
		{
			if (CanChangeWidth (i) && i > nColumn)
			{
				// Can sizing
				nSumMin += GetMinWidth (i);
			}
			else if (i != nColumn)
			{
				// Locked size
				nSumMin += pColumn->m_nDefaultSize;
			}
		}
	}

	nMin = GetMinWidth (nColumn);
	nMax = m_nTotalWidth - nSumMin;
	
	if (nMin >= nMax)
	{
		nMax = nMin;
		return FALSE;
	}
	
	return TRUE;		
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::GetColumnLocked (int nCol) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		return pColumnsItem->m_bLocked;
	}

	ASSERT (FALSE);
	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetColumnLocked (int nCol, BOOL bLocked)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		pColumnsItem->m_bLocked = bLocked;
		return TRUE;
	}

	ASSERT (FALSE);
	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::GetColumnVisible (int nCol) const
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		return pColumnsItem->m_bVisible;
	}

	ASSERT (FALSE);
	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::SetColumnVisible (int nCol, BOOL bVisible)
{
	ASSERT_VALID (this);

	POSITION pos = m_lstColumns.FindIndex (nCol);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumnsItem);

		if (pColumnsItem->m_bVisible != bVisible)
		{
			pColumnsItem->m_bVisible = bVisible;

			if (m_arrColumnOrder.GetSize () > 0)
			{
				// -------------------------------
				// Add a column to the order array
				// -------------------------------
				if (bVisible)
				{
					m_arrColumnOrder.Add (nCol);
				}

				// ------------------------------------
				// Remove a column from the order array
				// ------------------------------------
				else
				{
					int iToDel = -1;
					for (int i = 0; i < m_arrColumnOrder.GetSize (); i++)
					{
						if (m_arrColumnOrder [i] == nCol)
						{
							iToDel = i;
							break;
						}
					}

					if (iToDel != -1)
					{
						m_arrColumnOrder.RemoveAt (iToDel);
					}
				}
			}
		}

		return TRUE;
	}

	ASSERT (FALSE);
	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::CanChangeWidth (int nColumn) const // can't change width for icons etc
{
	return !GetColumnLocked (nColumn);
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::BestFitColumn (int nColumn) const
{
	if (m_pWndList != NULL)
	{
		ASSERT_VALID (m_pWndList);

		/*
		// TODO: best fit column
		int nAutoSize = m_pWndList->GetColumnAutoSize (nColumn);
		if (nAutoSize >= 0)
		{
			return nAutoSize;
		}
		*/
	}

	POSITION pos = m_lstColumns.FindIndex (nColumn);
	if (pos != NULL)
	{
		CBCGPGridColumnsItem* pColumn = m_lstColumns.GetAt (pos);
		ASSERT_VALID (pColumn);

		return pColumn->m_nDefaultSize;
	}

	ASSERT (FALSE);
	return 0;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::GetMinWidth (int /*nColumn*/) const
{
	if (m_pWndList != NULL)
	{
		ASSERT_VALID (m_pWndList);

		return m_pWndList->GetRowHeight ();
	}

	ASSERT (FALSE);
	return 0;
}

//*******************************************************************************************
void CBCGPGridColumnsInfo::GetColumnRect (int nColumn, CRect& rect) const
{
	if (m_pWndList == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	rect.SetRectEmpty ();

	BOOL bLast = (nColumn >= m_lstColumns.GetCount ());

	if (!bLast && !GetColumnVisible (nColumn))
	{
		return;
	}

	CRect rectColumn = m_pWndList->m_rectHeader;
	int x = rectColumn.left;

	BOOL bFirst = TRUE;

	int nPos = Begin ();
	while (nPos != End ())
	{
		int iColumn = Next (nPos);
		if (iColumn == -1)
		{
			return;	// column not found - hidden column
		}

		ASSERT (iColumn >= 0);
		ASSERT (iColumn < GetColumnCount ());

		rectColumn.left = x;
		rectColumn.right = rectColumn.left + GetColumnWidth (iColumn);

		if (bFirst)
		{
			rectColumn.right += 
				m_pWndList->GetHierarchyOffset () + 
				m_pWndList->GetExtraHierarchyOffset ();
			bFirst = FALSE;
		}

		if (iColumn == nColumn)
		{
			rect = rectColumn;
			rect.OffsetRect (-m_pWndList->m_nHorzScrollOffset, 0);
			return;
		}

		x = rectColumn.right;
	}

	ASSERT (bLast);

	rectColumn.left = x - m_pWndList->m_nHorzScrollOffset;
	rectColumn.right = m_pWndList->m_rectHeader.right + 2;
	rect = rectColumn;
}
//********************************************************************************
int CBCGPGridColumnsInfo::HitTestColumn (CPoint point, BOOL bDelimiter, int nDelta,
										 CBCGPGridColumnsInfo::ClickArea* pnArea) const
{
	if (m_pWndList == NULL)
	{
		ASSERT (FALSE);
		return -1;
	}

	if (!m_pWndList->m_rectHeader.PtInRect (point))
	{
		return -1;
	}

	int x = -m_pWndList->m_nHorzScrollOffset;
	BOOL bFirst = TRUE;

	int nPos = Begin ();
	while (nPos != End ())
	{
		int iColumn = Next (nPos);
		if (iColumn == -1)
		{
			break;	// no more visible columns
		}

		ASSERT (iColumn >= 0);
		ASSERT (iColumn < GetColumnCount ());

		int nColumnWidth = GetColumnWidth (iColumn);

		if (bFirst)
		{
			nColumnWidth += 
				m_pWndList->GetHierarchyOffset () + 
				m_pWndList->GetExtraHierarchyOffset ();
			bFirst = FALSE;
		}

		if (point.x <= x + nColumnWidth + nDelta)
		{
			if ((abs (point.x - (x + nColumnWidth)) <= nDelta))
			{
				if (pnArea != NULL)
				{
					*pnArea = CBCGPGridColumnsInfo::ClickDivider;
				}
			}
			else
			{
				if (bDelimiter)
				{
					return -1;
				}

				if (pnArea != NULL)
				{
					CRect rectColumn = m_pWndList->m_rectHeader;
					rectColumn.left = x;
					rectColumn.right = x + nColumnWidth;

					if (m_pWndList->GetHeaderMenuButtonRect (rectColumn, iColumn).PtInRect (point))
					{
						*pnArea = CBCGPGridColumnsInfo::ClickHeaderButton;
					}
					else
					{
						*pnArea = CBCGPGridColumnsInfo::ClickHeader;
					}
				}
			}

			return iColumn;
		}

		x += nColumnWidth;
	}

	return -1;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::Begin () const
{
	return 0;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::End () const
{
	// specified column order
	if (m_arrColumnOrder.GetSize () > 0)
	{
		return (int) m_arrColumnOrder.GetSize ();
	}

	// default column order
	else
	{
		return (int) m_lstColumns.GetCount ();
	}
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::Next (int& i) const
// Returns next visible column.
// Return value specifies the index in CBCGPGridColumnsInfo::m_lstColumns if succeed, 
// or -1 if visible column is not found.
{
	// specified column order
	if (m_arrColumnOrder.GetSize () > 0)
	{
        ASSERT (i < (int) m_arrColumnOrder.GetSize ());

        int nColumn = m_arrColumnOrder[i++];
        ASSERT (GetColumnVisible (nColumn));
        
        return nColumn;
	}

	// default column order
	else
	{
        ASSERT (i < (int) m_lstColumns.GetCount ());

        POSITION pos = m_lstColumns.FindIndex (i);

        BOOL bVisible;

        do
        {
            if (pos == NULL || i == (int) m_lstColumns.GetCount ())
            {
                return -1;  // no more visible columns
            }

            ASSERT (i < m_lstColumns.GetCount ());
            ASSERT (pos != NULL);

	        CBCGPGridColumnsItem* pColumnsItem = m_lstColumns.GetAt (pos);
	        ASSERT_VALID (pColumnsItem);

            bVisible = pColumnsItem->m_bVisible;

            ++i;
            m_lstColumns.GetNext (pos);
        }
        while (!bVisible); // skip hidden columns

        int nColumn = i - 1;

        return nColumn;
	}
}
//*******************************************************************************************
BOOL CBCGPGridColumnsInfo::ChangeColumnOrder (int nNewPos, int nColumn)
{
	ASSERT_VALID (this);

	if (!GetColumnVisible (nColumn))
	{
		return FALSE;
	}
	
	// -----------------
	// Get column order:
	// -----------------
	const int nColumnCount = GetColumnCount (TRUE);
	int* aColumnsOrder = new int [nColumnCount];
	memset (aColumnsOrder, 0, nColumnCount * sizeof (int));

	if (!GetColumnOrderArray ((LPINT) aColumnsOrder, nColumnCount))
	{
		ASSERT (FALSE);	// error getting columns order

		delete [] aColumnsOrder;
		return FALSE;
	}

	ASSERT (nNewPos >= 0);
	ASSERT (nNewPos < nColumnCount);

	// -----------------------------------------------
	// Move value (nColumn) saved in m_arrColumnOrder 
	// to m_arrColumnOrder[nNewPos]:
	// -----------------------------------------------
	m_arrColumnOrder.RemoveAll ();
	m_arrColumnOrder.SetSize (nColumnCount);

	int iSource = 0;
	int iTarget = 0;
	while (iTarget < nColumnCount && iSource <= nColumnCount)
	{
		if (iTarget == nNewPos)
		{
			m_arrColumnOrder[iTarget++] = nColumn;
		}
		else
		{
			if (aColumnsOrder[iSource] == nColumn)
			{
				iSource++;
			}
			else
			{
				m_arrColumnOrder[iTarget++] = aColumnsOrder[iSource++];
			}
		}
	}

	delete [] aColumnsOrder;

	return TRUE;
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::IndexToOrder (int nColumn) const
// Returns -1, if column is hidden or not found.
{
	ASSERT_VALID (this);

	if (nColumn < 0 || nColumn >= m_lstColumns.GetCount ())
	{
		// column not found
		ASSERT (FALSE);
		return -1;
	}

	if (!GetColumnVisible (nColumn))
	{
		return -1; // column is hidden
	}
	
	int nCount = 0;	// count visible columns

	int nPos = Begin ();
	while (nPos != End ())
	{
		int iColumn = Next (nPos);
		if (iColumn == -1)
		{
			break;	// no more visible columns
		}

		if (iColumn == nColumn)
		{
			return nCount;
		}

		nCount++;
	}

	return -1; // column is not found
}
//*******************************************************************************************
int CBCGPGridColumnsInfo::OrderToIndex (int nPosition) const
// Returns -1, if column is not found.
{
	ASSERT_VALID (this);

	if (nPosition < 0 || nPosition >= GetColumnCount (TRUE))
	{
		// column not found
		ASSERT (FALSE);
		return -1;
	}

	int nCount = 0;	// count visible columns

	int nPos = Begin ();
	while (nPos != End ())
	{
		int iColumn = Next (nPos);
		if (iColumn == -1)
		{
			break;	// no more visible columns
		}

		if (nCount == nPosition)
		{
			ASSERT (iColumn >= 0);
			ASSERT (iColumn < GetColumnCount ());

			return iColumn;
		}

		nCount++;
	}

	return -1;
}
//******************************************************************************************
int CBCGPGridColumnsInfo::GetFirstVisibleColumn () const
{
	int nPos = Begin ();
	if (nPos != End ())
	{
		return Next (nPos);
	}

	return -1;
}
//******************************************************************************************
int CBCGPGridColumnsInfo::GetLastVisibleColumn () const
{
	int iLastColumn = -1;

	int nPos = Begin ();
	while (nPos != End ())
	{
		int iColumn = Next (nPos);
		if (iColumn == -1)
		{
			break;	// no more visible columns
		}

		iLastColumn = iColumn;
	}

	return iLastColumn;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPHeaderItemDragWnd window

class CBCGPHeaderItemDragWnd : public CWnd
{
// Construction
public:
	CBCGPHeaderItemDragWnd();

// Attributes
public:
	CBCGPGridCtrl*		m_pWndGrid;
	int					m_nItem;
	BOOL				m_bDrop;

	static CString		m_strClassName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPHeaderItemDragWnd)
	public:
	virtual BOOL Create(CBCGPGridCtrl* pGrid, int nItem);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPHeaderItemDragWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPHeaderItemDragWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CBCGPHeaderItemDropWnd window

class CBCGPHeaderItemDropWnd : public CWnd
{
// Construction
public:
	CBCGPHeaderItemDropWnd();

// Attributes
public:

	static CString	m_strClassName;

// Operations
public:
	void Show (CPoint point);
	void Hide ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPHeaderItemDropWnd)
	public:
	virtual BOOL Create(int nColumnHeight);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPHeaderItemDropWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPHeaderItemDropWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridCtrl

IMPLEMENT_DYNAMIC(CBCGPGridCtrl, CWnd)

#define BCGPGRID_GROUPBYBOX_VMARGIN			7
#define BCGPGRID_GROUPBYBOX_HMARGIN			9
#define BCGPGRID_GROUPBYBOX_VSPACING		10
#define BCGPGRID_GROUPBYBOX_HSPACING		4
#define BCGPGRID_GROUPBYBOX_COLUMNWIDTH		49

HCURSOR CBCGPGridCtrl::m_hcurDeleteColumn = NULL;
HCURSOR CBCGPGridCtrl::m_hcurNoDropColumn = NULL;
BOOL	CBCGPGridCtrl::m_bUseSystemFont = TRUE;

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridCtrl notification messages:

UINT BCGM_GRID_ITEM_CHANGED = ::RegisterWindowMessage (_T("BCGM_GRID_ITEM_CHANGED"));
UINT BCGM_GRID_SEL_CHANGED = ::RegisterWindowMessage (_T("BCGM_GRID_SEL_CHANGED"));
UINT BCGM_GRID_ITEM_DBLCLICK = ::RegisterWindowMessage (_T("BCGM_GRID_ITEM_DBLCLICK"));
UINT BCGM_GRID_ON_HIDE_COLUMNCHOOSER = ::RegisterWindowMessage (_T("BCGM_GRID_ON_HIDE_COLUMNCHOOSER"));

CBCGPGridCtrl::CBCGPGridCtrl()
{
	m_nRowsBlockSize = 10; // Specifies the memory-allocation granularity for internal containers
	InitConstructor ();
}

CBCGPGridCtrl::CBCGPGridCtrl(int nMemBlockSize) :
	m_lstItems (nMemBlockSize), m_lstTerminalItems (nMemBlockSize), m_lstGroupedItems (nMemBlockSize),
	m_lstAutoGroups (nMemBlockSize), m_lstOldAutoGroups (nMemBlockSize)
{
	m_nRowsBlockSize = nMemBlockSize;
	InitConstructor ();
}

void CBCGPGridCtrl::InitConstructor ()
{
	m_bScrollVert = TRUE;
	m_bScrollHorz = TRUE;
	m_hFont = NULL;
	m_nEditLeftMargin = 0;
	m_bHeader = TRUE;
	m_dwHeaderFlags = (DWORD)-1;
	m_bGroupByBox = FALSE;
	m_bVirtualMode = FALSE;
	m_nVirtualRows = 0;
	m_pfnCallback = NULL;
	m_lParamCallback = 0;
	m_rectList.SetRectEmpty ();
	m_rectClip.SetRectEmpty ();
	m_rectTrackHeader.SetRectEmpty ();
	m_rectTrackHeaderLeft.SetRectEmpty ();
	m_rectHeader.SetRectEmpty ();

	m_pImagesHeader = NULL;
	m_pImagesHeaderBtn = NULL;
	m_pImages = NULL;

	m_nRowHeight = 0;
	m_nLargeRowHeight = 0;
	m_bAllowRowExtraHeight = FALSE;
	m_nGroupByBoxHeight = 0;
	m_nGridHeaderHeight = 0;
	m_nGridFooterHeight = 0;
	m_nTreeColumn = -1;
	m_nVertScrollOffset = 0;
	m_nVertScrollTotal = 0;
	m_nVertScrollPage = 0;
	m_nHorzScrollOffset = 0;
	m_nHorzScrollTotal = 0;
	m_nHorzScrollPage = 0;
	m_nFirstVisibleItem = -1;

	m_pDefaultItemRTC = NULL;

	m_pSelRow = NULL;
	m_pSelItem = NULL;
	m_pLastSelRow = NULL;
	m_pLastSelItem = NULL;
	m_pSetSelItem = NULL;
	m_bFocused = FALSE;
	m_bTracking = FALSE;
	m_nTrackColumn = 0;

	m_bMouseTracked = FALSE;
	m_bHeaderItemHovered = FALSE;

	m_bUpdateItemData = TRUE;

	m_strTrue = _T("True");
	m_strFalse = _T("False");

	m_cListDelimeter = _T(',');

	m_bReadOnly = FALSE;
	m_bSelectionBorder = TRUE;
	m_bEditFirstClick = TRUE;
	m_bHighlightGroups = FALSE;
	m_bControlBarColors = FALSE;
	m_bShowDragContext = TRUE;
	m_bSingleSel = FALSE;
	m_bWholeRowSel = FALSE;
	m_bMarkSortedColumn = FALSE;
	m_bDrawFocusRect = FALSE;
	m_bShowInPlaceToolTip = TRUE;
	m_bRowMarker = TRUE;
	m_bSelectionBorderActiveItem = TRUE;
	m_bHighlightActiveItem = FALSE;
	m_bGridItemBorders = TRUE;	// TODO FALSE by default
	m_bSelecting = FALSE;
	m_bClickTimer = FALSE;
	m_ptClickOnce = CPoint (0, 0);
	m_bIsFirstClick = FALSE;
	m_bIsButtonClick = FALSE;
	m_bRebuildTerminalItems = TRUE;
	m_bNoUpdateWindow = FALSE;

	m_clrSortedColumn = (COLORREF)-1;

	m_hPrinterFont = NULL;
	m_hPrinterBoldFont = NULL;
	m_hMirrorFont = NULL;
	m_hMirrorBoldFont = NULL;
	m_bIsPrinting = FALSE;
	m_pPrintDC = NULL;
	
	m_clrPrintBorder = (COLORREF)-1;
	m_clrPrintHeader = (COLORREF)-1;
	m_clrPrintHeaderBG = (COLORREF)-1;
	m_clrPrintGroup = (COLORREF)-1;
	m_clrPrintGroupBG = (COLORREF)-1;
	m_clrPrintLeftOffset = (COLORREF)-1;
	m_clrPrintBG = (COLORREF)-1;
	m_clrPrintLine = (COLORREF)-1;
	m_clrPrintText = (COLORREF)-1;

	m_bInAdjustLayout = FALSE;

	m_pWndHeaderDrag = NULL;
	m_pWndHeaderDrop = NULL;

	m_pColumnChooser = NULL;
	m_rectColumnChooser.SetRectEmpty ();
	m_bColumnChooserVisible = FALSE;

	m_bDragHeaderItems = TRUE;
	m_nDraggedColumn = -1;
	m_rectStartDrag.SetRectEmpty ();
	m_ptStartDrag = CPoint (0, 0);
	m_bDragGroupItem = FALSE;
	m_bDragFromChooser = FALSE;
	
	m_bFilter = FALSE;
	m_pfnFilterCallback = NULL;
	m_lFilterParam = 0;

	m_aSortOrder = NULL;
	m_nSortCount = 0;
	m_nGroupCount = 0;

	SetScrollBarsStyle (CBCGPScrollBar::BCGP_SBSTYLE_DEFAULT);
}

CBCGPGridCtrl::~CBCGPGridCtrl()
{
	if (m_pColumnChooser->GetSafeHwnd ())
	{
		m_pColumnChooser->DestroyWindow ();
		m_pColumnChooser = NULL;
	}

	if (m_pWndHeaderDrag != NULL)
	{
		m_pWndHeaderDrag->DestroyWindow ();
		m_pWndHeaderDrag = NULL;
	}

	if (m_pWndHeaderDrop != NULL)
	{
		m_pWndHeaderDrop->DestroyWindow ();
		m_pWndHeaderDrop = NULL;
	}
}

BEGIN_MESSAGE_MAP(CBCGPGridCtrl, CWnd)
	//{{AFX_MSG_MAP(CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CANCELMODE()
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_WM_STYLECHANGED()
	ON_CBN_SELENDOK(BCGPGRIDCTRL_ID_INPLACE, OnSelectCombo)
	ON_CBN_CLOSEUP(BCGPGRIDCTRL_ID_INPLACE, OnCloseCombo)
	ON_EN_KILLFOCUS(BCGPGRIDCTRL_ID_INPLACE, OnEditKillFocus)
	ON_CBN_KILLFOCUS(BCGPGRIDCTRL_ID_INPLACE, OnComboKillFocus)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Virtual mode methods:

void CBCGPGridCtrl::EnableVirtualMode (BCGPGRID_CALLBACK pCallback/* = NULL*/, 
									   LPARAM lParam/* = 0*/)
{
	m_pfnCallback = pCallback;
	m_lParamCallback = lParam;

	if (m_bVirtualMode)
	{
		// Already in virtual mode
		return;
	}

	if (!m_lstItems.IsEmpty ())
	{
		// you cannot set virtual mode after items were added
		ASSERT (FALSE);
		return;		
	}

	m_bVirtualMode = TRUE;
	m_nVirtualRows = 0;
}
//******************************************************************************************
void CBCGPGridCtrl::SetVirtualRows (int nRowsNum)
{
	if (!m_bVirtualMode)
	{
		ASSERT (FALSE);
		return;
	}

	RemoveAll ();

	m_nVirtualRows = nRowsNum;

	AdjustLayout ();
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridCtrl message handlers

void CBCGPGridCtrl::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState ();
	if (pThreadState->m_pWndInit == NULL)
	{
		Init ();
	}
}
//******************************************************************************************
int CBCGPGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init ();
	return 0;
}
//******************************************************************************************
void CBCGPGridCtrl::Init ()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (globalData.m_hcurStretch == NULL)
	{
		globalData.m_hcurStretch = AfxGetApp ()->LoadCursor (AFX_IDC_HSPLITBAR);
	}

	if (globalData.m_hcurStretchVert == NULL)
	{
		globalData.m_hcurStretchVert = AfxGetApp ()->LoadCursor (AFX_IDC_VSPLITBAR);
	}

	if (m_hcurDeleteColumn == NULL)
	{
		CBCGPLocalResource locaRes;
		m_hcurDeleteColumn = AfxGetApp ()->LoadCursor (IDC_BCGBARRES_DELETE_COLUMN);
	}

	if (m_hcurNoDropColumn == NULL)
	{
		m_hcurNoDropColumn = AfxGetApp ()->LoadCursor (AFX_IDC_NODROPCRSR);
	}

	InitColors ();

	m_Columns.SetOwnerList (this);

	m_wndScrollVert.Create (WS_CHILD | WS_VISIBLE | SBS_VERT, rectDummy, this, ID_SCROLL_VERT);
	m_wndScrollHorz.Create (WS_CHILD | WS_VISIBLE | SBS_HORZ, rectDummy, this, ID_SCROLL_HORZ);

	m_ToolTip.Create (this);

	CWnd* pWndParent = GetParent ();
	m_bControlBarColors = pWndParent == NULL ||
		!pWndParent->IsKindOf (RUNTIME_CLASS (CDialog));

	AdjustLayout ();
	UpdateFonts ();
	CalcEditMargin ();
}
//******************************************************************************************
void CBCGPGridCtrl::InitColors ()
{
	m_clrSortedColumn = visualManager->GetGridItemSortedColor (this);

	m_brBackground.DeleteObject ();

	if (m_ColorData.m_clrBackground != (COLORREF)-1)
	{
		m_brBackground.CreateSolidBrush (m_ColorData.m_clrBackground);
	}

	m_brGroupBackground.DeleteObject ();

	if (m_ColorData.m_GroupColors.m_clrBackground != (COLORREF)-1)
	{
		m_brGroupBackground.CreateSolidBrush (m_ColorData.m_GroupColors.m_clrBackground);
	}

	m_brSelBackground.DeleteObject ();

	if (m_ColorData.m_SelColors.m_clrBackground != (COLORREF)-1)
	{
		m_brSelBackground.CreateSolidBrush (m_ColorData.m_SelColors.m_clrBackground);
	}
	
	m_penHLine.DeleteObject ();

	m_penHLine.CreatePen (PS_SOLID, 1, 
		m_ColorData.m_clrHorzLine != (COLORREF)-1 ? m_ColorData.m_clrHorzLine : globalData.clrBtnFace);

	m_penVLine.DeleteObject ();

	m_penVLine.CreatePen (PS_SOLID, 1, 
		m_ColorData.m_clrVertLine != (COLORREF)-1 ? m_ColorData.m_clrVertLine : globalData.clrBtnFace);
}
//*****************************************************************************************
void CBCGPGridCtrl::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
	
	InitColors ();
	RedrawWindow ();
}
//*****************************************************************************************
void CBCGPGridCtrl::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL || m_bInAdjustLayout)
	{
		return;
	}

	m_bInAdjustLayout = TRUE;
	
	if (m_bRebuildTerminalItems)
	{
		ReposItems ();
	}

	SetRowHeight ();

	CRect rectClient;
	GetClientRect (rectClient);

	m_nGroupByBoxHeight = OnGetGroupByBoxRect (NULL, rectClient).Height ();
	m_rectHeader = OnGetHeaderRect (NULL, rectClient);

	m_rectList = rectClient;
	m_rectList.top += m_nGroupByBoxHeight;
	m_rectList.top += m_rectHeader.Height ();
	m_rectList.top += m_nGridHeaderHeight;
	if (m_rectList.top + m_nRowHeight < m_rectList.bottom)
	{
		m_rectList.bottom = 
			max (m_rectList.top + m_nRowHeight, m_rectList.bottom - m_nGridFooterHeight);
	}

	m_Columns.Resize (m_rectList.Width ());

	int cxScroll = m_bScrollVert ? ::GetSystemMetrics (SM_CXVSCROLL) : 0;
	int cyScroll = m_bScrollHorz ? ::GetSystemMetrics (SM_CYHSCROLL) : 0;
	SetScrollSizes ();

	CSize szScroll (0, 0);
	if (m_nVertScrollTotal > 0)
	{
		m_rectList.right -= cxScroll;
		m_rectHeader.right -= cxScroll;
		szScroll.cy = rectClient.Height ();

		if (m_Columns.IsAutoSize ())
		{
			m_Columns.Resize (m_rectList.Width ());
		}
	}
	if (m_nHorzScrollTotal > 0)
	{
		m_rectList.bottom -= cyScroll;
		szScroll.cx = max (0, m_rectList.Width ());
		szScroll.cy = max (0, szScroll.cy - cyScroll);
	}

	if (szScroll.cy > 0)
	{
		m_wndScrollVert.SetWindowPos (NULL, m_rectList.right, rectClient.top,
			cxScroll, szScroll.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		m_wndScrollVert.SetWindowPos (NULL, 0, 0,
			0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
	if (szScroll.cx > 0)
	{
		m_wndScrollHorz.SetWindowPos (NULL, m_rectList.left, rectClient.bottom - cyScroll,
			szScroll.cx, cyScroll, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		m_wndScrollHorz.SetWindowPos (NULL, 0, 0,
			0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	OnResizeColumns ();

	ReposItems ();

	OnPosSizeChanged ();
	m_rectTrackSel = OnGetSelectionRect ();

	CBCGPGridRow* pCurSel = GetCurSel ();
	if (pCurSel != NULL)
	{
		pCurSel->AdjustButtonRect ();
	}

	RedrawWindow ();

	m_bInAdjustLayout = FALSE;
}
//******************************************************************************************
void CBCGPGridCtrl::OnPosSizeChanged ()
{
	ASSERT_VALID (this);

	UpdateMergedItems ();
}
//******************************************************************************************
void CBCGPGridCtrl::SetRowHeight ()
{
	if (m_bIsPrinting)
	{
		ASSERT_VALID (m_pPrintDC);

		// map to printer metrics
		HDC hDCFrom = ::GetDC(NULL);
		int nYMul = m_pPrintDC->GetDeviceCaps(LOGPIXELSY);	// pixels in print dc
		int nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
		::ReleaseDC(NULL, hDCFrom);

		TEXTMETRIC tm;
		m_pPrintDC->GetTextMetrics (&tm);
		m_PrintParams.m_nRowHeight = tm.tmHeight + ::MulDiv (2 * TEXT_VMARGIN, nYMul, nYDiv);
		m_PrintParams.m_nLargeRowHeight = m_PrintParams.m_nRowHeight;
	}
	else
	{
		CClientDC dc (this);
		HFONT hfontOld = SetCurrFont (&dc);

		TEXTMETRIC tm;
		dc.GetTextMetrics (&tm);
		m_nRowHeight = tm.tmHeight + 2 * TEXT_VMARGIN;
		m_nLargeRowHeight = m_nRowHeight;

		::SelectObject (dc.GetSafeHdc (), hfontOld);
	}
}
//******************************************************************************************
void CBCGPGridCtrl::ReposItems ()
{
	ASSERT_VALID (this);

	m_nFirstVisibleItem = -1;

	if (m_bVirtualMode)
	{
		// iterate through cached items
		for (POSITION pos = m_CachedItems.m_lstCache.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridCachePageInfo& cpi = m_CachedItems.m_lstCache.GetNext (pos);
			ASSERT (cpi.pArrCachePage->GetSize() == cpi.nSize);
			for (int i = 0; i < cpi.nSize; i++)
			{
				CBCGPGridRow* pItem = cpi.pArrCachePage->GetAt (i);
				if (pItem != NULL)
				{
					ASSERT_VALID (pItem);

					int nIndex = cpi.nFirst + i;
					int y = m_rectList.top - 1 + nIndex * m_nRowHeight - m_nVertScrollOffset;
					m_idCur.m_nRow = nIndex;
					pItem->Repos (y);

					if (m_nFirstVisibleItem == -1 &&
						pItem->GetRect ().bottom >= m_rectList.top)
					{
						m_nFirstVisibleItem = nIndex;
					}
				}
			}
		}

		return;
	}

	if (m_bRebuildTerminalItems)
	{
		// Cleanup selection
		CBCGPGridItemID id;
		SetCurSel (id, SM_NONE, FALSE);
		m_pLastSelItem = NULL;

		CleanUpAutoGroups (AG_COPY_AUTOGROUPS);
		m_lstTerminalItems.RemoveAll ();
	}

	m_ToolTip.Hide ();

	m_idCur.m_nRow = 0;
	m_idCur.m_nColumn = -1;

	int y = m_rectList.top - m_nVertScrollOffset - 1;
	int nIndex = 0;

	if (!IsSortingMode () && !IsGrouping ())
	{
		m_bRebuildTerminalItems = FALSE;

		for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL;
			nIndex++)
		{
			CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
			ASSERT_VALID (pItem);

			pItem->Repos (y);

			if (m_nFirstVisibleItem == -1 &&
				pItem->GetRect ().bottom >= m_rectList.top)
			{
				m_nFirstVisibleItem = nIndex;
			}
		}

		return;
	}

	if (m_bRebuildTerminalItems)
	{
		// Get sorted list of terminal items:
		DoRebuildTerminalItems ();
	}

	m_bRebuildTerminalItems = FALSE;

	m_nFirstVisibleItem = -1;
	nIndex = 0;

	for (POSITION pos = m_lstTerminalItems.GetHeadPosition (); pos != NULL;
		nIndex++)
	{
		CBCGPGridRow* pItem = m_lstTerminalItems.GetNext (pos);
		ASSERT_VALID (pItem);

		pItem->Repos (y);

		if (m_nFirstVisibleItem == -1 &&
			pItem->GetRect ().bottom >= m_rectList.top)
		{
			m_nFirstVisibleItem = nIndex;
		}
	}
}
//******************************************************************************************
void CBCGPGridCtrl::ShiftItems (int dx, int dy)
{
	m_ToolTip.Deactivate ();

	m_nFirstVisibleItem = -1;

	if (m_bVirtualMode)
	{
		// iterate through cached items
		for (POSITION pos = m_CachedItems.m_lstCache.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridCachePageInfo& cpi = m_CachedItems.m_lstCache.GetNext (pos);
			ASSERT (cpi.pArrCachePage->GetSize() == cpi.nSize);
			for (int i = 0; i < cpi.nSize; i++)
			{
				CBCGPGridRow* pItem = cpi.pArrCachePage->GetAt (i);
				if (pItem != NULL)
				{
					ASSERT_VALID (pItem);

					pItem->Shift (dx, dy);

					if (m_nFirstVisibleItem == -1 &&
						pItem->GetRect ().bottom >= m_rectList.top)
					{
						int nIndex = cpi.nFirst + i;
						m_nFirstVisibleItem = nIndex;
					}
				}
			}
		}

		return;
	}

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	POSITION pos = NULL;

	int nIndex = 0;

	for (pos = lst.GetHeadPosition (); pos != NULL; nIndex++)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		if (pItem == NULL)
		{
			continue;
		}

		ASSERT_VALID (pItem);

		pItem->Shift (dx, dy);

		if (m_nFirstVisibleItem == -1 &&
			pItem->GetRect ().bottom >= m_rectList.top)
		{
			m_nFirstVisibleItem = nIndex;
		}
	}
}
//******************************************************************************************
// Get sorted list of terminal items:
void CBCGPGridCtrl::DoRebuildTerminalItems ()
{
	if (m_Columns.GetGroupColumnCount () + m_Columns.GetSortColumnCount () <= 0)
	{
		return;
	}

#define BCGPGRID_QSORT
#ifndef BCGPGRID_QSORT
	//----------------------------
	// old version: insertion sort
	//----------------------------
		for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL;)
		{
			CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
			ASSERT_VALID (pItem);

			pItem->m_nIdRow = -1;

			if (IsGrouping ())
			{
				pItem->AddGroupedItem (m_lstTerminalItems);
			}
			else
			{
				pItem->AddTerminalItem (m_lstTerminalItems);
			}
		}

		CleanUpAutoGroups (AG_CLEANUP_OLDAUTOGROUPS_ONLY);
#else
	//-------------------
	// Quick-sort version
	//-------------------
		CBCGPSortableArray arrSort;
		arrSort.SetSize (m_lstItems.GetCount ());

		//-------------
		// copy buffer:
		//-------------
		int i = 0;
		for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL; i++)
		{
			CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
			ASSERT_VALID (pItem);

			arrSort[i] = (pItem);
		}
		
		//----------------
		// Get sort order:
		//----------------
		const int nGroupCount = m_Columns.GetGroupColumnCount ();
		const int nSortCount = m_Columns.GetGroupColumnCount () + m_Columns.GetSortColumnCount ();
		int* aSortOrder = new int [nSortCount];
		memset (aSortOrder, 0, nSortCount * sizeof (int));

		if (!m_Columns.GetGroupingColumnOrderArray ((LPINT) aSortOrder, nSortCount))
		{
			ASSERT (FALSE);	// error getting sort order

			delete [] aSortOrder;
			return;
		}
		SetSortOrder (aSortOrder, nSortCount, nGroupCount);

		//-----
		// Sort
		//-----
		arrSort.Sort ();

		//--------------------
		// Copy results, group
		//--------------------
		CArray <POSITION, POSITION> arrPositions; // first item in group
		arrPositions.SetSize (nGroupCount);
		for (i = 0; i < arrPositions.GetSize (); i++)
		{
			arrPositions [i] = NULL;
		}

		CList<CBCGPGridRow*, CBCGPGridRow*>& lstItems = m_lstTerminalItems;
		for (i = 0; i < arrSort.GetSize (); i++)
		{
			CBCGPGridRow* pRowLast = (lstItems.IsEmpty ()) ? NULL : lstItems.GetTail ();

			POSITION posInserted = lstItems.AddTail (arrSort [i]);
			int nParentLevel = -1;
			POSITION posFirstItemInGroup = NULL;

			if (pRowLast == NULL) // list empty
			{
				// nParentLevel == -1 - create full hierarchy
				// this is the first item in group for all levels
				for (int j = 0; j < nGroupCount; j++)
				{
					arrPositions [j] = posInserted;
				}
				// posFirstItemInGroup == NULL
			}
			else
			{
				ASSERT_VALID (pRowLast);

				// find out nParentLevel:
				BOOL bTryNextSortedColumn = TRUE;
				int iLevel = 0;			// 0, ..., nGroupCount - 1
				while (bTryNextSortedColumn && iLevel < nGroupCount)
				{
					int iSortedColumn = aSortOrder [iLevel];
					int nCompare = CompareGroup (pRowLast, arrSort [i], iSortedColumn);
					if (nCompare == 0) // the same group
					{
						nParentLevel = iLevel;
						iLevel++;
					}
					else
					{
						bTryNextSortedColumn = FALSE;
					}
				}

				// this is the first item in group for next levels
				for (int j = max (0, nParentLevel + 1); j < nGroupCount; j++)
				{
					arrPositions [j] = posInserted;
				}
				// get FirstItemInGroup for nParentLevel
				if (nParentLevel >= 0 && nParentLevel < nGroupCount)
				{
					posFirstItemInGroup = arrPositions [nParentLevel];
				}
			}

			POSITION posInsertBefore = posInserted;	// insert autogroups before item
			POSITION posGroup = arrSort [i]->InsertAutoGroupBefore (lstItems, posInsertBefore, posFirstItemInGroup, nParentLevel, arrSort [i]);

			// Add item as subitem for the group
			if (posGroup != NULL)
			{
				CBCGPGridRow* pGroup = lstItems.GetAt (posGroup);
				BOOL bIsAutoGroup = pGroup != NULL && pGroup->IsGroup () && (pGroup->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
				if (bIsAutoGroup)
				{
					ASSERT_VALID (pGroup);

					// Add item as subitem for the group
					pGroup->m_lstSubItems.AddTail (arrSort [i]);
					arrSort [i]->m_pParent = pGroup;
				}
			}
		}
		
		SetSortOrder (NULL, 0, 0);
		arrSort.RemoveAll ();
		delete [] aSortOrder;

		CleanUpAutoGroups (AG_CLEANUP_OLDAUTOGROUPS_ONLY);
#endif // BCGPGRID_QSORT
}
//******************************************************************************************
void CBCGPGridCtrl::SetSortOrder (int* aSortOrder, int nSortCount, int nGroupCount)
{
	ASSERT_VALID (this);
	m_aSortOrder = aSortOrder;
	m_nSortCount = nSortCount;
	m_nGroupCount = nGroupCount;
}
//******************************************************************************************
BOOL CBCGPGridCtrl::GetSortOrder (int*& aSortOrder, int& nSortCount, int& nGroupCount) const
{
	ASSERT_VALID (this);
	aSortOrder = m_aSortOrder;
	nSortCount = m_nSortCount;
	nGroupCount = m_nGroupCount;

	return (aSortOrder != NULL);
}
//****************************************************************************************
// Called from CBCGPSortableArray::Compare
int CBCGPGridCtrl::DoMultiColumnCompare (const CBCGPGridRow* pRow1, const CBCGPGridRow* pRow2)
{
	int* aSortOrder = NULL;
	int nSortCount = 0;
	int nGroupCount = 0;
	if (!GetSortOrder (aSortOrder, nSortCount, nGroupCount))
	{
		return 0;
	}

	if (nSortCount <= 0)
	{
		return 0;
	}

	ASSERT (aSortOrder != NULL);
	ASSERT (AfxIsValidAddress (aSortOrder, nSortCount * sizeof(int)));

	int nCompare = 0;

	BOOL bTryNextSortedColumn = TRUE;
	int iLevel = 0;			// 0, ..., nSortCount - 1
	while (bTryNextSortedColumn && iLevel < nSortCount)
	{
		int iSortedColumn = aSortOrder [iLevel];

		if (iLevel < nGroupCount) // grouping
		{
			nCompare = CompareGroup (pRow1, pRow2, iSortedColumn);
		}
		else	// compare items inside same group
		{
			nCompare = CompareItems (pRow1, pRow2, iSortedColumn);
		}
		if (nCompare == 0)
		{
			iLevel++;
		}
		else
		{
			bTryNextSortedColumn = FALSE;
		}
	}

	return nCompare;
}
//******************************************************************************************
void CBCGPGridCtrl::CleanUpAutoGroups (CBCGPGridCtrl::AUTOGROUP_CLEANUP_MODE nMode)
{
	ASSERT_VALID (this);

	if (nMode == AG_COPY_AUTOGROUPS)
	{
		// Make copy and remove all auto groups (do not destroy auto groups):
		m_lstOldAutoGroups.AddTail (&m_lstAutoGroups);

		for (POSITION pos = m_lstAutoGroups.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRow* pRow = m_lstAutoGroups.GetNext (pos);
			ASSERT_VALID (pRow);

			// Remove parent-child links:
			for (POSITION pos = pRow->m_lstSubItems.GetHeadPosition (); pos != NULL; )
			{
				CBCGPGridRow* pChildRow = pRow->m_lstSubItems.GetNext (pos);
				ASSERT_VALID (pChildRow);

				BOOL bIsAutoGroup = pChildRow->IsGroup () && 
					(pChildRow->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
				if (!bIsAutoGroup)
				{
					pChildRow->m_pParent = NULL;
				}
			}
		}

		m_lstAutoGroups.RemoveAll ();
	}

	if (nMode == AG_CLEANUP_OLDAUTOGROUPS_ONLY || nMode == AG_FULL_CLEANUP)
	{
		// Destroy old auto groups:
		for (POSITION pos = m_lstOldAutoGroups.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRow* pRow = m_lstOldAutoGroups.GetNext (pos);
			ASSERT_VALID (pRow);

			pRow->m_lstSubItems.RemoveAll ();
		}

		while (!m_lstOldAutoGroups.IsEmpty ())
		{
			delete m_lstOldAutoGroups.RemoveTail ();
		}
	}

	if (nMode == AG_FULL_CLEANUP)
	{
		// Destroy auto groups:
		for (POSITION pos = m_lstAutoGroups.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRow* pRow = m_lstAutoGroups.GetNext (pos);
			ASSERT_VALID (pRow);

			// Remove parent-child links:
			for (POSITION pos = pRow->m_lstSubItems.GetHeadPosition (); pos != NULL; )
			{
				CBCGPGridRow* pChildRow = pRow->m_lstSubItems.GetNext (pos);
				ASSERT_VALID (pChildRow);

				pChildRow->m_pParent = NULL;
			}

			pRow->m_lstSubItems.RemoveAll ();
		}

		while (!m_lstAutoGroups.IsEmpty ())
		{
			delete m_lstAutoGroups.RemoveTail ();
		}
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	EndEditItem (FALSE);

	AdjustLayout ();
}
//******************************************************************************************
LRESULT CBCGPGridCtrl::OnSetFont (WPARAM wParam, LPARAM /*lParam*/)
{
	m_hFont = (HFONT) wParam;

	UpdateFonts ();
	CalcEditMargin ();
	AdjustLayout ();

	return 0;
}
//******************************************************************************************
LRESULT CBCGPGridCtrl::OnGetFont (WPARAM, LPARAM)
{
	return (LRESULT) (m_hFont != NULL ? m_hFont : GetDefaultFont ());
}
//******************************************************************************************
void CBCGPGridCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CWnd::OnSettingChange(uFlags, lpszSection);

	if (uFlags == SPI_SETNONCLIENTMETRICS ||
		uFlags == SPI_SETWORKAREA ||
		uFlags == SPI_SETICONTITLELOGFONT)
	{
#ifndef _BCGPGRID_STANDALONE
		if (CBCGPToolBar::GetAllToolbars ().IsEmpty ())
#endif
		{
			globalData.UpdateFonts();
		}

		UpdateFonts ();
		CalcEditMargin ();
		AdjustLayout ();
	}
}
//******************************************************************************************
void CBCGPGridCtrl::UpdateFonts ()
{
	CleanUpFonts ();

	// Create bold font:
	CFont* pFont = CFont::FromHandle (
		m_hFont != NULL ? m_hFont : GetDefaultFont ());
	ASSERT_VALID (pFont);

	LOGFONT lf;
	memset (&lf, 0, sizeof (LOGFONT));

	pFont->GetLogFont (&lf);

	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect (&lf);
}
//******************************************************************************************
void CBCGPGridCtrl::CleanUpFonts ()
{
	// Clean up bold font:
	if (m_fontBold.GetSafeHandle () != NULL)
	{
		m_fontBold.DeleteObject ();
	}
}
//******************************************************************************************
void CBCGPGridCtrl::CalcEditMargin ()
{
	CEdit editDummy;
	editDummy.Create (WS_CHILD, CRect (0, 0, 100, 20), this, (UINT)-1);

	editDummy.SetFont (GetFont ());
	m_nEditLeftMargin = LOWORD (editDummy.GetMargins ());

	editDummy.DestroyWindow ();
}
//******************************************************************************************
HFONT CBCGPGridCtrl::GetDefaultFont ()
{
	return CBCGPGridCtrl::m_bUseSystemFont ? 
		(HFONT) ::GetStockObject (DEFAULT_GUI_FONT) : (HFONT) globalData.fontRegular.GetSafeHandle ();
}
//******************************************************************************************
HFONT CBCGPGridCtrl::SetCurrFont (CDC* pDC)
{
	ASSERT_VALID (pDC);

	if (pDC->IsPrinting ())
	{
		if (m_hPrinterFont != NULL)
		{
			return (HFONT) ::SelectObject (pDC->GetSafeHdc (), m_hPrinterFont);
		}
	}
	
	return (HFONT) ::SelectObject (pDC->GetSafeHdc (), 
		m_hFont != NULL ? m_hFont : GetDefaultFont ());
}
//******************************************************************************************
void CBCGPGridCtrl::OnPaint() 
{
	CPaintDC dcPaint (this); // device context for painting
	dcPaint.GetClipBox (m_rectClip);

	if (m_rectClip.IsRectEmpty ())
	{
		m_rectClip = m_rectList;
	}

	m_rectClip.top = max (m_rectClip.top, m_rectList.top);

	CRect rect;
	GetClientRect (rect);
	rect.right = m_rectList.right;
	rect.bottom = m_rectList.bottom + m_nGridFooterHeight;

	CRect rectGripper;
	GetClientRect (rectGripper);
	rectGripper.left = m_rectList.right;
	rectGripper.top = m_rectList.bottom;
	dcPaint.FillRect (rectGripper, 
		m_bControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);

	if (rect.IsRectEmpty ())
	{
		return;
	}

	CBCGPMemDC memDC (dcPaint, rect);
	CDC* pDC = &memDC.GetDC ();

	m_clrGray = visualManager->GetGridLeftOffsetColor (this);

	CRect rectClient;
	GetClientRect (rectClient);

	CRect rectFill = rectClient;
	rectFill.top += m_rectHeader.Height () + m_nGroupByBoxHeight;

	OnFillBackground (pDC, rectFill);

	HFONT hfontOld = SetCurrFont (pDC);
	pDC->SetTextColor (GetTextColor ());
	pDC->SetBkMode (TRANSPARENT);

	CRect rectList = m_rectList;
	m_rectClip.NormalizeRect ();
	BOOL bDrawList = m_rectClip.IntersectRect (&rectList, &m_rectClip);

	m_rgnClip.CreateRectRgnIndirect (m_rectClip);
	pDC->SelectClipRgn (&m_rgnClip);

	if (bDrawList)
	{
		OnDrawList (pDC);
		OnDrawSelectionBorder (pDC);
	}

	pDC->SelectClipRgn (NULL);

	m_rectClip.SetRectEmpty ();
	m_rgnClip.DeleteObject ();

	OnDrawGridHeader (pDC);
	OnDrawGridFooter (pDC);

	CRect rectGroupByBox = rectClient;
	rectGroupByBox.bottom = min (rectGroupByBox.top + m_nGroupByBoxHeight, rectClient.bottom);
	OnDrawGroupByBox (pDC, rectGroupByBox);

	OnDrawHeader (pDC);

	::SelectObject (pDC->GetSafeHdc (), hfontOld);

	if (m_pColumnChooser == NULL && m_bColumnChooserVisible)
	{
		ShowColumnsChooser ();
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnFillBackground (CDC* pDC, CRect rectClient)
{
	ASSERT_VALID (pDC);

	if (m_brBackground.GetSafeHandle () == NULL)
	{
		visualManager->OnFillGridBackground (pDC, rectClient);
	}
	else
	{
		pDC->FillRect (rectClient, &m_brBackground);
	}
}
//******************************************************************************************
COLORREF CBCGPGridCtrl::OnFillSelItem (CDC* pDC, CRect rectFill, CBCGPGridItem* /*pItem*/)
{
	ASSERT_VALID (pDC);

	if (m_ColorData.m_SelColors.Draw (pDC, rectFill))
	{
		return m_ColorData.m_SelColors.m_clrText;
	}

	if (!m_bFocused)
	{
		if (m_brSelBackground.GetSafeHandle () != NULL)
		{
			pDC->FillRect (rectFill, &m_brSelBackground);
			return globalData.clrBtnText;
		}
		else
		{
			return visualManager->OnFillGridItem (this, pDC, rectFill,
				TRUE/*bSelected*/, FALSE/*bActive*/, FALSE/*bSortedColumn*/);
		}
	}
	else
	{
		if (m_brSelBackground.GetSafeHandle () != NULL)
		{
			pDC->FillRect (rectFill, &m_brSelBackground);
			return globalData.clrTextHilite;
		}
		else
		{
			return visualManager->OnFillGridItem (this, pDC, rectFill,
				TRUE/*bSelected*/, FALSE/*bActive*/, FALSE/*bSortedColumn*/);
		}
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnFillLeftOffset (CDC* pDC, CRect rectFill, CBCGPGridRow* /*pRow*/,
									  BOOL bDrawRightBorder)
{
	ASSERT_VALID (pDC);

	const int nRightBorder = rectFill.left + GetHierarchyOffset () + GetExtraHierarchyOffset ();

	// special drawing if custom gradient colors
	if (m_ColorData.m_LeftOffsetColors.m_clrBackground != (COLORREF)-1 &&
		m_ColorData.m_LeftOffsetColors.m_clrGradient != (COLORREF)-1 &&
		m_ColorData.m_LeftOffsetColors.m_nGradientAngle == 0)
	{
		CRect rectGradient = rectFill;
		rectGradient.right = nRightBorder;
		m_ColorData.m_LeftOffsetColors.Draw (pDC, rectGradient, TRUE);

		if (rectGradient.right < rectFill.right)
		{
			CRect rectOneColor = rectFill;
			rectOneColor.left = rectGradient.right;
			CBrush br (m_ColorData.m_LeftOffsetColors.m_clrGradient);
			pDC->FillRect (rectOneColor, &br);
		}
	}

	else if (!m_ColorData.m_LeftOffsetColors.Draw (pDC, rectFill, TRUE))
	{
		CBrush br (m_clrGray);
		pDC->FillRect (rectFill, &br);
	}

	if (bDrawRightBorder &&
		rectFill.right <= nRightBorder &&
		m_ColorData.m_LeftOffsetColors.m_clrBorder != (COLORREF)-1)
	{
		CPen pen (PS_SOLID, 1, m_ColorData.m_LeftOffsetColors.m_clrBorder);
		CPen* pOldPen = pDC->SelectObject (&pen);

		pDC->MoveTo (rectFill.right - 1, rectFill.top);
		pDC->LineTo (rectFill.right - 1, rectFill.bottom);

		pDC->SelectObject (pOldPen);
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnDrawList (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	CPen* pOldPen = pDC->SelectObject (&m_penVLine);
	ASSERT_VALID (pOldPen);

	int nXLeft = m_rectList.left - m_nHorzScrollOffset;
	int nCount = 0;	// count visible columns
	if (!IsKindOf (RUNTIME_CLASS (CBCGPReportCtrl)))
	{
		int nPos = m_Columns.Begin ();
		while (nPos != m_Columns.End ())
		{
			int iColumn = m_Columns.Next (nPos);
			if (iColumn == -1)
			{
				break; // no more visible columns
			}

			ASSERT (iColumn >= 0);
			ASSERT (iColumn < m_Columns.GetColumnCount ());

			BOOL bIsTreeColumn = (m_nTreeColumn == -1) ? (nCount == 0):
				(m_nTreeColumn == iColumn);

			int nWidth = m_Columns.GetColumnWidth (iColumn);
			if (bIsTreeColumn)
			{
				nWidth += GetHierarchyOffset () + GetExtraHierarchyOffset ();
			}

			nXLeft += nWidth;
			nCount ++;

			if (!m_bGridItemBorders)
			{
				// Draw default vertical grid lines
				pDC->MoveTo (nXLeft - 1, m_rectList.top - 1);
				pDC->LineTo (nXLeft - 1, m_rectList.bottom);
			}
		}
	}

	pDC->SelectObject (&m_penHLine);
	
	if (m_bVirtualMode)
	{
		int nIndex = m_nVertScrollOffset / m_nRowHeight;

		for (; nIndex < m_nVirtualRows; nIndex++)
		{
			//-----------------
			// Get virtual row:
			//-----------------
			CBCGPGridRow* pRow = GetVirtualRow (nIndex);
			if (pRow == NULL)
			{
				break;
			}

			//----------
			// Draw row:
			//----------
			if (!OnDrawItem (pDC, pRow))
			{
				break;
			}
		}
	}
	else
	{
		CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
			m_bVirtualMode || (!IsSortingMode () && !IsGrouping ()) ? 
				m_lstItems : m_lstTerminalItems;

		POSITION pos = m_nFirstVisibleItem == -1 ?
			lst.GetHeadPosition () : lst.FindIndex (m_nFirstVisibleItem);

		while (pos != NULL)
		{
			CBCGPGridRow* pItem = lst.GetNext (pos);
			if (pItem == NULL)
			{
				break;
			}

			ASSERT_VALID (pItem);

			if (!OnDrawItem (pDC, pItem))
			{
				break;
			}
		}
	}

	pDC->SelectObject (pOldPen);
}
//******************************************************************************************
BOOL CBCGPGridCtrl::OnDrawItem (CDC* pDC, CBCGPGridRow* pItem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (pItem);

	if (!pItem->m_Rect.IsRectEmpty ())
	{
		if (pItem->m_Rect.top >= m_rectClip.bottom)
		{
			return FALSE;
		}

		if (pItem->m_Rect.bottom >= m_rectClip.top)
		{
			int dx = IsSortingMode () && !IsGrouping () ? 0 : pItem->GetHierarchyLevel () * m_nRowHeight;
			
			BOOL bIsAutoGroup = pItem->IsGroup () && (pItem->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
			BOOL bGroupUnderline = !m_bHighlightGroups && !pItem->HasValueField ();

			COLORREF clrTextOld = (COLORREF)-1;

			// ----------------
			// Draw left margin
			// ----------------
			CRect rectLeft = pItem->m_Rect;
			rectLeft.top++;
			if (!m_bHighlightGroups || !pItem->IsGroup ())
			{
				rectLeft.right = rectLeft.left + GetExtraHierarchyOffset () + dx;
			}
			rectLeft.bottom++;

			CRgn rgnClipLeft;
			CRect rectLeftClip = rectLeft;
			rectLeftClip.left = max (rectLeftClip.left, m_rectList.left);
			rectLeftClip.bottom = min (rectLeftClip.bottom + 1, m_rectList.bottom);

			CRect rectCol;
			m_Columns.GetColumnRect (m_Columns.OrderToIndex (1), rectCol);
			if (!rectCol.IsRectNull ())
			{
				rectLeftClip.right = min (rectLeftClip.right, rectCol.left);
			}

			if (rectLeftClip.left < rectLeftClip.right)
			{
				rgnClipLeft.CreateRectRgnIndirect (&rectLeftClip);
				pDC->SelectClipRgn (&rgnClipLeft);

				OnFillLeftOffset (pDC, rectLeft, pItem, 
					pItem->HasValueField () && !bIsAutoGroup);

				pDC->SelectClipRgn (&m_rgnClip);
			}

			if (!pItem->IsEnabled ())
			{
				clrTextOld = pDC->SetTextColor (globalData.clrGrayedText);
			}
			
			CRect rectName = pItem->m_Rect;

			if (!pItem->HasValueField () || bIsAutoGroup)
			{
				// fill group background
				CRect rectFill = rectName;
				rectFill.top++;
				rectFill.DeflateRect (dx, 0, 0, 0);

				if (!bGroupUnderline)
				{
					rectFill.DeflateRect (GetExtraHierarchyOffset (), 0, 0, 0);
				}

				if (!m_bHighlightGroups)
				{
					if (IsKindOf (RUNTIME_CLASS (CBCGPReportCtrl)) ||
						!m_ColorData.m_GroupColors.Draw (pDC, rectFill))
					{
						if (m_brGroupBackground.GetSafeHandle () != NULL ||
							m_brBackground.GetSafeHandle () != NULL)
						{
							CBrush& br = (m_brGroupBackground.GetSafeHandle () != NULL) ? 
								m_brGroupBackground : m_brBackground;
							pDC->FillRect (rectFill, &br);
						}
						else
						{
							visualManager->OnFillGridGroupBackground (this, pDC, rectFill);
						}
					}
				}

				// draw group underline
				if (bGroupUnderline && pItem->IsGroup ())
				{
					rectFill.top = rectFill.bottom;
					rectFill.InflateRect (0, 1);

					visualManager->OnDrawGridGroupUnderline (this, pDC, rectFill);
				}
			}

			// ---------------
			// draw expandbox:
			// ---------------
			if (pItem->IsGroup () && (!IsSortingMode () || IsGrouping ()) &&
				!pItem->m_lstSubItems.IsEmpty ())
			{
				CRect rectExpand = rectName;
				rectName.left += m_nRowHeight + dx;
				rectExpand.right = rectName.left;
				rectExpand.DeflateRect (dx, 0, 0, 0);

				CRgn rgnClipExpand;
				CRect rectExpandClip = rectExpand;
				rectExpandClip.bottom = min (rectExpandClip.bottom, m_rectList.bottom);

				rgnClipExpand.CreateRectRgnIndirect (&rectExpandClip);
				pDC->SelectClipRgn (&rgnClipExpand);

				pItem->OnDrawExpandBox (pDC, rectExpand);

				pDC->SelectClipRgn (&m_rgnClip);
			}

			// ----------------
			// Draw row marker:
			// ----------------
			BOOL bActiveItem = (GetCurSel () == pItem);
			if (m_bRowMarker && bActiveItem && 
				!pItem->IsGroup() && GetExtraHierarchyOffset () > 0)
			{
				CRect rectRowMarker = rectLeft;
				rectRowMarker.left = max (
					rectRowMarker.right - GetExtraHierarchyOffset (), 
					rectRowMarker.left);

				CRgn rgnClipMarker;
				CRect rectMarkerClip = rectRowMarker;
				rectMarkerClip.left = min (rectMarkerClip.left, m_rectList.left);
				rectMarkerClip.bottom = min (rectMarkerClip.bottom, m_rectList.bottom);

				rgnClipMarker.CreateRectRgnIndirect (&rectMarkerClip);
				pDC->SelectClipRgn (&rgnClipMarker);

				pItem->OnDrawRowMarker (pDC, rectRowMarker);

				pDC->SelectClipRgn (&m_rgnClip);
			}

			// ----------
			// draw name:
			// ----------
			if (rectName.right > rectName.left)
			{
				CRgn rgnClipName;
				CRect rectNameClip = rectName;
				rectNameClip.bottom = min (rectNameClip.bottom, m_rectList.bottom);

				rgnClipName.CreateRectRgnIndirect (&rectNameClip);
				pDC->SelectClipRgn (&rgnClipName);

				HFONT hOldFont = NULL;
				if (pItem->IsGroup ())
				{
					hOldFont = (HFONT) ::SelectObject (pDC->GetSafeHdc (), m_fontBold.GetSafeHandle ());
				}

				pItem->OnDrawName (pDC, rectName);

				if (hOldFont != NULL)
				{
					::SelectObject (pDC->GetSafeHdc (), hOldFont);
				}

				pDC->SelectClipRgn (&m_rgnClip);
			}

			// ------------
			// draw values:
			// ------------
			if (pItem->HasValueField () && !bIsAutoGroup)
			{
				CRect rectItems = pItem->m_Rect;

				CRect rectValClip = rectItems;
				rectValClip.bottom = min (rectValClip.bottom, m_rectList.bottom);
				rectValClip.bottom++;

				m_rgnClipRow.CreateRectRgnIndirect (&rectValClip);
				pDC->SelectClipRgn (&m_rgnClipRow);

				pItem->OnDrawItems (pDC, rectItems);

				if (!m_bGridItemBorders)
				{
					// Draw default horizontal grid lines
					pDC->MoveTo (m_rectList.left + GetExtraHierarchyOffset () + dx - m_nHorzScrollOffset, pItem->m_Rect.bottom);
					pDC->LineTo (m_rectList.right, pItem->m_Rect.bottom);
				}

				pDC->SelectClipRgn (NULL);
				m_rgnClipRow.DeleteObject ();
			}
			else if (!m_bHighlightGroups && !bGroupUnderline)
			{
				pDC->SelectClipRgn (NULL);
				
				pDC->MoveTo (m_rectList.left + GetExtraHierarchyOffset () + dx - m_nHorzScrollOffset, pItem->m_Rect.bottom);
				pDC->LineTo (m_rectList.right, pItem->m_Rect.bottom);
			}

			pDC->SelectClipRgn (&m_rgnClip);

			if (m_bDrawFocusRect &&
				IsFocused () && IsWholeRowSel () && GetCurSel () == pItem)
			{
				CRect rect = pItem->m_Rect;
				rect.top++;
				rect.DeflateRect (dx, 0, 0, 0);
				pDC->DrawFocusRect (rect);
			}

			if (clrTextOld != (COLORREF)-1)
			{
				pDC->SetTextColor (clrTextOld);
			}
		}
	}

	return TRUE;
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawGroupByBox (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (!m_bGroupByBox)
	{
		return;
	}

	int nXMul = 1, nXDiv = 1;
	int nYMul = 1, nYDiv = 1;
	if (m_bIsPrinting)
	{
		// map to printer metrics
		HDC hDCFrom = ::GetDC(NULL);
		nXMul = pDC->GetDeviceCaps(LOGPIXELSX);			// pixels in print dc
		nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX);	// pixels in screen dc
		nYMul = pDC->GetDeviceCaps(LOGPIXELSY);			// pixels in print dc
		nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
		::ReleaseDC(NULL, hDCFrom);
	}

	const int CALCULATED_TEXT_MARGIN = m_bIsPrinting ? 
		::MulDiv (TEXT_MARGIN, nXMul, nXDiv) : TEXT_MARGIN;

	COLORREF clrText = (m_bIsPrinting ? m_clrPrintText : globalData.clrBtnText);
	CBrush br (m_clrPrintHeaderBG);
	if (m_bIsPrinting)
	{
		pDC->FillRect (rect, &br);
	}
	else
	{
		clrText = visualManager->OnFillGridGroupByBoxBackground (pDC, rect);
	}

	COLORREF clrTextOld = pDC->GetTextColor ();

	if (m_Columns.GetGroupColumnCount () > 0)
	{
		// map to printer metrics
		const int CALCULATED_HMARGIN = m_bIsPrinting ? 
			::MulDiv (BCGPGRID_GROUPBYBOX_HMARGIN, nXMul, nXDiv) : 
			BCGPGRID_GROUPBYBOX_HMARGIN;
		const int CALCULATED_VMARGIN = m_bIsPrinting ? 
			::MulDiv (BCGPGRID_GROUPBYBOX_VMARGIN, nYMul, nYDiv) : 
			BCGPGRID_GROUPBYBOX_VMARGIN;

		const int CALCULATED_COLUMNWIDTH = m_bIsPrinting ? 
			::MulDiv (BCGPGRID_GROUPBYBOX_COLUMNWIDTH, nXMul, nXDiv) : 
			BCGPGRID_GROUPBYBOX_COLUMNWIDTH;

		const int CALCULATED_HSPACING = m_bIsPrinting ? 
			::MulDiv (BCGPGRID_GROUPBYBOX_HSPACING, nXMul, nXDiv) : 
			BCGPGRID_GROUPBYBOX_HSPACING;
		const int CALCULATED_VSPACING = m_bIsPrinting ? 
			::MulDiv (BCGPGRID_GROUPBYBOX_VSPACING, nYMul, nYDiv) : 
			BCGPGRID_GROUPBYBOX_VSPACING;
		
		rect.DeflateRect (CALCULATED_HMARGIN, CALCULATED_VMARGIN, 0, 0);

		int nItemHeight = (m_bIsPrinting ? m_PrintParams.m_nRowHeight : m_nRowHeight) +
			CALCULATED_TEXT_MARGIN;

		CPen pen (PS_SOLID, 1, m_bIsPrinting ? m_clrPrintHeader : 
			visualManager->GetGridGroupByBoxLineColor ());
		CPen* pOldPen = pDC->SelectObject (&pen);

		for (int i = 0; i < m_Columns.GetGroupColumnCount (); i++)
		{
			int nCol = m_Columns.GetGroupColumn (i);
			if (nCol != -1)
			{
				CRect rectItem = rect;

				int nItemWidth = pDC->GetTextExtent (GetColumnName (nCol)).cx + 
					CALCULATED_COLUMNWIDTH + CALCULATED_TEXT_MARGIN;

				rectItem.bottom = min (rectItem.top + nItemHeight, rect.bottom);
				rectItem.right = min (rectItem.left + nItemWidth, rect.right);

				if (m_bIsPrinting)
				{
					pDC->FillRect (rectItem, &br);
				}

				pDC->SetTextColor (clrText);

				m_Columns.DrawColumn (pDC, nCol, rectItem,
					CALCULATED_TEXT_MARGIN, ::MulDiv(5, nXMul, nXDiv),
					m_bIsPrinting, FALSE, TRUE);

				//-----------------------
				// Draw connection lines:
				//-----------------------
				if (i < m_Columns.GetGroupColumnCount () - 1)
				{
					CPoint pt = rectItem.BottomRight ();
					pt.Offset (CALCULATED_HSPACING - CALCULATED_VSPACING + 1, 0);
					pDC->MoveTo (pt);
					pt.Offset (0, CALCULATED_HSPACING);
					pDC->LineTo (pt);
					pt.Offset (CALCULATED_VSPACING, 0);
					pDC->LineTo (pt);
				}

				rect.left += nItemWidth;
			}

			rect.DeflateRect (CALCULATED_HSPACING, CALCULATED_VSPACING, 0, 0);
		}

		pDC->SelectObject (pOldPen);
	}
	else if (!m_bIsPrinting)
	{
		CBCGPLocalResource locaRes;

		int nMargin = ::MulDiv (5, nXMul, nXDiv);
		rect.DeflateRect (nMargin, nMargin);

		CString strTitle;
		strTitle.LoadString (IDS_BCGBARRES_GRID_GROUP_TITLE);

		int nTextWidth = pDC->GetTextExtent (strTitle).cx;
		rect.right = min (rect.right, rect.left + nTextWidth + 2 * CALCULATED_TEXT_MARGIN);

		if (m_bIsPrinting)
		{
			CBrush br (m_clrPrintHeaderBG);
			pDC->FillRect (rect, &br);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brBtnFace);
		}

		CRect rectText = rect;
		rectText.DeflateRect (CALCULATED_TEXT_MARGIN, CALCULATED_TEXT_MARGIN, CALCULATED_TEXT_MARGIN, 0);
		
		pDC->SetTextColor (m_bIsPrinting ? m_clrPrintText : globalData.clrBtnShadow);

		pDC->DrawText (strTitle, rectText,
			DT_LEFT | DT_SINGLELINE | DT_TOP | DT_NOPREFIX | DT_END_ELLIPSIS);
	}

	pDC->SetTextColor (clrTextOld);
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawHeader (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rectHeader.IsRectEmpty ())
	{
		return;
	}

	OnFillHeaderBackground (pDC, m_rectHeader);

	for (int i = 0; i <= m_Columns.GetColumnCount(); i++)
	{
		if (i < m_Columns.GetColumnCount() &&
			!m_Columns.GetColumnVisible (i))
		{
			continue;
		}

		CRect rectColumn;
		m_Columns.GetColumnRect (i, rectColumn);

		m_Columns.DrawColumn (pDC, i, rectColumn);
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawSortArrow (CDC* pDC, CRect rectArrow, BOOL bAscending)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	visualManager->OnDrawGridSortArrow (this, pDC, rectArrow, bAscending);
}
//****************************************************************************************
void CBCGPGridCtrl::OnFillHeaderBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);

	if (!m_ColorData.m_HeaderColors.Draw (pDC, rect))
	{
		visualManager->OnFillGridHeaderBackground (this, pDC, rect);
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawHeaderItemBorder (CDC* pDC, CRect rect, int nCol)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);

	CBCGPGridColors::ColorData colorData = 
		IsColumnSelected (nCol) && m_ColorData.m_HeaderSelColors.m_clrBackground != -1 ? 
			m_ColorData.m_HeaderSelColors :
			m_ColorData.m_HeaderColors;

	if (!colorData.Draw (pDC, rect))
	{
		BOOL bPressed = (nCol == m_nDraggedColumn &&
			!m_bDragGroupItem && !m_bDragFromChooser);

		m_bHeaderItemHovered = (nCol == m_Columns.GetHighlightColumn () && nCol != -1);
		m_Columns.m_bInvertPressedColumn = 
			visualManager->OnDrawGridHeaderItemBorder (this, pDC, rect, bPressed);
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawGridHeader (CDC* pDC)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);

	CRect rectFill = GetGridHeaderRect ();
	pDC->FillRect (rectFill, &globalData.brBtnFace);
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawGridFooter (CDC* pDC)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);

	CRect rectFill = GetGridFooterRect ();
	pDC->FillRect (rectFill, &globalData.brBtnFace);
}
//****************************************************************************************
CRect CBCGPGridCtrl::OnGetSelectionRect ()
{
	if (m_lstSel.GetCount () > 1 ||
		!IsSelectionBorderEnabled ())
	{
		return CRect (0, 0, 0, 0);
	}

	CBCGPGridRow* pSelRow = NULL;
	CBCGPGridItem* pSelItem = NULL;
	CBCGPGridRow* pLastSelRow = NULL;
	CBCGPGridItem* pLastSelItem = NULL;

	if (m_bVirtualMode)
	{
		pSelRow = (m_idActive.m_nRow != -1) ? GetRow (m_idActive.m_nRow) : NULL;
		pSelItem = (pSelRow != NULL) ? pSelRow->GetItem (m_idActive.m_nColumn) : NULL;

		pLastSelRow = (m_idLastSel.m_nRow != -1) ? GetRow (m_idLastSel.m_nRow) : NULL;
		pLastSelItem = (pLastSelRow != NULL) ? pLastSelRow->GetItem (m_idLastSel.m_nColumn) : NULL;
	}
	else
	{
		pSelRow = m_pSelRow;
		pSelItem = m_pSelItem;

		pLastSelRow = m_pLastSelRow;
		pLastSelItem = m_pLastSelItem;
	}

	// The whole column is selected:
	if ((m_idActive.IsColumn () || m_idLastSel.IsColumn ()) && 
		pSelRow == NULL && pSelItem == NULL && m_lstSel.GetCount () == 1)
	{
		CBCGPGridRange* pRange = m_lstSel.GetTail ();
		if (pRange != NULL)
		{
			CBCGPGridRow* pRow1 = GetRow (pRange->m_nTop);
			CBCGPGridItem* pItem1 = (pRow1 != NULL) ? pRow1->GetItem (pRange->m_nLeft) : NULL;
			CBCGPGridRow* pRow2 = GetRow (pRange->m_nBottom);
			CBCGPGridItem* pItem2 = (pRow2 != NULL) ? pRow2->GetItem (pRange->m_nRight) : NULL;

			if (pRow1 != NULL && pItem1 != NULL && pRow2 != NULL && pItem2 != NULL)
			{
				pSelRow = pRow1;
				pSelItem = pItem1;
				pLastSelRow = pRow2;
				pLastSelItem = pItem2;
			}
		}
	}

	//-------------------------------
	// Calculate selection rectangle:
	//-------------------------------
	CRect rectSel (0, 0, 0, 0);
	if (pSelRow != NULL)
	{
		ASSERT_VALID (pSelRow);

		rectSel = (pSelItem != NULL) ? pSelItem->GetRect () : pSelRow->GetRect ();
		if (pSelItem == NULL)
		{
			rectSel.right = max (rectSel.right - 1, rectSel.left);
			rectSel.left = min (rectSel.right,
				rectSel.left + GetHierarchyOffset () + GetExtraHierarchyOffset ());
		}
		rectSel.NormalizeRect ();

		// Extend if merged:
		if (pSelItem != NULL && pSelItem->GetMergedCells () != NULL)
		{
			CRect rectMerged = pSelItem->GetMergedRect ();
			if (!rectMerged.IsRectEmpty ())
			{
				rectMerged.NormalizeRect ();
				rectSel.UnionRect (rectSel, rectMerged);
			}
		}
	}

	CRect rectLastSel (0, 0, 0, 0);
	if (pLastSelRow != NULL)
	{
		ASSERT_VALID (pLastSelRow);

		rectLastSel = (pLastSelItem != NULL) ? pLastSelItem->GetRect () : pLastSelRow->GetRect ();
		if (pLastSelItem == NULL)
		{
			rectLastSel.left = min (rectLastSel.right,
				rectLastSel.left + GetHierarchyOffset () + GetExtraHierarchyOffset ());
			rectLastSel.right = rectSel.right;
		}
		rectLastSel.NormalizeRect ();

		// Extend if merged:
		if (pLastSelItem != NULL && pLastSelItem->GetMergedCells () != NULL)
		{
			CRect rectMerged = pLastSelItem->GetMergedRect ();
			if (!rectMerged.IsRectEmpty ())
			{
				rectMerged.NormalizeRect ();
				rectLastSel.UnionRect (rectLastSel, rectMerged);
			}
		}
	}

	CRect rectTrackSel (0, 0, 0, 0);
	rectTrackSel.UnionRect (rectSel, rectLastSel);

	if (!rectTrackSel.IsRectEmpty ())
	{
		rectTrackSel.InflateRect (2, 1, 2, 2);
	}

	return rectTrackSel;
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawSelectionBorder (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_bIsPrinting || IsGrouping ())
	{
		return;
	}

	if (!m_rectTrackSel.IsRectEmpty ())
	{
		visualManager->OnDrawGridSelectionBorder (this, pDC, m_rectTrackSel);
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnItemChanged (CBCGPGridItem* pItem, int nRow, int nColumn)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	BCGPGRID_ITEM_INFO ii;
	memset (&ii, 0, sizeof (BCGPGRID_ITEM_INFO));
	ii.pItem = pItem;
	ii.nRow = nRow;
	ii.nCol = nColumn;

	GetOwner ()->SendMessage (BCGM_GRID_ITEM_CHANGED, GetDlgCtrlID (),
		LPARAM (&ii));
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//******************************************************************************************
void CBCGPGridCtrl::EnableHeader (BOOL bEnable, DWORD dwFlags)
{
	ASSERT_VALID (this);

	CBCGPGridRow* pItem = GetCurSel ();
	if (pItem != NULL)
	{
		pItem->OnEndEdit ();
	}

	m_bHeader = bEnable;
	m_dwHeaderFlags = dwFlags;

	if ((m_dwHeaderFlags & BCGP_GRID_HEADER_SORT) != 0 &&
		(m_dwHeaderFlags & BCGP_GRID_HEADER_MOVE_ITEMS) == 0)
	{
		EnableDragHeaderItems (FALSE);
	}

	AdjustLayout ();
	RedrawWindow ();
}
//*****************************************************************************************
void CBCGPGridCtrl::SetHeaderImageList (CImageList* pImagesHeader)
{
	m_pImagesHeader = pImagesHeader;
}
//*****************************************************************************************
void CBCGPGridCtrl::SetHeaderBtnImageList (CBCGPToolBarImages* pImagesHeaderBtn)
{
	m_pImagesHeaderBtn = pImagesHeaderBtn;
}
//*****************************************************************************************
void CBCGPGridCtrl::SetImageList (CImageList* pImages)
{
	m_pImages = pImages;
}
//*****************************************************************************************
void CBCGPGridCtrl::EnableGroupByBox (BOOL bEnable)
{
	ASSERT_VALID (this);

	CBCGPGridRow* pItem = GetCurSel ();
	if (pItem != NULL)
	{
		pItem->OnEndEdit ();
	}

	m_bGroupByBox = bEnable;

	AdjustLayout ();
	RedrawWindow ();
}
//*****************************************************************************************
void CBCGPGridCtrl::TrackHeader (int nOffset)
{
	CClientDC dc (this);

	BOOL bShowDragContext = m_bShowDragContext && !IsColumnAutoSizeEnabled ();

	if (!m_rectTrackHeader.IsRectEmpty () && !bShowDragContext)
	{
		dc.InvertRect (m_rectTrackHeader);
	}

	if (nOffset < 0)	// End of track
	{
		m_rectTrackHeader.SetRectEmpty ();

		dc.InvertRect (m_rectTrackHeaderLeft);
		m_rectTrackHeaderLeft.SetRectEmpty ();
	}
	else
	{
		m_rectTrackHeader = m_rectList;
		m_rectTrackHeader.left += nOffset;
		m_rectTrackHeader.right = m_rectTrackHeader.left + 1;

		// Calculate column left offset:
		int nLeftOffset = 0;
		int nCount = 0;	// count visible columns
		
		int nPos = m_Columns.Begin ();
		while (nPos != m_Columns.End ())
		{
			int i = m_Columns.Next (nPos);
			if (i == -1)
			{
				break; // no more visible columns
			}

			if (i == m_nTrackColumn)
			{
				break;
			}

			int nWidth = m_Columns.GetColumnWidth (i);
			if (nCount == 0)
			{
				nWidth += GetHierarchyOffset () + GetExtraHierarchyOffset ();
			}

			nLeftOffset += nWidth;
			nCount++;
		}

		CRect rectTrackHeaderLeft = m_rectList;
		rectTrackHeaderLeft.left += nLeftOffset - m_nHorzScrollOffset;
		rectTrackHeaderLeft.right = rectTrackHeaderLeft.left + 1;

		if (bShowDragContext)
		{
			int nNewWidth = nOffset + m_nHorzScrollOffset - nLeftOffset;
			if (nCount == 0)
			{
				nNewWidth -= GetHierarchyOffset () + GetExtraHierarchyOffset ();
			}
			nNewWidth = max (m_nRowHeight, nNewWidth);
			SetHeaderItemWidth (m_nTrackColumn, nNewWidth);

			AdjustLayout ();
			
			CBCGPGridRow* pSel = GetCurSel ();
			if (pSel != NULL)
			{
				pSel->AdjustButtonRect ();
			}
		}
		else
		{
			if (rectTrackHeaderLeft != m_rectTrackHeaderLeft)
			{
				if (!m_rectTrackHeaderLeft.IsRectEmpty ())
				{
					dc.InvertRect (m_rectTrackHeaderLeft);
				}

				m_rectTrackHeaderLeft = rectTrackHeaderLeft;
				dc.InvertRect (m_rectTrackHeaderLeft);
			}

			dc.InvertRect (m_rectTrackHeader);
		}
	}
}
//*****************************************************************************************
void CBCGPGridCtrl::TrackToolTip (CPoint point)
{
	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return;
	}

#ifndef _BCGPGRID_STANDALONE
	if (CBCGPPopupMenu::GetActiveMenu () != NULL)
	{
		return;
	}
#endif

	CPoint ptScreen = point;
	ClientToScreen (&ptScreen);

	CRect rectTT;
	m_ToolTip.GetLastRect (rectTT);

	if (rectTT.PtInRect (ptScreen) && m_ToolTip.IsWindowVisible ())
	{
		return;
	}

	if (::GetCapture () == GetSafeHwnd ())
	{
		ReleaseCapture ();
	}

	if (!m_bShowInPlaceToolTip)
	{
		m_ToolTip.Deactivate ();
		return;
	}

	if (m_rectColumnChooser.PtInRect (ptScreen))
	{
		m_ToolTip.Deactivate ();
		return;
	}

	CBCGPGridRow::ClickArea clickArea;
	CBCGPGridItemID id;
	CBCGPGridItem* pHitItem = NULL;
	CBCGPGridRow* pHitRow = HitTest (point, id, pHitItem, &clickArea);

	if (pHitRow == NULL)
	{
		m_ToolTip.Deactivate ();
		return;
	}

	ASSERT_VALID (pHitRow);

	CString strTipText;
	CRect rectToolTip;
	rectToolTip.SetRectEmpty ();

	if (clickArea == CBCGPGridRow::ClickName)
	{
		strTipText = pHitRow->GetNameTooltip ();
		rectToolTip = pHitRow->GetNameTooltipRect ();
		if (point.x < rectToolTip.left)
		{
			strTipText.Empty ();
		}
	}
	else if (clickArea == CBCGPGridRow::ClickValue)
	{
		BOOL bIsAutoGroup = pHitRow->IsGroup () && (pHitRow->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
		if (pHitRow->IsGroup () && 
			(!pHitRow->HasValueField () || bIsAutoGroup))
		{
			strTipText = pHitRow->GetValueTooltip ();
			rectToolTip = pHitRow->GetRect ();
		}
		else
		{
			if (pHitItem != NULL)
			{
				ASSERT_VALID (pHitItem);
				if (pHitItem->IsInPlaceEditing ())
				{
					return;
				}

				strTipText = pHitItem->GetValueTooltip ();
				rectToolTip = pHitItem->GetRect ();
			}
		}
	}
	
	if (!strTipText.IsEmpty ())
	{
		ClientToScreen (&rectToolTip);
		
		if (rectTT.TopLeft () == rectToolTip.TopLeft ())
		{
			// Tooltip on the same place, don't show it to prevent flashing
			return;
		}
		
		m_ToolTip.SetTextMargin (TEXT_MARGIN);
		m_ToolTip.SetFont (GetFont ());

		m_ToolTip.Track (rectToolTip, strTipText);
		SetCapture ();
	}
	else
	{
		m_ToolTip.Deactivate ();
	}
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetHeaderItemWidth (int nPos, int nWidth)
{
	if (!m_Columns.OnChangeColumnWidth (nPos, nWidth))
	{	// column can't be resized
		return FALSE;
	}

	BOOL bRes = m_Columns.ResizeColumn (nPos, nWidth);
	OnResizeColumns ();
	return bRes;
}
//*****************************************************************************************
void CBCGPGridCtrl::Sort (int nColumn, BOOL bAscending, BOOL bAdd)
{
	CWaitCursor wait;
	
	m_Columns.SetSortColumn (nColumn, bAscending, bAdd);
	SetCurSel (NULL);

	if (GetSafeHwnd () != NULL)
	{
		SetRebuildTerminalItems ();
		ReposItems ();

		AdjustLayout ();
		RedrawWindow ();
	}
}
//****************************************************************************************
void CBCGPGridCtrl::ToggleSortColumn (int nColumn)
{
	ASSERT_VALID (this);

	if ((m_dwHeaderFlags & BCGP_GRID_HEADER_SORT) == 0)
	{
		return;
	}

	int nColumnState = m_Columns.GetColumnState (nColumn);
	
	if (IsMultipleSort () && nColumnState < 0) // if descending, the third click clears sorting
	{
		RemoveSortColumn (nColumn);

		if (GetSafeHwnd () != NULL)
		{
			SetRebuildTerminalItems ();
			ReposItems ();

			AdjustLayout ();
			RedrawWindow ();
		}
		return;
	}

	BOOL bAscending = TRUE;

	if (nColumnState != 0)
	{
		bAscending = nColumnState <= 0;
	}

	Sort (nColumn, bAscending, IsMultipleSort ());
}
//****************************************************************************************
void CBCGPGridCtrl::SetSortColumn (int nColumn, BOOL bAscending, BOOL bAdd)
{
	m_Columns.SetSortColumn (nColumn, bAscending, bAdd);
	SetRebuildTerminalItems ();
}
//****************************************************************************************
void CBCGPGridCtrl::RemoveSortColumn (int nColumn)
{
	m_Columns.RemoveSortColumn (nColumn);
	SetRebuildTerminalItems ();
}
//****************************************************************************************
void CBCGPGridCtrl::EnableMultipleSort (BOOL bEnable)
{
	if (m_Columns.IsMultipleSort () == bEnable)
	{
		return;
	}

	m_Columns.EnableMultipleSort (bEnable);
	if (!bEnable)
	{
		SetRebuildTerminalItems ();
	}
}
//****************************************************************************************
BOOL CBCGPGridCtrl::IsMultipleSort () const
{
	return m_Columns.IsMultipleSort ();
}
//****************************************************************************************
void CBCGPGridCtrl::EnableMarkSortedColumn (BOOL bMark, BOOL bRedraw)
{
	m_bMarkSortedColumn = bMark;

	if (GetSafeHwnd () != NULL && bRedraw)
	{
		RedrawWindow ();
	}
}
//****************************************************************************************
void CBCGPGridCtrl::EnableFilter (BCGPGRID_FILTERCALLBACK pfnCallback, LPARAM lParam)
{
	m_bFilter = TRUE;
	m_pfnFilterCallback = pfnCallback;	// callback function
	m_lFilterParam = lParam;			// filter info which is used by the callback function
}
//****************************************************************************************
BCGPGRID_FILTERCALLBACK CBCGPGridCtrl::GetFilterCallbackFunct ()
{
	return m_pfnFilterCallback;
}
//****************************************************************************************
BOOL CBCGPGridCtrl::FilterItem (const CBCGPGridRow* pRow)
	// returns TRUE, if item is hidden (filtered)
{
	ASSERT_VALID (this);

	if (!m_bFilter)
	{
		return FALSE; // show item
	}

	ASSERT_VALID (pRow);

	if (m_pfnFilterCallback != NULL)
	{
		return (BOOL)m_pfnFilterCallback ((WPARAM) pRow, (LPARAM) m_lFilterParam);
	}

	return FALSE; // show item
}
//****************************************************************************************
BOOL CBCGPGridCtrl::IsHeaderMenuButtonEnabled (int /*nColumn*/) const
{
	return IsFilterEnabled ();
}
//****************************************************************************************
int CBCGPGridCtrl::GetHeaderMenuButtonImageIndex (int /*nColumn*/, BOOL /*bSortArrow*/) const
{
	return -1;
}
//****************************************************************************************
void CBCGPGridCtrl::OnDrawHeaderMenuButton (CDC* pDC, CRect rect, int nColumn, BOOL bSortArrow)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	//------------------------
	// Draw the header button:
	//------------------------
	int nImage = GetHeaderMenuButtonImageIndex (nColumn, bSortArrow);
	if (m_pImagesHeaderBtn != NULL && nImage >= 0)
	{			
		// draw button border:
		visualManager->OnDrawGridHeaderMenuButton (this, pDC, rect, nColumn == m_Columns.GetHighlightColumnBtn (),
			FALSE, FALSE);

		// draw image:
		const CSize sizeImage = m_pImagesHeaderBtn->GetImageSize ();

		CPoint pt = rect.TopLeft ();
		pt.x ++;
		pt.y += max (0, (rect.Height () - sizeImage.cy) / 2);

		if (rect.Width () > sizeImage.cx)
		{
			pt.x += (rect.Width () - sizeImage.cx) / 2;

			CBCGPDrawState ds;
			m_pImagesHeaderBtn->PrepareDrawImage (ds, sizeImage);
			m_pImagesHeaderBtn->Draw (pDC, pt.x, pt.y, nImage, FALSE);
			m_pImagesHeaderBtn->EndDrawImage (ds);
		}

		return;
	}

	if (bSortArrow)
	{
		int nSortVal = m_Columns.GetColumnState (nColumn);
		if (nSortVal != 0)
		{
			//-----------------
			// Draw sort arrow:
			//-----------------
			int nArrowMargin = 5;
			CRect rectArrow = rect;
			rectArrow.DeflateRect (nArrowMargin, nArrowMargin);
			if (rectArrow.Width () >= rectArrow.Height ())
			{
				rectArrow.left = rectArrow.right - rectArrow.Height ();
				rect.right = rectArrow.left - 1;

				int dy2 = (int) (.134 * rectArrow.Width ());
				rectArrow.DeflateRect (0, dy2);

				OnDrawSortArrow (pDC, rectArrow, nSortVal > 0);
			}
		}
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnHeaderMenuButtonClick (int /*nColumn*/, CRect /*rectMenuButton*/)
{
}
//****************************************************************************************
CRect CBCGPGridCtrl::GetHeaderMenuButtonRect (CRect rectItem, int nColumn) const
{
	CRect rectBtn;
	rectBtn.SetRectEmpty ();

	if (IsHeaderMenuButtonEnabled (nColumn))
	{
		rectBtn = rectItem;
		rectBtn.left = max (rectBtn.right - rectBtn.Height (), rectBtn.left);
	}

	return rectBtn;
}
//****************************************************************************************
int CBCGPGridCtrl::CompareItems (const CBCGPGridRow* pRow1, const CBCGPGridRow* pRow2, int iColumn) const
{ 
	ASSERT_VALID (this);
	ASSERT_VALID (pRow1);
	ASSERT_VALID (pRow2);

	if (!pRow1->HasValueField () ||
		!pRow2->HasValueField ())
	{
		return 0;
	}

	int nColumnState = m_Columns.GetColumnState (iColumn);
	BOOL bIsGroupColumn = m_Columns.IsGroupColumn (iColumn);

	if (nColumnState == 0 && !bIsGroupColumn)
	{
		// not sorted
		return 0;
	}

	BOOL bAscending = (nColumnState >= 0);
	int nRes = 0;

	if (pRow1->m_arrRowItems.GetSize () > iColumn &&
		pRow2->m_arrRowItems.GetSize () > iColumn)
	{
		// compare grid items
		nRes = CompareItems (pRow1->m_arrRowItems [iColumn], pRow2->m_arrRowItems [iColumn]);
	}

	if (!bAscending)
	{
		nRes = -nRes;
	}
		
	return nRes;
}
//*****************************************************************************************

#define DoCompareItems(item1, item2) ((item1) < (item2) ? -1 : (item1 == item2) ? 0 : 1)

int CBCGPGridCtrl::CompareItems (const CBCGPGridItem* pItem1, const CBCGPGridItem* pItem2) const
{ 
	ASSERT_VALID (this);
	ASSERT_VALID (pItem1);
	ASSERT_VALID (pItem2);

	const _variant_t& var1 = pItem1->m_varValue;
	const _variant_t& var2 = pItem2->m_varValue;

	if (var1.vt == var2.vt)
	{
		switch (var1.vt)
		{
		case VT_BSTR:
			{
                HRESULT hr = ::VarBstrCmp(var1.bstrVal, var2.bstrVal, LOCALE_USER_DEFAULT, 0);
				switch(hr)
				{
				case VARCMP_LT:
					return -1;
				case VARCMP_EQ:
					return 0;
				case VARCMP_GT:
					return 1;
				default:
					return 1;
				}
			}

		case VT_I2:
		case VT_UI2:
			return DoCompareItems ((short) var1, (short) var2);

		case VT_I4:
		case VT_INT:
		case VT_UINT:
		case VT_UI4:
			return DoCompareItems ((long) var1, (long) var2);

		case VT_UI1:
			return DoCompareItems ((short)(BYTE) var1, (short)(BYTE) var2);

		case VT_R4:
			return DoCompareItems ((float)  var1, (float) var2);

		case VT_R8:
			return DoCompareItems ((double)  var1, (double) var2);

		case VT_BOOL:
			return DoCompareItems ((bool)  var1, (bool) var2);

		case VT_DATE:
			return DoCompareItems ((DATE)  var1, (DATE) var2);
		}
	}

	CString str1 = ((CBCGPGridItem*) pItem1)->GetLabel ();
	CString str2 = ((CBCGPGridItem*) pItem2)->GetLabel ();

	// if image without text - sort by image index
	if (str1.IsEmpty () && str2.IsEmpty ())
	{
		return DoCompareItems (pItem1->m_iImage, pItem2->m_iImage);
	}

	return str1.Compare (str2);
}
//*****************************************************************************************
int CBCGPGridCtrl::CompareGroup (const CBCGPGridRow* pRow1, const CBCGPGridRow* pRow2, int iColumn)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRow1);
	ASSERT_VALID (pRow2);

	int nGroup1 = OnGetGroupNumber (pRow1, iColumn);
	int nGroup2 = OnGetGroupNumber (pRow2, iColumn);

	if (nGroup1 != -1 || nGroup2 != -1)
	{
		int nColumnState = m_Columns.GetColumnState (iColumn);
		BOOL bIsGroupColumn = m_Columns.IsGroupColumn (iColumn);

		if (nColumnState == 0 && !bIsGroupColumn)
		{
			// not sorted
			return 0;
		}

		int nRes = nGroup1 - nGroup2;

		BOOL bAscending = (nColumnState >= 0);
		if (!bAscending)
		{
			nRes = -nRes;
		}

		return nRes;
	}

	return CBCGPGridCtrl::CompareItems (pRow1, pRow2, iColumn);
}
//*****************************************************************************************
CString CBCGPGridCtrl::GetGroupName (int nGroupCol, CBCGPGridItem* pItem)
{
	CString strValue = pItem->GetLabel ();
	CString strColumn = m_Columns.GetColumnName (nGroupCol);

	CString strGroup;
	strGroup.Format (_T("%s: %s"), strColumn, strValue);

	return strGroup;
}
//*****************************************************************************************
BOOL CBCGPGridCtrl::GetRowName (CBCGPGridRow* pRow, CString& strName)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRow);

    BOOL bIsAutoGroup = pRow->IsGroup () && (pRow->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
	if (!bIsAutoGroup)
	{
		return FALSE;
	}

    CBCGPGridItem* pItem = pRow->GetItem (0);
	if (pItem != NULL)
	{
		ASSERT_VALID (pItem);

		strName = pItem->FormatItem ();
		return !strName.IsEmpty ();
	}

	return FALSE;
}
//*****************************************************************************************
int CBCGPGridCtrl::InsertGroupColumn (int nPos, int nColumn)
{
	int nRes = m_Columns.InsertGroupColumn (nPos, nColumn);

	if (nRes != -1)
	{
		SetRebuildTerminalItems ();
		AdjustLayout ();
	}

	return nRes;
}
//*****************************************************************************************
BOOL CBCGPGridCtrl::RemoveGroupColumn (int nPos)
{
	BOOL bRes = m_Columns.RemoveGroupColumn (nPos);

	if (bRes)
	{
		SetRebuildTerminalItems ();
		AdjustLayout ();
	}

	return bRes;
}
//*****************************************************************************************
BOOL CBCGPGridCtrl::RemoveGroupColumnByVal (int nColumn)
{
	BOOL bRes = m_Columns.RemoveGroupColumnByVal (nColumn);

	if (bRes)
	{
		SetRebuildTerminalItems ();
		ReposItems ();
		
		AdjustLayout ();
	}

	return TRUE;
}
//*******************************************************************************************
int CBCGPGridCtrl::GetGroupColumnRect (int nPos, CRect& rect, CDC* pDC)
{
	ASSERT_VALID (pDC);

	rect.SetRectEmpty ();

	CRect rectClient;
	GetClientRect (rectClient);

	CRect rectGroupByBox = rectClient;
	rectGroupByBox.bottom = min (rectGroupByBox.top + m_nGroupByBoxHeight, rectClient.bottom);

	rectGroupByBox.DeflateRect (BCGPGRID_GROUPBYBOX_HMARGIN, BCGPGRID_GROUPBYBOX_VMARGIN, 0, 0);

	int nItemHeight = m_nRowHeight + TEXT_MARGIN;
	int nIndex = 0;

	for (int i = 0; i < m_Columns.GetGroupColumnCount (); i++)
	{
		int nCol = m_Columns.GetGroupColumn (i);
		if (nCol != -1)
		{
			CRect rectItem = rectGroupByBox;
			CString strColumn = m_Columns.GetColumnName (nCol);

			int nItemWidth = pDC->GetTextExtent (strColumn).cx + 
				BCGPGRID_GROUPBYBOX_COLUMNWIDTH + TEXT_MARGIN;

			rectItem.bottom = min (rectItem.top + nItemHeight, rectGroupByBox.bottom);
			rectItem.right = min (rectItem.left + nItemWidth, rectGroupByBox.right);

			if (nIndex == nPos)
			{
				rect = rectItem;
				return nCol;
			}

			rectGroupByBox.left += nItemWidth;
			nIndex++;
		}

		rectGroupByBox.DeflateRect (BCGPGRID_GROUPBYBOX_HSPACING, BCGPGRID_GROUPBYBOX_VSPACING, 0, 0);
	}

	return -1;
}
//*****************************************************************************************
BOOL CBCGPGridCtrl::IsGrouping () const
{
	return m_Columns.GetGroupColumnCount () > 0;
}
//*****************************************************************************************
int CBCGPGridCtrl::AddRow (BOOL bRedraw)
{
	ASSERT_VALID (this);
	
	CBCGPGridRow* pRow = CreateRow ();
	ASSERT_VALID (pRow);

	pRow->SetOwnerList (this);

	const int nColumnsNum = m_Columns.GetColumnCount ();
	for (int i = 0; i < nColumnsNum ; i++)
	{
		CBCGPGridItem* pItem = pRow->CreateItem (
			(int) pRow->m_arrRowItems.GetSize (), i);
		ASSERT_VALID (pItem);

		pItem->SetOwnerRow (pRow);
		int nIndex = (int) pRow->m_arrRowItems.Add (pItem);
		pItem->m_nIdColumn = nIndex;
	}

	return AddRow (pRow, bRedraw);
}
//*****************************************************************************************
int CBCGPGridCtrl::AddRow (CBCGPGridRow* pItem, BOOL bRedraw)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

#ifdef _DEBUG
	for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridRow* pListItem = m_lstItems.GetNext (pos);
		ASSERT_VALID (pListItem);

		if (pListItem == pItem || pListItem->IsSubItem (pItem))
		{
			// Can't ad the same item twice
			ASSERT (FALSE);
			return -1;
		}
	}
#endif // _DEBUG

	pItem->SetOwnerList (this);

	m_lstItems.AddTail (pItem);
	int nIndex = (int) m_lstItems.GetCount () - 1;
	pItem->m_nIdRow = nIndex;

	SetRebuildTerminalItems ();

	if (bRedraw)
	AdjustLayout ();

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		pItem->Redraw ();
	}

	return nIndex;
}
//*****************************************************************************************
void CBCGPGridCtrl::RebuildIndexes (int nStartFrom)
{
	if (m_bVirtualMode)
	{
		ASSERT (FALSE);
		return;
	}

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst = 
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	POSITION posStartFrom = NULL;
	int nRowIndex = 0;
	if (nStartFrom >= 0 && nStartFrom < lst.GetCount ())
	{
		posStartFrom = lst.FindIndex (nStartFrom);
		nRowIndex = nStartFrom;
	}

	if (posStartFrom == NULL)
	{
		posStartFrom = lst.GetHeadPosition ();
		nRowIndex = 0;
	}

	for (POSITION pos = posStartFrom; pos != NULL; )
	{
		CBCGPGridRow* pGridItem = lst.GetNext (pos);
		ASSERT_VALID (pGridItem);

		if (pGridItem->IsItemVisible ())
		{
			pGridItem->m_nIdRow = nRowIndex;
			nRowIndex++;
		}
	}
}
//*****************************************************************************************
int CBCGPGridCtrl::InsertRowAfter (int nPos, CBCGPGridRow* pItem, BOOL bRedraw)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	if (m_bVirtualMode)
	{
		// Cannot insert row in the virtual mode
		ASSERT (FALSE);
		return -1;
	}

#ifdef _DEBUG
	for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridRow* pListItem = m_lstItems.GetNext (pos);
		ASSERT_VALID (pListItem);

		if (pListItem == pItem || pListItem->IsSubItem (pItem))
		{
			// Can't ad the same item twice
			ASSERT (FALSE);
			return -1;
		}
	}
#endif // _DEBUG

	pItem->SetOwnerList (this);

	POSITION posInsertAfter = NULL;
	if (nPos >= 0 && nPos < m_lstItems.GetCount ())
	{
		posInsertAfter = m_lstItems.FindIndex (nPos);
	}

	int nIndex = -1;
	if (posInsertAfter != NULL)
	{
		// insert after
		m_lstItems.InsertAfter (posInsertAfter, pItem);
		nIndex = nPos + 1;
	}
	else
	{
		// add last
		m_lstItems.AddTail (pItem);
		nIndex = (int) m_lstItems.GetCount () - 1;
	}
	pItem->m_nIdRow = nIndex;

	SetRebuildTerminalItems ();

	if (bRedraw)
	AdjustLayout ();

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		pItem->Redraw ();
	}

	return nIndex;
}
//*****************************************************************************************
int CBCGPGridCtrl::RemoveRow (int nPos, BOOL bRedraw)
{
	ASSERT_VALID (this);

	if (m_bVirtualMode)
	{
		// Cannot remove row in the virtual mode
		ASSERT (FALSE);
		return 0;
	}

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst = 
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	POSITION posRemove = NULL;
	if (nPos >= 0 && nPos < lst.GetCount ())
	{
		posRemove = lst.FindIndex (nPos);
	}

	//--------------------------------
	// Collect list of items to delete
	//--------------------------------
	CList <POSITION, POSITION> lstDelItemsPos;
	CList <CBCGPGridRow*, CBCGPGridRow*> lstDelItemsPtr;

	if (posRemove != NULL)
	{
		CBCGPGridRow* pItemDel = m_lstItems.GetAt (posRemove);
		ASSERT_VALID (pItemDel);

		// if possible store position
		if (!IsSortingMode () && !IsGrouping ())
		{
			lstDelItemsPos.AddTail (posRemove);	// position in m_lstItems
		}
		// store a pointer
		else
		{
			lstDelItemsPtr.AddTail (pItemDel);
		}

		CList<CBCGPGridRow*, CBCGPGridRow*>	lstSubItems;
		pItemDel->GetSubItems (lstSubItems, TRUE);
		
		for (POSITION posSubItem = lstSubItems.GetHeadPosition ();
			posSubItem != NULL; )
		{
			CBCGPGridRow* pSubItemDel = lstSubItems.GetNext (posSubItem);
			ASSERT_VALID (pSubItemDel);

			// if possible store position
			if (!IsSortingMode () && !IsGrouping ())
			{
				int nPos2 = pSubItemDel->GetRowId ();
				POSITION posRemove2 = NULL;
				if (nPos2 >= 0 && nPos2 < lst.GetCount ())
				{
					posRemove2 = lst.FindIndex (nPos2);
				}

				if (posRemove2 != NULL)
				{
					lstDelItemsPos.AddTail (posRemove2);	// position in m_lstItems
				}
			}
			// store a pointer
			else
			{
				lstDelItemsPtr.AddTail (pSubItemDel);
			}
		}

	}

	//--------------
	// Remove items:
	//--------------
	int nCount = 0;
	POSITION pos;
	for (pos = lstDelItemsPos.GetHeadPosition (); pos != NULL;)
	{
		POSITION posDel = lstDelItemsPos.GetNext (pos);
		if (DoRemoveRow (posDel, FALSE))
		{
			nCount++;
		}
	}
	for (pos = lstDelItemsPtr.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItemDel = lstDelItemsPtr.GetNext (pos);
		if (DoRemoveRow (pItemDel, FALSE))
		{
			nCount++;
		}
	}

	if (bRedraw)
	{
		AdjustLayout ();
	}

	return nCount;
}
//*****************************************************************************************
BOOL CBCGPGridCtrl::DoRemoveRow (CBCGPGridRow* pItemDel, BOOL bRedraw)
{
	if (pItemDel == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pItemDel);

	BOOL bIsAutoGroup = pItemDel->IsGroup () && (pItemDel->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
	if (bIsAutoGroup)
	{
		return FALSE;
	}

	POSITION posDel = m_lstItems.Find (pItemDel);
	return DoRemoveRow (posDel, bRedraw);
}
//*****************************************************************************************
BOOL CBCGPGridCtrl::DoRemoveRow (POSITION posDel, BOOL bRedraw)
{
	ASSERT_VALID (this);

	if (posDel != NULL)
	{
		CBCGPGridRow* pItemDel = m_lstItems.GetAt (posDel);
		ASSERT_VALID (pItemDel);

		if (pItemDel->IsSelected ())
		{
			// Cleanup selection:
			m_idActive.SetNull ();
			m_idLastSel.SetNull ();
			SetCurSel (m_idActive, SM_NONE, FALSE);
		}

		// Remove parent-child links:
		CBCGPGridRow* pParent = pItemDel->GetParent ();
		if (pParent != NULL)
		{
			for (POSITION pos = pParent->m_lstSubItems.GetHeadPosition (); pos != NULL; )
			{
				POSITION posSave = pos;

				CBCGPGridRow* pChildRow = pParent->m_lstSubItems.GetNext (pos);
				ASSERT_VALID (pChildRow);

				if (pChildRow == pItemDel)
				{
					pParent->m_lstSubItems.RemoveAt (posSave);
					break;
				}
			}
		}

		for (POSITION pos = pItemDel->m_lstSubItems.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRow* pChildRow = pItemDel->m_lstSubItems.GetNext (pos);
			ASSERT_VALID (pChildRow);

			pChildRow->m_pParent = NULL;
		}


		m_lstItems.RemoveAt (posDel);

		delete pItemDel;

		SetRebuildTerminalItems ();

		if (bRedraw)
		{
			AdjustLayout ();
		}

		return TRUE;
	}

	return FALSE;
}
//*****************************************************************************************
void CBCGPGridCtrl::RemoveAll ()
{
	ASSERT_VALID (this);

	m_idActive.SetNull ();
	m_idLastSel.SetNull ();
	SetCurSel (m_idActive, SM_NONE, FALSE);

	CleanUpAutoGroups ();

	while (!m_lstItems.IsEmpty ())
	{
		delete m_lstItems.RemoveTail ();
	}

	m_ToolTip.Hide ();

	m_lstTerminalItems.RemoveAll ();

	m_pSelRow = NULL;
	m_pSelItem = NULL;
	m_pLastSelRow = NULL;
	m_pLastSelItem = NULL;

	SetRebuildTerminalItems ();
	m_CachedItems.CleanUpCache ();
}
//******************************************************************************************
CBCGPGridRow* CBCGPGridCtrl::GetRow (int nIndex) const
{
	ASSERT_VALID (this);

	if (m_bVirtualMode)
	{
		return ((CBCGPGridCtrl*) this)->GetVirtualRow (nIndex);
	}

	if (IsSortingMode () || IsGrouping ())
	{
		if (nIndex >= 0 && nIndex < m_lstTerminalItems.GetCount ())
		{
			POSITION pos = m_lstTerminalItems.FindIndex (nIndex);
			if (pos != NULL)
			{
				CBCGPGridRow* pItem = m_lstTerminalItems.GetAt (pos);
				ASSERT_VALID (pItem);

				if (pItem->m_nIdRow == nIndex)
				{
					return pItem;
				}
			}

			ASSERT (FALSE);
		}

		if (IsGrouping () && nIndex >= m_lstTerminalItems.GetCount ())
		{
			int nAutoGroupIdx = nIndex - (int) m_lstTerminalItems.GetCount ();
			POSITION pos = m_lstAutoGroups.FindIndex (nAutoGroupIdx);
			if (pos != NULL)
			{
				return m_lstAutoGroups.GetAt (pos);
			}

			ASSERT (FALSE);
		}


		return NULL;
	}

	BOOL bSearchSubItems = TRUE;

	for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pItem->m_nIdRow == nIndex)
		{
			return pItem;
		}

		if (bSearchSubItems) 
		{
			pItem = pItem->FindSubItemById (nIndex);

			if  (pItem != NULL)
			{
				ASSERT_VALID (pItem);
				return pItem;
			}
		}
	}

	return NULL;
}
//******************************************************************************************
CBCGPGridRow* CBCGPGridCtrl::FindRowByData (DWORD_PTR dwData, BOOL bSearchSubItems/* = TRUE*/) const
{
	ASSERT_VALID (this);

	for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pItem->m_dwData == dwData)
		{
			return pItem;
		}

		if (bSearchSubItems) 
		{
			pItem = pItem->FindSubItemByData (dwData);

			if  (pItem != NULL)
			{
				ASSERT_VALID (pItem);
				return pItem;
			}
		}
	}

	return NULL;
}
//*******************************************************************************************
CBCGPGridRow* CBCGPGridCtrl::HitTest (
							CPoint pt, CBCGPGridItemID &id,
							CBCGPGridItem*& pGridItem,
							CBCGPGridRow::ClickArea* pnArea,
							BOOL bPropsOnly)
{
	ASSERT_VALID (this);

	if (!m_rectList.PtInRect (pt) && !bPropsOnly)
	{
		id.SetNull ();
		pGridItem = NULL;
		return NULL;
	}

	if (m_bVirtualMode)
	{
		//-----------------
		// Get virtual row:
		//-----------------
		id = HitTestVirtual (pt, pnArea);

		CBCGPGridRow* pRow = GetVirtualRow (id.m_nRow);

		if (pRow != NULL)
		{
			ASSERT_VALID (pRow);
			pGridItem = pRow->GetItem (id.m_nColumn);
		}

		return pRow;
	}

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst = 
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	int nFirstItem = m_bVirtualMode ?
		m_nVertScrollOffset / m_nRowHeight : m_nFirstVisibleItem;

	POSITION pos = nFirstItem == -1 ?
		lst.GetHeadPosition () : lst.FindIndex (nFirstItem);

	while (pos != NULL)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		if (pItem == NULL)
		{
			continue;
		}

		ASSERT_VALID (pItem);

		int iColumn = -1;
		CBCGPGridRow* pHit = pItem->HitTest (pt, iColumn, pGridItem, pnArea);
		if (pHit != NULL)
		{
			id = CBCGPGridItemID (pHit->m_nIdRow, iColumn);

			BOOL bIsAutoGroup = pItem->IsGroup () && (pItem->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0;
			if (bIsAutoGroup)
			{
				id.m_nColumn = -1;
			}

			return pHit;
		}

		CRect rectRow = pItem->GetRect ();
		if (rectRow.top > m_rectList.bottom)
		{
			break;
		}
    }

	id.SetNull ();
	pGridItem = NULL;
	return NULL;
}
//*******************************************************************************************
void CBCGPGridCtrl::SetCurSel (CBCGPGridRow* pItem, BOOL bRedraw)
{
	ASSERT_VALID (this);

	CBCGPGridItemID id;

	if (pItem == NULL)
	{
		SetCurSel (id, SM_NONE, bRedraw);
	}
	else
	{
		ASSERT_VALID (pItem);

		id.m_nRow =	pItem->GetRowId ();
		id.m_nColumn = -1;

		SetCurSel (id, SM_SINGE_SEL_GROUP | SM_SINGLE_ITEM | SM_ROW, bRedraw);
	}
}
//******************************************************************************************
CBCGPGridRow* CBCGPGridCtrl::GetCurSel () const
{
	if (m_bVirtualMode)
	{
		return ((CBCGPGridCtrl*) this)->GetVirtualRow (m_idActive.m_nRow);
	}
	if (m_pSelRow == NULL)
	{
		if (m_pSelItem != NULL)
		{
			return m_pSelItem->m_pGridRow;
		}

		return NULL;
	}

	return m_pSelRow;
}
//******************************************************************************************
CBCGPGridItem* CBCGPGridCtrl::GetCurSelItem (CBCGPGridRow* pCurRow) const
{
	if (m_bVirtualMode)
	{
		if (pCurRow == NULL)
		{
			pCurRow = GetCurSel ();
		}
		return (pCurRow != NULL) ? pCurRow->GetItem (m_idActive.m_nColumn) : NULL;
	}

	return m_pSelItem;
}
//******************************************************************************************
BOOL CBCGPGridCtrl::SelectColumn (int nColumn, BOOL bRedraw)
{
	CBCGPGridItemID idItem;
	idItem.m_nRow = -1;
	idItem.m_nColumn = nColumn;

	SetCurSel (idItem, SM_SINGE_SEL_GROUP | SM_SINGLE_ITEM, bRedraw);

	return TRUE;
}
//******************************************************************************************
BOOL CBCGPGridCtrl::IsRowSelected (int nRow) const
{
	for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRange* pSelRange = m_lstSel.GetNext (pos);
		ASSERT (pSelRange != NULL);

		if (pSelRange->m_nTop <= nRow && pSelRange->m_nBottom >= nRow)
		{
			return TRUE;
		}
	}

	return FALSE;
}
//******************************************************************************************
BOOL CBCGPGridCtrl::IsColumnSelected (int nColumn) const
{
	if (nColumn < 0 || nColumn >= m_Columns.GetColumnCount ())
	{
		// column not found
		return FALSE;
	}

	for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRange* pSelRange = m_lstSel.GetNext (pos);
		ASSERT (pSelRange != NULL);

		int nPos = m_Columns.IndexToOrder (nColumn);
		int nPosLeft = m_Columns.IndexToOrder (pSelRange->m_nLeft);
		int nPosRight = m_Columns.IndexToOrder (pSelRange->m_nRight);

		if (nPos != -1 && nPosLeft != -1 && nPosRight != -1 &&
			min (nPosLeft, nPosRight) <= nPos && max (nPosLeft, nPosRight) >= nPos)
		{
			return TRUE;
		}
	}

	return FALSE;
}
//******************************************************************************************
// Retrieves list of visible selected items
void CBCGPGridCtrl::GetSelectedItems (CList <CBCGPGridItem*, CBCGPGridItem*> &lstSelected)
{
	if (m_bVirtualMode)
	{
		// Can't work in virtual mode.
		// Storing a pointer is not safe.
		// Use m_lstSel member to get list of selected ranges
		ASSERT (FALSE);
		return;
	}

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	for (POSITION posRange = m_lstSel.GetHeadPosition (); posRange != NULL; )
	{
		CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (posRange);
		ASSERT (pRange != NULL);

		POSITION pos = lst.FindIndex (pRange->m_nTop);
		for (int i = pRange->m_nTop; pos != NULL && i <= pRange->m_nBottom; i++)
		{
			CBCGPGridRow* pRow = lst.GetNext (pos);
			ASSERT_VALID (pRow);

			if (pRow->IsParentExpanded ())
			{
				BOOL bFirstItem = TRUE; // first visible grid item in the specified range
				BOOL bInRange = FALSE;	// is inside the specified range?

				int nPos = m_Columns.Begin ();
				while (nPos != m_Columns.End ())
				{
					int iColumn = m_Columns.Next (nPos);
					if (iColumn == -1)
					{
						break; // no more visible columns
					}

					BOOL bIsRangeBound = (iColumn == pRange->m_nLeft || iColumn == pRange->m_nRight);
					if (bIsRangeBound || bInRange)
					{
						CBCGPGridItem* pItem = pRow->GetItem (iColumn);
						if (pItem != NULL)
						{
							if (!m_bWholeRowSel || bFirstItem)
							{
								lstSelected.AddTail (pItem);
								bFirstItem = FALSE;
							}
						}
					}

					if (bIsRangeBound)
					{
						if (bInRange || pRange->m_nLeft == pRange->m_nRight)
						{
							break;	// last visible column in range
						}

						bInRange = TRUE;
					}

				}
			}
		}
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	SetFocus ();

	CRect rectColumn;
	int nGroupHit = HitTestGroupByBox (point, rectColumn);

	if (nGroupHit >= 0)
	{
		m_ptStartDrag = point;
		StartDragColumn (nGroupHit, rectColumn, TRUE, FALSE);
		return;
	}

	// -------------
	// Track header:
	// -------------
	CPoint ptHeader = point;
	if (OnTrackHeader () && m_rectList.PtInRect (point))
	{
		ptHeader.y = m_rectHeader.top;
	}
	int nColumnHit = m_Columns.HitTestColumn (ptHeader, TRUE, STRETCH_DELTA);
	if (nColumnHit != -1)
	{
		int nLButton = GetSystemMetrics (SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
		if ((GetAsyncKeyState (nLButton) & 0x8000) == 0)
		{
			return;
		}

		if (!m_Columns.CanChangeWidth (nColumnHit))
		{
			// column can't be resized
			return;
		}

		CRect rectHeaderColumn;
		m_Columns.GetColumnRect (nColumnHit, rectHeaderColumn);

		SetCapture ();
		m_nTrackColumn = nColumnHit;
		TrackHeader (rectHeaderColumn.right);
		m_bTracking = TRUE;
		return;
	}

	//--------------
	// Header click:
	//--------------
	CBCGPGridColumnsInfo::ClickArea clickAreaHeader;
	nColumnHit = m_Columns.HitTestColumn (point, FALSE, STRETCH_DELTA, &clickAreaHeader);

	if (nColumnHit >= 0)
	{
		if (clickAreaHeader == CBCGPGridColumnsInfo::ClickHeaderButton)
		{
			CRect rectHeaderColumn;
			m_Columns.GetColumnRect (nColumnHit, rectHeaderColumn);

			CRect rectMenuButton;
			OnHeaderMenuButtonClick (nColumnHit, GetHeaderMenuButtonRect (rectHeaderColumn, nColumnHit));
			return;
		}

		CRect rectHeaderColumn;
		m_Columns.GetColumnRect (nColumnHit, rectHeaderColumn);

		m_ptStartDrag = point;
		StartDragColumn (nColumnHit, rectHeaderColumn, FALSE, FALSE);
		return;
	}

	// --------------
	// Set selection:
	// --------------
	CBCGPGridRow::ClickArea clickArea;
	CBCGPGridItemID id;
	CBCGPGridItem* pHitItem = NULL;
	CBCGPGridRow* pHitRow = HitTest (point, id, pHitItem, &clickArea);

	BOOL bSelChanged = id != m_idLastSel;
	BOOL bIsButtonClick = pHitItem != NULL && pHitItem->m_rectButton.PtInRect (point);

	const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;
	const BOOL bCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0;

	DWORD dwSelMode = SM_NONE;
	if (!id.IsNull ())
	{
		dwSelMode = SM_FIRST_CLICK |
			(m_bSingleSel ? SM_SINGE_SEL_GROUP :
			(bCtrl ? SM_ADD_SEL_GROUP :
			(bShift ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP)));
	}

	if (pHitRow != NULL && id.IsRow ())
	{
		dwSelMode |= SM_ROW;
	}
	if (id.IsColumn ())
	{
		dwSelMode |= SM_COLUMN;
	}
	
	m_bNoUpdateWindow = TRUE; // prevent flickering
	m_pSetSelItem = m_bVirtualMode ? NULL : pHitItem;

	SetCurSel (id, dwSelMode);

	m_pSetSelItem = NULL;
	m_bNoUpdateWindow = FALSE;

	if (id.IsNull () || pHitRow == NULL)
	{
		return;
	}

	ASSERT_VALID (pHitRow);
	EnsureVisible (pHitRow);

	CBCGPGridRow* pCurSel = GetCurSel ();
	CBCGPGridItem* pItem = GetCurSelItem (pCurSel);
	if (id != m_idActive || pCurSel == NULL || (pItem == NULL && clickArea == CBCGPGridRow::ClickValue))
	{
		// The hitten item is not active - do not translate a click to the grid item.
		// Translate a click for single item only.
		return;
	}

	// ------------------------------
	// Translate a click to the item:
	// ------------------------------
	ASSERT_VALID (pCurSel); // pCurSel - hitten row

	switch (clickArea)
	{
	case CBCGPGridRow::ClickExpandBox:
		pCurSel->Expand (!pCurSel->IsExpanded ());
		break;

	case CBCGPGridRow::ClickName:
		pCurSel->OnClickName (point);

		// Start selecting range of items:
		StartSelectItems ();
		break;

	case CBCGPGridRow::ClickValue:
		ASSERT_VALID (pItem);	// pItem - hitten active item
		if (pCurSel->m_bEnabled && pItem->IsEnabled ())
		{
			if (bIsButtonClick || m_bSingleSel)
			{
				DoClickValue (pItem, WM_LBUTTONDOWN, point, bSelChanged, bIsButtonClick);
			}
			else
			{
				// Defer the item click:
				SetTimer (GRID_CLICKVALUE_TIMER_ID, GRID_CLICKVALUE_TIMER_INTERVAL, NULL);
				m_bClickTimer = TRUE;
				m_ptClickOnce = point;
				m_bIsFirstClick = bSelChanged;
				m_bIsButtonClick = bIsButtonClick;

				StartSelectItems ();
				return;
			}
		}
		break;

	default:
		break;
	}
}
//************************************************************************************
void CBCGPGridCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);

	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return;
	}
	
	SetFocus ();

	if (OnTrackHeader () && point.y >= m_rectList.top)
	{
		CPoint ptHeader = point;
		ptHeader.y = m_rectHeader.top;

		if (m_Columns.HitTestColumn (ptHeader, TRUE, STRETCH_DELTA) != -1)
		{
			return;
		}
	}

	CBCGPGridRow::ClickArea clickArea;
	CBCGPGridItemID id;
	CBCGPGridItem* pHitItem = NULL;
	CBCGPGridRow* pHitRow = HitTest (point, id, pHitItem, &clickArea);

	BOOL bSaveSelection = pHitRow != NULL &&
		(pHitItem != NULL ? pHitItem->IsSelected () : pHitRow->IsSelected () );

	BOOL bSelChanged = id != m_idLastSel;

	const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;
	const BOOL bCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0;

	DWORD dwSelMode = SM_NONE;
	if (!id.IsNull ())
	{
		dwSelMode = SM_FIRST_CLICK |
			(bCtrl ? SM_ADD_SEL_GROUP :
			(bShift ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP));
	}

	if (!bSaveSelection)
	{
		m_pSetSelItem = m_bVirtualMode ? NULL : pHitItem;

		SetCurSel (id, dwSelMode);

		m_pSetSelItem = NULL;
	}

	if (pHitRow != NULL)
	{
		ASSERT_VALID (pHitRow);
		EnsureVisible (pHitRow);
	}

	CBCGPGridRow* pCurSel = GetCurSel ();
	CBCGPGridItem* pItem = GetCurSelItem (pCurSel);
	if (id != m_idActive || pCurSel == NULL || pItem == NULL)
	{
		// The hitten item is not active - do not translate a click to the grid item.
		// Translate a click for single item only.
		return;
	}

	ASSERT_VALID (pCurSel); // pCurSel - hitten row
	ASSERT_VALID (pItem);	// pItem - hitten active item

	switch (clickArea)
	{
	case CBCGPGridRow::ClickExpandBox:
		break;

	case CBCGPGridRow::ClickName:
		pCurSel->OnRClickName (point);
		break;

	case CBCGPGridRow::ClickValue:
		pCurSel->OnRClickValue (point, bSelChanged);

		if (pCurSel->m_bEnabled && !bSelChanged && !bSaveSelection)
		{
			if (EditItem (pCurSel, &point) && pCurSel->m_bInPlaceEdit)
			{
				if (pItem->m_rectButton.PtInRect (point))
				{
					return;
				}

				pItem->OnClickValue (WM_RBUTTONDOWN, point);
			}
		}
		break;

	default:
		break;
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonUp(nFlags, point);

	if (m_nDraggedColumn >= 0)
	{
		// Cancel dragging a column:
		CPoint pt(-1, -1);
		StopDragColumn (pt, FALSE);
	}
	if (m_bTracking)
	{
		TrackHeader (-1);
		m_bTracking = FALSE;

		if (::GetCapture () == GetSafeHwnd ())
		{
			ReleaseCapture ();
		}
	}
	if (m_bSelecting)
	{
		KillTimer (GRID_CLICKVALUE_TIMER_ID);
		m_bClickTimer = FALSE;
		m_ptClickOnce = CPoint (0, 0);

		StopSelectItems ();
	}
}
//******************************************************************************************
BOOL CBCGPGridCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create (
		globalData.RegisterWindowClass (_T("BCGPGridCtrl")),
		_T(""), dwStyle, rect, pParentWnd, nID, NULL);
}
//******************************************************************************************
BOOL CBCGPGridCtrl::EditItem (CBCGPGridRow* pItem, LPPOINT lptClick)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	if (!EndEditItem ())
	{
		return FALSE;
	}

	if (pItem->IsGroup () && !pItem->HasValueField ())
	{
		return FALSE;
	}

	ASSERT (pItem == GetCurSel ());

	if (pItem->OnEdit (lptClick))
	{
		pItem->Redraw ();
		//SetCurSel (m_idActive, SM_SET_ACTIVE_ITEM);
		SetCapture ();
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
//******************************************************************************************
void CBCGPGridCtrl::OnClickButton (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPGridRow* pSel = GetCurSel ();
	ASSERT_VALID (pSel);

	if (pSel->OnUpdateValue ())
	{
		CBCGPGridItem* pItem = GetCurSelItem (pSel);
		if (pItem!= NULL)
		{
			pItem->DoClickButton (point);
		}
	}
}
//******************************************************************************************
BOOL CBCGPGridCtrl::EndEditItem (BOOL bUpdateData/* = TRUE*/)
{
	ASSERT_VALID (this);

	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel == NULL)
	{
		return TRUE;
	}

	ASSERT_VALID (pSel);

	if (!pSel->m_bInPlaceEdit)
	{
		return TRUE;
	}

	if (bUpdateData)
	{
		if (!ValidateItemData (pSel) || !pSel->OnUpdateValue ())
		{
			return FALSE;
		}
	}
	else
	{
		m_bUpdateItemData = FALSE;
	}

	if (!pSel->OnEndEdit ())
	{
		if (!bUpdateData)
		{
			m_bUpdateItemData = TRUE;
		}
		return FALSE;
	}

	if (!bUpdateData)
	{
		m_bUpdateItemData = TRUE;
	}

	if (::GetCapture () == GetSafeHwnd ())
	{
		ReleaseCapture ();
	}

	pSel->Redraw ();

	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return CWnd::PreTranslateMessage(pMsg);
	}

   	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCRBUTTONUP:
	case WM_NCMBUTTONUP:
		if (m_ToolTip.GetSafeHwnd () != NULL &&
			m_ToolTip.IsWindowVisible ())
		{
			m_ToolTip.Hide ();

			if (::GetCapture () == GetSafeHwnd ())
			{
				ReleaseCapture ();
			}

			return CWnd::PreTranslateMessage(pMsg);
		}
		break;
		
	case WM_MOUSEMOVE:
		if (pMsg->wParam == 0)	// No buttons pressed
		{
			CPoint ptCursor;
			::GetCursorPos (&ptCursor);
			ScreenToClient (&ptCursor);

			TrackToolTip (ptCursor);
		}
		break;
	}

	CBCGPGridRow* pSel = GetCurSel ();

	if (pMsg->message == WM_SYSKEYDOWN && 
		(pMsg->wParam == VK_DOWN || pMsg->wParam == VK_RIGHT) && 
		pSel != NULL && pSel->m_bEnabled)
	{
		CBCGPGridItem* pItem = GetCurSelItem (pSel);

  		if (pItem != NULL && 
			((pItem->m_dwFlags) & BCGP_GRID_ITEM_HAS_LIST) &&
			EditItem (pSel))
		{
			pItem->DoClickButton (CPoint (-1, -1));
		}

		return TRUE;
	}

	if (pSel != NULL && pSel->m_bInPlaceEdit && pSel->m_bEnabled)
	{
		ASSERT_VALID (pSel);

		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_RETURN:
				{
					CComboBox* pWndCombo = pSel->GetComboWnd ();
					if (pWndCombo != NULL && pWndCombo->GetDroppedState ())
					{
						pSel->OnSelectCombo ();

						CWnd* pWndInPlace = pSel->GetInPlaceWnd ();
						ASSERT_VALID (pWndInPlace);
						pWndInPlace->SetFocus ();
						return TRUE;
					}
				}

				if (!EndEditItem ())
				{
					MessageBeep ((UINT)-1);
				}

				SetFocus ();
				break;

			case VK_ESCAPE:
				EndEditItem (FALSE);
				SetFocus ();
				return TRUE;

			default:
				if (!pSel->m_bAllowEdit)
				{
					pSel->PushChar ((UINT) pMsg->wParam);
					return TRUE;
				}

				if (ProcessClipboardAccelerators ((UINT) pMsg->wParam))
				{
					return TRUE;
				}

				return FALSE;
			}

			return TRUE;
		}
		else if (pMsg->message >= WM_MOUSEFIRST &&
				 pMsg->message <= WM_MOUSELAST)
		{
			CPoint ptCursor;
			::GetCursorPos (&ptCursor);
			ScreenToClient (&ptCursor);

			CSpinButtonCtrl* pWndSpin = pSel->GetSpinWnd ();
			if (pWndSpin != NULL)
			{
				ASSERT_VALID (pWndSpin);
				ASSERT (pWndSpin->GetSafeHwnd () != NULL);

				CRect rectSpin;
				pWndSpin->GetClientRect (rectSpin);
				pWndSpin->MapWindowPoints (this, rectSpin);

				if (rectSpin.PtInRect (ptCursor))
				{
					MapWindowPoints (pWndSpin, &ptCursor, 1); 

					pWndSpin->SendMessage (pMsg->message, pMsg->wParam, 
						MAKELPARAM (ptCursor.x, ptCursor.y));
					return TRUE;
				}
			}

			CWnd* pWndInPlaceEdit = pSel->GetInPlaceWnd ();
			if (pWndInPlaceEdit == NULL)
			{
				return CWnd::PreTranslateMessage(pMsg);
			}

			ASSERT_VALID (pWndInPlaceEdit);

			if (!pSel->m_bAllowEdit)
			{
				pWndInPlaceEdit->HideCaret ();
			}

			CRect rectEdit;
			pWndInPlaceEdit->GetClientRect (rectEdit);
			pWndInPlaceEdit->MapWindowPoints (this, rectEdit);

			if (rectEdit.PtInRect (ptCursor) &&
				pMsg->message == WM_LBUTTONDBLCLK)
			{
				if (pSel->OnDblClick (ptCursor))
				{
					return TRUE;
				}
			}

			if (rectEdit.PtInRect (ptCursor) && 
				pMsg->message == WM_RBUTTONDOWN &&
				!pSel->m_bAllowEdit)
			{
				return TRUE;
			}

			if (!rectEdit.PtInRect (ptCursor) &&
				(pMsg->message == WM_LBUTTONDOWN ||
				pMsg->message == WM_NCLBUTTONDOWN))
			{
				CBCGPGridItem* pItem = GetCurSelItem (pSel);
				if (pItem!= NULL && pItem->m_rectButton.PtInRect (ptCursor))
				{
					pItem->DoClickButton (ptCursor);
					return TRUE;
				}

				if (!EndEditItem ())
				{
					m_bNoUpdateWindow = FALSE;
					return TRUE;
				}
			}
			else
			{
				MapWindowPoints (pWndInPlaceEdit, &ptCursor, 1); 
				pWndInPlaceEdit->SendMessage (pMsg->message, pMsg->wParam, 
					MAKELPARAM (ptCursor.x, ptCursor.y));
				return TRUE;
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}
//*******************************************************************************************
void CBCGPGridCtrl::OnCancelMode() 
{
	if (m_nDraggedColumn >= 0)
	{
		// Cancel dragging a column:
		CPoint pt(-1, -1);
		StopDragColumn (pt, FALSE);
	}
	if (m_bTracking)
	{
		TrackHeader (-1);
		m_bTracking = FALSE;

		if (::GetCapture () == GetSafeHwnd ())
		{
			ReleaseCapture ();
		}
	}
	if (m_bSelecting)
	{
		KillTimer (GRID_CLICKVALUE_TIMER_ID);
		m_bClickTimer = FALSE;
		m_ptClickOnce = CPoint (0, 0);

		StopSelectItems ();
	}

	//---------------------
	// Header highlighting:
	//---------------------
	BOOL bRedraw = FALSE;
	if (m_Columns.GetHighlightColumn () >= 0)
	{
		m_Columns.SetHighlightColumn (-1);
		bRedraw = TRUE;
	}
	if (m_Columns.GetHighlightColumnBtn () >= 0)
	{
		m_Columns.SetHighlightColumnBtn (-1);
		bRedraw = TRUE;
	}

	if (bRedraw)
	{
		RedrawWindow ();
	}

	//---------
	// Tooltip:
	//---------
	if (::GetCapture () == GetSafeHwnd ())
	{
		ReleaseCapture ();
	}
	m_ToolTip.Deactivate ();

	EndEditItem (FALSE);
	CWnd::OnCancelMode();
}
//******************************************************************************************
void CBCGPGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_bFocused = TRUE;
	
	CBCGPGridRow* pSel = GetCurSel ();
	if (pSel != NULL)
	{
		InvalidateRect (pSel->m_Rect);

		for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
			ASSERT (pRange != NULL);

			InvalidateRange (*pRange);
		}

		if (!m_bNoUpdateWindow)
		{
			UpdateWindow ();
		}
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	BOOL bIsParentView =
		pNewWnd != NULL &&
		pNewWnd->IsKindOf (RUNTIME_CLASS (CBCGPGridView)) &&
		GetParent ()->GetSafeHwnd () == pNewWnd->GetSafeHwnd ();

	HWND hwndColumnChooser = 
		(m_pColumnChooser != NULL) ? m_pColumnChooser->GetSafeHwnd () : NULL;

	if (!bIsParentView && !IsChild (pNewWnd) &&
		pNewWnd->GetSafeHwnd () != hwndColumnChooser)
	{
		CBCGPGridRow* pSel = GetCurSel ();

		if (pSel == NULL || pSel->OnKillFocus (pNewWnd))
		{
			EndEditItem (FALSE);
			m_bFocused = FALSE;

			if (pSel != NULL)
			{
				for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL; )
				{
					CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
					ASSERT (pRange != NULL);

					InvalidateRange (*pRange);
				}

				if (m_bVirtualMode)
				{
					pSel = GetCurSel ();

					if (pSel != NULL)
					{
						ASSERT_VALID (pSel);
						pSel->Redraw ();
					}
				}
				else
				{
					pSel->Redraw ();
				}
			}
		}
	}

	CWnd::OnKillFocus(pNewWnd);
}
//******************************************************************************************
void CBCGPGridCtrl::OnStyleChanged (int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	CWnd::OnStyleChanged (nStyleType, lpStyleStruct);
	SetRebuildTerminalItems ();
	AdjustLayout ();
}
//******************************************************************************************
UINT CBCGPGridCtrl::OnGetDlgCode() 
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}
//******************************************************************************************
void CBCGPGridCtrl::SetScrollSizes ()
{
	ASSERT_VALID (this);

	// -----------------------
	// Calculate scroll sizes:
	// -----------------------
	if (m_nRowHeight == 0)
	{
		m_nVertScrollPage = 0;
		m_nVertScrollTotal = 0;
	}
	else
	{
		m_nVertScrollPage = m_rectList.Height ();

		int nGroupsCount = GetGroupsCount (TRUE);
		int nItemsCount = GetTotalItems (TRUE) - nGroupsCount;
		int nPreviewHeightTotal = CalcExtraHeightTotal ();

		m_nVertScrollTotal = nGroupsCount * m_nLargeRowHeight + 
			nItemsCount * m_nRowHeight + nPreviewHeightTotal;
	}

	if (FALSE)
	{
		m_nHorzScrollPage = 0;
		m_nHorzScrollTotal = 0;
	}
	else
	{
		m_nHorzScrollPage = m_rectList.Width ();
		m_nHorzScrollTotal = m_Columns.GetTotalWidth ();
	}

	int cxScroll = m_bScrollVert ? ::GetSystemMetrics (SM_CXVSCROLL) : 0;
	int cyScroll = m_bScrollHorz ? ::GetSystemMetrics (SM_CYHSCROLL) : 0;

	// Scrollbars deflate the visible area
	if (m_nVertScrollTotal > m_nVertScrollPage && !m_Columns.IsAutoSize ())
	{
		m_nHorzScrollPage -= cxScroll;
	}
	if (m_nHorzScrollTotal > m_nHorzScrollPage)
	{
		m_nVertScrollPage -= cyScroll;
	}

	// --------------------------
	// Set vertical scroll sizes:
	// --------------------------
	if (m_wndScrollVert.GetSafeHwnd () == NULL)
	{
		return;
	}
	
	if (m_nRowHeight == 0)
	{
		m_nVertScrollOffset = 0;
	}
	else
	{
		if (m_nVertScrollTotal <= m_nVertScrollPage)
		{
			m_nVertScrollPage = 0;
			m_nVertScrollTotal = 0;
		}

		int nPrevOffset = m_nVertScrollOffset;
		m_nVertScrollOffset = max (0, min (m_nVertScrollOffset, 
			m_nVertScrollTotal - m_nVertScrollPage + 1));
		if (m_bVirtualMode)
		{
			// row alignment
			int nResidue = m_nVertScrollOffset % m_nRowHeight;
			if (nResidue > 0)
			{
				m_nVertScrollOffset -= nResidue;
				if (m_nVertScrollOffset + m_nVertScrollPage < m_nVertScrollTotal)
				{
					m_nVertScrollOffset += m_nRowHeight;
				}
			}
		}

		if (m_nVertScrollOffset != nPrevOffset)
		{
			OnUpdateVScrollPos (m_nVertScrollOffset, nPrevOffset);
		}
	}

	SCROLLINFO si;

	ZeroMemory (&si, sizeof (SCROLLINFO));
	si.cbSize = sizeof (SCROLLINFO);

	si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
	si.nMin = 0;
	si.nMax = m_nVertScrollTotal;
	si.nPage = m_nVertScrollPage;
	si.nPos = m_nVertScrollOffset;

	SetScrollInfo (SB_VERT, &si, TRUE);
	m_wndScrollVert.EnableScrollBar (m_nVertScrollTotal > 0 ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
	m_wndScrollVert.EnableWindow ();

	// ----------------------------
	// Set horizontal scroll sizes:
	// ----------------------------
	if (m_wndScrollHorz.GetSafeHwnd () == NULL)
	{
		return;
	}

	if (FALSE)
	{
		m_nHorzScrollOffset = 0;
	}
	else
	{
		if (m_nHorzScrollTotal <= m_nHorzScrollPage || m_Columns.IsAutoSize ())
		{
			m_nHorzScrollPage = 0;
			m_nHorzScrollTotal = 0;
		}

		m_nHorzScrollOffset = min (max (0, m_nHorzScrollOffset), 
			m_nHorzScrollTotal - m_nHorzScrollPage + 1);
	}

	ZeroMemory (&si, sizeof (SCROLLINFO));
	si.cbSize = sizeof (SCROLLINFO);

	si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
	si.nMin = 0;
	si.nMax = m_nHorzScrollTotal;
	si.nPage = m_nHorzScrollPage;
	si.nPos = m_nHorzScrollOffset;

	SetScrollInfo (SB_HORZ, &si, TRUE);
	m_wndScrollHorz.EnableScrollBar (m_nHorzScrollTotal > 0 ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
	m_wndScrollHorz.EnableWindow ();
}
//******************************************************************************************
int CBCGPGridCtrl::CalcExtraHeightTotal ()
{
	ASSERT_VALID (this);

	if (!IsRowExtraHeightAllowed ())
	{
		return 0;
	}

	if (IsVirtualMode ())
	{
		return 0;
	}

	int nPreviewHeightTotal = 0;
	int nIndex = 0;

	if (!IsSortingMode () && !IsGrouping ())
	{
		for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL;
			nIndex++)
		{
			CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
			ASSERT_VALID (pItem);

			if (!pItem->IsGroup () && pItem->IsItemVisible ())
			{
				int nHeight = pItem->m_Rect.Height ();
				if (nHeight > m_nRowHeight)
				{
					nPreviewHeightTotal += nHeight - m_nRowHeight;
				}
			}
		}
	}
	else
	{
		for (POSITION pos = m_lstTerminalItems.GetHeadPosition (); pos != NULL;
			nIndex++)
		{
			CBCGPGridRow* pItem = m_lstTerminalItems.GetNext (pos);
			ASSERT_VALID (pItem);

			if (!pItem->IsGroup () && pItem->IsItemVisible ())
			{
				int nHeight = pItem->m_Rect.Height ();
				if (nHeight > m_nRowHeight)
				{
					nPreviewHeightTotal += nHeight - m_nRowHeight;
				}
			}
		}
	}

	return nPreviewHeightTotal;
}
//******************************************************************************************
int CBCGPGridCtrl::GetPageItems (int& nFirst, int& nLast, int nSearchFrom) const
{	
	ASSERT_VALID (this);
	ASSERT (nSearchFrom >= 0);

	int nCount = 0;
	nFirst = -1;

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	int i = 0;
	POSITION pos = lst.GetHeadPosition ();
	if (nSearchFrom > 0)
	{
		i = nSearchFrom;
		pos = lst.FindIndex (nSearchFrom);
	}
	for (; pos != NULL; i++)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

		if (!pItem->m_Rect.IsRectEmpty ())
		{
			if (pItem->m_Rect.bottom >= m_rectList.bottom)
			{
				nLast = i - 1;
				return nCount;
			}

			if (pItem->m_Rect.bottom >= m_rectList.top)
			{
				nCount ++;
				
				if (nFirst == -1)
				{
					nFirst = i;
				}
			}
		}
	}

	nLast = i - 1;

	if (nFirst == -1)
	{
		nLast = -1;
	}
	return nCount;
}
//******************************************************************************************
int CBCGPGridCtrl::GetTotalItems (BOOL bCalcVisibleOnly) const
{
	ASSERT_VALID (this);

	if (m_bVirtualMode)
	{
		return m_nVirtualRows;
	}

	if (bCalcVisibleOnly)
	{
		int nCount = 0;
		
		const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
			(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

		BOOL bShowAllItems = (IsSortingMode () && !IsGrouping ());

		for (POSITION pos = lst.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRow* pItem = lst.GetNext (pos);
			ASSERT_VALID (pItem);

			BOOL bShowItem = bShowAllItems ? !pItem->IsItemFiltered () : pItem->IsItemVisible ();
			if (bShowItem)
			{
				nCount++;
			}
		}

		return nCount;
	}

	if (!IsSortingMode () && !IsGrouping ())
	{
		return (int) m_lstItems.GetCount ();
	}
	else
	{
		return (int) m_lstTerminalItems.GetCount ();
	}
}
//******************************************************************************************
int CBCGPGridCtrl::GetTotalItems (int nCountFrom, int nCountTo, 
								  BOOL bCalcVisibleOnly) const
{
	ASSERT_VALID (this);
	
	if (nCountTo < 0 || nCountFrom < 0)
	{
		return 0;
	}

	if (nCountTo < nCountFrom)
	{
		int nTmp = nCountFrom;
		nCountFrom = nCountTo;
		nCountTo = nTmp;
	}

	if (m_bVirtualMode)
	{
		nCountFrom = min (m_nVirtualRows, nCountFrom);
		nCountTo = min (m_nVirtualRows, nCountTo);
		return nCountTo - nCountFrom;
	}

	int nCount = 0;

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	BOOL bShowAllItems = (IsSortingMode () && !IsGrouping ());

	int i = 0;
	for (POSITION pos = lst.GetHeadPosition (); pos != NULL && i < nCountTo; i++)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

		if (i >= nCountFrom)
		{
			if (bCalcVisibleOnly)
			{
				BOOL bShowItem = bShowAllItems ? !pItem->IsItemFiltered () : pItem->IsItemVisible ();
				if (bShowItem)
				{
					nCount++;
				}
			}
			else
			{
				nCount++;
			}
		}
	}

	return nCount;
}
//******************************************************************************************
int CBCGPGridCtrl::GetGroupsCount (BOOL bCalcVisibleOnly) const
{
	ASSERT_VALID (this);

	if (m_bVirtualMode)
	{
		return 0;
	}

	int nCount = 0;
	
	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	BOOL bShowAllItems = (IsSortingMode () && !IsGrouping ());

	for (POSITION pos = lst.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

        if (bCalcVisibleOnly)
        {
			BOOL bShowItem = bShowAllItems ? !pItem->IsItemFiltered () : pItem->IsItemVisible ();
			if (pItem->IsGroup () && bShowItem)
			{
				nCount++;
			}
        }
        else
        {
			if (pItem->IsGroup ())
			{
				nCount++;
			}
        }
	}

	return nCount;
}
//******************************************************************************************
int CBCGPGridCtrl::GetGroupsCount (int nCountFrom, int nCountTo,
								   BOOL bCalcVisibleOnly) const
{
	ASSERT_VALID (this);
	ASSERT (nCountFrom >= 0);
	ASSERT (nCountTo >= 0);

	if (m_bVirtualMode)
	{
		return 0;
	}

	if (nCountTo < nCountFrom)
	{
		int nTmp = nCountFrom;
		nCountFrom = nCountTo;
		nCountTo = nTmp;
	}

	int nCount = 0;

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	BOOL bShowAllItems = (IsSortingMode () && !IsGrouping ());

	int i = 0;
	for (POSITION pos = lst.GetHeadPosition (); pos != NULL && i < nCountTo; i++)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

		if (i >= nCountFrom)
		{
			if (bCalcVisibleOnly)
			{
				BOOL bShowItem = bShowAllItems ? !pItem->IsItemFiltered () : pItem->IsItemVisible ();
				if (pItem->IsGroup () && bShowItem)
				{
					nCount++;
				}
			}
			else
			{
				if (pItem->IsGroup ())
				{
					nCount++;
				}
			}
		}
	}

	return nCount;
}
//******************************************************************************************
// Goes up or down from nStartFrom, skips invisible rows.
// nOffsetCount - count of the visible rows
// Returns an index of the row
int CBCGPGridCtrl::OffsetVisibleRow (int nStartFrom, int nOffsetCount, BOOL bDirForward) const
{
	ASSERT_VALID (this);

	if (nOffsetCount <= 0 || nStartFrom < 0)
	{
		return nStartFrom;
	}

	if (m_bVirtualMode)
	{
		int nResult = ((bDirForward) ? nStartFrom + nOffsetCount : nStartFrom - nOffsetCount);
		nResult = max (0, nResult);
		return min (m_nVirtualRows, nResult);
	}

	int nCount = 0;

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	BOOL bShowAllItems = (IsSortingMode () && !IsGrouping ());

	int iResult = nStartFrom;
	for (POSITION pos = lst.FindIndex (nStartFrom); pos != NULL && nCount < nOffsetCount; )
	{
		CBCGPGridRow* pItem = bDirForward ? lst.GetNext (pos) : lst.GetPrev (pos);
		ASSERT_VALID (pItem);

		BOOL bShowItem = bShowAllItems ? !pItem->IsItemFiltered () : pItem->IsItemVisible ();
		if (bShowItem)
		{
			nCount++;
		}
		if (bDirForward) 
		{
			iResult++;
		}
		else
		{
			iResult--;
		}
	}

	return iResult;
}
//******************************************************************************************
void CBCGPGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return;
	}

	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel != NULL && 
		pScrollBar->GetSafeHwnd () != NULL)
	{
		CSpinButtonCtrl* pWndSpin = pSel->GetSpinWnd ();
		if (pWndSpin != NULL && 
			pWndSpin->GetSafeHwnd () == pScrollBar->GetSafeHwnd ())
		{
			return;
		}
	}

	EndEditItem (FALSE);

	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_TRACKPOS;

	if (nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION)
	{
		GetScrollInfo(SB_VERT, &info);
		nPos = info.nTrackPos;
	}

	int nPrevOffset = m_nVertScrollOffset;

	int nVertScrollPage = m_nVertScrollPage;
	if (m_bVirtualMode)
	{
		int nItemsCount = max (1, m_nVertScrollPage / m_nRowHeight);
		nVertScrollPage = nItemsCount * m_nRowHeight;
	}

	switch (nSBCode)
	{
	case SB_LINEUP:
		m_nVertScrollOffset -= m_nRowHeight;
		break;

	case SB_LINEDOWN:
		m_nVertScrollOffset += m_nRowHeight;
		break;

	case SB_TOP:
		m_nVertScrollOffset = 0;
		break;

	case SB_BOTTOM:
		m_nVertScrollOffset = m_nVertScrollTotal;
		break;

	case SB_PAGEUP:
		m_nVertScrollOffset -= nVertScrollPage;
		break;

	case SB_PAGEDOWN:
		m_nVertScrollOffset += nVertScrollPage;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_nVertScrollOffset = nPos;
		break;

	default:
		return;
	}

	m_nVertScrollOffset = max (0, min (m_nVertScrollOffset, 
		m_nVertScrollTotal - nVertScrollPage + 1));
	if (m_bVirtualMode)
	{
		// row alignment
		int nResidue = m_nVertScrollOffset % m_nRowHeight;
		if (nResidue > 0)
		{
			m_nVertScrollOffset -= nResidue;
			if (m_nVertScrollOffset + m_nVertScrollPage < m_nVertScrollTotal)
			{
				m_nVertScrollOffset += m_nRowHeight;
			}
		}
	}

	if (m_nVertScrollOffset == nPrevOffset)
	{
		return;
	}

	OnUpdateVScrollPos (m_nVertScrollOffset, nPrevOffset);

	SetScrollPos (SB_VERT, m_nVertScrollOffset);

	int dy = nPrevOffset - m_nVertScrollOffset;

	ShiftItems (0, dy);
	OnPosSizeChanged ();
	m_rectTrackSel = OnGetSelectionRect ();
	ScrollWindow (0, dy, m_rectList, m_rectList);

	pSel = GetCurSel ();
	if (pSel != NULL)
	{
		pSel->AdjustButtonRect ();
	}
	
	UpdateWindow ();
}
//*******************************************************************************************
void CBCGPGridCtrl::OnUpdateVScrollPos (int /*nVOffset*/, int /*nPrevVOffset*/)
{
}
//*******************************************************************************************
void CBCGPGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return;
	}

	EndEditItem (FALSE);

	int nPrevOffset = m_nHorzScrollOffset;

	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_TRACKPOS;

	if (nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION)
	{
		GetScrollInfo(SB_HORZ, &info);
		nPos = info.nTrackPos;
	}

	switch (nSBCode)
	{
	case SB_LINEUP:
		m_nHorzScrollOffset -= max (1, m_nRowHeight);
		break;

	case SB_LINEDOWN:
		m_nHorzScrollOffset += max (1, m_nRowHeight);
		break;

	case SB_TOP:
		m_nHorzScrollOffset = 0;
		break;

	case SB_BOTTOM:
		m_nHorzScrollOffset = m_nHorzScrollTotal;
		break;

	case SB_PAGEUP:
		m_nHorzScrollOffset -= m_nHorzScrollPage;
		break;

	case SB_PAGEDOWN:
		m_nHorzScrollOffset += m_nHorzScrollPage;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_nHorzScrollOffset = nPos;
		break;

	default:
		return;
	}

	m_nHorzScrollOffset = min (max (0, m_nHorzScrollOffset), 
		m_nHorzScrollTotal - m_nHorzScrollPage + 1);

	if (m_nHorzScrollOffset == nPrevOffset)
	{
		return;
	}

	SetScrollPos (SB_HORZ, m_nHorzScrollOffset);

	int dx = nPrevOffset - m_nHorzScrollOffset;

	ShiftItems (dx, 0);
	OnPosSizeChanged ();
	m_rectTrackSel = OnGetSelectionRect ();
	ScrollWindow (dx, 0, m_rectList, m_rectList);

	if (!m_rectHeader.IsRectEmpty ())
	{
		RedrawWindow (m_rectHeader);
	}

	if (m_nGridHeaderHeight > 0)
	{
		RedrawWindow (GetGridHeaderRect ());
	}

	if (m_nGridFooterHeight > 0)
	{
		RedrawWindow (GetGridFooterRect ());
	}
}
//*******************************************************************************************
CScrollBar* CBCGPGridCtrl::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_HORZ && m_wndScrollHorz.GetSafeHwnd () != NULL)
	{
		return (CScrollBar* ) &m_wndScrollHorz;
	}
	
	if (nBar == SB_VERT && m_wndScrollVert.GetSafeHwnd () != NULL)
	{
		return (CScrollBar* ) &m_wndScrollVert;
	}

	return NULL;
}
//******************************************************************************************
BOOL CBCGPGridCtrl::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/) 
{
#ifndef _BCGPGRID_STANDALONE
	if (CBCGPPopupMenu::GetActiveMenu () != NULL)
	{
		return TRUE;
	}
#endif

	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return FALSE;
	}

	int nSteps = abs(zDelta) / WHEEL_DELTA;

	for (int i = 0; i < nSteps; i++)
	{
		OnVScroll (zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, NULL);
	}

	return TRUE;
}
//*******************************************************************************************
void CBCGPGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);

	// if header double click:
    if (m_rectHeader.PtInRect (point))
    {
		if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
		{
			// do nothing
		}
		else
		{
			int nClickColumn = m_Columns.HitTestColumn (point, TRUE, STRETCH_DELTA);
			if (nClickColumn >= 0)
			{
				OnHeaderDividerDblClick (nClickColumn);
			}
		}

		return;
    }
	
	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel == NULL || !pSel->GetRect ().PtInRect (point))
	{
		return;
	}

	ASSERT_VALID (pSel);

	if (pSel->IsGroup ())
	{
		CBCGPGridRow::ClickArea clickArea;
		CBCGPGridItemID id;
		CBCGPGridItem* pHitItem = NULL;
		HitTest (point, id, pHitItem, &clickArea);

		if (clickArea != CBCGPGridRow::ClickExpandBox)
		{
			pSel->Expand (!pSel->IsExpanded ());
		}
	}
	else if (pSel->m_bEnabled)
	{
		CWnd* pWndInPlace = pSel->GetInPlaceWnd ();
		if (EditItem (pSel) && pWndInPlace != NULL)
		{
			pWndInPlace->SendMessage (WM_LBUTTONDOWN);
			pWndInPlace->SendMessage (WM_LBUTTONUP);
		}

		pSel->OnDblClick (point);
	}
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		CPoint point;

		::GetCursorPos (&point);
		ScreenToClient (&point);

 		if (m_rectList.PtInRect (point) || m_rectHeader.PtInRect (point))
		{
			if (OnTrackHeader () || m_rectHeader.PtInRect (point))
			{
				CPoint ptHeader = point;
				ptHeader.y = m_rectHeader.top;

				int nColumnHit = m_Columns.HitTestColumn (ptHeader, TRUE, STRETCH_DELTA);
				if (nColumnHit != -1)
				{
					if (m_Columns.GetColumnWidth (nColumnHit) > 0)
					{
						if (m_Columns.CanChangeWidth (nColumnHit))
						{
							::SetCursor (globalData.m_hcurStretch);
							return TRUE;
						}
					}
				}
			}

			CBCGPGridRow::ClickArea clickArea;
			CBCGPGridItemID id;
			CBCGPGridItem* pHitItem = NULL;
			HitTest (point, id, pHitItem, &clickArea);

			if (pHitItem != NULL && clickArea == CBCGPGridRow::ClickValue)
			{
				BOOL bIsOverButton = pHitItem->m_rectButton.PtInRect (point);
				if (!bIsOverButton && pHitItem->OnSetCursor ())
				{
					return TRUE;
				}
			}
		}
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
//******************************************************************************************
void CBCGPGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
	{
		if (m_nDraggedColumn >= 0)
		{
			// Cancel dragging a column:
			CPoint pt(-1, -1);
			StopDragColumn (pt, FALSE);
		}
		if (m_bTracking)
		{
			// Cancel resizing a column:
			TrackHeader (-1);
			m_bTracking = FALSE;

			if (::GetCapture () == GetSafeHwnd ())
			{
				ReleaseCapture ();
			}
		}
	}

	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	
	if (m_lstItems.GetCount() == 0 && !m_bVirtualMode)
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	CBCGPGridRow* pSel = GetCurSel ();

	switch (nChar)
	{
	case VK_F2:
		if (pSel != NULL && pSel->m_bEnabled)
		{
			EditItem (pSel);
		}
		break;

	case VK_F4:
		if (pSel != NULL && pSel->m_bEnabled && EditItem (pSel))
		{
			CBCGPGridItem* pItem = GetCurSelItem (pSel);
			if (pItem != NULL && 
				((pItem->m_dwFlags) & (BCGP_GRID_ITEM_HAS_BUTTON | BCGP_GRID_ITEM_HAS_LIST)))
			{
				pItem->DoClickButton (CPoint (-1, -1));
			}
			return;
		}
		break;

	case VK_SPACE:
		if (!m_bWholeRowSel && !m_bSingleSel)
		{
			const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;
			const BOOL bCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0;

			if (bShift && bCtrl) 
			{
				if (!m_idActive.IsNull ())
				{
					SetCurSel (m_idActive, SM_ALL);
				}
				return;
			}
			else if (bShift)
			{
				if (!m_idLastSel.IsNull ())
				{
					SetCurSel (m_idLastSel, SM_SINGLE_ITEM | SM_CONTINUE_SEL_GROUP | SM_ROW);
				}
				return;
			}
			else if (bCtrl)
			{
				if (!m_idLastSel.IsNull ())
				{
					SetCurSel (m_idLastSel, SM_SINGLE_ITEM | SM_CONTINUE_SEL_GROUP | SM_COLUMN);
				}
				return;
			}
		}
		break;

	case 0x41: // Ctrl+ A
		if (!m_bSingleSel && (::GetAsyncKeyState (VK_CONTROL) & 0x8000) != 0)
		{
			SetCurSel (m_idActive, SM_ALL);
			return;
		}
		break;

	case VK_LEFT:
		if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
		{
			return;
		}
		else if (pSel != NULL && pSel->IsGroup () && pSel->IsExpanded () && 
			(!IsSortingMode () || (pSel->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0) &&
			!pSel->m_lstSubItems.IsEmpty ())
		{
			pSel->Expand (FALSE);
			return;
		}
		else if (!m_bWholeRowSel)
		{
			const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

			// Go left to the nearest item:
			CBCGPGridRow* pRowCurSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
			CBCGPGridItem* pItemCurSel = (bShift && !m_bSingleSel) ? m_pLastSelItem : m_pSelItem;
			if (m_bVirtualMode)
			{
				CBCGPGridItemID idCurSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
				pRowCurSel = GetVirtualRow (idCurSel.m_nRow);
				pItemCurSel = (pRowCurSel != NULL) ? pRowCurSel->GetItem (idCurSel.m_nColumn) : NULL;
			}

			CBCGPGridItemID id;
			CBCGPGridItem* pItem = NULL;

			if (pItemCurSel != NULL)
			{
				if (m_bVirtualMode)
				{
					id = pItemCurSel->GetGridItemID ();
					//id.m_nColumn --;
					int nPos = m_Columns.IndexToOrder (id.m_nColumn);

					// Get position order of the merged cell
					if (pItemCurSel->GetMergedCells () != NULL)
					{
						CBCGPGridRange range;
						if (pItemCurSel->GetMergedRange (range))
						{
							int nPosLeft = m_Columns.IndexToOrder (range.m_nLeft);
							int nPosRight = m_Columns.IndexToOrder (range.m_nRight);
							if (nPosLeft != -1 && nPosRight != -1)
							{
								nPos = max (nPosLeft, nPosRight);
							}
							else
							{
								nPos = -1;
							}
						}
					}

					if (nPos > 0)
					{
						id.m_nColumn = m_Columns.OrderToIndex (--nPos);
					}
					else
					{
						id.m_nColumn = -1;
					}
				}
				else
				{
					CRect rect = pItemCurSel->GetRect ();
					if (pItemCurSel->GetMergedCells () != NULL)
					{
						rect.left = pItemCurSel->GetMergedRect ().left;
					}
					CPoint point (rect.left - 2, rect.bottom - 1);

					int nColumn = -1;
					if (pItemCurSel->m_pGridRow->HitTest (point, nColumn, pItem) != NULL)
					{
						id = CBCGPGridItemID (pItemCurSel->m_pGridRow->GetRowId (), nColumn);
					}
				}
			}
				
			if (!id.IsNull () && pRowCurSel != NULL &&
				!(id.IsRow () && m_nHorzScrollOffset > 0)) // first scroll, then select thole row
			{
				if (id.IsRow () && (bShift && !m_bSingleSel))
				{
					// do not select thole row when "shift")
				}
				else
				{
					DWORD dwSelMode = SM_SINGLE_ITEM |
						((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);
					m_pSetSelItem = m_bVirtualMode ? NULL : pItem;

					SetCurSel (id, dwSelMode);

					m_pSetSelItem = NULL;

					EnsureVisible (pRowCurSel);
				}
			}
			else
			{
				OnHScroll (SB_TOP, 0, NULL);
			}
			return;
		}
		
		// else ==> act as VK_UP!

	case VK_UP:
		{
			if (pSel == NULL)
			{
				SetCurSel (CBCGPGridItemID (0, 0));
				OnVScroll (SB_TOP, 0, NULL);
				return;
			}

			// Select prev. item:
			const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

			CBCGPGridItemID idCurSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
			CBCGPGridRow* pRowCurSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
			CBCGPGridItem* pItemCurSel = (bShift && !m_bSingleSel) ? m_pLastSelItem : m_pSelItem;
			if (m_bVirtualMode)
			{
				pRowCurSel = GetVirtualRow (idCurSel.m_nRow);
				pItemCurSel = (pRowCurSel != NULL) ? pRowCurSel->GetItem (idCurSel.m_nColumn) : NULL;
			}
			if (pRowCurSel != NULL)
			{
				ASSERT_VALID (pRowCurSel);
				CRect rect = pRowCurSel->GetRect ();
				if (pItemCurSel != NULL && pItemCurSel->GetMergedCells () != NULL)
				{
					rect.top = pItemCurSel->GetMergedRect ().top;
				}
				CPoint point (rect.right - 1, rect.top - 2);

				CBCGPGridRow* pRowNewSel = HitTest (point, NULL, TRUE);

				if (pRowNewSel != NULL)
				{
					ASSERT_VALID (pRowNewSel);

					CBCGPGridItemID id (pRowNewSel->GetRowId (), idCurSel.m_nColumn);

					if (!id.IsNull ())
					{
						DWORD dwSelMode = SM_SINGLE_ITEM |
							((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);

						m_bNoUpdateWindow = TRUE; // prevent flickering
						SetCurSel (id, dwSelMode);
						m_bNoUpdateWindow = FALSE;

						EnsureVisible (pRowNewSel);
					}
				}
				else
				{
					OnVScroll (SB_TOP, 0, NULL);
				}
			}
			return;
		}

	case VK_RIGHT:
		if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
		{
			return;
		}
		else if (pSel != NULL && pSel->IsGroup () && !pSel->IsExpanded () && 
			(!IsSortingMode () || (pSel->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0))
		{
			pSel->Expand ();
			return;
		}
		else if (!m_bWholeRowSel)
		{
			const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

			// Go right to the nearest item:
			CBCGPGridRow* pRowCurSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
			CBCGPGridItem* pItemCurSel = (bShift && !m_bSingleSel) ? m_pLastSelItem : m_pSelItem;
			if (m_bVirtualMode)
			{
				CBCGPGridItemID idCurSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
				pRowCurSel = GetVirtualRow (idCurSel.m_nRow);
				pItemCurSel = (pRowCurSel != NULL) ? pRowCurSel->GetItem (idCurSel.m_nColumn) : NULL;
			}

			CBCGPGridItemID id;
			CBCGPGridItem* pItem = NULL;

			if (pItemCurSel != NULL)
			{
				if (m_bVirtualMode)
				{
					id = pItemCurSel->GetGridItemID ();
					//id.m_nColumn ++;
					int nPos = m_Columns.IndexToOrder (id.m_nColumn);

					// Get position order of the merged cell
					if (pItemCurSel->GetMergedCells () != NULL)
					{
						CBCGPGridRange range;
						if (pItemCurSel->GetMergedRange (range))
						{
							int nPosLeft = m_Columns.IndexToOrder (range.m_nLeft);
							int nPosRight = m_Columns.IndexToOrder (range.m_nRight);
							if (nPosLeft != -1 && nPosRight != -1)
							{
								nPos = max (nPosLeft, nPosRight);
							}
							else
							{
								nPos = -1;
							}
						}
					}

					if (nPos >= 0 && nPos < m_Columns.GetColumnCount (TRUE) - 1)
					{
						id.m_nColumn = m_Columns.OrderToIndex (++nPos);
					}
					else
					{
						id.m_nColumn = -1;
					}
				}
				else
				{
					CRect rect = pItemCurSel->GetRect ();
					if (pItemCurSel->GetMergedCells () != NULL)
					{
						rect.right = pItemCurSel->GetMergedRect ().right;
					}
					CPoint point (rect.right + 2, rect.bottom - 1);

					int nColumn = -1;
					if (pItemCurSel->m_pGridRow->HitTest (point, nColumn, pItem) != NULL)
					{
						id = CBCGPGridItemID (pItemCurSel->m_pGridRow->GetRowId (), nColumn);
					}

				}
			}
			else if (pRowCurSel != NULL)
			{
				id = CBCGPGridItemID (pRowCurSel->GetRowId (), m_Columns.GetFirstVisibleColumn ());
			}

			if (!id.IsNull () && pRowCurSel != NULL &&
				!id.IsRow ()) // scroll, never select thole row
			{
				DWORD dwSelMode = SM_SINGLE_ITEM |
					((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);
				m_pSetSelItem = m_bVirtualMode ? NULL : pItem;

				SetCurSel (id, dwSelMode);

				m_pSetSelItem = NULL;

				EnsureVisible (pRowCurSel);
			}
			else
			{
				OnHScroll (SB_BOTTOM, 0, NULL);
			}
			return;
		}
		
		// else ==> act as VK_DOWN!

	case VK_DOWN:
		{
			if (pSel == NULL)
			{
				SetCurSel (CBCGPGridItemID (0, 0));
				OnVScroll (SB_TOP, 0, NULL);
				return;
			}

			if ((::GetAsyncKeyState (VK_MENU) & 0x8000) && nChar == VK_DOWN)
			{
				CBCGPGridItem* pItem = GetCurSelItem (pSel);
				if (pItem != NULL)
				{
					pItem->DoClickButton (CPoint (-1, -1));
				}

				return;
			}

			// Select next item:
			const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

			CBCGPGridItemID idCurSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
			CBCGPGridRow* pRowCurSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
			CBCGPGridItem* pItemCurSel = (bShift && !m_bSingleSel) ? m_pLastSelItem : m_pSelItem;
			if (m_bVirtualMode)
			{
				pRowCurSel = GetVirtualRow (idCurSel.m_nRow);
				pItemCurSel = (pRowCurSel != NULL) ? pRowCurSel->GetItem (idCurSel.m_nColumn) : NULL;
			}
			if (pRowCurSel != NULL)
			{
				ASSERT_VALID (pRowCurSel);
				CRect rect = pRowCurSel->GetRect ();
				if (pItemCurSel != NULL && pItemCurSel->GetMergedCells () != NULL)
				{
					rect.bottom = pItemCurSel->GetMergedRect ().bottom;
				}
				CPoint point (rect.right - 1, rect.bottom + 2);

				CBCGPGridRow* pRowNewSel = HitTest (point, NULL, TRUE);

				if (pRowNewSel != NULL)
				{
					ASSERT_VALID (pRowNewSel);

					CBCGPGridItemID id (pRowNewSel->GetRowId (), idCurSel.m_nColumn);

					if (!id.IsNull ())
					{
						DWORD dwSelMode = SM_SINGLE_ITEM |
							((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);

						m_bNoUpdateWindow = TRUE; // prevent flickering
						SetCurSel (id, dwSelMode);
						m_bNoUpdateWindow = FALSE;

						EnsureVisible (pRowNewSel);
					}
				}
				else
				{
					//OnVScroll (SB_BOTTOM, 0, NULL);
				}
			}
			return;
		}

	case VK_NEXT:
		{
			if (pSel == NULL)
			{
				SetCurSel (CBCGPGridItemID (0, 0));
				OnVScroll (SB_TOP, 0, NULL);
				return;
			}

			if (m_nVertScrollPage != 0)
			{
				const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

				CBCGPGridItemID idCurSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
				CBCGPGridRow* pRowCurSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
				if (m_bVirtualMode)
				{
					pRowCurSel = GetVirtualRow (idCurSel.m_nRow);
				}

				if (pRowCurSel != NULL)
				{
					//OnVScroll (SB_PAGEDOWN, 0, NULL);

					// Get row id:
					ASSERT_VALID (pRowCurSel);
					CRect rect = pRowCurSel->GetRect ();
					CPoint point (rect.right - 1,
								  max (rect.top + m_nVertScrollPage - m_nRowHeight,
									   rect.bottom + 1));

					CBCGPGridRow* pRowNewSel = HitTest (point, NULL, TRUE);

					CBCGPGridItemID id;
					if (pRowNewSel != NULL)
					{
						ASSERT_VALID (pRowNewSel);
						id = CBCGPGridItemID (pRowNewSel->GetRowId (), idCurSel.m_nColumn);
					}
					else
					{
						//---------------------
						// Calculate page size:
						//---------------------
						int nFirst = -1, nLast = -1;
						int nPageItems = GetPageItems (nFirst, nLast, m_nFirstVisibleItem);
						if (nPageItems <= 0)
						{
							nPageItems = 1;
						}

						const int nLastRow = GetTotalItems () - 1;
						const int nNextRow = OffsetVisibleRow (pRowCurSel->GetRowId (), nPageItems, TRUE);
						id = CBCGPGridItemID (min (nNextRow, nLastRow), idCurSel.m_nColumn);
					}

					if (!id.IsNull ())
					{
						// Change selection:
						DWORD dwSelMode = SM_SINGLE_ITEM |
							((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);
						SetCurSel (id, dwSelMode);
						
						pRowNewSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
						if (m_bVirtualMode)
						{
							CBCGPGridItemID idNewSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
							pRowNewSel = GetVirtualRow (idNewSel.m_nRow);
						}

						// Scroll:
						if (pRowNewSel != NULL)
						{
							EnsureVisible (pRowNewSel);
						}
						else
						{
							OnVScroll (SB_BOTTOM, 0, NULL);
						}
					}
				}
			}
			return;
		}

	case VK_END:
		{
			const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

			const int nRowCount = GetTotalItems ();

			CBCGPGridItemID id;
			id.m_nRow = nRowCount - 1;
			id.m_nColumn = (bShift && !m_bSingleSel && !m_idLastSel.IsNull ()) ?
				m_idLastSel.m_nColumn :
				((!m_idActive.IsNull ()) ? m_idActive.m_nColumn : 0);

			DWORD dwSelMode = SM_SINGLE_ITEM |
				((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);

			SetCurSel (id, dwSelMode);
			OnVScroll (SB_BOTTOM, 0, NULL);
		}
		return;

	case VK_PRIOR:
		{
			if (pSel == NULL)
			{
				SetCurSel (CBCGPGridItemID (0, 0));
				OnVScroll (SB_TOP, 0, NULL);
				return;
			}

			if (m_nVertScrollPage != 0)
			{
				const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

				CBCGPGridItemID idCurSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
				CBCGPGridRow* pRowCurSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
				if (m_bVirtualMode)
				{
					pRowCurSel = GetVirtualRow (idCurSel.m_nRow);
				}
				
				if (pRowCurSel != NULL)
				{
					//OnVScroll (SB_PAGEUP, 0, NULL);
					
					// Get row id:
					ASSERT_VALID (pRowCurSel);
					CRect rect = pRowCurSel->GetRect ();
					CPoint point (rect.right - 1,
								  min (rect.bottom - m_nVertScrollPage + m_nRowHeight,
									   rect.top - 1));

					CBCGPGridRow* pRowNewSel = HitTest (point, NULL, TRUE);

					CBCGPGridItemID id;
					if (pRowNewSel != NULL)
					{
						ASSERT_VALID (pRowNewSel);
						id = CBCGPGridItemID (pRowNewSel->GetRowId (), idCurSel.m_nColumn);
					}
					else
					{
						//---------------------
						// Calculate page size:
						//---------------------
						int nFirst = -1, nLast = -1;
						int nPageItems = GetPageItems (nFirst, nLast, m_nFirstVisibleItem);
						if (nPageItems <= 0)
						{
							nPageItems = 1;
						}

						const int nFirstRow = 0;
						const int nPrevRow = OffsetVisibleRow (pRowCurSel->GetRowId (), nPageItems, FALSE);
						id = CBCGPGridItemID (max (nPrevRow, nFirstRow), idCurSel.m_nColumn);
					}
					
					if (!id.IsNull ())
					{
						// Change selection:
						DWORD dwSelMode = SM_SINGLE_ITEM |
							((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);
						SetCurSel (id, dwSelMode);
			
						pRowNewSel = (bShift && !m_bSingleSel) ? m_pLastSelRow : m_pSelRow;
						if (m_bVirtualMode)
						{
							CBCGPGridItemID idNewSel = (bShift && !m_bSingleSel) ? m_idLastSel: m_idActive;
							pRowNewSel = GetVirtualRow (idNewSel.m_nRow);
						}

						// Scroll:
						if (pRowNewSel != NULL)
						{
							EnsureVisible (pRowNewSel);
						}
						else
						{
							OnVScroll (SB_TOP, 0, NULL);
						}
					}
				}
			}
			return;
		}

	case VK_HOME:
		{
			const BOOL bShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000) != 0;

			CBCGPGridItemID id;
			id.m_nRow = 0;
			id.m_nColumn = (bShift && !m_bSingleSel && !m_idLastSel.IsNull ()) ?
				m_idLastSel.m_nColumn :
				((!m_idActive.IsNull ()) ? m_idActive.m_nColumn : 0);

			DWORD dwSelMode = SM_SINGLE_ITEM |
				((bShift && !m_bSingleSel) ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP);
			
			SetCurSel (id, dwSelMode);
			OnVScroll (SB_TOP, 0, NULL);
		}

		return;

	case VK_ADD:
		if (pSel != NULL && pSel->IsGroup () && !pSel->IsExpanded () &&
			!pSel->IsInPlaceEditing () &&
			(!IsSortingMode () || (pSel->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0))
		{
			pSel->Expand ();
		}
		return;

	case VK_SUBTRACT:
		if (pSel != NULL && pSel->IsGroup () && pSel->IsExpanded () && 
			!pSel->IsInPlaceEditing () &&
			(!IsSortingMode () || (pSel->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0))
		{
			pSel->Expand (FALSE);
		}
		return;

	case VK_RETURN:
		if (pSel != NULL && pSel->IsGroup () && !pSel->IsInPlaceEditing () &&
			(!IsSortingMode () || (pSel->m_dwFlags & BCGP_GRID_ITEM_AUTOGROUP) != 0))
		{
			pSel->Expand (!pSel->IsExpanded ());
		}
		return;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
//******************************************************************************************
void CBCGPGridCtrl::EnsureVisible (CBCGPGridRow* pRowItem, BOOL bExpandParents/* = FALSE*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRowItem);

	if (m_nRowHeight == 0)
	{
		return;
	}

	if (bExpandParents && !IsSortingMode () && !IsGrouping () && pRowItem->GetParent () != NULL)
	{
		CBCGPGridRow* pParent = pRowItem;
        
		while ((pParent = pParent->GetParent ()) != NULL)
		{
			ASSERT_VALID (pParent);
			pParent->Expand (TRUE);
		}
	}

	// ------------------
	// Scroll vertically:
	// ------------------
	CRect rectRow = pRowItem->m_Rect;

	if (rectRow.top < m_rectList.top - 1 || rectRow.bottom > m_rectList.bottom)
	{
		CBCGPGridItem* pItem = GetCurSelItem ();
		if (pItem != NULL)
		{
			CRect rectButton = pItem->m_rectButton;
			pItem->m_rectButton.SetRectEmpty ();
			RedrawWindow (rectButton);
		}

		int nNewVertOffset = (rectRow.bottom > m_rectList.bottom) ?
			// scroll down
			m_nVertScrollOffset + (rectRow.bottom - m_rectList.bottom) :
			// scroll up
			m_nVertScrollOffset + (rectRow.top - m_rectList.top);

		SetScrollPos (SB_VERT, nNewVertOffset, FALSE);
		OnVScroll (SB_THUMBPOSITION, nNewVertOffset, NULL);

		if (pItem != NULL && !pItem->m_rectButton.IsRectEmpty ())
		{
			pItem->AdjustButtonRect ();
			RedrawWindow (pItem->m_rectButton);
		}
	}

	// --------------------
	// Scroll horizontally:
	// --------------------
	CBCGPGridItem* pItem = (!m_bSingleSel && !m_pLastSelItem != NULL) ? m_pLastSelItem : m_pSelItem;
	if (m_bVirtualMode)
	{
		CBCGPGridItemID idItem = (!m_bSingleSel && !m_idLastSel.IsNull ()) ? m_idLastSel : m_idActive;
		CBCGPGridRow* pRow = GetVirtualRow (idItem.m_nRow);
		pItem = (pRow != NULL) ? pRow->GetItem (idItem.m_nColumn) : NULL;
	}

	if (pItem == NULL)
	{
		return;
	}

	ASSERT_VALID (pItem);

	CRect rectItem = pItem->m_Rect;

	if (rectItem.left < m_rectList.left - 1 || rectItem.right > m_rectList.right)
	{
		int nNewHorzOffset = (rectItem.right > m_rectList.right) ?
			// scroll rigth
			m_nHorzScrollOffset + (rectItem.right - m_rectList.right) :
			// scroll left
			m_nHorzScrollOffset + (rectItem.left - m_rectList.left);

		SetScrollPos (SB_HORZ, nNewHorzOffset, FALSE);
		OnHScroll (SB_THUMBPOSITION, nNewHorzOffset, NULL);

		if (pItem != NULL && !pItem->m_rectButton.IsRectEmpty ())
		{
			pItem->AdjustButtonRect ();
			RedrawWindow (pItem->m_rectButton);
		}
	}
}
//******************************************************************************************
void CBCGPGridCtrl::ExpandAll (BOOL bExpand/* = TRUE*/)
{
	ASSERT_VALID (this);

	CWaitCursor wait;

	if (IsGrouping ())
	{
		for (POSITION pos = m_lstAutoGroups.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRow* pItem = m_lstAutoGroups.GetNext (pos);
			ASSERT_VALID (pItem);

			pItem->ExpandDeep (bExpand);
		}
	}

	for (POSITION pos = m_lstItems.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridRow* pItem = m_lstItems.GetNext (pos);
		ASSERT_VALID (pItem);

		pItem->ExpandDeep (bExpand);
	}

	BOOL bRebuildTerminalItemsOld = m_bRebuildTerminalItems;
	m_bRebuildTerminalItems = FALSE;

	wait.Restore ();

	AdjustLayout ();

	m_bRebuildTerminalItems = bRebuildTerminalItemsOld;

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//******************************************************************************************
void CBCGPGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd::OnChar(nChar, nRepCnt, nFlags);

	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return;
	}

	if (nChar == VK_ESCAPE)
	{
		return;
	}

	// TODO: return if shortcut (Ctrl + A, Shift + Space)

	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel == NULL || !pSel->m_bEnabled)
	{
		return;
	}

	ASSERT_VALID (pSel);

	if (!EditItem (pSel))
	{
		return;
	}

	pSel->PushChar (nChar);
}
//*******************************************************************************************
HBRUSH CBCGPGridCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	CBCGPGridItem* pItem = GetCurSelItem ();
	if (pItem != NULL && pItem->m_pWndInPlace != NULL &&
		pWnd->GetSafeHwnd () == pItem->m_pWndInPlace->GetSafeHwnd ())
	{
		HBRUSH hbrProp = pItem->OnCtlColor (pDC, nCtlColor);
		if (hbrProp != NULL)
		{
			return hbrProp;
		}
	}

	return hbr;
}

#ifndef _BCGPGRID_STANDALONE

void CBCGPGridCtrl::UpdateColor (COLORREF color)
{
	ASSERT_VALID (this);

	CBCGPGridRow* pSel = GetCurSel ();
	ASSERT_VALID (pSel);

	CBCGPGridColorItem* pColorItem = DYNAMIC_DOWNCAST(CBCGPGridColorItem, GetCurSelItem (pSel));
	if (pColorItem == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bChanged = color != pColorItem->GetColor ();
	pColorItem->SetColor (color);

	if (bChanged)
	{
		CBCGPGridItemID id = pColorItem->GetGridItemID ();
		pSel->OnItemChanged (pColorItem, id.m_nRow, id.m_nColumn);
		pColorItem->m_strLabel = pColorItem->FormatItem ();
	}

	if (pColorItem->m_pWndInPlace != NULL)
	{
		pColorItem->OnUpdateValue ();
	}
}
//****************************************************************************************
void CBCGPGridCtrl::CloseColorPopup ()
{
	ASSERT_VALID (this);

	CBCGPGridRow* pSel = GetCurSel ();
	ASSERT_VALID (pSel);

	CBCGPGridColorItem* pColorItem = DYNAMIC_DOWNCAST(CBCGPGridColorItem, GetCurSelItem (pSel));
	if (pColorItem == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	pColorItem->m_pPopup = NULL;

	pColorItem->m_bButtonIsDown = FALSE;
	pColorItem->Redraw ();

	if (pColorItem->m_pWndInPlace != NULL)
	{
		pColorItem->m_pWndInPlace->SetFocus ();
	}
}

#endif

void CBCGPGridCtrl::OnSelectCombo()
{
	ASSERT_VALID (this);
	
	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ASSERT_VALID (pSel);
	pSel->OnSelectCombo ();
}
//****************************************************************************************
void CBCGPGridCtrl::OnCloseCombo()
{
	ASSERT_VALID (this);

	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ASSERT_VALID (pSel);
	pSel->OnCloseCombo ();
}
//****************************************************************************************
void CBCGPGridCtrl::OnEditKillFocus()
{
	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel != NULL && pSel->m_bInPlaceEdit && pSel->m_bEnabled)
	{
		ASSERT_VALID (pSel);

		if (!IsChild (GetFocus()) && pSel->OnEditKillFocus())
		{
			if (!EndEditItem ())
			{
				CWnd* pWndInPlace = pSel->GetInPlaceWnd ();
				if (pWndInPlace != NULL)
				{
					pWndInPlace->SetFocus();
				}
			}
			else
			{
				OnKillFocus(GetFocus());
			}
		}
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnComboKillFocus()
{
	CBCGPGridRow* pSel = GetCurSel ();

	if (pSel != NULL && pSel->m_bEnabled)
	{
		ASSERT_VALID (pSel);

		CComboBox* pWndCombo = pSel->GetComboWnd ();
		if (pWndCombo != NULL)
		{
			ASSERT_VALID (pWndCombo);

			if (!IsChild (GetFocus()))
			{
				if (!EndEditItem ())
				{
					pWndCombo->SetFocus();
				}
				else
				{
					OnKillFocus(GetFocus());
				}
			}
		}
	}
}
//****************************************************************************************
void CBCGPGridCtrl::SetBoolLabels (LPCTSTR lpszTrue, LPCTSTR lpszFalse)
{
	ASSERT_VALID (this);
	ASSERT (lpszTrue != NULL);
	ASSERT (lpszFalse != NULL);

	m_strTrue = lpszTrue;
	m_strFalse = lpszFalse;

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//****************************************************************************************
void CBCGPGridCtrl::SetListDelimiter (TCHAR c)
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);	// Should be called before window create

	m_cListDelimeter = c;
}
//****************************************************************************************
void CBCGPGridCtrl::OnDestroy() 
{
	while (!m_lstSel.IsEmpty ())
	{
		delete m_lstSel.RemoveTail ();
	}

	CleanUpAutoGroups ();

	while (!m_lstItems.IsEmpty ())
	{
		delete m_lstItems.RemoveTail ();
	}

	m_lstTerminalItems.RemoveAll ();

	CleanUpFonts ();

	m_pSelRow = NULL;
	m_pSelItem = NULL;
	m_pLastSelRow = NULL;
	m_pLastSelItem = NULL;
	m_ToolTip.DestroyWindow ();

	CWnd::OnDestroy();
}
//****************************************************************************************
void CBCGPGridCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (m_nDraggedColumn >= 0)
	{
		CPoint pt = point;
		ClientToScreen (&pt);
		DragColumn (pt);
	}
	else if (m_bTracking)
	{
		TrackHeader (point.x);
	}
	else if (m_bSelecting && !m_bSingleSel)
	{
		const int nXDelta = 5;
		const int nYDelta = 5;

		if (abs (point.x - m_ptClickOnce.x) > nXDelta ||
			abs (point.y - m_ptClickOnce.y) > nYDelta)
		{
			KillTimer (GRID_CLICKVALUE_TIMER_ID);
			m_bClickTimer = FALSE;
			m_ptClickOnce = CPoint (0, 0);

			SetCursor (::LoadCursor (NULL, IDC_ARROW));

			SelectItems (point);
		}
	}

	if (!(m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting))
	{
		//----------------------
		// Highlight header item
		//----------------------
		int nHighlightColumn = -1;
		int nHighlightColumnBtn = -1;

		CBCGPGridColumnsInfo::ClickArea clickAreaHeader;
		int nHitColumn = m_Columns.HitTestColumn (point, FALSE, STRETCH_DELTA, &clickAreaHeader);
		if (nHitColumn != -1)
		{
			if (clickAreaHeader == CBCGPGridColumnsInfo::ClickHeader)
			{
				nHighlightColumn = nHitColumn;
			}
			else if (clickAreaHeader == CBCGPGridColumnsInfo::ClickHeaderButton)
			{
				nHighlightColumnBtn = nHitColumn;
			}
		}

		BOOL bChanged = (m_Columns.GetHighlightColumn () != nHighlightColumn ||
			m_Columns.GetHighlightColumnBtn () != nHighlightColumnBtn);
		if (bChanged)
		{
			m_Columns.SetHighlightColumn (nHighlightColumn);
			m_Columns.SetHighlightColumnBtn (nHighlightColumnBtn);
		}

		if (!m_bMouseTracked)
		{
			m_bMouseTracked = TRUE;
			
			TRACKMOUSEEVENT trackmouseevent;
			trackmouseevent.cbSize = sizeof(trackmouseevent);
			trackmouseevent.dwFlags = TME_LEAVE;
			trackmouseevent.hwndTrack = GetSafeHwnd();
			trackmouseevent.dwHoverTime = HOVER_DEFAULT;
			::BCGPTrackMouse (&trackmouseevent);
		}

		if (bChanged)
		{
			RedrawWindow ();
		}
	}
}
//*****************************************************************************************
LRESULT CBCGPGridCtrl::OnMouseLeave(WPARAM,LPARAM)
{
	m_bMouseTracked = FALSE;

	//---------------------
	// Header highlighting:
	//---------------------
	BOOL bRedraw = FALSE;
	if (m_Columns.GetHighlightColumn () >= 0)
	{
		m_Columns.SetHighlightColumn (-1);
		bRedraw = TRUE;
	}
	if (m_Columns.GetHighlightColumnBtn () >= 0)
	{
		m_Columns.SetHighlightColumnBtn (-1);
		bRedraw = TRUE;
	}

	if (bRedraw)
	{
		RedrawWindow ();
	}

	return 0;
}
//*****************************************************************************************
void CBCGPGridCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	if (m_nDraggedColumn >= 0)
	{
		StopDragColumn (point, TRUE);
	}
	else if (m_bTracking)
	{
		CRect rectTrackHeader = m_rectTrackHeader;

		TrackHeader (-1);
		m_bTracking = FALSE;

		if (::GetCapture () == GetSafeHwnd ())
		{
			ReleaseCapture ();
		}

		// ------------------
		// rearrange columns:
		// ------------------
		int nLeftOffset = 0;
		int nCount = 0;	// count visible columns

		int nPos = m_Columns.Begin ();
		while (nPos != m_Columns.End ())
		{
			int i = m_Columns.Next (nPos);
			if (i == -1)
			{
				break; // no more visible columns
			}

			if (i == m_nTrackColumn)
			{
				break;
			}

			int nWidth = m_Columns.GetColumnWidth (i);
			if (nCount == 0)
			{
				nWidth += GetHierarchyOffset () + GetExtraHierarchyOffset ();
			}

			nLeftOffset += nWidth;
			nCount++;
		}

		int nNewWidth = point.x + m_nHorzScrollOffset - nLeftOffset;
		if (nCount == 0)
		{
			nNewWidth -= GetHierarchyOffset () + GetExtraHierarchyOffset ();
		}

		BOOL bUpdate = (m_rectList.left + rectTrackHeader.left == point.x);
		if (bUpdate)
		{
			nNewWidth = max (m_nRowHeight, nNewWidth);
			SetHeaderItemWidth (m_nTrackColumn, nNewWidth);

			if (!m_bInAdjustLayout)
			{
				ReposItems ();

				OnPosSizeChanged ();
				m_rectTrackSel = OnGetSelectionRect ();

				RedrawWindow ();
			}
		}
	}
	else if (m_bSelecting)
	{
		if (m_bClickTimer)
		{
			// "Click once" event - translate a click to the item:
			KillTimer (GRID_CLICKVALUE_TIMER_ID);
			m_bClickTimer = FALSE;

			CBCGPGridItem* pItem = GetCurSelItem ();
			if (pItem != NULL)
			{
				DoClickValue (pItem, WM_LBUTTONDOWN, m_ptClickOnce, m_bIsFirstClick, m_bIsButtonClick);
				if (!m_bIsButtonClick)
				{
					pItem->OnClickValue (WM_LBUTTONUP, point);
				}
			}

			m_ptClickOnce = CPoint (0, 0);
		}

		StopSelectItems ();
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (GetStyle() & WS_BORDER) 
	{
		lpncsp->rgrc[0].left++; 
		lpncsp->rgrc[0].top++ ;
		lpncsp->rgrc[0].right--;
		lpncsp->rgrc[0].bottom--;
	}
}
//****************************************************************************************
void CBCGPGridCtrl::OnNcPaint() 
{
	if (GetStyle () & WS_BORDER)
	{
		visualManager->OnDrawControlBorder (this);
	}
}
//********************************************************************************
void CBCGPGridCtrl::SetShowInPlaceToolTip (BOOL bShow)
{
	m_bShowInPlaceToolTip = bShow;

	if (!bShow)
	{
		m_ToolTip.Deactivate ();
	}
}
//********************************************************************************
void CBCGPGridCtrl::SetColorTheme (const BCGP_GRID_COLOR_DATA& theme, BOOL bRedraw)
{
	m_ColorData = theme;

	InitColors ();
	
	if (bRedraw && GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//********************************************************************************
void CBCGPGridCtrl::SetCustomColors (
		COLORREF	clrBackground,
		COLORREF	clrText,
		COLORREF	clrGroupBackground,
		COLORREF	clrGroupText,
		COLORREF	clrLeftOffset,
		COLORREF	clrLine)
{
	CBCGPGridColors theme;
	theme.m_clrBackground = clrBackground;
	theme.m_clrText = clrText;
	theme.m_GroupColors.m_clrBackground = clrGroupBackground;
	theme.m_GroupColors.m_clrText = clrGroupText;
	theme.m_LeftOffsetColors.m_clrBackground = clrLeftOffset;
	theme.m_clrHorzLine = clrLine;
	theme.m_clrVertLine = clrLine;

	SetColorTheme (theme);
}
//********************************************************************************
void CBCGPGridCtrl::GetCustomColors (
		COLORREF&	clrBackground,
		COLORREF&	clrText,
		COLORREF&	clrGroupBackground,
		COLORREF&	clrGroupText,
		COLORREF&	clrLeftOffset,
		COLORREF&	clrLine)
{
	CBCGPGridColors theme = GetColorTheme ();

	clrBackground = theme.m_clrBackground;
	clrText = theme.m_clrText;
	clrGroupBackground = theme.m_GroupColors.m_clrBackground;
	clrGroupText = theme.m_GroupColors.m_clrText;
	clrLeftOffset = theme.m_LeftOffsetColors.m_clrBackground;
	clrLine = theme.m_clrHorzLine;
}
//********************************************************************************
void CBCGPGridCtrl::SetPreviewTextColor (COLORREF clr)
{
	m_ColorData.m_clrPreviewText = clr;
}
//********************************************************************************
COLORREF CBCGPGridCtrl::GetPreviewTextColor (BOOL bSelected) const
{
	if (bSelected)
	{
		return (COLORREF)-1; // color is not set, use default
	}

	return m_ColorData.m_clrPreviewText;
}
//********************************************************************************
BOOL CBCGPGridCtrl::ProcessClipboardAccelerators (UINT nChar)
{
	CBCGPGridItem* pSelItem = GetCurSelItem ();
	if (pSelItem == NULL || pSelItem->m_pWndInPlace == NULL ||
		pSelItem->m_pWndInPlace->GetSafeHwnd () == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pSelItem);

	BOOL bIsCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000);
	BOOL bIsShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000);

	if (bIsCtrl && (nChar == _T('C') || nChar == VK_INSERT))
	{
		pSelItem->m_pWndInPlace->SendMessage (WM_COPY);
		return TRUE;
	}

	if (bIsCtrl && nChar == _T('V') || (bIsShift && nChar == VK_INSERT))
	{
		pSelItem->m_pWndInPlace->SendMessage (WM_PASTE);
		return TRUE;
	}

	if (bIsCtrl && nChar == _T('X') || (bIsShift && nChar == VK_DELETE))
	{
		pSelItem->m_pWndInPlace->SendMessage (WM_CUT);
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
int CBCGPGridCtrl::InsertColumn (int nPos, LPCTSTR lpszColumn, int nWidth, int iImage)
{
	ASSERT_VALID (this);
	ASSERT (lpszColumn != NULL);
	ASSERT (nWidth >= 0);

	SetRebuildTerminalItems ();
	return m_Columns.InsertColumn (nPos, lpszColumn, nWidth, iImage);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::DeleteColumn (int nPos)
{
	ASSERT_VALID (this);

	SetRebuildTerminalItems ();
	return m_Columns.DeleteColumn (nPos);
}
//*******************************************************************************************
void CBCGPGridCtrl::DeleteAllColumns ()
{
	ASSERT_VALID (this);

	SetRebuildTerminalItems ();
	m_Columns.DeleteAllColumns ();
}
//*******************************************************************************************
int CBCGPGridCtrl::GetColumnCount() const
{
	return m_Columns.GetColumnCount ();
}
//*******************************************************************************************
int CBCGPGridCtrl::GetColumnWidth (int nCol) const
{
	return m_Columns.GetColumnWidth (nCol);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetColumnWidth (int nCol, int nWidth)
{
	ASSERT_VALID (this);
	ASSERT (nCol >= 0);
	ASSERT (nWidth >= 0);

	return m_Columns.SetColumnWidth (nCol, nWidth);
}
//*******************************************************************************************
CString CBCGPGridCtrl::GetColumnName (int nCol) const
{
	return m_Columns.GetColumnName (nCol);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetColumnName (int nCol, LPCTSTR lpszColumn)
{
	ASSERT_VALID (this);
	ASSERT (nCol >= 0);
	ASSERT (lpszColumn != NULL);

	return m_Columns.SetColumnName (nCol, lpszColumn);
}
//*******************************************************************************************
int CBCGPGridCtrl::GetColumnAlign (int nCol) const
{
	return m_Columns.GetColumnAlign (nCol);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetColumnAlign (int nCol, int nAlign)
{
	ASSERT_VALID (this);
	ASSERT (nCol >= 0);

	return m_Columns.SetColumnAlign (nCol, nAlign);
}
//*******************************************************************************************
int CBCGPGridCtrl::GetHeaderAlign (int nCol) const
{
	return m_Columns.GetHeaderAlign (nCol);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetHeaderAlign (int nCol, int nAlign)
{
	ASSERT_VALID (this);
	ASSERT (nCol >= 0);

	return m_Columns.SetHeaderAlign (nCol, nAlign);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::GetColumnLocked (int nCol) const
{
	ASSERT_VALID (this);
	ASSERT (nCol >= 0);

	return m_Columns.GetColumnLocked (nCol);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetColumnLocked (int nCol, BOOL bLockedSize)
{
	ASSERT_VALID (this);
	ASSERT (nCol >= 0);

	return m_Columns.SetColumnLocked (nCol, bLockedSize);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::GetColumnVisible (int nColumn) const
{
	ASSERT_VALID (this);
	ASSERT (nColumn >= 0);

	return m_Columns.GetColumnVisible (nColumn);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetColumnVisible (int nColumn, BOOL bVisible)
{
	ASSERT_VALID (this);
	ASSERT (nColumn >= 0);

	return m_Columns.SetColumnVisible (nColumn, bVisible);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::IsTextColumn (int nColumn) const
{
	ASSERT_VALID (this);
	ASSERT (nColumn >= 0);

	return m_Columns.IsTextColumn (nColumn);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetTextColumn (int nColumn, BOOL bText)
{
	ASSERT_VALID (this);
	ASSERT (nColumn >= 0);

	return m_Columns.SetTextColumn (nColumn, bText);
}
//*******************************************************************************************
int CBCGPGridCtrl::GetColumnOrderArray (LPINT piArray, int iCount) const
{
	return m_Columns.GetColumnOrderArray (piArray, iCount);
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetColumnOrderArray (int iCount, LPINT piArray)
{
	return m_Columns.SetColumnOrderArray (iCount, piArray);
}
//*******************************************************************************************
void CBCGPGridCtrl::EnableColumnAutoSize (BOOL bEnable)
{
	m_Columns.EnableAutoSize (bEnable);
	
	AdjustLayout ();
	RedrawWindow ();
}
//*******************************************************************************************
void CBCGPGridCtrl::ShowColumnsChooser (BOOL bShow/* = TRUE*/)
{
	if (!bShow)
	{
		if (m_pColumnChooser != NULL &&
			m_pColumnChooser->GetSafeHwnd () != NULL)
		{
			m_pColumnChooser->ShowWindow (SW_HIDE);
		}

		m_bColumnChooserVisible = FALSE;
		return;
	}

	if (m_pColumnChooser == NULL)
	{
		m_pColumnChooser = new CBCGPGridColumnChooser (m_Columns);

		if (m_rectColumnChooser.IsRectEmpty ())
		{
			CRect rectWindow;
			GetWindowRect (rectWindow);

			CSize sizeChooser (100, 100);

			m_rectColumnChooser =
				CRect (CPoint (
					rectWindow.right - sizeChooser.cx,
					rectWindow.top + 50),
				sizeChooser);
		}

		m_pColumnChooser->Create (this, m_rectColumnChooser, this);
	}

	m_pColumnChooser->ShowWindow (SW_SHOWNOACTIVATE);
	m_bColumnChooserVisible = TRUE;
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::IsColumnsChooserVisible () const
{
	return m_pColumnChooser->GetSafeHwnd () != NULL &&
		m_pColumnChooser->IsWindowVisible ();
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::IsValidID (const CBCGPGridItemID &id) const
{
	return (id.m_nColumn >= 0 && id.m_nColumn < m_Columns.GetColumnCount () &&
			id.m_nRow >= 0 && id.m_nRow < GetTotalItems ());
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::IsValidRange (const CBCGPGridRange &range) const
{
	return (range.IsValid () &&
			range.m_nLeft >= 0 && range.m_nRight < m_Columns.GetColumnCount () &&
			range.m_nTop >= 0 && range.m_nBottom < GetTotalItems ());
}
//*******************************************************************************************
void CBCGPGridCtrl::SelectRange (const CBCGPGridRange &range, BOOL bSelect, BOOL bRedraw)
{
	ASSERT_VALID (this);

	OnSelChanging (range, bSelect);

	if (range.m_nLeft < 0 || range.m_nTop < 0 ||
		range.m_nRight < 0 || range.m_nBottom < 0)
	{
		OnSelChanged (range, bSelect);
		return;
	}

	CBCGPGridRange rangeNormalized (
			min (range.m_nLeft, range.m_nRight),
			min (range.m_nBottom, range.m_nTop),
			max (range.m_nLeft, range.m_nRight),
			max (range.m_nBottom, range.m_nTop));

	if (m_bVirtualMode)
	{
		// iterate through cached items
		for (POSITION pos = m_CachedItems.m_lstCache.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridCachePageInfo& cpi = m_CachedItems.m_lstCache.GetNext (pos);
			ASSERT (cpi.pArrCachePage->GetSize() == cpi.nSize);

			int nOffset = max (0, rangeNormalized.m_nTop - cpi.nFirst);
			while (nOffset >= 0 && nOffset < cpi.nSize && 
				nOffset <= rangeNormalized.m_nBottom - cpi.nFirst)
			{
				CBCGPGridRow* pRow = cpi.pArrCachePage->GetAt (nOffset);
				if (pRow != NULL)
				{
					ASSERT_VALID (pRow);
					DoSelectRowInRange (pRow, rangeNormalized, bSelect, bRedraw);
				}

				nOffset++;
			}
		}
	}
	else
	//if (IsValidRange (range))
	{
		const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
			(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

		POSITION pos = lst.FindIndex (rangeNormalized.m_nTop);
		for (int i = rangeNormalized.m_nTop; pos != NULL && i <= rangeNormalized.m_nBottom; i++)
		{
			CBCGPGridRow* pRow = lst.GetNext (pos);
			ASSERT_VALID (pRow);

			DoSelectRowInRange (pRow, rangeNormalized, bSelect, bRedraw);
		}
	}

	OnSelChanged (range, bSelect);
}
//*******************************************************************************************
void CBCGPGridCtrl::DoSelectRowInRange (CBCGPGridRow* pRow, const CBCGPGridRange &range, 
										BOOL bSelect, BOOL bRedraw)
{
	ASSERT_VALID (pRow);

	BOOL bInRange = FALSE;

	int nPos = m_Columns.Begin ();
	while (nPos != m_Columns.End ())
	{
		int iColumn = m_Columns.Next (nPos);
		if (iColumn == -1)
		{
			break; // no more visible columns
		}

		BOOL bIsRangeBound = (iColumn == range.m_nLeft || iColumn == range.m_nRight);

		if (bIsRangeBound || bInRange)
		{
			CBCGPGridItem* pItem = pRow->GetItem (iColumn);

			if (pItem != NULL)
			{
				ASSERT_VALID (pItem);
				CRect rectButton = pItem->m_rectButton;
				pItem->Select (bSelect);

				if (bRedraw)
				{
					CRect rect = pItem->m_Rect;
					rect.InflateRect (1, 1);
					InvalidateRect (rect);
					InvalidateRect (rectButton);
				}
			}
		}

		if (bIsRangeBound)
		{
			if (bInRange || range.m_nLeft == range.m_nRight)
			{
				break;	// last visible column in range
			}

			bInRange = TRUE;
		}
	}

	pRow->Select (bSelect);
	if (pRow->IsGroup ())
	{
		if (bRedraw)
		{
			CRect rect = pRow->m_Rect;
			rect.InflateRect (1, 1);
			InvalidateRect (rect);
		}
	}
	else if (m_bRowMarker && GetExtraHierarchyOffset () > 0)
	{
		// update row marker:
		int dx = IsSortingMode () && !IsGrouping () ? 0 : pRow->GetHierarchyLevel () * m_nRowHeight;
		CRect rect = pRow->m_Rect;
		rect.right = rect.left + GetExtraHierarchyOffset () + dx;
		InvalidateRect (rect);
	}
}
//*******************************************************************************************
void CBCGPGridCtrl::IncludeRect (CRect& rect, const CRect& rectNew)
{
	if (rectNew.IsRectEmpty ())
	{
		return;
	}

	if (rect.IsRectEmpty ())
	{
		rect = rectNew;
		rect.NormalizeRect ();
	}
	else
	{
		CRect rect2 = rectNew;
		rect2.NormalizeRect ();
		rect.UnionRect (rect, rect2);
	}
}
//*******************************************************************************************
void CBCGPGridCtrl::DoInvalidateRowInRange (CBCGPGridRow* pRow, const CBCGPGridRange &range,
											CRect& rectUpdate) const
{
	if (pRow != NULL)
	{
		ASSERT_VALID (pRow);

		if (!pRow->IsGroup ())
		{
			CBCGPGridItem* pItemLeft = pRow->GetItem (range.m_nLeft);
			if (pItemLeft != NULL)
			{
				IncludeRect (rectUpdate, pItemLeft->GetRect ());
				IncludeRect (rectUpdate, pItemLeft->m_rectButton);
			}

			CBCGPGridItem* pItemRight = pRow->GetItem (range.m_nRight);
			if (pItemRight != NULL)
			{
				IncludeRect (rectUpdate, pItemRight->GetRect ());
				IncludeRect (rectUpdate, pItemRight->m_rectButton);
			}
		}
		
		if (pRow->IsGroup ())
		{
			// update the row:
			IncludeRect (rectUpdate, pRow->GetRect ());
		}
		else if (m_bRowMarker && GetExtraHierarchyOffset () > 0)
		{
			// update row marker:
			int dx = IsSortingMode () && !IsGrouping () ? 0 : pRow->GetHierarchyLevel () * m_nRowHeight;
			CRect rect = pRow->GetRect ();
			rect.right = rect.left + GetExtraHierarchyOffset () + dx;
			IncludeRect (rectUpdate, rect);
		}
	}
}
//*******************************************************************************************
CRect CBCGPGridCtrl::GetRect (const CBCGPGridRange &range)
{
	ASSERT_VALID (this);

	if (m_bVirtualMode)
	{
		CRect rectUpdate;
		rectUpdate.SetRectEmpty ();

		DoInvalidateRowInRange (GetVirtualRow (range.m_nTop), range, rectUpdate);
		DoInvalidateRowInRange (GetVirtualRow (range.m_nBottom), range, rectUpdate);

		return rectUpdate;
	}
	else
	{
		const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
			(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

		CRect rectUpdate;
		rectUpdate.SetRectEmpty ();

		POSITION pos = lst.FindIndex (range.m_nTop);
		if (pos != NULL)
		{
			DoInvalidateRowInRange (lst.GetAt (pos), range, rectUpdate);
		}

		pos = lst.FindIndex (range.m_nBottom);
		if (pos != NULL)
		{
			DoInvalidateRowInRange (lst.GetAt (pos), range, rectUpdate);
		}

		return rectUpdate;
	}
}
//*******************************************************************************************
void CBCGPGridCtrl::InvalidateRange (const CBCGPGridRange &range)
{
	ASSERT_VALID (this);

	CRect rectUpdate = GetRect (range);
	rectUpdate.InflateRect (1, 1);
	InvalidateRect (rectUpdate);
}
//*******************************************************************************************
CBCGPGridRow* CBCGPGridCtrl::GetCurSel (CBCGPGridItemID &id) const
{
	ASSERT_VALID (this);

	id = m_idActive;

	if (m_bVirtualMode)
	{
		return ((CBCGPGridCtrl*) this)->GetVirtualRow (id.m_nRow);
	}
	else if (m_pSelRow == NULL)
	{
		if (m_pSelItem != NULL)
		{
			return m_pSelItem->m_pGridRow;
		}

		return NULL;
	}

	return m_pSelRow;
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::SetCurSel (CBCGPGridItemID idItem,
							   DWORD dwSelMode, BOOL bRedraw)
{
	ASSERT_VALID (this);

	CBCGPGridRow* pSel = GetCurSel ();
	CBCGPGridRow* pOldSelectedItem = pSel;

	if ( m_idLastSel == idItem &&
		 !(dwSelMode == SM_NONE) &&
		 !( ((SM_ALL | SM_COLUMN | SM_ROW | SM_SINGLE_ITEM | 
			  SM_FIRST_CLICK | SM_SECOND_CLICK | SM_INVERT_SEL) & dwSelMode) != 0) )
	{
		return FALSE;
	}

	if (pSel != NULL && pSel->m_bInPlaceEdit)
	{
		EndEditItem (FALSE);
	}

	BOOL	bExcelSel = TRUE;			// Excell behavior

	const int nFirstColumn = m_Columns.GetFirstVisibleColumn ();
	const int nFirstRow = 0;
	const int nLastColumn = m_Columns.GetLastVisibleColumn ();
	const int nLastRow = GetTotalItems () - 1;

	ASSERT (nLastColumn <= m_Columns.GetColumnCount () - 1);

	if (nLastColumn < 0 || nLastRow < 0)
	{
		dwSelMode = SM_NONE;
	}

	// -----------------
	// Change selection:
	// -----------------
	if ((dwSelMode & SM_ALL) != 0)
	{
		// Store previous active item, select all items
		if (m_idActive.IsNull ())
		{
			m_idActive = idItem;
		}
		m_idLastSel = idItem;

		while (!m_lstSel.IsEmpty ())
		{
			delete m_lstSel.RemoveTail ();
		}

		CBCGPGridRange* pSelRange = 
			new CBCGPGridRange (nFirstColumn, nFirstRow, nLastColumn, nLastRow);
		m_lstSel.AddTail (pSelRange);
	}
	else if ((dwSelMode & SM_NONE) != 0 ||
		idItem.IsNull () || 
		nLastColumn < 0 || nLastRow < 0)
	{
		// Remove selection
		m_idActive.SetNull ();
		m_idLastSel.SetNull ();

		while (!m_lstSel.IsEmpty ())
		{
			CBCGPGridRange* pRange = m_lstSel.RemoveTail ();
			ASSERT (pRange != NULL);

			SelectRange (*pRange, FALSE, bRedraw);
			delete pRange;
		}
	}
	else if ((dwSelMode & SM_SET_ACTIVE_ITEM) != 0)
	{
		// idItem should be within selection only
		// Store selection, change active item
		BOOL bRow = idItem.IsRow ();
		for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRange* pSelRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
			ASSERT (pSelRange != NULL);

			if (bRow ? 
				(pSelRange->IsInRange (idItem.m_nRow, nFirstColumn) &&
				pSelRange->IsInRange (idItem.m_nRow, nLastColumn)) :
				pSelRange->IsInRange (idItem))
			{
				m_idActive = idItem;
				break;
			}
		}
	}

	else if ((dwSelMode & SM_SINGLE_ITEM) != 0 && !bExcelSel)
	{
	}
	else if ((dwSelMode & SM_FIRST_CLICK) != 0 ||
			(dwSelMode & SM_SINGLE_ITEM) != 0)
	{
		// Regarding nSelMode, modify selection or set new selection.
		// If not INVERT_SEL, the active item is always selected.

		if ((dwSelMode & SM_SINGE_SEL_GROUP) != 0)
		{
			// Remove selection, set active item, add selected block
			while (!m_lstSel.IsEmpty ())
			{
				CBCGPGridRange* pRange = m_lstSel.RemoveTail ();
				ASSERT (pRange != NULL);

				SelectRange (*pRange, FALSE, bRedraw);
				delete pRange;
			}

            BOOL bAddSelGroup = TRUE;
            DoSetSelection (idItem, dwSelMode, bAddSelGroup, bRedraw, 
							nFirstColumn, nFirstRow, nLastColumn, nLastRow);
		}
		else if ((dwSelMode & SM_CONTINUE_SEL_GROUP) != 0)
		{
			// Store previous active item, modify selected block
            BOOL bAddSelGroup = FALSE;
            DoSetSelection (idItem, dwSelMode, bAddSelGroup, bRedraw,
							nFirstColumn, nFirstRow, nLastColumn, nLastRow);
		}
		else if ((dwSelMode & SM_ADD_SEL_GROUP) != 0)
		{
			// Store selection, set new active item, add new selected block
            BOOL bAddSelGroup = TRUE;
            DoSetSelection (idItem, dwSelMode, bAddSelGroup, bRedraw,
							nFirstColumn, nFirstRow, nLastColumn, nLastRow);
		}
		else
		{
			return FALSE;
		}
	}
	else if ((dwSelMode & SM_SECOND_CLICK) != 0)
	{
		// Store previous active item, modify selected block
        BOOL bAddSelGroup = FALSE;
        DoSetSelection (idItem, dwSelMode, bAddSelGroup, bRedraw,
						nFirstColumn, nFirstRow, nLastColumn, nLastRow);
	}
	else
	{
		return FALSE;
	}

	if (m_bWholeRowSel && !m_lstSel.IsEmpty ())
	{
		// Inflate last selected block to the whole row
		CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetTail ();
		ASSERT (pRange != NULL);

		pRange->m_nLeft = nFirstColumn;
		pRange->m_nRight = nLastColumn;
	}

	CBCGPGridItem* pPrevItem = GetCurSelItem ();
	if (m_bVirtualMode)
	{
		m_pSelRow = NULL;
		m_pSelItem = NULL;

		m_pLastSelRow = NULL;
		m_pLastSelItem = NULL;
	}
	else
	{
		m_pSelRow = (m_idActive.m_nRow != -1) ? GetRow (m_idActive.m_nRow) : NULL;
		m_pSelItem = (m_pSelRow != NULL) ? m_pSelRow->GetItem (m_idActive.m_nColumn) : NULL;

		m_pLastSelRow = (m_idLastSel.m_nRow != -1) ? GetRow (m_idLastSel.m_nRow) : NULL;
		m_pLastSelItem = (m_pLastSelRow != NULL) ? m_pLastSelRow->GetItem (m_idLastSel.m_nColumn) : NULL;
	}

	CRect rectTrackSelOld = m_rectTrackSel;
	m_rectTrackSel = OnGetSelectionRect ();
	if (bRedraw)
	{
		InvalidateRect (rectTrackSelOld);
		InvalidateRect (m_rectTrackSel);
	}

	// -------------------------
	// Notify control and items:
	// -------------------------
	if (m_bWholeRowSel && m_bSingleSel)
	{
		CBCGPGridRow* pSel = GetCurSel ();
		OnChangeSelection (pSel, pOldSelectedItem);

		if (pOldSelectedItem != NULL)
		{
			ASSERT_VALID (pOldSelectedItem);

			pOldSelectedItem->OnKillSelection (pSel);

			if (bRedraw)
			{
				CRect rectOld = pOldSelectedItem->m_Rect;
				InvalidateRect (rectOld);
			}
		}

		if (pSel != NULL)
		{
			ASSERT_VALID (pSel);

			pSel->OnSetSelection (pOldSelectedItem);

			if (bRedraw)
			{
				CRect rect = pSel->m_Rect;
				InvalidateRect (rect);
			}
		}
	}

	for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
		ASSERT (pRange != NULL);

		SelectRange (*pRange, TRUE, bRedraw);
	}

	if (pPrevItem != NULL)
	{
		CRect rectButton = pPrevItem->m_rectButton;
		pPrevItem->m_rectButton.SetRectEmpty ();

		InvalidateRect (rectButton);
	}

	CBCGPGridItem* pCurItem = GetCurSelItem ();
	if (pCurItem != NULL && pCurItem->HasButton ())
	{
		pCurItem->AdjustButtonRect ();

		InvalidateRect (pCurItem->m_rectButton);
	}
	
	// ---------------
	// Update control:
	// ---------------
	if (bRedraw && !m_bNoUpdateWindow)
	{
		UpdateWindow ();
	}

/*
	//ListCtrl behavior
		// arrows
		// Remove selection, set active item, add selected block
		// Ctrl + arrows
		// Change current item. Active item can be not selected.
		// Ctrl + Space
		// Invert selection for the active item.
		// Ctrl + Shift + arrows
		// Invert selection for range from active item to current item.

	//Excell behavior
	// change selection:
	switch (nSelMode)
	case SM_INVERT_SEL:		// non-Excell behavior
		// Add items to m_arrSel at first time.
		// If items are already selected then remove them from m_arrSel .

	case SM_SINGE_SEL_GROUP:// click, arrows
		// Remove selection, set active item, add selected block
	case SM_CONTINUE_SEL_GROUP:// Shift + click, Shift + arrows
		// Store previous active item, modify selected block
	case SM_ADD_SEL_GROUP:	// Ctrl + click
		// Store selection, set new active item, add new selected block

	case SM_SET_ACTIVE_ITEM:// Tab, Enter
		// Should be within selection only
		// Store selection, change active item

	case SM_NONE:
		// Remove selection
	case SM_ALL:			// Ctrl + A
		// Store previous active item, select all items
	case SM_SINGLE_ROW:		// Shift + Space
		// Store previous active item, inflate selected block to the whole row
	case SM_SINGLE_COLUMN:	// Ctrl + Space
		// Store previous active item, inflate selected block to the whole column

	case SM_SINGLE_ITEM:	// arrows
	case SM_FIRST_CLICK:
		// Regarding nSelMode, set active item or set new selection.
		// If not INVERT_SEL, the active item is always selected.
	case SM_SECOND_CLICK:
		// Store previous active item, modify selected block
		m_arrSel.Add (pHitItem);
*/
	if (GetSafeHwnd () != NULL)
	{
		CWnd* pOwnerWnd = GetOwner ();
		if (pOwnerWnd != NULL)
		{
			pOwnerWnd->SendMessage (BCGM_GRID_SEL_CHANGED, 0, (LPARAM) this);
		}
	}
	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::DoSetSelection (const CBCGPGridItemID& idItem, const DWORD dwSelMode,
									BOOL bAddSelGroup, BOOL bRedraw,
									const int nFirstColumn, const int nFirstRow,
									const int nLastColumn, const int nLastRow)
{
	ASSERT_VALID (this);
	ASSERT (!idItem.IsNull ());
	ASSERT (nFirstColumn >= 0);
	ASSERT (nFirstRow >= 0);
	ASSERT (nLastColumn >= 0);
	ASSERT (nLastRow >= 0);

    if (bAddSelGroup)
    {
        // --------------------
        // Set new active item.
        // --------------------
   		m_idActive = idItem;
		m_idLastSel = idItem;

        // ---------------------------------
        // Store previously selected blocks.
        // Add new selected block.
        // ---------------------------------

		BOOL bRow = ((dwSelMode & SM_ROW) != 0 || m_idActive.IsRow ());
		BOOL bColumn = ((dwSelMode & SM_COLUMN) != 0 || m_idActive.IsColumn ());

		CBCGPGridRange* pSelRange = new CBCGPGridRange (
			bRow ? nFirstColumn : m_idActive.m_nColumn,
			bColumn ? nFirstRow : m_idActive.m_nRow,
			bRow ? nLastColumn : m_idActive.m_nColumn,
			bColumn ? nLastRow : m_idActive.m_nRow );

		CBCGPGridRange range;
		UnionRange (pSelRange, GetMergedRange (idItem, m_pSetSelItem, range));

		m_lstSel.AddTail (pSelRange);
    }
    else
    {
        // ---------------------------
        // Store previous active item.
        // ---------------------------
		if (m_idActive.IsNull ())
		{
			m_idActive = idItem;
		}
		m_idLastSel = idItem;

		if (m_lstSel.IsEmpty ())
		{
			BOOL bRow = ((dwSelMode & SM_ROW) != 0 || m_idActive.IsRow ());
			BOOL bColumn = ((dwSelMode & SM_COLUMN) != 0 || m_idActive.IsColumn ());

			CBCGPGridRange* pSelRange = new CBCGPGridRange (
				bRow ? nFirstColumn : m_idActive.m_nColumn,
				bColumn ? nFirstRow : m_idActive.m_nRow,
				bRow ? nLastColumn : m_idActive.m_nColumn,
				bColumn ? nLastRow : m_idActive.m_nRow );
			
			CBCGPGridRange range;
			UnionRange (pSelRange, GetMergedRange (m_idActive, GetCurSelItem (), range));

			m_lstSel.AddTail (pSelRange);
		}

        // ---------------------------
        // Modify last selected block.
        // ---------------------------
		CBCGPGridRange* pRangeLast = (CBCGPGridRange*) m_lstSel.GetTail ();
		ASSERT (pRangeLast != NULL);

		SelectRange (*pRangeLast, FALSE, bRedraw);

		BOOL bRow = ((dwSelMode & SM_ROW) != 0 || m_idActive.IsRow ());
		BOOL bColumn = ((dwSelMode & SM_COLUMN) != 0 || m_idActive.IsColumn () || idItem.IsColumn ());

		int nLeftColumn = pRangeLast->m_nLeft;
		int nRightColumn = pRangeLast->m_nRight;

		if (!bRow && !idItem.IsRow ())
		{
			nLeftColumn = min (m_idActive.m_nColumn, idItem.m_nColumn);
			nRightColumn = max (m_idActive.m_nColumn, idItem.m_nColumn);
		}

		pRangeLast->Set (
			bRow ? nFirstColumn : nLeftColumn,
			bColumn ? nFirstRow : min (m_idActive.m_nRow, idItem.m_nRow),
			bRow ? nLastColumn : nRightColumn,
			bColumn ? nLastRow : max (m_idActive.m_nRow, idItem.m_nRow) );

		CBCGPGridRange range1;
		UnionRange (pRangeLast, GetMergedRange (m_idActive, GetCurSelItem (), range1));
		CBCGPGridRange range2;
		UnionRange (pRangeLast, GetMergedRange (idItem, m_pSetSelItem, range2));
    }

	return TRUE;
}
//*******************************************************************************************
void CBCGPGridCtrl::UnionRange (CBCGPGridRange* pRange, const CBCGPGridRange* pRange2)
{
	if (pRange != NULL && pRange2 != NULL)
	{
//		int nPosLeft1 = m_Columns.IndexToOrder (pRange->m_nLeft);
//		int nPosRight1 = m_Columns.IndexToOrder (pRange->m_nRight);
//		int nPosLeft2 = m_Columns.IndexToOrder (pRange2->m_nLeft);
//		int nPosRight2 = m_Columns.IndexToOrder (pRange2->m_nRight);
		int nPosLeft1 = pRange->m_nLeft;
		int nPosRight1 = pRange->m_nRight;
		int nPosLeft2 = pRange2->m_nLeft;
		int nPosRight2 = pRange2->m_nRight;

		if (nPosLeft2 != -1 && nPosRight2 != -1)
		{
			if (nPosLeft1 != -1 && min (nPosLeft2, nPosRight2) < nPosLeft1)
			{
				pRange->m_nLeft = pRange2->m_nLeft;
			}

			if (nPosRight1 != -1 && max (nPosLeft2, nPosRight2) > nPosRight1)
			{
				pRange->m_nRight = pRange2->m_nRight;
			}
		}

		int nTop = min (pRange2->m_nTop, pRange2->m_nBottom);
		int nBottom = max (pRange2->m_nTop, pRange2->m_nBottom);

		if (nTop < pRange->m_nTop)
		{
			pRange->m_nTop = nTop;
		}

		if (nBottom > pRange->m_nBottom)
		{
			pRange->m_nBottom = nBottom;
		}
	}
}
//*******************************************************************************************
CBCGPGridRow* CBCGPGridCtrl::CreateRow (int nColumns)
{
	ASSERT_VALID (this);
	
	CBCGPGridRow* pRow = CreateRow ();
	ASSERT_VALID (pRow);

	pRow->SetOwnerList (this);

	for (int i = 0; i < nColumns ; i++)
	{
		CBCGPGridItem* pItem = pRow->CreateItem (
			(int) GetRowCount (), i);

		pItem->SetOwnerRow (pRow);
		int nIndex = (int) pRow->m_arrRowItems.Add (pItem);
		pItem->m_nIdColumn = nIndex;
	}

	return pRow;
}
//*******************************************************************************************
CBCGPGridRow* CBCGPGridCtrl::CreateRow (CString strName)
{
	ASSERT_VALID (this);

	CBCGPGridRow* pGroup = CreateRow ();
	ASSERT_VALID (pGroup);
	pGroup->AllowSubItems (TRUE);

	CBCGPGridItem* pGridItem = new CBCGPGridItem (_variant_t (strName));
	pGridItem->m_bEnabled = FALSE;
	pGroup->AddItem (pGridItem);

	return pGroup;
}
//*******************************************************************************************
CBCGPGridItem* CBCGPGridCtrl::CreateItem (int nRow, int nColumn)
{
	CBCGPGridItem* pItem = NULL;

	CRuntimeClass* pItemRTC = NULL;
	if (m_mapItemsRTC.Lookup (nColumn, pItemRTC))
	{
		ASSERT (pItemRTC != NULL);

		pItem = DYNAMIC_DOWNCAST (CBCGPGridItem, pItemRTC->CreateObject ());
	}
	else if (m_pDefaultItemRTC != NULL)
	{
		pItem = DYNAMIC_DOWNCAST (CBCGPGridItem, m_pDefaultItemRTC->CreateObject ());
	}
	else
	{
		pItem = new CBCGPGridItem ();
	}

	if (pItem == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	ASSERT_VALID (pItem);
	
	if (!pItem->OnCreate (nRow, nColumn))
	{
		delete pItem;
		return NULL;
	}

	return pItem;
}
//*******************************************************************************************
void CBCGPGridCtrl::SetItemRTC (int nColumn, CRuntimeClass* pRuntimeClass)
{
	if (pRuntimeClass != NULL)
	{
		m_mapItemsRTC.SetAt (nColumn, pRuntimeClass);
	}
	else
	{
		m_mapItemsRTC.RemoveKey (nColumn);
	}
}
//*******************************************************************************************
CRuntimeClass* CBCGPGridCtrl::GetItemRTC (int nColumn) const
{
	CRuntimeClass* pItemRTC = NULL;
	m_mapItemsRTC.Lookup (nColumn, pItemRTC);

	return pItemRTC;
}
//*******************************************************************************************
int CBCGPGridCtrl::HitTestGroupByBox (CPoint point, LPRECT lprectItem/* = NULL*/)
{
	ASSERT_VALID (this);

	CClientDC dc (this);
	HFONT hfontOld = SetCurrFont (&dc);

	int nResult = -1;

	for (int i = 0; i < m_Columns.GetGroupColumnCount (); i++)
	{
		CRect rectColumn;
		int nColumn = GetGroupColumnRect (i, rectColumn, &dc);

		if (nColumn >= 0 && rectColumn.PtInRect (point))
		{
			nResult = nColumn;
			if (lprectItem != NULL)
			{
				*lprectItem = rectColumn;
			}
			break;
		}
	}

	::SelectObject (dc.GetSafeHdc (), hfontOld);
	return nResult;
}
//*******************************************************************************************
int CBCGPGridCtrl::GetGroupByBoxDropIndex (CPoint point, LPPOINT lpptDrop/* = NULL*/)
{
	ASSERT_VALID(this);

	CRect rectClient;
	GetClientRect (rectClient);

	CRect rectGroupByBox = rectClient;
	rectGroupByBox.bottom = min (rectGroupByBox.top + m_nGroupByBoxHeight, rectClient.bottom);

	rectGroupByBox.DeflateRect (BCGPGRID_GROUPBYBOX_HMARGIN, BCGPGRID_GROUPBYBOX_VMARGIN, 0, 0);

	if (!rectGroupByBox.PtInRect (point))
	{
		return -1;
	}

	CClientDC dc (this);
	HFONT hfontOld = SetCurrFont (&dc);

	CPoint ptDrop;

	ptDrop.x = rectGroupByBox.left;
	ptDrop.y = (rectGroupByBox.top + rectGroupByBox.bottom - BCGPGRID_GROUPBYBOX_VMARGIN) / 2;

	int i = 0;
	BOOL bFirst = TRUE;

	for (i = 0; i < m_Columns.GetGroupColumnCount (); i++)
	{
		CRect rectColumn;
		if (GetGroupColumnRect (i, rectColumn, &dc) < 0)
		{
			continue;
		}

		if (bFirst)
		{
			ptDrop.x = rectColumn.left;
			ptDrop.y = (rectColumn.top + rectColumn.bottom) / 2;

			bFirst = FALSE;
		}

		if (point.x <= rectColumn.CenterPoint ().x)
		{
			break;
		}

		ptDrop.x = rectColumn.right;
		ptDrop.y = (rectColumn.top + rectColumn.bottom) / 2;
	}

	::SelectObject (dc.GetSafeHdc (), hfontOld);

	if (lpptDrop != NULL)
	{
		*lpptDrop = ptDrop;
	}

	return i;
}
//*******************************************************************************************
void CBCGPGridCtrl::SetPrinterFont(CFont* pFont, CFont* pFontBold)
{
	ASSERT_VALID(this);

	m_hPrinterFont = (HFONT)pFont->GetSafeHandle();
	m_hPrinterBoldFont = (HFONT)pFontBold->GetSafeHandle();
}
//*******************************************************************************************
void CBCGPGridCtrl::Print ()
{
	// Printing without the Document/View framework
	ASSERT_VALID(this);

    CDC dc;
    CPrintDialog printDlg(FALSE);
	 
    if (printDlg.DoModal() == IDCANCEL)
        return;
	
    dc.Attach(printDlg.GetPrinterDC());
    dc.m_bPrinting = TRUE;

    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);

    DOCINFO di;
    ::ZeroMemory (&di, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);

	m_bIsPrinting = TRUE;
	m_pPrintDC = &dc;

    CPrintInfo printInfo;
    printInfo.m_rectDraw.SetRect(0,0,
							dc.GetDeviceCaps(HORZRES), 
							dc.GetDeviceCaps(VERTRES));

	// -----------------------------------------
	// Prepare page for printing, calc page size
	// -----------------------------------------
	int nFirstItem = 0;						// By default print all grid items
	int nLastItem = GetTotalItems () - 1;	// from first row to the last
	OnPreparePrintPages (&printInfo, nFirstItem, nLastItem);

    OnBeginPrinting(&dc, &printInfo);

	// ---------------
	// Set up margins:
	// ---------------
	CRect rectMargins = OnGetPageMargins (&dc, &printInfo);
	printInfo.m_rectDraw.DeflateRect (&rectMargins);

	int nPagesCount = OnCalcPrintPages (&dc, &printInfo);
	printInfo.SetMaxPage (nPagesCount);

	CRect rectDraw = printInfo.m_rectDraw;

    for (printInfo.m_nCurPage = printInfo.GetMinPage(); 
         printInfo.m_nCurPage <= printInfo.GetMaxPage() && bPrintingOK; 
         printInfo.m_nCurPage++)
    {
		printInfo.m_rectDraw = rectDraw;

        dc.StartPage();

        OnPrint(&dc, &printInfo);
        bPrintingOK = (dc.EndPage() > 0);
    }
    OnEndPrinting(&dc, &printInfo);

    if (bPrintingOK)
        dc.EndDoc();
    else
        dc.AbortDoc();

	m_bIsPrinting = FALSE;
	m_pPrintDC = NULL;

    dc.DeleteDC();

	AdjustLayout ();
}
//*******************************************************************************************
void CBCGPGridCtrl::OnPreparePrintPages (CPrintInfo* pInfo,
										 int nFirstItem, int nLastItem)
{
	ASSERT_VALID(this);
	ASSERT (pInfo != NULL);
	ASSERT (nFirstItem >= 0);
	ASSERT (nLastItem >= 0);

	m_PrintParams.m_nRowHeight = 0;
	m_PrintParams.m_nLargeRowHeight = 0;
	m_PrintParams.m_nGroupByBoxHeight = 0;
	m_PrintParams.m_rectHeader.SetRectEmpty ();
	m_PrintParams.m_rectList.SetRectEmpty ();
	m_PrintParams.m_CachedPrintItems.CleanUpCache ();
	m_PrintParams.m_nVertScrollOffset = 0;
	m_PrintParams.m_nHorzScrollOffset = 0;

	m_PrintParams.m_pPrintInfo = pInfo;
	m_PrintParams.m_pageInfo.Init (		// set up printing range
		min (nFirstItem, nLastItem), max (nFirstItem, nLastItem),
		0, m_Columns.GetColumnCount (TRUE));

	m_PrintParams.m_idCur.SetNull ();

	pInfo->m_lpUserData = &m_PrintParams.m_pageInfo;
}
//*******************************************************************************************
int CBCGPGridCtrl::OnCalcPrintPages (CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT (pInfo != NULL);
	ASSERT (pInfo == m_PrintParams.m_pPrintInfo);
	ASSERT (m_bIsPrinting);

	ASSERT (m_hPrinterFont != NULL);		// fonts must be initialized
	ASSERT (m_hPrinterBoldFont != NULL);

	// -------------------------
	// Init printing parameters:
	// -------------------------
	SetRowHeight ();

    //----------------------------------
    // Get page header and footer sizes:
    //----------------------------------
	CRect rectPageHeader;
	rectPageHeader.SetRectEmpty ();
	OnGetPageHeaderRect (pDC, pInfo, rectPageHeader);
	int nPageHeaderHeight = rectPageHeader.Height ();

	CRect rectPageFooter;
	rectPageFooter.SetRectEmpty ();
	OnGetPageFooterRect (pDC, pInfo, rectPageFooter);
	int nPageFooterHeight = rectPageFooter.Height ();

	m_PrintParams.m_rectList = pInfo->m_rectDraw;
	m_PrintParams.m_rectList.top += nPageHeaderHeight;
	m_PrintParams.m_rectList.bottom -= nPageFooterHeight;

	m_PrintParams.m_nGroupByBoxHeight = OnGetGroupByBoxRect (pDC, m_PrintParams.m_rectList).Height ();
	m_PrintParams.m_rectHeader = OnGetHeaderRect (pDC, m_PrintParams.m_rectList);

	m_PrintParams.m_rectList.top += m_PrintParams.m_nGroupByBoxHeight;
	m_PrintParams.m_rectList.top += m_PrintParams.m_rectHeader.Height ();

	// ----------------
	// Calc pages info:
	// ----------------
	return CalcPages (pDC, pInfo);
}
//*******************************************************************************************
void CBCGPGridCtrl::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	if (m_hPrinterFont == NULL || m_hPrinterBoldFont == NULL)
	{
		// get current screen font object metrics
		CFont* pFont = GetFont();
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));
		LOGFONT lfSys;
		ZeroMemory(&lfSys, sizeof(lfSys));

		if (pFont == NULL)
		{
			pFont = CFont::FromHandle (GetDefaultFont ());
			if (pFont == NULL)
			{
				return;
			}
		}

		VERIFY(pFont->GetObject(sizeof(LOGFONT), &lf));
		VERIFY(::GetObject(::GetStockObject(SYSTEM_FONT), sizeof(LOGFONT),
			&lfSys));
		if (lstrcmpi((LPCTSTR)lf.lfFaceName, (LPCTSTR)lfSys.lfFaceName) == 0)
			return;

		// map to printer font metrics
		HDC hDCFrom = ::GetDC(NULL);
		lf.lfHeight = ::MulDiv(lf.lfHeight, pDC->GetDeviceCaps(LOGPIXELSY),
			::GetDeviceCaps(hDCFrom, LOGPIXELSY));
		lf.lfWidth = ::MulDiv(lf.lfWidth, pDC->GetDeviceCaps(LOGPIXELSX),
			::GetDeviceCaps(hDCFrom, LOGPIXELSX));
		::ReleaseDC(NULL, hDCFrom);

		if (m_hPrinterFont == NULL)
		{
			// create it, if it fails we just use the printer's default.
			m_hMirrorFont = ::CreateFontIndirect(&lf);
			m_hPrinterFont = m_hMirrorFont;
		}

		if (m_hPrinterBoldFont == NULL)
		{
			lf.lfWeight = FW_BOLD;
			m_hMirrorBoldFont = ::CreateFontIndirect(&lf);
			m_hPrinterBoldFont = m_hMirrorBoldFont;
		}
	}

	SetPrintColors ();

	ASSERT_VALID(this);
}
//*******************************************************************************************
void CBCGPGridCtrl::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	ASSERT_VALID(this);

	if (m_hMirrorFont != NULL && m_hPrinterFont == m_hMirrorFont)
	{
		AfxDeleteObject((HGDIOBJ*)&m_hMirrorFont);
		m_hPrinterFont = NULL;
	}

	if (m_hMirrorBoldFont != NULL && m_hPrinterBoldFont == m_hMirrorBoldFont)
	{
		AfxDeleteObject((HGDIOBJ*)&m_hMirrorBoldFont);
		m_hPrinterBoldFont = NULL;
	}
}
//*******************************************************************************************
void CBCGPGridCtrl::SetPrintColors ()
{
	ASSERT_VALID (this);

	m_clrPrintBorder = RGB (0, 0, 0);			// black
	m_clrPrintHeader = RGB (128, 128, 128);		// grey
	m_clrPrintHeaderBG = RGB (255, 255, 255);	// white
	m_clrPrintGroup = m_clrPrintHeader;
	m_clrPrintGroupBG = m_clrPrintHeaderBG;
	m_clrPrintLeftOffset = RGB (192, 192, 192);	// light grey
	m_clrPrintBG = RGB (255, 255, 255);			// white
	m_clrPrintLine = RGB (192, 192, 192);			// light grey
	m_clrPrintText = RGB (0, 0, 0);				// black
}
//*******************************************************************************************
void CBCGPGridCtrl::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_bContinuePrinting);

	HFONT hfontOld = SetCurrFont (pDC);
	pDC->SetTextColor (m_clrPrintText);
	pDC->SetBkMode (TRANSPARENT);

	// ---------------------------
	// Layout page, calc page size
	// ---------------------------
	PrintLayout (pDC, pInfo);

	// ----------------------------
	// print page header and footer
	// ----------------------------
	OnPrintPageHeader(pDC, pInfo);
	OnPrintPageFooter(pDC, pInfo);

	// --------------------------------------------
	// print current page inside pInfo->m_rectDraw:
	// --------------------------------------------
	CRect rectGroupByBox = pInfo->m_rectDraw;
	rectGroupByBox.bottom = rectGroupByBox.top + m_PrintParams.m_nGroupByBoxHeight;
	rectGroupByBox.bottom = min (rectGroupByBox.bottom, pInfo->m_rectDraw.bottom);
	OnDrawGroupByBox (pDC, rectGroupByBox);

	OnPrintHeader (pDC, pInfo);
	OnPrintList (pDC, pInfo);

	::SelectObject (pDC->GetSafeHdc (), hfontOld);
}
//*******************************************************************************************
void CBCGPGridCtrl::OnPrintPageHeader(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);

	CRect rectHeader;
	if (OnGetPageHeaderRect (pDC, pInfo, rectHeader))
	{
		// use page header - deflate drawing area at top
		if (pInfo->m_rectDraw.top < rectHeader.bottom)
		{
			pInfo->m_rectDraw.top = min (rectHeader.bottom, pInfo->m_rectDraw.bottom);
		}
	}
}
//*******************************************************************************************
void CBCGPGridCtrl::OnPrintPageFooter(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);

	CRect rectFooter;
	if (OnGetPageFooterRect (pDC, pInfo, rectFooter))
	{
		// use page footer - deflate drawing area at bottom
		if (pInfo->m_rectDraw.bottom > rectFooter.top)
		{
			pInfo->m_rectDraw.bottom = max (rectFooter.top, pInfo->m_rectDraw.top);
		}
	}
}
//*******************************************************************************************
CRect CBCGPGridCtrl::OnGetPageMargins (CDC* pDC, CPrintInfo* /*pInfo*/)
{
	ASSERT_VALID (pDC);

	// Page margins:
	double dLeftOffset = 0.5;
	double dTopOffset = 0.5;
	double dRightOffset = 0.5;
	double dBottomOffset = 0.5;
	CRect rectMargins (
		(int)(pDC->GetDeviceCaps(LOGPIXELSX) * dLeftOffset),
		(int)(pDC->GetDeviceCaps(LOGPIXELSY) * dTopOffset),
		(int)(pDC->GetDeviceCaps(LOGPIXELSX) * dRightOffset),
		(int)(pDC->GetDeviceCaps(LOGPIXELSY) * dBottomOffset));

	return rectMargins;
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::OnGetPageHeaderRect (CDC* /*pDC*/, CPrintInfo* /*pInfo*/, CRect& /*rect*/)
{
	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPGridCtrl::OnGetPageFooterRect (CDC* /*pDC*/, CPrintInfo* /*pInfo*/, CRect& /*rect*/)
{
	return FALSE;
}
//*******************************************************************************************
CRect CBCGPGridCtrl::OnGetGroupByBoxRect (CDC* pDC, const CRect& rectDraw)
{
	CRect rect;
	if (m_bGroupByBox)
	{
		int nGroupByBoxHeight = 0;

		int nYMul = 1;
		int nYDiv = 1;
		if (m_bIsPrinting && pDC != NULL)
		{
			ASSERT_VALID (pDC);

			// map to printer metrics
			HDC hDCFrom = ::GetDC(NULL);
			nYMul = pDC->GetDeviceCaps(LOGPIXELSY);			// pixels in print dc
			nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
			::ReleaseDC(NULL, hDCFrom);
		}

		if (!m_bIsPrinting && m_Columns.GetGroupColumnCount () > 1 ||
			m_bIsPrinting && m_Columns.GetGroupColumnCount () >= 1)
		{
			int nItemHeight = (m_bIsPrinting ? m_PrintParams.m_nRowHeight : m_nRowHeight) +
				::MulDiv (TEXT_MARGIN, nYMul, nYDiv);
			nGroupByBoxHeight = nItemHeight +
				2 * ::MulDiv (BCGPGRID_GROUPBYBOX_VMARGIN, nYMul, nYDiv) + 
				(m_Columns.GetGroupColumnCount () - 1) * 
				::MulDiv (BCGPGRID_GROUPBYBOX_VSPACING, nYMul, nYDiv);
		}
		else if (!m_bIsPrinting)
		{
			nGroupByBoxHeight = m_nRowHeight + 16;
		}

		rect = rectDraw;
		rect.bottom = rect.top + nGroupByBoxHeight;
	}
	else
	{
		rect.SetRectEmpty ();
	}

	return rect;
}
//*******************************************************************************************
CRect CBCGPGridCtrl::OnGetHeaderRect (CDC* pDC, const CRect& rectDraw)
{
	CRect rect;
	if (m_bHeader)
	{
		int nYMul = 1;
		int nYDiv = 1;
		if (m_bIsPrinting && pDC != NULL)
		{
			ASSERT_VALID (pDC);

			// map to printer metrics
			HDC hDCFrom = ::GetDC(NULL);
			nYMul = pDC->GetDeviceCaps(LOGPIXELSY);			// pixels in print dc
			nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
			::ReleaseDC(NULL, hDCFrom);
		}

		int nHeaderHeight = (m_bIsPrinting ? m_PrintParams.m_nRowHeight : m_nRowHeight) + ::MulDiv (4, nYMul, nYDiv);

		rect = rectDraw;
		rect.top += m_bIsPrinting ? m_PrintParams.m_nGroupByBoxHeight : m_nGroupByBoxHeight;
		rect.bottom = rect.top + nHeaderHeight;
	}
	else
	{
		rect.SetRectEmpty ();
	}

	return rect;
}
//*******************************************************************************************
CRect CBCGPGridCtrl::GetGridHeaderRect () const
{
	if (m_bIsPrinting)
	{
		return CRect (0, 0, 0, 0);
	}

	CRect rectClient;
	GetClientRect (rectClient);

	CRect rectHeader = rectClient;
	rectHeader.top += m_nGroupByBoxHeight + m_rectHeader.Height ();
	rectHeader.bottom = min (rectHeader.top + m_nGridHeaderHeight, rectHeader.bottom);
	return rectHeader;
}
//****************************************************************************************
CRect CBCGPGridCtrl::GetGridFooterRect () const
{
	if (m_bIsPrinting)
	{
		return CRect (0, 0, 0, 0);
	}

	CRect rectClient;
	GetClientRect (rectClient);

	CRect rectFooter = rectClient;
	if (m_nHorzScrollTotal > 0)
	{
		int cyScroll = ::GetSystemMetrics (SM_CYHSCROLL);
		rectFooter.bottom -= cyScroll;
	}

	rectFooter.top = m_rectList.bottom;
	rectFooter.bottom = max (rectFooter.top + m_nGridFooterHeight, rectFooter.bottom);

	return rectFooter;
}
//****************************************************************************************
void CBCGPGridCtrl::PrintLayout (CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID (pDC);
	ASSERT (pInfo != NULL);
	ASSERT (pInfo == m_PrintParams.m_pPrintInfo);

	int nPage = (int) pInfo->m_nCurPage - 1;
	if (nPage < 0 || nPage >= m_arrPages.GetSize ())
	{
		//ASSERT (FALSE); // wrong page
		return;
	}

	// -------------------------------
	// Count items before current page
	// -------------------------------
	ASSERT (m_PrintParams.m_pageInfo.m_nFirstItem >= 0);
	ASSERT (m_PrintParams.m_pageInfo.m_nLastItem >= m_PrintParams.m_pageInfo.m_nFirstItem);
	
	int nTop = 0;
	int nBottom = m_PrintParams.m_pageInfo.m_nFirstItem;
	int nCountVisible = GetTotalItems (nTop, nBottom, TRUE);
	
	m_PrintParams.m_pageInfo.m_nGroupsCount = GetGroupsCount (nTop, nBottom, TRUE);
    m_PrintParams.m_pageInfo.m_nItemsCount = nCountVisible - m_PrintParams.m_pageInfo.m_nGroupsCount;

	for (int i = 0; i < nPage && i < m_arrPages.GetSize (); i++)
	{
		CBCGPGridPage& page = m_arrPages [i];

		if (page.m_nHorzOffset == 0)
		{
			m_PrintParams.m_pageInfo.m_nItemsCount += page.m_nItems;
			m_PrintParams.m_pageInfo.m_nGroupsCount += page.m_nGroups;
		}
	}

	// ----------------------
	// Get current page info:
	// ----------------------
    m_PrintParams.m_pageInfo.m_nPage = pInfo->m_nCurPage;

	CBCGPGridPage& page = m_arrPages [nPage];
	m_PrintParams.m_pageInfo.m_nFirstInPage = page.m_nFirstInPage;
	m_PrintParams.m_pageInfo.m_nItemsInPage = page.m_nItems;
	m_PrintParams.m_pageInfo.m_nGroupsInPage = page.m_nGroups;
	m_PrintParams.m_pageInfo.m_nTotalInPage = page.m_nTotal;


	// --------------------------
	// Prepare page for printing:
	// --------------------------
	// map to printer metrics
	HDC hDCFrom = ::GetDC(NULL);
	int nXMul = pDC->GetDeviceCaps(LOGPIXELSX);			// pixels in print dc
	int nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX);	// pixels in screen dc
	int nYMul = pDC->GetDeviceCaps(LOGPIXELSY);			// pixels in print dc
	int nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
	::ReleaseDC(NULL, hDCFrom);

	m_PrintParams.m_pageInfo.m_szOne.cx = ::MulDiv (1, nXMul, nXDiv);;
	m_PrintParams.m_pageInfo.m_szOne.cy = ::MulDiv (1, nYMul, nYDiv);;

	m_PrintParams.m_nVertScrollOffset = 
		m_PrintParams.m_pageInfo.m_nGroupsCount * m_PrintParams.m_nLargeRowHeight + 
		m_PrintParams.m_pageInfo.m_nItemsCount * m_PrintParams.m_nRowHeight;
	m_PrintParams.m_nHorzScrollOffset = page.m_nHorzOffset;

	// ----------------
	// Reposition items
	// ----------------
	if (m_bVirtualMode)
	{
		return;
	}

	m_PrintParams.m_idCur.m_nRow = m_PrintParams.m_pageInfo.m_nFirstInPage;
	m_PrintParams.m_idCur.m_nColumn = -1;

	int y = m_PrintParams.m_rectList.top - 1;
	
	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	int nRow = m_PrintParams.m_pageInfo.m_nFirstInPage;
	POSITION pos = (nRow < lst.GetCount ()) ? lst.FindIndex (nRow) : NULL;
	for (; pos != NULL && nRow <= m_PrintParams.m_pageInfo.m_nLastItem; nRow++)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

		pItem->Repos (y);
	}
}
//*******************************************************************************************
int CBCGPGridCtrl::CalcPages (CDC* pDC, CPrintInfo* /*pInfo*/)
{
	ASSERT (m_bIsPrinting);

	// map to printer metrics
	HDC hDCFrom = ::GetDC(NULL);
	int nXMul = pDC->GetDeviceCaps(LOGPIXELSX);			// pixels in print dc
	int nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX);	// pixels in screen dc
	int nYMul = pDC->GetDeviceCaps(LOGPIXELSY);			// pixels in print dc
	int nYDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSY);	// pixels in screen dc
	::ReleaseDC(NULL, hDCFrom);

	m_PrintParams.m_pageInfo.m_szOne.cx = ::MulDiv (1, nXMul, nXDiv);;
	m_PrintParams.m_pageInfo.m_szOne.cy = ::MulDiv (1, nYMul, nYDiv);;

	if (m_bVirtualMode)
	{
		//-----------------
		// Calculate pages:
		//-----------------
		m_arrPages.RemoveAll ();

		const int nLastVirtualRow = (m_nVirtualRows > 0) ? m_nVirtualRows - 1 : 0;

		int nFirstInPage = min (nLastVirtualRow, m_PrintParams.m_pageInfo.m_nFirstItem);
		const int nLast = min (nLastVirtualRow, m_PrintParams.m_pageInfo.m_nLastItem);
		const int nItems = m_PrintParams.m_nRowHeight != 0 ?
			m_PrintParams.m_rectList.Height () / m_PrintParams.m_nRowHeight : 0;

		do
		{
			if (nFirstInPage <= nLast)
			{
				int nPageItems = min (nItems, nLast - nFirstInPage + 1);

				CBCGPGridPage page;
				page.m_nFirstInPage = nFirstInPage;
				page.m_nItems = nPageItems;
				page.m_nGroups = 0;
				page.m_nTotal = nPageItems;

				m_arrPages.Add (page);

				nFirstInPage += nPageItems;
			}
		}
		while (nFirstInPage <= nLast);

		return (int) m_arrPages.GetSize ();
	}


	m_PrintParams.m_idCur.m_nRow = 0;
	m_PrintParams.m_idCur.m_nColumn = -1;

	int y = m_PrintParams.m_rectList.top - 1;
	
	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	POSITION pos = NULL;
	for (pos = lst.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

		pItem->Repos (y);
	}

	//-----------------
	// Calculate pages:
	//-----------------
	m_arrPages.RemoveAll ();

	CRect rectPage = m_PrintParams.m_rectList;
	rectPage.top = m_PrintParams.m_rectList.top - 1;

	int nFirstInPage = m_PrintParams.m_pageInfo.m_nFirstItem;
	int nItems = 0;		// visible item count
	int nGroups = 0;	// visible group count
	int nTotal = 0;		// total item count

	pos = (nFirstInPage < lst.GetCount ()) ? lst.FindIndex (nFirstInPage) : NULL;
	while (pos != NULL && nFirstInPage + nTotal <= m_PrintParams.m_pageInfo.m_nLastItem)
	{
		POSITION posSave = pos;

		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

		if (!pItem->m_Rect.IsRectEmpty ())
		{
			//if (pItem->m_Rect.top >= rectPage.bottom)
			if (pItem->m_Rect.bottom >= rectPage.bottom)
			{
				if (nTotal == 0)
				{
					ASSERT (FALSE);
					break;
				}

				// item is above page bottom - next page
				CBCGPGridPage page;
				page.m_nFirstInPage = nFirstInPage;
				page.m_nItems = nItems;
				page.m_nGroups = nGroups;
				page.m_nTotal = nTotal;

				//---------------------------------------------
				// Add horizontal row of one or more GridPages:
				//---------------------------------------------
				OnAddGridPageRowForPrint (page, rectPage);

				rectPage.OffsetRect (0, nItems * m_PrintParams.m_nRowHeight + nGroups * m_PrintParams.m_nLargeRowHeight);
					
				nFirstInPage += nTotal;
				nItems = 0;
				nGroups = 0;
				nTotal = 0;

				pos = posSave;
				continue;
			}
			else if (pItem->m_Rect.bottom >= rectPage.top)
			{
				// item is below page top - item fits in the current page
				if (pItem->IsGroup ())
				{
					nGroups++;
				}
				else
				{
					nItems++;
				}
			}
			else
			{
				// else - item at previous pages - skip item
				ASSERT (FALSE);
			}
		}

		nTotal++;
	}

	if (nTotal > 0)
	{
		CBCGPGridPage page;
		page.m_nFirstInPage = nFirstInPage;
		page.m_nItems = nItems;
		page.m_nGroups = nGroups;
		page.m_nTotal = nTotal;

		OnAddGridPageRowForPrint (page, rectPage);
	}

	return (int) m_arrPages.GetSize ();
}
//*******************************************************************************************
int CBCGPGridCtrl::OnAddGridPageRowForPrint (CBCGPGridPage& page, CRect rectPage)
{
	//m_PrintParams.m_pageInfo.m_nFirstCol
	//m_PrintParams.m_pageInfo.m_nLastCol
	page.m_nHorzOffset = 0;

	//---------------------------------------------
	// Add horizontal row of one or more GridPages:
	//---------------------------------------------
	m_arrPages.Add (page);
	int nCount = 1;

	int nHorzOffset = 0;
	int nHorzPage = rectPage.Width ();
	int nHorzTotal = m_Columns.GetTotalWidth () * m_PrintParams.m_pageInfo.m_szOne.cx;
	while (nHorzOffset + nHorzPage < nHorzTotal)
	{
		nHorzOffset += nHorzPage;
		page.m_nHorzOffset = nHorzOffset;
		m_arrPages.Add (page);
		nCount++;
	}

	return nCount;
}
//*******************************************************************************************
void CBCGPGridCtrl::OnPrintHeader(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	// map to printer metrics
	HDC hDCFrom = ::GetDC(NULL);
	int nXMul = pDC->GetDeviceCaps(LOGPIXELSX);			// pixels in print dc
	int nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX);	// pixels in screen dc
	::ReleaseDC(NULL, hDCFrom);

	ASSERT (m_bIsPrinting);

	CRect rectHeader = m_PrintParams.m_rectHeader;

	// ------------------------------------
	// Print grid header inside rectHeader:
	// ------------------------------------
	CBrush brHeaderBG (m_clrPrintHeaderBG);
	pDC->FillRect (rectHeader, &brHeaderBG);

	CPen penHeader (PS_SOLID, 1, m_clrPrintHeader);
	CPen* pOldPen = pDC->SelectObject (&penHeader);
	COLORREF clrOld = pDC->GetTextColor ();

	CSize szOne = m_PrintParams.m_pageInfo.m_szOne;
	int nHorzScrollOffset = m_PrintParams.m_nHorzScrollOffset;

	int nColumnCount = 0; // count visible columns
	int nXLeft = m_PrintParams.m_rectList.left - nHorzScrollOffset;
	int nPos = m_Columns.Begin ();
	while (nPos != m_Columns.End ())
	{
		int iColumn = m_Columns.Next (nPos);
		if (iColumn == -1)
		{
			break; // no more visible columns
		}

		ASSERT (iColumn >= 0);

		int nWidth = m_Columns.GetColumnWidth (iColumn);
		nWidth = ::MulDiv(nWidth, nXMul, nXDiv);

		BOOL bIsTreeColumn = (m_nTreeColumn == -1) ? (nColumnCount == 0):
			(m_nTreeColumn == iColumn);
		if (bIsTreeColumn)
		{
			nWidth += GetHierarchyOffset () * szOne.cx +
				GetExtraHierarchyOffset () * szOne.cx;
		}

		if (nWidth > 0) // if column visible
		{
			CRect rectItem = rectHeader;
			rectItem.left = nXLeft;
			rectItem.right = rectItem.left + nWidth;

			CRect rectClipItem = rectItem;
			if (rectItem.left < rectHeader.left)
			{
				rectClipItem.left = max (rectHeader.left, rectClipItem.left);
			}
			if (rectItem.right > rectHeader.right)
			{
				rectClipItem.right = min (rectHeader.right, rectClipItem.right);
			}

			if (rectClipItem.Width () > 0)
			{
				//-----------------
				// Fill background:
				//-----------------
				pDC->FillRect (rectClipItem, &brHeaderBG);

				//-----------
				// Draw text:
				//-----------
				CRect rectText = rectItem;
				int nTextMargin = ::MulDiv(TEXT_MARGIN, nXMul, nXDiv);
				//rectText.DeflateRect (nTextMargin, nTextMargin, 0, 0);
				rectText.DeflateRect (nTextMargin, nTextMargin, nTextMargin, 0);

				pDC->SetTextColor (m_clrPrintText);

				CString strColumn = m_Columns.GetColumnName (iColumn);
				pDC->ExtTextOut (rectText.left, rectText.top, ETO_CLIPPED, &rectClipItem, strColumn, NULL);

				//-----------------
				// Draw sort arrow:
				//-----------------
				int nColState = m_Columns.GetColumnState (iColumn);
				if (nColState != 0)
				{
					CRect rectArrow = rectItem;
					int nMargin = ::MulDiv(5, nXMul, nXDiv);
					rectArrow.DeflateRect (nMargin, nMargin);
					rectArrow.left = max (rectArrow.right - rectArrow.Height (), rectArrow.left);

					int dy2 = (int) (.134 * rectArrow.Width ());
					rectArrow.DeflateRect (0, dy2);

					BOOL bAscending = nColState >= 0;
					OnDrawSortArrow (pDC, rectArrow, bAscending);
				}

				//-------------
				// Draw border:
				//-------------
				if (rectClipItem.left <= rectItem.left && rectClipItem.right >= rectItem.left)
				{
					pDC->MoveTo (rectItem.TopLeft ());
					pDC->LineTo (rectItem.left, rectItem.bottom);
				}
			}
		}

		nXLeft += nWidth;
		nColumnCount++;
	}

	//-------------
	// Draw border:
	//-------------
	CRect rectItems = rectHeader;
	rectItems.left = max (m_PrintParams.m_rectList.left - nHorzScrollOffset, rectHeader.left);
	rectItems.right = min (nXLeft, rectHeader.right);

	if (rectHeader.left <= nXLeft && rectHeader.right >= nXLeft)
	{
		pDC->MoveTo (rectItems.right, rectItems.top);
		pDC->LineTo (rectItems.BottomRight ());
	}

	pDC->MoveTo (rectItems.TopLeft ());
	pDC->LineTo (rectItems.right, rectItems.top);
	pDC->MoveTo (rectItems.left, rectItems.bottom);
	pDC->LineTo (rectItems.BottomRight ());

	pDC->SelectObject (pOldPen);
	pDC->SetTextColor (clrOld);
}
//*******************************************************************************************
void CBCGPGridCtrl::OnPrintList(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT (pInfo != NULL);

	CPen penLine (PS_SOLID, 1, m_clrPrintLine);
	CPen* pOldPen = pDC->SelectObject (&penLine);

	CBCGPGridPageInfo* pPageInfo = (CBCGPGridPageInfo*) pInfo->m_lpUserData;
	ASSERT (pPageInfo != NULL);

	ASSERT (m_bIsPrinting);

	// calc rectItems
	int nItemsBottom = m_PrintParams.m_rectList.top + 
		pPageInfo->m_nGroupsInPage * m_PrintParams.m_nLargeRowHeight + 
		pPageInfo->m_nItemsInPage * m_PrintParams.m_nRowHeight;
	nItemsBottom = min (nItemsBottom, m_PrintParams.m_rectList.bottom);

	CRect rectItems = m_PrintParams.m_rectList;
	rectItems.bottom = nItemsBottom;

	// ----------------------------------
	// Print grid items inside rectItems:
	// ----------------------------------
	// map to printer metrics
	HDC hDCFrom = ::GetDC(NULL);
	int nXMul = pDC->GetDeviceCaps(LOGPIXELSX);			// pixels in print dc
	int nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX);	// pixels in screen dc
	::ReleaseDC(NULL, hDCFrom);

	int nHorzScrollOffset = m_PrintParams.m_nHorzScrollOffset;
	CSize szOne = m_PrintParams.m_pageInfo.m_szOne;

	BOOL bDrawVLines = !IsKindOf (RUNTIME_CLASS (CBCGPReportCtrl));

	int nXLeft = m_PrintParams.m_rectList.left - nHorzScrollOffset;
	int nColumnCount = 0;	// count visible columns

	int nPos = m_Columns.Begin ();
	while (nPos != m_Columns.End ())
	{
		int iColumn = m_Columns.Next (nPos);
		if (iColumn == -1)
		{
			break; // no more visible columns
		}

		ASSERT (iColumn >= 0);
		ASSERT (iColumn < m_Columns.GetColumnCount ());

		int nWidth = m_Columns.GetColumnWidth (iColumn);
		nWidth = ::MulDiv(nWidth, nXMul, nXDiv);

		BOOL bIsTreeColumn = (m_nTreeColumn == -1) ? (nColumnCount == 0):
			(m_nTreeColumn == iColumn);
		if (bIsTreeColumn)
		{
			nWidth += GetHierarchyOffset () * szOne.cx +
				GetExtraHierarchyOffset () * szOne.cx;
		}

		nXLeft += nWidth;
		nColumnCount ++;

		// print vertical grid lines
		if (bDrawVLines && 
			nXLeft >= m_PrintParams.m_rectList.left && nXLeft <= m_PrintParams.m_rectList.right)
		{
			pDC->MoveTo (nXLeft - 1, m_PrintParams.m_rectList.top - 1);
			pDC->LineTo (nXLeft - 1, nItemsBottom);
		}
	}

	CRect rectClipItems = rectItems;
	rectClipItems.left = m_PrintParams.m_rectList.left - nHorzScrollOffset;
	rectClipItems.right = nXLeft;

	rectClipItems.NormalizeRect ();
	pPageInfo->m_rectPageItems.IntersectRect (rectClipItems, m_PrintParams.m_rectList);

	int nFirst = pPageInfo->m_nFirstInPage;
	int nCount = pPageInfo->m_nTotalInPage;

	if (m_bVirtualMode)
	{
		for (int i = 0; i < pPageInfo->m_nTotalInPage; i++)
		{
			const int nIndex = pPageInfo->m_nFirstInPage + i;

			CBCGPGridRow* pItem = m_PrintParams.m_CachedPrintItems.GetCachedRow (nIndex);
			if (pItem == NULL)
			{
				pItem = CreateVirtualRow (nIndex);

				if (!m_PrintParams.m_CachedPrintItems.SetCachedRow (nIndex, pItem))
				{
					ASSERT (FALSE);
					break;
				}
			}

			if (pItem != NULL)
			{
				ASSERT_VALID (pItem);

				int y = m_PrintParams.m_rectList.top - 1 + i * m_PrintParams.m_nRowHeight;
				m_PrintParams.m_idCur.m_nRow = nIndex;
				
				pItem->Repos (y);
				OnPrintItem (pDC, pInfo, pItem);
			}
		}
	}

	const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
		(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

	POSITION pos = (nFirst < lst.GetCount ()) ? lst.FindIndex (nFirst) : NULL;
	for (int j = nFirst; pos != NULL && j < nFirst + nCount; j++)
	{
		CBCGPGridRow* pItem = lst.GetNext (pos);
		ASSERT_VALID (pItem);

		if (!OnPrintItem (pDC, pInfo, pItem))
		{
			break;
		}
	}

	pDC->SelectObject (pOldPen);
}
//*******************************************************************************************
int CBCGPGridCtrl::OnPrintItem (CDC* pDC, CPrintInfo* pInfo, CBCGPGridRow* pItem) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT (pInfo != NULL);
	ASSERT_VALID (pItem);
	ASSERT (m_bIsPrinting);

	if (!pItem->m_Rect.IsRectEmpty ())
	{
		if (pItem->m_Rect.top >= m_PrintParams.m_rectList.bottom)
		{
			return FALSE;
		}

		if (pItem->m_Rect.bottom >= m_PrintParams.m_rectList.top)
		{
			CBCGPGridPageInfo* pPageInfo = (CBCGPGridPageInfo*) pInfo->m_lpUserData;
			ASSERT (pPageInfo != NULL);

			int dx = IsSortingMode () && !IsGrouping () ? 0 : pItem->GetHierarchyLevel () * m_PrintParams.m_nRowHeight;
			
			// --------------------------
			// draw left hierarchy offset
			// --------------------------
			CRect rectLeft = pItem->m_Rect;
			rectLeft.top += pPageInfo->m_szOne.cy;
			rectLeft.right = rectLeft.left + GetExtraHierarchyOffset () * pPageInfo->m_szOne.cx + dx;
			rectLeft.bottom += pPageInfo->m_szOne.cy;

			CRect rectClipLeft = rectLeft;

			CRect rectCol;
			m_Columns.GetColumnRect (m_Columns.OrderToIndex (0), rectCol);
			int nCol0Right = pItem->m_Rect.right;
			if (!rectCol.IsRectNull ())
			{
				nCol0Right = pItem->m_Rect.left + rectCol.Width () * pPageInfo->m_szOne.cx;
				rectClipLeft.right = min (rectClipLeft.right, nCol0Right);
			}

			rectClipLeft.NormalizeRect ();
			rectClipLeft.IntersectRect (&m_PrintParams.m_rectList, &rectClipLeft);

			if (rectClipLeft.Width () > 0)
			{
				CBrush br (m_clrPrintLeftOffset);
				pDC->FillRect (rectClipLeft, &br);
			}

			CRect rectName = pItem->m_Rect;

			// ---------------
			// fill background
			// ---------------
			if (!pItem->HasValueField ())
			{
				CRect rectFill = rectName;
				rectFill.top += pPageInfo->m_szOne.cy;
				rectFill.DeflateRect (dx, 0, 0, 0);

				CRect rectClipFill = rectFill;
				rectClipFill.NormalizeRect ();
				rectClipFill.IntersectRect (&rectClipFill, &pPageInfo->m_rectPageItems);

				if (rectClipFill.Width () > 0)
				{
					CBrush brBackground (m_clrPrintBG);
					pDC->FillRect (rectClipFill, &brBackground);

					// draw group underline
					if (pItem->IsGroup ())
					{
						CRect rectUnderline = rectClipFill;
						rectUnderline.top = rectUnderline.bottom;
						rectUnderline.InflateRect (0, pPageInfo->m_szOne.cy);

						COLORREF clrOld = pDC->GetBkColor ();
						pDC->FillSolidRect (rectUnderline, m_clrPrintGroup);
						pDC->SetBkColor (clrOld);
					}
				}
			}

			// ---------------
			// draw expandbox:
			// ---------------
			if (pItem->IsGroup () && (!IsSortingMode () || IsGrouping ()))
			{
				CRect rectExpand = rectName;
				rectName.left += m_PrintParams.m_nRowHeight + dx;
				rectExpand.right = rectName.left;
				rectExpand.DeflateRect (dx, 0, 0, 0);

				CRect rectClipExpand = rectExpand;
				rectClipExpand.NormalizeRect ();
				rectClipExpand.IntersectRect (&rectClipExpand, &pPageInfo->m_rectPageItems);

				if (rectClipExpand.Width () > 0)
				{
					pItem->OnDrawExpandBox (pDC, rectClipExpand);
				}
			}

			// ----------
			// draw name:
			// ----------
			if (rectName.right > rectName.left)
			{
				HFONT hOldFont = NULL;
				if (pItem->IsGroup () && m_hPrinterBoldFont != NULL)
				{
					hOldFont = (HFONT) ::SelectObject (pDC->GetSafeHdc (), m_hPrinterBoldFont);
				}

				pItem->OnPrintName (pDC, rectName);

				if (hOldFont != NULL)
				{
					::SelectObject (pDC->GetSafeHdc (), hOldFont);
				}
			}

			// ------------
			// draw values:
			// ------------
			if (pItem->HasValueField ())
			{
				CRect rectItems = pItem->m_Rect;

				pItem->OnPrintItems (pDC, rectItems);

				// print horizontal grid lines
				rectItems.left += GetExtraHierarchyOffset () * pPageInfo->m_szOne.cx  + dx;
				rectItems.left = min (rectItems.left, nCol0Right);

				CRect rectClipItems = rectItems;
				rectClipItems.NormalizeRect ();
				rectClipItems.IntersectRect (&rectClipItems, &pPageInfo->m_rectPageItems);

				if (rectClipItems.Width () > 0 &&
					rectItems.bottom > rectClipItems.top && rectItems.bottom <= rectClipItems.bottom)
				{
					pDC->MoveTo (rectClipItems.left, rectItems.bottom);
					pDC->LineTo (rectClipItems.right, rectItems.bottom);
				}
			}

			// ---------------
			// draw focus rect
			// ---------------
			if (m_bDrawFocusRect &&
				IsFocused () && IsWholeRowSel () && GetCurSel () == pItem)
			{
			}
		}
	}

	return TRUE;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::StartDragColumn (int nItem, CRect rect, 
									 BOOL bDragGroupItem, BOOL bDragFromChooser)
{
	int nLButton = GetSystemMetrics (SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
	if ((GetAsyncKeyState (nLButton) & 0x8000) == 0)
	{
		return FALSE;
	}

	BOOL bHeaderSortClick = !bDragGroupItem && !bDragFromChooser &&
		(m_dwHeaderFlags & BCGP_GRID_HEADER_SORT) != 0;

	if ((m_dwHeaderFlags & BCGP_GRID_HEADER_MOVE_ITEMS) == 0 && !bHeaderSortClick)
	{
		return FALSE;
	}
	
	m_nDraggedColumn = nItem;

	m_bDragGroupItem = bDragGroupItem;
	m_bDragFromChooser = bDragFromChooser;

	RedrawWindow (rect);

	ClientToScreen (&rect);
	m_rectStartDrag = rect;

	SetCapture ();
	return TRUE;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::DragColumn (CPoint ptScreen)
{
	if (m_nDraggedColumn < 0)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CPoint point = ptScreen;
	ScreenToClient (&point);

	// Allow user limited mouse movement without starting a drag operation:
	BOOL bWasMoved = 
		(abs (m_ptStartDrag.x - point.x) > ::GetSystemMetrics (SM_CXDRAG) ||
		 abs (m_ptStartDrag.y - point.y) > ::GetSystemMetrics (SM_CYDRAG));

	if (!m_bDragHeaderItems ||
		!bWasMoved && m_pWndHeaderDrag == NULL)
	{
		SetCursor (::LoadCursor (NULL, IDC_ARROW));
		return FALSE;
	}

	BOOL bToGroupByBox = FALSE;
	BOOL bNoDropToHeader = FALSE;

	if (m_pWndHeaderDrag == NULL)
	{
		m_pWndHeaderDrag = new CBCGPHeaderItemDragWnd;
		ASSERT_VALID (m_pWndHeaderDrag);

		CRect rectItem = m_rectStartDrag;

		BOOL bCreated = m_pWndHeaderDrag->Create (this, m_nDraggedColumn);
		if (!bCreated)
		{
			TRACE (_T("CBCGPHeaderItemDragWnd::DragColumn: Unable to create m_pWndHeaderDrag\n"));

			if (::GetCapture () == GetSafeHwnd ())
			{
				ReleaseCapture ();
			}
			return FALSE;
		}
	}
	else
	{
		BOOL bDrop = m_pWndHeaderDrag->m_bDrop;

		m_pWndHeaderDrag->m_bDrop = FALSE;

		// Is over the column chooser?
		BOOL bDragOverChooser = FALSE;
		
		if (IsColumnsChooserVisible ())
		{
			CRect rectChooser;
			m_pColumnChooser->GetWindowRect (rectChooser);

			bDragOverChooser = rectChooser.PtInRect (ptScreen);
		}

		CRect rectHeader = m_rectHeader;
		ClientToScreen (&rectHeader);

		// Is over the grid header?
		BOOL bDragOverHeader = FALSE;
		int iDropIndex = -1;
		if (rectHeader.PtInRect (ptScreen))
		{
			bDragOverHeader = TRUE;
			iDropIndex = m_Columns.HitTestColumn (point);

			if (iDropIndex == -1 && m_Columns.GetColumnCount (TRUE) > 0)
			{
				bDragOverHeader = FALSE;
			}
		}

		if (bDragOverChooser)
		{
			// ---------------------------------
			// Dragging over the column chooser.
			// Hide the drop indicator:
			// ---------------------------------
			if (m_pWndHeaderDrop != NULL)
			{
				m_pWndHeaderDrop->Hide ();
			}

			m_pWndHeaderDrag->m_bDrop = TRUE;
		}
		else if (bDragOverHeader)
		{
			// -----------------------------------
			// Dragging over the grid header.
			// Set position of the drop indicator:
			// -----------------------------------
			int nPosDraggedColumn = m_Columns.IndexToOrder(m_nDraggedColumn);
			int nPosDrop = iDropIndex >= 0 ? m_Columns.IndexToOrder (iDropIndex) : -1;

			int x = 0;

			if (nPosDrop >= 0 && nPosDrop < m_Columns.GetColumnCount (TRUE))
			{
				CRect rectItem;
				m_Columns.GetColumnRect (iDropIndex, rectItem);

				if (nPosDrop == nPosDraggedColumn || nPosDraggedColumn == -1)
				{
					if (point.x > rectItem.CenterPoint ().x)
					{
						x = rectItem.right;
						nPosDrop ++;
					}
					else
					{
						x = rectItem.left;
					}
				}
				else if (nPosDrop > nPosDraggedColumn)
				{
					x = rectItem.right;
					nPosDrop ++;
				}
				else
				{
					x = rectItem.left;
				}
			}
			else
			{
				nPosDrop = 0; // all columns are hidden - insert first
			}

			if (nPosDrop == nPosDraggedColumn && !m_bDragGroupItem)
			{
				// position has not changed - hide drop indicator
				if (m_pWndHeaderDrop != NULL)
				{
					m_pWndHeaderDrop->Hide ();
				}
			}
			else if (!CanDropColumn (nPosDrop, nPosDraggedColumn))
			{
				// can't drop here - hide drop indicator
				if (m_pWndHeaderDrop != NULL)
				{
					m_pWndHeaderDrop->Hide ();
				}

				bNoDropToHeader = TRUE;
			}
			else
			{
				CPoint ptDrop (x, m_rectHeader.CenterPoint ().y);
				ClientToScreen (&ptDrop);

				ShowDropIndicator (ptDrop);
			}

			m_pWndHeaderDrag->m_bDrop = TRUE;
		}
		else 
		{
			CPoint point = ptScreen;
			ScreenToClient (&point);

			CRect rectClient;
			GetClientRect (rectClient);

			CRect rectGroupByBox = rectClient;
			rectGroupByBox.bottom = min (rectGroupByBox.top + m_nGroupByBoxHeight, rectClient.bottom);

			if (rectGroupByBox.PtInRect (point))
			{
				// -----------------------------------
				// Dragging over the group-by-box.
				// Set position of the drop indicator:
				// -----------------------------------
				CPoint ptDrop;

				int nPos = GetGroupByBoxDropIndex (point, &ptDrop);
				if (nPos >= 0)
				{
					BOOL bPosChanged = TRUE;

					int nPosDraggedColumn = m_Columns.GetGroupColumnPos (m_nDraggedColumn);
					if (nPosDraggedColumn >= 0)
					{
						bPosChanged = (nPosDraggedColumn != nPos && 
							nPosDraggedColumn + 1 != nPos);
					}

					if (!m_bDragGroupItem && nPosDraggedColumn >= 0 ||
						!OnDropToGroupByBox (m_nDraggedColumn))
					{
						// column is already in the group-by-box - do nothing
						if (m_pWndHeaderDrop != NULL)
						{
							m_pWndHeaderDrop->Hide ();
						}
					}
					else if (bPosChanged)
					{
						ClientToScreen (&ptDrop);
						ShowDropIndicator (ptDrop);
					}
					else
					{
						// position has not changed - hide drop indicator
						if (m_pWndHeaderDrop != NULL)
						{
							m_pWndHeaderDrop->Hide ();
						}
					}
				}

				bToGroupByBox = TRUE;
				m_pWndHeaderDrag->m_bDrop = TRUE;
			}
			else
			{
				// --------------------
				// Dragging away.
				// Hide drop indicator:
				// --------------------
				if (m_pWndHeaderDrop != NULL)
				{
					m_pWndHeaderDrop->Hide ();
				}
			}
		}

		CRect rectWnd;
		m_pWndHeaderDrag->GetWindowRect (&rectWnd);

		m_pWndHeaderDrag->SetWindowPos
			(&wndTop,  
			ptScreen.x - rectWnd.Width () / 2, 
			ptScreen.y - rectWnd.Height () + 6,
			-1, -1, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);

		if (bDrop != m_pWndHeaderDrag->m_bDrop)
		{
			m_pWndHeaderDrag->RedrawWindow ();
		}
	}

	// -----------
	// Set cursor:
	// -----------
	BOOL bCursorSet = FALSE;

	if (m_bDragFromChooser)
	{
		if (m_pWndHeaderDrop == NULL ||
			!(m_pWndHeaderDrop->GetStyle () & WS_VISIBLE))
		{
			SetCursor (m_hcurNoDropColumn);
			bCursorSet = TRUE;
		}
	}
	else if (m_pWndHeaderDrag->m_bDrop &&
		(m_pWndHeaderDrop == NULL || !(m_pWndHeaderDrop->GetStyle () & WS_VISIBLE)) &&
		!m_bDragGroupItem && bToGroupByBox)
	{
		// column is already in the group-by-box
		SetCursor (m_hcurNoDropColumn);
		bCursorSet = TRUE;
	}
	else if (!m_pWndHeaderDrag->m_bDrop)
	{
		if (CanHideColumn (m_nDraggedColumn))
		{
			SetCursor (m_hcurDeleteColumn);
			bCursorSet = TRUE;
		}
		else
		{
			SetCursor (m_hcurNoDropColumn);
			bCursorSet = TRUE;
		}
	}

	if (!bCursorSet && bNoDropToHeader)
	{
		SetCursor (m_hcurNoDropColumn);
		bCursorSet = TRUE;
	}

	if (!bCursorSet)
	{
		SetCursor (::LoadCursor (NULL, IDC_ARROW));
	}

	return TRUE;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::StopDragColumn (CPoint point, BOOL bUpdate)
{
	CWaitCursor wait;

	if (m_nDraggedColumn < 0)
	{
		ASSERT (m_pWndHeaderDrag == NULL);
		return FALSE;
	}

	BOOL bWasMoved = (m_pWndHeaderDrag != NULL);

	if (m_pWndHeaderDrag != NULL)
	{
		m_pWndHeaderDrag->DestroyWindow ();
		m_pWndHeaderDrag = NULL;
	}

	if (m_pWndHeaderDrop != NULL)
	{
		m_pWndHeaderDrop->DestroyWindow ();
		m_pWndHeaderDrop = NULL;
	}

	CRect rect;
	m_Columns.GetColumnRect (m_nDraggedColumn, rect);

	int nDraggedColumn = m_nDraggedColumn;
	m_nDraggedColumn = -1;

	InvalidateRect (rect);
	UpdateWindow ();

	if (::GetCapture () == GetSafeHwnd ())
	{
		ReleaseCapture ();
	}

	if (!bUpdate)
	{
		return TRUE;
	}

	BOOL bHeaderSortClick = !m_bDragFromChooser && !m_bDragGroupItem &&
		(m_dwHeaderFlags & BCGP_GRID_HEADER_SORT) != 0 &&
		(m_dwHeaderFlags & BCGP_GRID_HEADER_MOVE_ITEMS) == 0;

	if (!m_bDragHeaderItems && !bHeaderSortClick)
	{
		OnHeaderColumnClick (nDraggedColumn);
		ToggleSortColumn (nDraggedColumn);
		return TRUE;
	}

	// ---------------------------
	// Drop to the column chooser:
	// ---------------------------
	if (IsColumnsChooserVisible () && m_bDragHeaderItems)
	{
		CRect rectChooser;
		m_pColumnChooser->GetWindowRect (rectChooser);

		CPoint ptScreen = point;
		ClientToScreen (&ptScreen);

		if (rectChooser.PtInRect (ptScreen))
		{
			if (!m_bDragFromChooser)
			{
				if (m_bDragGroupItem)
				{
					// ------------------------
					// Stop group by the column
					// ------------------------
					RemoveGroupColumnByVal (nDraggedColumn);
				}

				else
				{
					// -----------
					// Hide column
					// -----------
					m_Columns.SetColumnVisible (nDraggedColumn, FALSE);
					AdjustLayout ();
					m_pColumnChooser->UpdateList ();
				}
			}

			return TRUE;
		}
	}

	// -------------------------
	// Drop to the group-by-box:
	// -------------------------
	int nPos = GetGroupByBoxDropIndex (point);
	if (nPos >= 0 && m_bDragHeaderItems)
	{
		BOOL bPosChanged = TRUE;

		int nPosDraggedColumn = m_Columns.GetGroupColumnPos (nDraggedColumn);
		if (nPosDraggedColumn >= 0)
		{
			bPosChanged = (nPosDraggedColumn != nPos && 
				nPosDraggedColumn + 1 != nPos);
		}

		if (!m_bDragGroupItem && nPosDraggedColumn >= 0 ||
			!OnDropToGroupByBox (nDraggedColumn))
		{
			// column is already in the group-by-box - do nothing
		}
		else if (!bPosChanged)
		{
			if (m_bDragGroupItem && !bWasMoved)
			{
				OnHeaderColumnClick (nDraggedColumn);
				ToggleSortColumn (nDraggedColumn);
			}
		}
		else
		{
			if (!m_bDragFromChooser && !m_bDragGroupItem &&
				OnHideInsertedGroupColumn (nDraggedColumn))
			{
				m_Columns.SetColumnVisible (nDraggedColumn, FALSE);
				if (m_pColumnChooser != NULL)
				{
					m_pColumnChooser->UpdateList ();
				}
			}

			InsertGroupColumn (nPos, nDraggedColumn);
			m_pSetSelItem = m_bVirtualMode ? NULL : GetCurSelItem ();
			SetCurSel (m_idActive);
			m_pSetSelItem = NULL;
			AdjustLayout ();
		}
	}
	else
	{
		// Is over the grid header?
		BOOL bDragOverHeader = FALSE;
		int nColumn = -1;

		if (m_rectHeader.PtInRect (point))
		{
			bDragOverHeader = TRUE;
			nColumn = m_Columns.HitTestColumn (point);

			if (nColumn == -1 && m_Columns.GetColumnCount (TRUE) > 0)
			{
				bDragOverHeader = FALSE;
			}
		}

		// ------------------------
		// Drop to the grid header:
		// ------------------------
		if (bDragOverHeader)
		{
			BOOL bWasHidden = !m_Columns.GetColumnVisible (nDraggedColumn);

			// ---------------------------------------
			// Get new and old positions of the column
			// ---------------------------------------
			int nPosDraggedColumn = m_Columns.IndexToOrder(nDraggedColumn);
			int nPosDrop = nColumn >= 0 ? m_Columns.IndexToOrder (nColumn) : -1;

			if (bWasHidden && nColumn == -1)
			{
				nPosDrop = 0;//m_Columns.GetColumnCount (TRUE);
			}
			else if (nColumn >= 0 && (m_bDragGroupItem || m_bDragFromChooser))
			{
				CRect rectItem;
				m_Columns.GetColumnRect (nColumn, rectItem);

				if (point.x > rectItem.CenterPoint ().x)
				{
					nPosDrop++; // insert after
				}
			}

			// ----------------------
			// Check if can drop here
			// ----------------------
			if (nPosDrop >= 0 && m_bDragHeaderItems)
			{
				if (!CanDropColumn (nPosDrop, nPosDraggedColumn))
				{
					// Can't drop here - do nothing
					return FALSE;
				}
			}

			BOOL bNeedUpdate = FALSE;

			if (m_bDragGroupItem)
			{
				// ------------------------
				// Stop group by the column
				// ------------------------
				RemoveGroupColumnByVal (nDraggedColumn);
				bNeedUpdate = FALSE;
			}

			if (nColumn == nDraggedColumn)	// position was not changed
			{
				if (!m_bDragGroupItem && !m_bDragFromChooser && !bWasMoved)
				{
					OnHeaderColumnClick (nDraggedColumn);
					ToggleSortColumn (nDraggedColumn);
				}
			}
			else if (nPosDrop >= 0 && m_bDragHeaderItems)
			{
				// --------------------------------------
				// Insert (show) previously hidden column
				// --------------------------------------
				if (bWasHidden)//if (!m_Columns.GetColumnVisible (nDraggedColumn))
				{
					m_Columns.SetColumnVisible (nDraggedColumn, TRUE);
					if (m_pColumnChooser != NULL)
					{
						m_pColumnChooser->UpdateList ();
					}

					bNeedUpdate = TRUE;
				}

				// -----------------------------
				// Change position of the column
				// -----------------------------
				m_pSetSelItem = m_bVirtualMode ? NULL : GetCurSelItem ();
				SetCurSel (m_idActive);
				m_pSetSelItem = NULL;

				if (!m_Columns.ChangeColumnOrder (nPosDrop, nDraggedColumn))
				{
					return FALSE;
				}
				bNeedUpdate = TRUE;
			}

			if (bNeedUpdate && GetSafeHwnd () != NULL)
			{
				SetRebuildTerminalItems ();
				ReposItems ();
				
				AdjustLayout ();
			}
		}
		
		// -----------------------
		// Drop away. Hide column:
		// -----------------------
		else
		{
			if (!m_bDragHeaderItems)
			{
				// Do nothing
				return FALSE;
			}

			if (m_bDragFromChooser)
			{
				// Do nothing
				return FALSE;
			}

			if (!CanHideColumn (nDraggedColumn))
			{
				// Do nothing
				return FALSE;
			}

			if (m_bDragGroupItem)
			{
				// Stop group by the column
				RemoveGroupColumnByVal (nDraggedColumn);
				return TRUE;
			}

			m_Columns.SetColumnVisible (nDraggedColumn, FALSE);
			m_pSetSelItem = m_bVirtualMode ? NULL : GetCurSelItem ();
			SetCurSel (m_idActive);
			m_pSetSelItem = NULL;
			AdjustLayout ();
			if (m_pColumnChooser != NULL)
			{
				m_pColumnChooser->UpdateList ();
			}
		}
			
	}

	return TRUE;
}
//*******************************************************************************
void CBCGPGridCtrl::ShowDropIndicator (CPoint pt)
{
	if (m_pWndHeaderDrop == NULL)
	{
		m_pWndHeaderDrop = new CBCGPHeaderItemDropWnd;
		m_pWndHeaderDrop->Create (m_rectHeader.Height ());
	}

	m_pWndHeaderDrop->Show (pt);
}

#ifndef _BCGPGRID_STANDALONE

BOOL CBCGPGridCtrl::LoadState (LPCTSTR lpszProfileName, int nIndex)
{
	CString strProfileName;

#ifndef _BCGSUITE_
	if (g_pWorkspace == NULL ||
		AfxGetApp () == NULL || AfxGetApp ()->m_pszRegistryKey == NULL)
	{
		return FALSE;
	}

	strProfileName = g_pWorkspace->GetRegSectionPath (strGridsProfile);
#else
	CWinAppEx* pApp = DYNAMIC_DOWNCAST(CWinAppEx, AfxGetApp ());
	if (pApp == NULL || pApp->m_pszRegistryKey == NULL)
	{
		return FALSE;
	}

	strProfileName = pApp->GetRegSectionPath (strGridsProfile);
#endif

	if (lpszProfileName != NULL)
	{
		strProfileName += lpszProfileName;
	}

	CString strSection;
	strSection.Format (REG_SECTION_FMT, strProfileName, nIndex);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (!reg.Open (strSection))
	{
		return FALSE;
	}

	//-----------------------------
	// Load column order and width:
	//-----------------------------
	CArray<int, int> arColumns;
	CArray<int, int> arColumnWidth;

	reg.Read (_T ("Columns"), arColumns);
	reg.Read (_T ("ColumnWidth"), arColumnWidth);

	if (arColumns.GetSize () == arColumnWidth.GetSize () &&
		arColumnWidth.GetSize () == m_Columns.m_lstColumns.GetCount ())
	{
		int nColumn = 0;
		for (POSITION pos = m_Columns.m_lstColumns.GetHeadPosition (); pos != NULL; nColumn++)
		{
			CBCGPGridColumnsItem* pColumn = m_Columns.m_lstColumns.GetNext (pos);
			ASSERT_VALID (pColumn);

			pColumn->m_bVisible = (BOOL) arColumns [nColumn];
			pColumn->m_nDefaultSize = arColumnWidth [nColumn];
		}
	}
	else
	{
		return FALSE;
	}

	m_Columns.m_arrColumnOrder.RemoveAll ();
	reg.Read (_T ("ColumnOrder"), m_Columns.m_arrColumnOrder);

	//---------------------
	// Load groupping info:
	//---------------------
	m_Columns.m_lstGroupingColumns.RemoveAll ();
	reg.Read (_T ("Groupping"), m_Columns.m_lstGroupingColumns);

	//-------------------------
	// Load field chooser info:
	//-------------------------
	reg.Read (_T ("LastFieldChooserPos"), m_rectColumnChooser);
	reg.Read (_T ("IsFieldChooserVisible"), m_bColumnChooserVisible);

	//-------------------
	// Load sorting info:
	//-------------------
	m_Columns.m_mapSortColumn.RemoveAll ();
	reg.Read (_T ("SortColumns"), m_Columns.m_mapSortColumn);

	SetRebuildTerminalItems ();
	ReposItems ();

	AdjustLayout ();
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPGridCtrl::SaveState (LPCTSTR lpszProfileName, int nIndex)
{
	CString strProfileName;

#ifndef _BCGSUITE_
	if (g_pWorkspace == NULL ||
		AfxGetApp () == NULL || AfxGetApp ()->m_pszRegistryKey == NULL)
	{
		return FALSE;
	}

	strProfileName = g_pWorkspace->GetRegSectionPath (strGridsProfile);
#else
	CWinAppEx* pApp = DYNAMIC_DOWNCAST(CWinAppEx, AfxGetApp ());
	if (pApp == NULL || pApp->m_pszRegistryKey == NULL)
	{
		return FALSE;
	}

	strProfileName = pApp->GetRegSectionPath (strGridsProfile);
#endif

	if (lpszProfileName != NULL)
	{
		strProfileName += lpszProfileName;
	}


	CString strSection;
	strSection.Format (REG_SECTION_FMT, strProfileName, nIndex);

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (!reg.CreateKey (strSection))
	{
		return FALSE;
	}

	//-----------------------------
	// Save column order and width:
	//-----------------------------
	CArray<int, int> arColumns;
	CArray<int, int> arColumnWidth;

	for (POSITION pos = m_Columns.m_lstColumns.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridColumnsItem* pColumn = m_Columns.m_lstColumns.GetNext (pos);
		ASSERT_VALID (pColumn);

		arColumns.Add ((int) pColumn->m_bVisible);
		arColumnWidth.Add (pColumn->m_nDefaultSize);
	}

	reg.Write (_T ("Columns"), arColumns);
	reg.Write (_T ("ColumnWidth"), arColumnWidth);
	reg.Write (_T ("ColumnOrder"), m_Columns.m_arrColumnOrder);

	//---------------------
	// Save groupping info:
	//---------------------
	reg.Write (_T ("Groupping"), m_Columns.m_lstGroupingColumns);

	//-------------------------
	// Save field chooser info:
	//-------------------------
	reg.Write (_T ("LastFieldChooserPos"), m_rectColumnChooser);
	reg.Write (_T ("IsFieldChooserVisible"), m_bColumnChooserVisible);

	//-------------------
	// Save sorting info:
	//-------------------
	reg.Write (_T ("SortColumns"), m_Columns.m_mapSortColumn);

	return TRUE;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::ResetState ()
{
	// TODO!
	return TRUE;
}

#endif
void CBCGPGridCtrl::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == GRID_CLICKVALUE_TIMER_ID)
	{
		// "Click and hold" event - do not translate click, just select the item.
		KillTimer (GRID_CLICKVALUE_TIMER_ID);
		m_bClickTimer = FALSE;
		m_ptClickOnce = CPoint (0, 0);

		SetCursor (::LoadCursor (NULL, IDC_ARROW));
	}
	
	CWnd::OnTimer(nIDEvent);
}
//*****************************************************************************
BOOL CBCGPGridCtrl::DoClickValue (CBCGPGridItem* pItem, UINT uiMsg, CPoint point, 
								  BOOL bFirstClick, BOOL bButtonClick)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pItem);

	CBCGPGridRow* pCurSel = GetCurSel ();
	if (pCurSel == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pCurSel);

	// Start inplace editing:
	if (AllowInPlaceEdit () && pItem->IsAllowEdit ()
		&& (!bFirstClick || IsEditFirstClick ()))
	{
		EditItem (pCurSel, &point);
	}

	// Translate click:
	if (bButtonClick)
	{
		if (pCurSel->m_bInPlaceEdit)
		{
			if (::GetCapture () == GetSafeHwnd ())
			{
				ReleaseCapture ();
			}
		}

		pItem->DoClickButton (point);
	}
	else
	{
		return pItem->OnClickValue (uiMsg, point);
	}

	return TRUE;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::StartSelectItems ()
{
	int nLButton = GetSystemMetrics (SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
	if ((GetAsyncKeyState (nLButton) & 0x8000) == 0)
	{
		return FALSE;
	}

	// Start selecting range of items:
	SetCapture ();
	m_bSelecting = TRUE;

    return TRUE;
}
//*****************************************************************************
void CBCGPGridCtrl::StopSelectItems ()
{
	m_bSelecting = FALSE;

	if (::GetCapture () == GetSafeHwnd ())
	{
		ReleaseCapture ();
	}
}
//*****************************************************************************
BOOL CBCGPGridCtrl::SelectItems (CPoint ptClient)
{
	// ------------------------
	// perform range selection:
	// ------------------------
	CBCGPGridItemID id;
	CBCGPGridItem* pHitItem = NULL;
	CBCGPGridRow* pHitRow = HitTest (ptClient, id, pHitItem);

	BOOL bSelChanged = id != m_idLastSel;

	if (!id.IsNull () && bSelChanged)
	{
		DWORD dwSelMode = SM_SECOND_CLICK | SM_CONTINUE_SEL_GROUP;
		m_pSetSelItem = m_bVirtualMode ? NULL : pHitItem;

		SetCurSel (id, dwSelMode);

		m_pSetSelItem = NULL;

		EnsureVisible (pHitRow);
	}

    return TRUE;
}
//*****************************************************************************
CBCGPGridRow* CBCGPGridCtrl::GetVirtualRow (int nRow)
{
	if (nRow < 0)
	{
		return NULL;
	}

	CBCGPGridRow* pRow = m_CachedItems.GetCachedRow (nRow);
	if (pRow == NULL)
	{
		pRow = CreateVirtualRow (nRow);

		if (!m_CachedItems.SetCachedRow (nRow, pRow))
		{
			ASSERT (FALSE);
			return NULL;
		}

		//-------------------------------
		// Check whether row is selected:
		//-------------------------------
		for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
			ASSERT (pRange != NULL);

			// Is row in selected range?
			if (pRange->m_nTop <= nRow && pRange->m_nBottom >= nRow)
			{
				DoSelectRowInRange (pRow, *pRange, TRUE, FALSE);
			}
		}
	}

	return pRow;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::SendDispInfoRequest (BCGPGRID_DISPINFO* pdi) const
{
    ASSERT (pdi != NULL);

	pdi->item.varValue.Clear ();
	pdi->item.dwData = 0;
	pdi->item.iImage = -1;
	pdi->item.clrBackground = m_ColorData.m_clrBackground;
	pdi->item.clrText = m_ColorData.m_clrText;
	memset (&pdi->item.lfFont, 0, sizeof (LOGFONT));

	if (m_pfnCallback != NULL)
	{
		return m_pfnCallback (pdi, m_lParamCallback);
	}
	else
	{
		// Send the notification message
		pdi->hdr.hwndFrom = m_hWnd;
		pdi->hdr.idFrom   = GetDlgCtrlID();
		pdi->hdr.code     = BCGPGN_GETDISPINFO;

		CWnd* pOwner = GetOwner ();

		if (pOwner != NULL && IsWindow(pOwner->m_hWnd))
		{
			return (BOOL) pOwner->SendMessage (	WM_NOTIFY, 
												pdi->hdr.idFrom, (LPARAM)pdi);
		}
	}

    return FALSE;
}
//*****************************************************************************
CBCGPGridRow* CBCGPGridCtrl::CreateVirtualRow (int nRowIndex)
{
	if (!m_bVirtualMode)
	{
		ASSERT (FALSE);
		return NULL;
	}

	//------------
	// Create row:
	//------------
	static BCGPGRID_DISPINFO di;
	di.item.nRow	= nRowIndex;	// Row index
	di.item.nCol	= -1;			// Request row info

	if (!SendDispInfoRequest (&di))
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPGridRow* pRow = OnCreateVirtualRow (&di);
	if (pRow == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	ASSERT_VALID (pRow);

	//--------------
	// Create items:
	//--------------
	for (int i = 0; i < GetColumnCount () ; i++)
	{
		di.item.nRow	= nRowIndex;
		di.item.nCol	= i;

		if (!SendDispInfoRequest (&di))
		{
			ASSERT (FALSE);
			break;
		}

		CBCGPGridItem* pItem = OnCreateVirtualItem (&di);
		if (pItem != NULL)
		{
			ASSERT_VALID (pItem);

			pItem->SetOwnerRow (pRow);
			int nColIndex = (int) pRow->m_arrRowItems.Add (pItem);
			pItem->m_nIdColumn = nColIndex;

			pItem->m_strLabel = pItem->FormatItem ();
		}
	}

	//-------------------
	// Calc row position:
	//-------------------
	if (m_bIsPrinting)
	{
		int y = m_PrintParams.m_rectList.top - 1 + nRowIndex * m_PrintParams.m_nRowHeight -
			m_PrintParams.m_nVertScrollOffset;
		m_PrintParams.m_idCur.m_nRow = nRowIndex;
		pRow->Repos (y);
	}
	else
	{
		int y = m_rectList.top - 1 + nRowIndex * m_nRowHeight - m_nVertScrollOffset;
		m_idCur.m_nRow = nRowIndex;
		pRow->Repos (y);
	}

	return pRow;
}
//*****************************************************************************
CBCGPGridRow* CBCGPGridCtrl::OnCreateVirtualRow (BCGPGRID_DISPINFO *pdi)
{
	ASSERT_VALID (this);
	ASSERT (pdi != NULL);

	int nRow = pdi->item.nRow;

	CBCGPGridRow* pRow = CreateRow ();
	ASSERT_VALID (pRow);

	pRow->m_pWndList = this;
	pRow->m_nIdRow = nRow;

	pRow->m_dwData = pdi->item.dwData;

	return pRow;
}
//*****************************************************************************
CBCGPGridItem* CBCGPGridCtrl::OnCreateVirtualItem (BCGPGRID_DISPINFO *pdi)
{
	ASSERT_VALID (this);
	ASSERT (pdi != NULL);

	int nRow = pdi->item.nRow;
	int nColumn = pdi->item.nCol;

	CBCGPGridItem* pItem = NULL;

	switch (pdi->item.varValue.vt)
	{
	case VT_BOOL:
	case VT_BSTR:
	case VT_R4:
	case VT_R8:
	case VT_UI1:
	case VT_I2:
	case VT_I4:
	case VT_INT:
	case VT_UINT:
	case VT_UI2:
	case VT_UI4:
	case VT_DATE:

		pItem = CreateItem (nRow, nColumn);
		ASSERT_VALID (pItem);

		pItem->OnEndEdit ();

		pItem->m_varValue = pdi->item.varValue;

		pItem->Init ();
		pItem->SetFlags ();
		break;

	default:
		// Empty item for unsupported types:
		pItem = CreateItem (nRow, nColumn);
		ASSERT_VALID (pItem);

		pItem->OnEndEdit ();
		pItem->m_varValue.Clear ();

		pItem->Init ();
		pItem->SetFlags ();
		break;
	}

	if (pItem != NULL)
	{
		pItem->m_nIdColumn = nColumn;

		pItem->m_dwData = pdi->item.dwData;
		pItem->m_clrBackground = pdi->item.clrBackground;
		pItem->m_clrText = pdi->item.clrText;
		pItem->m_iImage = pdi->item.iImage;
	}

	return pItem;
}
//*****************************************************************************
CRect CBCGPGridCtrl::GetVirtualRowRect (int nRow) const
{
    ASSERT_VALID (this);

	if (nRow < 0 || nRow >= m_nVirtualRows)
	{
		CRect rectEmpty;
		rectEmpty.SetRectEmpty ();
		return rectEmpty;
	}

    int nHorzScrollOffset = m_nHorzScrollOffset;

    int nRowTop = m_rectList.top - m_nVertScrollOffset - 1 + nRow * m_nRowHeight;
	int nRowLeft = m_rectList.left - nHorzScrollOffset;
	int nRowWidth = m_Columns.GetTotalWidth ();

	BOOL bIsGroup = FALSE;//IsGroup ();
	CRect rect = CRect (nRowLeft, nRowTop,
		nRowLeft + nRowWidth, nRowTop + (bIsGroup ? m_nLargeRowHeight : m_nRowHeight));

    return rect;
}
//*****************************************************************************
CRect CBCGPGridCtrl::GetVirtualItemRect (int nRow, int nColumn) const
{
	ASSERT_VALID (this);
	ASSERT (nRow >= 0);
	ASSERT (nColumn >= 0);

	CRect rectRow = GetVirtualRowRect (nRow);
	CRect rect = rectRow;

	int nXMul = 1, nXDiv = 1;
	if (m_bIsPrinting)
	{
		// map to printer metrics
		ASSERT_VALID (m_pPrintDC);
		HDC hDCFrom = ::GetDC(NULL);

		nXMul = m_pPrintDC->GetDeviceCaps(LOGPIXELSX); // pixels in print dc
		nXDiv = ::GetDeviceCaps(hDCFrom, LOGPIXELSX); // pixels in screen dc

		::ReleaseDC(NULL, hDCFrom);
	}

	int nXLeft = rectRow.left;
	int nCount = 0;

	int nPos = m_Columns.Begin ();
	while (nPos != m_Columns.End ())
	{
		int iColumn = m_Columns.Next (nPos);
		if (iColumn == -1)
		{
			break; // no more visible columns
		}

		ASSERT (iColumn >= 0);
		int nWidth = m_Columns.GetColumnWidth (iColumn);

		if (m_bIsPrinting)
		{
			nWidth = ::MulDiv (nWidth, nXMul, nXDiv);
		}

        if (nColumn == iColumn)
		{
			rect.left = nXLeft;
			rect.right = rect.left + nWidth;
			return rect;
		}

		nXLeft += nWidth;
		nCount ++;
	}

	rect.SetRectEmpty ();
	return rect;
}
//*****************************************************************************
CBCGPGridItemID CBCGPGridCtrl::HitTestVirtual (CPoint pt, 
											   CBCGPGridRow::ClickArea* pnArea) const
{
	ASSERT (m_bIsPrinting == FALSE);

	//----------------
	// Calc row index:
	//----------------
	int nYTop = (pt.y - m_rectList.top + m_nVertScrollOffset + 1);
	int nRowIdx = (nYTop >= 0) ? (nYTop / m_nRowHeight) : -1;
	if (pnArea != NULL)
	{
		*pnArea = CBCGPGridRow::ClickName;
	}
	
	if (nRowIdx < 0 ||
		nRowIdx >= m_nVirtualRows ||
		!GetVirtualRowRect (nRowIdx).PtInRect (pt))
	{
		CBCGPGridItemID idNull;
		return idNull;
	}

	//-------------------
	// Calc column index:
	//-------------------
	int nColumnIdx = -1;

	int dx = 0;//GetHierarchyLevel () * m_nRowHeight;
	int nXLeft = m_rectList.left - m_nHorzScrollOffset;
	int nCount = 0;

	int nPos = m_Columns.Begin ();
	while (nPos != m_Columns.End ())
	{
		int iColumn = m_Columns.Next (nPos);
		if (iColumn == -1)
		{
			break; // no more visible columns
		}

		ASSERT (iColumn >= 0);
		int nWidth = m_Columns.GetColumnWidth (iColumn);

		BOOL bIsTreeColumn = (m_nTreeColumn == -1) ? (nCount == 0): (m_nTreeColumn == iColumn);
		if (bIsTreeColumn)
		{
			nWidth += GetExtraHierarchyOffset () + GetHierarchyOffset ();
		}

		if (pt.x < nXLeft + nWidth)
		{
			int nTreeOffset = bIsTreeColumn ? GetExtraHierarchyOffset () + dx : 0; 
			if (pt.x >= nXLeft + nTreeOffset)
			{
				nColumnIdx = iColumn;
				if (pnArea != NULL)
				{
					*pnArea = CBCGPGridRow::ClickValue;
				}
				break;
			}

			BOOL bIsGroup = FALSE;//IsGroup ();
			if (bIsGroup && pt.x >= nXLeft + dx)
			{
				if (pnArea != NULL)
				{
					*pnArea = CBCGPGridRow::ClickExpandBox;
				}
				break;
			}
		}

		nXLeft += nWidth;
		nCount++;
	}

	CBCGPGridItemID id (nRowIdx, nColumnIdx);
	return id;
}
//*****************************************************************************
void CBCGPGridCtrl::OnSelChanging (const CBCGPGridRange &range, BOOL bSelect)
{
	BCGPGRID_NOTIFICATION gn;

    gn.nRow = range.m_nTop;
	gn.nCol = range.m_nLeft;
    gn.nRowTo = range.m_nBottom;
	gn.nColTo = range.m_nRight;
	gn.lParam = (LPARAM) bSelect;

	SendNotification (&gn, BCGPGN_SELCHANGING);
}
//*****************************************************************************
void CBCGPGridCtrl::OnSelChanged (const CBCGPGridRange &range, BOOL bSelect)
{
	BCGPGRID_NOTIFICATION gn;

    gn.nRow = range.m_nTop;
	gn.nCol = range.m_nLeft;
    gn.nRowTo = range.m_nBottom;
	gn.nColTo = range.m_nRight;
	gn.lParam = (LPARAM) bSelect;

	SendNotification (&gn, BCGPGN_SELCHANGED);

	InvalidateRect (m_rectHeader);
}
//*****************************************************************************
BOOL CBCGPGridCtrl::SendNotification (BCGPGRID_NOTIFICATION* pn, UINT uCode) const
{
    ASSERT (pn != NULL);

	// Send the notification message
	pn->hdr.hwndFrom = m_hWnd;
	pn->hdr.idFrom   = GetDlgCtrlID();
	pn->hdr.code     = uCode;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
	{
		return (BOOL) pOwner->SendMessage(WM_NOTIFY, pn->hdr.idFrom, (LPARAM)pn);
	}

    return FALSE;
}
//*****************************************************************************
void CBCGPGridCtrl::OnResizeColumns ()
{
}
//*****************************************************************************
void CBCGPGridCtrl::OnHideColumnChooser ()
{
	ASSERT_VALID (this);

	CWnd* pOwner = GetOwner ();
	if (pOwner != NULL)
	{
		pOwner->SendMessage (BCGM_GRID_ON_HIDE_COLUMNCHOOSER);
	}
}
//*****************************************************************************
void CBCGPGridCtrl::UpdateColumnsChooser ()
{
	ASSERT_VALID (this);

	if (m_pColumnChooser != NULL)
	{
		m_pColumnChooser->UpdateList ();
	}
}
//*****************************************************************************
void CBCGPGridCtrl::MergeSelection (BOOL bRedraw)
{
	ASSERT_VALID (this);

	for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRange* pSelRange = m_lstSel.GetNext (pos);
		ASSERT (pSelRange != NULL);

		MergeRange (*pSelRange, bRedraw);
	}
}
//*****************************************************************************
void CBCGPGridCtrl::UnMergeSelection (BOOL bRedraw)
{
	ASSERT_VALID (this);

	for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRange* pSelRange = m_lstSel.GetNext (pos);
		ASSERT (pSelRange != NULL);

		UnMergeRange (*pSelRange, bRedraw);
	}
}
//*****************************************************************************
void CBCGPGridCtrl::MergeRange (const CBCGPGridRange& range, BOOL bRedraw)
{
	ASSERT_VALID (this);

	if (IsRowExtraHeightAllowed ())
	{
		ASSERT (FALSE); // The grid must have fixed row size
		return;
	}

	if (range.m_nLeft < 0 || range.m_nTop < 0 ||
		range.m_nRight < 0 || range.m_nBottom < 0)
	{
		return;
	}

	EndEditItem ();

	if (!CanMergeRange (range, TRUE))
	{
		return;
	}

	CBCGPGridRange rangeExt = range;
	if (ExtendMergedRange (rangeExt))
	{
		// Range already contains some merged items. Unmerge them first.
		TRACE0 ("\nCBCGPGridCtrl::MergeRange: Can't merge.");
		return;
	}

	OnMergeCellsChanging (range, TRUE);

	CBCGPGridMergedCells* pMergedRange = new CBCGPGridMergedCells;
	pMergedRange->SetRange (range);

	CBCGPGridRange rangeNormalized (
			min (range.m_nLeft, range.m_nRight),
			min (range.m_nBottom, range.m_nTop),
			max (range.m_nLeft, range.m_nRight),
			max (range.m_nBottom, range.m_nTop));

	DoMergeInRange (rangeNormalized, pMergedRange);

	pMergedRange->Release ();
	pMergedRange = NULL; // pMergedRange can't be used now

	OnMergeCellsChanged (range, TRUE);

	if (bRedraw)
	{
		AdjustLayout ();
	}
}
//*****************************************************************************
void CBCGPGridCtrl::UnMergeRange (const CBCGPGridRange& range, BOOL bRedraw)
{
	ASSERT_VALID (this);

	if (IsRowExtraHeightAllowed ())
	{
		ASSERT (FALSE); // The grid must have fixed row size
		return;
	}

	if (range.m_nLeft < 0 || range.m_nTop < 0 ||
		range.m_nRight < 0 || range.m_nBottom < 0)
	{
		return;
	}

	EndEditItem ();

	if (!CanMergeRange (range, FALSE))
	{
		return;
	}

	CBCGPGridRange rangeExt = range;
	if (ExtendMergedRange (rangeExt))
	{
		// Some merged items are partitially in range. Range should be extended.
		TRACE0 ("\nCBCGPGridCtrl::UnMergeRange: Can't unmerge.");
		return;
	}

	CBCGPGridRange rangeNormalized (
			min (range.m_nLeft, range.m_nRight),
			min (range.m_nBottom, range.m_nTop),
			max (range.m_nLeft, range.m_nRight),
			max (range.m_nBottom, range.m_nTop));

	OnMergeCellsChanging (rangeNormalized, FALSE);

	DoMergeInRange (rangeNormalized, NULL);

	OnMergeCellsChanged (rangeNormalized, FALSE);

	if (bRedraw)
	{
		AdjustLayout ();
	}
}
//*****************************************************************************
void CBCGPGridCtrl::DoMergeInRange (const CBCGPGridRange& rangeNormalized, CBCGPGridMergedCells* pMergedCells)
{	// if pMergedCells is NULL, then unmerge cells
	ASSERT_VALID (this);

	if (m_bVirtualMode)
	{
		// iterate through cached items
		for (POSITION pos = m_CachedItems.m_lstCache.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridCachePageInfo& cpi = m_CachedItems.m_lstCache.GetNext (pos);
			ASSERT (cpi.pArrCachePage->GetSize() == cpi.nSize);

			int nOffset = max (0, rangeNormalized.m_nTop - cpi.nFirst);
			while (nOffset >= 0 && nOffset < cpi.nSize && 
				nOffset <= rangeNormalized.m_nBottom - cpi.nFirst)
			{
				CBCGPGridRow* pRow = cpi.pArrCachePage->GetAt (nOffset);
				if (pRow != NULL)
				{
					ASSERT_VALID (pRow);
					DoMergeRowItemsInRange (pRow, rangeNormalized, pMergedCells);
				}

				nOffset++;
			}
		}
	}
	else
	//if (IsValidRange (range))
	{
		const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
			(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

		POSITION pos = lst.FindIndex (rangeNormalized.m_nTop);
		for (int i = rangeNormalized.m_nTop; pos != NULL && i <= rangeNormalized.m_nBottom; i++)
		{
			CBCGPGridRow* pRow = lst.GetNext (pos);
			ASSERT_VALID (pRow);

			DoMergeRowItemsInRange (pRow, rangeNormalized, pMergedCells);
		}
	}
}
//*****************************************************************************
void CBCGPGridCtrl::DoMergeRowItemsInRange (CBCGPGridRow* pRow, const CBCGPGridRange& range,
											CBCGPGridMergedCells* pMergedCells)
{	// if pMergedCells is NULL, then unmerge cells
	ASSERT_VALID (pRow);

	BOOL bInRange = FALSE;

	int nPos = m_Columns.Begin ();
	while (nPos != m_Columns.End ())
	{
		int iColumn = m_Columns.Next (nPos);
		if (iColumn == -1)
		{
			break; // no more visible columns
		}

		BOOL bIsRangeBound = (iColumn == range.m_nLeft || iColumn == range.m_nRight);

		if (bIsRangeBound || bInRange)
		{
			CBCGPGridItem* pItem = pRow->GetItem (iColumn);

			if (pItem != NULL)
			{
				ASSERT_VALID (pItem);
				pItem->Merge (pMergedCells);
			}
		}

		if (bIsRangeBound)
		{
			if (bInRange || range.m_nLeft == range.m_nRight)
			{
				break;	// last visible column in range
			}

			bInRange = TRUE;
		}
	}
}
//*****************************************************************************************************
BOOL CBCGPGridCtrl::ExtendMergedRange (CBCGPGridRange& range)
{
	ASSERT_VALID (this);

	// Extends the range to make it larger if has merged cells. 
	// The result range contains all existing merged items that are partitially fit in the source range.
	CList <CBCGPGridRange, CBCGPGridRange&> lstMergedRanges;
	if (GetMergedItemsInRange (range, lstMergedRanges) > 0)
	{
		CBCGPGridRange rangeNew = range;

		for (POSITION pos = lstMergedRanges.GetHeadPosition (); pos != NULL; )
		{
			CBCGPGridRange& rangeMerged = lstMergedRanges.GetNext (pos);
			UnionRange (&rangeNew, &rangeMerged);
		}

		return (rangeNew != range);
	}

	return FALSE;
}
//*****************************************************************************************************
int CBCGPGridCtrl::GetMergedItemsInRange (const CBCGPGridRange& range, 
										  CList <CBCGPGridRange, CBCGPGridRange&> &lstRanges)
{
	ASSERT_VALID (this);

	CBCGPGridRange rangeNormalized (
			min (range.m_nLeft, range.m_nRight),
			min (range.m_nBottom, range.m_nTop),
			max (range.m_nLeft, range.m_nRight),
			max (range.m_nBottom, range.m_nTop));

	if (!m_bVirtualMode)
	{
		const CList<CBCGPGridRow*, CBCGPGridRow*>& lst =
			(!IsSortingMode () && !IsGrouping ()) ? m_lstItems : m_lstTerminalItems;

		POSITION pos = lst.FindIndex (rangeNormalized.m_nTop);
		for (int i = rangeNormalized.m_nTop; pos != NULL && i <= rangeNormalized.m_nBottom; i++)
		{
			CBCGPGridRow* pRow = lst.GetNext (pos);
			ASSERT_VALID (pRow);

			DoGetMergedItemsInRange (pRow, rangeNormalized, lstRanges);
		}
	}

	return (int) lstRanges.GetCount ();
}
//*****************************************************************************************************
void CBCGPGridCtrl::DoGetMergedItemsInRange (CBCGPGridRow* pRow, const CBCGPGridRange& range, 
											 CList <CBCGPGridRange, CBCGPGridRange&> &lstRanges)
{
	ASSERT_VALID (pRow);

	BOOL bInRange = FALSE;

	int nPos = m_Columns.Begin ();
	while (nPos != m_Columns.End ())
	{
		int iColumn = m_Columns.Next (nPos);
		if (iColumn == -1)
		{
			break; // no more visible columns
		}

		BOOL bIsRangeBound = (iColumn == range.m_nLeft || iColumn == range.m_nRight);

		if (bIsRangeBound || bInRange)
		{
			CBCGPGridItem* pItem = pRow->GetItem (iColumn);

			if (pItem != NULL)
			{
				ASSERT_VALID (pItem);

				CBCGPGridRange range;
				if (pItem->GetMergedRange (range))
				{
					// add once:
					BOOL bAlreadyInList = FALSE;
					for (POSITION pos = lstRanges.GetTailPosition (); pos != NULL && !bAlreadyInList; )
					{
						CBCGPGridRange& rangeSaved = lstRanges.GetPrev (pos);
						if (rangeSaved == range)
						{
							bAlreadyInList = TRUE;
						}
					}

					if (!bAlreadyInList)
					{
						lstRanges.AddTail (range);
					}
				}
			}
		}

		if (bIsRangeBound)
		{
			if (bInRange || range.m_nLeft == range.m_nRight)
			{
				break;	// last visible column in range
			}

			bInRange = TRUE;
		}
	}
}
//*****************************************************************************
void CBCGPGridCtrl::MarkMergedItemChanged (const CRect& rectNew, CBCGPGridItem* pItem)
{
	ASSERT_VALID (this);

	if (pItem == NULL)
	{
		return;
	}

	ASSERT_VALID (pItem);

	if (pItem->m_Rect.top >= m_rectList.bottom ||
		pItem->m_Rect.bottom < m_rectList.top)
	{
		return; // item does not fit in the visible area
	}

	CBCGPGridMergedCells* pMerged = pItem->GetMergedCells ();
	if (pMerged != NULL)
	{
		ASSERT_VALID (pMerged);

		// save id of the first visible item in range
		CBCGPGridItemID id = pItem->GetGridItemID ();
		pMerged->MarkChanged (rectNew, id);

		// save merged range in list to update later
		CBCGPGridItemID idFirstVisible = pMerged->GetVisibleItemID ();
		if (id == idFirstVisible)
		{
			m_lstMergedItemsToUpdate.AddTail (pItem);
		}
	}
}
//*****************************************************************************
void CBCGPGridCtrl::UpdateMergedItems ()
{
	ASSERT_VALID (this);

	while (!m_lstMergedItemsToUpdate.IsEmpty ())
	{
		CBCGPGridItem* pMergedItem = m_lstMergedItemsToUpdate.RemoveHead ();
		ASSERT_VALID (pMergedItem);

		CBCGPGridMergedCells* pMerged = pMergedItem->GetMergedCells ();
		ASSERT_VALID (pMerged);

		if (pMerged->IsChanged () && !pMerged->GetVisibleItemID ().IsNull ())
		{
			CBCGPGridRange& range = pMerged->GetRange ();

			//------------------------------------
			// Calc boundaries of the merged item:
			//------------------------------------
			CRect rectMerged = GetMergedRect (&range, pMerged->GetVisibleItemID ());
			pMerged->SetRect (rectMerged);

			//TODO add notofication
			//for each item or for main item
			//pItem->OnMergedItemChanged (&range, rectMerged);

			pMerged->MarkUpdated ();
		}
	}
}
//*****************************************************************************
CRect CBCGPGridCtrl::GetMergedRect (const CBCGPGridRange* pRange,
									 const CBCGPGridItemID& idVisible)
{	// TODO to speed up add parameter (const CBCGPGridItem* pItemVisible)
	ASSERT_VALID (this);
	ASSERT (pRange != NULL);
	ASSERT (pRange->IsValid ());
	ASSERT (pRange->IsInRange (idVisible));

	CRect rectResult (0, 0, 0, 0);

	CBCGPGridRow* pRowVisible = GetRow (idVisible.m_nRow);
	CBCGPGridItem* pItemVisible = (pRowVisible != NULL) ?
		pRowVisible->GetItem (idVisible.m_nColumn) : NULL;
	if (pItemVisible == NULL)
	{
		return rectResult;
	}

	ASSERT_VALID (pItemVisible);

	CRect rectVisible = pItemVisible->GetRect ();
	if (rectVisible.IsRectEmpty ())
	{
		return rectResult;
	}

	//-------------------------------------
	// Calculate top and bottom boundaries:
	//-------------------------------------
	rectResult = rectVisible;
	if (pRange->m_nTop < idVisible.m_nRow)
	{
		int nRowsBefore = idVisible.m_nRow - pRange->m_nTop;
		rectResult.top -= nRowsBefore * rectVisible.Height ();
	}
	if (pRange->m_nBottom > idVisible.m_nRow)
	{
		int nRowsAfter = pRange->m_nBottom - idVisible.m_nRow;
		rectResult.bottom += nRowsAfter * rectVisible.Height ();
	}

	//-------------------------------------
	// Calculate left and right boundaries:
	//-------------------------------------
	if (pRange->m_nLeft < idVisible.m_nColumn)
	{
		int nBefore = 0;

		for (int iColumn = pRange->m_nLeft;
			iColumn < idVisible.m_nColumn && iColumn < m_Columns.GetColumnCount ();
			iColumn++)
		{
			int nWidth = m_Columns.GetColumnWidth (iColumn);
			if (m_nTreeColumn == iColumn)
			{
				nWidth += GetHierarchyOffset () + GetExtraHierarchyOffset ();
			}
			nBefore += nWidth;
		}

		rectResult.left -= nBefore;
	}
	if (pRange->m_nRight > idVisible.m_nColumn)
	{
		int nAfter = 0;

		for (int iColumn = idVisible.m_nColumn + 1;
			iColumn <= pRange->m_nRight && iColumn < m_Columns.GetColumnCount ();
			iColumn++)
		{
			int nWidth = m_Columns.GetColumnWidth (iColumn);
			BOOL bIsTreeColumn = (m_nTreeColumn == -1) ? 
				(0 == iColumn) : (m_nTreeColumn == iColumn);
			if (bIsTreeColumn)
			{
				nWidth += GetHierarchyOffset () + GetExtraHierarchyOffset ();
			}
			nAfter += nWidth;
		}

		rectResult.right += nAfter;
	}

	return rectResult;
}
//*****************************************************************************
CBCGPGridRange* CBCGPGridCtrl::GetMergedRange (CBCGPGridItemID id,
											   CBCGPGridItem* pItem, 
											   CBCGPGridRange& range)
{
	ASSERT_VALID (this);

	CBCGPGridItem* pMergedItem = pItem;
	if (m_bVirtualMode || pItem == NULL)
	{
		CBCGPGridRow* pRow = (id.m_nRow != -1) ? GetRow (id.m_nRow) : NULL;
		pMergedItem = (pRow != NULL) ? pRow->GetItem (id.m_nColumn) : NULL;
	}

	if (pMergedItem != NULL)
	{
		ASSERT_VALID (pMergedItem);

		if (pMergedItem->GetMergedRange (range))
		{
			return &range;
		}
	}

	return NULL;
}
//*****************************************************************************
void CBCGPGridCtrl::AllowRowExtraHeight (BOOL bAllow)
{
	m_bAllowRowExtraHeight = bAllow;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::IsRowExtraHeightAllowed () const
{
	return m_bAllowRowExtraHeight;
}
//*****************************************************************************
BOOL CBCGPGridCtrl::IsPreviewRowEnabled () const
{
	return FALSE;
}
//*****************************************************************************
CRect CBCGPGridCtrl::OnGetPreviewRowMargins (CBCGPGridRow*) const
{
	CRect rect;
	rect.SetRectEmpty ();

	return rect;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPHeaderItemDragWnd

CString	CBCGPHeaderItemDragWnd::m_strClassName;

CBCGPHeaderItemDragWnd::CBCGPHeaderItemDragWnd()
{
	m_pWndGrid = NULL;
	m_nItem = -1;
	m_bDrop = TRUE;

	if (m_strClassName.IsEmpty ())
	{
		m_strClassName = ::AfxRegisterWndClass (
			CS_SAVEBITS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_BTNFACE + 1), NULL);
	}
}

CBCGPHeaderItemDragWnd::~CBCGPHeaderItemDragWnd()
{
}

BEGIN_MESSAGE_MAP(CBCGPHeaderItemDragWnd, CWnd)
	//{{AFX_MSG_MAP(CBCGPHeaderItemDragWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPHeaderItemDragWnd message handlers

BOOL CBCGPHeaderItemDragWnd::Create (CBCGPGridCtrl* pGrid, int nItem)
{
	ASSERT_VALID(pGrid);
	ASSERT(nItem >= 0);

	m_pWndGrid = pGrid;
	m_nItem = nItem;

	CRect rect;
	pGrid->m_Columns.GetColumnRect (nItem, rect);

	pGrid->ClientToScreen (&rect);

	if (m_pWndGrid->m_bDragGroupItem || m_pWndGrid->m_bDragFromChooser)
	{
		CString strLabel = m_pWndGrid->m_Columns.GetColumnName (m_nItem);

		CClientDC dc (m_pWndGrid);
		HFONT hfontOld = m_pWndGrid->SetCurrFont (&dc);

		CSize sizeText = dc.GetTextExtent (strLabel);

		if (m_pWndGrid->m_bDragGroupItem)
		{
			rect.right = rect.left + sizeText.cx + 
				BCGPGRID_GROUPBYBOX_COLUMNWIDTH + TEXT_MARGIN;
		}
		else
		{
			int nWidth = (pGrid->m_pColumnChooser != NULL) ? 
				pGrid->m_pColumnChooser->GetColumnWidth () : 0;
			rect.right = rect.left + max (sizeText.cx + 20, nWidth);
		}
		rect.bottom = rect.top + sizeText.cy + 10;

		::SelectObject (dc.GetSafeHdc (), hfontOld);
	}

	return CreateEx (WS_EX_TOOLWINDOW | WS_EX_TOPMOST, 
		m_strClassName, NULL, WS_POPUP | WS_DISABLED,
		rect, NULL, 0);
}
//*******************************************************************************
void CBCGPHeaderItemDragWnd::OnPaint() 
{
	ASSERT_VALID(m_pWndGrid);

	CPaintDC dc(this); // device context for painting
	
	CRect rectClient;
	GetClientRect (rectClient);

	dc.FillRect (rectClient, &globalData.brBtnFace);

	HFONT hfontOld = m_pWndGrid->SetCurrFont (&dc);

	dc.SetTextColor (globalData.clrBtnText);
	dc.SetBkMode (TRANSPARENT);

	if (m_pWndGrid->m_bDragGroupItem || m_pWndGrid->m_bDragFromChooser)
	{
		CRect rectLabel = rectClient;
		rectLabel.DeflateRect (5, 0);

		CString strLabel = m_pWndGrid->m_Columns.GetColumnName (m_nItem);
		UINT uiTextFlags = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;

		dc.DrawText (strLabel, rectLabel, uiTextFlags);

		dc.Draw3dRect (rectClient,
			globalData.clrBtnHilite,
			globalData.clrBtnShadow);
	}
	else
	{
		m_pWndGrid->m_Columns.m_bDrawingDraggedColumn = TRUE;
		m_pWndGrid->m_Columns.DrawColumn (&dc, m_nItem, rectClient, 5, 5, FALSE, TRUE, TRUE);
		m_pWndGrid->m_Columns.m_bDrawingDraggedColumn = FALSE;
	}

	::SelectObject (dc.GetSafeHdc (), hfontOld);
}
//*******************************************************************************
BOOL CBCGPHeaderItemDragWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//*******************************************************************************
void CBCGPHeaderItemDragWnd::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}
//*******************************************************************************
BOOL CBCGPHeaderItemDragWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (!m_bDrop)
	{
		SetCursor (NULL);
		return TRUE;
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPHeaderItemDropWnd

CString	CBCGPHeaderItemDropWnd::m_strClassName;

CBCGPHeaderItemDropWnd::CBCGPHeaderItemDropWnd()
{
	if (m_strClassName.IsEmpty ())
	{
		m_strClassName = ::AfxRegisterWndClass (
			CS_SAVEBITS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_BTNFACE + 1), NULL);
	}
}

CBCGPHeaderItemDropWnd::~CBCGPHeaderItemDropWnd()
{
}


BEGIN_MESSAGE_MAP(CBCGPHeaderItemDropWnd, CWnd)
	//{{AFX_MSG_MAP(CBCGPHeaderItemDropWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPHeaderItemDropWnd message handlers


BOOL CBCGPHeaderItemDropWnd::Create (int nColumnHeight) 
{
	const int nArrowSize = 9;

	int nWndHeight = nColumnHeight + nArrowSize + nArrowSize;

	CRect rectWindow (0, 0, nArrowSize, nWndHeight);

	if (!CreateEx (WS_EX_TOOLWINDOW,
		m_strClassName, NULL, WS_POPUP | WS_DISABLED,
		rectWindow, NULL, 0))
	{
		return FALSE;
	}

	#define ARROW_PTS 8

	POINT pts1 [ARROW_PTS] =
	{
		{	3,	0	},
		{	3,	4	},
		{	0,	4	},
		{	5,	9	},
		{	5,	8	},
		{	9,	4	},
		{	6,	4	},
		{	6,	0	},
	};

	CRgn rgn1;
	rgn1.CreatePolygonRgn (pts1, ARROW_PTS, ALTERNATE);

	POINT pts2 [ARROW_PTS] =
	{
		{	3,	nColumnHeight + nArrowSize + 9	},
		{	3,	nColumnHeight + nArrowSize + 5	},
		{	-1,	nColumnHeight + nArrowSize + 5	},
		{	5,	nColumnHeight + nArrowSize - 1	},
		{	5,	nColumnHeight + nArrowSize + 0	},
		{	9,	nColumnHeight + nArrowSize + 5	},
		{	6,	nColumnHeight + nArrowSize + 5	},
		{	6,	nColumnHeight + nArrowSize + 9	},
	};


	CRgn rgn2;
	rgn2.CreatePolygonRgn (pts2, ARROW_PTS, ALTERNATE);

	CRgn rgn;
	rgn.CreateRectRgnIndirect (rectWindow);

	rgn.CombineRgn (&rgn1, &rgn2, RGN_OR);

	SetWindowRgn (rgn, FALSE);

	rgn1.DeleteObject();
	rgn2.DeleteObject();
	rgn.DeleteObject();

	return TRUE;
}
//*******************************************************************************
void CBCGPHeaderItemDropWnd::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}
//*******************************************************************************
BOOL CBCGPHeaderItemDropWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);

	pDC->FillSolidRect (rectClient, RGB (255, 0, 0));
	return TRUE;
}
//*******************************************************************************
void CBCGPHeaderItemDropWnd::Show (CPoint point)
{
	CRect rectClient;
	GetClientRect (rectClient);

	int x = point.x - rectClient.Width () / 2;
	int y = point.y - rectClient.Height () / 2;

	SetWindowPos
			(&wndTop,  x, y,
			-1, -1, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
}
//*******************************************************************************
void CBCGPHeaderItemDropWnd::Hide ()
{
	ShowWindow (SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColumnChooser

IMPLEMENT_DYNAMIC(CBCGPGridColumnChooser, CMiniFrameWnd)

CBCGPGridColumnChooser::CBCGPGridColumnChooser(CBCGPGridColumnsInfo& columns)
	: m_wndList (columns),
	m_pOwnerGrid (NULL),
	m_bIsEmpty (TRUE)
{
}

CBCGPGridColumnChooser::~CBCGPGridColumnChooser()
{
	if (m_pOwnerGrid != NULL)
	{
		ASSERT_VALID (m_pOwnerGrid);
		m_pOwnerGrid->m_pColumnChooser = NULL;
	}
}


BEGIN_MESSAGE_MAP(CBCGPGridColumnChooser, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CBCGPGridColumnChooser)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_GETMINMAXINFO()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColumnChooser message handlers

void CBCGPGridColumnChooser::OnSize(UINT nType, int cx, int cy) 
{
	CMiniFrameWnd::OnSize(nType, cx, cy);
	
	if (m_wndList.GetSafeHwnd () != NULL)
	{
		m_wndList.SetWindowPos (NULL, -1, -1, cx, cy,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//*******************************************************************************
BOOL CBCGPGridColumnChooser::Create(CBCGPGridCtrl* pOwnerGrid,
									const RECT& rect, CWnd* pParentWnd)
{
	ASSERT_VALID (pParentWnd);
	ASSERT_VALID (pOwnerGrid);

	m_pOwnerGrid = pOwnerGrid;

	CString strClassName = ::AfxRegisterWndClass (
			CS_SAVEBITS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_BTNFACE + 1), NULL);

	CBCGPLocalResource locaRes;

	CString strTitle;
	strTitle.LoadString (IDS_BCGBARRES_GRID_FIELD_CHOOSER);

	if (!CMiniFrameWnd::CreateEx (
				pParentWnd->GetExStyle() & WS_EX_LAYOUTRTL,
				strClassName, strTitle,
				WS_POPUP | WS_CAPTION | WS_SYSMENU |
				MFS_MOVEFRAME | MFS_SYNCACTIVE | MFS_BLOCKSYSMENU,
				rect,
				pParentWnd))
	{
		return FALSE;
	}

	CMenu* pSysMenu = GetSystemMenu (FALSE);

	if (pSysMenu->GetSafeHmenu () != NULL)
	{
		pSysMenu->DeleteMenu (SC_SIZE, MF_BYCOMMAND);
		pSysMenu->DeleteMenu (SC_MINIMIZE, MF_BYCOMMAND);
		pSysMenu->DeleteMenu (SC_MAXIMIZE, MF_BYCOMMAND);
		pSysMenu->DeleteMenu (SC_RESTORE, MF_BYCOMMAND);

		CString strHide;

		if (strHide.LoadString (AFX_IDS_HIDE))
		{
			pSysMenu->DeleteMenu (SC_CLOSE, MF_BYCOMMAND);
			pSysMenu->AppendMenu (MF_STRING|MF_ENABLED, SC_CLOSE, strHide);
		}
	}

	return TRUE;
}
//*******************************************************************************
int CBCGPGridColumnChooser::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	GetClientRect (&rect);
	
	m_wndList.Create (	WS_CHILD | LBS_OWNERDRAWFIXED | 
						LBS_NOINTEGRALHEIGHT | WS_VSCROLL, 
						rect, this, 1);
	CRect rectWindow;
	GetWindowRect (rectWindow);

	CPoint ptPos = rectWindow.TopLeft ();
	CPoint ptPosCurr = ptPos;

	CSize size = rectWindow.Size ();

	CRect rectScreen;

	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);

	if (GetMonitorInfo (MonitorFromPoint (ptPos, MONITOR_DEFAULTTONEAREST), &mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	if (ptPos.x < rectScreen.left)
	{
		ptPos.x = rectScreen.left;
	}
	else if (ptPos.x + size.cx > rectScreen.right)
	{
		ptPos.x = rectScreen.right - size.cx;
	}

	if (ptPos.y < rectScreen.top)
	{
		ptPos.y = rectScreen.top;
	}
	else if (ptPos.y + size.cy > rectScreen.bottom)
	{
		ptPos.y = rectScreen.bottom - size.cy;
	}

	if (ptPos != ptPosCurr)
	{
		SetWindowPos (&wndTop, ptPos.x, ptPos.y, -1, -1,
					SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColumnListBox

CBCGPGridColumnListBox::CBCGPGridColumnListBox(CBCGPGridColumnsInfo& columns)
	: m_Columns (columns)
{
}

CBCGPGridColumnListBox::~CBCGPGridColumnListBox()
{
}


BEGIN_MESSAGE_MAP(CBCGPGridColumnListBox, CListBox)
	//{{AFX_MSG_MAP(CBCGPGridColumnListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridColumnListBox message handlers

int CBCGPGridColumnListBox::CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/) 
{
	return 0;
}
//*******************************************************************************
void CBCGPGridColumnListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT (lpDIS != NULL);
	ASSERT_VALID (m_Columns.m_pWndList);

	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rectItem = lpDIS->rcItem;
	int nIndex = lpDIS->itemID;

	if (nIndex < 0)
	{
		return;
	}

	CFont* pFontOld = pDC->SelectObject (m_Columns.m_pWndList->GetFont ());

	int nColumn = (int) GetItemData (nIndex);
	ASSERT (nColumn >= 0);
	ASSERT (nColumn < m_Columns.m_pWndList->GetColumnCount ());
	CString strName = m_Columns.GetColumnName (nColumn);

	pDC->SetTextColor (globalData.clrWindowText);
	pDC->SetBkMode (TRANSPARENT);

	pDC->FillRect (rectItem, &globalData.brBtnFace);

	pDC->Draw3dRect (rectItem,
		globalData.clrBtnLight,
		globalData.clrBtnDkShadow);

	rectItem.DeflateRect (1, 1);

	pDC->Draw3dRect (rectItem,
		globalData.clrBtnHilite,
		globalData.clrBtnShadow);

	rectItem.DeflateRect (1, 1);

	CRect rectText = rectItem;
	rectText.DeflateRect (5, 0);

	pDC->DrawText (strName, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	pDC->SelectObject (pFontOld);

	if (lpDIS->itemState & ODS_SELECTED)
	{
		pDC->InvertRect (rectItem);
	}
}
//*******************************************************************************
void CBCGPGridColumnListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS) 
{
	ASSERT (lpMIS != NULL);
	ASSERT_VALID (m_Columns.m_pWndList);

	CClientDC dc (this);
	CFont* pFontOld = dc.SelectObject (m_Columns.m_pWndList->GetFont ());

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);
	lpMIS->itemHeight = tm.tmHeight + 6;

	dc.SelectObject (pFontOld);
}
//*******************************************************************************
void CBCGPGridColumnListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT_VALID (m_Columns.m_pWndList);

	CListBox::OnLButtonDown(nFlags, point);

	for (int i = 0; i < GetCount (); i++)
	{
		CRect rectItem;
		GetItemRect (i, rectItem);

		if (rectItem.PtInRect (point))
		{
			int nColumn = (int) GetItemData (i);
			ASSERT (nColumn >= 0);
			ASSERT (nColumn < m_Columns.m_pWndList->GetColumnCount ());

			MapWindowPoints (m_Columns.m_pWndList, rectItem);
			m_Columns.m_pWndList->m_ptStartDrag = CPoint (0, 0);
			m_Columns.m_pWndList->StartDragColumn (nColumn, rectItem, FALSE, TRUE);
			return;
		}
	}
}
//*******************************************************************************
int CBCGPGridColumnListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	ASSERT_VALID (m_Columns.m_pWndList);
	ASSERT_VALID (GetParent ());

	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CClientDC dc (this);
	CFont* pFontOld = dc.SelectObject (m_Columns.m_pWndList->GetFont ());

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);
	int nRowHeight = tm.tmHeight + 6;

	int nHeight = min (10, m_Columns.GetColumnCount ()) * nRowHeight;

	CString strParentCaption;
	GetParent ()->GetWindowText (strParentCaption);

	int nMaxColumnWidth = dc.GetTextExtent (strParentCaption).cx + 20;

	for (int i = 0; i < m_Columns.GetColumnCount (); i++)
	{
		CString strColumn = m_Columns.GetColumnName (i);

		nMaxColumnWidth = max (nMaxColumnWidth,
			dc.GetTextExtent (strColumn).cx);

		if (!m_Columns.GetColumnVisible (i))
		{
			int nItem = AddString (strColumn);
			SetItemData (nItem, (DWORD_PTR) i);
		}
	}

	int cxScroll = ::GetSystemMetrics (SM_CXHSCROLL);

	GetParent ()->SetWindowPos (NULL, -1, -1, nMaxColumnWidth + cxScroll + 20,
		nHeight + 20, SWP_NOZORDER | SWP_NOMOVE);

	if (GetCount () == 0)
	{
		ShowWindow (SW_HIDE);
	}
	else if ((GetStyle () & WS_VISIBLE) == 0)
	{
		ShowWindow (SW_SHOWNOACTIVATE);
	}

	dc.SelectObject (pFontOld);
	return 0;
}
//*******************************************************************************
void CBCGPGridColumnListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	ASSERT_VALID (m_Columns.m_pWndList);
	
	if (nChar == VK_ESCAPE &&
		m_Columns.m_pWndList->m_nDraggedColumn >= 0)
	{
		// Cancel dragging a column:
		CPoint pt(-1, -1);
		m_Columns.m_pWndList->StopDragColumn (pt, FALSE);

		return;
	}

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}
//*******************************************************************************
void CBCGPGridColumnChooser::OnClose() 
{
	ASSERT_VALID (m_pOwnerGrid);

	ShowWindow (SW_HIDE);
	
	m_pOwnerGrid->m_bColumnChooserVisible = FALSE;
	m_pOwnerGrid->OnHideColumnChooser ();
}
//*******************************************************************************
void CBCGPGridColumnChooser::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	ASSERT_VALID (m_pOwnerGrid);

	CMiniFrameWnd::OnWindowPosChanged(lpwndpos);
	GetWindowRect (m_pOwnerGrid->m_rectColumnChooser);
}
//*******************************************************************************
void CBCGPGridColumnChooser::UpdateList ()
{
    ASSERT_VALID (this);

	if (GetSafeHwnd () == NULL)
    {
        return;
    }

	m_wndList.ResetContent ();

	for (int i = 0; i < m_wndList.m_Columns.GetColumnCount (); i++)
	{
		if (!m_wndList.m_Columns.GetColumnVisible (i))
		{
			int nItem = m_wndList.AddString (m_wndList.m_Columns.GetColumnName (i));
            m_wndList.SetItemData (nItem, (DWORD_PTR) i);
		}
	}
	
	if (m_wndList.GetCount () == 0)
	{
		m_bIsEmpty = TRUE;
		m_wndList.ShowWindow (SW_HIDE);
	}
	else if ((m_wndList.GetStyle () & WS_VISIBLE) == 0)
	{
		m_bIsEmpty = FALSE;
		m_wndList.ShowWindow (SW_SHOWNOACTIVATE);
	}
}
//*******************************************************************************
int CBCGPGridColumnChooser::GetColumnWidth () const
{
    ASSERT_VALID (this);

	if (GetSafeHwnd () == NULL)
    {
        return 0;
    }

	int cxScroll = ::GetSystemMetrics (SM_CXHSCROLL);

	CRect rectLB;
	m_wndList.GetWindowRect (&rectLB);

	return (int) rectLB.Width () - cxScroll;
}
//*******************************************************************************
void CBCGPGridColumnChooser::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (!m_bIsEmpty)
	{
		return;
	}

	CRect rect;
	GetClientRect (rect);

	dc.FillRect (rect, &globalData.brBtnFace);
	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor (globalData.clrGrayedText);
	
	CFont* pOldFont = dc.SelectObject (&globalData.fontRegular);
	ASSERT (pOldFont != NULL);

	CString str;
	{
		CBCGPLocalResource locaRes;
		str.LoadString (IDS_BCGBARRES_GRID_NO_FIELDS_TITLE);
	}

	rect.top = max (rect.top, rect.CenterPoint ().y - globalData.GetTextHeight () / 2);
	dc.DrawText (str, rect, DT_CENTER | DT_WORDBREAK);

	dc.SelectObject (pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPGridCache object - page cache implementation for virtual mode

CBCGPGridCache::CBCGPGridCache ()
{
	m_nCachePageSize = 150;
	m_nCachePageCount = 5;
}
//*******************************************************************************
CBCGPGridCache::~CBCGPGridCache ()
{
	CleanUpCache ();
}
//*******************************************************************************
// Get item by ID
CBCGPGridRow* CBCGPGridCache::GetCachedRow (int nId)
{
	for (POSITION pos = m_lstCache.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridCachePageInfo& cpi = m_lstCache.GetNext (pos);
		int nOffset = nId - cpi.nFirst;
		if (nOffset >= 0 && nOffset < cpi.nSize)
		{
			ASSERT (cpi.nSize == cpi.pArrCachePage->GetSize ());
			cpi.bReferenced = TRUE;
			return cpi.pArrCachePage->GetAt (nOffset);
		}
	}

	return NULL;
}
//*******************************************************************************
// Save item in cache
BOOL CBCGPGridCache::SetCachedRow (int nId, CBCGPGridRow* pItem)
{
	if (nId < 0)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	int nNewPageSize = m_nCachePageSize;
	ASSERT (nNewPageSize > 0);
	int nNewPageFirst = max (0, nId - nNewPageSize);

	for (POSITION pos = m_lstCache.GetHeadPosition (); pos != NULL; )
	{
		CBCGPGridCachePageInfo& cpi = m_lstCache.GetNext (pos);
		int nOffset = nId - cpi.nFirst;
		if (nOffset >= 0 && nOffset < cpi.nSize)
		{
			ASSERT (cpi.nSize == cpi.pArrCachePage->GetSize ());
			cpi.bReferenced = TRUE;
			cpi.pArrCachePage->SetAt (nOffset, pItem);
			return TRUE;
		}
		else if (nOffset + nNewPageSize >= 0 && nOffset < cpi.nSize)
		{
			nNewPageSize = cpi.nFirst - nId;
			ASSERT (nNewPageSize > 0);
			ASSERT (nNewPageSize <= m_nCachePageSize);
		}
		else if (nOffset >= cpi.nSize && nNewPageFirst < cpi.nFirst + cpi.nSize)
		{
			nNewPageFirst = min (cpi.nFirst + cpi.nSize, nId);
			ASSERT (nNewPageFirst >= 0);
			ASSERT (nNewPageFirst <= nId);
		}
	}

	int nShiftUpperBound = 0;
	if (nNewPageSize < m_nCachePageSize && nNewPageFirst < nId)
	{
		nShiftUpperBound = min (
			m_nCachePageSize - nNewPageSize,	// try maximum page size
			nId - nNewPageFirst);				// as possible
		ASSERT (nId - nShiftUpperBound >= 0);
		ASSERT (nShiftUpperBound + nNewPageSize <= m_nCachePageSize);
	}

	CachePageArray* pArray = new CachePageArray;
	pArray->SetSize (nShiftUpperBound + nNewPageSize);
	DoAddCache (pArray, nId - nShiftUpperBound);

	CBCGPGridCachePageInfo& cpiNew = m_lstCache.GetHead ();
	cpiNew.bReferenced = TRUE;
	pArray->SetAt (nShiftUpperBound, pItem);

	DoSwapCache ();

	return TRUE;
}
//*******************************************************************************
// New cache page request
BOOL CBCGPGridCache::AddCache (int nId)
{
	return SetCachedRow (nId, NULL);
}
//*******************************************************************************
// Clean up cache
void CBCGPGridCache::CleanUpCache ()
{
	while (!m_lstCache.IsEmpty ())
	{
		CBCGPGridCachePageInfo cpi = m_lstCache.RemoveTail ();
		DoFreeCachePage (cpi);
	}
}
//*******************************************************************************
// Add cache page
void CBCGPGridCache::DoAddCache (CachePageArray* pArray, int nIdFirst)
{
	ASSERT (pArray != NULL);

	CBCGPGridCachePageInfo cpi;
	cpi.nFirst = nIdFirst;
	cpi.nSize = (int) pArray->GetSize ();
	cpi.pArrCachePage = pArray;
	cpi.bReferenced = FALSE;

	m_lstCache.AddHead (cpi);
}
//*******************************************************************************
// Free cache page
void CBCGPGridCache::DoFreeCachePage (CBCGPGridCachePageInfo& cpi)
{
	for (int i = (int) cpi.pArrCachePage->GetUpperBound (); i >= 0; i--)
	{
		delete cpi.pArrCachePage->GetAt (i);
		cpi.pArrCachePage->SetAt (i, NULL);
	}
	cpi.pArrCachePage->RemoveAll ();
	delete cpi.pArrCachePage;
}
//*******************************************************************************
// Free unreferenced cache page on exceed capacity (over cache)
void CBCGPGridCache::DoSwapCache ()
{
	ASSERT (m_nCachePageCount > 0);
	while (m_lstCache.GetCount () > m_nCachePageCount)
	{
		CBCGPGridCachePageInfo cpi = m_lstCache.RemoveTail ();
		if (cpi.bReferenced)
		{
			cpi.bReferenced = FALSE;
			m_lstCache.AddHead (cpi);
		}
		else
		{
			DoFreeCachePage (cpi);
		}
	}
}

#endif // BCGP_EXCLUDE_GRID_CTRL
