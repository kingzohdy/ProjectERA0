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
// BCGPVisualManagerXP.cpp: implementation of the CBCGPVisualManagerXP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BCGPVisualManagerXP.h"
#include "BCGPDrawManager.h"
#include "BCGPMenuBar.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPOutlookBarPane.h"
#include "BCGPColorBar.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPAutoHideDockBar.h"
#include "BCGGlobals.h"
#include "BCGPOutlookButton.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPTaskPaneMiniFrameWnd.h"
#include "BCGPToolbarEditBoxButton.h"
#include "BCGPTasksPane.h"
#include "BCGPCalendarBar.h"
#include "BCGPCalculator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPVisualManagerXP, CBCGPVisualManager)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManagerXP::CBCGPVisualManagerXP(BOOL bIsTemporary) :
		CBCGPVisualManager (bIsTemporary)
{
	m_bConnectMenuToParent = TRUE;

	m_nVertMargin = 4;
	m_nHorzMargin = 4;
	m_nGroupVertOffset = 4;
	m_nGroupCaptionHeight = 0;
	m_nGroupCaptionHorzOffset = 0;
	m_nGroupCaptionVertOffset = 0;
	m_nTasksHorzOffset = 12;
	m_nTasksIconHorzOffset = 5;
	m_nTasksIconVertOffset = 4;
	m_bActiveCaptions = FALSE;
	
	m_bMenuFlatLook = TRUE;
	m_bShadowHighlightedImage = TRUE;
	m_bEmbossDisabledImage = FALSE;
	m_bFadeInactiveImage = TRUE;
	m_bLook2000 = TRUE;
	m_nMenuShadowDepth = 4;
	m_nMenuBorderSize = 1;

	m_bOfficeXPStyleMenus = TRUE;

	globalData.UpdateSysColors ();
	OnUpdateSystemColors ();
}
//**********************************************************************************
CBCGPVisualManagerXP::~CBCGPVisualManagerXP()
{
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnUpdateSystemColors ()
{
	m_brBarBkgnd.DeleteObject ();
	m_brMenuLight.DeleteObject ();
	m_brHighlight.DeleteObject ();
	m_brHighlightDn.DeleteObject ();
	m_brHighlightChecked.DeleteObject ();

	m_brFloatToolBarBorder.DeleteObject ();

	m_penSeparator.DeleteObject ();
	m_brTabBack.DeleteObject ();

	COLORREF clrTabBack;
	COLORREF clrSeparator;
	COLORREF clrFloatToolBarBorder;

	if (globalData.m_nBitsPerPixel > 8)
	{
		COLORREF clrWindow = globalData.clrBarWindow;
		COLORREF clrFace = globalData.clrBarFace;

		m_clrMenuLight = CBCGPDrawManager::PixelAlpha (
			clrWindow, .976, .973, .969);

		double H, S, L;
		CBCGPDrawManager::RGBtoHSL (clrFace, &H, &S, &L);

		double S1;
		double L1;

		if (S < 0.1)
		{
			L1 = min (1., L + (1. - L) * .5);
			S1 = S == 0 ? 0 : min (1., S + .1);
		}
		else
		{
			L1 = min (1., 0.5 * L + 0.5);
			S1 = min (1., S * 2);
		}

		clrTabBack = CBCGPDrawManager::HLStoRGB_ONE (H, L1, S1);
		if (L > .9)
		{
			m_clrBarBkgnd = clrFace;
		}
		else
		{
			m_clrBarBkgnd = CBCGPDrawManager::HLStoRGB_ONE (H, L * 1.03, S);
		}
		m_clrInactiveTabText = CBCGPDrawManager::HLStoRGB_ONE (H, L * .4, S);

		COLORREF clrHL = globalData.clrHilite;
		m_clrHighlight = CBCGPDrawManager::PixelAlpha (RGB (
			(GetRValue (clrHL) + 2 * GetRValue (m_clrMenuLight)) / 3,
			(GetGValue (clrHL) + 2 * GetGValue (m_clrMenuLight)) / 3,
			(GetBValue (clrHL) + 2 * GetBValue (m_clrMenuLight)) / 3), 
			105);

		m_clrHighlightDn = CBCGPDrawManager::PixelAlpha (
			m_clrHighlight, .87, .87, .87);

		m_clrHighlightChecked = CBCGPDrawManager::PixelAlpha (RGB (
			(GetRValue (clrHL) + 5 * GetRValue (m_clrMenuLight)) / 6,
			(GetGValue (clrHL) + 5 * GetGValue (m_clrMenuLight)) / 6,
			(GetBValue (clrHL) + 5 * GetBValue (m_clrMenuLight)) / 6), 
			100);

		clrSeparator = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, .86, .86, .86);

		m_clrPaneBorder = globalData.clrBarShadow;

		m_clrMenuBorder = CBCGPDrawManager::PixelAlpha (
			clrFace, 55);

		clrFloatToolBarBorder = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarShadow, .85, .85, .85);

		m_clrGripper = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarShadow, 110);
	}
	else
	{
		m_clrMenuLight = globalData.clrWindow;

		m_clrBarBkgnd = globalData.clrBtnFace;
		m_clrHighlight = globalData.clrBtnFace;
		m_clrHighlightDn = globalData.clrBtnFace;
		m_clrHighlightChecked = globalData.clrWindow;
		clrTabBack = globalData.clrBtnFace;
		m_clrInactiveTabText = globalData.clrBtnDkShadow;
		clrSeparator = globalData.clrBtnShadow;
		m_clrGripper = globalData.clrBtnShadow;
		m_clrPaneBorder = globalData.clrBtnShadow;
		m_clrMenuBorder = globalData.clrBtnDkShadow;
		clrFloatToolBarBorder = globalData.clrBtnShadow;
	}

	m_brBarBkgnd.CreateSolidBrush (m_clrBarBkgnd);
	m_brMenuLight.CreateSolidBrush (m_clrMenuLight);

	m_brHighlight.CreateSolidBrush (m_clrHighlight);
	m_brHighlightDn.CreateSolidBrush (m_clrHighlightDn);
	m_brHighlightChecked.CreateSolidBrush (m_clrHighlightChecked);
	m_brTabBack.CreateSolidBrush (clrTabBack);
	m_penSeparator.CreatePen (PS_SOLID, 1, clrSeparator);

	m_clrMenuItemBorder = globalData.clrHilite;

	m_brFloatToolBarBorder.CreateSolidBrush (clrFloatToolBarBorder);

	m_clrMenuShadowBase = (COLORREF)-1;	// Used in derived classes
}
//***************************************************************************************
void CBCGPVisualManagerXP::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz,
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pDC);

	if (pBar != NULL && pBar->IsDialogControl ())
	{
		CBCGPVisualManager::OnDrawBarGripper (pDC, rectGripper, bHorz, pBar);
		return;
	}

	if (m_brGripperHorz.GetSafeHandle () == NULL)
	{
		CreateGripperBrush ();
	}

	BOOL bSideBar = pBar->IsKindOf (RUNTIME_CLASS (CBCGPDockingControlBar));
	BOOL bMenuBar = pBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar));

	CRect rectFill = rectGripper;

	if (!bSideBar)
	{
		if (bHorz)
		{
			int xCenter = rectFill.CenterPoint ().x;
			rectFill.left = xCenter - 1;
			rectFill.right = xCenter + 2;
			rectFill.DeflateRect (0, 5);
		}
		else
		{
			int yCenter = rectFill.CenterPoint ().y;
			rectFill.top = yCenter - 1;
			rectFill.bottom = yCenter + 2;
			rectFill.DeflateRect (5, 0);
		}
	}
	else
	{
		if (bHorz)
		{
			rectFill.DeflateRect (4, 0);
		}
		else
		{
			rectFill.DeflateRect (4, 0);
		}

		bHorz = !bHorz;
	}

	COLORREF clrTextOld = pDC->SetTextColor (m_clrGripper);
	COLORREF clrBkOld = pDC->SetBkColor (bSideBar || bMenuBar ? 
		globalData.clrBarFace : m_clrBarBkgnd);

	pDC->FillRect (rectFill, bHorz ? &m_brGripperHorz : &m_brGripperVert);

	if (bSideBar)
	{
		//------------------
		// Draw bar caption:
		//------------------
		int nOldBkMode = pDC->SetBkMode (OPAQUE);
		pDC->SetTextColor (globalData.clrBarText);

		const CFont& font = CBCGPMenuBar::GetMenuFont (bHorz);

		CFont* pOldFont = pDC->SelectObject ((CFont*) &font);

		CString strCaption;
		pBar->GetWindowText (strCaption);
		strCaption = _T(" ") + strCaption + _T(" ");

		CRect rectText = rectGripper;
		UINT uiTextFormat = 0;

		TEXTMETRIC tm;
		pDC->GetTextMetrics (&tm);

		CPoint ptTextOffset (0, 0);
		if (bHorz)
		{
			ptTextOffset.y = (rectGripper.Height () - tm.tmHeight - 1) / 2;
		}
		else
		{
			ptTextOffset.x = (rectGripper.Width () - tm.tmHeight + 1) / 2;
		}

		if (bHorz)
		{
			rectText.top += ptTextOffset.y;
			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}
		else
		{
			rectText.left = rectText.right - ptTextOffset.x;
			rectText.top = rectGripper.top + ptTextOffset.y;
			rectText.bottom = rectGripper.top + 3 * ptTextOffset.y;

			uiTextFormat |= DT_NOCLIP;

			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}

		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nOldBkMode);
	}

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pMenu);

	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, m_clrMenuLight, m_clrMenuLight);

	CRect rectLeft (1, 1, 2, rect.bottom - 1);
	pDC->FillRect (rectLeft, &m_brBarBkgnd);

	//------------------------------------------------
	// Quick Customize Office XP like draw popup menu
	//------------------------------------------------
	CBCGPPopupMenu* pParentPopup = pMenu->GetParentPopupMenu();
	if (pParentPopup != NULL)
	{
		if (pParentPopup->IsQuickCustomize())
		{
			CBCGPToolbarMenuButton* pParentBtn = pMenu->GetParentButton();
			if ((pParentBtn != NULL) && (pParentBtn->IsQuickMode()))
			{
				CBCGPPopupMenu* pParent = (CBCGPPopupMenu*)pMenu->GetParentPopupMenu();

				CRect rcParent;
				pParent->GetWindowRect(rcParent);

				CRect rcCurrent;
				pMenu->GetWindowRect(rcCurrent);

				CBCGPToolbarMenuButton* pBtn = pMenu->GetMenuItem(0);
				CRect rcButton = pBtn->Rect();

				CRect rectBorder;
				rectBorder.SetRectEmpty();

				if (rcParent.left > rcCurrent.left) 
				{
					rectBorder.SetRect(rect.right-1 , rect.top, rect.right+ 1, rcButton.bottom - 4);
				}
				else
				{
					rectBorder.SetRect(rect.left - 1 , rect.top, rect.left + 1 , rcButton.bottom - 4);
				}
				
				pDC->FillRect (rectBorder, &m_brBarBkgnd);
			}
		}
	}

	if (!CBCGPToolBar::IsCustomizeMode ())
	{
		//-------------------------------------
		// "Connect" menu to the parent button:
		//-------------------------------------
		CBCGPToolbarMenuButton* pParentMenuBtn = pMenu->GetParentButton ();
		if (m_bConnectMenuToParent &&
			pParentMenuBtn != NULL && pMenu->GetParentPopupMenu () == NULL && 
			pParentMenuBtn->IsBorder ())
		{
			CRect rectConnect;
			rectConnect.SetRectEmpty ();

			CRect rectParent = pParentMenuBtn->Rect ();
			CWnd* pWnd = pParentMenuBtn->GetParentWnd();
			pWnd->ClientToScreen(rectParent);
			pMenu->ScreenToClient(&rectParent);

			switch (pMenu->GetDropDirection ())
			{
			case CBCGPPopupMenu::DROP_DIRECTION_BOTTOM:
				rectConnect = CRect (rectParent.left + 1, rect.top - 1, rectParent.right - 2, rect.top);
				
				if (rectConnect.Width () > rect.Width () + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_TOP:
				rectConnect = CRect (rectParent.left + 1, rect.bottom, rectParent.right - 2, rect.bottom + 1);

				if (rectConnect.Width () > rect.Width () + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_RIGHT:
				rectConnect = CRect (rect.left - 1, rectParent.top + 1, rect.left, rectParent.bottom - 2);

				if (rectConnect.Height () > rect.Height () + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_LEFT:
				rectConnect = CRect (rect.right, rectParent.top + 1, rect.right + 1, rectParent.bottom - 2);

				if (rectConnect.Height () > rect.Height () + 2)
				{
					return;
				}

				break;
			}

			CRect rectBorder = rect;
			rectBorder.InflateRect (1, 1);
			rectConnect.IntersectRect (&rectConnect, &rectBorder);
			rectParent.InflateRect (1, 1);
			rectConnect.IntersectRect (&rectConnect, &rectParent);

			pDC->FillRect (rectConnect, &m_brBarBkgnd);
		}
	}
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuShadow (CDC* pPaintDC, const CRect& rectClient, const CRect& rectExclude,
								int nDepth,  int iMinBrightness,  int iMaxBrightness,  
								CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight)
{
	ASSERT_VALID (pPaintDC);
	ASSERT_VALID (pBmpSaveBottom);
	ASSERT_VALID (pBmpSaveRight);

	if (rectExclude.IsRectNull())
	{
		//------------------------
		// Simple draw the shadow:
		//------------------------
		CBCGPDrawManager dm (*pPaintDC);
		dm.DrawShadow (rectClient, nDepth, iMinBrightness, iMaxBrightness,
					pBmpSaveBottom, pBmpSaveRight, m_clrMenuShadowBase);
	}
	else
	{
		//--------------------------------------------
		// Copy screen content into the memory bitmap:
		//--------------------------------------------
		CDC dcMem;
		if (!dcMem.CreateCompatibleDC (pPaintDC))
		{
			ASSERT (FALSE);
			return;
		}

		//--------------------------------------------
		// Gets the whole menu and changes the shadow.
		//--------------------------------------------
		CRect rectBmp (0, 0, rectClient.Width(), rectClient.Height());
		int cx = rectBmp.Width() + nDepth;
		int cy = rectBmp.Height() + nDepth;
		CBitmap	bmpMem;
		if (!bmpMem.CreateCompatibleBitmap (pPaintDC, cx, cy))
		{
			ASSERT (FALSE);
			return;
		}

		CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);
		ASSERT (pOldBmp != NULL);

		dcMem.BitBlt (0, 0, cx, cy, pPaintDC, rectClient.left, rectClient.top, SRCCOPY);

		//-----------------
		// Draw the shadow:
		//-----------------
		CBCGPDrawManager dm (dcMem);
		dm.DrawShadow (rectBmp, nDepth, iMinBrightness, iMaxBrightness,
					pBmpSaveBottom, pBmpSaveRight, m_clrMenuShadowBase);

		//------------------------------------------
		// Do not cover rectExclude with the shadow:
		//------------------------------------------
		dcMem.BitBlt (rectExclude.left - rectClient.left, rectExclude.top - rectClient.top,
			rectExclude.Width(), rectExclude.Height(), 
			pPaintDC, rectExclude.left, rectExclude.top, SRCCOPY);

		//-----------------------------------------
		// Copy shadowed bitmap back to the screen:
		//-----------------------------------------
		pPaintDC->BitBlt (rectClient.left, rectClient.top, cx, cy, &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pOldBmp);
	}
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	if (pBar->IsDialogControl ())
	{
		CBCGPVisualManager::OnDrawBarBorder (pDC, pBar, rect);
		return;
	}

	DWORD dwBarStyle = pBar->GetBarStyle ();
	if (!(dwBarStyle & CBRS_BORDER_ANY))
	{
		return;
	}

	COLORREF clrBckOld = pDC->GetBkColor ();	// FillSolidRect changes it

	if (dwBarStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, 0, 1, rect.Height() - 1, globalData.clrBarFace);
	if (dwBarStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.Width()-1 , 1, globalData.clrBarFace);
	if (dwBarStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect.right, 0/*RGL~:1*/, -1,
			rect.Height()/*RGL-: - 1*/, globalData.clrBarFace);	
	if (dwBarStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect.bottom, rect.Width()-1, -1, globalData.clrBarFace);

	if (dwBarStyle & CBRS_BORDER_LEFT)
		++rect.left;
	if (dwBarStyle & CBRS_BORDER_TOP)
		++rect.top;
	if (dwBarStyle & CBRS_BORDER_RIGHT)
		--rect.right;
	if (dwBarStyle & CBRS_BORDER_BOTTOM)
		--rect.bottom;

	// Restore Bk color:
	pDC->SetBkColor (clrBckOld);
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL /*bNCArea*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (rectClip.IsRectEmpty ())
	{
		rectClip = rectClient;
	}

	CRuntimeClass* pBarClass = pBar->GetRuntimeClass ();

	if (pBarClass == NULL || pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPMenuBar)))
	{
		CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPOutlookBarPane)))
	{
		CBCGPOutlookBarPane* pOlBar = DYNAMIC_DOWNCAST (CBCGPOutlookBarPane, pBar);
		ASSERT_VALID (pOlBar);

		if (pOlBar->IsBackgroundTexture ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
			return;
		}
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPColorBar)))
	{
		if (pBar->IsDialogControl ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, 
				((CBCGPColorBar*) pBar)->IsTearOff () ?
					&m_brBarBkgnd : &m_brMenuLight);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalculator)))
	{
		if (pBar->IsDialogControl ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, &m_brMenuLight);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalendarBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		CRect rectImages = rectClient;
		rectImages.right = rectImages.left + CBCGPToolBar::GetMenuImageSize ().cx + 
					2 * GetMenuImageMargin () + 1;
		return;
	}

    if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		BOOL bQuickMode = FALSE;

		CBCGPPopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pBar);
		if (!pMenuBar->m_bDisableSideBarInXPMode)
		{
			CWnd* pWnd = pMenuBar->GetParent();

			if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CBCGPPopupMenu)))
			{
				CBCGPPopupMenu* pMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, pWnd);

				if (pMenu->IsCustomizePane())
				{
					bQuickMode = TRUE;
				}
			}


			CRect rectImages = rectClient;

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

			pDC->FillRect (rectImages, &m_brBarBkgnd);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPToolBar)))
	{
		if (pBar->IsDialogControl ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, &m_brBarBkgnd);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPAutoHideDockBar)))
	{
		pDC->FillRect (rectClip, &m_brTabBack);
		return;
	}

	CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (pBar->IsDialogControl ())
	{
		CBCGPVisualManager::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	CRect rectSeparator = rect;

	CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
	ASSERT (pOldPen != NULL);

	int x1, x2;
	int y1, y2;

	if (bHorz)
	{
		x1 = x2 = (rect.left + rect.right) / 2;
		y1 = rect.top;
		y2 = rect.bottom - 1;
	}
	else
	{
		y1 = y2 = (rect.top + rect.bottom) / 2;
		x1 = rect.left;
		x2 = rect.right;

		if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)) &&
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPColorBar)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalculator)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalendarBar)))
		{
			
			x1 = rect.left + CBCGPToolBar::GetMenuImageSize ().cx + 
						GetMenuImageMargin () + 1;

			x2 = 32767;

			//---------------------------------
			//	Maybe Quick Customize separator
			//---------------------------------
			if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
			{
				CWnd* pWnd = pBar->GetParent();
				if (pWnd != NULL && pWnd->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenu)))
				{
					CBCGPPopupMenu* pMenu = (CBCGPPopupMenu*)pWnd;
					if (pMenu->IsCustomizePane())
					{
						x1 = rect.left + 2*CBCGPToolBar::GetMenuImageSize ().cx + 
								3*GetMenuImageMargin () + 2;
					}
				}
			}
		}
	}

	pDC->MoveTo (x1, y1);
	pDC->LineTo (x2, y2);

	pDC->SelectObject (pOldPen);
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (state != ButtonsIsPressed && state != ButtonsIsHighlighted)
	{
		ASSERT ((pButton->m_nStyle & TBBS_CHECKED) == 0);
		return;
	}

	COLORREF clrBorder = m_clrMenuItemBorder;

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	if (!bIsPopupMenu || state != ButtonsIsHighlighted)
	{
		rect.right--;
		rect.bottom--;
	}

	if (bIsMenuButton && !bIsPopupMenu && 
		pMenuButton->IsDroppedDown ())
	{
		CBCGPPopupMenu* pPopupMenu= pMenuButton->GetPopupMenu ();
		if (pPopupMenu != NULL && 
			(pPopupMenu->IsWindowVisible () || pPopupMenu->IsShown()))
		{
			clrBorder = m_clrMenuBorder;
			ExtendMenuButton (pMenuButton, rect);

			if (CBCGPMenuBar::IsMenuShadows () &&
				!CBCGPToolBar::IsCustomizeMode () &&
				globalData.m_nBitsPerPixel > 8)
			{
				CBCGPDrawManager dm (*pDC);
				dm.DrawShadow (rect, m_nMenuShadowDepth, 100, 75, NULL, NULL,
					m_clrMenuShadowBase);
			}
		}
	}

	switch (state)
	{
	case ButtonsIsPressed:
	case ButtonsIsHighlighted:
		if (bIsPopupMenu && (pButton->m_nStyle & TBBS_CHECKED))
		{
			rect.bottom ++;
		}

		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}
}
//*****************************************************************************************
void CBCGPVisualManagerXP::OnFillButtonInterior (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (state != ButtonsIsPressed && state != ButtonsIsHighlighted)
	{
		return;
	}

	if (CBCGPToolBar::IsCustomizeMode ())
	{
		return;
	}

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	if (!bIsPopupMenu && !m_bEnableToolbarButtonFill)
	{
		return;
	}

	if (!bIsPopupMenu || state != ButtonsIsHighlighted)
	{
		rect.right --;
		rect.bottom --;
	}

	CBrush* pBrush = ((pButton->m_nStyle & TBBS_PRESSED) && !bIsPopupMenu) ? 
		&m_brHighlightDn : &m_brHighlight;

	if (bIsMenuButton && !bIsPopupMenu && pMenuButton->IsDroppedDown ())
	{
		ExtendMenuButton (pMenuButton, rect);
		pBrush = &m_brBarBkgnd;
	}
	
	if (pButton->m_nStyle & TBBS_CHECKED)
	{
		pBrush = (state == ButtonsIsHighlighted) ? 
				&m_brHighlightDn : &m_brHighlightChecked;
	}

	if (bIsMenuButton && (pButton->m_nStyle & TBBS_DISABLED))
	{
		pBrush = &m_brMenuLight;
	}

	switch (state)
	{
	case ButtonsIsPressed:
	case ButtonsIsHighlighted:
		if ((pButton->m_nStyle & TBBS_CHECKED) == 0)
		{
			rect.DeflateRect (1, 1);
		}

		OnFillHighlightedArea (pDC, rect, pBrush, pButton);
	}
}
//************************************************************************************
void CBCGPVisualManagerXP::OnHighlightMenuItem (CDC* pDC, CBCGPToolbarMenuButton* pButton,
											CRect rect, COLORREF& clrText)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CBrush* pBrush = (pButton->m_nStyle & TBBS_DISABLED) ? 
					&m_brMenuLight : &m_brHighlight;

	OnFillHighlightedArea (pDC, rect, pBrush, pButton);

	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	//-------------------------
	//For Quick Customize mode
	//-------------------------

	CBCGPPopupMenu* pMenu = pButton->GetPopupMenu();

	if (pButton->IsQuickMode() && (pMenu != NULL) 
							  && (pMenu->GetSafeHwnd() != NULL))
	{

		CRect rectDefault = rect;
		CBCGPPopupMenuBar* pMenuBar = (CBCGPPopupMenuBar*)pButton->GetParentWnd();
		CBCGPPopupMenu* pPopupMenu = (CBCGPPopupMenu*)pMenuBar->GetParent();

		CRect rcFrame;
	    CWnd* pWnd = BCGCBProGetTopLevelFrame(pMenuBar);
		pWnd->GetWindowRect(rcFrame);

		CRect rcMenu;
		pPopupMenu->GetWindowRect(rcMenu);

		if ((rcFrame.left - rcMenu.left) < 200)
		{
			rectDefault.right += 10;
		}

		if ((rcFrame.right - rcMenu.right) < 200)
		{
			rectDefault.left -= 10;
		}
	

		pDC->FillRect (rectDefault, &m_brBarBkgnd);
		pDC->Draw3dRect (rectDefault, m_clrMenuBorder, m_clrMenuBorder); 
	}
	
	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	int nRectImagesRight = 2 * CBCGPToolBar::GetMenuImageSize ().cx + 
					2 * GetMenuImageMargin () + 1;
	if (bIsPopupMenu && rect.left > GetMenuImageMargin () - 1 &&
		rect.left <= nRectImagesRight)
	{
		CRect rectConnect = rect;

		rectConnect.right = rect.left + 1;
		rectConnect.left = CBCGPToolBar::GetMenuImageSize ().cx + 
			2 * GetMenuImageMargin () - 1;

		if (pButton->m_nStyle & TBBS_CHECKED)
		{
			rectConnect.left++;
		}

		pDC->Draw3dRect (rectConnect, m_clrMenuItemBorder, m_clrMenuItemBorder);
		rectConnect.DeflateRect (0, 1);
		OnFillHighlightedArea (pDC, rectConnect, pBrush, pButton);
	}

	// Now, we should define a menu text color...
	if (pButton->m_nStyle & TBBS_DISABLED)
	{
		clrText = globalData.clrGrayedText;
	}
	else
	{
		if (GetRValue (m_clrHighlight) > 128 &&
			GetGValue (m_clrHighlight) > 128 &&
			GetBValue (m_clrHighlight) > 128)
		{
			clrText = RGB (0, 0, 0);
		}
		else
		{
			clrText = RGB (255, 255, 255);
		}
	}
}
//************************************************************************************
void CBCGPVisualManagerXP::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	rectRarelyUsed.left --;
	rectRarelyUsed.right = rectRarelyUsed.left + CBCGPToolBar::GetMenuImageSize ().cx + 
		2 * GetMenuImageMargin () + 2;
	dm.HighlightRect (rectRarelyUsed, 94);

}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	#define TEXT_MARGIN				4
	#define IMAGE_MARGIN			4

	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	if (pTabWnd->IsFlatTab () || pTabWnd->IsOneNoteStyle () ||
		pTabWnd->IsColored ())
	{
		CBCGPVisualManager::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	COLORREF	clrDark;
	COLORREF	clrBlack;
	COLORREF	clrHighlight;
	COLORREF	clrFace;
	COLORREF	clrDarkShadow;
	COLORREF	clrLight;
	CBrush*		pbrFace = NULL;
	CBrush*		pbrBlack = NULL;
				   
	GetTabFrameColors (
		pTabWnd, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight,
		pbrFace, pbrBlack);

	CPen penGray (PS_SOLID, 1, clrDark);
	CPen penDkGray (PS_SOLID, 1, clrBlack);
	CPen penHiLight (PS_SOLID, 1, clrHighlight);
	
	CPen* pOldPen = pDC->SelectObject (&penGray);
	ASSERT (pOldPen != NULL);
	
	if (iTab != pTabWnd->GetActiveTab () - 1)
	{
		pDC->MoveTo (rectTab.right, rectTab.top + 3);
		pDC->LineTo (rectTab.right, rectTab.bottom - 3);
	}

	if (bIsActive)
	{
		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			CRect rectFace = rectTab;
			rectFace.top -= pTabWnd->IsFlatFrame () ? 2 : 3;
			
			OnFillTab (pDC, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
			
			pDC->SelectObject (&penDkGray);

			pDC->MoveTo (rectTab.right, rectTab.top);
			pDC->LineTo (rectTab.right, rectTab.bottom);
			pDC->LineTo (rectTab.left, rectTab.bottom);

			pDC->SelectObject (&penHiLight);
			pDC->LineTo (rectTab.left, rectTab.top);
		}
		else
		{
			CPen penLight (PS_SOLID, 1, m_clrMenuLight);

			CRect rectFace = rectTab;
			rectFace.bottom += 3;
			rectFace.left ++;
			
			OnFillTab (pDC, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
			
			pDC->SelectObject (&penDkGray);
			pDC->MoveTo (rectTab.right, rectTab.bottom);
			pDC->LineTo (rectTab.right, rectTab.top);

			pDC->SelectObject (&penHiLight);

			pDC->LineTo (rectTab.right, rectTab.top);
			pDC->LineTo(rectTab.left, rectTab.top);
			pDC->LineTo(rectTab.left, rectTab.bottom);
		}
	}

	pDC->SelectObject (pOldPen);

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd,
		bIsActive ? globalData.clrBarText : m_clrInactiveTabText);
}
//*********************************************************************************
void CBCGPVisualManagerXP::OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill,
									 int iTab, BOOL bIsActive, 
									 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pbrFill);
	ASSERT_VALID (pTabWnd);

	if (!bIsActive && pTabWnd->GetTabBkColor (iTab) != (COLORREF)-1)
	{
		CBrush br (pTabWnd->GetTabBkColor (iTab));
		pDC->FillRect (rectFill, &br);
	}
	else
	{
		if (bIsActive)
		{
			pDC->FillRect (rectFill, pbrFill);
		}
	}
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (pTabWnd->IsFlatTab ())
	{
		CBCGPVisualManager::OnEraseTabsArea (pDC, rect, pTabWnd);
		return;
	}

	pDC->FillRect (rect, &m_brTabBack);
}
//**************************************************************************************
COLORREF CBCGPVisualManagerXP::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* /*pBar*/, 
			BOOL bActive, CRect rectCaption, CRect /*rectButtons*/)
{
	ASSERT_VALID (pDC);

	CPen pen (PS_SOLID, 1, 
		bActive ? globalData.clrBarLight : globalData.clrBarShadow);
	CPen* pOldPen = pDC->SelectObject (&pen);

	CBrush* pOldBrush = (CBrush*) pDC->SelectObject (
		bActive ? &globalData.brActiveCaption : &globalData.brBarFace);

	if (bActive)
	{
		rectCaption.InflateRect (1, 1);
	}

	pDC->RoundRect (rectCaption, CPoint (2, 2));

	pDC->SelectObject (pOldBrush);
	pDC->SelectObject (pOldPen);

    // get the text color
    COLORREF clrCptnText = bActive ?
        globalData.clrCaptionText :
        globalData.clrBarText;

	return clrCptnText;
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, 
											BOOL bActive,
											BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
											int nImageID /*= -1*/)
{
	ASSERT_VALID (pDC);
	ASSERT (pButton != NULL);

    CRect rc = pButton->GetRect ();

	if (pButton->m_bPushed && pButton->m_bFocused && !bDisabled) 
	{
		OnFillHighlightedArea (pDC, rc, &m_brHighlightDn, NULL);
		bActive = TRUE;
	}
	else if (pButton->m_bPushed || pButton->m_bFocused)
	{
		if (!bDisabled)
		{
			OnFillHighlightedArea (pDC, rc, &m_brHighlight, NULL);
		}

		bActive = FALSE;
	}

	CMenuImages::IMAGES_IDS id = (CMenuImages::IMAGES_IDS)-1;
	
	if (nImageID != -1)
	{
		id = (CMenuImages::IMAGES_IDS)nImageID;
	}
	else
	{
		switch (pButton->GetHit ())
		{
		case HTCLOSE:
		case HTCLOSE_BCG:
			id = CMenuImages::IdClose;
			break;

		case HTMINBUTTON:
			id = bHorz ? 
				bMaximized ? CMenuImages::IdArowLeft : CMenuImages::IdArowRight : 
				bMaximized ? CMenuImages::IdArowDown :  CMenuImages::IdArowUp;
			break;

		case HTMAXBUTTON:
			id = bMaximized ? CMenuImages::IdPinHorz : CMenuImages::IdPinVert;
			break;

		case HTLEFTBUTTON_BCG:
			id = CMenuImages::IdArowBack;
			break;

		case HTRIGHTBUTTON_BCG:
			id = CMenuImages::IdArowForward;
			break;

		case HTMENU_BCG:
			id = CMenuImages::IdArowDown;
			break;

		default:
			ASSERT(FALSE);
		}
	}

	if (id != (CMenuImages::IMAGES_IDS)-1)
	{
		CSize sizeImage = CMenuImages::Size ();
		CPoint ptImage (rc.left + (rc.Width () - sizeImage.cx) / 2,
						rc.top + (rc.Height () - sizeImage.cy) / 2);

		OnDrawCaptionButtonIcon (pDC, pButton, id, bActive, bDisabled, ptImage);
	}

	if ((pButton->m_bPushed || pButton->m_bFocused) && !bDisabled)
	{
		COLORREF clrDark = globalData.clrBarDkShadow;
		pDC->Draw3dRect (rc, clrDark, clrDark);
	}
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawCaptionButtonIcon (CDC* pDC, 
													CBCGPCaptionButton* pButton,
													CMenuImages::IMAGES_IDS id,
													BOOL bActive, BOOL bDisabled,
													CPoint ptImage)
{
	ASSERT_VALID (pDC);

	CMenuImages::IMAGE_STATE imageState;
	
	if (bDisabled)
	{
		imageState = CMenuImages::ImageGray;
	}
	else if (pButton->m_clrForeground == (COLORREF)-1)
	{
		imageState = bActive ? CMenuImages::ImageWhite : CMenuImages::ImageBlack;
	}
	else
	{
		if (GetRValue (pButton->m_clrForeground) > 192 &&
			GetGValue (pButton->m_clrForeground) > 192 &&
			GetBValue (pButton->m_clrForeground) > 192)
		{
			imageState = CMenuImages::ImageWhite;
		}
		else
		{
			imageState = CMenuImages::ImageBlack;
		}
	}

	CMenuImages::Draw (pDC, id, ptImage, imageState);
}
//********************************************************************************
COLORREF CBCGPVisualManagerXP::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	COLORREF clrText = globalData.clrBarText;

	int iImageWidth = CBCGPToolBar::GetMenuImageSize ().cx + GetMenuImageMargin ();

	if (bIsSelected)
	{
		if (m_bEnableToolbarButtonFill)
		{
			rect.left = 0;
		}

		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);

		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

		// Now, we should define a menu text color...
		if (GetRValue (m_clrHighlight) > 128 &&
			GetGValue (m_clrHighlight) > 128 &&
			GetBValue (m_clrHighlight) > 128)
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

		pDC->FillRect (rectImages, &m_brBarBkgnd);

		clrText = globalData.clrBarText;
	}

	return clrText;
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuArrowOnCustomizeList (CDC* pDC, 
	CRect rectCommand, BOOL /*bSelected*/)
{
	ASSERT_VALID (pDC);

	CRect rectTriangle = rectCommand;
	rectTriangle.left = rectTriangle.right - CMenuImages::Size ().cx;

	CPoint pointTriangle (
		rectTriangle.left + (rectTriangle.Width () - CMenuImages::Size ().cx) / 2,
		rectTriangle.top + (rectTriangle.Height () - CMenuImages::Size ().cy) / 2);

	CMenuImages::IMAGE_STATE state = CMenuImages::ImageBlack;
	COLORREF clrPen = RGB (0, 0, 0);

	if (GetRValue (m_clrHighlight) < 128 ||
		GetGValue (m_clrHighlight) < 128 ||
		GetBValue (m_clrHighlight) < 128)
	{
		state = CMenuImages::ImageWhite;
		clrPen = RGB (255, 255, 255);
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArowRightLarge, pointTriangle, state);

	CPen penLine (PS_SOLID, 1, clrPen);
	CPen* pOldPen = pDC->SelectObject (&penLine);
	ASSERT (pOldPen != NULL);

	pDC->MoveTo (rectTriangle.left - 1, rectCommand.top + 2);
	pDC->LineTo (rectTriangle.left - 1, rectCommand.bottom - 2);

	pDC->SelectObject (pOldPen);
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	const int iBorderSize = 1;
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);

	rect.DeflateRect (iBorderSize, 1);
	OnFillHighlightedArea (pDC, rect, bIsActive ? &m_brHighlight : &m_brBarBkgnd,
		NULL);
	
	// Draw gripper:
	int nGripperWidth = max (20, CBCGPToolBar::GetMenuImageSize ().cx * 2);

	CRect rectGripper = rect;
	rectGripper.DeflateRect ((rectGripper.Width () - nGripperWidth) / 2, 1);

	if (m_brGripperHorz.GetSafeHandle () == NULL)
	{
		CreateGripperBrush ();
	}

	COLORREF clrTextOld = pDC->SetTextColor (bIsActive ?
		globalData.clrBarDkShadow : globalData.clrBarShadow);
	COLORREF clrBkOld = pDC->SetBkColor (
		bIsActive ? m_clrHighlight : m_clrBarBkgnd);

	if (bIsActive)
	{
		rectGripper.DeflateRect (0, 1);
	}

	pDC->FillRect (rectGripper, &m_brGripperHorz);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);

	if (bIsActive)
	{
		pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
	}
}
//***********************************************************************************
COLORREF CBCGPVisualManagerXP::OnFillCalendarBarNavigationRow (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);

	CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
	ASSERT (pOldPen != NULL);

	pDC->MoveTo (rect.left, rect.bottom);
	pDC->LineTo (rect.right, rect.bottom);

	pDC->SelectObject (pOldPen);
    
	return globalData.clrBarText;
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnFillCalendarBarWeekColumn (CDC* pDC, CRect rect,
								  COLORREF& clrText, COLORREF& clrTextDisabled)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);

	clrText = globalData.clrBarText;
	clrTextDisabled = globalData.clrGrayedText;
}
//************************************************************************************
void CBCGPVisualManagerXP::CreateGripperBrush ()
{
	ASSERT (m_brGripperHorz.GetSafeHandle () == NULL);
	ASSERT (m_brGripperVert.GetSafeHandle () == NULL);

	WORD horzHatchBits [8] = { 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00 };

	CBitmap bmpGripperHorz;
	bmpGripperHorz.CreateBitmap (8, 8, 1, 1, horzHatchBits);

	m_brGripperHorz.CreatePatternBrush (&bmpGripperHorz);

	WORD vertHatchBits[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

	CBitmap bmpGripperVert;
	bmpGripperVert.CreateBitmap (8, 8, 1, 1, vertHatchBits);

	m_brGripperVert.CreatePatternBrush (&bmpGripperVert);
}
//***********************************************************************************
void CBCGPVisualManagerXP::ExtendMenuButton (CBCGPToolbarMenuButton* pMenuButton,
											CRect& rect)
{
	ASSERT_VALID (pMenuButton);

	int nGrow = 4;

	CBCGPPopupMenu* pPopupMenu= pMenuButton->GetPopupMenu ();
	if (pPopupMenu != NULL && pPopupMenu->GetSafeHwnd () != NULL)
	{
		CRect rectMenu;
		pPopupMenu->GetWindowRect (rectMenu);

		switch (pPopupMenu->GetDropDirection ())
		{
		case CBCGPPopupMenu::DROP_DIRECTION_BOTTOM:
			if (rectMenu.Width () < rect.Width ())
			{
				nGrow = 1;
			}

			rect.bottom += nGrow;
			break;

		case CBCGPPopupMenu::DROP_DIRECTION_TOP:
			if (rectMenu.Width () < rect.Width ())
			{
				nGrow = 1;
			}

			rect.top -= nGrow;
			break;

		case CBCGPPopupMenu::DROP_DIRECTION_RIGHT:
			if (rectMenu.Height () < rect.Height ())
			{
				nGrow = 1;
			}

			rect.right += nGrow;
			break;

		case CBCGPPopupMenu::DROP_DIRECTION_LEFT:
			if (rectMenu.Height () < rect.Height ())
			{
				nGrow = 1;
			}

			rect.left -= nGrow;
			break;
		}
	}
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuSystemButton (CDC* pDC, CRect rect, UINT uiSystemCommand, 
										UINT nStyle, BOOL bHighlight)
{
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = (nStyle & TBBS_DISABLED);
	BOOL bIsPressed = (nStyle & TBBS_PRESSED);

	CMenuImages::IMAGES_IDS imageID;

	switch (uiSystemCommand)
	{
	case SC_CLOSE:
		imageID = CMenuImages::IdClose;
		break;

	case SC_MINIMIZE:
		imageID = CMenuImages::IdMinimize;
		break;

	case SC_RESTORE:
		imageID = CMenuImages::IdRestore;
		break;

	default:
		return;
	}

	if (bHighlight && !bIsDisabled)
	{
		OnFillHighlightedArea (pDC, rect, 
			bIsPressed ? &m_brHighlightDn : &m_brHighlight, NULL);

		COLORREF clrBorder = m_clrMenuItemBorder;
		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}

	CSize sizeImage = CMenuImages::Size ();
	CPoint ptImage = rect.TopLeft () + 
		CSize ((rect.Width () - sizeImage.cx) / 2, (rect.Height () - sizeImage.cy) / 2);
	CMenuImages::Draw (pDC, imageID, ptImage,
		bIsDisabled ? CMenuImages::ImageGray : bHighlight ? 
			CMenuImages::ImageWhite : CMenuImages::ImageBlack);
}
//********************************************************************************
void CBCGPVisualManagerXP::OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* /*pBar*/,
					CRect rectPane, UINT /*uiID*/, UINT nStyle)
{
	if (!(nStyle & SBPS_NOBORDERS))
	{
		if (nStyle & SBPS_POPOUT)
		{
			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectPane);
		}

		// Draw pane border:
		pDC->Draw3dRect (rectPane, m_clrPaneBorder, m_clrPaneBorder);
	}
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawComboDropButton (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
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
		pDC->FillSolidRect (rect, globalData.clrBarFace);
		pDC->Draw3dRect (rect, globalData.clrBarWindow, globalData.clrBarWindow);
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, pointTriangle,
		bDisabled ? CMenuImages::ImageGray : (bIsDropped && bIsHighlighted) ? 
			CMenuImages::ImageWhite : CMenuImages::ImageBlack);
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	if (bIsHighlighted || bIsDropped)
	{
		rect.DeflateRect (1, 1);
		pDC->Draw3dRect (&rect,  m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//*********************************************************************************
void CBCGPVisualManagerXP::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pWndTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pWndTab);

	if (pWndTab->IsFlatTab ())
	{
		CBrush* pBrush = pButton->IsPressed () ? 
			&m_brHighlightDn : pButton->IsHighlighted () ? &m_brHighlight : &globalData.brBarFace;

		pDC->FillRect (rect, pBrush);
		OnFillHighlightedArea (pDC, rect, pBrush, NULL);
	}
	else
	{
		pDC->FillRect (rect, &m_brTabBack);
	}
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT /*uiState*/,
												 CBCGPBaseTabWnd* pWndTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pWndTab);

	if (pWndTab->IsFlatTab ())
	{
		if (pButton->IsPushed () || pButton->IsHighlighted ())
		{
			COLORREF clrDark = globalData.clrBarDkShadow;
			pDC->Draw3dRect (rect, clrDark, clrDark);
		}
	}
	else
	{
		if (pButton->IsPushed () || pButton->IsHighlighted ())
		{
			if (pButton->IsPressed ())
			{
				pDC->Draw3dRect (rect, globalData.clrBarDkShadow, m_clrGripper);
			}
			else
			{
				pDC->Draw3dRect (rect, m_clrGripper, globalData.clrBarDkShadow);
			}
		}
	}
}
//*********************************************************************************
COLORREF CBCGPVisualManagerXP::OnFillMiniFrameCaption (CDC* pDC, 
								CRect rectCaption, 
								CBCGPMiniFrameWnd* pFrameWnd,
								BOOL bActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	BOOL bIsToolBar = FALSE;
	BOOL bIsTasksPane = pFrameWnd->IsKindOf( RUNTIME_CLASS( CBCGPTaskPaneMiniFrameWnd ) );

	if (DYNAMIC_DOWNCAST (CBCGPBaseToolBar, pFrameWnd->GetControlBar ()) != NULL)
	{
		bActive = FALSE;
		bIsToolBar = TRUE;
	}

	if (bIsToolBar)
	{
		pDC->FillRect (rectCaption, &m_brFloatToolBarBorder);
		return globalData.clrCaptionText;
	}
	else if (bIsTasksPane)
	{
		pDC->FillRect (rectCaption, &globalData.brBarFace);
		return globalData.clrBarText;
	}

	pDC->FillRect (rectCaption, 
		bActive ? &globalData.brActiveCaption : &globalData.brInactiveCaption);

    // get the text color
	return globalData.clrCaptionText;
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	BOOL bIsTasksPane = pFrameWnd->IsKindOf( RUNTIME_CLASS( CBCGPTaskPaneMiniFrameWnd ) );
	if (bIsTasksPane)
	{
		CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
		ASSERT (pOldBrush != NULL);

		pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
		pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
		pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
		pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

		rectBorderSize.DeflateRect (2, 2);
		rectBorder.DeflateRect (2, 2);

		pDC->SelectObject (bIsTasksPane ? &globalData.brLight : &globalData.brBarFace);

		pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 2, PATCOPY);
		pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
		pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 2, PATCOPY);
		pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

		pDC->SelectObject (pOldBrush);
	}
	else
	{
		CBCGPVisualManager::OnDrawMiniFrameBorder (pDC, pFrameWnd, rectBorder, rectBorderSize);
	}
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawFloatingToolbarBorder (
												CDC* pDC, CBCGPBaseToolBar* /*pToolBar*/, 
												CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);

	CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
	ASSERT (pOldBrush != NULL);

	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

	rectBorderSize.DeflateRect (2, 2);
	rectBorder.DeflateRect (2, 2);

	pDC->SelectObject (&globalData.brBarFace);

	pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

	pDC->SelectObject (pOldBrush);
}
//**************************************************************************************
COLORREF CBCGPVisualManagerXP::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton, 
														CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode () && !pButton->IsEditable ()) ||
		(!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED));

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)))
	{
		return bDisabled ? globalData.clrGrayedText : globalData.clrBarText;
	}

	if (state == ButtonsIsHighlighted && 
		(pButton->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)))
	{
		return globalData.clrTextHilite;
	}

	return	CBCGPVisualManager::GetToolbarButtonTextColor (pButton, state);
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnDrawEditBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsHighlighted,
												CBCGPToolbarEditBoxButton* pButton)
{
	if (!CBCGPToolbarEditBoxButton::IsFlatMode ())
	{
		CBCGPVisualManager::OnDrawEditBorder (pDC, rect, bDisabled, bIsHighlighted, pButton);
		return;
	}

	if (bIsHighlighted)
	{
		pDC->Draw3dRect (&rect,  m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
						BOOL bIsHighlighted, BOOL /*bIsSelected*/, BOOL bCanCollapse)
{
	ASSERT_VALID(pDC);
	ASSERT(pGroup != NULL);

	// ------------------------------
	// Draw group caption (Office XP)
	// ------------------------------
	
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
	COLORREF clrTextOld = pDC->SetTextColor (globalData.clrWindowText);
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
		rectButton.left = max(rectButton.left, rectButton.right - sizeButton.cx);
		rectButton.top = max(rectButton.top, rectButton.bottom - sizeButton.cy);
		
		if (rectButton.Width () >= sizeButton.cx && rectButton.Height () >= sizeButton.cy)
		{
			if (bIsHighlighted)
			{
				// Draw button frame
				CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penHilite);
				CBrush* pBrushOld = (CBrush*) pDC->SelectObject (&m_brHighlight);
				COLORREF clrBckOld = pDC->GetBkColor ();

				pDC->Rectangle(&rectButton);

				pDC->SetBkColor (clrBckOld);
				pDC->SelectObject (pPenOld);
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
//**********************************************************************************
void CBCGPVisualManagerXP::OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL /*bSpecial*/)
{
	ASSERT_VALID(pDC);

	// Draw underline
	CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarShadow);
	pDC->MoveTo (rect.left, rect.top);
	pDC->LineTo (rect.right, rect.top);
	pDC->SelectObject (&pPenOld);

}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawTasksGroupAreaBorder(CDC* /*pDC*/, CRect /*rect*/, 
													 BOOL /*bSpecial*/, BOOL /*bNoTitle*/)
{
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pIcons);
	ASSERT(pTask != NULL);

	CRect rectText = pTask->m_rect;

	if (pTask->m_bIsSeparator)
	{
		CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarShadow);

		pDC->MoveTo (rectText.left, rectText.CenterPoint ().y);
		pDC->LineTo (rectText.right, rectText.CenterPoint ().y);

		pDC->SelectObject (&pPenOld);
		return;
	}

	// ---------
	// Draw icon
	// ---------
	CSize sizeIcon(0, 0);
	::ImageList_GetIconSize (pIcons->m_hImageList, (int*) &sizeIcon.cx, (int*) &sizeIcon.cy);
	if (pTask->m_nIcon >= 0 && sizeIcon.cx > 0)
	{
		pIcons->Draw (pDC, pTask->m_nIcon, rectText.TopLeft (), ILD_TRANSPARENT);
	}
	int nTaskPaneOffset = pTask->m_pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	rectText.left += sizeIcon.cx + (nTaskPaneOffset != -1 ? nTaskPaneOffset : m_nTasksIconHorzOffset);

	// ---------
	// Draw text
	// ---------
	BOOL bIsLabel = (pTask->m_uiCommandID == 0);

	CFont* pFontOld = NULL;
	COLORREF clrTextOld = pDC->GetTextColor();
	if (bIsLabel)
	{
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
		pDC->SetTextColor (globalData.clrWindowText);
	}
	else if (!pTask->m_bEnabled)
	{
		pDC->SetTextColor (globalData.clrGrayedText);
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
	}
	else if (bIsHighlighted)
	{
		pDC->SetTextColor (globalData.clrHotText);
		pFontOld = pDC->SelectObject (&globalData.fontUnderline);
	}
	else
	{
		pDC->SetTextColor (globalData.clrHotText);
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
	}
	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText (pTask->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	ASSERT_VALID (pDC);

	CRect rectImage (CPoint (0, 0), CMenuImages::Size ());

	CRect rectFill = rect;
	rectFill.top -= nBorderSize;

	pDC->FillRect (rectFill, &globalData.brWindow);

	if (bHilited)
	{
		pDC->FillRect (rect, &m_brHighlight);
		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}

	CPoint point (
		rect.left + (rect.Width () - rectImage.Width ()) / 2,
		rect.top + (rect.Height () - rectImage.Height ()) / 2);

	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, point);
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawSpinButtons (CDC* pDC, CRect rectSpin, 
	int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* /*pSpinCtrl*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	CRect rect [2];
	rect[0] = rect[1] = rectSpin;

	if (!bOrientation) 
	{
		rect[0].DeflateRect(0, 0, 0, rect[0].Height() / 2);
		rect[1].top = rect[0].bottom ;
	}
	else
	{
		rect[0].DeflateRect(0, 0, rect[0].Width() / 2, 0);
		rect[1].left = rect[0].right ;
	}

	CMenuImages::IMAGES_IDS id[2][2] = {{CMenuImages::IdArowUp, CMenuImages::IdArowDown}, {CMenuImages::IdArowLeft, CMenuImages::IdArowRight}};

	int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
	
	int idxHighlighted = -1;
	if (nState & SPIN_HIGHLIGHTEDUP)
	{
		idxHighlighted = 0;
	}
	else if (nState & SPIN_HIGHLIGHTEDDOWN)
	{
		idxHighlighted = 1;
	}

	BOOL bDisabled = nState & SPIN_DISABLED;

	for (int i = 0; i < 2; i ++)
	{
		CPoint pointTriangle (
			rect[i].left + (rect[i].Width () - CMenuImages::Size ().cx) / 2 ,
			rect[i].top + (rect[i].Height () - CMenuImages::Size ().cy) / 2 );

		if (idxPressed == i || idxHighlighted == i)
		{
			OnFillHighlightedArea (pDC, rect [i], 
				(idxPressed == i) ? &m_brHighlightDn : &m_brHighlight, NULL);
		}
		else
		{
			pDC->FillRect (rect[i], &globalData.brBarFace);
			pDC->Draw3dRect (rect[i], globalData.clrBarHilite, globalData.clrBarHilite);
		}

		CMenuImages::Draw (pDC, id[bOrientation][i], pointTriangle,
			bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
	}

	if (idxHighlighted >= 0)
	{
		CRect rectHot = rect [idxHighlighted];
		pDC->Draw3dRect (rectHot, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnDrawControlBorder (CWnd* pWndCtrl)
{
	CBCGPVisualManager::OnDrawControlBorder (pWndCtrl);
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnFillHighlightedArea (CDC* pDC, CRect rect, 
		CBrush* pBrush, CBCGPToolbarButton* /*pButton*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);

	pDC->FillRect (rect, pBrush);
}
//********************************************************************************
void CBCGPVisualManagerXP::OnDrawSplitterBorder (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
	ASSERT_VALID(pDC);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
	rect.InflateRect(-CX_BORDER, -CY_BORDER);
	pDC->Draw3dRect (rect, globalData.clrBarFace, globalData.clrBarFace);
}
//********************************************************************************
void CBCGPVisualManagerXP::OnDrawSplitterBox (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect& rect)
{
	ASSERT_VALID(pDC);
	pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarFace);
}
//********************************************************************************
BOOL CBCGPVisualManagerXP::OnDrawCalculatorButton (CDC* pDC, 
	CRect rect, CBCGPToolbarButton* pButton, 
	CBCGPVisualManager::BCGBUTTON_STATE state, 
	int /*cmd*/ /* CBCGPCalculator::CalculatorCommands */,
	CBCGPCalculator* /*pCalculator*/)
{
	ASSERT_VALID (pButton);
	ASSERT_VALID (pDC);

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->FillRect (rect, &m_brHighlightDn);
		pDC->SetTextColor (globalData.clrWindow);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &m_brHighlight);
		break;
	}

	pDC->Draw3dRect (&rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManagerXP::OnDrawCalculatorDisplay (CDC* pDC, CRect rect, 
												  const CString& /*strText*/, BOOL /*bMem*/,
												  CBCGPCalculator* /*pCalculator*/)
{
	pDC->FillRect (rect, &globalData.brWindow);
	pDC->Draw3dRect (&rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManagerXP::OnDrawBrowseButton (CDC* pDC, CRect rect, 
	CBCGPEdit* /*pEdit*/, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);

	CRect rectFrame = rect;
	rectFrame.InflateRect (0, 1, 1, 1);

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->FillRect (rect, &m_brHighlightDn);
		pDC->Draw3dRect (&rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);
		pDC->SetTextColor (globalData.clrWindow);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &m_brHighlight);
		pDC->Draw3dRect (&rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);
		break;

	default:
		pDC->FillRect (rect, &globalData.brBtnFace);
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);
		break;
	}

	return TRUE;
}
