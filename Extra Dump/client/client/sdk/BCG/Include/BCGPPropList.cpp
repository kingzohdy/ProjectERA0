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
//
// BCGPPropList.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPColorBar.h"
#include "BCGPPropList.h"
#include "BCGPWorkspace.h"
#include "BCGPShellManager.h"
#include "BCGPDrawManager.h"
#include "BCGPMaskEdit.h"
#include "MenuImages.h"
#include "BCGPVisualManager.h"
#include "BCGPToolbarComboBoxButton.h"
#include "BCGPSpinButtonCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_HEADER		1
#define ID_SCROLL_VERT	2
#define STRETCH_DELTA	2

#define TEXT_MARGIN		4

extern CBCGPWorkspace* g_pWorkspace;

/////////////////////////////////////////////////////////////////////////////
// CBCGPProp

IMPLEMENT_DYNAMIC(CBCGPProp, CObject)

#define PROP_HAS_LIST	0x0001
#define PROP_HAS_BUTTON	0x0002
#define PROP_HAS_SPIN	0x0004

CString CBCGPProp::m_strFormatChar = _T("%c");
CString CBCGPProp::m_strFormatShort = _T("%d");
CString CBCGPProp::m_strFormatLong = _T("%ld");
CString CBCGPProp::m_strFormatUShort = _T("%u");
CString CBCGPProp::m_strFormatULong = _T("%u");
CString CBCGPProp::m_strFormatFloat = _T("%f");
CString CBCGPProp::m_strFormatDouble = _T("%lf");

CBCGPProp::CBCGPProp(const CString& strName, const _variant_t& varValue, 
				   LPCTSTR lpszDescr, DWORD dwData, LPCTSTR lpszEditMask,
				   LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) :
	m_strName (strName),
	m_varValue (varValue),
	m_strDescr (lpszDescr == NULL ? _T("") : lpszDescr),
	m_strEditMask (lpszEditMask == NULL ? _T("") : lpszEditMask),
	m_strEditTempl (lpszEditTemplate == NULL ? _T("") : lpszEditTemplate),
	m_strValidChars (lpszValidChars == NULL ? _T("") : lpszValidChars),
	m_dwData (dwData)
{
	m_bGroup = FALSE;
	m_bIsValueList = FALSE;

	Init ();
	SetFlags ();

	if (m_varValue.vt == VT_BOOL)
	{
		m_bAllowEdit = FALSE;
	}
}
//******************************************************************************************
CBCGPProp::CBCGPProp(const CString& strGroupName, DWORD dwData,
					 BOOL bIsValueList) :
	m_strName (strGroupName),
	m_dwData (dwData),
	m_bIsValueList (bIsValueList)
{
	m_bGroup = TRUE;

	Init ();
	SetFlags ();
}
//****************************************************************************************
void CBCGPProp::SetFlags ()
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
		m_dwFlags = PROP_HAS_LIST;
		break;

	default:
		break;
	}
}
//******************************************************************************************
void CBCGPProp::Init ()
{
	m_pWndList = NULL;
	m_bExpanded = !m_bIsValueList;
	m_bEnabled = TRUE;
	m_pParent = NULL;
	m_pWndInPlace = NULL;
	m_pWndCombo = NULL;
	m_pWndSpin = NULL;
	m_bInPlaceEdit = FALSE;
	m_bButtonIsDown = FALSE;
	m_bAllowEdit = TRUE;
	m_bNameIsTrancated = FALSE;
	m_bValueIsTrancated = FALSE;

	m_Rect.SetRectEmpty ();
	m_rectButton.SetRectEmpty ();

	m_nMinValue = 0;
	m_nMaxValue = 0;
}
//*******************************************************************************************
CBCGPProp::~CBCGPProp()
{
	while (!m_lstSubItems.IsEmpty ())
	{
		delete m_lstSubItems.RemoveHead ();
	}

	OnDestroyWindow ();
}
//******************************************************************************************
void CBCGPProp::OnDestroyWindow ()
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
BOOL CBCGPProp::HasButton () const
{
	return	(m_dwFlags & PROP_HAS_LIST) ||
			(m_dwFlags & PROP_HAS_BUTTON);
}
//****************************************************************************************
BOOL CBCGPProp::AddSubItem (CBCGPProp* pProp)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pProp);

	if (!IsGroup ())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (pProp->m_pWndList != NULL)
	{
		for (POSITION pos = pProp->m_pWndList->m_lstProps.GetHeadPosition (); pos != NULL;)
		{
			CBCGPProp* pListProp = pProp->m_pWndList->m_lstProps.GetNext (pos);
			ASSERT_VALID (pListProp);

			if (pListProp == pProp || pListProp->IsSubItem (pProp))
			{
				// Can't ad the same property twice
				ASSERT (FALSE);
				return FALSE;
			}
		}
	}

	pProp->m_pParent = this;

	m_lstSubItems.AddTail (pProp);
	pProp->m_pWndList = m_pWndList;

	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPProp::AddOption (LPCTSTR lpszOption)
{
	ASSERT_VALID (this);
	ASSERT (lpszOption != NULL);

	if (m_lstOptions.Find (lpszOption) != NULL)
	{
		return FALSE;
	}

	m_lstOptions.AddTail (lpszOption);
	m_dwFlags = PROP_HAS_LIST;

	return TRUE;
}
//****************************************************************************************
void CBCGPProp::RemoveAllOptions ()
{
	m_lstOptions.RemoveAll ();
	m_dwFlags = 0;
}
//*****************************************************************************************
int CBCGPProp::GetExpandedSubItems () const
{
	ASSERT_VALID (this);

	if (!m_bExpanded)
	{
		return 0;
	}

	int nCount = 0;

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pProp);

		nCount += pProp->GetExpandedSubItems () + 1;
	}

	return nCount;
}
//*******************************************************************************************
CBCGPProp* CBCGPProp::HitTest (CPoint point, ClickArea* pnArea)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (m_Rect.PtInRect (point))
	{
		if (pnArea != NULL)
		{
			if (IsGroup () && point.x < m_Rect.left + m_Rect.Height ())
			{
				*pnArea = ClickExpandBox;
			}
			else if (HasValueField () && 
				point.x > m_pWndList->m_rectList.left + m_pWndList->m_nLeftColumnWidth)
			{
				*pnArea = ClickValue;
			}
			else
			{
				*pnArea = ClickName;
			}
		}

		return this;
	}

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pProp);

		CBCGPProp* pHit = pProp->HitTest (point, pnArea);
		if (pHit != NULL)
		{
			return pHit;
		}
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPProp::Expand (BOOL bExpand)
{
	ASSERT_VALID (this);
	ASSERT (IsGroup ());

	if (m_bExpanded == bExpand ||
		m_lstSubItems.IsEmpty ())
	{
		return;
	}

	m_bExpanded = bExpand;

	if (m_pWndList != NULL && m_pWndList->GetSafeHwnd () != NULL)
	{
		ASSERT_VALID (m_pWndList);
		m_pWndList->AdjustLayout ();

		CRect rectRedraw = m_pWndList->m_rectList;
		rectRedraw.top = m_Rect.top;

		m_pWndList->RedrawWindow (rectRedraw);
	}
}
//*******************************************************************************************
void CBCGPProp::ExpandDeep (BOOL bExpand)
{
	ASSERT_VALID (this);

	m_bExpanded = bExpand;

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pProp);

		pProp->ExpandDeep (bExpand);
	}
}
//*******************************************************************************************
void CBCGPProp::Redraw ()
{
	ASSERT_VALID (this);

	if (m_pWndList != NULL)
	{
		ASSERT_VALID (m_pWndList);
		m_pWndList->InvalidateRect (m_Rect);

		if (m_pParent != NULL && m_pParent->m_bIsValueList)
		{
			m_pWndList->InvalidateRect (m_pParent->m_Rect);
		}

		if (m_bIsValueList)
		{
			for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
			{
				CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
				ASSERT_VALID (pProp);

				m_pWndList->InvalidateRect (pProp->m_Rect);
			}
		}

		m_pWndList->UpdateWindow ();
	}
}
//*******************************************************************************************
void CBCGPProp::EnableSpinControl (BOOL bEnable, int nMin, int nMax)
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
		m_dwFlags |= PROP_HAS_SPIN;
	}
	else
	{
		m_dwFlags &= ~PROP_HAS_SPIN;
	}
}
//*******************************************************************************************
BOOL CBCGPProp::IsSelected () const
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	return m_pWndList->m_pSel == this;
}
//******************************************************************************************
void CBCGPProp::SetName (LPCTSTR lpszName, BOOL bRedraw)
{
	ASSERT_VALID (this);

	if (lpszName == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	m_strName = lpszName;

	if (bRedraw)
	{
		Redraw ();
	}
}
//******************************************************************************************
void CBCGPProp::SetValue (const _variant_t& varValue)
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
	Redraw ();

	if (bInPlaceEdit)
	{
		ASSERT_VALID (m_pWndList);
		m_pWndList->EditItem (this);
	}
}
//*****************************************************************************************
BOOL CBCGPProp::IsParentExpanded () const
{
	ASSERT_VALID (this);

	for (CBCGPProp* pProp = m_pParent; pProp != NULL;)
	{
		ASSERT_VALID (pProp);

		if (!pProp->IsExpanded ())
		{
			return FALSE;
		}

		pProp = pProp->m_pParent;
	}

	return TRUE;
}
//******************************************************************************************
int CBCGPProp::GetHierarchyLevel () const
{
	ASSERT_VALID (this);

	int nLevel = 0;
	for (CBCGPProp* pParent = m_pParent; pParent != NULL;
		pParent = pParent->m_pParent)
	{
		nLevel++;
	}

	return nLevel;
}
//*******************************************************************************************
CBCGPProp* CBCGPProp::GetSubItem (int nIndex) const
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
void CBCGPProp::Enable (BOOL bEnable/* = TRUE*/)
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
void CBCGPProp::SetOwnerList (CBCGPPropList* pWndList)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pWndList);

	m_pWndList = pWndList;

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pProp);

		pProp->SetOwnerList (m_pWndList);
	}
}
//*******************************************************************************************
void CBCGPProp::Repos (int& y)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	CRect rectOld = m_Rect;
	
	BOOL bShowProperty = IsParentExpanded () || m_pWndList->m_bAlphabeticMode;

	if (m_pWndList->m_bAlphabeticMode && m_pParent != NULL &&
		m_pParent->m_bIsValueList && !IsParentExpanded ())
	{
		bShowProperty = FALSE;
	}

	if (bShowProperty)
	{
		int dx = m_pWndList->m_bAlphabeticMode ?
			m_pWndList->m_nRowHeight :
			GetHierarchyLevel () * m_pWndList->m_nRowHeight;

		if (m_pWndList->m_bAlphabeticMode && m_bIsValueList)
		{
			dx = 0;
		}

		m_Rect = CRect (
			m_pWndList->m_rectList.left + dx,
			y, 
			m_pWndList->m_rectList.right, 
			y + m_pWndList->m_nRowHeight);

		if (!m_rectButton.IsRectEmpty ())
		{
			m_rectButton.top = m_Rect.top + 1;
			m_rectButton.bottom = m_Rect.bottom;
		}

		y += m_pWndList->m_nRowHeight;

		CRect rectName = m_Rect;
		rectName.right = m_pWndList->m_rectList.left + m_pWndList->m_nLeftColumnWidth;

		if (IsWindow (m_pWndList->m_ToolTip.GetSafeHwnd ()))
		{
			m_pWndList->m_ToolTip.AddTool (m_pWndList, LPSTR_TEXTCALLBACK, rectName, m_pWndList->m_nTooltipsCount + 1);
			m_pWndList->m_nTooltipsCount ++;

			if (!IsGroup ())
			{
				CRect rectValue = m_Rect;
				rectValue.left = rectName.right + 1;
				m_pWndList->m_ToolTip.AddTool (m_pWndList, LPSTR_TEXTCALLBACK, rectValue, m_pWndList->m_nTooltipsCount + 1);

				m_pWndList->m_nTooltipsCount ++;
			}
		}
	}
	else
	{
		m_Rect.SetRectEmpty ();
		m_rectButton.SetRectEmpty ();
	}

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pProp);

		pProp->Repos (y);
	}

	OnPosSizeChanged (rectOld);
}
//******************************************************************************************
void CBCGPProp::AddTerminalProp (CList<CBCGPProp*, CBCGPProp*>& lstProps)
{
	ASSERT_VALID (this);

	if (!m_bGroup || m_bIsValueList)
	{
		// Insert sorted:
		BOOL bInserted = FALSE;
		for (POSITION pos = lstProps.GetHeadPosition (); !bInserted && pos != NULL;)
		{
			POSITION posSave = pos;

			CBCGPProp* pProp = lstProps.GetNext (pos);
			if (pProp->m_strName > m_strName)
			{
				lstProps.InsertBefore (posSave, this);
				bInserted = TRUE;
			}
		}

		if (!bInserted)
		{
			lstProps.AddTail (this);
		}
		return;
	}

	m_Rect.SetRectEmpty ();

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pProp);

		pProp->AddTerminalProp (lstProps);
	}
}
//****************************************************************************************
BOOL CBCGPProp::IsSubItem (CBCGPProp* pSubProp) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pSubProp);

	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
		ASSERT_VALID (pProp);

		if (pSubProp == pProp || pProp->IsSubItem (pSubProp))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//******************************************************************************************
