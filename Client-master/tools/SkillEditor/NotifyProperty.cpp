
#include "stdafx.h"
#include "NotifyProperty.h"
#include "Resource.h"
#include "ExtPropList.h"
#include "MainFrm.h"
#include "SkillEditor.h"

IMPLEMENT_DYNAMIC(CNotifiedProp, CBCGPProp);

#define PROP_HAS_LIST	0x0001
#define PROP_HAS_BUTTON	0x0002
#define PROP_HAS_SPIN	0x0004

CNotifiedProp::CNotifiedProp(const CString& strGroupName, DWORD dwData,
							 BOOL bIsValueList) 
							 : m_hNotifyTo(NULL)
							 , m_fFloatDigitRatio(1.0f)
							 , m_iIntDigitRatio(1)
							 , m_bRelated(false)
							 , m_fIncStep(0.0f)
							 , m_bEnableDigit(false)
							 , m_fMax(32766.0f)
							 , m_fMin(-32767.0f)
							 , m_bHasMax(false)
							 , m_bHasMin(false)
							 , CBCGPProp(strGroupName, dwData, bIsValueList)
{
}

CNotifiedProp::CNotifiedProp(const CString& strName, const _variant_t& varValue, 
							 LPCTSTR lpszDescr , DWORD dwData,
							 LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate,
							 LPCTSTR lpszValidChars, BOOL bHasButton) 
							 : m_hNotifyTo(NULL)
							 , m_fFloatDigitRatio(1.0f)
							 , m_iIntDigitRatio(1)
							 , m_fMax(32766.0f)
							 , m_fMin(-32767.0f)
							 , CBCGPProp(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	if(bHasButton)
	{
		m_dwFlags = PROP_HAS_BUTTON;
	}
}

CNotifiedProp::CNotifiedProp(const CString& strName, const _variant_t& varValue, 
							 BOOL bHasButton, HWND hNotifyTo)
							 :  m_fFloatDigitRatio(1.0f)
							 , m_iIntDigitRatio(1)
							 , m_fMax(32766.0f)
							 , m_fMin(-32767.0f)
							 , CBCGPProp(strName, varValue, NULL, NULL, NULL, NULL, NULL)
{
	if(bHasButton)
		m_dwFlags = PROP_HAS_BUTTON;
	m_hNotifyTo = hNotifyTo;
}


CNotifiedProp::~CNotifiedProp(void)
{
}


BOOL CNotifiedProp::OnEndEdit()
{
	//if(!CBCGPProp::OnEndEdit())
	//	return FALSE;


	if(m_hNotifyTo != NULL )
	{
		PROPNM nm;
		nm.bCancel = FALSE;
		nm.pProp = this;
		::SendMessage(m_hNotifyTo, EEN_PROP_END_EDIT, 0, (LPARAM)&nm);
		if(nm.bCancel == TRUE)
		{
			if(IsInPlaceEditing() && m_pWndInPlace != NULL)
			{
				m_pWndInPlace->SetFocus();
			}
			return FALSE;
		}
	}
	return CBCGPProp::OnEndEdit();
}

BOOL CNotifiedProp::OnEdit(LPPOINT lptClick)
{
	//m_vOldVal = GetValue();
	return CBCGPProp::OnEdit(lptClick);
}

void CNotifiedProp::OnClickButton(CPoint point)
{

	if(m_hNotifyTo != NULL)
	{
		PROPNM nm;
		nm.bCancel = FALSE;
		nm.pProp = this;
		m_bButtonIsDown = TRUE;
		Redraw ();
		::SendMessage(m_hNotifyTo, EEN_PROP_CLICK_BUTTON, 0, (LPARAM)&nm);
		m_bButtonIsDown = FALSE;
		Redraw ();
	}
	CBCGPProp::OnClickButton(point);
}

void CNotifiedProp::OnSelectCombo()
{
	CBCGPProp::OnSelectCombo();
	if(m_hNotifyTo != NULL)
	{
		PROPNM nm;
		nm.bCancel = FALSE;
		nm.pProp = this;
		::SendMessage(m_hNotifyTo, EEN_PROP_SELECT_COMBO, 0, (LPARAM)&nm);
	}
}

BOOL CNotifiedProp::OnUpdateValue()
{
	CBCGPProp::OnUpdateValue();
	if(m_hNotifyTo != NULL)
	{
		PROPNM nm;
		nm.bCancel = FALSE;
		nm.pProp = this;
		::SendMessage(m_hNotifyTo, EEN_PROP_UPDATE_VALUE, 0, (LPARAM)&nm);
	}
	return TRUE;
}

void CNotifiedProp::OnClickName(CPoint pt)
{
	_variant_t var = GetValue();
	if(var.vt == VT_R4 || var.vt == VT_I4)
		DoDigit();
}

