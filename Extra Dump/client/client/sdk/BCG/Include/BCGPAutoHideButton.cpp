// BCGPAutoHideButton.cpp: implementation of the CBCGPAutoHideButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPAutoHideButton.h"
#include "BCGPDockingControlBar.h"
#include "BCGGlobals.h"
#include "BCGPGlobalUtils.h"
#include "BCGPVisualManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPAutoHideButton, CObject)

int CBCGPAutoHideButton::m_nBorderSize = 1;
int CBCGPAutoHideButton::m_nMarginSize = 2; // from border to icon
int CBCGPAutoHideButton::m_nTextMargin = 10;
int CBCGPAutoHideButton::m_nTextSizeNoIcon = 20;
BOOL CBCGPAutoHideButton::m_bOverlappingTabs = TRUE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPAutoHideButton::CBCGPAutoHideButton()
{
	m_bTop				= FALSE;
	m_pParentBar		= NULL;
	m_pAutoHideWindow	= NULL;
	m_dwAlignment		= 0;
	m_bVisible			= FALSE;
	m_rect.SetRectEmpty ();
}
//*************************************************************************************
CBCGPAutoHideButton::~CBCGPAutoHideButton()
{
}
//**********************************************************************************//
BOOL CBCGPAutoHideButton::Create (CBCGPAutoHideToolBar* pParentBar, 
								CBCGPDockingControlBar* pAutoHideWnd, DWORD dwAlignment)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pAutoHideWnd);
	ASSERT (dwAlignment & CBRS_ALIGN_ANY);

	m_pParentBar		= pParentBar;
	m_pAutoHideWindow	= pAutoHideWnd;
	m_dwAlignment		= dwAlignment;

	pAutoHideWnd->SetAutoHideParents (pParentBar, this);

	CSize size = GetSize ();
	m_rect.SetRect (0, 0, size.cx, size.cy);

	m_bVisible = TRUE;

	return TRUE;
}
//**********************************************************************************//
void CBCGPAutoHideButton::Move (int nOffset)
{
	ASSERT_VALID (this);
	// nOffset in pixels
	IsHorizontal () ? m_rect.OffsetRect (nOffset, 0) : m_rect.OffsetRect (0, nOffset);
}
//**********************************************************************************//
CSize CBCGPAutoHideButton::GetSize () const
{
	ASSERT_VALID (this);
	CSize size (m_nMarginSize + 2 * m_nBorderSize, m_nMarginSize + 2 * m_nBorderSize);

	if (m_pAutoHideWindow != NULL)
	{
		BOOL bHorz = IsHorizontal ();
		HICON hIcon = m_pAutoHideWindow->GetBarIcon (FALSE);
		CSize sizeText = GetTextSize ();

		CSize sizeIcon (0, 0);
		if (hIcon != NULL)
		{
			sizeIcon = globalData.m_sizeSmallIcon;
		}

		int nSpacing = 0;
		if (hIcon != NULL && 
			(sizeText.cx > 0 && bHorz || sizeText.cy > 0 && !bHorz))
		{
			nSpacing = m_nTextMargin;
			nSpacing += IsHorizontal () ? sizeIcon.cx : sizeIcon.cy; 
		}
		else if (hIcon == NULL)
		{
			nSpacing += m_nMarginSize + m_nTextSizeNoIcon;
		}
		else 
		{
			nSpacing += m_nMarginSize + 1;
		}

		if (bHorz)
		{
			size.cx += sizeIcon.cx + sizeText.cx + nSpacing;
			size.cy += max (sizeIcon.cy, sizeText.cy);
		}
		else
		{
			size.cx += max (sizeIcon.cx, sizeText.cx);
			size.cy += sizeIcon.cy + sizeText.cy + nSpacing;
		}
	}
	return size;
}
//**********************************************************************************//
void CBCGPAutoHideButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);

	CSize size = GetSize ();
	m_rect.SetRect (0, 0, size.cx, size.cy);

	CRect rectDraw = m_rect;

	OnFillBackground (pDC, m_rect);

	// calculate border size and draw the border
	CRect rectBorderSize (m_nBorderSize, 0, m_nBorderSize, m_nBorderSize);

	switch (m_dwAlignment & CBRS_ALIGN_ANY)
	{
	case CBRS_ALIGN_RIGHT:
		globalUtils.FlipRect (rectBorderSize, 90);
		break;
	case CBRS_ALIGN_BOTTOM:
		globalUtils.FlipRect (rectBorderSize, 180);
		break;
	case CBRS_ALIGN_LEFT:
		globalUtils.FlipRect (rectBorderSize, -90);
		break;
	}
	
	OnDrawBorder (pDC, m_rect, rectBorderSize);

	rectDraw.DeflateRect (rectBorderSize);
	
	if (m_pAutoHideWindow == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pAutoHideWindow);

	rectDraw.DeflateRect (m_nMarginSize, m_nMarginSize);

	if (m_pAutoHideWindow != NULL)
	{
		// draw the icon (if any)
		HICON hIcon = m_pAutoHideWindow->GetBarIcon (FALSE);
		if (hIcon != NULL)
		{
			CSize sizeIcon (globalData.m_sizeSmallIcon);

			int dx = IsHorizontal () ? 0 : (rectDraw.Width () - sizeIcon.cx) / 2;
			int dy = IsHorizontal () ? (rectDraw.Height () - sizeIcon.cy) / 2 : 0;

			::DrawIconEx (pDC->GetSafeHdc (), 
						rectDraw.left + dx, rectDraw.top + dy, hIcon,
						sizeIcon.cx, sizeIcon.cy, 0, NULL, DI_NORMAL);

			if (IsHorizontal ())
			{
				rectDraw.left += m_nTextMargin + sizeIcon.cx;
			}
			else
			{
				rectDraw.top += m_nTextMargin + sizeIcon.cy;
			}
		}
		else
		{
			if (IsHorizontal ())
			{
				rectDraw.left += m_nMarginSize;
			}
			else
			{
				rectDraw.top += m_nMarginSize;
			}
		}

		// Draw text:
		CString strText;
		m_pAutoHideWindow->GetWindowText (strText);

		if (!strText.IsEmpty () && m_pParentBar->m_bActiveInGroup ||
			 hIcon == NULL || !m_bOverlappingTabs)
		{
			int nOldMode = pDC->SetBkMode (TRANSPARENT);

			CFont* pFontOld = (CFont*) pDC->SelectObject (IsHorizontal () ? 
										&globalData.fontRegular : &globalData.fontVert);
			ASSERT(pFontOld != NULL);

			pDC->SetTextColor (globalData.clrBarText);

			if (IsHorizontal ())
			{
				pDC->DrawText (strText, &rectDraw, DT_SINGLELINE | DT_VCENTER);
			}
			else
			{
				TEXTMETRIC tm;
				pDC->GetTextMetrics (&tm);

				CRect rectText = rectDraw;

				rectText.left = rectText.right - (rectDraw.Width () - tm.tmHeight + 1) / 2;
				rectText.bottom = rectDraw.top + m_nTextMargin;

				pDC->DrawText (strText, &rectText, DT_SINGLELINE | DT_VCENTER | DT_NOCLIP);
			}

			pDC->SelectObject (pFontOld);
			pDC->SetBkMode (nOldMode);
		}
	}
}
//**********************************************************************************
void CBCGPAutoHideButton::OnFillBackground (CDC* pDC, CRect rect)
{
	CBCGPVisualManager::GetInstance ()->OnFillAutoHideButtonBackground (pDC, rect, this);
}
//**********************************************************************************
void CBCGPAutoHideButton::OnDrawBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize)
{
	CBCGPVisualManager::GetInstance ()->OnDrawAutoHideButtonBorder (pDC, rectBounds, rectBorderSize, this);
}
//**********************************************************************************
CSize CBCGPAutoHideButton::GetTextSize () const
{
	CSize size (0, 0);

	if (m_pAutoHideWindow != NULL && m_pParentBar != NULL)
	{
		CString strText;
		m_pAutoHideWindow->GetWindowText (strText);	

		if (!strText.IsEmpty ())
		{
			CWindowDC dc (m_pParentBar);
			

			CFont* pFontOld = (CFont*) dc.SelectObject (IsHorizontal () ? 
										&globalData.fontRegular : &globalData.fontVert);
			ASSERT (pFontOld != NULL);
			size = dc.GetTextExtent (strText);
			size.cx += m_nMarginSize;
			size.cy += m_nMarginSize;

			dc.SelectObject (pFontOld);

			if (!IsHorizontal ())
			{
				int n = size.cy;
				size.cy = size.cx;
				size.cx = n;
			}

		}

		if (!m_pParentBar->m_bActiveInGroup && m_bOverlappingTabs)
		{
			IsHorizontal () ? size.cx = 0 : size.cy = 0;
		}	
	}
	return size;
}
//**********************************************************************************
BOOL CBCGPAutoHideButton::IsHorizontal () const
{
	return (m_dwAlignment & CBRS_ALIGN_TOP || m_dwAlignment & CBRS_ALIGN_BOTTOM);
}
//**********************************************************************************
void CBCGPAutoHideButton::ShowAttachedWindow (BOOL bShow)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pAutoHideWindow);
	
	// conllapse window out only if it is visible
	// expand window only id it is not visible

	if (bShow && !m_pAutoHideWindow->IsWindowVisible () ||
		!bShow && m_pAutoHideWindow->IsWindowVisible ())
	{
		m_pAutoHideWindow->Slide (bShow);
	}

	m_pParentBar->SetActiveInGroup (bShow);

}
//**********************************************************************************
void CBCGPAutoHideButton::ShowButton (BOOL bShow)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParentBar);

	m_bVisible = bShow;
}
//**********************************************************************************
void CBCGPAutoHideButton::UnSetAutoHideMode (CBCGPDockingControlBar* pFirstBarInGroup)
{
	if (m_pAutoHideWindow != NULL)
	{
		m_pAutoHideWindow->UnSetAutoHideMode (pFirstBarInGroup);
	}
}
