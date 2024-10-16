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

// BCGPVisualManager2003.cpp: implementation of the CBCGPVisualManager2003 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "multimon.h"
#include "bcgcbpro.h"
#include "BCGPVisualManager2003.h"
#include "BCGPDrawManager.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPMenuBar.h"
#include "bcgglobals.h"
#include "BCGPToolbarMenuButton.h"
#include "CustomizeButton.h"
#include "MenuImages.h"
#include "BCGPCaptionBar.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPColorBar.h"
#include "BCGPCalculator.h"
#include "BCGPCalendarBar.h"
#include "BCGPTabWnd.h"
#include "BCGPTasksPane.h"
#include "BCGPStatusBar.h"
#include "BCGPAutoHideButton.h"
#include "BCGPHeaderCtrl.h"
#include "BCGPReBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPVisualManager2003, CBCGPVisualManagerXP)

BOOL CBCGPVisualManager2003::m_bUseGlobalTheme = TRUE;
BOOL CBCGPVisualManager2003::m_bStatusBarOfficeXPLook = TRUE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManager2003::CBCGPVisualManager2003()
{
	m_bShadowHighlightedImage = FALSE;
	m_bFadeInactiveImage = FALSE;
	m_nMenuShadowDepth = 3;

	m_nVertMargin = 8;
	m_nHorzMargin = 8;
	m_nGroupVertOffset = 8;
	m_nGroupCaptionHeight = 18;
	m_nGroupCaptionHorzOffset = 3;
	m_nGroupCaptionVertOffset = 3;
	m_nTasksHorzOffset = 8;
	m_nTasksIconHorzOffset = 5;
	m_nTasksIconVertOffset = 4;
	m_bActiveCaptions = TRUE;
	
	OnUpdateSystemColors ();
}
//****************************************************************************************
CBCGPVisualManager2003::~CBCGPVisualManager2003()
{
}
//****************************************************************************************
void CBCGPVisualManager2003::DrawCustomizeButton (CDC* pDC, CRect rect, BOOL bIsHorz,
						  CBCGPVisualManager::BCGBUTTON_STATE state,
						  BOOL bIsCustomize, BOOL bIsMoreButtons)
{
	ASSERT_VALID (pDC);

    COLORREF clrDark = state == ButtonsIsRegular ?
		m_clrCustomizeButtonGradientDark : m_clrHighlightGradientDark;

	COLORREF clrLight = state == ButtonsIsRegular ?
		m_clrCustomizeButtonGradientLight : m_clrHighlightGradientLight;

	COLORREF clrLightShadow = CBCGPDrawManager::PixelAlpha (clrLight, 90);

	CPen penDark (PS_SOLID, 1, clrDark);
	CPen penLight (PS_SOLID, 1, clrLight);

	CPen* pOldPen = pDC->SelectObject (&penDark);
	ASSERT (pOldPen != NULL);

	const int nEllipse = 2;
	for (int i = 0; i < nEllipse; i++)
	{
		if (bIsHorz)
		{
			int x1 = rect.left + i;
			int x2 = rect.right - nEllipse + i;
			int y = rect.top + i;

			pDC->SelectObject (&penLight);

			pDC->MoveTo (x1, y);
			pDC->LineTo (x2, y);

			pDC->SetPixel (x1, y, clrLightShadow);
			pDC->SetPixel (x2, y, clrLightShadow);

			pDC->SelectObject (&penDark);

			y = rect.bottom - i - 1;

			pDC->MoveTo (x1, y);
			pDC->LineTo (x2, y);

			pDC->SetPixel (x1, y, clrLight);
			pDC->SetPixel (x2, y, clrLight);
		}
		else
		{
			int y1 = rect.top + i;
			int y2 = rect.bottom - nEllipse + i;
			int x = rect.left + i;

			pDC->SelectObject (&penDark);

			pDC->MoveTo (x, y1);
			pDC->LineTo (x, y2);

			pDC->SetPixel (x, y1, clrLight);
			pDC->SetPixel (x, y2, clrLight);

			pDC->SelectObject (&penLight);

			x = rect.right - i - 1;

			pDC->MoveTo (x, y1);
			pDC->LineTo (x, y2);

			pDC->SetPixel (x, y1, clrLightShadow);
			pDC->SetPixel (x, y1, clrLightShadow);
		}
	}

	pDC->SelectObject (pOldPen);

	if (bIsHorz)
	{
		rect.DeflateRect (0, nEllipse);
		rect.left += nEllipse;
	}
	else
	{
		rect.DeflateRect (nEllipse, 0);
		rect.top += nEllipse;
	}

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, clrDark, clrLight, bIsHorz);

	//---------------------
	// Draw button content:
	//---------------------
	const int nMargin = GetToolBarCustomizeButtonMargin ();

	CSize sizeImage = CMenuImages::Size ();

	if (bIsCustomize)
	{
		//-----------------
		// Draw menu image:
		//-----------------
		CRect rectMenu = rect;
		if (bIsHorz)
		{
			rectMenu.top = rectMenu.bottom - sizeImage.cy - 2 * nMargin;
		}
		else
		{
			rectMenu.left = rectMenu.right - sizeImage.cx - 2 * nMargin;
		}

		rectMenu.DeflateRect (
			(rectMenu.Width () - sizeImage.cx) / 2,
			(rectMenu.Height () - sizeImage.cy) / 2);

		rectMenu.OffsetRect (1, 1);

		CMenuImages::IMAGES_IDS id = bIsHorz ? 
			CMenuImages::IdCustomizeArowDown : CMenuImages::IdCustomizeArowLeft;

		CMenuImages::Draw (	pDC, id, rectMenu.TopLeft (),
			CMenuImages::ImageWhite);

		rectMenu.OffsetRect (-1, -1);

		CMenuImages::Draw (	pDC, id, rectMenu.TopLeft ());
	}

	if (bIsMoreButtons)
	{
		//-------------------
		// Draw "more" image:
		//-------------------
		CRect rectMore = rect;
		if (bIsHorz)
		{
			rectMore.bottom = rectMore.top + sizeImage.cy + 2 * nMargin;
		}
		else
		{
			rectMore.right = rectMore.left + sizeImage.cx + 2 * nMargin;
		}

		rectMore.DeflateRect (
			(rectMore.Width () - sizeImage.cx) / 2,
			(rectMore.Height () - sizeImage.cy) / 2);

		CMenuImages::IMAGES_IDS id = 
			bIsHorz ? 
				CMenuImages::IdCustomizeMoreButtonsHorz : 
				CMenuImages::IdCustomizeMoreButtonsVert;

		rectMore.OffsetRect (1, 1);
		CMenuImages::Draw (pDC, id, rectMore.TopLeft (), CMenuImages::ImageWhite);

		rectMore.OffsetRect (0, -1);
		CMenuImages::Draw (pDC, id, rectMore.TopLeft ());
	}
}
//***********************************************************************************
void CBCGPVisualManager2003::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
								CRect rectClient, CRect rectClip,
								BOOL bNCArea)
{
	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	if (DYNAMIC_DOWNCAST (CBCGPReBar, pBar) != NULL ||
		DYNAMIC_DOWNCAST (CBCGPReBar, pBar->GetParent ()))
	{
		if (rectClip.IsRectEmpty ())
		{
			rectClip = rectClient;
		}

		pDC->FillRect (rectClip, &globalData.brBtnFace);
		return;
	}

	CRuntimeClass* pBarClass = pBar->GetRuntimeClass ();

	if (globalData.m_nBitsPerPixel <= 8 ||
		pBar->IsDialogControl () ||
		pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPColorBar)) ||
		pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalculator)) ||
		pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalendarBar)))
	{
		CBCGPVisualManagerXP::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		return;
	}

	if (m_bStatusBarOfficeXPLook &&
		pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPStatusBar)))
	{
		pDC->FillRect (	rectClip.IsRectEmpty () ? rectClient : rectClip, 
						&globalData.brBtnFace);
		return;
	}

	if (rectClip.IsRectEmpty ())
	{
		rectClip = rectClient;
	}

	CBCGPDrawManager dm (*pDC);

    if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPCaptionBar)))
	{
		dm.FillGradient (rectClient, 
			m_clrCaptionBarGradientDark, m_clrCaptionBarGradientLight, TRUE);
		return;
	}

    if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		BOOL bQuickMode = FALSE;

		CBCGPPopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pBar);
		if (!pMenuBar->m_bDisableSideBarInXPMode)
		{
			CWnd* pWnd = pMenuBar->GetParent();

			if(pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CBCGPPopupMenu)))
			{
				CBCGPPopupMenu* pMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, pWnd);

				if(pMenu->IsCustomizePane())
				{
					bQuickMode = TRUE;
				}
			}

			CRect rectImages = rectClient;
			rectImages.DeflateRect (0, 1);

			if (bQuickMode)
			{
				rectImages.right = rectImages.left + 2*CBCGPToolBar::GetMenuImageSize ().cx + 
							4 * GetMenuImageMargin () + 4;
			}
			else
			{
				rectImages.right = rectImages.left + CBCGPToolBar::GetMenuImageSize ().cx + 
							2 * GetMenuImageMargin () + 1;
			}

			dm.FillGradient (rectImages, m_clrToolBarGradientLight, m_clrToolBarGradientDark, FALSE);
		}

		return;
	}

	BOOL bIsHorz = (pBar->GetBarStyle () & CBRS_ORIENT_HORZ);
	BOOL bIsToolBar = pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)) &&
						!pBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar));

	COLORREF clr1 = bIsHorz ? m_clrToolBarGradientDark : m_clrToolBarGradientVertLight;
	COLORREF clr2 = bIsHorz ? m_clrToolBarGradientLight : m_clrToolBarGradientVertDark;

	if (!bIsToolBar)
	{
		bIsHorz = FALSE;

		clr1 = m_clrBarGradientDark;
		clr2 = m_clrBarGradientLight;

		CRect rectScreen;

		MONITORINFO mi;
		mi.cbSize = sizeof (MONITORINFO);
		if (GetMonitorInfo (MonitorFromPoint (CPoint (rectClient.right, rectClient.top), 
			MONITOR_DEFAULTTONEAREST), &mi))
		{
			rectScreen = mi.rcWork;
		}
		else
		{
			::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
		}

		rectClient.right = rectScreen.right;
	}

	BOOL bRoundedCorners = FALSE;

	CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, pBar);
	if (bNCArea && pToolBar != NULL && pToolBar->IsDocked () &&
		!pToolBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
	{
		bRoundedCorners = TRUE;

		CBCGPBaseControlBar* pParentBar = DYNAMIC_DOWNCAST (CBCGPBaseControlBar,
			pBar->GetParent ());

		if (pParentBar != NULL)
		{
			CPoint pt (0, 0);
			pBar->MapWindowPoints (pParentBar, &pt, 1);
			pt = pDC->OffsetWindowOrg (pt.x, pt.y);

			CRect rectParent;
			pParentBar->GetClientRect (rectParent);

			OnFillBarBackground (pDC, pParentBar, rectParent, rectParent);

			pDC->SetWindowOrg(pt.x, pt.y);
		}

		CRect rectFill = rectClient;
		rectFill.DeflateRect (1, 0);

		dm.FillGradient (rectFill, clr1, clr2, bIsHorz);

		CRect rectLeft = rectClient;
		rectLeft.top ++;
		rectLeft.right = rectLeft.left + 1;

		dm.FillGradient (rectLeft, clr1, clr2, bIsHorz);

		CRect rectRight = rectClient;
		rectLeft.top ++;
		rectRight.left = rectRight.right - 1;

		dm.FillGradient (rectRight, clr1, clr2, bIsHorz);
	}
	else
	{
		CRect rectFill = rectClient;

		if (bIsToolBar && pBar->IsDocked ())
		{
			ASSERT_VALID (pToolBar);

			rectFill.left -= pToolBar->m_cxLeftBorder;
			rectFill.right += pToolBar->m_cxRightBorder;
			rectFill.top -= pToolBar->m_cyTopBorder;
			rectFill.bottom += pToolBar->m_cyBottomBorder;
		}

		dm.FillGradient (rectFill, clr1, clr2, bIsHorz);
	}

	if (bNCArea)
	{
		if (bRoundedCorners)
		{
			//------------------------
			// Draw bottom/right edge:
			//------------------------
			CPen pen (PS_SOLID, 1, m_clrToolBarBottomLine);
			CPen* pOldPen = pDC->SelectObject (&pen);
			ASSERT (pOldPen != NULL);

			CRect rectFill = rectClient;

			if (bIsHorz)
			{
				pDC->MoveTo (rectClient.left + 2, rectClient.bottom - 1);
				pDC->LineTo (rectClient.right - 1, rectClient.bottom - 1);

				rectFill.left = rectFill.right - 1;
				rectFill.DeflateRect (0, 1);
			}
			else
			{
				pDC->MoveTo (rectClient.right - 1, rectClient.top + 2);
				pDC->LineTo (rectClient.right - 1, rectClient.bottom - 1);

				rectFill.top = rectFill.bottom + 1;
				rectFill.DeflateRect (1, 0);
			}

			pDC->SelectObject (pOldPen);

			dm.FillGradient (rectFill, m_clrToolBarBottomLine, m_clrToolBarGradientDark, bIsHorz);

			//----------------------
			// Draw rounded corners:
			//----------------------
			pDC->SetPixel (rectClient.left + 1, rectClient.top, m_colorToolBarCornerTop);
			pDC->SetPixel (rectClient.left, rectClient.top + 1, m_colorToolBarCornerTop);

			pDC->SetPixel (rectClient.right - 2, rectClient.top, m_colorToolBarCornerTop);
			pDC->SetPixel (rectClient.right - 1, rectClient.top + 1, m_colorToolBarCornerTop);

			pDC->SetPixel (rectClient.left + 1, rectClient.bottom - 2, m_colorToolBarCornerBottom);
			pDC->SetPixel (rectClient.right - 2, rectClient.bottom - 2, m_colorToolBarCornerBottom);
		}

		if (pToolBar != NULL && pToolBar->GetCount () > 0)
		{
			CCustomizeButton* pButton = 
				DYNAMIC_DOWNCAST (CCustomizeButton, pToolBar->GetButton (pToolBar->GetCount () - 1));
			if (pButton != NULL && !pButton->Rect ().IsRectEmpty () &&
				pButton->IsPipeStyle ())
			{
				//----------------------------------------
				// Special drawing for "Customize" button:
				//----------------------------------------
				CRect rectWindow;
				pBar->GetWindowRect (rectWindow);

				CRect rectCustomizeButton = pButton->Rect ();
				pBar->ClientToScreen (&rectCustomizeButton);

				CRect rectButton = rectClient;

				if (pToolBar->IsHorizontal ())
				{
					rectButton.left = rectButton.right - pButton->Rect ().Width () - 
						rectWindow.right + rectCustomizeButton.right;

					pButton->SetExtraSize (
						0,
						rectWindow.bottom - rectCustomizeButton.bottom);
				}
				else
				{
					rectButton.top = rectButton.bottom - pButton->Rect ().Height () - 
						rectWindow.bottom + rectCustomizeButton.bottom;
					pButton->SetExtraSize (
						rectWindow.right - rectCustomizeButton.right,
						0);
				}

				BCGBUTTON_STATE state = ButtonsIsRegular;

				if (pToolBar->IsButtonHighlighted (pToolBar->GetCount () - 1) ||
					pButton->IsDroppedDown ())
				{
					state = ButtonsIsHighlighted;
				}
				else if (pButton->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
				{
					//-----------------------
					// Pressed in or checked:
					//-----------------------
					state = ButtonsIsPressed;
				}

				DrawCustomizeButton (pDC, rectButton,
					pToolBar->IsHorizontal (), state,
					(int) pButton->GetCustomizeCmdId () > 0,
					!pButton->GetInvisibleButtons ().IsEmpty ());
			}
		}
	}
}
//****************************************************************************************
void CBCGPVisualManager2003::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	ASSERT_VALID (pBar);

	if (pBar->IsDialogControl () || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawBarBorder (pDC, pBar, rect);
	}
}
//***************************************************************************************
void CBCGPVisualManager2003::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz,
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pDC);

	if (pBar != NULL && pBar->IsDialogControl () ||
		globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawBarGripper (pDC, rectGripper, bHorz, pBar);
		return;
	}

	const int nBoxSize = 4;

	if (bHorz)
	{
		rectGripper.left = rectGripper.right - nBoxSize;
	}
	else
	{
		rectGripper.top = rectGripper.bottom - nBoxSize;
	}

	CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, pBar);
	if (pToolBar != NULL)
	{
		if (bHorz)
		{
			const int nDelta = max (0, (pToolBar->GetButtonSize ().cy - pToolBar->GetImageSize ().cy) / 2);
			rectGripper.DeflateRect (0, nDelta);
		}
		else
		{
			const int nDelta = max (0, (pToolBar->GetButtonSize ().cx - pToolBar->GetImageSize ().cx) / 2);
			rectGripper.DeflateRect (nDelta, 0);
		}
	}

	const int nBoxesNumber = bHorz ?
		(rectGripper.Height () - nBoxSize) / nBoxSize : 
		(rectGripper.Width () - nBoxSize) / nBoxSize;

	int nOffset = bHorz ? 
		(rectGripper.Height () - nBoxesNumber * nBoxSize) / 2 :
		(rectGripper.Width () - nBoxesNumber * nBoxSize) / 2;

	for (int nBox = 0; nBox < nBoxesNumber; nBox++)
	{
		int x = bHorz ? 
			rectGripper.left : 
			rectGripper.left + nOffset;

		int y = bHorz ? 
			rectGripper.top + nOffset : 
			rectGripper.top;

		pDC->FillSolidRect (x + 1, y + 1, nBoxSize / 2, nBoxSize / 2, 
			globalData.clrBtnHilite);
		pDC->FillSolidRect (x, y, nBoxSize / 2, nBoxSize / 2, 
			m_clrGripper);

		nOffset += nBoxSize;
	}
}
//**************************************************************************************
void CBCGPVisualManager2003::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawComboBorder (pDC, rect,
												bDisabled,
												bIsDropped,
												bIsHighlighted,
												pButton);
		return;
	}

	if (bIsHighlighted || bIsDropped || bDisabled)
	{
		rect.DeflateRect (1, 1);

		COLORREF colorBorder = bDisabled ? globalData.clrBtnShadow : m_clrMenuItemBorder;
		pDC->Draw3dRect (&rect, colorBorder, colorBorder);
	}
}
//*********************************************************************************
void CBCGPVisualManager2003::OnFillOutlookPageButton (CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillOutlookPageButton (pDC, rect,
										bIsHighlighted, bIsPressed,
										clrText);
		return;
	}

	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	if (bIsPressed)
	{
		dm.FillGradient (rect,	m_clrHighlightDnGradientDark,
								m_clrHighlightDnGradientLight,
								TRUE);
	}
	else if (bIsHighlighted)
	{
		dm.FillGradient (rect,	m_clrHighlightGradientDark,
								m_clrHighlightGradientLight,
								TRUE);
	}
	else
	{
		dm.FillGradient (rect,	m_clrBarGradientDark,
								m_clrBarGradientLight,
								TRUE);
	}

	clrText = globalData.clrBtnText;
}
//****************************************************************************************
void CBCGPVisualManager2003::OnDrawOutlookPageButtonBorder (CDC* pDC, 
							CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawOutlookPageButtonBorder (pDC, 
							rectBtn, bIsHighlighted, bIsPressed);
		return;
	}

	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rectBtn, globalData.clrBtnHilite, m_clrGripper);
}
//**********************************************************************************
void CBCGPVisualManager2003::OnFillButtonInterior (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST (CCustomizeButton, pButton);
	if (pCustButton == NULL || !pCustButton->IsPipeStyle () || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	CBCGPToolBar* pToolBar = pCustButton->GetParentToolbar ();
	if (pToolBar != NULL)
	{
		ASSERT_VALID (pToolBar);

		CRect rectToolbar;
		pToolBar->GetClientRect (rectToolbar);

		if (pToolBar->IsHorizontal ())
		{
			rect.right = rectToolbar.right;
		}
		else
		{
			rect.bottom = rectToolbar.bottom;
		}
	}

	CSize sizeExtra = pCustButton->GetExtraSize ();

	rect.InflateRect (sizeExtra);
	DrawCustomizeButton (pDC, rect, pToolBar->IsHorizontal (), state,
		(int) pCustButton->GetCustomizeCmdId () > 0,
		!pCustButton->GetInvisibleButtons ().IsEmpty ());

	pCustButton->SetDefaultDraw (FALSE);
}
//**************************************************************************************
void CBCGPVisualManager2003::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST (CCustomizeButton, pButton);
	if (pCustButton == NULL || !pCustButton->IsPipeStyle () || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawButtonBorder (pDC, pButton, rect, state);
	}

	// Do nothing - the border is already painted in OnFillButtonInterior
}
//**************************************************************************************
void CBCGPVisualManager2003::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pBar);

	if (pBar->IsDialogControl () ||
		pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)) ||
		globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, pBar);
	if (pToolBar == NULL)
	{
		CBCGPVisualManagerXP::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
	ASSERT (pOldPen != NULL);

	if (bHorz)
	{
		const int nDelta = max (0, (pToolBar->GetButtonSize ().cy - pToolBar->GetImageSize ().cy) / 2);
		rect.DeflateRect (0, nDelta);

		int x = rect.left += rect.Width () / 2 - 1;

		pDC->MoveTo (x, rect.top);
		pDC->LineTo (x, rect.bottom - 1);

		pDC->SelectObject (&m_penSeparatorLight);

		pDC->MoveTo (x + 1, rect.top + 1);
		pDC->LineTo (x + 1, rect.bottom);

	}
	else
	{
		const int nDelta = max (0, (pToolBar->GetButtonSize ().cx - pToolBar->GetImageSize ().cx) / 2);
		rect.DeflateRect (nDelta, 0);

		int y = rect.top += rect.Height () / 2 - 1;

		pDC->MoveTo (rect.left, y);
		pDC->LineTo (rect.right - 1, y);

		pDC->SelectObject (&m_penSeparatorLight);

		pDC->MoveTo (rect.left + 1, y + 1);
		pDC->LineTo (rect.right, y + 1);
	}

	pDC->SelectObject (pOldPen);
}
//***********************************************************************************
void CBCGPVisualManager2003::OnUpdateSystemColors ()
{
	CBCGPWinXPThemeManager::UpdateSystemColors ();

	m_bIsStandardWinXPTheme = FALSE;
	m_strWinXPThemeColor.Empty ();

	BOOL bIsOliveGreenTheme = FALSE;
	BOOL bIsSilverTheme = FALSE;

	BOOL bIsAppThemed = m_bUseGlobalTheme || (m_pfGetWindowTheme != NULL && 
		(*m_pfGetWindowTheme) (AfxGetMainWnd ()->GetSafeHwnd ()) != NULL);

	//------------------------
	// Get current theme name:
	//------------------------
	if (m_pfGetCurrentThemeName != NULL && bIsAppThemed)
	{
		WCHAR szName [256];
		WCHAR szColor [256];

		if ((*m_pfGetCurrentThemeName) (szName, 255, szColor, 255, NULL, 0) == S_OK)
		{
			CString strThemeName = szName;
			m_strWinXPThemeColor = szColor;

			TCHAR fname[_MAX_FNAME];   
			_tsplitpath (strThemeName, NULL, NULL, fname, NULL);

			strThemeName = fname;
			m_bIsStandardWinXPTheme = (strThemeName.CompareNoCase (_T("Luna")) == 0);

			bIsOliveGreenTheme = m_bIsStandardWinXPTheme &&
				m_strWinXPThemeColor.CompareNoCase (_T("homestead")) == 0;

			bIsSilverTheme = m_bIsStandardWinXPTheme &&
				m_strWinXPThemeColor.CompareNoCase (_T("metallic")) == 0;
		}
	}

	//----------------------
	// Modify global colors:
	//----------------------
	ModifyGlobalColors ();

	CBCGPVisualManagerXP::OnUpdateSystemColors ();

	if (globalData.m_nBitsPerPixel <= 8)
	{
		m_clrTaskPaneGradientDark  = globalData.clrWindow;
		m_clrTaskPaneGradientLight  = globalData.clrWindow;
		m_clrTaskPaneGroupCaptionDark  = globalData.clrBarFace;
		m_clrTaskPaneGroupCaptionLight  = globalData.clrBarFace;
		m_clrTaskPaneGroupCaptionSpecDark  = globalData.clrBarFace;
		m_clrTaskPaneGroupCaptionSpecLight  = globalData.clrBarFace;
		m_clrTaskPaneGroupAreaLight  = globalData.clrWindow;
		m_clrTaskPaneGroupAreaDark  = globalData.clrWindow;
		m_clrTaskPaneGroupAreaSpecLight  = globalData.clrWindow;
		m_clrTaskPaneGroupAreaSpecDark  = globalData.clrWindow;
		m_clrTaskPaneGroupBorder  = globalData.clrBtnShadow;

		m_clrBarGradientLight = m_clrToolBarGradientLight = globalData.clrBarLight;

		m_penTaskPaneGroupBorder.DeleteObject ();
		m_penTaskPaneGroupBorder.CreatePen (PS_SOLID, 1, m_clrTaskPaneGroupBorder);

		return;
	}

	//--------------------------------------------------
	// Calculate control bars bakground gradient colors:
	//--------------------------------------------------
	COLORREF clrBase = GetThemeColor (m_hThemeWindow, /*COLOR_MENUHILIGHT*/29);
	if (!m_bIsStandardWinXPTheme)
	{
		clrBase = globalData.clrBarFace;
	}

	if (bIsOliveGreenTheme)
	{
		m_clrToolBarGradientDark = CBCGPDrawManager::PixelAlpha (
			clrBase, 120);

		m_clrBarGradientDark = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_3DFACE),
			.87, 1, 3);

		m_clrToolBarGradientLight = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_WINDOW),
			1., 2, 1);

		m_clrBarGradientLight = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_WINDOW),
			1.03);
	}
	else if (bIsSilverTheme)
	{
		m_clrToolBarGradientDark = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_3DFACE),
			0.75, 2);

		m_clrBarGradientDark = CBCGPDrawManager::PixelAlpha (
			clrBase, 120);

		m_clrToolBarGradientLight = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_WINDOW),
			1.03);

		m_clrBarGradientLight = m_clrToolBarGradientLight;
	}
	else	// Normal "blue" theme
	{
		m_clrToolBarGradientDark = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_3DFACE),
			0.93, 2);

		m_clrBarGradientDark = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_3DLIGHT),
			.99, 2, 1);

		m_clrToolBarGradientLight = CBCGPDrawManager::SmartMixColors (
			clrBase, 
			GetThemeColor (m_hThemeWindow, COLOR_WINDOW),
			1.03);

		m_clrBarGradientLight = m_clrToolBarGradientLight;
	}

	m_clrToolBarGradientVertLight = m_clrToolBarGradientLight;

	m_clrToolBarGradientVertDark = CBCGPDrawManager::PixelAlpha (
			m_clrToolBarGradientDark, 98);

	COLORREF clrSeparatorDark;

	//-------------------------------------
	// Calculate highlight gradient colors:
	//-------------------------------------
	if (m_bIsStandardWinXPTheme)
	{
		ASSERT (m_pfGetThemeColor != NULL);

		COLORREF clr1, clr2, clr3;

		(*m_pfGetThemeColor) (m_hThemeButton, 1, 0, 3823, &clr1);
		(*m_pfGetThemeColor) (m_hThemeButton, 2, 0, 3823, &clr2);
		(*m_pfGetThemeColor) (m_hThemeWindow, 18, 0, 3821, &clr3);

		m_clrHighlightMenuItem = CBCGPDrawManager::SmartMixColors (
			clr1, 
			clr2,
			1.3, 1, 1);

		m_clrHighlightGradientLight = CBCGPDrawManager::SmartMixColors (
			clr1, 
			clr3,
			1.55, 2, 1);

		m_clrHighlightGradientDark = CBCGPDrawManager::SmartMixColors (
			clr1, 
			clr2,
			1.03, 2, 1);

		m_clrHighlightDnGradientLight = CBCGPDrawManager::SmartMixColors (
			clr1, 
			clr3,
			1.03, 1, 2);

		m_brFloatToolBarBorder.DeleteObject ();

		COLORREF clrCustom;
		(*m_pfGetThemeColor) (m_hThemeButton, 2, 0, 3822, &clrCustom);

		COLORREF clrToolbarBorder = CBCGPDrawManager::SmartMixColors (
			clrCustom, 
			clrBase,
			0.84, 1, 4);
		m_brFloatToolBarBorder.CreateSolidBrush (clrToolbarBorder);

		m_clrCustomizeButtonGradientDark = CBCGPDrawManager::SmartMixColors (
			clrCustom, 
			clrBase,
			0.63, 1, 3);

		globalData.clrBarDkShadow = m_clrCustomizeButtonGradientDark;

		(*m_pfGetThemeColor) (m_hThemeButton, 1, 5, 3823, &clrCustom);

		m_clrCustomizeButtonGradientLight = CBCGPDrawManager::SmartMixColors (
			clrCustom,
			clrBase,
			1.2, 1, 3);

		globalData.clrBarShadow = CBCGPDrawManager::SmartMixColors (
			clrCustom,
			clrBase,
			1.4, 1, 3);

		m_clrToolBarBottomLine = CBCGPDrawManager::PixelAlpha (m_clrToolBarGradientDark, 50);
		m_colorToolBarCornerTop = CBCGPDrawManager::PixelAlpha (m_clrToolBarGradientLight, 92);
		m_colorToolBarCornerBottom = CBCGPDrawManager::PixelAlpha (m_clrToolBarGradientDark, 97);

		m_clrGripper = 
			CBCGPDrawManager::PixelAlpha (m_clrToolBarGradientVertDark, 40);

		clrSeparatorDark = 
			CBCGPDrawManager::PixelAlpha (m_clrToolBarGradientVertDark, 81);

		m_clrMenuItemBorder = m_clrGripper;
		
		m_clrMenuBorder = 
			CBCGPDrawManager::PixelAlpha (clrBase, 80);

		m_clrCaptionBarGradientDark = m_clrCustomizeButtonGradientDark;
		m_clrCaptionBarGradientLight = m_clrCustomizeButtonGradientLight;
	}
	else
	{
		m_clrHighlightMenuItem = (COLORREF)-1;

		m_clrHighlightGradientLight = m_clrHighlight;
		m_clrHighlightGradientDark = m_clrHighlightDn;
		m_clrHighlightDnGradientLight = 
			CBCGPDrawManager::PixelAlpha (m_clrHighlightGradientLight, 120);

		m_clrCustomizeButtonGradientDark = globalData.clrBarShadow;
		m_clrCustomizeButtonGradientLight = 
			CBCGPDrawManager::PixelAlpha (globalData.clrBarShadow, 125);

		m_clrToolBarBottomLine = m_clrToolBarGradientDark;
		m_colorToolBarCornerTop = globalData.clrBarLight;
		m_colorToolBarCornerBottom = m_clrToolBarGradientDark;

		m_clrGripper = globalData.clrBarShadow;
		clrSeparatorDark = globalData.clrBarShadow;

		m_clrCaptionBarGradientLight = globalData.clrBarShadow;
		m_clrCaptionBarGradientDark = globalData.clrBarDkShadow;
	}

	m_clrHighlightDnGradientDark = m_clrHighlightGradientDark;

	m_clrHighlightCheckedGradientLight = m_clrHighlightDnGradientDark;

	m_clrHighlightCheckedGradientDark = 
		CBCGPDrawManager::PixelAlpha (m_clrHighlightDnGradientLight, 120);

	m_brTabBack.DeleteObject ();
	m_brTabBack.CreateSolidBrush (m_clrToolBarGradientLight);

	m_penSeparatorLight.DeleteObject ();
	m_penSeparatorLight.CreatePen (PS_SOLID, 1, globalData.clrBarHilite);

	m_brTearOffCaption.DeleteObject ();
	m_brTearOffCaption.CreateSolidBrush (globalData.clrBarFace);

	m_brFace.DeleteObject ();
	m_brFace.CreateSolidBrush (m_clrToolBarGradientLight);

	m_penSeparator.DeleteObject ();
	m_penSeparator.CreatePen (PS_SOLID, 1, clrSeparatorDark);

	m_clrMenuShadowBase = globalData.clrBarFace;

	// --------------------------
	// Calculate TaskPane colors:
	// --------------------------
	if (m_bIsStandardWinXPTheme && m_hThemeExplorerBar != NULL)
	{
		ASSERT (m_pfGetThemeColor != NULL);

		(*m_pfGetThemeColor) (m_hThemeExplorerBar, 0, 0, 3810, &m_clrTaskPaneGradientLight);// GRADIENTCOLOR1
		(*m_pfGetThemeColor) (m_hThemeExplorerBar, 0, 0, 3811, &m_clrTaskPaneGradientDark);	// GRADIENTCOLOR2

		(*m_pfGetThemeColor) (m_hThemeExplorerBar, 5, 0, 3802, &m_clrTaskPaneGroupCaptionDark);		// FILLCOLOR
		(*m_pfGetThemeColor) (m_hThemeExplorerBar, 12, 0, 3802, &m_clrTaskPaneGroupCaptionSpecDark);// FILLCOLOR
		m_clrTaskPaneGroupCaptionSpecLight = m_clrTaskPaneGroupCaptionDark;

		(*m_pfGetThemeColor) (m_hThemeExplorerBar, 5, 0, 3802, &m_clrTaskPaneGroupAreaLight);	// FILLCOLOR
		m_clrTaskPaneGroupAreaDark = m_clrTaskPaneGroupAreaLight;
		(*m_pfGetThemeColor) (m_hThemeExplorerBar, 9, 0, 3821, &m_clrTaskPaneGroupAreaSpecLight);	// FILLCOLORHINT
		m_clrTaskPaneGroupAreaSpecDark = m_clrTaskPaneGroupAreaSpecLight;
		(*m_pfGetThemeColor) (m_hThemeExplorerBar, 5, 0, 3801, &m_clrTaskPaneGroupBorder);	// BORDERCOLOR
		m_clrTaskPaneGroupCaptionLight = m_clrTaskPaneGroupBorder;
	}
	else
	{
		m_clrTaskPaneGradientDark  = m_clrBarGradientDark;
		m_clrTaskPaneGradientLight  = m_clrToolBarGradientLight;
		m_clrTaskPaneGroupCaptionDark  = m_clrBarGradientDark;
		m_clrTaskPaneGroupCaptionLight  = m_clrToolBarGradientLight;
		m_clrTaskPaneGroupCaptionSpecDark = CBCGPDrawManager::SmartMixColors (
			m_clrCustomizeButtonGradientDark, 
			m_clrCustomizeButtonGradientLight);
		m_clrTaskPaneGroupCaptionSpecLight  = m_clrCustomizeButtonGradientLight;
		m_clrTaskPaneGroupAreaLight  = m_clrToolBarGradientLight;
		m_clrTaskPaneGroupAreaDark  = m_clrToolBarGradientLight;
		m_clrTaskPaneGroupAreaSpecLight  = m_clrToolBarGradientLight;
		m_clrTaskPaneGroupAreaSpecDark  = m_clrToolBarGradientLight;
		m_clrTaskPaneGroupBorder  = m_clrToolBarGradientLight;
	}

	m_penTaskPaneGroupBorder.DeleteObject ();
	m_penTaskPaneGroupBorder.CreatePen (PS_SOLID, 1, m_clrTaskPaneGroupBorder);
}
//***********************************************************************************
void CBCGPVisualManager2003::OnFillHighlightedArea (CDC* pDC, CRect rect, 
							CBrush* pBrush, CBCGPToolbarButton* pButton)
{
	if (!m_bIsStandardWinXPTheme || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillHighlightedArea (pDC, rect, pBrush, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);

	BOOL bIsHorz = TRUE;
	BOOL bIsPopupMenu = FALSE;

	COLORREF clr1 = (COLORREF)-1;
	COLORREF clr2 = (COLORREF)-1;

	if (pButton != NULL)
	{
		ASSERT_VALID (pButton);

		bIsHorz = pButton->IsHorizontal ();

		CBCGPToolbarMenuButton* pMenuButton = 
			DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);

		bIsPopupMenu = pMenuButton != NULL &&
			pMenuButton->GetParentWnd () != NULL &&
			pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

		if (bIsPopupMenu && pBrush == &m_brHighlight)
		{
			if (m_clrHighlightMenuItem != (COLORREF)-1)
			{
				CBrush br (m_clrHighlightMenuItem);
				pDC->FillRect (&rect, &br);
				return;
			}
		}

		if (pMenuButton != NULL &&
			!bIsPopupMenu &&
			pMenuButton->IsDroppedDown ())
		{
			clr1 = m_clrToolBarGradientDark;
			clr2 = m_clrToolBarGradientLight;
		}
	}

	if (pBrush == &m_brHighlight)
	{
		clr1 = m_clrHighlightGradientDark;
		clr2 = bIsPopupMenu ? clr1 : m_clrHighlightGradientLight;
	}
	else if (pBrush == &m_brHighlightDn)
	{
		clr1 = bIsPopupMenu ? m_clrHighlightDnGradientLight : m_clrHighlightDnGradientDark;
		clr2 = m_clrHighlightDnGradientLight;
	}
	else if (pBrush == &m_brHighlightChecked)
	{
		clr1 = bIsPopupMenu ? m_clrHighlightCheckedGradientLight : m_clrHighlightCheckedGradientDark;
		clr2 = m_clrHighlightCheckedGradientLight;
	}

	if (clr1 == (COLORREF)-1 || clr2 == (COLORREF)-1)
	{
		CBCGPVisualManagerXP::OnFillHighlightedArea (pDC, rect, pBrush, pButton);
		return;
	}

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, clr1, clr2, bIsHorz);
}
//***********************************************************************************
void CBCGPVisualManager2003::OnDrawShowAllMenuItems (CDC* pDC, CRect rect, 
												 CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);

	if (globalData.m_nBitsPerPixel > 8)
	{
		const int nRadius = 8;

		rect = CRect (rect.CenterPoint () - CSize (nRadius - 1, nRadius - 1), CSize (nRadius * 2, nRadius * 2));

		CBCGPDrawManager dm (*pDC);
		dm.DrawGradientRing (rect, m_clrToolBarGradientDark, m_clrMenuLight,
			(COLORREF)-1,
			45, nRadius);
	}

	CBCGPVisualManager::OnDrawShowAllMenuItems (pDC, rect, state);
}
//************************************************************************************
int CBCGPVisualManager2003::GetShowAllMenuItemsHeight (CDC* pDC, const CSize& sizeDefault)
{
	int nHeight = CBCGPVisualManager::GetShowAllMenuItemsHeight (pDC, sizeDefault);
	return nHeight + 4;
}
//***********************************************************************************
void CBCGPVisualManager2003::OnDrawCaptionBarBorder (CDC* pDC, 
	CBCGPCaptionBar* /*pBar*/, CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder)
{
	ASSERT_VALID (pDC);

	if (clrBarBorder == (COLORREF) -1)
	{
		clrBarBorder = globalData.clrBarFace;
	}

	CBrush brBorder (clrBarBorder);
	pDC->FillRect (rect, &brBorder);

	if (!bFlatBorder)
	{
		pDC->Draw3dRect (rect, m_clrBarGradientLight, m_clrToolBarBottomLine);
	}
}
//**************************************************************************************
void CBCGPVisualManager2003::OnDrawComboDropButton (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawComboDropButton (pDC, rect,
												bDisabled, bIsDropped,
												bIsHighlighted, pButton);
		return;
	}

	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	CPoint pointTriangle (
		rect.left + (rect.Width () - CMenuImages::Size ().cx) / 2 + 1,
		rect.top + (rect.Height () - CMenuImages::Size ().cy) / 2 + 1);

	if (bIsDropped || bIsHighlighted)
	{
		OnFillHighlightedArea (pDC, rect, 
			bIsDropped ? &m_brHighlightDn : &m_brHighlight,
			NULL);

		CPen pen (PS_SOLID, 1, m_clrMenuItemBorder);
		CPen* pOldPen = pDC->SelectObject (&pen);
		ASSERT (pOldPen != NULL);

		pDC->MoveTo (rect.left, rect.top);
		pDC->LineTo (rect.left, rect.bottom);

		pDC->SelectObject (pOldPen);
	}
	else
	{
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rect, m_clrToolBarGradientDark, m_clrToolBarGradientLight, TRUE);

		pDC->Draw3dRect (rect, globalData.clrWindow, globalData.clrWindow);
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, pointTriangle,
		bDisabled ? CMenuImages::ImageGray : (bIsDropped && bIsHighlighted) ? 
			CMenuImages::ImageWhite : CMenuImages::ImageBlack);
}
//***********************************************************************************
void CBCGPVisualManager2003::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawTearOffCaption (pDC, rect, bIsActive);
		return;
	}

	const int iBorderSize = 1;
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);

	rect.DeflateRect (iBorderSize, 1);

	if (bIsActive)
	{
		OnFillHighlightedArea (pDC, rect, bIsActive ? &m_brHighlight : &m_brBarBkgnd,
			NULL);
	}
	else
	{
		pDC->FillRect (rect, &m_brTearOffCaption);
	}
	
	// Draw gripper:
	OnDrawBarGripper (pDC, rect, FALSE, NULL);

	if (bIsActive)
	{
		pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
	}
}
//***********************************************************************************
void CBCGPVisualManager2003::OnDrawMenuBorder (CDC* pDC, 
		CBCGPPopupMenu* pMenu, CRect rect)
{
	BOOL bConnectMenuToParent = m_bConnectMenuToParent;

	if (DYNAMIC_DOWNCAST (CCustomizeButton, pMenu->GetParentButton ()) != NULL)
	{
		m_bConnectMenuToParent = FALSE;
	}

	CBCGPVisualManagerXP::OnDrawMenuBorder (pDC, pMenu, rect);
	m_bConnectMenuToParent = bConnectMenuToParent;
}
//***********************************************************************************
COLORREF CBCGPVisualManager2003::GetThemeColor (HTHEME hTheme, int nIndex) const
{
	if (hTheme != NULL && m_pfGetThemeSysColor != NULL)
	{
		return (*m_pfGetThemeSysColor) (hTheme, nIndex);
	}
	
	return ::GetSysColor (nIndex);
}
//***********************************************************************************
void CBCGPVisualManager2003::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (pTabWnd->IsDialogControl ())
	{
		pDC->FillRect (rect, &globalData.brBtnFace);
		return;
	}

	if (pTabWnd->IsFlatTab () || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnEraseTabsArea (pDC, rect, pTabWnd);
		return;
	}

	CBCGPDrawManager dm (*pDC);

	COLORREF clr1 = m_clrToolBarGradientDark;
	COLORREF clr2 = m_clrToolBarGradientLight;

	if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
		dm.FillGradient (rect, clr1, clr2, TRUE);
	}
	else
	{
		dm.FillGradient (rect, clr2, clr1, TRUE);
	}

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());
	if (clrActiveTab == (COLORREF)-1)
	{
		clrActiveTab = RGB (255, 255, 255);
	}

	if (pTabWnd->IsOneNoteStyle ())
	{
		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			rect.bottom = rect.top + 2;
		}
		else
		{
			rect.top = rect.bottom - 2;
		}

		pDC->FillSolidRect (rect, clrActiveTab);
	}
}
//*************************************************************************************
void CBCGPVisualManager2003::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	if (!pTabWnd->IsOneNoteStyle () || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	CRect rectClip;
	pTabWnd->GetTabsRect (rectClip);

	const int nExtra = (iTab == 0 || bIsActive) ? 0 : rectTab.Height ();

	if (rectTab.left + nExtra + 10 > rectClip.right ||
		rectTab.right - 10 <= rectClip.left)
	{
		return;
	}

	const BOOL bIsHighlight = iTab == pTabWnd->GetHighlightedTab ();

	COLORREF clrTab = pTabWnd->GetTabBkColor (iTab);
	if (clrTab == (COLORREF)-1 && bIsActive)
	{
		clrTab = RGB (255, 255, 255);
	}

	if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
		rectTab.OffsetRect (0, -1);
	}

	CRect rectFill = rectTab;

	#define POINTS_NUM	8
	POINT pts [POINTS_NUM];

	const int nHeight = rectFill.Height ();

	pts [0].x = rectFill.left;
	pts [0].y = rectFill.bottom;

	pts [1].x = rectFill.left + 2;
	pts [1].y = rectFill.bottom - 1;

	pts [2].x = rectFill.left + 4;
	pts [2].y = rectFill.bottom - 2;
	
	pts [3].x = rectFill.left + nHeight;
	pts [3].y = rectFill.top + 2;
	
	pts [4].x = rectFill.left + nHeight + 4;
	pts [4].y = rectFill.top;
	
	pts [5].x = rectFill.right - 2;
	pts [5].y = rectFill.top;
	
	pts [6].x = rectFill.right;
	pts [6].y = rectFill.top + 2;

	pts [7].x = rectFill.right;
	pts [7].y = rectFill.bottom;

	BOOL bIsCutted = FALSE;

	for (int i = 0; i < POINTS_NUM; i++)
	{
		if (pts [i].x > rectClip.right)
		{
			pts [i].x = rectClip.right;
			bIsCutted = TRUE;
		}

		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			pts [i].y = rectFill.bottom - pts [i].y + rectFill.top;
		}
	}

	CRgn rgn;
	rgn.CreatePolygonRgn (pts, POINTS_NUM, WINDING);

	pDC->SelectClipRgn (&rgn);

	CRect rectLeft;
	pTabWnd->GetClientRect (rectLeft);
	rectLeft.right = rectClip.left - 1;

	pDC->ExcludeClipRect (rectLeft);

	CBCGPDrawManager dm (*pDC);

	COLORREF clrFill = bIsHighlight ? m_clrHighlightMenuItem : clrTab;
	COLORREF clr2;

	if (clrFill != (COLORREF)-1)
	{
		clr2 = CBCGPDrawManager::PixelAlpha (clrFill, 150);
	}
	else
	{
		clrFill = m_clrToolBarGradientDark;
		clr2 = m_clrToolBarGradientLight;
	}

	if (pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM)
	{
		COLORREF clr = clrFill;
		clrFill = clr2;
		clr2 = clr;

		rectFill.top++;
	}

	dm.FillGradient (rectFill, clrFill, clr2);
	pDC->SelectClipRgn (NULL);

	pDC->ExcludeClipRect (rectLeft);

	if (iTab > 0 && !bIsActive && iTab != pTabWnd->GetFirstVisibleTabNum ())
	{
		CRect rectLeftTab = rectClip;
		rectLeftTab.right = rectFill.left + rectFill.Height () - 10;

		if (pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM)
		{
			rectLeftTab.top -= 2;
		}
		else
		{
			rectLeftTab.bottom++;
		}

		pDC->ExcludeClipRect (rectLeftTab);
	}

	CPen penGray (PS_SOLID, 1, globalData.clrBarDkShadow);
	CPen penShadow (PS_SOLID, 1, globalData.clrBarShadow);

	CPen* pOldPen = pDC->SelectObject (&penGray);
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject (NULL_BRUSH);

	pDC->Polyline (pts, POINTS_NUM);

	if (bIsCutted)
	{
		pDC->MoveTo (rectClip.right, rectTab.top);
		pDC->LineTo (rectClip.right, rectTab.bottom);
	}

	CRect rectRight = rectClip;
	rectRight.left = rectFill.right;

	pDC->ExcludeClipRect (rectRight);

	CPen penLight (PS_SOLID, 1, bIsHighlight ?
		globalData.clrBarDkShadow : globalData.clrBarHilite);

	pDC->SelectObject (&penLight);

	if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
	}
	else
	{
		pDC->MoveTo (pts [2].x, pts [2].y + 1);
		pDC->LineTo (pts [3].x + 1, pts [3].y);

		pDC->MoveTo (pts [3].x + 1, pts [3].y + 1);
		pDC->LineTo (pts [3].x + 2, pts [3].y + 1);

		pDC->MoveTo (pts [3].x + 2, pts [3].y);
		pDC->LineTo (pts [3].x + 4, pts [3].y);

		pDC->MoveTo (pts [4].x, pts [4].y + 1);
		pDC->LineTo (pts [5].x + 1, pts [5].y + 1);

		if (!bIsActive && !bIsCutted)
		{
			pDC->SelectObject (&penShadow);

			pDC->MoveTo (pts [6].x - 2, pts [6].y - 1);
			pDC->LineTo (pts [6].x - 1, pts [6].y - 1);
		}

		pDC->MoveTo (pts [6].x - 1, pts [6].y);
		pDC->LineTo (pts [7].x - 1, pts [7].y);
	}

	pDC->SelectObject (pOldPen);
	pDC->SelectObject (pOldBrush);

	if (bIsActive)
	{
		const int iBarHeight = (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM) ? 2 : 3;
		const int y = (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM) ? 
			(rectTab.top - iBarHeight) : (rectTab.bottom);

		CRect rectFill (CPoint (rectTab.left + 2, y), 
						CSize (rectTab.Width () - 1, iBarHeight));
		
		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			rectFill.OffsetRect (-1, 1);
		}

		rectFill.right = min (rectFill.right, rectClip.right);

		CBrush br (clrTab);
		pDC->FillRect (rectFill, &br);
	}

	if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
		rectTab.left += rectTab.Height () + CBCGPBaseTabWnd::TAB_IMAGE_MARGIN;
	}
	else
	{
		rectTab.left += rectTab.Height ();
		rectTab.right -= CBCGPBaseTabWnd::TAB_IMAGE_MARGIN;
	}

	COLORREF clrText = pTabWnd->GetTabTextColor (iTab);
	
	COLORREF cltTextOld = (COLORREF)-1;
	if (!bIsActive && clrText != (COLORREF)-1)
	{
		cltTextOld = pDC->SetTextColor (clrText);
	}

	rectTab.right = min (rectTab.right, rectClip.right - 2);

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, (COLORREF)-1);

	if (cltTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (cltTextOld);
	}

	pDC->SelectClipRgn (NULL);
}
//*********************************************************************************
void CBCGPVisualManager2003::OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill,
									 int iTab, BOOL bIsActive, 
									 const CBCGPBaseTabWnd* pTabWnd)
{
	if (pTabWnd->IsFlatTab () || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillTab (pDC, rectFill, pbrFill,
									 iTab, bIsActive, pTabWnd);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	COLORREF clr1 = m_clrBarGradientDark;
	
	if (pTabWnd->GetTabBkColor (iTab) != (COLORREF)-1)
	{
		clr1 = pTabWnd->GetTabBkColor (iTab);
	}
	else if (!bIsActive)
	{
		return;
	}

	COLORREF clr2 = CBCGPDrawManager::PixelAlpha (clr1, 150);

	CBCGPDrawManager dm (*pDC);

	if (pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_TOP)
	{
		dm.FillGradient (rectFill, clr1, clr2, TRUE);
	}
	else
	{
		dm.FillGradient (rectFill, clr2, clr1, TRUE);
	}
}
//***********************************************************************************
BOOL CBCGPVisualManager2003::OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd)
{	
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (pTabWnd->IsFlatTab () || globalData.m_nBitsPerPixel <= 8)
	{
		return CBCGPVisualManagerXP::OnEraseTabsFrame (pDC, rect, pTabWnd);
	}

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());
	if (clrActiveTab == (COLORREF)-1 && pTabWnd->IsOneNoteStyle ())
	{
		clrActiveTab = RGB (255, 255, 255);
	}

	CBCGPDrawManager dm (*pDC);

	COLORREF clr1 = m_clrBarGradientDark;

	if (clrActiveTab != (COLORREF)-1)
	{
		clr1 = clrActiveTab;
	}

	COLORREF clr2 = CBCGPDrawManager::PixelAlpha (clr1, 130);

	if (pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM)
	{
		COLORREF clr = clr1;
		clr1 = clr2;
		clr2 = clr;
	}

	dm.FillGradient2 (rect, clr1, clr2, 45);
	return TRUE;
}
//*********************************************************************************
void CBCGPVisualManager2003::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pBaseTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pBaseTab);

	CBCGPTabWnd* pWndTab = DYNAMIC_DOWNCAST (CBCGPTabWnd, pBaseTab);

	if (pWndTab == NULL || pBaseTab->IsFlatTab () || globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnEraseTabsButton (pDC, rect, pButton, pBaseTab);
		return;
	}

	if (pBaseTab->IsOneNoteStyle () && 
		(pButton->IsPressed () || pButton->IsHighlighted ()))
	{
		CBCGPDrawManager dm (*pDC);

		if (pButton->IsPressed ())
		{
			dm.FillGradient (rect, m_clrHighlightDnGradientDark, m_clrHighlightDnGradientLight);
		}
		else
		{
			dm.FillGradient (rect, m_clrHighlightGradientDark, m_clrHighlightGradientLight);
		}

		return;
	}

	CRgn rgn;
	rgn.CreateRectRgnIndirect (rect);

	pDC->SelectClipRgn (&rgn);

	CRect rectTabs;
	pWndTab->GetClientRect (&rectTabs);

	CRect rectTabArea;
	pWndTab->GetTabsRect (rectTabArea);

	if (pWndTab->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
	{
		rectTabs.top = rectTabArea.top - pWndTab->GetTabBorderSize ();
	}
	else
	{
		rectTabs.bottom = rectTabArea.bottom + pWndTab->GetTabBorderSize ();
	}

	pWndTab->MapWindowPoints (pButton, rectTabs);
	OnEraseTabsArea (pDC, rectTabs, pWndTab);

	pDC->SelectClipRgn (NULL);
}
//************************************************************************************
void CBCGPVisualManager2003::ModifyGlobalColors ()
{
	if (globalData.m_nBitsPerPixel <= 8 || !m_bIsStandardWinXPTheme)
	{
		//----------------------------------------------
		// Theme color may differ from the system color:
		//----------------------------------------------
		globalData.clrBarFace = GetThemeColor (m_hThemeButton, COLOR_3DFACE);
		globalData.clrBarShadow = GetThemeColor (m_hThemeButton, COLOR_3DSHADOW);
		globalData.clrBarHilite = GetThemeColor (m_hThemeButton, COLOR_3DHIGHLIGHT);
		globalData.clrBarDkShadow = GetThemeColor (m_hThemeButton, COLOR_3DDKSHADOW);
		globalData.clrBarLight = GetThemeColor (m_hThemeButton, COLOR_3DLIGHT);
	}
	else
	{
		BOOL bIsOliveGreenTheme = 
			m_strWinXPThemeColor.CompareNoCase (_T("homestead")) == 0;

		BOOL bIsSilverTheme = 
			m_strWinXPThemeColor.CompareNoCase (_T("metallic")) == 0;

		COLORREF clrBase = GetThemeColor (m_hThemeWindow, /*COLOR_MENUHILIGHT*/29);

		if (bIsOliveGreenTheme)
		{
			COLORREF clrToolBarGradientDark = CBCGPDrawManager::PixelAlpha (
				clrBase, 120);

			COLORREF clrToolBarGradientLight = CBCGPDrawManager::SmartMixColors (
				clrBase, 
				GetThemeColor (m_hThemeWindow, COLOR_WINDOW),
				1., 2, 1);

			globalData.clrBarFace = CBCGPDrawManager::SmartMixColors (
				clrToolBarGradientDark,
				clrToolBarGradientLight, 1., 2, 1);
		}
		else if (bIsSilverTheme)
		{
			COLORREF clrToolBarGradientDark = CBCGPDrawManager::SmartMixColors (
				clrBase, 
				GetThemeColor (m_hThemeWindow, COLOR_3DFACE),
				0.75, 2);

			COLORREF clrToolBarGradientLight = CBCGPDrawManager::SmartMixColors (
				clrBase, 
				GetThemeColor (m_hThemeWindow, COLOR_WINDOW),
				1.03);

			globalData.clrBarFace = CBCGPDrawManager::PixelAlpha (CBCGPDrawManager::SmartMixColors (
				clrToolBarGradientDark,
				clrToolBarGradientLight), 95);
		}
		else
		{
			globalData.clrBarFace = CBCGPDrawManager::SmartMixColors (
				GetThemeColor (m_hThemeWindow, /*COLOR_HIGHLIGHT*/29),
				GetThemeColor (m_hThemeWindow, COLOR_WINDOW));
		}

		globalData.clrBarShadow = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, 70);
		globalData.clrBarHilite = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, 130);
		globalData.clrBarDkShadow = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, 50);
		globalData.clrBarLight = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, 110);
	}

	globalData.brBarFace.DeleteObject ();
	globalData.brBarFace.CreateSolidBrush (globalData.clrBarFace);
}
//************************************************************************************
void CBCGPVisualManager2003::SetUseGlobalTheme (BOOL bUseGlobalTheme/* = TRUE*/)
{
	m_bUseGlobalTheme = bUseGlobalTheme;

	CBCGPVisualManager::GetInstance ()->OnUpdateSystemColors ();
	CBCGPVisualManager::GetInstance ()->RedrawAll ();
}
//************************************************************************************
void CBCGPVisualManager2003::SetStatusBarOfficeXPLook (BOOL bStatusBarOfficeXPLook/* = TRUE*/)
{
	m_bStatusBarOfficeXPLook = bStatusBarOfficeXPLook;

	CBCGPVisualManager::GetInstance ()->RedrawAll ();
}
//***********************************************************************************
void CBCGPVisualManager2003::GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack)
{
	ASSERT_VALID (pTabWnd);
	
	CBCGPVisualManagerXP::GetTabFrameColors (pTabWnd,
			   clrDark, clrBlack,
			   clrHighlight, clrFace,
			   clrDarkShadow, clrLight,
			   pbrFace, pbrBlack);

	if (pTabWnd->IsOneNoteStyle () || globalData.m_nBitsPerPixel <= 8)
	{
		return;
	}

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());

	if (clrActiveTab == (COLORREF)-1)
	{
		clrFace = RGB (255, 255, 255);
	}

	clrDark = globalData.clrBarShadow;
	clrBlack = globalData.clrBarDkShadow;
	clrHighlight = globalData.clrBarLight;
	clrDarkShadow = globalData.clrBarShadow;
	clrLight = globalData.clrBarFace;
}
//************************************************************************************
void CBCGPVisualManager2003::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillTasksPaneBackground (pDC, rectWorkArea);
		return;
	}

	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rectWorkArea, m_clrTaskPaneGradientDark, m_clrTaskPaneGradientLight, TRUE);
}
//************************************************************************************
void CBCGPVisualManager2003::OnDrawTasksGroupCaption(
										CDC* pDC, CBCGPTasksGroup* pGroup, 
										BOOL bIsHighlighted /*= FALSE*/, BOOL /*bIsSelected*/ /*= FALSE*/, 
										BOOL bCanCollapse /*= FALSE*/)
{
	ASSERT_VALID(pDC);
	ASSERT(pGroup != NULL);

	// -----------------------
	// Draw caption background
	// -----------------------
	POINT pts [7];

	const int nLeft = pGroup->m_rect.left;
	const int nTop = pGroup->m_rect.top;

	pts [0].x = nLeft;
	pts [0].y = pGroup->m_rect.bottom;

	pts [1].x = nLeft;
	pts [1].y = nTop + 4;

	pts [2].x = nLeft + 1;
	pts [2].y = nTop + 2;
	
	pts [3].x = nLeft + 2;
	pts [3].y = nTop + 1;
	
	pts [4].x = nLeft + 4;
	pts [4].y = nTop;

	pts [5].x = pGroup->m_rect.right;
	pts [5].y = nTop;

	pts [6].x = pGroup->m_rect.right;
	pts [6].y = pGroup->m_rect.bottom;

	CRgn rgn;
	rgn.CreatePolygonRgn (pts, 7, WINDING);

	pDC->SelectClipRgn (&rgn);

	CBCGPDrawManager dm (*pDC);
	if (pGroup->m_bIsSpecial)
	{
		dm.FillGradient (pGroup->m_rect, m_clrTaskPaneGroupCaptionSpecDark, 
			m_clrTaskPaneGroupCaptionSpecLight, FALSE);
	}
	else
	{
		dm.FillGradient (pGroup->m_rect, m_clrTaskPaneGroupCaptionLight, 
			m_clrTaskPaneGroupCaptionDark, FALSE);
	}

	pDC->SelectClipRgn (NULL);

	// ---------------------------
	// Draw an icon if it presents
	// ---------------------------
	BOOL bShowIcon = (pGroup->m_hIcon != NULL 
		&& pGroup->m_sizeIcon.cx < pGroup->m_rect.Width () - pGroup->m_rect.Height());
	if (bShowIcon)
	{
		CPoint pointIcon(pGroup->m_rect.left+1, pGroup->m_rect.bottom - pGroup->m_sizeIcon.cy);
		pDC->DrawIcon(pointIcon, pGroup->m_hIcon);
	}

	// -----------------------
	// Draw group caption text
	// -----------------------
	CFont* pFontOld = pDC->SelectObject (&globalData.fontBold);
	COLORREF clrTextOld = pDC->SetTextColor (pGroup->m_bIsSpecial ? 
		m_clrTaskPaneGroupBorder : globalData.clrHilite);
	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);
	
	int nTaskPaneHOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();

	CRect rectText = pGroup->m_rect;
	rectText.left += (bShowIcon ? pGroup->m_sizeIcon.cx	: 
		(nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset));
	rectText.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectText.right = max(rectText.left, rectText.right - pGroup->m_rect.Height());

	pDC->DrawText (pGroup->m_strName, rectText, DT_SINGLELINE);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);

	// -------------------------
	// Draw group caption button
	// -------------------------
	if (bCanCollapse && !pGroup->m_strName.IsEmpty())
	{
		CSize sizeButton = CMenuImages::Size();
		CRect rectButton = pGroup->m_rect;
		rectButton.left = max(rectButton.left, 
			rectButton.right - (rectButton.Height()+1)/2 - (sizeButton.cx+1)/2);
		rectButton.top = max(rectButton.top, 
			rectButton.bottom - (rectButton.Height()+1)/2 - (sizeButton.cy+1)/2);
		rectButton.right = rectButton.left + sizeButton.cx;
		rectButton.bottom = rectButton.top + sizeButton.cy;

		if (rectButton.right <= pGroup->m_rect.right && rectButton.bottom <= pGroup->m_rect.bottom)
		{
			if (bIsHighlighted)
			{
				// Draw button frame
				CBrush* pBrushOld = (CBrush*) pDC->SelectObject (&globalData.brBarFace);
				COLORREF clrBckOld = pDC->GetBkColor ();

				pDC->Draw3dRect(&rectButton, globalData.clrWindow, globalData.clrBarShadow);

				pDC->SetBkColor (clrBckOld);
				pDC->SelectObject (pBrushOld);
			}

			if (!pGroup->m_bIsCollapsed)
			{
				CMenuImages::Draw(pDC, CMenuImages::IdArowUp, rectButton.TopLeft());
			}
			else
			{
				CMenuImages::Draw(pDC, CMenuImages::IdArowDown, rectButton.TopLeft());
			}
		}
	}
}
//************************************************************************************
void CBCGPVisualManager2003::OnFillTasksGroupInterior(
								CDC* pDC, CRect rect, BOOL bSpecial /*= FALSE*/)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillTasksGroupInterior(pDC, rect, bSpecial);
		return;
	}

	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);
	if (bSpecial)
	{
		dm.FillGradient (rect, m_clrTaskPaneGroupCaptionSpecDark, 
			m_clrTaskPaneGroupCaptionSpecLight, TRUE);
	}
	else
	{
		dm.FillGradient (rect, m_clrTaskPaneGroupAreaDark, 
			m_clrTaskPaneGroupAreaLight, TRUE);
	}
}
//************************************************************************************
void CBCGPVisualManager2003::OnDrawTasksGroupAreaBorder(
											CDC* pDC, CRect rect, BOOL /*bSpecial = FALSE*/,
											BOOL /*bNoTitle = FALSE*/)
{
	ASSERT_VALID(pDC);

	// Draw underline
	CPen* pPenOld = (CPen*) pDC->SelectObject (&m_penTaskPaneGroupBorder);

	rect.right -= 1;
	rect.bottom -= 1;
	pDC->MoveTo (rect.left, rect.top);
	pDC->LineTo (rect.right, rect.top);
	pDC->LineTo (rect.right, rect.bottom);
	pDC->LineTo (rect.left, rect.bottom);
	pDC->LineTo (rect.left, rect.top);

	pDC->SelectObject (&pPenOld);
}
//************************************************************************************
void CBCGPVisualManager2003::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, 
							BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/)
{
	ASSERT_VALID (pTask);
	ASSERT_VALID (pDC);

	if (pTask->m_bIsSeparator)
	{
		CRect rectText = pTask->m_rect;

		CPen* pPenOld = (CPen*) pDC->SelectObject (&m_penSeparator);

		pDC->MoveTo (rectText.left, rectText.CenterPoint ().y);
		pDC->LineTo (rectText.right, rectText.CenterPoint ().y);

		pDC->SelectObject (&pPenOld);
		return;
	}

	COLORREF clrOld = globalData.clrHotText;
	globalData.clrHotText = globalData.clrHilite;

	CBCGPVisualManagerXP::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, bIsSelected);

	globalData.clrHotText = clrOld;
}
//**********************************************************************************
void CBCGPVisualManager2003::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	ASSERT_VALID (pDC);

	CRect rectImage (CPoint (0, 0), CMenuImages::Size ());

	CRect rectFill = rect;
	rectFill.top -= nBorderSize;

	pDC->FillRect (rectFill, &globalData.brBarFace);

	if (bHilited)
	{
		CBrush br (globalData.m_nBitsPerPixel <= 8 ?
			globalData.clrWindow : m_clrHighlightMenuItem == (COLORREF)-1 ? 
			m_clrHighlight : m_clrHighlightMenuItem);

		pDC->FillRect (rect, &br);
		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
	else
	{
		pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
	}

	CPoint point (
		rect.left + (rect.Width () - rectImage.Width ()) / 2,
		rect.top + (rect.Height () - rectImage.Height ()) / 2);

	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, point);
}
//**********************************************************************************
COLORREF CBCGPVisualManager2003::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		return CBCGPVisualManagerXP::OnFillCommandsListBackground (pDC, rect, bIsSelected);
	}

	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	COLORREF clrText = globalData.clrBarText;

	int iImageWidth = CBCGPToolBar::GetMenuImageSize ().cx + GetMenuImageMargin ();

	if (bIsSelected)
	{
		rect.left = 0;

		COLORREF color = m_clrHighlightMenuItem == (COLORREF)-1 ?
			m_clrHighlight : m_clrHighlightMenuItem;
		
		CBrush br (color);
		pDC->FillRect (&rect, &br);

		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

		// Now, we should define a menu text color...
		if (GetRValue (color) > 128 &&
			GetGValue (color) > 128 &&
			GetBValue (color) > 128)
		{
			clrText = RGB (0, 0, 0);
		}
		else
		{
			clrText = RGB (255, 255, 255);
		}
	}
	else
	{
		pDC->FillRect (rect, &m_brMenuLight);

		CRect rectImages = rect;
		rectImages.right = rectImages.left + iImageWidth + MENU_IMAGE_MARGIN;

		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rectImages, m_clrToolBarGradientLight, m_clrToolBarGradientDark, FALSE);

		clrText = globalData.clrBarText;
	}

	return clrText;
}
//**************************************************************************************
void CBCGPVisualManager2003::OnDrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* pStatusBar,
			CRect rectProgress, int nProgressTotal, int nProgressCurr,
			COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
			BOOL bProgressText)
{
	if (!DrawStatusBarProgress (pDC, pStatusBar,
			rectProgress, nProgressTotal, nProgressCurr,
			clrBar, clrProgressBarDest, clrProgressText, bProgressText))
	{
		CBCGPVisualManagerXP::OnDrawStatusBarProgress (pDC, pStatusBar,
			rectProgress, nProgressTotal, nProgressCurr,
			clrBar, clrProgressBarDest, clrProgressText, bProgressText);
	}
}
//****************************************************************************************
void CBCGPVisualManager2003::OnFillHeaderCtrlBackground (CBCGPHeaderCtrl* pCtrl,
													 CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	CBCGPVisualManagerXP::OnFillHeaderCtrlBackground (pCtrl, pDC, rect);
}
//**************************************************************************************
COLORREF CBCGPVisualManager2003::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
			BOOL bActive, CRect rectCaption, CRect rectButtons)
{
	ASSERT_VALID (pDC);

	if (globalData.m_nBitsPerPixel <= 8)
	{
		return CBCGPVisualManagerXP::OnDrawControlBarCaption (pDC, pBar, 
			bActive, rectCaption, rectButtons);
	}

	CBCGPDrawManager dm (*pDC);

	if (!bActive)
	{
		dm.FillGradient (rectCaption, 
						m_clrToolBarGradientDark, 
						m_clrToolBarGradientLight, TRUE);
	}
	else
	{
		dm.FillGradient (rectCaption,	
								m_clrHighlightGradientDark,
								m_clrHighlightGradientLight,
								TRUE);
	}

	return globalData.clrBarText;
}
//*********************************************************************************
void CBCGPVisualManager2003::OnFillAutoHideButtonBackground (CDC* pDC, CRect rect, CBCGPAutoHideButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillAutoHideButtonBackground (pDC, rect, pButton);
		return;
	}

	CBCGPDrawManager dm (*pDC);

	if (pButton->IsActive ())
	{
		dm.FillGradient (rect, 
			m_clrHighlightGradientLight, m_clrHighlightGradientDark,
			pButton->IsHorizontal ());
	}
	else
	{
		dm.FillGradient (rect, 
			m_clrBarGradientLight, m_clrBarGradientDark,
			pButton->IsHorizontal ());
	}
}
//*********************************************************************************
void CBCGPVisualManager2003::OnDrawAutoHideButtonBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* pButton)
{
	ASSERT_VALID (pDC);

	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawAutoHideButtonBorder (pDC, rectBounds, rectBorderSize, pButton);
		return;
	}

	COLORREF clr = globalData.clrBarShadow;
	COLORREF clrText = pDC->GetTextColor ();

	if (rectBorderSize.left > 0)
	{
		pDC->FillSolidRect (rectBounds.left, rectBounds.top, 
							rectBounds.left + rectBorderSize.left, 
							rectBounds.bottom, clr);
	}
	if (rectBorderSize.top > 0)
	{
		pDC->FillSolidRect (rectBounds.left, rectBounds.top, 
							rectBounds.right, 
							rectBounds.top + rectBorderSize.top, clr);
	}
	if (rectBorderSize.right > 0)
	{
		pDC->FillSolidRect (rectBounds.right - rectBorderSize.right, rectBounds.top, 
							rectBounds.right, 
							rectBounds.bottom, clr);
	}
	if (rectBorderSize.bottom > 0)
	{
		pDC->FillSolidRect (rectBounds.left, rectBounds.bottom - rectBorderSize.bottom,  
							rectBounds.right, 
							rectBounds.bottom, clr);
	}

	pDC->SetTextColor (clrText);
}
//*******************************************************************************
void CBCGPVisualManager2003::OnDrawOutlookBarSplitter (CDC* pDC, CRect rectSplitter)
{
	ASSERT_VALID (pDC);

	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnDrawOutlookBarSplitter (pDC, rectSplitter);
		return;
	}

	CBCGPDrawManager dm (*pDC);

	dm.FillGradient (rectSplitter,
					m_clrCaptionBarGradientDark,
					m_clrCaptionBarGradientLight,
					TRUE);

	const int nBoxesNumber = 10;
	const int nBoxSize = rectSplitter.Height () - 3;

	int x = rectSplitter.CenterPoint ().x - nBoxSize * nBoxesNumber / 2;
	int y = rectSplitter.top + 2;

	for (int nBox = 0; nBox < nBoxesNumber; nBox++)
	{
		pDC->FillSolidRect (x + 1, y + 1, nBoxSize / 2, nBoxSize / 2, 
			globalData.clrBtnHilite);
		pDC->FillSolidRect (x, y, nBoxSize / 2, nBoxSize / 2, 
			m_clrGripper);

		x += nBoxSize;
	}
}
//*******************************************************************************
void CBCGPVisualManager2003::OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, COLORREF& clrText)
{
	if (globalData.m_nBitsPerPixel <= 8)
	{
		CBCGPVisualManagerXP::OnFillOutlookBarCaption (pDC, rectCaption, clrText);
		return;
	}

	CBCGPDrawManager dm (*pDC);

	dm.FillGradient (rectCaption,
		m_clrCaptionBarGradientDark, m_clrCaptionBarGradientLight, TRUE);
	clrText = globalData.clrBarHilite;
}
//********************************************************************************
BOOL CBCGPVisualManager2003::OnDrawCalculatorButton (CDC* pDC, 
	CRect rect, CBCGPToolbarButton* pButton, 
	CBCGPVisualManager::BCGBUTTON_STATE state, 
	int /*cmd*/ /* CBCGPCalculator::CalculatorCommands */,
	CBCGPCalculator* pCalculator)
{
	ASSERT_VALID (pButton);
	ASSERT_VALID (pDC);
	ASSERT_VALID (pCalculator);

	switch (state)
	{
	case ButtonsIsPressed:
		OnFillHighlightedArea (pDC, rect, &m_brHighlightDn, pButton);
		break;

	case ButtonsIsHighlighted:
		OnFillHighlightedArea (pDC, rect, &m_brHighlight, pButton);
		break;

	default:
		{
			CBCGPDrawManager dm (*pDC);

			if (pCalculator->IsDialogControl ())
			{
				dm.FillGradient (rect, globalData.clrBtnFace, globalData.clrBtnHilite);
				pDC->Draw3dRect (&rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
				return TRUE;
			}
			else
			{
				dm.FillGradient (rect, m_clrToolBarGradientDark, m_clrToolBarGradientLight);
			}
		}
		break;
	}

	pDC->Draw3dRect (&rect, m_clrToolBarGradientDark, m_clrToolBarGradientDark);

	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManager2003::OnDrawCalculatorDisplay (CDC* pDC, CRect rect, 
												  const CString& /*strText*/, BOOL /*bMem*/,
												  CBCGPCalculator* /*pCalculator*/)
{
	pDC->FillRect (rect, &globalData.brWindow);
	pDC->Draw3dRect (&rect, m_clrToolBarGradientDark, m_clrToolBarGradientDark);

	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManager2003::OnDrawBrowseButton (CDC* pDC, CRect rect, 
	CBCGPEdit* /*pEdit*/, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);

	CRect rectFrame = rect;
	rectFrame.InflateRect (0, 1, 1, 1);

	switch (state)
	{
	case ButtonsIsPressed:
		OnFillHighlightedArea (pDC, rect, &m_brHighlightDn, NULL);
		pDC->Draw3dRect (&rectFrame, m_clrToolBarGradientDark, m_clrToolBarGradientDark);
		break;

	case ButtonsIsHighlighted:
		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);
		pDC->Draw3dRect (&rectFrame, m_clrToolBarGradientDark, m_clrToolBarGradientDark);
		break;

	default:
		{
			CBCGPDrawManager dm (*pDC);

			dm.FillGradient (rect, globalData.clrBtnFace, globalData.clrBtnHilite);
			pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);
		}
		break;
	}

	return TRUE;
}