CBCGPProp* CBCGPProp::FindSubItemByData (DWORD dwData) const
{
	ASSERT_VALID (this);
	
	for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
        ASSERT_VALID (pProp);
		
        if (pProp->m_dwData == dwData)
        {
			return pProp;
		}
		
		pProp = pProp->FindSubItemByData (dwData);
		
		if (pProp != NULL)
		{
			return pProp;
        }
    }
	
	return NULL;
}
//*****************************************************************************************
CString CBCGPProp::FormatProperty ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	_variant_t& var = m_varValue;

	CString strVal;

	if (m_bIsValueList)
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition (); pos != NULL;)
		{
			CBCGPProp* pProp = m_lstSubItems.GetNext (pos);
			ASSERT_VALID (pProp);

			strVal += pProp->FormatProperty ();

			if (pos != NULL)
			{
				strVal += m_pWndList->m_cListDelimeter;
				strVal += _T(' ');
			}
		}
		
		return strVal;
	}

	switch (var.vt)
	{
	case VT_BSTR:
		strVal = (LPCTSTR)(_bstr_t)var;
		break;

    case VT_I2:
		strVal.Format (m_strFormatShort, (short) var);
		break;

	case VT_I4:
	case VT_INT:
		strVal.Format (m_strFormatLong, (long) var);
		break;

	case VT_UI1:
		if ((BYTE) var != 0)
		{
			strVal.Format (m_strFormatChar, (TCHAR)(BYTE) var);
		}
		break;

    case VT_UI2:
		strVal.Format( m_strFormatUShort, var.uiVal);
		break;

	case VT_UINT:
	case VT_UI4:
		strVal.Format (m_strFormatULong, var.ulVal);
		break;

    case VT_R4:
		strVal.Format (m_strFormatFloat, (float) var);
		break;

    case VT_R8:
		strVal.Format (m_strFormatDouble, (double) var);
		break;

    case VT_BOOL:
		{
			bool bVal = (bool) var;
			strVal = bVal ? m_pWndList->m_strTrue : m_pWndList->m_strFalse;
		}
		break;

	default:
		// Unsupported type
		strVal = _T("*** error ***");
	}

	return strVal;
}
//****************************************************************************************
void CBCGPProp::OnDrawName (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);

	COLORREF clrTextOld = (COLORREF)-1;

	if (IsSelected () && (!m_pWndList->m_bVSDotNetLook || !IsGroup () ||  m_bIsValueList))
	{
		CRect rectFill = rect;
		rectFill.top++;

		if (!m_pWndList->m_bFocused)
		{
			clrTextOld = pDC->SetTextColor (globalData.clrBtnText);

			pDC->FillRect (rectFill, 
				m_pWndList->m_bControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);
		}
		else
		{
			clrTextOld = pDC->SetTextColor (globalData.clrTextHilite);
			pDC->FillRect (rectFill, &globalData.brHilite);
		}
	}

	if (m_pWndList->m_bVSDotNetLook && IsGroup () && !m_bIsValueList)
	{
		if (m_pWndList->m_clrGroupText != (COLORREF)-1)
		{
			clrTextOld = pDC->SetTextColor (m_pWndList->m_clrGroupText);
		}
		else
		{
			clrTextOld = pDC->SetTextColor (
				m_pWndList->m_bControlBarColors ? globalData.clrBarShadow : globalData.clrBtnShadow);
		}
	}

	rect.DeflateRect (TEXT_MARGIN, 0);

	if (m_pParent != NULL && m_pParent->m_bIsValueList)
	{
		rect.left += rect.Height ();
	}

	int nTextHeight = pDC->DrawText (m_strName, rect, 
		DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	m_bNameIsTrancated = pDC->GetTextExtent (m_strName).cx > rect.Width ();

	if (IsSelected () && m_pWndList->m_bVSDotNetLook && IsGroup () && !m_bIsValueList)
	{
		CRect rectFocus = rect;
		rectFocus.top = rectFocus.CenterPoint ().y - nTextHeight / 2;
		rectFocus.bottom = rectFocus.top + nTextHeight;
		rectFocus.right = 
			min (rect.right, rectFocus.left + pDC->GetTextExtent (m_strName).cx);
		rectFocus.InflateRect (2, 0);

		COLORREF clrShadow = m_pWndList->m_bControlBarColors ?
			globalData.clrBarShadow : globalData.clrBtnShadow;

		pDC->Draw3dRect (rectFocus, clrShadow, clrShadow);
	}

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}
}
//******************************************************************************************
void CBCGPProp::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if ((IsGroup () && !m_bIsValueList) || !HasValueField ())
	{
		return;
	}

	CString strVal = FormatProperty ();

	rect.DeflateRect (TEXT_MARGIN, 0);

	pDC->DrawText (strVal, rect, 
		DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	m_bValueIsTrancated = pDC->GetTextExtent (strVal).cx > rect.Width ();
}
//******************************************************************************************
void CBCGPProp::OnDrawButton (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);

	CBCGPToolbarComboBoxButton button;

	CBCGPVisualManager::BCGBUTTON_STATE state = 
					m_bButtonIsDown ?
					CBCGPVisualManager::ButtonsIsPressed :
					CBCGPVisualManager::ButtonsIsRegular;

	CBCGPVisualManager::GetInstance ()->OnFillButtonInterior (pDC,
		&button, rect, state);

	pDC->FillRect (rect, m_pWndList->m_bControlBarColors ?
		&globalData.brBarFace : &globalData.brBtnFace);

	if (m_dwFlags & PROP_HAS_LIST)
	{
		CBCGPVisualManager::GetInstance ()->OnDrawComboDropButton (pDC,
			rect, !m_bEnabled, m_bButtonIsDown, m_bButtonIsDown, &button);
		return;
	}

	CString str = _T("...");
	pDC->DrawText (str, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC, &button, rect, state);
}
//****************************************************************************************
void CBCGPProp::OnDrawExpandBox (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);
	ASSERT (IsGroup ());

	CPoint ptCenter = rect.CenterPoint ();

	int nBoxSize = min (9, rect.Width ());

	rect = CRect (ptCenter, CSize (1, 1));
	rect.InflateRect (nBoxSize / 2, nBoxSize / 2);

	if (m_bIsValueList)
	{
		if (m_pWndList->m_brBackground.GetSafeHandle () != NULL)
		{
			pDC->FillRect (rect, &m_pWndList->m_brBackground);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brWindow);
		}
	}

	COLORREF clrShadow = m_pWndList->m_bControlBarColors ? 
		globalData.clrBarShadow : globalData.clrBtnShadow;
	COLORREF clrText = m_pWndList->m_bControlBarColors ? 
		globalData.clrBarText : globalData.clrBtnText;

	CBCGPVisualManager::GetInstance ()->OnDrawExpandingBox (pDC, rect, 
		m_bExpanded && !m_lstSubItems.IsEmpty (),
		m_pWndList->m_bVSDotNetLook ? clrText : clrShadow);
}
//******************************************************************************************
BOOL CBCGPProp::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT_VALID (m_pWndList);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	CString strText;
	m_pWndInPlace->GetWindowText (strText);

	BOOL bRes = FALSE;
	BOOL bIsChanged = FormatProperty () != strText;

	if (m_bIsValueList)
	{
		CString strDelimeter = m_pWndList->m_cListDelimeter;

		for (int i = 0; !strText.IsEmpty () && i < GetSubItemsCount (); i++)
		{
			CString strItem = strText.SpanExcluding (strDelimeter);

			if (strItem.GetLength () + 1 > strText.GetLength ())
			{
				strText.Empty ();
			}
			else
			{
				strText = strText.Mid (strItem.GetLength () + 1);
			}

			strItem.TrimLeft ();
			strItem.TrimRight ();

			CBCGPProp* pSubItem = GetSubItem (i);
			ASSERT_VALID (pSubItem);

			pSubItem->TextToVar (strItem);
		}

		bRes = TRUE;
	}
	else
	{
		bRes = TextToVar (strText);
	}

	if (bRes && bIsChanged)
	{
		m_pWndList->OnPropertyChanged (this);
	}

	return bRes;
}
//******************************************************************************************
BOOL CBCGPProp::TextToVar (const CString& strText)
{
	switch (m_varValue.vt)
	{
	case VT_BSTR:
		m_varValue = (LPCTSTR) strText;
		return TRUE;

	case VT_UI1:
		m_varValue = (BYTE) strText[0];
		return TRUE;

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
			float fVal;
			_stscanf (strText, m_strFormatFloat, &fVal);
			m_varValue = fVal;
		}
		return TRUE;

	case VT_R8:
		{
			double dblVal;
			_stscanf (strText, m_strFormatDouble, &dblVal);
			m_varValue = dblVal;
		}
		return TRUE;

	case VT_BOOL:
		m_varValue = (bool) (strText == m_pWndList->m_strTrue);
		return TRUE;
	}

	return FALSE;
}
//*****************************************************************************
BOOL CBCGPProp::OnEdit (LPPOINT /*lptClick*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (!HasValueField ())
	{
		return FALSE;
	}

	m_pWndInPlace = NULL;

	CRect rectEdit = m_Rect;
	rectEdit.DeflateRect (0, 2);

	rectEdit.left = m_pWndList->m_rectList.left + m_pWndList->m_nLeftColumnWidth + TEXT_MARGIN + 1;

	if (HasButton ())
	{
		m_rectButton = m_Rect;
		m_rectButton.left = m_rectButton.right - m_rectButton.Height () + 3;
		m_rectButton.top ++;

		rectEdit.right = m_rectButton.left;
	}

	CRect rectSpin;

	if (m_dwFlags & PROP_HAS_SPIN)
	{
		rectSpin = m_Rect;
		rectSpin.right = rectEdit.right;
		rectSpin.left = rectSpin.right - rectSpin.Height ();
		rectSpin.top ++;
		rectEdit.right = rectSpin.left;
	}

	BOOL bDefaultFormat = FALSE;
	m_pWndInPlace = CreateInPlaceEdit (rectEdit, bDefaultFormat);

	if (m_pWndInPlace != NULL)
	{
		if (bDefaultFormat)
		{
			m_pWndInPlace->SetWindowText (FormatProperty ());
		}

		if (m_dwFlags & PROP_HAS_LIST)
		{
			CRect rectCombo = m_Rect;
			rectCombo.left = rectEdit.left - 4;

			m_pWndCombo = CreateCombo (m_pWndList, rectCombo);
			ASSERT_VALID (m_pWndCombo);

			m_pWndCombo->SetFont (m_pWndList->GetFont ());

			//-----------------------------------------------------------------------
			// Synchronize bottom edge of the combobox with the property bottom edge:
			//-----------------------------------------------------------------------
			m_pWndCombo->GetWindowRect (rectCombo);
			m_pWndList->ScreenToClient (&rectCombo);

			int dy = rectCombo.Height () - m_Rect.Height ();

			m_pWndCombo->SetWindowPos (NULL, rectCombo.left,
				rectCombo.top - dy + 1, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

			if (m_varValue.vt == VT_BOOL)
			{
				m_lstOptions.AddTail (m_pWndList->m_strTrue);
				m_lstOptions.AddTail (m_pWndList->m_strFalse);
			}

			for (POSITION pos = m_lstOptions.GetHeadPosition (); pos != NULL;)
			{
				m_pWndCombo->AddString (m_lstOptions.GetNext (pos));
			}
		}

		if (m_dwFlags & PROP_HAS_SPIN)
		{
			m_pWndSpin = CreateSpinControl (rectSpin);
		}

		m_pWndInPlace->SetFont (m_pWndList->GetFont ());
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
CWnd* CBCGPProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
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
	case VT_BOOL:
		break;

	default:
		if (!m_bIsValueList)
		{
			return NULL;
		}
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

		pWndEdit = pWndEditMask;
	}
	else
	{
		pWndEdit = new CEdit;
	}

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL;

	if (!m_bEnabled || !m_bAllowEdit)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->Create (dwStyle, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);

	bDefaultFormat = TRUE;
	return pWndEdit;
}
//*****************************************************************************
CSpinButtonCtrl* CBCGPProp::CreateSpinControl (CRect rectSpin)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	CSpinButtonCtrl* pWndSpin = new CBCGPSpinButtonCtrl;

	if (!pWndSpin->Create (
		WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_SETBUDDYINT | UDS_NOTHOUSANDS,
		rectSpin, m_pWndList, BCGPROPLIST_ID_INPLACE))
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
BOOL CBCGPProp::OnEndEdit ()
{
	ASSERT_VALID (this);

	m_bInPlaceEdit = FALSE;
	OnDestroyWindow ();
	return TRUE;
}
//*****************************************************************************************
CComboBox* CBCGPProp::CreateCombo (CWnd* pWndParent, CRect rect)
{
	ASSERT_VALID (this);

	rect.bottom = rect.top + 400;

	CComboBox* pWndCombo = new CComboBox;
	if (!pWndCombo->Create (WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL, 
		rect, pWndParent, BCGPROPLIST_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}

	return pWndCombo;
}
//****************************************************************************************
void CBCGPProp::OnClickButton (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

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
BOOL CBCGPProp::OnDblClick (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

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

	if (m_dwFlags & PROP_HAS_BUTTON)
	{
		CWaitCursor wait;

		CString strPrevVal = FormatProperty ();

		OnClickButton (CPoint (-1, -1));

		if (strPrevVal != FormatProperty ())
		{
			m_pWndList->OnPropertyChanged (this);
		}

		return TRUE;
	}

	return FALSE;
}
//*****************************************************************************************
void CBCGPProp::OnSelectCombo ()
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
void CBCGPProp::OnCloseCombo ()
{
	ASSERT_VALID (this);

	m_bButtonIsDown = FALSE;
	Redraw ();

	ASSERT_VALID (m_pWndInPlace);
	m_pWndInPlace->SetFocus ();
}
//****************************************************************************************
BOOL CBCGPProp::OnSetCursor () const
{
	if (m_bInPlaceEdit)
	{
		return FALSE;
	}

	if (m_bIsValueList)
	{
		SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_IBEAM));
		return TRUE;
	}

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

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPProp::PushChar (UINT nChar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);
	ASSERT (m_pWndList->m_pSel == this);
	ASSERT_VALID (m_pWndInPlace);

	if (m_bIsValueList)
	{
		if (m_bEnabled && m_bAllowEdit &&
			nChar != _T('+') && nChar != _T('-'))
		{
			m_pWndInPlace->SetWindowText (_T(""));
			m_pWndInPlace->SendMessage (WM_CHAR, (WPARAM) nChar);
			return TRUE;
		}
	}

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
			CString strChar = (TCHAR) nChar;
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
	ReleaseCapture ();
	return FALSE;
}
//*******************************************************************************************
CString CBCGPProp::GetNameTooltip ()
{
	ASSERT_VALID (this);
	return m_bNameIsTrancated ? m_strName : _T("");
}
//*******************************************************************************************
CString CBCGPProp::GetValueTooltip ()
{
	ASSERT_VALID (this);
	return m_bValueIsTrancated ? FormatProperty () : _T("");
}
//*******************************************************************************************
HBRUSH CBCGPProp::OnCtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	if (m_pWndList->m_clrText != (COLORREF)-1)
	{
		pDC->SetTextColor (m_pWndList->m_clrText);
	}

	if (m_pWndList->m_brBackground.GetSafeHandle () != NULL)
	{
		pDC->SetBkColor (m_pWndList->m_clrBackground);
		return (HBRUSH) m_pWndList->m_brBackground.GetSafeHandle ();
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

/////////////////////////////////////////////////////////////////////////////
// CBCGPColorProp object

IMPLEMENT_DYNAMIC(CBCGPColorProp, CBCGPProp)

CBCGPColorProp::CBCGPColorProp(const CString& strName, const COLORREF& color, 
							   CPalette* pPalette, LPCTSTR lpszDescr, DWORD dwData) :
	CBCGPProp (strName, _variant_t(), lpszDescr, dwData),
	m_Color (color)
{
	CBCGPColorBar::InitColors (pPalette, m_Colors);

	m_varValue = (LONG) color ;
	m_dwFlags = PROP_HAS_LIST;

	m_pPopup = NULL;
	m_bStdColorDlg = FALSE;
	m_ColorAutomatic = RGB (0, 0, 0);
	m_nColumnsNumber = 5;
}
//*****************************************************************************************
CBCGPColorProp::~CBCGPColorProp()
{
}
//*******************************************************************************************
void CBCGPColorProp::OnDrawValue (CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height ();
	CBCGPProp::OnDrawValue (pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height ();
	rectColor.DeflateRect (1, 1);
	rectColor.top++;
	rectColor.left++;

	CBrush br (m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color);
	pDC->FillRect (rectColor, &br);
	pDC->Draw3dRect (rectColor, 0, 0);
}
//****************************************************************************************
void CBCGPColorProp::OnClickButton (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	m_bButtonIsDown = TRUE;
	Redraw ();

	CList<COLORREF,COLORREF> lstDocColors;

	m_pPopup = new CColorPopup (NULL, m_Colors, m_Color,
		NULL, NULL, NULL, lstDocColors,
		m_nColumnsNumber, m_ColorAutomatic);
	m_pPopup->SetPropList (m_pWndList);

	if (!m_strOtherColor.IsEmpty ())	// Other color button
	{
		m_pPopup->m_wndColorBar.EnableOtherButton (m_strOtherColor, !m_bStdColorDlg);
	}

	if (!m_strAutoColor.IsEmpty ())	// Automatic color button
	{
		m_pPopup->m_wndColorBar.EnableAutomaticButton (m_strAutoColor, m_ColorAutomatic);
	}

	CPoint pt (
		m_pWndList->m_rectList.left + m_pWndList->m_nLeftColumnWidth + 1, 
		m_rectButton.bottom + 1);
	m_pWndList->ClientToScreen (&pt);

	if (!m_pPopup->Create (m_pWndList, pt.x, pt.y, NULL, FALSE))
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
BOOL CBCGPColorProp::OnEdit (LPPOINT /*lptClick*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);

	m_pWndInPlace = NULL;

	CRect rectEdit = m_Rect;
	rectEdit.DeflateRect (0, 2);

	rectEdit.left = m_pWndList->m_rectList.left + 
		m_pWndList->m_nLeftColumnWidth + TEXT_MARGIN + m_Rect.Height () + 1;

	m_rectButton = m_Rect;
	m_rectButton.left = m_rectButton.right - m_rectButton.Height () + 3;
	m_rectButton.top ++;

	rectEdit.right = m_rectButton.left;

	CEdit* pWndEdit = new CEdit;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;

	if (!m_bEnabled)
	{
		dwStyle |= ES_READONLY;
	}

	pWndEdit->Create (dwStyle, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
	m_pWndInPlace = pWndEdit;

	m_pWndInPlace->SetWindowText (FormatProperty ());

	m_pWndInPlace->SetFont (m_pWndList->GetFont ());
	m_pWndInPlace->SetFocus ();

	m_bInPlaceEdit = TRUE;
	return TRUE;
}
//****************************************************************************************
CString CBCGPColorProp::FormatProperty ()
{
	ASSERT_VALID (this);

	CString str;
	str.Format (_T("%06x"), m_Color);

	return str;
}
//******************************************************************************************
void CBCGPColorProp::SetColor (COLORREF color)
{
	ASSERT_VALID (this);

	m_Color = color;
	m_varValue = (LONG) color;

	if (::IsWindow (m_pWndList->GetSafeHwnd())) 
	{
		CRect rect = m_Rect;
		rect.DeflateRect (0, 1);

		m_pWndList->InvalidateRect (rect);
		m_pWndList->UpdateWindow ();
	}

	if (m_pWndInPlace != NULL)
	{
		ASSERT_VALID (m_pWndInPlace);
		m_pWndInPlace->SetWindowText (FormatProperty ());
	}
}
//********************************************************************************
void CBCGPColorProp::SetColumnsNumber (int nColumnsNumber)
{
	ASSERT_VALID (this);
	ASSERT (nColumnsNumber > 0);

	m_nColumnsNumber = nColumnsNumber;
}
//*************************************************************************************
void CBCGPColorProp::EnableAutomaticButton (LPCTSTR lpszLabel, COLORREF colorAutomatic, BOOL bEnable)
{
	ASSERT_VALID (this);

	m_ColorAutomatic = colorAutomatic;
	m_strAutoColor = (!bEnable || lpszLabel == NULL) ? _T("") : lpszLabel;
}
//*************************************************************************************
void CBCGPColorProp::EnableOtherButton (LPCTSTR lpszLabel, BOOL bAltColorDlg, BOOL bEnable)
{
	ASSERT_VALID (this);

	m_bStdColorDlg = !bAltColorDlg;
	m_strOtherColor = (!bEnable || lpszLabel == NULL) ? _T("") : lpszLabel;
}
//*****************************************************************************************
BOOL CBCGPColorProp::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	CString strText;
	m_pWndInPlace->GetWindowText (strText);

	COLORREF colorCurr = m_Color;
	_stscanf (strText, _T("%x"), &m_Color);

	if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged (this);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPFileProp object

IMPLEMENT_DYNAMIC(CBCGPFileProp, CBCGPProp)

CBCGPFileProp::CBCGPFileProp(const CString& strName, const CString& strFolderName, DWORD dwData, LPCTSTR lpszDescr) :
	CBCGPProp (strName, _variant_t((LPCTSTR)strFolderName), lpszDescr, dwData),
	m_bIsFolder (TRUE)
{
	m_dwFlags = PROP_HAS_BUTTON;
}
//*****************************************************************************************
CBCGPFileProp::CBCGPFileProp (	const CString& strName, 
								BOOL bOpenFileDialog,
								const CString& strFileName, 
								LPCTSTR lpszDefExt,
								DWORD dwFileFlags, 
								LPCTSTR lpszFilter,
								LPCTSTR lpszDescr,
								DWORD dwData) :
	CBCGPProp (strName, _variant_t((LPCTSTR)strFileName), lpszDescr, dwData),
	m_bIsFolder (FALSE),
	m_bOpenFileDialog (bOpenFileDialog),
	m_dwFileOpenFlags (dwFileFlags)
{
	m_dwFlags = PROP_HAS_BUTTON;
	m_strDefExt = lpszDefExt == NULL ? _T("") : lpszDefExt;
	m_strFilter = lpszFilter == NULL ? _T("") : lpszFilter;
}
//*****************************************************************************************
CBCGPFileProp::~CBCGPFileProp()
{
}
//****************************************************************************************
void CBCGPFileProp::OnClickButton (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	m_bButtonIsDown = TRUE;
	Redraw ();

	CString strPath = (LPCTSTR)(_bstr_t)m_varValue;
	BOOL bUpdate = FALSE;

	if (m_bIsFolder)
	{
		if (g_pShellManager == NULL)
		{
			if (g_pWorkspace != NULL)
			{
				g_pWorkspace->InitShellManager ();
			}
		}

		if (g_pShellManager == NULL)
		{
			ASSERT (FALSE);
		}
		else
		{
			bUpdate = g_pShellManager->BrowseForFolder (strPath, m_pWndList, strPath);
		}
	}
	else
	{
		CFileDialog dlg (m_bOpenFileDialog, m_strDefExt, strPath, m_dwFileOpenFlags, m_strFilter,
			m_pWndList);
		if (dlg.DoModal () == IDOK)
		{
			bUpdate = TRUE;
			strPath = dlg.GetPathName ();
		}
	}

	if (bUpdate)
	{
		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText (strPath);
		}

		m_varValue = (LPCTSTR) strPath;
	}

	m_bButtonIsDown = FALSE;
	Redraw ();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus ();
	}
	else
	{
		m_pWndList->SetFocus ();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPDateTimeProp object

IMPLEMENT_DYNAMIC(CBCGPDateTimeProp, CBCGPProp)

CBCGPDateTimeProp::CBCGPDateTimeProp(const CString& strName, const COleDateTime& date, 
		LPCTSTR lpszDescr/* = NULL*/, DWORD dwData/* = 0*/,
		UINT nFlags/* = CBCGPDateTimeCtrl::DTM_DATE | CBCGPDateTimeCtrl::DTM_TIME | CBCGPDateTimeCtrl::DTM_DROPCALENDAR*/) :
	CBCGPProp (strName, _variant_t(date, VT_DATE), lpszDescr, dwData)
{
	m_nFlags = nFlags;
}
//*****************************************************************************************
CBCGPDateTimeProp::~CBCGPDateTimeProp()
{
}
//****************************************************************************************
void CBCGPDateTimeProp::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (m_pWndList);

	rect.DeflateRect (0, 2);

	rect.left = m_pWndList->GetListRect ().left + 
				m_pWndList->GetPropertyColumnWidth () + TEXT_MARGIN + 1;

	DRAWITEMSTRUCT dis;
	memset (&dis, 0, sizeof (DRAWITEMSTRUCT));

	dis.CtlType = ODT_BUTTON;
	dis.hDC = pDC->GetSafeHdc ();
	dis.rcItem = rect;

	m_wndDateTime.m_bDrawDateTimeOnly = !IsSelected () || !m_bEnabled;
	
	COLORREF clrTextOld = pDC->GetTextColor ();

	m_wndDateTime.SetTextColor (m_bEnabled ? m_pWndList->m_clrText : globalData.clrGrayedText, FALSE);

	if (m_pWndList->m_clrBackground != NULL)
	{
		m_wndDateTime.SetBackgroundColor (m_pWndList->m_clrBackground, FALSE);
	}

	m_wndDateTime.DrawItem (&dis);
	pDC->SetTextColor (clrTextOld);
}
//*****************************************************************************
CWnd* CBCGPDateTimeProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	ASSERT_VALID (m_pWndList);

	CBCGPDateTimeCtrl* pDateTime = new CBCGPDateTimeCtrl;
	ASSERT_VALID (pDateTime);

	pDateTime->SetAutoResize (FALSE);

	rectEdit.bottom++;

	pDateTime->Create (_T(""), WS_CHILD | WS_VISIBLE, rectEdit, 
		m_pWndList, BCGPROPLIST_ID_INPLACE);
	pDateTime->SetFont (m_pWndList->GetFont ());

	SetState (*pDateTime);
	pDateTime->SetDate (GetDate ());

	pDateTime->SetTextColor (m_pWndList->m_clrText, FALSE);

	if (m_pWndList->m_clrBackground != NULL)
	{
		pDateTime->SetBackgroundColor (m_pWndList->m_clrBackground, FALSE);
	}

	bDefaultFormat = FALSE;

	return pDateTime;
}
//*******************************************************************************
void CBCGPDateTimeProp::OnPosSizeChanged (CRect /*rectOld*/)
{
	ASSERT_VALID (m_pWndList);

	CRect rectEdit = m_Rect;
	rectEdit.DeflateRect (0, 2);

	rectEdit.left = m_pWndList->GetListRect ().left + 
					m_pWndList->GetPropertyColumnWidth () + TEXT_MARGIN + 1;
	rectEdit.bottom++;

	if (m_wndDateTime.GetSafeHwnd () == NULL)
	{
		m_wndDateTime.SetAutoResize (FALSE);
		m_wndDateTime.Create (_T(""), WS_CHILD, rectEdit,
								m_pWndList, (UINT)-1);

		m_wndDateTime.SetFont (m_pWndList->GetFont (), FALSE);
		SetState (m_wndDateTime);
		m_wndDateTime.SetDate (GetDate ());
	}
	else
	{
		m_wndDateTime.SetFont (m_pWndList->GetFont (), FALSE);

		m_wndDateTime.SetWindowPos (NULL, rectEdit.left, rectEdit.top,
			rectEdit.Width (), rectEdit.Height (),
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	m_wndDateTime.AdjustControl (rectEdit);
}
//******************************************************************************************
BOOL CBCGPDateTimeProp::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT_VALID (m_pWndList);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	CBCGPDateTimeCtrl* pDateTime = DYNAMIC_DOWNCAST (CBCGPDateTimeCtrl, m_pWndInPlace);
	ASSERT_VALID (pDateTime);

	COleDateTime dateOld = GetDate ();
	COleDateTime dateNew = pDateTime->GetDate ();

	m_varValue = _variant_t (dateNew, VT_DATE);
	m_wndDateTime.SetDate (dateNew);

	if (dateOld != dateNew)
	{
		m_pWndList->OnPropertyChanged (this);
	}

	return TRUE;
}
//********************************************************************************
void CBCGPDateTimeProp::SetDate (COleDateTime date)
{
	ASSERT_VALID (this);
	SetValue (_variant_t (date, VT_DATE));
}
//*********************************************************************************
void CBCGPDateTimeProp::SetState (CBCGPDateTimeCtrl& wnd)
{
	ASSERT (wnd.GetSafeHwnd () != NULL);

	UINT stateFlags = 0;

	if (m_nFlags & (CBCGPDateTimeCtrl::DTM_DATE))
	{
		stateFlags |= (CBCGPDateTimeCtrl::DTM_DATE | CBCGPDateTimeCtrl::DTM_DROPCALENDAR);
	}
	
	if (m_nFlags & (CBCGPDateTimeCtrl::DTM_TIME))
	{
		stateFlags |= (CBCGPDateTimeCtrl::DTM_TIME);
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
//********************************************************************************
void CBCGPDateTimeProp::OnSetSelection (CBCGPProp* /*pOldSel*/)
{
	Redraw ();
}
//********************************************************************************
void CBCGPDateTimeProp::OnKillSelection (CBCGPProp* /*pNewSel*/)
{
	Redraw ();
}
//********************************************************************************
BOOL CBCGPDateTimeProp::PushChar (UINT nChar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);
	ASSERT (m_pWndList->GetCurSel () == this);
	ASSERT_VALID (m_pWndInPlace);

	if (m_bEnabled && m_bAllowEdit)
	{
		CString str = (TCHAR) nChar;
		str.MakeUpper ();

		m_pWndInPlace->SendMessage (WM_KEYDOWN, (WPARAM) str [0]);
		return TRUE;
	}

	OnEndEdit ();
	ReleaseCapture ();
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPFontProp object

IMPLEMENT_DYNAMIC(CBCGPFontProp, CBCGPProp)

CBCGPFontProp::CBCGPFontProp(const CString& strName, LOGFONT& lf, DWORD dwFontDialogFlags, LPCTSTR lpszDescr, DWORD dwData, COLORREF color) :
	CBCGPProp (strName, _variant_t((LPCTSTR)lf.lfFaceName), lpszDescr, dwData),
	m_dwFontDialogFlags (dwFontDialogFlags)
{
	m_dwFlags = PROP_HAS_BUTTON;
	m_lf = lf;
	m_bAllowEdit = FALSE;
	m_Color = color;
}
//*****************************************************************************************
CBCGPFontProp::~CBCGPFontProp()
{
}
//****************************************************************************************
void CBCGPFontProp::OnClickButton (CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndList);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	LOGFONT lfPrev = m_lf;
	COLORREF nColorPrev = m_Color;

	m_bButtonIsDown = TRUE;
	Redraw ();

	CFontDialog dlg (&m_lf, m_dwFontDialogFlags, NULL, m_pWndList);

	if (m_Color != (COLORREF)-1)
	{
		dlg.m_cf.rgbColors = m_Color;
	}

	if (dlg.DoModal () == IDOK)
	{
		dlg.GetCurrentFont (&m_lf);
		m_Color = dlg.GetColor ();

		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText (FormatProperty ());

			if (memcmp (&lfPrev, &m_lf, sizeof (lfPrev) - sizeof (lfPrev.lfFaceName)) ||
				_tcscmp( lfPrev.lfFaceName, m_lf.lfFaceName) ||
				nColorPrev != m_Color)
			{
				m_pWndList->OnPropertyChanged (this);
			}
		}
		else
		{
			m_varValue = (LPCTSTR) FormatProperty ();
		}
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus ();
	}
	else
	{
		m_pWndList->SetFocus ();
	}

	m_bButtonIsDown = FALSE;
	Redraw ();
}
//*****************************************************************************
CString CBCGPFontProp::FormatProperty ()
{
	CString str;
	CWindowDC dc (m_pWndList);

	int nLogY = dc.GetDeviceCaps (LOGPIXELSY);
	if (nLogY != 0)
	{
		str.Format( _T("%s(%i)"), m_lf.lfFaceName, 
			MulDiv (72, -m_lf.lfHeight, nLogY));
	}
	else
	{
		str = m_lf.lfFaceName;
	}

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropList

IMPLEMENT_DYNAMIC(CBCGPPropList, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropList notification messages:

UINT BCGM_PROPERTY_CHANGED = ::RegisterWindowMessage (_T("BCGM_PROPERTYCHANGED"));

CBCGPPropList::CBCGPPropList()
{
	m_hFont = NULL;
	m_bHeaderCtrl = TRUE;
	m_bDescriptionArea = FALSE;
	m_nDescrHeight = -1;
	m_bAlphabeticMode = FALSE;
	m_bVSDotNetLook = FALSE;
	m_rectList.SetRectEmpty ();
	m_nLeftColumnWidth = 0;
	m_rectTrackHeader.SetRectEmpty ();
	m_rectTrackDescr.SetRectEmpty ();
	m_nRowHeight = 0;
	m_nHeaderHeight = 0;
	m_nVertScrollOffset = 0;
	m_nVertScrollTotal = 0;
	m_nVertScrollPage = 0;
	m_pSel = NULL;
	m_bFocused = FALSE;
	m_nTooltipsCount = 0;
	m_bTracking = FALSE;
	m_bTrackingDescr = FALSE;

	m_strTrue = _T("True");
	m_strFalse = _T("False");

	m_cListDelimeter = _T(',');

	m_bControlBarColors = FALSE;
	m_bGroupNameFullWidth = TRUE;
	m_bShowDragContext = TRUE;

	m_clrBackground = (COLORREF)-1;
	m_clrText = (COLORREF)-1;
	m_clrGroupBackground = (COLORREF)-1;
	m_clrGroupText = (COLORREF)-1;
	m_clrDescriptionBackground = (COLORREF)-1;
	m_clrDescriptionText = (COLORREF)-1;
	m_clrLine = (COLORREF)-1;
}

CBCGPPropList::~CBCGPPropList()
{
}

BEGIN_MESSAGE_MAP(CBCGPPropList, CWnd)
	//{{AFX_MSG_MAP(CBCGPPropList)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CANCELMODE()
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_VSCROLL()
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
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_NOTIFY(HDN_ITEMCHANGED, ID_HEADER, OnHeaderItemChanged)
	ON_NOTIFY(HDN_TRACK, ID_HEADER, OnHeaderTrack)
	ON_NOTIFY(HDN_ENDTRACK, ID_HEADER, OnHeaderEndTrack)
	ON_WM_STYLECHANGED()
	ON_CBN_SELENDOK(BCGPROPLIST_ID_INPLACE, OnSelectCombo)
	ON_CBN_CLOSEUP(BCGPROPLIST_ID_INPLACE, OnCloseCombo)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnNeedTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropList message handlers

void CBCGPPropList::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState ();
	if (pThreadState->m_pWndInit == NULL)
	{
		Init ();
	}
}
//******************************************************************************************
int CBCGPPropList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init ();
	return 0;
}
//******************************************************************************************
void CBCGPPropList::Init ()
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

	InitHeader ();

	HDITEM hdItem;
	hdItem.mask = HDI_TEXT | HDI_FORMAT;
	hdItem.fmt = HDF_LEFT;
	hdItem.pszText = _T("Property");
	hdItem.cchTextMax = 100;

	GetHeaderCtrl ().InsertItem (0, &hdItem);

	hdItem.pszText = _T("Value");
	hdItem.cchTextMax = 100;

	GetHeaderCtrl ().InsertItem (1, &hdItem);

	m_wndScrollVert.Create (WS_CHILD | WS_VISIBLE | SBS_VERT, rectDummy, this, ID_SCROLL_VERT);

	m_ToolTip.Create (this, TTS_ALWAYSTIP);
	m_ToolTip.Activate (TRUE);
	if(globalData.m_nMaxToolTipWidth != -1)
	{
		m_ToolTip.SetMaxTipWidth(globalData.m_nMaxToolTipWidth);
	}
	m_ToolTip.BringWindowToTop ();

	CWnd* pWndParent = GetParent ();
	m_bControlBarColors = pWndParent == NULL ||
		!pWndParent->IsKindOf (RUNTIME_CLASS (CDialog));

	AdjustLayout ();
	CreateBoldFont ();
}
//******************************************************************************************
void CBCGPPropList::InitHeader ()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	m_wndHeader.Create (WS_CHILD | WS_VISIBLE | HDS_HORZ, rectDummy, this, ID_HEADER);
}
//*****************************************************************************************
void CBCGPPropList::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CClientDC dc (this);
	HFONT hfontOld = SetCurrFont (&dc);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);
	m_nRowHeight = tm.tmHeight + 4;

	m_nHeaderHeight = 0;

	CRect rectClient;
	GetClientRect (rectClient);

	if (m_bHeaderCtrl)
	{
		m_nHeaderHeight = m_nRowHeight + 4;

		GetHeaderCtrl ().SendMessage (WM_SETFONT,
			(WPARAM) (m_hFont != NULL ? m_hFont : ::GetStockObject (DEFAULT_GUI_FONT)));

		GetHeaderCtrl ().SetWindowPos (NULL, rectClient.left, rectClient.top, 
			rectClient.Width (), m_nHeaderHeight,
			SWP_NOZORDER | SWP_NOACTIVATE);

		HDITEM hdItem;
		hdItem.mask = HDI_WIDTH ;
		hdItem.cxy = m_nLeftColumnWidth + 2;

		GetHeaderCtrl ().SetItem (0, &hdItem);

		hdItem.cxy = rectClient.Width () + 10;
		GetHeaderCtrl ().SetItem (1, &hdItem);

		GetHeaderCtrl ().ShowWindow (SW_SHOWNOACTIVATE);
	}
	else
	{
		GetHeaderCtrl ().ShowWindow (SW_HIDE);
	}

	::SelectObject (dc.GetSafeHdc (), hfontOld);

	m_rectList = rectClient;
	m_rectList.top += m_nHeaderHeight;

	if (m_bDescriptionArea && m_nDescrHeight != -1)
	{
		m_nDescrHeight = max (m_nDescrHeight, m_nRowHeight);
		m_nDescrHeight = min (m_nDescrHeight, rectClient.Height () - m_nRowHeight);
		m_rectList.bottom -= m_nDescrHeight;
	}

	int cxScroll = ::GetSystemMetrics (SM_CXHSCROLL);
	SetScrollSizes ();

	if (m_nVertScrollTotal > 0)
	{
		m_rectList.right -= cxScroll;
		m_wndScrollVert.SetWindowPos (NULL, m_rectList.right, m_rectList.top,
			cxScroll, m_rectList.Height (), SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		m_wndScrollVert.SetWindowPos (NULL, 0, 0,
			0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	ReposProperties ();

	if (m_pSel != NULL && m_pSel->HasButton ())
	{
		ASSERT_VALID (m_pSel);

		m_pSel->m_rectButton = m_pSel->m_Rect;
		m_pSel->m_rectButton.top++;
		m_pSel->m_rectButton.left = m_pSel->m_rectButton.right - m_pSel->m_rectButton.Height () + 2;
	}

	RedrawWindow ();
}
//******************************************************************************************
void CBCGPPropList::ReposProperties ()
{
	ASSERT_VALID (this);
	m_lstTerminalProps.RemoveAll ();

	if (m_ToolTip.GetSafeHwnd () != NULL)
	{
		while (m_nTooltipsCount > 0)
		{
			m_ToolTip.DelTool (this, m_nTooltipsCount);
			m_nTooltipsCount--;
		}
	}

	int y = m_rectList.top - m_nRowHeight * m_nVertScrollOffset - 1;

	if (!m_bAlphabeticMode)
	{
		for (POSITION pos = m_lstProps.GetHeadPosition (); pos != NULL;)
		{
			CBCGPProp* pProp = m_lstProps.GetNext (pos);
			ASSERT_VALID (pProp);

			pProp->Repos (y);
		}

		return;
	}

	// Get sorted list of terminal properties:
	for (POSITION pos = m_lstProps.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstProps.GetNext (pos);
		ASSERT_VALID (pProp);

		pProp->AddTerminalProp (m_lstTerminalProps);
	}

	for (pos = m_lstTerminalProps.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstTerminalProps.GetNext (pos);
		ASSERT_VALID (pProp);

		pProp->Repos (y);
	}
}
//******************************************************************************************
void CBCGPPropList::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	EndEditItem (FALSE);

	m_nLeftColumnWidth = cx / 2;
	AdjustLayout ();
}
//******************************************************************************************
LRESULT CBCGPPropList::OnSetFont (WPARAM wParam, LPARAM /*lParam*/)
{
	m_hFont = (HFONT) wParam;

	CreateBoldFont ();
	AdjustLayout ();
	return 0;
}
//******************************************************************************************
LRESULT CBCGPPropList::OnGetFont (WPARAM, LPARAM)
{
	return (LRESULT) (m_hFont != NULL ? m_hFont : ::GetStockObject (DEFAULT_GUI_FONT));
}
//******************************************************************************************
void CBCGPPropList::CreateBoldFont ()
{
	if (m_fontBold.GetSafeHandle () != NULL)
	{
		m_fontBold.DeleteObject ();
	}

	CFont* pFont = CFont::FromHandle (
		m_hFont != NULL ? m_hFont : (HFONT) ::GetStockObject (DEFAULT_GUI_FONT));
	ASSERT_VALID (pFont);

	LOGFONT lf;
	memset (&lf, 0, sizeof (LOGFONT));

	pFont->GetLogFont (&lf);

	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect (&lf);
}
//******************************************************************************************
HFONT CBCGPPropList::SetCurrFont (CDC* pDC)
{
	ASSERT_VALID (pDC);
	
	return (HFONT) ::SelectObject (pDC->GetSafeHdc (), 
		m_hFont != NULL ? m_hFont : ::GetStockObject (DEFAULT_GUI_FONT));
}
//******************************************************************************************
void CBCGPPropList::OnPaint() 
{
	CPaintDC dcPaint (this); // device context for painting
	CBCGPMemDC memDC (dcPaint, this);
	CDC* pDC = &memDC.GetDC ();
	
	if (globalData.m_nBitsPerPixel <= 8)
	{
		m_clrGray = m_bControlBarColors ?
			globalData.clrBarShadow : globalData.clrBtnShadow;
	}
	else
	{
		m_clrGray = CBCGPDrawManager::PixelAlpha (
			m_bControlBarColors ? globalData.clrBarFace : globalData.clrBtnFace, 94);
	}

	CRect rectClient;
	GetClientRect (rectClient);

	OnFillBackground (pDC, rectClient);

	HFONT hfontOld = SetCurrFont (pDC);
	pDC->SetTextColor (m_clrText == (COLORREF)-1 ? 
		globalData.clrWindowText : m_clrText);
	pDC->SetBkMode (TRANSPARENT);

	OnDrawList (pDC);
	
	if (m_bDescriptionArea)
	{
		CRect rectDescr = rectClient;
		rectDescr.top = m_rectList.bottom;
		rectDescr.DeflateRect (1, 1);

		OnDrawDescription (pDC, rectDescr);
	}

	::SelectObject (pDC->GetSafeHdc (), hfontOld);
}
//******************************************************************************************
void CBCGPPropList::OnFillBackground (CDC* pDC, CRect rectClient)
{
	ASSERT_VALID (pDC);

	if (m_brBackground.GetSafeHandle () == NULL)
	{
		pDC->FillRect (rectClient, &globalData.brWindow);
	}
	else
	{
		pDC->FillRect (rectClient, &m_brBackground);
	}
}
//******************************************************************************************
void CBCGPPropList::OnDrawBorder (CDC* /*pDC*/)
{
	ASSERT (FALSE);	// This method is obsolete
}
//******************************************************************************************
void CBCGPPropList::OnDrawList (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	COLORREF clrShadow = m_bControlBarColors ?
		globalData.clrBarShadow : globalData.clrBtnShadow;

	CPen penLine (PS_SOLID, 1, m_clrLine != (COLORREF)-1 ?
		m_clrLine : (m_bVSDotNetLook ? m_clrGray : clrShadow));
	CPen* pOldPen = pDC->SelectObject (&penLine);

	int nXCenter = m_rectList.left + m_nLeftColumnWidth;

	pDC->MoveTo (nXCenter, m_rectList.top - 1);
	pDC->LineTo (nXCenter, m_rectList.bottom);

	const CList<CBCGPProp*, CBCGPProp*>& lst = m_bAlphabeticMode ? 
		m_lstTerminalProps : m_lstProps;

	for (POSITION pos = lst.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = lst.GetNext (pos);
		ASSERT_VALID (pProp);

		if (!OnDrawProperty (pDC, pProp))
		{
			break;
		}
	}

	pDC->SelectObject (pOldPen);
}
//****************************************************************************************
void CBCGPPropList::OnDrawDescription (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_clrDescriptionBackground != (COLORREF)-1)
	{
		CBrush br (m_clrDescriptionBackground);
		pDC->FillRect (rect, &br);
	}
	else
	{
		pDC->FillRect (rect, 
			m_bControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);
	}

	rect.top += TEXT_MARGIN;

	COLORREF clrShadow = m_bControlBarColors ?
		globalData.clrBarShadow : globalData.clrBtnShadow;

	pDC->Draw3dRect (rect, clrShadow, clrShadow);

	if (m_pSel == NULL)
	{
		return;
	}

	rect.DeflateRect (TEXT_MARGIN, TEXT_MARGIN);

	ASSERT_VALID (m_pSel);

	COLORREF clrTextOld = (COLORREF)-1;

	if (m_clrDescriptionText != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor (m_clrDescriptionText);
	}

	HFONT hOldFont = hOldFont = (HFONT) ::SelectObject (pDC->GetSafeHdc (), m_fontBold.GetSafeHandle ());
	int nHeight = pDC->DrawText (m_pSel->m_strName, rect, DT_SINGLELINE | DT_NOPREFIX);

	::SelectObject (pDC->GetSafeHdc (), hOldFont);

	rect.top += nHeight + 2;
	pDC->DrawText (m_pSel->m_strDescr, rect, DT_WORDBREAK | DT_NOPREFIX | DT_END_ELLIPSIS);

	if (clrTextOld == (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}
}
//******************************************************************************************
BOOL CBCGPPropList::OnDrawProperty (CDC* pDC, CBCGPProp* pProp) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (pProp);

	if (!pProp->m_Rect.IsRectEmpty ())
	{
		if (pProp->m_Rect.top >= m_rectList.bottom)
		{
			return FALSE;
		}

		if (pProp->m_Rect.bottom >= m_rectList.top)
		{
			const int nXCenter = m_rectList.left + m_nLeftColumnWidth;
			COLORREF clrTextOld = (COLORREF)-1;

			if (m_bVSDotNetLook)
			{
				CRect rectLeft = pProp->m_Rect;

				if (!pProp->IsGroup ())
				{
					rectLeft.right = rectLeft.left;
				}

				if (pProp->m_bIsValueList)
				{
					rectLeft.right = rectLeft.left + rectLeft.Height ();
				}

				rectLeft.left = m_rectList.left;
				rectLeft.bottom = min (rectLeft.bottom, m_rectList.bottom);

				if (rectLeft.left < rectLeft.right)
				{
					CBrush br (m_clrGroupBackground != (COLORREF)-1 ? 
						m_clrGroupBackground : m_clrGray);
					pDC->FillRect (rectLeft, &br);
				}
			}

			if (!pProp->IsEnabled ())
			{
				clrTextOld = pDC->SetTextColor (globalData.clrGrayedText);
			}

			CRect rectName = pProp->m_Rect;

			if ((!pProp->IsGroup () || pProp->m_bIsValueList || !m_bGroupNameFullWidth) &&
				pProp->HasValueField ())
			{
				rectName.right = nXCenter;
			}

			if (pProp->IsGroup ())
			{
				if (m_bGroupNameFullWidth && !m_bVSDotNetLook && !pProp->m_bIsValueList)
				{
					CRect rectFill = rectName;
					rectFill.top++;

					if (m_brBackground.GetSafeHandle () != NULL)
					{
						CBrush& br = ((CBCGPPropList*) this)->m_brBackground;
						pDC->FillRect (rectFill, &br);
					}
					else
					{
						pDC->FillRect (rectFill, &globalData.brWindow);
					}
				}

				CRect rectExpand = rectName;
				rectName.left += m_nRowHeight;
				rectExpand.right = rectName.left;

				CRgn rgnClipExpand;
				CRect rectExpandClip = rectExpand;
				rectExpandClip.bottom = min (rectExpandClip.bottom, m_rectList.bottom);

				rgnClipExpand.CreateRectRgnIndirect (&rectExpandClip);
				pDC->SelectClipRgn (&rgnClipExpand);

				pProp->OnDrawExpandBox (pDC, rectExpand);
			}
			else if (!pProp->HasValueField ())
			{
				CRect rectFill = rectName;
				rectFill.top++;

				if (m_brBackground.GetSafeHandle () != NULL)
				{
					CBrush& br = ((CBCGPPropList*) this)->m_brBackground;
					pDC->FillRect (rectFill, &br);
				}
				else
				{
					pDC->FillRect (rectFill, &globalData.brWindow);
				}
			}

			if (rectName.right > rectName.left)
			{
				CRgn rgnClipName;
				CRect rectNameClip = rectName;
				rectNameClip.bottom = min (rectNameClip.bottom, m_rectList.bottom);

				rgnClipName.CreateRectRgnIndirect (&rectNameClip);
				pDC->SelectClipRgn (&rgnClipName);

				HFONT hOldFont = NULL;
				if (pProp->IsGroup () && !pProp->m_bIsValueList)
				{
					hOldFont = (HFONT) ::SelectObject (pDC->GetSafeHdc (), m_fontBold.GetSafeHandle ());
				}

				pProp->OnDrawName (pDC, rectName);

				if (hOldFont != NULL)
				{
					::SelectObject (pDC->GetSafeHdc (), hOldFont);
				}
			}

			CRect rectValue = pProp->m_Rect;
			rectValue.left = nXCenter + 1;

			CRgn rgnClipVal;
			CRect rectValClip = rectValue;
			rectValClip.bottom = min (rectValClip.bottom, m_rectList.bottom);

			rgnClipVal.CreateRectRgnIndirect (&rectValClip);
			pDC->SelectClipRgn (&rgnClipVal);

			pProp->OnDrawValue (pDC, rectValue);

			if (!pProp->m_rectButton.IsRectEmpty ())
			{
				pProp->OnDrawButton (pDC, pProp->m_rectButton);
			}

			pDC->SelectClipRgn (NULL);

			pDC->MoveTo (m_rectList.left, pProp->m_Rect.bottom);
			pDC->LineTo (m_rectList.right, pProp->m_Rect.bottom);

			if (clrTextOld != (COLORREF)-1)
			{
				pDC->SetTextColor (clrTextOld);
			}
		}
	}

	if (pProp->IsExpanded () || m_bAlphabeticMode)
	{
		for (POSITION pos = pProp->m_lstSubItems.GetHeadPosition (); pos != NULL;)
		{
			if (!OnDrawProperty (pDC, pProp->m_lstSubItems.GetNext (pos)))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}
//****************************************************************************************
void CBCGPPropList::OnPropertyChanged (CBCGPProp* pProp) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pProp);

	GetOwner ()->SendMessage (BCGM_PROPERTY_CHANGED, GetDlgCtrlID (),
		LPARAM (pProp));
}
//*******************************************************************************************
BOOL CBCGPPropList::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//******************************************************************************************
void CBCGPPropList::OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pHeader = (NMHEADER*) pNMHDR;

	if (pHeader->iItem == 0)
	{
		HDITEM hdItem;
		hdItem.mask = HDI_WIDTH;

		GetHeaderCtrl ().GetItem (0, &hdItem);

		CRect rectClient;
		GetClientRect (rectClient);

		m_nLeftColumnWidth = min (max (m_nRowHeight, hdItem.cxy - 2), rectClient.Width () - ::GetSystemMetrics (SM_CXHSCROLL) - 5);

		ReposProperties ();

		InvalidateRect (m_rectList);
		UpdateWindow ();
	}
	
	*pResult = 0;
}
//******************************************************************************************
void CBCGPPropList::EnableHeaderCtrl (BOOL bEnable, LPCTSTR lpszLeftColumn,
									  LPCTSTR lpszRightColumn)
{
	ASSERT_VALID (this);
	ASSERT (lpszLeftColumn != NULL);
	ASSERT (lpszRightColumn != NULL);

	CBCGPProp* pProp = GetCurSel ();
	if (pProp != NULL)
	{
		pProp->OnEndEdit ();
	}

	m_bHeaderCtrl = bEnable;

	if (m_bHeaderCtrl)
	{
		HDITEM hdItem;
		hdItem.mask = HDI_TEXT;

		hdItem.pszText = (LPTSTR) lpszLeftColumn;
		hdItem.cchTextMax = lstrlen (lpszLeftColumn) + 1;
		GetHeaderCtrl ().SetItem (0, &hdItem);

		hdItem.pszText = (LPTSTR) lpszRightColumn;
		hdItem.cchTextMax = lstrlen (lpszRightColumn) + 1;
		GetHeaderCtrl ().SetItem (1, &hdItem);
	}

	AdjustLayout ();
	RedrawWindow ();
}
//******************************************************************************************
void CBCGPPropList::EnableDescriptionArea (BOOL bEnable)
{
	ASSERT_VALID (this);

	m_bDescriptionArea = bEnable;

	AdjustLayout ();

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//*****************************************************************************************
void CBCGPPropList::OnHeaderTrack(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pHeader = (NMHEADER*) pNMHDR;

	pHeader->pitem->cxy = min (pHeader->pitem->cxy, m_rectList.Width ());

	TrackHeader (pHeader->pitem->cxy);
	*pResult = 0;
}
//******************************************************************************************
void CBCGPPropList::OnHeaderEndTrack(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	TrackHeader (-1);
	*pResult = 0;
}
//*****************************************************************************************
void CBCGPPropList::TrackHeader (int nOffset)
{
	CClientDC dc (this);

	if (!m_rectTrackHeader.IsRectEmpty () && !m_bShowDragContext)
	{
		dc.InvertRect (m_rectTrackHeader);
	}

	if (nOffset < 0)	// End of track
	{
		m_rectTrackHeader.SetRectEmpty ();
	}
	else
	{
		m_rectTrackHeader = m_rectList;
		m_rectTrackHeader.left += nOffset;
		m_rectTrackHeader.right = m_rectTrackHeader.left + 1;

		if (m_bShowDragContext)
		{
			CRect rectClient;
			GetClientRect (rectClient);

			m_nLeftColumnWidth = min (max (m_nRowHeight, nOffset), rectClient.Width () - ::GetSystemMetrics (SM_CXHSCROLL) - 5);

			HDITEM hdItem;
			hdItem.mask = HDI_WIDTH ;
			hdItem.cxy = m_nLeftColumnWidth + 2;

			GetHeaderCtrl ().SetItem (0, &hdItem);

			hdItem.cxy = rectClient.Width () + 10;
			GetHeaderCtrl ().SetItem (1, &hdItem);
		}
		else
		{
			dc.InvertRect (m_rectTrackHeader);
		}
	}
}
//*****************************************************************************************
void CBCGPPropList::TrackDescr (int nOffset)
{
	CClientDC dc (this);

	if (!m_rectTrackDescr.IsRectEmpty ())
	{
		dc.InvertRect (m_rectTrackDescr);
	}

	if (nOffset == INT_MIN)	// End of track
	{
		m_rectTrackDescr.SetRectEmpty ();
	}
	else
	{
		CRect rectClient;
		GetClientRect (rectClient);

		nOffset = max (nOffset, rectClient.top + m_nRowHeight + m_nHeaderHeight);
		nOffset = min (nOffset, rectClient.bottom - m_nRowHeight);

		m_rectTrackDescr = rectClient;
		m_rectTrackDescr.top = nOffset - 1;
		m_rectTrackDescr.bottom = m_rectTrackDescr.top + 2;

		dc.InvertRect (m_rectTrackDescr);
	}
}
//******************************************************************************************
int CBCGPPropList::AddProperty (CBCGPProp* pProp, BOOL bRedraw)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pProp);

	for (POSITION pos = m_lstProps.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pListProp = m_lstProps.GetNext (pos);
		ASSERT_VALID (pListProp);

		if (pListProp == pProp || pListProp->IsSubItem (pProp))
		{
			// Can't ad the same property twice
			ASSERT (FALSE);
			return -1;
		}
	}

	pProp->SetOwnerList (this);

	m_lstProps.AddTail (pProp);
	int nIndex = (int) m_lstProps.GetCount () - 1;

	AdjustLayout ();

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		pProp->Redraw ();
	}

	return nIndex;
}
//*****************************************************************************************
void CBCGPPropList::RemoveAll ()
{
	ASSERT_VALID (this);

	while (!m_lstProps.IsEmpty ())
	{
		delete m_lstProps.RemoveHead ();
	}

	while (m_nTooltipsCount > 0)
	{
		m_ToolTip.DelTool (this, m_nTooltipsCount);
		m_nTooltipsCount--;
	}

	m_pSel = NULL;
}
//******************************************************************************************
CBCGPProp* CBCGPPropList::GetProperty (int nIndex) const
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= m_lstProps.GetCount ())
	{
		ASSERT (FALSE);
		return NULL;
	}

	return m_lstProps.GetAt (m_lstProps.FindIndex (nIndex));
}
//******************************************************************************************
CBCGPProp* CBCGPPropList::FindItemByData (DWORD dwData, BOOL bSearchSubItems/* = TRUE*/) const
{
	ASSERT_VALID (this);

	for (POSITION pos = m_lstProps.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstProps.GetNext (pos);
		ASSERT_VALID (pProp);

		if (pProp->m_dwData == dwData)
		{
			return pProp;
		}

		if (bSearchSubItems) 
		{
			pProp = pProp->FindSubItemByData (dwData);

			if  (pProp != NULL)
			{
				ASSERT_VALID (pProp);
				return pProp;
			}
		}
	}

	return NULL;
}
//*******************************************************************************************
CBCGPProp* CBCGPPropList::HitTest (CPoint pt, CBCGPProp::ClickArea* pnArea) const
{
	ASSERT_VALID (this);

	const CList<CBCGPProp*, CBCGPProp*>& lst = m_bAlphabeticMode ? 
		m_lstTerminalProps : m_lstProps;

	for (POSITION pos = lst.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = lst.GetNext (pos);
		ASSERT_VALID (pProp);

		CBCGPProp* pHit = pProp->HitTest (pt, pnArea);
		if (pHit != NULL)
		{
			return pHit;
		}
	}

	return NULL;
}
//*******************************************************************************************
void CBCGPPropList::SetCurSel (CBCGPProp* pProp, BOOL bRedraw)
{
	ASSERT_VALID (this);

	CBCGPProp* pOldSelectedItem = m_pSel;
	if (pOldSelectedItem == pProp)
	{
		return;
	}

	if (m_pSel != NULL && m_pSel->m_bInPlaceEdit)
	{
		EndEditItem (FALSE);
	}

	m_pSel = pProp;
	OnChangeSelection (m_pSel, pOldSelectedItem);

	if (pOldSelectedItem != NULL)
	{
		pOldSelectedItem->OnKillSelection (pProp);

		CRect rectButton = pOldSelectedItem->m_rectButton;
		pOldSelectedItem->m_rectButton.SetRectEmpty ();

		if (bRedraw)
		{
			CRect rectOld = pOldSelectedItem->m_Rect;

			if (!pOldSelectedItem->IsGroup () || !m_bGroupNameFullWidth)
			{
				if (!pOldSelectedItem->IsGroup () && pOldSelectedItem->HasValueField ())
				{
					rectOld.right = rectOld.left + m_nLeftColumnWidth;
				}
			}

			InvalidateRect (rectOld);
			InvalidateRect (rectButton);
		}
	}

	if (pProp != NULL)
	{
		pProp->OnSetSelection (pOldSelectedItem);

		if (pProp->HasButton ())
		{
			pProp->m_rectButton = pProp->m_Rect;
			pProp->m_rectButton.top++;
			pProp->m_rectButton.left = pProp->m_rectButton.right - pProp->m_rectButton.Height () + 2;
		}

		if (bRedraw)
		{
			CRect rect = pProp->m_Rect;

			if (!pProp->IsGroup () || !m_bGroupNameFullWidth)
			{
				if (!pProp->IsGroup () && pProp->HasValueField ())
				{
					rect.right = rect.left + m_nLeftColumnWidth;
				}
			}

			InvalidateRect (rect);
			InvalidateRect (pProp->m_rectButton);
		}
	}

	if (bRedraw)
	{
		if (m_bDescriptionArea)
		{
			CRect rectClient;
			GetClientRect (rectClient);

			CRect rectDescr = rectClient;
			rectDescr.top = m_rectList.bottom;
			rectDescr.DeflateRect (TEXT_MARGIN, TEXT_MARGIN);
			InvalidateRect (rectDescr);
		}

		UpdateWindow ();
	}
}
//******************************************************************************************
void CBCGPPropList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	SetFocus ();

	CRect rectClient;
	GetClientRect (rectClient);

	if (m_bDescriptionArea)
	{
		if (abs (point.y - (m_rectList.bottom + TEXT_MARGIN)) <= STRETCH_DELTA)
		{
			SetCapture ();
			TrackDescr (point.y);
			m_bTrackingDescr = TRUE;
			return;
		}

		if (point.y > m_rectList.bottom)
		{
			return;
		}
	}

	if (abs (point.x - (m_rectList.left + m_nLeftColumnWidth)) <= STRETCH_DELTA)
	{
		SetCapture ();
		TrackHeader (point.x);
		m_bTracking = TRUE;
		return;
	}

	CBCGPProp::ClickArea clickArea;
	CBCGPProp* pHit = HitTest (point, &clickArea);

	BOOL bSelChanged = pHit != GetCurSel ();

	SetCurSel (pHit);
	if (pHit == NULL)
	{
		return;
	}

	EnsureVisible (pHit);

	switch (clickArea)
	{
	case CBCGPProp::ClickExpandBox:
		pHit->Expand (!pHit->IsExpanded ());
		break;

	case CBCGPProp::ClickName:
		pHit->OnClickName (point);
		break;

	case CBCGPProp::ClickValue:
		if (pHit->m_bEnabled)
		{
			if (EditItem (pHit, &point) && pHit->m_pWndInPlace != NULL)
			{
				if (pHit->m_rectButton.PtInRect (point))
				{
					CString strPrevVal = pHit->FormatProperty ();

					::ReleaseCapture();

					CWaitCursor wait;
					pHit->OnClickButton (point);

					if (strPrevVal != pHit->FormatProperty ())
					{
						OnPropertyChanged (pHit);
					}
				}
				else if (!bSelChanged || pHit->IsProcessFirstClick ())
				{
					ASSERT_VALID (pHit->m_pWndInPlace);

					CPoint ptEdit = point;
					::MapWindowPoints (GetSafeHwnd (), pHit->m_pWndInPlace->GetSafeHwnd (), &ptEdit, 1);
					pHit->m_pWndInPlace->SendMessage (WM_LBUTTONDOWN, 0, MAKELPARAM (ptEdit.x, ptEdit.y));
				}
			}
		}
		break;

	default:
		break;
	}
}
//************************************************************************************
void CBCGPPropList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);

	SetFocus ();

	CRect rectClient;
	GetClientRect (rectClient);

	if (m_bDescriptionArea)
	{
		if (abs (point.y - (m_rectList.bottom + TEXT_MARGIN)) <= STRETCH_DELTA)
		{
			return;
		}

		if (point.y > m_rectList.bottom)
		{
			return;
		}
	}

	if (abs (point.x - (m_rectList.left + m_nLeftColumnWidth)) <= STRETCH_DELTA)
	{
		return;
	}

	CBCGPProp::ClickArea clickArea;
	CBCGPProp* pHit = HitTest (point, &clickArea);

	BOOL bSelChanged = pHit != GetCurSel ();

	SetCurSel (pHit);
	if (pHit == NULL)
	{
		return;
	}

	EnsureVisible (pHit);

	switch (clickArea)
	{
	case CBCGPProp::ClickExpandBox:
		break;

	case CBCGPProp::ClickName:
		pHit->OnRClickName (point);
		break;

	case CBCGPProp::ClickValue:
		pHit->OnRClickValue (point, bSelChanged);

		if (pHit->m_bEnabled && !bSelChanged)
		{
			if (EditItem (pHit, &point) && pHit->m_pWndInPlace != NULL)
			{
				if (pHit->m_rectButton.PtInRect (point))
				{
					return;
				}
				else if (pHit->IsProcessFirstClick ())
				{
					ASSERT_VALID (pHit->m_pWndInPlace);

					CPoint ptEdit = point;
					::MapWindowPoints (GetSafeHwnd (), pHit->m_pWndInPlace->GetSafeHwnd (), &ptEdit, 1);
					pHit->m_pWndInPlace->SendMessage (WM_RBUTTONDOWN, 0, MAKELPARAM (ptEdit.x, ptEdit.y));
				}
			}
		}
		break;

	default:
		break;
	}
}
//******************************************************************************************
BOOL CBCGPPropList::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID, NULL);
}
//******************************************************************************************
BOOL CBCGPPropList::EditItem (CBCGPProp* pProp, LPPOINT lptClick)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pProp);

	if (!EndEditItem ())
	{
		return FALSE;
	}

	if (pProp->IsGroup () && !pProp->m_bIsValueList)
	{
		return FALSE;
	}

	if (pProp->OnEdit (lptClick))
	{
		pProp->Redraw ();
		SetCurSel (pProp);
		SetCapture ();
	}

	return TRUE;
}
//******************************************************************************************
void CBCGPPropList::OnClickButton (CPoint point)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pSel);

	if (m_pSel->OnUpdateValue ())
	{
		CString strPrevVal = m_pSel->FormatProperty ();

		CWaitCursor wait;
		m_pSel->OnClickButton (point);

		if (strPrevVal != m_pSel->FormatProperty ())
		{
			OnPropertyChanged (m_pSel);
		}
	}
}
//******************************************************************************************
BOOL CBCGPPropList::EndEditItem (BOOL bUpdateData/* = TRUE*/)
{
	ASSERT_VALID (this);

	if (m_pSel == NULL)
	{
		return TRUE;
	}

	ASSERT_VALID (m_pSel);

	if (!m_pSel->m_bInPlaceEdit)
	{
		return TRUE;
	}

	if (bUpdateData)
	{
		if (!ValidateItemData (m_pSel) || !m_pSel->OnUpdateValue ())
		{
			return FALSE;
		}
	}

	if (!m_pSel->OnEndEdit ())
	{
		return FALSE;
	}

	ReleaseCapture ();
	m_pSel->Redraw ();

	return TRUE;
}
//*******************************************************************************************
BOOL CBCGPPropList::PreTranslateMessage(MSG* pMsg) 
{
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
	case WM_MOUSEMOVE:
		m_ToolTip.RelayEvent(pMsg);
		break;
	}

	if (pMsg->message == WM_SYSKEYDOWN && 
		(pMsg->wParam == VK_DOWN || pMsg->wParam == VK_RIGHT) && 
		m_pSel != NULL && m_pSel->m_bEnabled && 
		((m_pSel->m_dwFlags) & PROP_HAS_BUTTON) &&
		EditItem (m_pSel))
	{
		CString strPrevVal = m_pSel->FormatProperty ();

		CWaitCursor wait;
		m_pSel->OnClickButton (CPoint (-1, -1));

		if (strPrevVal != m_pSel->FormatProperty ())
		{
			OnPropertyChanged (m_pSel);
		}

		return TRUE;
	}

	if (m_pSel != NULL && m_pSel->m_bInPlaceEdit != NULL && m_pSel->m_bEnabled)
	{
		ASSERT_VALID (m_pSel);

		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_RETURN:
				if (m_pSel->m_pWndCombo != NULL && m_pSel->m_pWndCombo->GetDroppedState ())
				{
					m_pSel->OnSelectCombo ();
					m_pSel->m_pWndInPlace->SetFocus ();
					return TRUE;
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
				break;

			default:
				if (!m_pSel->m_bAllowEdit)
				{
					m_pSel->PushChar ((UINT) pMsg->wParam);
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

			if (m_pSel->m_pWndSpin != NULL)
			{
				ASSERT_VALID (m_pSel->m_pWndSpin);
				ASSERT (m_pSel->m_pWndSpin->GetSafeHwnd () != NULL);

				CRect rectSpin;
				m_pSel->m_pWndSpin->GetClientRect (rectSpin);
				m_pSel->m_pWndSpin->MapWindowPoints (this, rectSpin);

				if (rectSpin.PtInRect (ptCursor))
				{
					MapWindowPoints (m_pSel->m_pWndSpin, &ptCursor, 1); 

					m_pSel->m_pWndSpin->SendMessage (pMsg->message, pMsg->wParam, 
						MAKELPARAM (ptCursor.x, ptCursor.y));
					return TRUE;
				}
			}

			CWnd* pWndInPlaceEdit = m_pSel->m_pWndInPlace;
			ASSERT_VALID (pWndInPlaceEdit);

			if (!m_pSel->m_bAllowEdit)
			{
				m_pSel->m_pWndInPlace->HideCaret ();
			}

			CRect rectEdit;
			pWndInPlaceEdit->GetClientRect (rectEdit);
			pWndInPlaceEdit->MapWindowPoints (this, rectEdit);

			if (rectEdit.PtInRect (ptCursor) &&
				pMsg->message == WM_LBUTTONDBLCLK)
			{
				if (m_pSel->OnDblClick (ptCursor))
				{
					return TRUE;
				}
			}

			if (rectEdit.PtInRect (ptCursor) && 
				pMsg->message == WM_RBUTTONDOWN &&
				!m_pSel->m_bAllowEdit)
			{
				return TRUE;
			}

			if (!rectEdit.PtInRect (ptCursor) &&
				(pMsg->message == WM_LBUTTONDOWN ||
				pMsg->message == WM_NCLBUTTONDOWN))
			{
				if (m_pSel->m_rectButton.PtInRect (ptCursor))
				{
					CString strPrevVal = m_pSel->FormatProperty ();

					CWaitCursor wait;
					OnClickButton (ptCursor);

					if (strPrevVal != m_pSel->FormatProperty ())
					{
						OnPropertyChanged (m_pSel);
					}
					return TRUE;
				}

				if (!EndEditItem ())
				{
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
		else
		{
			return FALSE;
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}
//*******************************************************************************************
void CBCGPPropList::OnCancelMode() 
{
	if (m_bTracking)
	{
		TrackHeader (-1);
		m_bTracking = FALSE;
		ReleaseCapture ();
	}

	if (m_bTrackingDescr)
	{
		TrackDescr (INT_MIN);
		m_bTrackingDescr = FALSE;
		ReleaseCapture ();
	}

	EndEditItem (FALSE);
	CWnd::OnCancelMode();
}
//******************************************************************************************
void CBCGPPropList::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_bFocused = TRUE;
	
	if (m_pSel != NULL)
	{
		RedrawWindow (m_pSel->m_Rect);
	}
}
//******************************************************************************************
void CBCGPPropList::OnKillFocus(CWnd* pNewWnd) 
{
	if (!IsChild (pNewWnd))
	{
		if (m_pSel == NULL || m_pSel->OnKillFocus (pNewWnd))
		{
			EndEditItem (FALSE);
			m_bFocused = FALSE;

			if (m_pSel != NULL)
			{
				m_pSel->Redraw ();
			}
		}
	}

	CWnd::OnKillFocus(pNewWnd);
}
//******************************************************************************************
void CBCGPPropList::OnStyleChanged (int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	CWnd::OnStyleChanged (nStyleType, lpStyleStruct);
	AdjustLayout ();
}
//******************************************************************************************
UINT CBCGPPropList::OnGetDlgCode() 
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}
//******************************************************************************************
void CBCGPPropList::SetScrollSizes ()
{
	ASSERT_VALID (this);

	if (m_wndScrollVert.GetSafeHwnd () == NULL)
	{
		return;
	}

	if (m_nRowHeight == 0)
	{
		m_nVertScrollPage = 0;
		m_nVertScrollTotal = 0;
		m_nVertScrollOffset = 0;
	}
	else
	{
		m_nVertScrollPage = m_rectList.Height () / m_nRowHeight - 1;
		m_nVertScrollTotal = GetTotalItems ();

		if (m_nVertScrollTotal <= m_nVertScrollPage)
		{
			m_nVertScrollPage = 0;
			m_nVertScrollTotal = 0;
		}

		m_nVertScrollOffset = min (m_nVertScrollOffset, m_nVertScrollTotal);
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
}
//******************************************************************************************
int CBCGPPropList::GetTotalItems () const
{
	ASSERT_VALID (this);

	if (m_bAlphabeticMode)
	{
		return (int) m_lstTerminalProps.GetCount ();
	}

	int nCount = 0;

	for (POSITION pos = m_lstProps.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstProps.GetNext (pos);
		ASSERT_VALID (pProp);

		nCount += pProp->GetExpandedSubItems () + 1;
	}

	return nCount;
}
//******************************************************************************************
void CBCGPPropList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (m_pSel != NULL && 
		pScrollBar->GetSafeHwnd () != NULL &&
		m_pSel->m_pWndSpin->GetSafeHwnd () == pScrollBar->GetSafeHwnd ())
	{
		return;
	}

	EndEditItem (FALSE);

	int nPrevOffset = m_nVertScrollOffset;

	switch (nSBCode)
	{
	case SB_LINEUP:
		m_nVertScrollOffset--;
		break;

	case SB_LINEDOWN:
		m_nVertScrollOffset++;
		break;

	case SB_TOP:
		m_nVertScrollOffset = 0;
		break;

	case SB_BOTTOM:
		m_nVertScrollOffset = m_nVertScrollTotal;
		break;

	case SB_PAGEUP:
		m_nVertScrollOffset -= m_nVertScrollPage;
		break;

	case SB_PAGEDOWN:
		m_nVertScrollOffset += m_nVertScrollPage;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_nVertScrollOffset = nPos;
		break;

	default:
		return;
	}

	m_nVertScrollOffset = min (max (0, m_nVertScrollOffset), 
		m_nVertScrollTotal - m_nVertScrollPage + 1);

	if (m_nVertScrollOffset == nPrevOffset)
	{
		return;
	}

	SetScrollPos (SB_VERT, m_nVertScrollOffset);

	ReposProperties ();

	int dy = m_nRowHeight * (nPrevOffset - m_nVertScrollOffset);
	ScrollWindow (0, dy, m_rectList, m_rectList);

	if (m_pSel != NULL)
	{
		ASSERT_VALID (m_pSel);
		RedrawWindow (m_pSel->m_rectButton);
	}
}
//*******************************************************************************************
CScrollBar* CBCGPPropList::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_HORZ || m_wndScrollVert.GetSafeHwnd () == NULL)
	{
		return NULL;
	}

	return (CScrollBar* ) &m_wndScrollVert;
}
//******************************************************************************************
BOOL CBCGPPropList::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/) 
{
	int nSteps = abs(zDelta) / WHEEL_DELTA;

	for (int i = 0; i < nSteps; i++)
	{
		OnVScroll (zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, NULL);
	}

	return TRUE;
}
//*******************************************************************************************
void CBCGPPropList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);

	if (m_pSel == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pSel);

	if (m_pSel->IsGroup () &&
		(!m_pSel->m_bIsValueList || 
		point.x < m_rectList.left + m_nLeftColumnWidth))
	{
		m_pSel->Expand (!m_pSel->IsExpanded ());
	}
	else if (m_pSel->m_bEnabled)
	{
		if (EditItem (m_pSel) && m_pSel->m_pWndInPlace != NULL)
		{
			m_pSel->m_pWndInPlace->SendMessage (WM_LBUTTONDOWN);
			m_pSel->m_pWndInPlace->SendMessage (WM_LBUTTONUP);
		}

		m_pSel->OnDblClick (point);
	}
}
//*******************************************************************************************
BOOL CBCGPPropList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		CPoint point;

		::GetCursorPos (&point);
		ScreenToClient (&point);

		if (m_bDescriptionArea &&
			abs (point.y - (m_rectList.bottom + TEXT_MARGIN)) <= STRETCH_DELTA)
		{
			::SetCursor (globalData.m_hcurStretchVert);
			return TRUE;
		}

		if (point.y <= m_rectList.bottom)
		{
			if (abs (point.x - (m_rectList.left + m_nLeftColumnWidth)) <= STRETCH_DELTA)
			{
				::SetCursor (globalData.m_hcurStretch);
				return TRUE;
			}

			CBCGPProp::ClickArea clickArea;
			CBCGPProp* pHit = HitTest (point, &clickArea);

			if (pHit != NULL && pHit == m_pSel && clickArea == CBCGPProp::ClickValue &&
				!pHit->m_rectButton.PtInRect (point) &&
				pHit->OnSetCursor ())
			{
				return TRUE;
			}
		}
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
//******************************************************************************************
void CBCGPPropList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_lstProps.IsEmpty ())
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	switch (nChar)
	{
	case VK_F4:
		if (m_pSel != NULL && m_pSel->m_bEnabled && EditItem (m_pSel))
		{
			if ((m_pSel->m_dwFlags) & PROP_HAS_BUTTON)
			{
				CString strPrevVal = m_pSel->FormatProperty ();

				CWaitCursor wait;
				m_pSel->OnClickButton (CPoint (-1, -1));

				if (strPrevVal != m_pSel->FormatProperty ())
				{
					OnPropertyChanged (m_pSel);
				}
			}
			return;
		}
		break;

	case VK_LEFT:
		if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
		{
			BOOL bShowDragContext = m_bShowDragContext;
			m_bShowDragContext = TRUE;

			TrackHeader (m_nLeftColumnWidth - 5);

			m_bShowDragContext = bShowDragContext;
			return;
		}
		else if (m_pSel != NULL && m_pSel->IsGroup () && m_pSel->IsExpanded ())
		{
			m_pSel->Expand (FALSE);
			return;
		}
		
		// else ==> act as VK_UP!

	case VK_UP:
		{
			if (m_pSel == NULL)
			{
				SetCurSel (m_bAlphabeticMode ? m_lstTerminalProps.GetHead () : m_lstProps.GetHead ());
				OnVScroll (SB_TOP, 0, NULL);
				return;
			}

			// Select prev. item:
			CPoint point (m_pSel->m_Rect.right - 1, m_pSel->m_Rect.top - 2);

			CBCGPProp* pHit = HitTest (point);
			if (pHit != NULL)
			{
				SetCurSel (pHit);
				EnsureVisible (pHit);
			}
		}
		return;

	case VK_RIGHT:
		if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
		{
			BOOL bShowDragContext = m_bShowDragContext;
			m_bShowDragContext = TRUE;

			TrackHeader (m_nLeftColumnWidth + 5);

			m_bShowDragContext = bShowDragContext;
			return;
		}
		else if (m_pSel != NULL && m_pSel->IsGroup () && !m_pSel->IsExpanded ())
		{
			m_pSel->Expand ();
			return;
		}
		
		// else ==> act as VK_DOWN!

	case VK_DOWN:
		{
			if (m_pSel == NULL)
			{
				SetCurSel (m_bAlphabeticMode ? m_lstTerminalProps.GetHead () : m_lstProps.GetHead ());
				OnVScroll (SB_TOP, 0, NULL);
				return;
			}

			if ((::GetAsyncKeyState (VK_MENU) & 0x8000) && nChar == VK_DOWN)
			{
				CString strPrevVal = m_pSel->FormatProperty ();

				CWaitCursor wait;
				m_pSel->OnClickButton (CPoint (-1, -1));

				if (strPrevVal != m_pSel->FormatProperty ())
				{
					OnPropertyChanged (m_pSel);
				}

				return;
			}

			// Select next item:
			CPoint point (m_pSel->m_Rect.right - 1, m_pSel->m_Rect.bottom + 2);

			CBCGPProp* pHit = HitTest (point);
			if (pHit != NULL)
			{
				SetCurSel (pHit);
				EnsureVisible (pHit);
			}
		}
		return;

	case VK_NEXT:
		{
			if (m_pSel == NULL)
			{
				SetCurSel (m_bAlphabeticMode ? m_lstTerminalProps.GetHead () : m_lstProps.GetHead ());
				OnVScroll (SB_TOP, 0, NULL);
				return;
			}

			if (m_nVertScrollPage != 0)
			{
				EnsureVisible (m_pSel);

				CPoint point (m_pSel->m_Rect.right - 1, 
					m_pSel->m_Rect.top + m_nVertScrollPage * m_nRowHeight);

				CBCGPProp* pHit = HitTest (point);
				if (pHit != NULL)
				{
					SetCurSel (pHit);
					OnVScroll (SB_PAGEDOWN, 0, NULL);
					return;
				}
			}
		}

	case VK_END:
		{
			CBCGPProp* pLastProp = NULL;
			
			if (m_bAlphabeticMode)
			{
				pLastProp = m_lstTerminalProps.GetTail ();
				ASSERT_VALID (pLastProp);
			}
			else
			{
				pLastProp = m_lstProps.GetTail ();
				ASSERT_VALID (pLastProp);

				while (!pLastProp->m_lstSubItems.IsEmpty () && pLastProp->IsExpanded ())
				{
					pLastProp = pLastProp->m_lstSubItems.GetTail ();
				}
			}

			SetCurSel (pLastProp);
			OnVScroll (SB_BOTTOM, 0, NULL);
		}
		return;

	case VK_PRIOR:
		{
			if (m_pSel != NULL && m_nVertScrollPage != 0)
			{
				EnsureVisible (m_pSel);

				CPoint point (m_pSel->m_Rect.right - 1, 
					m_pSel->m_Rect.top - m_nVertScrollPage * m_nRowHeight);

				CBCGPProp* pHit = HitTest (point);
				if (pHit != NULL)
				{
					SetCurSel (pHit);
					OnVScroll (SB_PAGEUP, 0, NULL);
					return;
				}
			}
		}

	case VK_HOME:
		SetCurSel (m_bAlphabeticMode ? m_lstTerminalProps.GetHead () : m_lstProps.GetHead ());
		OnVScroll (SB_TOP, 0, NULL);
		return;


	case VK_ADD:
		if (m_pSel != NULL && m_pSel->IsGroup () && !m_pSel->IsExpanded () &&
			!m_pSel->IsInPlaceEditing ())
		{
			m_pSel->Expand ();
		}
		return;

	case VK_SUBTRACT:
		if (m_pSel != NULL && m_pSel->IsGroup () && m_pSel->IsExpanded () &&
			!m_pSel->IsInPlaceEditing ())
		{
			m_pSel->Expand (FALSE);
		}
		return;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
//******************************************************************************************
void CBCGPPropList::EnsureVisible (CBCGPProp* pProp)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pProp);

	if (m_nRowHeight == 0)
	{
		return;
	}

	CRect rect = pProp->m_Rect;

	if (rect.top >= m_rectList.top - 1 && rect.bottom <= m_rectList.bottom)
	{
		return;
	}

	CRect rectButton = pProp->m_rectButton;
	pProp->m_rectButton.SetRectEmpty ();
	RedrawWindow (rectButton);

	if (rect.top < m_rectList.top - 1 && rect.bottom >= m_rectList.top - 1)
	{
		OnVScroll (SB_LINEUP, 0, NULL);
	}
	else if (rect.bottom > m_rectList.bottom && rect.top <= m_rectList.bottom)
	{
		OnVScroll (SB_LINEDOWN, 0, NULL);
	}
	else
	{
		OnVScroll (SB_THUMBPOSITION, rect.top / m_nRowHeight - m_nVertScrollOffset, NULL);
	}

	if (!rectButton.IsRectEmpty ())
	{
		pProp->m_rectButton = pProp->m_Rect;
		pProp->m_rectButton.top++;
		pProp->m_rectButton.left = pProp->m_rectButton.right - pProp->m_rectButton.Height () + 2;

		RedrawWindow (pProp->m_rectButton);
	}
}
//******************************************************************************************
void CBCGPPropList::ExpandAll (BOOL bExapand/* = TRUE*/)
{
	ASSERT_VALID (this);

	if (m_bAlphabeticMode)
	{
		return;
	}

	for (POSITION pos = m_lstProps.GetHeadPosition (); pos != NULL;)
	{
		CBCGPProp* pProp = m_lstProps.GetNext (pos);
		ASSERT_VALID (pProp);

		pProp->ExpandDeep (bExapand);
	}

	AdjustLayout ();

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//******************************************************************************************
void CBCGPPropList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd::OnChar(nChar, nRepCnt, nFlags);

	if (m_pSel == NULL || !m_pSel->m_bEnabled)
	{
		return;
	}

	ASSERT_VALID (m_pSel);

	if (!EditItem (m_pSel))
	{
		return;
	}

	m_pSel->PushChar (nChar);
}
//*******************************************************************************************
HBRUSH CBCGPPropList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (m_pSel != NULL && 
		pWnd->GetSafeHwnd () == m_pSel->m_pWndInPlace->GetSafeHwnd ())
	{
		HBRUSH hbrProp = m_pSel->OnCtlColor (pDC, nCtlColor);
		if (hbrProp != NULL)
		{
			return hbrProp;
		}
	}

	return hbr;
}
//****************************************************************************************
void CBCGPPropList::UpdateColor (COLORREF color)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pSel);

	CBCGPColorProp* pColorProp = DYNAMIC_DOWNCAST(CBCGPColorProp, m_pSel);
	if (pColorProp == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bChanged = color != pColorProp->GetColor ();
	pColorProp->SetColor (color);

	if (bChanged)
	{
		OnPropertyChanged (pColorProp);
	}

	pColorProp->OnUpdateValue ();
}
//****************************************************************************************
void CBCGPPropList::CloseColorPopup ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pSel);

	CBCGPColorProp* pColorProp = DYNAMIC_DOWNCAST(CBCGPColorProp, m_pSel);
	if (pColorProp == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	pColorProp->m_pPopup = NULL;

	pColorProp->m_bButtonIsDown = FALSE;
	pColorProp->Redraw ();

	if (pColorProp->m_pWndInPlace != NULL)
	{
		pColorProp->m_pWndInPlace->SetFocus ();
	}
}
//****************************************************************************************
void CBCGPPropList::OnSelectCombo()
{
	ASSERT_VALID (this);
	
	if (m_pSel == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ASSERT_VALID (m_pSel);
	m_pSel->OnSelectCombo ();
}
//****************************************************************************************
void CBCGPPropList::OnCloseCombo()
{
	ASSERT_VALID (this);
	
	if (m_pSel == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ASSERT_VALID (m_pSel);
	m_pSel->OnCloseCombo ();
}
//****************************************************************************************
void CBCGPPropList::SetBoolLabels (LPCTSTR lpszTrue, LPCTSTR lpszFalse)
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
void CBCGPPropList::SetListDelimiter (TCHAR c)
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);	// Should be called before window create

	m_cListDelimeter = c;
}
//******************************************************************************
void CBCGPPropList::SetAlphabeticMode (BOOL bSet)
{
	ASSERT_VALID (this);

	if (m_bAlphabeticMode == bSet)
	{
		return;
	}

	m_bAlphabeticMode = bSet;
	m_nVertScrollOffset = 0;

	SetCurSel (NULL);

	if (GetSafeHwnd () != NULL)
	{
		if (m_bAlphabeticMode)
		{
			ReposProperties ();
		}

		AdjustLayout ();
		RedrawWindow ();
	}
}
//*****************************************************************************************
void CBCGPPropList::SetVSDotNetLook (BOOL bSet)
{
	ASSERT_VALID (this);
	m_bVSDotNetLook = bSet;

	if (GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//****************************************************************************************
BOOL CBCGPPropList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	BOOL bRes = CWnd::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT (pNMHDR != NULL);

	if (pNMHDR->code == TTN_SHOW)
	{
		m_ToolTip.SetWindowPos (&wndTop, -1, -1, -1, -1,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}
//****************************************************************************************
void CBCGPPropList::OnDestroy() 
{
	while (!m_lstProps.IsEmpty ())
	{
		delete m_lstProps.RemoveHead ();
	}

	m_pSel = NULL;
	m_ToolTip.DestroyWindow ();

	CWnd::OnDestroy();
}
//************************************************************************************
BOOL CBCGPPropList::OnNeedTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/)
{
	static CString strTipText;

	CPoint point;
	::GetCursorPos (&point);
	ScreenToClient (&point);

	CBCGPProp* pProp = HitTest (point);
	if (pProp == NULL)
	{
		return FALSE;
	}

	if (point.x < m_rectList.left + m_nLeftColumnWidth)
	{
		strTipText = pProp->GetNameTooltip ();
	}
	else
	{
		strTipText = pProp->GetValueTooltip ();
	}

	if (strTipText.IsEmpty ())
	{
		return FALSE;
	}

	LPNMTTDISPINFO	pTTDispInfo	= (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR) strTipText);

	m_ToolTip.SetFont (GetFont (), FALSE);
	return TRUE;
}
//****************************************************************************************
void CBCGPPropList::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (m_bTracking)
	{
		TrackHeader (point.x);
	}

	if (m_bTrackingDescr)
	{
		TrackDescr (point.y);
	}
}
//*****************************************************************************************
void CBCGPPropList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	if (m_bTracking)
	{
		TrackHeader (-1);
		m_bTracking = FALSE;
		ReleaseCapture ();

		CRect rectClient;
		GetClientRect (rectClient);

		m_nLeftColumnWidth = min (max (m_nRowHeight, point.x), rectClient.Width () - ::GetSystemMetrics (SM_CXHSCROLL) - 5);

		HDITEM hdItem;
		hdItem.mask = HDI_WIDTH ;
		hdItem.cxy = m_nLeftColumnWidth + 2;

		GetHeaderCtrl ().SetItem (0, &hdItem);

		hdItem.cxy = rectClient.Width () + 10;
		GetHeaderCtrl ().SetItem (1, &hdItem);
	}

	if (m_bTrackingDescr)
	{
		TrackDescr (INT_MIN);
		m_bTrackingDescr = FALSE;
		ReleaseCapture ();

		CRect rectClient;
		GetClientRect (rectClient);

		point.y = max (point.y, m_nRowHeight + m_nHeaderHeight);
		m_nDescrHeight = rectClient.Height () - point.y + 2;
		m_nDescrHeight = max (m_nRowHeight, m_nDescrHeight);

		AdjustLayout ();
		RedrawWindow ();
	}
}
//****************************************************************************************
void CBCGPPropList::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
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
void CBCGPPropList::OnNcPaint() 
{
	if (m_nDescrHeight == -1)
	{
		m_nDescrHeight = m_nRowHeight * 3 + m_nRowHeight / 2;
		AdjustLayout ();
	}

	if (GetStyle () & WS_BORDER)
	{
		CBCGPVisualManager::GetInstance ()->OnDrawControlBorder (this);
	}
}
//********************************************************************************
void CBCGPPropList::SetGroupNameFullWidth (BOOL bGroupNameFullWidth,
										   BOOL bRedraw/* = TRUE*/)
{
	m_bGroupNameFullWidth = bGroupNameFullWidth;

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		RedrawWindow ();
	}
}
//********************************************************************************
void CBCGPPropList::SetCustomColors (
		COLORREF	clrBackground,
		COLORREF	clrText,
		COLORREF	clrGroupBackground,
		COLORREF	clrGroupText,
		COLORREF	clrDescriptionBackground,
		COLORREF	clrDescriptionText,
		COLORREF	clrLine)
{
	m_clrBackground = clrBackground;
	m_clrText = clrText;
	m_clrGroupBackground = clrGroupBackground;
	m_clrGroupText = clrGroupText;
	m_clrDescriptionBackground = clrDescriptionBackground;
	m_clrDescriptionText = clrDescriptionText;
	m_clrLine = clrLine;

	m_brBackground.DeleteObject ();

	if (m_clrBackground != (COLORREF)-1)
	{
		m_brBackground.CreateSolidBrush (m_clrBackground);
	}
}
//********************************************************************************
void CBCGPPropList::GetCustomColors (
		COLORREF&	clrBackground,
		COLORREF&	clrText,
		COLORREF&	clrGroupBackground,
		COLORREF&	clrGroupText,
		COLORREF&	clrDescriptionBackground,
		COLORREF&	clrDescriptionText,
		COLORREF&	clrLine)
{
	clrBackground = m_clrBackground;
	clrText = m_clrText;
	clrGroupBackground = m_clrGroupBackground;
	clrGroupText = m_clrGroupText;
	clrDescriptionBackground = m_clrDescriptionBackground;
	clrDescriptionText = m_clrDescriptionText;
	clrLine = m_clrLine;
}
//*********************************************************************************
BOOL CBCGPPropList::ProcessClipboardAccelerators (UINT nChar)
{
	if (m_pSel == NULL || m_pSel->m_pWndInPlace->GetSafeHwnd () == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (m_pSel);

	BOOL bIsCtrl = (::GetAsyncKeyState (VK_CONTROL) & 0x8000);
	BOOL bIsShift = (::GetAsyncKeyState (VK_SHIFT) & 0x8000);

	if (bIsCtrl && (nChar == _T('C') || nChar == VK_INSERT))
	{
		m_pSel->m_pWndInPlace->SendMessage (WM_COPY);
		return TRUE;
	}

	if (bIsCtrl && nChar == _T('V') || (bIsShift && nChar == VK_INSERT))
	{
		m_pSel->m_pWndInPlace->SendMessage (WM_PASTE);
		return TRUE;
	}

	if (bIsCtrl && nChar == _T('X') || (bIsShift && nChar == VK_DELETE))
	{
		m_pSel->m_pWndInPlace->SendMessage (WM_CUT);
		return TRUE;
	}

	return FALSE;
}