void CNotifiedProp::DoDigit()
{
	MSG msg;
	bool bExit = false;
	POINT initpos;
	::GetCursorPos(&initpos);
	_variant_t oldval = GetValue();
	while (!bExit)
	{
		BOOL bHasMsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		CMainFrame* pMainFrame = static_cast<CMainFrame*>(theApp.m_pMainWnd);
		if(pMainFrame->GetSafeHwnd())
		{
			pMainFrame->DoFrame();
		}

		switch(msg.message)
		{
		case WM_MOUSEMOVE:
			{
				POINT curpos;
				GetCursorPos(&curpos);
				if(oldval.vt == VT_I4)
				{
					_variant_t newval;
					newval.vt = VT_I4;
					newval.intVal = (int)(oldval.intVal - (curpos.y -initpos.y) * m_fFloatDigitRatio);

					if(newval.intVal < m_fMin)
					{
						newval.intVal = (int)(m_fMin + 0.5f);
					}

					if(newval.intVal > m_fMax)
					{
						newval.intVal = (int)(m_fMax + 0.5f);
					}

					SetValue(newval);

					PROPNM nm;
					nm.bCancel = FALSE;
					nm.pProp = this;
					::SendMessage(m_hNotifyTo, EEN_PROP_UPDATE_VALUE, 0, (LPARAM)&nm);

				}
				else if(oldval.vt == VT_R4)
				{
					_variant_t newval;
					newval.vt = VT_R4;
					newval.fltVal = oldval.fltVal - (curpos.y -initpos.y) * m_fFloatDigitRatio;

					if(newval.fltVal < m_fMin)
					{
						newval.fltVal = m_fMin;
					}

					if(newval.fltVal > m_fMax)
					{
						newval.fltVal = m_fMax;
					}
					SetValue(newval);

					PROPNM nm;
					nm.bCancel = FALSE;
					nm.pProp = this;
					::SendMessage(m_hNotifyTo, EEN_PROP_UPDATE_VALUE, 0, (LPARAM)&nm);

				}
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			{
				bExit = true;
			}
			break;
		default:
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void CNotifiedProp::OnDrawName (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);
	if( !m_pWndList->IsKindOf(RUNTIME_CLASS(CExtPropList)))
	{
		CBCGPProp::OnDrawName(pDC, rect);
		return;
	}

	CExtPropList* pWndList = (CExtPropList*)m_pWndList;

	COLORREF clrTextOld = (COLORREF)-1;

	if ((IsSelected () || m_bRelated == true) && (!pWndList->m_bVSDotNetLook || !IsGroup () ||  m_bIsValueList))
	{
		CRect rectFill = rect;
		rectFill.top++;

		if ((pWndList->m_bFocused && IsSelected()))
		{//ÉîÉ«
			clrTextOld = pDC->SetTextColor (globalData.clrTextHilite);
			CBrush br(RGB(255, 0, 0));
			pDC->FillRect (rectFill, &globalData.brHilite);	
		}
		else 
		{//Ç³É«
			clrTextOld = pDC->SetTextColor (globalData.clrBtnText);
			CBrush br(RGB(0, 255, 0));
			pDC->FillRect (rectFill,
				pWndList->m_bControlBarColors ? &globalData.brBarFace : &globalData.brBtnFace);
		}
	}


	if (pWndList->m_bVSDotNetLook && IsGroup () && !m_bIsValueList)
	{
		if (pWndList->m_clrGroupText != (COLORREF)-1)
		{
			clrTextOld = pDC->SetTextColor (pWndList->m_clrGroupText);
		}
		else 
		{
			clrTextOld = pDC->SetTextColor (
				pWndList->m_bControlBarColors ? globalData.clrBarShadow : globalData.clrBtnShadow);
		}
	}

	rect.DeflateRect (TEXT_MARGIN, 0);

	if (m_pParent != NULL && ((CNotifiedProp*)m_pParent)->m_bIsValueList)
	{
		rect.left += rect.Height ();
	}

	int nTextHeight = pDC->DrawText (m_strName, rect, 
		DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

	m_bNameIsTrancated = pDC->GetTextExtent (m_strName).cx > rect.Width ();

	if (IsSelected () && pWndList->m_bVSDotNetLook && IsGroup () && !m_bIsValueList)
	{
		CRect rectFocus = rect;
		rectFocus.top = rectFocus.CenterPoint ().y - nTextHeight / 2;
		rectFocus.bottom = rectFocus.top + nTextHeight;
		rectFocus.right = 
			min (rect.right, rectFocus.left + pDC->GetTextExtent (m_strName).cx);
		rectFocus.InflateRect (2, 0);

		COLORREF clrShadow = pWndList->m_bControlBarColors ?
			globalData.clrBarShadow : globalData.clrBtnShadow;

		pDC->Draw3dRect (rectFocus, clrShadow, clrShadow);
	}

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (clrTextOld);
	}
}

// ------------------CNotifiedColorProp-------------------------------------- [6/20/2008]
IMPLEMENT_DYNAMIC(CNotifiedColorProp, CBCGPColorProp);


CNotifiedColorProp::CNotifiedColorProp(const CString& strName, const COLORREF& varValue, CPalette* pPalltte,
									   LPCTSTR lpszDescr, DWORD dwData) 
									   : m_hNotifyTo(NULL)
									   , CBCGPColorProp(strName, varValue, NULL, lpszDescr,dwData)
{

}

BOOL CNotifiedColorProp::OnUpdateValue()
{
	//CBCGPColorProp::OnUpdateValue();

	
	CString strText;
	COLORREF colorCurr = m_Color;
	if(m_pWndInPlace->GetSafeHwnd())
	{
		m_pWndInPlace->GetWindowText (strText);
		_stscanf (strText, _T("%x"), &m_Color);
	}
	/*if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged (this);
	}*/
	

	CBCGPColorProp::OnUpdateValue();
	m_pWndList->OnPropertyChanged (this);

	if(m_hNotifyTo != NULL)
	{
		PROPNM nm;
		nm.bCancel = FALSE;
		nm.pProp = this;
		::SendMessage(m_hNotifyTo, EEN_PROP_UPDATE_VALUE, 0, (LPARAM)&nm);
	}
	return TRUE;
}