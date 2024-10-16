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

// BCGPVisualManager.cpp: implementation of the CBCGPPVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPVisualManager.h"
#include "BCGPToolbarButton.h"
#include "BCGPOutlookBarPane.h"
#include "BCGPOutlookButton.h"
#include "BCGGlobals.h"
#include "BCGPDockingControlBar.h"
#include "BCGPBaseControlBar.h"
#include "BCGPToolBar.h"
#include "BCGPTabWnd.h"
#include "BCGPDrawManager.h"
#include "BCGPShowAllButton.h"
#include "BCGPButton.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPCaptionBar.h"
#include "BCGPOutlookButton.h"
#include "BCGPTasksPane.h"
#include "BCGPSlider.h"
#include "BCGPCalendarBar.h"
#include "MenuImages.h"
#include "BCGPHeaderCtrl.h"
#include "BCGPSpinButtonCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE (CBCGPVisualManager, CObject)

extern CObList	gAllToolbars;

CBCGPVisualManager*	CBCGPVisualManager::m_pVisManager = NULL;
CRuntimeClass*		CBCGPVisualManager::m_pRTIDefault = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManager::CBCGPVisualManager(BOOL bIsTemporary)
{
	m_bAutoDestroy = FALSE;
	m_bIsTemporary = bIsTemporary;

	if (!bIsTemporary)
	{
		if (m_pVisManager != NULL)
		{
			ASSERT (FALSE);
		}
		else
		{
			m_pVisManager = this;
		}
	}

	m_bLook2000 = FALSE;
	m_bMenuFlatLook = FALSE;
	m_nMenuShadowDepth = 6;
	m_bShadowHighlightedImage = FALSE;
	m_bEmbossDisabledImage = TRUE;
	m_bFadeInactiveImage = FALSE;
	m_bEnableToolbarButtonFill = TRUE;

	m_nVertMargin = 12;
	m_nHorzMargin = 12;
	m_nGroupVertOffset = 15;
	m_nGroupCaptionHeight = 25;
	m_nGroupCaptionHorzOffset = 13;
	m_nGroupCaptionVertOffset = 7;
	m_nTasksHorzOffset = 12;
	m_nTasksIconHorzOffset = 5;
	m_nTasksIconVertOffset = 4;
	m_bActiveCaptions = TRUE;

	m_bOfficeXPStyleMenus = FALSE;
	m_nMenuBorderSize = 2;

	globalData.UpdateSysColors ();
}
//*************************************************************************************
CBCGPVisualManager::~CBCGPVisualManager()
{
	if (!m_bIsTemporary)
	{
		m_pVisManager = NULL;
	}
}
//*************************************************************************************
void CBCGPVisualManager::SetDefaultManager (CRuntimeClass* pRTI)
{
	if (pRTI != NULL &&
		!pRTI->IsDerivedFrom (RUNTIME_CLASS (CBCGPVisualManager)))
	{
		ASSERT (FALSE);
		return;
	}

	m_pRTIDefault = pRTI;

	if (m_pVisManager != NULL)
	{
		ASSERT_VALID (m_pVisManager);

		delete m_pVisManager;
		m_pVisManager = NULL;
	}

	globalData.UpdateSysColors ();
	RedrawAll ();
}
//*************************************************************************************
void CBCGPVisualManager::RedrawAll ()
{
	if (AfxGetMainWnd () != NULL)
	{
		AfxGetMainWnd ()->RedrawWindow (NULL, NULL,
					RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}

	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
	{
		CBCGPBaseControlBar* pToolBar = (CBCGPBaseControlBar*) gAllToolbars.GetNext (posTlb);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID (pToolBar);
			
			pToolBar->RedrawWindow (NULL, NULL,
				RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
		}
	}

	CBCGPMiniFrameWnd::RedrawAll ();
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz,
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pDC);

	const COLORREF clrHilite = pBar != NULL && pBar->IsDialogControl () ?
		globalData.clrBtnHilite : globalData.clrBarHilite;
	const COLORREF clrShadow = pBar != NULL && pBar->IsDialogControl () ?
		globalData.clrBtnShadow : globalData.clrBarShadow;

	const BOOL bSingleGripper = m_bLook2000;

	const int iGripperSize = 3;
	const int iGripperOffset = bSingleGripper ? 0 : 1;
	const int iLinesNum = bSingleGripper ? 1 : 2;

	if (bHorz)
	{
		//-----------------
		// Gripper at left:
		//-----------------
		rectGripper.DeflateRect (0, bSingleGripper ? 3 : 2);

		// ET: Center the grippers
		rectGripper.left = iGripperOffset + rectGripper.CenterPoint().x - 
			( iLinesNum*iGripperSize + (iLinesNum-1)*iGripperOffset) / 2;

		rectGripper.right = rectGripper.left + iGripperSize;

 
		for (int i = 0; i < iLinesNum; i ++)
		{
			pDC->Draw3dRect (rectGripper, 
							clrHilite,
							clrShadow);

			// ET: not used for NewFlat look
			if(! bSingleGripper ) {
				//-----------------------------------
				// To look same as MS Office Gripper!
				//-----------------------------------
				pDC->SetPixel (CPoint (rectGripper.left, rectGripper.bottom - 1),
								clrHilite);
			}

			rectGripper.OffsetRect (iGripperSize+1, 0);
		}
	} 
	else 
	{
		//----------------
		// Gripper at top:
		//----------------
		rectGripper.top += iGripperOffset;
		rectGripper.DeflateRect (bSingleGripper ? 3 : 2, 0);

		// ET: Center the grippers
		rectGripper.top = iGripperOffset + rectGripper.CenterPoint().y - 
			( iLinesNum*iGripperSize + (iLinesNum-1)) / 2;

		rectGripper.bottom = rectGripper.top + iGripperSize;

		for (int i = 0; i < iLinesNum; i ++)
		{
			pDC->Draw3dRect (rectGripper,
							clrHilite,
							clrShadow);

			// ET: not used for NewFlat look
			if(! bSingleGripper ) {
				//-----------------------------------
				// To look same as MS Office Gripper!
				//-----------------------------------
				pDC->SetPixel (CPoint (rectGripper.right - 1, rectGripper.top),
								clrHilite);
			}

			rectGripper.OffsetRect (0, iGripperSize+1);
		}
	}
}
//*************************************************************************************
void CBCGPVisualManager::SetLook2000 (BOOL bLook2000)
{
	m_bLook2000 = bLook2000;

	if (AfxGetMainWnd () != NULL)
	{
		AfxGetMainWnd()->RedrawWindow (NULL, NULL,
			RDW_INVALIDATE | RDW_ERASENOW | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_FRAME);
	}

	RedrawAll ();
}
//*************************************************************************************
void CBCGPVisualManager::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL /*bNCArea*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBar);

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPOutlookBarPane)))
	{
		((CBCGPOutlookBarPane*) pBar)->OnEraseWorkArea (pDC, rectClient);
		return;
	}

	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPCaptionBar)))
	{
		CBCGPCaptionBar* pCaptionBar = (CBCGPCaptionBar*) pBar;

		pDC->FillSolidRect	(rectClip, pCaptionBar->m_clrBarBackground == -1 ? 
			globalData.clrBarShadow : pCaptionBar->m_clrBarBackground);
		return;
	}

	// By default, control bar background is filled by 
	// the system 3d background color

	pDC->FillRect (rectClip.IsRectEmpty () ? rectClient : rectClip, 
				pBar->IsDialogControl () ?
					&globalData.brBtnFace : &globalData.brBarFace);
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	if (pBar->IsFloating ())
	{
		return;
	}

	DWORD dwStyle = pBar->GetBarStyle  ();
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	COLORREF clrBckOld = pDC->GetBkColor ();	// FillSolidRect changes it

	// Copyright (C) 1997,'98 by Joerg Koenig
	// All rights reserved
	//

	const COLORREF clrHilite = pBar->IsDialogControl () ?
		globalData.clrBtnHilite : globalData.clrBarHilite;
	const COLORREF clrShadow = pBar->IsDialogControl () ?
		globalData.clrBtnShadow : globalData.clrBarShadow;

	COLORREF clr = clrHilite;

	if(dwStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, 0, 1, rect.Height() - 1, clr);
	if(dwStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.Width()-1 , 1, clr);
	if(dwStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect.right, 0/*RGL~:1*/, -1,
			rect.Height()/*RGL-: - 1*/, clrShadow);
	if(dwStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect.bottom, rect.Width()-1, -1, clrShadow);

	// if undockable toolbar at top of frame, apply special formatting to mesh
	// properly with frame menu
	if(!pBar->CanFloat ()) 
	{
		pDC->FillSolidRect(0,0,rect.Width(),1,clrShadow);
		pDC->FillSolidRect(0,1,rect.Width(),1,clrHilite);
	}

	if (dwStyle & CBRS_BORDER_LEFT)
		++rect.left;
	if (dwStyle & CBRS_BORDER_TOP)
		++rect.top;
	if (dwStyle & CBRS_BORDER_RIGHT)
		--rect.right;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		--rect.bottom;

	// Restore Bk color:
	pDC->SetBkColor (clrBckOld);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* /*pMenu*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, globalData.clrBarLight, globalData.clrBarDkShadow);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawMenuShadow (CDC* pPaintDC, const CRect& rectClient, const CRect& /*rectExclude*/,
								int nDepth,  int iMinBrightness,  int iMaxBrightness,  
								CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight)
{
	ASSERT_VALID (pPaintDC);
	ASSERT_VALID (pBmpSaveBottom);
	ASSERT_VALID (pBmpSaveRight);

	//------------------------------------------------------
	// Simple draw the shadow, ignore rectExclude parameter:
	//------------------------------------------------------
	CBCGPDrawManager dm (*pPaintDC);
	dm.DrawShadow (rectClient, nDepth, iMinBrightness, iMaxBrightness,
				pBmpSaveBottom, pBmpSaveRight);
}
//************************************************************************************
void CBCGPVisualManager::OnFillButtonInterior (CDC* pDC,
				CBCGPToolbarButton* pButton, CRect rect,
				CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPShowAllButton)))
	{
		if (state == ButtonsIsHighlighted)
		{
			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rect);
		}

		return;
	}

	if (!m_bEnableToolbarButtonFill)
	{
		BOOL bIsPopupMenu = FALSE;

		CBCGPToolbarMenuButton* pMenuButton = 
			DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
		if (pMenuButton != NULL)
		{
			bIsPopupMenu = pMenuButton->GetParentWnd () != NULL &&
				pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));
		}

		if (!bIsPopupMenu)
		{
			return;
		}
	}

	if (!pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)) &&
		!CBCGPToolBar::IsCustomizeMode () && state != ButtonsIsHighlighted &&
		(pButton->m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
	{
		CRect rectDither = rect;
		rectDither.InflateRect (-afxData.cxBorder2, -afxData.cyBorder2);

		CBCGPToolBarImages::FillDitheredRect (pDC, rectDither);
	}
}
//************************************************************************************
void CBCGPVisualManager::OnHighlightMenuItem (CDC*pDC, CBCGPToolbarMenuButton* /*pButton*/,
											CRect rect, COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brHilite);
}
//************************************************************************************
void CBCGPVisualManager::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);
	dm.HighlightRect (rectRarelyUsed);

	pDC->Draw3dRect (rectRarelyUsed, globalData.clrBarShadow, globalData.clrBarHilite);
}
//************************************************************************************
void CBCGPVisualManager::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	BOOL bIsOutlookButton = pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton));
	COLORREF clrDark = bIsOutlookButton ? 
					   globalData.clrBarDkShadow : globalData.clrBarShadow;

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->Draw3dRect (&rect, clrDark, globalData.clrBarHilite);
		return;

	case ButtonsIsHighlighted:
		pDC->Draw3dRect (&rect, globalData.clrBarHilite, clrDark);
		return;
	}
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state,
		BOOL /*bHorz*/)
{
	ASSERT_VALID (pButton);

	if (!m_bMenuFlatLook || !pButton->IsDroppedDown ())
	{
		OnDrawButtonBorder (pDC, pButton, rect, state);
	}
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pBar);
	ASSERT_VALID (pDC);

	CRect rectSeparator = rect;

	if (bHorz)
	{
		rectSeparator.left += rectSeparator.Width () / 2 - 1;
		rectSeparator.right = rectSeparator.left + 2;
	}
	else
	{
		rectSeparator.top += rectSeparator.Height () / 2 - 1;
		rectSeparator.bottom = rectSeparator.top + 2;
	}

	const COLORREF clrHilite = pBar->IsDialogControl () ?
		globalData.clrBtnHilite : globalData.clrBarHilite;
	const COLORREF clrShadow = pBar->IsDialogControl () ?
		globalData.clrBtnShadow : globalData.clrBarShadow;

	pDC->Draw3dRect (rectSeparator, clrShadow,
									clrHilite);
}
//***************************************************************************************
COLORREF CBCGPVisualManager::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* /*pBar*/, 
			BOOL bActive, CRect rectCaption, CRect /*rectButtons*/)
{
	ASSERT_VALID (pDC);

	CBrush br (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
	pDC->FillRect (rectCaption, &br);

    // get the text color
	return globalData.clrCaptionText;
}
//****************************************************************************************
void CBCGPVisualManager::OnDrawCaptionButton (
						CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive,
						BOOL bHorz, BOOL bMaximized, BOOL bDisabled, 
						int nImageID /*= -1*/)
{
	ASSERT_VALID (pDC);
    CRect rc = pButton->GetRect ();

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

	CSize sizeImage = CMenuImages::Size ();
	CPoint ptImage (rc.left + (rc.Width () - sizeImage.cx) / 2,
					rc.top + (rc.Height () - sizeImage.cy) / 2);

	if (pButton->m_bPushed && pButton->m_bFocused)
	{
		ptImage.Offset (1, 1);
	}

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
		if (GetRValue (pButton->m_clrForeground) > 128 &&
			GetGValue (pButton->m_clrForeground) > 128 &&
			GetBValue (pButton->m_clrForeground) > 128)
		{
			imageState = CMenuImages::ImageWhite;
		}
		else
		{
			imageState = CMenuImages::ImageBlack;
		}
	}

	CMenuImages::Draw (pDC, id, ptImage, imageState);

	if (!bDisabled)
	{
		if (pButton->m_bPushed && pButton->m_bFocused)
		{
			pDC->Draw3dRect (rc, globalData.clrBarDkShadow, globalData.clrBarLight);
			rc.DeflateRect (1, 1);
			pDC->Draw3dRect (rc, globalData.clrBarDkShadow, globalData.clrBarHilite);
		}
		else if (!m_bLook2000)
		{
			pDC->Draw3dRect (rc, globalData.clrBarLight, globalData.clrBarDkShadow);
			rc.DeflateRect (1, 1);
			pDC->Draw3dRect (rc, globalData.clrBarHilite, globalData.clrBarShadow);
		}
		else if (pButton->m_bFocused || pButton->m_bPushed)
		{
			pDC->Draw3dRect (rc, globalData.clrBarHilite, globalData.clrBarShadow);
		}
	}
}
//***********************************************************************************
void CBCGPVisualManager::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	pDC->FillRect (rect, &globalData.brBarFace);

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());

	if (pTabWnd->IsOneNoteStyle () && clrActiveTab != (COLORREF)-1)
	{
		rect.top = rect.bottom - 2;
		pDC->FillSolidRect (rect, clrActiveTab);
	}
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	COLORREF clrTab = pTabWnd->GetTabBkColor (iTab);

	if (pTabWnd->IsFlatTab ())
	{
		//----------------
		// Draw tab edges:
		//----------------
		#define FLAT_POINTS_NUM	4
		POINT pts [FLAT_POINTS_NUM];

		const int nHalfHeight = pTabWnd->GetTabsHeight () / 2;

		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			rectTab.bottom --;

			pts [0].x = rectTab.left;
			pts [0].y = rectTab.top;

			pts [1].x = rectTab.left + nHalfHeight;
			pts [1].y = rectTab.bottom;

			pts [2].x = rectTab.right - nHalfHeight;
			pts [2].y = rectTab.bottom;

			pts [3].x = rectTab.right;
			pts [3].y = rectTab.top;
		}
		else
		{
			rectTab.top ++;

			pts [0].x = rectTab.left + nHalfHeight;
			pts [0].y = rectTab.top;

			pts [1].x = rectTab.left;
			pts [1].y = rectTab.bottom;

			pts [2].x = rectTab.right;
			pts [2].y = rectTab.bottom;

			pts [3].x = rectTab.right - nHalfHeight;
			pts [3].y = rectTab.top;

			rectTab.left += 2;
		}

		CBrush* pOldBrush = NULL;
		CBrush br (clrTab);

		if (!bIsActive && clrTab != (COLORREF)-1)
		{
			pOldBrush = pDC->SelectObject (&br);
		}

		pDC->Polygon (pts, FLAT_POINTS_NUM);

		if (pOldBrush != NULL)
		{
			pDC->SelectObject (pOldBrush);
		}
	}
	else	// 3D Tab
	{
		CRect rectClip;
		pTabWnd->GetTabsRect (rectClip);

		BOOL bIsCutted = FALSE;

		const BOOL bIsOneNote = pTabWnd->IsOneNoteStyle ();
		if (bIsOneNote)
		{
			const int nExtra = (iTab == 0 || bIsActive) ? 0 : rectTab.Height ();

			if (rectTab.left + nExtra + 10 > rectClip.right ||
				rectTab.right - 10 <= rectClip.left)
			{
				return;
			}
		}

		const int iVertOffset = 2;
		const int iHorzOffset = 2;

		#define POINTS_NUM	8
		POINT pts [POINTS_NUM];

		if (!bIsActive || bIsOneNote || clrTab != (COLORREF)-1)
		{
			if (!bIsOneNote && !bIsActive)
			{
				if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
				{
					rectTab.bottom -= iVertOffset;
				}
				else
				{
					rectTab.top += iVertOffset;
				}
			}

			if (clrTab != (COLORREF)-1 || bIsOneNote)
			{
				CRgn rgn;
				CBrush br (clrTab == (COLORREF)-1 ? globalData.clrBtnFace : clrTab);

				CRect rectFill = rectTab;

				if (bIsOneNote)
				{
					CRect rectFill = rectTab;

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

					for (int i = 0; i < POINTS_NUM; i++)
					{
						if (pts [i].x > rectClip.right)
						{
							pts [i].x = rectClip.right;
							bIsCutted = TRUE;
						}

						if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
						{
							pts [i].y = rectFill.bottom - pts [i].y + rectFill.top - 1;
						}
					}

					rgn.CreatePolygonRgn (pts, POINTS_NUM, WINDING);
					pDC->SelectClipRgn (&rgn);
				}
				else
				{
					rectFill.DeflateRect (1, 0);

					if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
					{
						rectFill.bottom--;
					}
					else
					{
						rectFill.top++;
					}
				}

				OnFillTab (pDC, rectFill, &br, iTab, bIsActive, pTabWnd);
				pDC->SelectClipRgn (NULL);

				if (bIsOneNote)
				{
					CRect rectLeft;
					pTabWnd->GetClientRect (rectLeft);
					rectLeft.right = rectClip.left - 1;

					pDC->ExcludeClipRect (rectLeft);

					if (iTab > 0 && !bIsActive && iTab != pTabWnd->GetFirstVisibleTabNum ())
					{
						CRect rectLeftTab = rectClip;
						rectLeftTab.right = rectFill.left + rectFill.Height () - 10;

						if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
						{
							rectLeftTab.top--;
						}
						else
						{
							rectLeftTab.bottom++;
						}

						pDC->ExcludeClipRect (rectLeftTab);
					}

					pDC->Polyline (pts, POINTS_NUM);

					if (bIsCutted)
					{
						pDC->MoveTo (rectClip.right, rectTab.top);
						pDC->LineTo (rectClip.right, rectTab.bottom);
					}

					CRect rectRight = rectClip;
					rectRight.left = rectFill.right;

					pDC->ExcludeClipRect (rectRight);
				}
			}
		}

		CPen penLight (PS_SOLID, 1, globalData.clrBarHilite);
		CPen penShadow (PS_SOLID, 1, globalData.clrBarShadow);
		CPen penDark (PS_SOLID, 1, globalData.clrBarDkShadow);

		CPen* pOldPen = NULL;

		if (bIsOneNote)
		{
			pOldPen = (CPen*) pDC->SelectObject (&penLight);
			ASSERT(pOldPen != NULL);

			if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
			{
				if (!bIsCutted)
				{
					int yTop = bIsActive ? pts [7].y - 1 : pts [7].y;

					pDC->MoveTo (pts [6].x - 1, pts [6].y);
					pDC->LineTo (pts [7].x - 1, yTop);
				}
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
		}
		else
		{
			if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
			{
				pOldPen = (CPen*) pDC->SelectObject (&penLight);
				ASSERT(pOldPen != NULL);

				pDC->MoveTo (rectTab.left, rectTab.top);
				pDC->LineTo (rectTab.left, rectTab.bottom - iVertOffset);

				pDC->SelectObject (&penDark);

				pDC->LineTo (rectTab.left + iHorzOffset, rectTab.bottom);
				pDC->LineTo (rectTab.right - iHorzOffset, rectTab.bottom);
				pDC->LineTo (rectTab.right, rectTab.bottom - iVertOffset);
				pDC->LineTo (rectTab.right, rectTab.top - 1);

				pDC->SelectObject(&penShadow);

				pDC->MoveTo (rectTab.left + iHorzOffset + 1, rectTab.bottom - 1);
				pDC->LineTo (rectTab.right - iHorzOffset, rectTab.bottom - 1);
				pDC->LineTo (rectTab.right - 1, rectTab.bottom - iVertOffset);
				pDC->LineTo (rectTab.right - 1, rectTab.top - 1);
			}
			else
			{
				pOldPen = pDC->SelectObject (&penDark);
				ASSERT(pOldPen != NULL);

				pDC->MoveTo (rectTab.right, bIsActive ? rectTab.bottom : rectTab.bottom - 1);
				pDC->LineTo (rectTab.right, rectTab.top + iVertOffset);
				pDC->LineTo (rectTab.right - iHorzOffset, rectTab.top);
				
				pDC->SelectObject (&penLight);
				
				pDC->LineTo (rectTab.left + iHorzOffset, rectTab.top);
				pDC->LineTo (rectTab.left, rectTab.top + iVertOffset);

				pDC->LineTo (rectTab.left, rectTab.bottom);
				
				pDC->SelectObject (&penShadow);
				
				pDC->MoveTo (rectTab.right - 1, bIsActive ? rectTab.bottom : rectTab.bottom - 1);
				pDC->LineTo (rectTab.right - 1, rectTab.top + iVertOffset - 1);
 			}
		}

		if (bIsActive)
		{
			const int iBarHeight = 3;
			const int y = (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM) ? 
				(rectTab.top - iBarHeight) : (rectTab.bottom);

			CRect rectFill (CPoint (rectTab.left, y), 
							CSize (rectTab.Width () - 1, iBarHeight + 1));

			COLORREF clrActiveTab = pTabWnd->GetTabBkColor (iTab);

			if (clrActiveTab != (COLORREF)-1)
			{
				CBrush br (clrActiveTab);
				pDC->FillRect (rectFill, &br);
			}
			else
			{
				pDC->FillRect (rectFill, &globalData.brBarFace);
			}
		}

		pDC->SelectObject (pOldPen);

		if (bIsOneNote)
		{
			rectTab.left += rectTab.Height ();
			rectTab.right -= CBCGPBaseTabWnd::TAB_IMAGE_MARGIN;
		}
	}

	COLORREF clrText = pTabWnd->GetTabTextColor (iTab);
	
	COLORREF cltTextOld = (COLORREF)-1;
	if (!bIsActive && clrText != (COLORREF)-1)
	{
		cltTextOld = pDC->SetTextColor (clrText);
	}

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, (COLORREF)-1);

	if (cltTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor (cltTextOld);
	}
}
//*********************************************************************************
void CBCGPVisualManager::OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill,
									 int /*iTab*/, BOOL /*bIsActive*/, 
									 const CBCGPBaseTabWnd* /*pTabWnd*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pbrFill);

	pDC->FillRect (rectFill, pbrFill);
}
//*********************************************************************************
BOOL CBCGPVisualManager::OnEraseTabsFrame (CDC* pDC, CRect rect, 
										   const CBCGPBaseTabWnd* pTabWnd) 
{	
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());

	if (clrActiveTab == (COLORREF)-1)
	{
		return FALSE;
	}

	pDC->FillSolidRect (rect, clrActiveTab);
	return TRUE;
}
//*********************************************************************************
void CBCGPVisualManager::OnDrawTabContent (CDC* pDC, CRect rectTab,
						int iTab, BOOL /*bIsActive*/, const CBCGPBaseTabWnd* pTabWnd,
						COLORREF clrText)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	CString strText;
	pTabWnd->GetTabLabel (iTab, strText);

	if (pTabWnd->IsFlatTab ())
	{
		//---------------
		// Draw tab text:
		//---------------
		UINT nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
		if (pTabWnd->IsDrawNoPrefix ())
		{
			nFormat |= DT_NOPREFIX;
		}

		pDC->DrawText (strText, rectTab, nFormat);
	}
	else
	{
		CSize sizeImage = pTabWnd->GetImageSize ();
		UINT uiIcon = pTabWnd->GetTabIcon (iTab);
		HICON hIcon = pTabWnd->GetTabHicon (iTab);

		if (uiIcon == (UINT)-1 && hIcon == NULL)
		{
			sizeImage.cx = 0;
		}

		if (sizeImage.cx + CBCGPBaseTabWnd::TAB_IMAGE_MARGIN <= rectTab.Width ())
		{
			if (hIcon != NULL)
			{
				//---------------------
				// Draw the tab's icon:
				//---------------------
				CRect rectImage = rectTab;

				rectImage.top += (rectTab.Height () - sizeImage.cy) / 2;
				rectImage.bottom = rectImage.top + sizeImage.cy;

				rectImage.left += IMAGE_MARGIN;
				rectImage.right = rectImage.left + sizeImage.cx;

				pDC->DrawState (rectImage.TopLeft (), rectImage.Size (), 
					hIcon, DSS_NORMAL, (HBRUSH) NULL);
			}
			else
			{
				const CImageList* pImageList = pTabWnd->GetImageList ();
				if (pImageList != NULL && uiIcon != (UINT)-1)
				{
					//----------------------
					// Draw the tab's image:
					//----------------------
					CRect rectImage = rectTab;

					rectImage.top += (rectTab.Height () - sizeImage.cy) / 2;
					rectImage.bottom = rectImage.top + sizeImage.cy;

					rectImage.left += IMAGE_MARGIN;
					rectImage.right = rectImage.left + sizeImage.cx;

					ASSERT_VALID (pImageList);
					((CImageList*) pImageList)->Draw (pDC, uiIcon, rectImage.TopLeft (), ILD_TRANSPARENT);
				}
			}

			//------------------------------
			// Finally, draw the tab's text:
			//------------------------------
			CRect rcText = rectTab;
			rcText.left += sizeImage.cx + 2 * TEXT_MARGIN;
			
			if (clrText != (COLORREF)-1)
			{
				pDC->SetTextColor (clrText);
			}

			CString strText;
			pTabWnd->GetTabLabel (iTab, strText);

			UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
			if (pTabWnd->IsDrawNoPrefix ())
			{
				nFormat |= DT_NOPREFIX;
			}

			if (pTabWnd->IsOneNoteStyle ())
			{
				nFormat |= DT_CENTER;
			}
			else
			{
				nFormat |= DT_LEFT;
			}

			pDC->DrawText (strText, rcText, nFormat);
		}
	}
}
//**********************************************************************************
void CBCGPVisualManager::OnEraseTabsButton (CDC* pDC, CRect rect, 
											CBCGPButton* /*pButton*/,
											CBCGPBaseTabWnd* /*pWndTab*/)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBarFace);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT uiState,
												 CBCGPBaseTabWnd* /*pWndTab*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsPressed () || (uiState & ODS_SELECTED))
	{
		pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarHilite);

		rect.left += 2;
		rect.top += 2;
	}
	else
	{
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarDkShadow);
	}

	rect.DeflateRect (2, 2);
}
//**********************************************************************************
COLORREF CBCGPVisualManager::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (bIsSelected)
	{
		pDC->FillRect (rect, &globalData.brHilite);

		const int nFrameSize = 1;

		rect.DeflateRect (1, 1);
		rect.right--;
		rect.bottom--;

		pDC->PatBlt (rect.left, rect.top + nFrameSize, nFrameSize, rect.Height (), PATINVERT);
		pDC->PatBlt (rect.left, rect.top, rect.Width (), nFrameSize, PATINVERT);
		pDC->PatBlt (rect.right, rect.top, nFrameSize, rect.Height (), PATINVERT);
		pDC->PatBlt (rect.left + nFrameSize, rect.bottom, rect.Width (), nFrameSize, PATINVERT);

		return globalData.clrTextHilite;
	}

	pDC->FillRect (rect, &globalData.brBarFace);

	return globalData.clrBarText;
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawMenuArrowOnCustomizeList (CDC* pDC, 
	CRect rectCommand, BOOL bSelected)
{
	CRect rectTriangle = rectCommand;
	rectTriangle.left = rectTriangle.right - CMenuImages::Size ().cx;

	CPoint pointTriangle (
		rectTriangle.left + (rectTriangle.Width () - CMenuImages::Size ().cx) / 2,
		rectTriangle.top + (rectTriangle.Height () - CMenuImages::Size ().cy) / 2);

	CMenuImages::Draw (pDC, CMenuImages::IdArowRightLarge, pointTriangle,
		bSelected ? CMenuImages::ImageWhite : CMenuImages::ImageBlack);

	CRect rectLine = rectCommand;
	rectLine.right = rectTriangle.left - 1;
	rectLine.left = rectLine.right - 2;
	rectLine.DeflateRect (0, 2);

	pDC->Draw3dRect (&rectLine, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
//*********************************************************************************
CBCGPVisualManager* CBCGPVisualManager::CreateVisualManager (
					CRuntimeClass* pVisualManager)
{
	if (pVisualManager == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPVisualManager* pVisManagerOld = m_pVisManager;
	
	CObject* pObj = pVisualManager->CreateObject ();
	if (pObj == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	ASSERT_VALID (pObj);
	
	if (pVisManagerOld != NULL)
	{
		ASSERT_VALID (pVisManagerOld);
		delete pVisManagerOld;
	}
	
	m_pVisManager = (CBCGPVisualManager*) pObj;
	m_pVisManager->m_bAutoDestroy = TRUE;

	return m_pVisManager;
}
//*************************************************************************************
void CBCGPVisualManager::DestroyInstance ()
{
	if (m_pVisManager != NULL)
	{
		delete m_pVisManager;
	}
	
	m_pVisManager = NULL;
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	const int iBorderSize = 2;

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBarFace);
	
	rect.DeflateRect (iBorderSize, 1);

	pDC->FillSolidRect (rect, 
		bIsActive ? 
		globalData.clrActiveCaption :
		globalData.clrInactiveCaption);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawMenuSystemButton (CDC* pDC, CRect rect, 
												UINT uiSystemCommand, 
												UINT nStyle, BOOL /*bHighlight*/)
{
	ASSERT_VALID (pDC);

	UINT uiState = 0;

	switch (uiSystemCommand)
	{
	case SC_CLOSE:
		uiState |= DFCS_CAPTIONCLOSE;
		break;

	case SC_MINIMIZE:
		uiState |= DFCS_CAPTIONMIN;
		break;

	case SC_RESTORE:
		uiState |= DFCS_CAPTIONRESTORE;
		break;

	default:
		return;
	}

	if (nStyle & TBBS_PRESSED)
	{
		uiState |= DFCS_PUSHED;
	}
	
	if (nStyle & TBBS_DISABLED) // Jan Vasina: Add support for disabled buttons
	{
		uiState |= DFCS_INACTIVE;
	}

	pDC->DrawFrameControl (rect, DFC_CAPTION, uiState);
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawComboDropButton (CDC* pDC, CRect rect,
											    BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	pDC->FillSolidRect (rect, globalData.clrBarFace);
	pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);

	CPoint pointTriangle (
		rect.left + (rect.Width () - CMenuImages::Size ().cx) / 2 + 1,
		rect.top + (rect.Height () - CMenuImages::Size ().cy) / 2 + 1);

	if (bIsDropped)
	{
		rect.OffsetRect (1, 1);
		pointTriangle.Offset (1, 1);

		pDC->Draw3dRect (&rect, globalData.clrBarShadow, globalData.clrBarHilite);
	}
	else if (bIsHighlighted)
	{
		pDC->Draw3dRect (&rect, globalData.clrBarHilite, globalData.clrBarShadow);
	}


	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, pointTriangle,
		bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	ASSERT_VALID (pDC);

	if (bIsHighlighted || bIsDropped)
	{
		if (m_bMenuFlatLook)
		{
			CRect rectBorder = rect;
			rectBorder.DeflateRect (1, 1);

			pDC->Draw3dRect (&rectBorder, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
		}
		else
		{
			pDC->Draw3dRect (&rect, globalData.clrBarShadow, globalData.clrBarHilite);
		}
	}
}
//********************************************************************************
void CBCGPVisualManager::OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* /*pBar*/,
					CRect rectPane, UINT /*uiID*/, UINT nStyle)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	if (!(nStyle & SBPS_NOBORDERS))
	{
		// draw the borders
		COLORREF clrHilite;
		COLORREF clrShadow;

		if (nStyle & SBPS_POPOUT)
		{
			// reverse colors
			clrHilite = globalData.clrBarShadow;
			clrShadow = globalData.clrBarHilite;
		}
		else
		{
			// normal colors
			clrHilite = globalData.clrBarHilite;
			clrShadow = globalData.clrBarShadow;
		}

		pDC->Draw3dRect (rectPane, clrShadow, clrHilite);
	}
}
//*********************************************************************************
COLORREF CBCGPVisualManager::OnFillMiniFrameCaption (CDC* pDC, 
								CRect rectCaption, 
								CBCGPMiniFrameWnd* pFrameWnd, BOOL bActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	if (DYNAMIC_DOWNCAST (CBCGPBaseToolBar, pFrameWnd->GetControlBar ()) != NULL)
	{
		bActive = TRUE;
	}

	CBrush br (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
	pDC->FillRect (rectCaption, &br);

    // get the text color
	return globalData.clrCaptionText;
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);
	
	BCGP_PREDOCK_STATE preDockState = pFrameWnd->GetPreDockState ();

	if (preDockState == PDS_DOCK_REGULAR)
	{
		// draw outer edge;
		pDC->Draw3dRect (rectBorder, RGB (127, 0, 0), globalData.clrBarDkShadow);
		rectBorder.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, RGB (127, 0, 0));
	}
	else if (preDockState == PDS_DOCK_TO_TAB)
	{
		// draw outer edge;
		pDC->Draw3dRect (rectBorder, RGB (0, 0, 127), globalData.clrBarDkShadow);
		rectBorder.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, RGB (0, 0, 127));
	}
	else
	{
		// draw outer edge;
		pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarDkShadow);
		rectBorder.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, globalData.clrBarShadow);
	}	

	// draw the inner egde
	rectBorder.DeflateRect (rectBorderSize.right - 2, rectBorderSize.top - 2);
	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarFace);
	rectBorder.InflateRect (1, 1);
	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarFace);
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawFloatingToolbarBorder (
												CDC* pDC, CBCGPBaseToolBar* /*pToolBar*/, 
												CRect rectBorder, CRect /*rectBorderSize*/)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarDkShadow);
	rectBorder.DeflateRect (1, 1);
	pDC->Draw3dRect (rectBorder, globalData.clrBarHilite, globalData.clrBarShadow);
	rectBorder.DeflateRect (1, 1);
	pDC->Draw3dRect (rectBorder, globalData.clrBarFace, globalData.clrBarFace);
}
//**************************************************************************************
COLORREF CBCGPVisualManager::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												  CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode () && !pButton->IsEditable ()) ||
		(!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED));

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)))
	{
		return bDisabled ? globalData.clrBarFace : globalData.clrWindow;
	}

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPCalendarButton)))
	{
		CBCGPCalendarButton* pCalendarButton = (CBCGPCalendarButton*) pButton;
		COLORREF clrText = pCalendarButton->GetTextColor ();
		if (clrText != (COLORREF) -1)
		{
			return clrText;
		}

		if (!pCalendarButton->IsCurrMonth ())
		{
			return globalData.clrGrayedText;
		}
	}

	return	(bDisabled ? globalData.clrGrayedText : 
			(state == ButtonsIsHighlighted) ? 
				CBCGPToolBar::GetHotTextColor () : globalData.clrBarText);
}
//***************************************************************************************
void CBCGPVisualManager::OnFillOutlookPageButton (CDC* pDC, 
												  const CRect& rect,
												  BOOL /*bIsHighlighted*/, BOOL /*bIsPressed*/,
												  COLORREF& clrText)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBarFace);
	clrText = globalData.clrBarText;
}
//****************************************************************************************
void CBCGPVisualManager::OnDrawOutlookPageButtonBorder (
	CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
	ASSERT_VALID (pDC);

	if (bIsHighlighted && bIsPressed)
	{
		pDC->Draw3dRect (rectBtn, globalData.clrBarDkShadow, globalData.clrBarFace);
		rectBtn.DeflateRect (1, 1);
		pDC->Draw3dRect (rectBtn, globalData.clrBarShadow, globalData.clrBarHilite);
	}
	else
	{
		if (bIsHighlighted || bIsPressed)
		{
			pDC->Draw3dRect (rectBtn, globalData.clrBarFace, globalData.clrBarDkShadow);
			rectBtn.DeflateRect (1, 1);
		}

		pDC->Draw3dRect (rectBtn, globalData.clrBarHilite, globalData.clrBarShadow);
	}

	rectBtn.DeflateRect (1, 1);
}
//**********************************************************************************
COLORREF CBCGPVisualManager::GetCaptionBarTextColor (CBCGPCaptionBar* /*pBar*/)
{
	return globalData.clrWindow;
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawCaptionBarBorder (CDC* pDC, 
	CBCGPCaptionBar* /*pBar*/, CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder)
{
	ASSERT_VALID (pDC);

	if (clrBarBorder == (COLORREF) -1)
	{
		pDC->FillRect (rect, &globalData.brBarFace);
	}
	else
	{
		CBrush brBorder;
		brBorder.CreateSolidBrush (clrBarBorder);
		pDC->FillRect (rect, &brBorder);
	}

	if (!bFlatBorder)
	{
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
	}
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* /*pStatusBar*/,
			CRect rectProgress, int nProgressTotal, int nProgressCurr,
			COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
			BOOL bProgressText)
{
	ASSERT_VALID (pDC);

	if (nProgressTotal == 0)
	{
		return;
	}

	CRect rectComplete = rectProgress;
	rectComplete.right = rectComplete.left + 
		nProgressCurr * rectComplete.Width () / nProgressTotal;

	if (clrProgressBarDest == (COLORREF)-1)
	{
		// one-color bar
		CBrush br (clrBar);
		pDC->FillRect (rectComplete, &br);
	}
	else
	{
		// gradient bar:
		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rectComplete, clrBar, clrProgressBarDest, FALSE);
	}

	if (bProgressText)
	{
		CString strText;
		strText.Format (_T("%d%%"), nProgressCurr * 100 / nProgressTotal);

		COLORREF clrText = pDC->SetTextColor (globalData.clrBarText);

		pDC->DrawText (strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

		CRgn rgn;
		rgn.CreateRectRgnIndirect (rectComplete);
		pDC->SelectClipRgn (&rgn);

		pDC->SetTextColor (clrProgressText == (COLORREF)-1 ?
			globalData.clrTextHilite : clrProgressText);
		
		pDC->DrawText (strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
		pDC->SelectClipRgn (NULL);
		pDC->SetTextColor (clrText);
	}
}
//****************************************************************************************
void CBCGPVisualManager::OnFillHeaderCtrlBackground (CBCGPHeaderCtrl* pCtrl,
													 CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, 
		pCtrl->IsDialogControl () ? &globalData.brBtnFace : &globalData.brBarFace);
}
//****************************************************************************************
void CBCGPVisualManager::OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC,
		CRect& rect, BOOL bIsPressed, BOOL /*bIsHighlighted*/)
{
	ASSERT_VALID (pDC);

	if (bIsPressed)
	{
		if (pCtrl->IsDialogControl ())
		{
			pDC->Draw3dRect (rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
		}
		else
		{
			pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
		}

		rect.left++;
		rect.top++;
	}
	else
	{
		if (pCtrl->IsDialogControl ())
		{
			pDC->Draw3dRect (rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
		}
		else
		{
			pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);
		}
	}
}
//*****************************************************************************************
void CBCGPVisualManager::OnDrawHeaderCtrlSortArrow (CBCGPHeaderCtrl* pCtrl, 
												   CDC* pDC,
												   CRect& rectArrow, BOOL bIsUp)
{
	CPen penLight (1, PS_SOLID, 
		pCtrl->IsDialogControl () ? globalData.clrBtnHilite : globalData.clrBarHilite);
	CPen penDark (1, PS_SOLID, 
		pCtrl->IsDialogControl () ? globalData.clrBtnDkShadow : globalData.clrBarDkShadow);

	CPen* pPenOld = pDC->SelectObject (&penLight);
	ASSERT_VALID (pPenOld);

	if (!bIsUp)
	{
		pDC->MoveTo (rectArrow.right, rectArrow.top);
		pDC->LineTo (rectArrow.CenterPoint ().x, rectArrow.bottom);

		pDC->SelectObject (&penDark);
		pDC->LineTo (rectArrow.left, rectArrow.top);
		pDC->LineTo (rectArrow.right, rectArrow.top);
	}
	else
	{
		pDC->MoveTo (rectArrow.left, rectArrow.bottom);
		pDC->LineTo (rectArrow.right, rectArrow.bottom);
		pDC->LineTo (rectArrow.CenterPoint ().x, rectArrow.top);

		pDC->SelectObject (&penDark);
		pDC->LineTo (rectArrow.left, rectArrow.bottom);
	}

	pDC->SelectObject (pPenOld);
}
//*****************************************************************************************
void CBCGPVisualManager::OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* /*pStatBar*/,
			CRect rectSizeBox)
{
	ASSERT_VALID (pDC);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontMarlett);
	ASSERT (pOldFont != NULL);

	const CString strSizeBox (_T("o"));	// Char of the sizing box in "Marlett" font

	UINT nTextAlign = pDC->SetTextAlign (TA_RIGHT | TA_BOTTOM);
	COLORREF clrText = pDC->SetTextColor (globalData.clrBarShadow);

	pDC->ExtTextOut (rectSizeBox.right, rectSizeBox.bottom,
		ETO_CLIPPED, &rectSizeBox, strSizeBox, NULL);

	pDC->SelectObject (pOldFont);
	pDC->SetTextColor (clrText);
	pDC->SetTextAlign (nTextAlign);
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawEditBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsHighlighted,
												CBCGPToolbarEditBoxButton* /*pButton*/)
{
	ASSERT_VALID (pDC);

	if (bIsHighlighted)
	{
		pDC->DrawEdge (rect, EDGE_SUNKEN, BF_RECT);
	}
}
//**********************************************************************************
void CBCGPVisualManager::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rectWorkArea, &globalData.brWindow);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
						BOOL bIsHighlighted, BOOL /*bIsSelected*/, BOOL bCanCollapse)
{
	ASSERT_VALID(pDC);
	ASSERT(pGroup != NULL);

	// ---------------------------------
	// Draw caption background (Windows)
	// ---------------------------------
	COLORREF clrBckOld = pDC->GetBkColor ();
	pDC->FillSolidRect(pGroup->m_rect, 
		(pGroup->m_bIsSpecial ? globalData.clrHilite : globalData.clrBarFace)); 
	pDC->SetBkColor (clrBckOld);

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
			globalData.clrWindow : globalData.clrWindowText);
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

			if (pGroup->m_bIsSpecial)
			{
				if (!pGroup->m_bIsCollapsed)
				{
					CMenuImages::Draw(pDC, CMenuImages::IdArowUp, rectButton.TopLeft());
				}
				else
				{
					CMenuImages::Draw(pDC, CMenuImages::IdArowDown, rectButton.TopLeft());
				}
			}
			else
			{
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
}
//**********************************************************************************
void CBCGPVisualManager::OnFillTasksGroupInterior(CDC* /*pDC*/, CRect /*rect*/, BOOL /*bSpecial*/)
{
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial, 
												   BOOL bNoTitle)
{
	ASSERT_VALID(pDC);

	// Draw caption background:
	CPen* pPenOld = (CPen*) pDC->SelectObject (bSpecial ? &globalData.penHilite : &globalData.penBarFace);

	pDC->MoveTo (rect.left, rect.top);
	pDC->LineTo (rect.left, rect.bottom-1);
	pDC->LineTo (rect.right-1, rect.bottom-1);
	pDC->LineTo (rect.right-1, rect.top);
	if (bNoTitle)
	{
		pDC->LineTo (rect.left, rect.top);
	}
	else
	{
		pDC->LineTo (rect.right-1, rect.top-1);
	}
	pDC->SelectObject (&pPenOld);
	
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, 
								   BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pIcons);
	ASSERT(pTask != NULL);

	CRect rectText = pTask->m_rect;

	if (pTask->m_bIsSeparator)
	{
		CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarFace);

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
	COLORREF clrTextOld = pDC->GetTextColor ();
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
		pFontOld = pDC->SelectObject (&globalData.fontUnderline);
		pDC->SetTextColor (globalData.clrWindowText);
	}
	else
	{
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
		pDC->SetTextColor (globalData.clrWindowText);
	}
	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText (pTask->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);
	
	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	ASSERT_VALID (pDC);

	CRect rectImage (CPoint (0, 0), CMenuImages::Size ());

	CRect rectFill = rect;
	rectFill.top -= nBorderSize;

	pDC->FillRect (rectFill, &globalData.brBarFace);

	if (bHilited)
	{
		CBCGPDrawManager dm (*pDC);
		dm.HighlightRect (rect);

		pDC->Draw3dRect (rect,
			globalData.clrBarHilite,
			globalData.clrBarDkShadow);
	}

	CPoint point (
		rect.left + (rect.Width () - rectImage.Width ()) / 2,
		rect.top + (rect.Height () - rectImage.Height ()) / 2);

	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, point);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawSlider (CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pSlider);

	pDC->FillRect (rect, &globalData.brBarFace);

	if (bAutoHideMode)
	{
		// draw outer edge;

		DWORD dwAlgn = pSlider->GetCurrentAlignment ();
		CRect rectBorder = rect;

		COLORREF clrBorder = globalData.clrBarDkShadow;
		
		if (dwAlgn & CBRS_ALIGN_LEFT)
		{
			rectBorder.left = rectBorder.right;
		}
		else if (dwAlgn & CBRS_ALIGN_RIGHT)
		{
			rectBorder.right = rectBorder.left;
			clrBorder = globalData.clrBarHilite;
		}
		else if (dwAlgn & CBRS_ALIGN_TOP)
		{
			rectBorder.top = rectBorder.bottom;
		}
		else if (dwAlgn & CBRS_ALIGN_BOTTOM)
		{
			rectBorder.bottom = rectBorder.top;
			clrBorder = globalData.clrBarHilite;
		}
		else
		{
			ASSERT(FALSE);
			return;
		}

		pDC->Draw3dRect (rectBorder, clrBorder, clrBorder);
	}
}
//********************************************************************************
void CBCGPVisualManager::OnDrawSplitterBorder (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
	ASSERT_VALID(pDC);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);
	rect.InflateRect(-CX_BORDER, -CY_BORDER);
	pDC->Draw3dRect (rect, globalData.clrBarFace, globalData.clrBarFace);
}
//********************************************************************************
void CBCGPVisualManager::OnDrawSplitterBox (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect& rect)
{
	ASSERT_VALID(pDC);
	pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarShadow);
}
//********************************************************************************
void CBCGPVisualManager::OnFillSplitterBackground (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
	ASSERT_VALID(pDC);
	pDC->FillSolidRect (rect, globalData.clrBarFace);
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawCheckBox (CDC *pDC, CRect rect, 
										 BOOL bHighlighted, 
										 BOOL bChecked,
										 BOOL bEnabled)
{
	if (bHighlighted)
	{
		pDC->DrawFocusRect (rect);
	}

	rect.DeflateRect (1, 1);
	pDC->FillSolidRect (&rect, bEnabled ? globalData.clrWindow :
		globalData.clrBarFace);

	pDC->Draw3dRect (&rect, 
					globalData.clrBarDkShadow,
					globalData.clrBarHilite);
	
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (&rect,
					globalData.clrBarShadow,
					globalData.clrBarLight);

	if (bChecked)
	{
		CSize sizeImage = CMenuImages::Size ();
		CPoint ptImage (rect.left + (rect.Width () - sizeImage.cx) / 2,
						rect.top + (rect.Height () - sizeImage.cy) / 2);

		CMenuImages::Draw (pDC, CMenuImages::IdCheck, ptImage,
							CMenuImages::ImageBlack);
	}
}
//**********************************************************************************
void CBCGPVisualManager::OnDrawSpinButtons (CDC* pDC, CRect rectSpin, 
	int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);
	ASSERT_VALID (pSpinCtrl);

	rectSpin.DeflateRect (1, 1);

	CRect rect[2];

	rect[0] = rect[1] = rectSpin;

	if (!bOrientation)
	{
		rect[0].DeflateRect(0, 0, 0, rect[0].Height() / 2);
		rect[1].top = rect[0].bottom + 1;
	}
	else
	{
		rect[0].DeflateRect(0, 0, rect[0].Width() / 2, 0);
		rect[1].left = rect[0].right + 1;
	}

	pDC->FillRect (rectSpin, &globalData.brBarFace);
	pDC->Draw3dRect (rectSpin, globalData.clrBarHilite, globalData.clrBarHilite);

	CMenuImages::IMAGES_IDS id[2][2] = {{CMenuImages::IdArowUp, CMenuImages::IdArowDown}, {CMenuImages::IdArowLeft, CMenuImages::IdArowRight}};

	int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
	BOOL bDisabled = nState & SPIN_DISABLED;

	for (int i = 0; i < 2; i ++)
	{
		CPoint pointTriangle (
			rect[i].left + (rect[i].Width () - CMenuImages::Size ().cx) / 2 ,
			rect[i].top + (rect[i].Height () - CMenuImages::Size ().cy) / 2 );

		if (idxPressed == i)
		{
			pDC->Draw3dRect (&rect[i], globalData.clrBarShadow, globalData.clrBarHilite);
		}
		else
		{
			pDC->Draw3dRect (&rect[i], globalData.clrBarHilite, globalData.clrBarShadow);
		}

		CMenuImages::Draw (pDC, id[bOrientation][i], pointTriangle,
			bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
	}
}
//**************************************************************************************
void CBCGPVisualManager::OnDrawExpandingBox (CDC* pDC, CRect rect, BOOL bIsOpened,
											COLORREF colorBox)
{
	ASSERT_VALID(pDC);

	pDC->Draw3dRect (rect, colorBox, colorBox);

	rect.DeflateRect (2, 2);

	CPen penLine (PS_SOLID, 1, globalData.clrBarText);
	CPen* pOldPen = pDC->SelectObject (&penLine);

	CPoint ptCenter = rect.CenterPoint ();

	pDC->MoveTo (rect.left, ptCenter.y);
	pDC->LineTo (rect.right, ptCenter.y);

	if (!bIsOpened)
	{
		pDC->MoveTo (ptCenter.x, rect.top);
		pDC->LineTo (ptCenter.x, rect.bottom);
	}

	pDC->SelectObject (pOldPen);
}
//***********************************************************************************
COLORREF CBCGPVisualManager::OnFillCalendarBarNavigationRow (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	CBrush br (globalData.clrInactiveCaption);
	pDC->FillRect (rect, &br);

	pDC->Draw3dRect (rect, globalData.clrInactiveBorder, globalData.clrInactiveBorder);
	return globalData.clrInactiveCaptionText;
}
//*************************************************************************************
void CBCGPVisualManager::OnFillCalendarBarWeekColumn (CDC* pDC, CRect rect,
								  COLORREF& clrText, COLORREF& clrTextDisabled)
{
	ASSERT_VALID (pDC);

	pDC->FillSolidRect (rect, globalData.clrBarShadow);

	clrText = globalData.clrBarHilite;
	clrTextDisabled = globalData.clrBarLight;
}
//*************************************************************************************
void CBCGPVisualManager::OnDrawControlBorder (CWnd* pWndCtrl)
{
	ASSERT_VALID (pWndCtrl);

	CWindowDC dc (pWndCtrl);

	CRect rect;
	pWndCtrl->GetWindowRect (rect);

	rect.bottom -= rect.top;
	rect.right -= rect.left;
	rect.left = rect.top = 0;
	
	dc.Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarHilite);
}
//***********************************************************************************
void CBCGPVisualManager::OnDrawShowAllMenuItems (CDC* pDC, CRect rect, 
												 CBCGPVisualManager::BCGBUTTON_STATE /*state*/)
{
	ASSERT_VALID (pDC);

	CPoint pointImage (
		rect.left + (rect.Width () - CMenuImages::Size ().cx) / 2,
		rect.top + (rect.Height () - CMenuImages::Size ().cy) / 2);

	CMenuImages::Draw (pDC, CMenuImages::IdArowShowAll, pointImage);
}
//************************************************************************************
int CBCGPVisualManager::GetShowAllMenuItemsHeight (CDC* /*pDC*/, const CSize& /*sizeDefault*/)
{
	return CMenuImages::Size ().cy + 2 * TEXT_MARGIN;
}
//**********************************************************************************
void CBCGPVisualManager::GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
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

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());

	if (pTabWnd->IsOneNoteStyle () && clrActiveTab != (COLORREF)-1)
	{
		clrFace = clrActiveTab;
	}
	else
	{
		clrFace = globalData.clrBarFace;
	}

	clrDark = globalData.clrBarShadow;
	clrBlack = globalData.clrBarText;
	clrHighlight = globalData.clrBarHilite;
	clrDarkShadow = globalData.clrBarDkShadow;
	clrLight = globalData.clrBarLight;

	pbrFace = &globalData.brBarFace;
	pbrBlack = &globalData.brBlack;
}
//*********************************************************************************
void CBCGPVisualManager::OnFillAutoHideButtonBackground (CDC* pDC, CRect rect, CBCGPAutoHideButton* /*pButton*/)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &globalData.brBarFace);
}
//*********************************************************************************
void CBCGPVisualManager::OnDrawAutoHideButtonBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* /*pButton*/)
{
	ASSERT_VALID (pDC);

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
void CBCGPVisualManager::OnDrawOutlookBarSplitter (CDC* pDC, CRect rectSplitter)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rectSplitter, &globalData.brBarFace);
	pDC->Draw3dRect (rectSplitter, globalData.clrBarHilite, globalData.clrBarShadow);
}
//********************************************************************************
void CBCGPVisualManager::OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, 
												  COLORREF& clrText)
{

	pDC->FillSolidRect	(rectCaption, globalData.clrBarShadow);
	clrText = globalData.clrBarHilite;
}
//********************************************************************************
BOOL CBCGPVisualManager::OnDrawCalculatorButton (CDC* pDC, 
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
		pDC->FillRect (rect, &globalData.brLight);
		pDC->Draw3dRect (&rect, globalData.clrBarShadow, globalData.clrBarHilite);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &globalData.brLight);

	default:
		pDC->Draw3dRect (&rect, globalData.clrBarHilite, globalData.clrBarShadow);
	}

	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManager::OnDrawCalculatorDisplay (CDC* pDC, CRect rect, 
												  const CString& /*strText*/, BOOL /*bMem*/,
												  CBCGPCalculator* /*pCalculator*/)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brWindow);
	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarHilite);

	return TRUE;
}
//*********************************************************************************
BOOL CBCGPVisualManager::OnDrawBrowseButton (CDC* pDC, CRect rect, 
											 CBCGPEdit* /*pEdit*/, 
											 CBCGPVisualManager::BCGBUTTON_STATE state,
											 COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (&rect, &globalData.brBtnFace);

	CRect rectFrame = rect;
	rectFrame.InflateRect (0, 1, 1, 1);

	pDC->Draw3dRect (rectFrame, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);

	rectFrame.DeflateRect (1, 1);
	pDC->DrawEdge (rectFrame, state == ButtonsIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
// CBCGPWinXPThemeManager

CBCGPWinXPThemeManager::CBCGPWinXPThemeManager ()
{
	m_hThemeToolBar = NULL;
	m_hThemeButton = NULL;
	m_hThemeStatusBar = NULL;
	m_hThemeWindow = NULL;
	m_hThemeRebar = NULL;
	m_hThemeComboBox = NULL;
	m_hThemeProgress = NULL;
	m_hThemeHeader = NULL;
	m_hThemeScrollBar = NULL;
	m_hThemeExplorerBar = NULL;
	m_hThemeTree = NULL;
	m_hThemeStartPanel = NULL;
	m_hThemeTaskBand = NULL;
	m_hThemeTaskBar = NULL;
	m_hThemeSpin = NULL;

	m_hinstUXDLL = LoadLibrary (_T("UxTheme.dll"));

	if (m_hinstUXDLL != NULL)
	{
		m_pfOpenThemeData = (OPENTHEMEDATA)::GetProcAddress (m_hinstUXDLL, "OpenThemeData");
		m_pfCloseThemeData = (CLOSETHEMEDATA)::GetProcAddress (m_hinstUXDLL, "CloseThemeData");
		m_pfDrawThemeBackground = (DRAWTHEMEBACKGROUND)::GetProcAddress (m_hinstUXDLL, "DrawThemeBackground");
		m_pfGetThemeColor = (GETTHEMECOLOR)::GetProcAddress (m_hinstUXDLL, "GetThemeColor");
		m_pfGetThemeSysColor = (GETTHEMESYSCOLOR)::GetProcAddress (m_hinstUXDLL, "GetThemeSysColor");
		m_pfGetCurrentThemeName = (GETCURRENTTHEMENAME)::GetProcAddress (m_hinstUXDLL, "GetCurrentThemeName");
		m_pfGetWindowTheme = (GETWINDOWTHEME)::GetProcAddress (m_hinstUXDLL, "GetWindowTheme");

		UpdateSystemColors ();
	}
	else
	{
		m_pfOpenThemeData = NULL;
		m_pfCloseThemeData = NULL;
		m_pfDrawThemeBackground = NULL;
		m_pfGetThemeColor = NULL;
		m_pfGetThemeSysColor = NULL;
		m_pfGetCurrentThemeName = NULL;
		m_pfGetWindowTheme = NULL;
	}
}
//*************************************************************************************
CBCGPWinXPThemeManager::~CBCGPWinXPThemeManager ()
{
	if (m_hinstUXDLL != NULL)
	{
		CleanUpThemes ();
		::FreeLibrary (m_hinstUXDLL);
	}
}
//*************************************************************************************
void CBCGPWinXPThemeManager::UpdateSystemColors ()
{
	if (m_hinstUXDLL != NULL)
	{
		CleanUpThemes ();

		if (m_pfOpenThemeData == NULL ||
			m_pfCloseThemeData == NULL ||
			m_pfDrawThemeBackground == NULL)
		{
			ASSERT (FALSE);
		}
		else
		{
			m_hThemeToolBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TOOLBAR");
			m_hThemeButton = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"BUTTON");
			m_hThemeStatusBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"STATUS");
			m_hThemeWindow = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"WINDOW");
			m_hThemeRebar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"REBAR");
			m_hThemeComboBox = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"COMBOBOX");
			m_hThemeProgress = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"PROGRESS");
			m_hThemeHeader = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"HEADER");
			m_hThemeScrollBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"SCROLLBAR");
			m_hThemeExplorerBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"EXPLORERBAR");
			m_hThemeTree = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TREEVIEW");
			m_hThemeStartPanel = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"STARTPANEL");
			m_hThemeTaskBand = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TASKBAND");
			m_hThemeTaskBar = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"TASKBAR");
			m_hThemeSpin = (*m_pfOpenThemeData)(AfxGetMainWnd ()->GetSafeHwnd (), L"SPIN");
		}
	}
}
//************************************************************************************
void CBCGPWinXPThemeManager::CleanUpThemes ()
{
	if (m_pfCloseThemeData == NULL)
	{
		return;
	}

	if (m_hThemeToolBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeToolBar);
	}

	if (m_hThemeRebar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeRebar);
	}

	if (m_hThemeStatusBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeStatusBar);
	}

	if (m_hThemeButton != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeButton);
	}

	if (m_hThemeWindow != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeWindow);
	}

	if (m_hThemeComboBox != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeComboBox);
	}

	if (m_hThemeProgress != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeProgress);
	}

	if (m_hThemeHeader != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeHeader);
	}

	if (m_hThemeScrollBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeScrollBar);
	}

	if (m_hThemeExplorerBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeExplorerBar);
	}

	if (m_hThemeTree != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTree);
	}

	if (m_hThemeStartPanel != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeStartPanel);
	}

	if (m_hThemeTaskBand != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTaskBand);
	}

	if (m_hThemeTaskBar != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeTaskBar);
	}

	if (m_hThemeSpin != NULL)
	{
		(*m_pfCloseThemeData) (m_hThemeSpin);
	}
}
//**********************************************************************************
BOOL CBCGPWinXPThemeManager::DrawPushButton (CDC* pDC, CRect rect, 
											 CBCGPButton* pButton, UINT /*uiState*/)
{
	#define BP_PUSHBUTTON		1

	#define PBS_NORMAL			1
	#define PBS_HOT				2
	#define PBS_PRESSED			3
	#define PBS_DISABLED		4
	#define PBS_DEFAULTED		5

	if (m_hThemeButton == NULL)
	{
		return FALSE;
	}

	int nState = PBS_NORMAL;

	if (!pButton->IsWindowEnabled ())
	{
		nState = PBS_DISABLED;
	}
	else if (pButton->IsPressed () || pButton->GetCheck ())
	{
		nState = PBS_PRESSED;
	}
	else if (pButton->IsHighlighted ())
	{
		nState = PBS_HOT;
	}

	globalData.DrawParentBackground (pButton, pDC, rect);

	(*m_pfDrawThemeBackground) (m_hThemeButton, pDC->GetSafeHdc(), BP_PUSHBUTTON, 
		nState, &rect, 0);

	return TRUE;
}
//**************************************************************************************
BOOL CBCGPWinXPThemeManager::DrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* /*pStatusBar*/,
			CRect rectProgress, int nProgressTotal, int nProgressCurr,
			COLORREF /*clrBar*/, COLORREF /*clrProgressBarDest*/, COLORREF /*clrProgressText*/,
			BOOL bProgressText)
{
	#define	PP_BAR				1
	#define	PP_CHUNK			3

	if (m_hThemeProgress == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pDC);

	(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(), 
								PP_BAR, 0, &rectProgress, 0);

	if (nProgressTotal == 0)
	{
		return TRUE;
	}

	CRect rectComplete = rectProgress;
	rectComplete.DeflateRect (3, 3);

	rectComplete.right = rectComplete.left + 
		nProgressCurr * rectComplete.Width () / nProgressTotal;

	(*m_pfDrawThemeBackground) (m_hThemeProgress, pDC->GetSafeHdc(), 
								PP_CHUNK, 0, &rectComplete, 0);

	if (bProgressText)
	{
		CString strText;
		strText.Format (_T("%d%%"), nProgressCurr * 100 / nProgressTotal);

		COLORREF clrText = pDC->SetTextColor (globalData.clrBtnText);
		pDC->DrawText (strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor (clrText);
	}

	return TRUE;
}
//********************************************************************************
BOOL CBCGPWinXPThemeManager::DrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted)
{
	#define CP_DROPDOWNBUTTON	1

	#define CBXS_NORMAL			1
	#define CBXS_HOT			2
	#define CBXS_PRESSED		3
	#define CBXS_DISABLED		4

	if (m_hThemeComboBox == NULL)
	{
		return FALSE;
	}

	int nState = bDisabled ? CBXS_DISABLED : bIsDropped ? CBXS_PRESSED : bIsHighlighted ? CBXS_HOT : CBXS_NORMAL;

	(*m_pfDrawThemeBackground) (m_hThemeComboBox, pDC->GetSafeHdc(), CP_DROPDOWNBUTTON, 
		nState, &rect, 0);

	return TRUE;
}
//********************************************************************************
BOOL CBCGPWinXPThemeManager::DrawComboBorder (CDC* pDC, CRect rect,
										BOOL /*bDisabled*/,
										BOOL bIsDropped,
										BOOL bIsHighlighted)
{
	if (m_hThemeWindow == NULL)
	{
		return FALSE;
	}

	if (bIsHighlighted || bIsDropped)
	{
		rect.DeflateRect (1, 1);
		pDC->Draw3dRect (&rect,  globalData.clrHilite, globalData.clrHilite);
	}

	return TRUE;
}
