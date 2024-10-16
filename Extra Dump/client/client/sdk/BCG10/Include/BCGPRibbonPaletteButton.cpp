//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of BCGControlBar Library Professional Edition
// Copyright (C) 1998-2008 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonPaletteButton.cpp: implementation of the CBCGPRibbonPaletteButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonCategory.h"
#include "BCGGlobals.h"
#include "MenuImages.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonLabel.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int nScrollUpID		= -1;
const int nScrollDownID		= -2;
const int nMenuID			= -3;
const int nImageMargin		= 4;
const int nBorderMarginX	= 1;
const int nBorderMarginY	= 3;

////////////////////////////////////////////
// CBCGPRibbonPaletteIcon

IMPLEMENT_DYNCREATE(CBCGPRibbonPaletteIcon, CBCGPRibbonButton)

CBCGPRibbonPaletteIcon::CBCGPRibbonPaletteIcon (
		CBCGPRibbonPaletteButton* pOwner, int nIndex) :
		m_pOwner (pOwner),
		m_nIndex (nIndex)
{
	if (m_pOwner != NULL)
	{
		m_pParent = m_pOwner->m_pParent;
	}

	m_bIsFirstInRow = FALSE;
	m_bIsLastInRow = FALSE;
	m_bIsFirstInColumn = FALSE;
	m_bIsLastInColumn = FALSE;
}
//**************************************************************************
void CBCGPRibbonPaletteIcon::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pOwner);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	if (m_nIndex >= 0)
	{
		if (!m_pOwner->IsDisabled ())
		{
			COLORREF clrText = (COLORREF)-1;

			if (m_pOwner->m_bDefaultButtonStyle || !m_pOwner->m_bIsOwnerDraw)
			{
				clrText = OnFillBackground (pDC);
			}

			m_pOwner->OnDrawPaletteIcon (pDC, m_rect, m_nIndex, this, clrText);

			if (m_pOwner->m_bDefaultButtonStyle || !m_pOwner->m_bIsOwnerDraw)
			{
				OnDrawBorder (pDC);
			}
		}
	}
	else
	{
		CBCGPVisualManager::GetInstance ()->OnDrawRibbonPaletteButton (
			pDC, this);

		// Draw scroll/menu button:
		CBCGPMenuImages::IMAGES_IDS id = 
			m_nIndex == nScrollUpID ? CBCGPMenuImages::IdArowUp :
			m_nIndex == nScrollDownID ? CBCGPMenuImages::IdArowDown : CBCGPMenuImages::IdCustomizeArowDown;

		CRect rectImage = m_rect;

		if (m_nIndex == nMenuID && rectImage.Height () > rectImage.Width () + 2)
		{
			rectImage.bottom = rectImage.top + rectImage.Width () + 2;
		}

		CRect rectWhite = rectImage;
		rectWhite.OffsetRect (0, 1);

		CBCGPMenuImages::Draw (pDC, id, rectWhite, CBCGPMenuImages::ImageWhite);
		CBCGPMenuImages::Draw (pDC, id, rectImage,
			m_bIsDisabled ? CBCGPMenuImages::ImageGray : CBCGPMenuImages::ImageBlack);
	}
}
//***************************************************************************
void CBCGPRibbonPaletteIcon::OnClick (CPoint point)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	m_pOwner->OnClickPaletteIcon (m_pOriginal == NULL ? this : (CBCGPRibbonPaletteIcon*) m_pOriginal);

	if (m_nIndex < 0)
	{
		return;
	}

	CBCGPRibbonPanelMenuBar* pParentMenu = m_pParentMenu;
	if (pParentMenu == NULL && m_nIndex >= 0)
	{
		pParentMenu = m_pOwner->m_pParentMenu;
	}

	if (pParentMenu != NULL)
	{
		ASSERT_VALID (pParentMenu);

		if (m_pOwner->m_nPaletteID != 0)
		{
			m_pOwner->SetNotifyParentID (TRUE);
		}

		m_pOwner->m_bIsFocused = FALSE;
		m_pOwner->OnSetFocus (FALSE);

		pParentMenu->OnClickButton (m_pOwner, point);
	}
	else if (m_nIndex >= 0)
	{
		m_pOwner->NotifyCommand ();
	}
}
//*****************************************************************************
void CBCGPRibbonPaletteIcon::OnLButtonDown (CPoint point)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	if (m_nIndex != nMenuID)
	{
		CBCGPRibbonButton::OnLButtonDown (point);
		return;
	}

	m_bIsHighlighted = m_bIsPressed = FALSE;
	Redraw ();

	m_pOwner->OnShowPopupMenu ();
}
//*****************************************************************************
void CBCGPRibbonPaletteIcon::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	CBCGPRibbonPaletteIcon& src = (CBCGPRibbonPaletteIcon&) s;

	m_nIndex = src.m_nIndex;
	m_pOwner = src.m_pOwner;
	m_bIsChecked = src.m_bIsChecked;
}
//***************************************************************************
CSize CBCGPRibbonPaletteIcon::GetCompactSize (CDC* pDC)
{
	return GetRegularSize (pDC);
}
//***************************************************************************
CSize CBCGPRibbonPaletteIcon::GetRegularSize (CDC* /*pDC*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	CSize sizeIcon = m_pOwner->GetIconSize ();

	if (!m_pOwner->m_bSmallIcons)
	{
		sizeIcon.cx += 2 * nImageMargin;
		sizeIcon.cy += 2 * nImageMargin;
	}

	return sizeIcon;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::IsFirst () const
{
	ASSERT_VALID (this);
	return m_nIndex == nScrollUpID;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::IsLast () const
{
	ASSERT_VALID (this);
	return m_nIndex == nMenuID;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::IsAutoRepeatMode (int& /*nDelay*/) const
{
	ASSERT_VALID (this);
	return m_nIndex == nScrollUpID || m_nIndex == nScrollDownID;
}
//***************************************************************************
BOOL CBCGPRibbonPaletteIcon::OnAutoRepeat ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	if (m_bIsDisabled)
	{
		return FALSE;
	}

	m_pOwner->OnClickPaletteIcon (this);
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPRibbonPaletteIcon::OnAddToQAToolbar (CBCGPRibbonQuickAccessToolbar& qat)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	m_pOwner->OnAddToQAToolbar (qat);
	return TRUE;
}
//*************************************************************************************
void CBCGPRibbonPaletteIcon::OnHighlight (BOOL bHighlight)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	CBCGPRibbonButton::OnHighlight (bHighlight);

	if (!bHighlight)
	{
		CPoint ptCursor;
		::GetCursorPos (&ptCursor);

		CBCGPRibbonPaletteIcon* pCurrIcon = NULL;

		if (m_pParentMenu != NULL)
		{
			m_pParentMenu->ScreenToClient (&ptCursor);

			CBCGPRibbonPanel* pPanel = GetParentPanel ();

			if (pPanel != NULL)
			{
				pCurrIcon = DYNAMIC_DOWNCAST (
					CBCGPRibbonPaletteIcon, pPanel->HitTest (ptCursor));
			}
		}
		else
		{
			m_pOwner->GetParentWnd ()->ScreenToClient (&ptCursor);

			pCurrIcon = DYNAMIC_DOWNCAST (
				CBCGPRibbonPaletteIcon, m_pOwner->HitTest (ptCursor));
		}

		if (pCurrIcon != NULL && pCurrIcon->m_nIndex >= 0)
		{
			return;
		}
	}

	if (m_nIndex >= 0)
	{
		m_pOwner->NotifyHighlightListItem (bHighlight ? m_nIndex : -1);
	}
}
//*************************************************************************************
CString CBCGPRibbonPaletteIcon::GetToolTipText () const
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	return m_pOwner->GetIconToolTip (this);
}
//*************************************************************************************
CString CBCGPRibbonPaletteIcon::GetDescription () const
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pOwner);

	return m_pOwner->GetIconDescription (this);
}
//*************************************************************************************
BOOL CBCGPRibbonPaletteIcon::SetACCData (CWnd* pParent, CBCGPAccessibilityData& data)
{
	CBCGPRibbonButton::SetACCData (pParent, data);

	switch (m_nIndex)
	{
	case nMenuID:
		data.m_nAccRole = ROLE_SYSTEM_BUTTONDROPDOWNGRID;

		data.m_bAccState |= STATE_SYSTEM_HASPOPUP;
		data.m_strAccDefAction = _T("Open");

		if (IsDroppedDown ())
		{
			data.m_bAccState |= STATE_SYSTEM_PRESSED;
			data.m_strAccDefAction = _T("Close");
		}

	case nScrollUpID:
	case nScrollDownID:
		data.m_strAccName = GetToolTipText ();
		break;

	default:
		{
			data.m_bAccState = STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_SELECTABLE;

			if (IsHighlighted ())
			{
				 data.m_bAccState |= STATE_SYSTEM_SELECTED | STATE_SYSTEM_FOCUSED;
			}

			if (IsChecked ())
			{
				data.m_bAccState |= STATE_SYSTEM_CHECKED;
			}
		
			data.m_strAccName = GetToolTipText ();
			data.m_nAccRole = ROLE_SYSTEM_LISTITEM;
			data.m_strAccDefAction = _T("DoubleClick");
		}
		break;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// CBCGPRibbonPaletteButton

IMPLEMENT_DYNCREATE(CBCGPRibbonPaletteButton, CBCGPRibbonButton)

CMap<UINT,UINT,int,int>	CBCGPRibbonPaletteButton::m_mapSelectedItems;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonPaletteButton::CBCGPRibbonPaletteButton()
{
	CommonInit ();
}
//***************************************************************************
CBCGPRibbonPaletteButton::CBCGPRibbonPaletteButton(
		UINT				nID,
		LPCTSTR				lpszText, 
		int					nSmallImageIndex,
		int					nLargeImageIndex,
		CBCGPToolBarImages& imagesPalette) :
	CBCGPRibbonButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex)
{
	CommonInit ();
	imagesPalette.CopyTo (m_imagesPalette);

	m_nIcons = m_imagesPalette.GetCount ();

	CreateIcons ();
}
//***************************************************************************
CBCGPRibbonPaletteButton::CBCGPRibbonPaletteButton(
		UINT				nID,
		LPCTSTR				lpszText, 
		int					nSmallImageIndex,
		int					nLargeImageIndex,
		UINT				uiImagesPaletteResID,
		int					cxPaletteImage) :
	CBCGPRibbonButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex)
{
	CommonInit ();

	if (uiImagesPaletteResID != 0)
	{
		ASSERT (cxPaletteImage != 0);

		m_imagesPalette.Load (uiImagesPaletteResID);

		BITMAP bmp;
		GetObject (m_imagesPalette.GetImageWell (), sizeof (BITMAP), &bmp);

		m_imagesPalette.SetImageSize (
			CSize (cxPaletteImage, bmp.bmHeight), TRUE);

		m_nIcons = m_imagesPalette.GetCount ();

		CreateIcons ();
	}
}
//***************************************************************************
CBCGPRibbonPaletteButton::CBCGPRibbonPaletteButton (
		UINT				nID,
		LPCTSTR				lpszText, 
		int					nSmallImageIndex,
		int					nLargeImageIndex,
		CSize				sizeIcon,
		int					nIconsNum,
		BOOL				bDefaultButtonStyle) :
	CBCGPRibbonButton (nID, lpszText, nSmallImageIndex, nLargeImageIndex)
{
	CommonInit ();

	m_bIsOwnerDraw = TRUE;
	m_bDefaultButtonStyle = bDefaultButtonStyle;

	m_imagesPalette.SetImageSize (sizeIcon);
	m_nIcons = nIconsNum;
}
//***************************************************************************
CBCGPRibbonPaletteButton::~CBCGPRibbonPaletteButton()
{
	RemoveAll ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::AddGroup (
		LPCTSTR lpszGroupName,
		UINT	uiImagesPaletteResID,
		int		cxPaletteImage)
{
	ASSERT_VALID (this);
	ASSERT (lpszGroupName != NULL);

	if (m_bIsOwnerDraw)
	{
		ASSERT (FALSE);
		return;
	}

	m_arGroupNames.Add (lpszGroupName);
	m_arGroupLen.Add (m_imagesPalette.GetCount ());

	if (m_imagesPalette.GetCount () == 0)
	{
		m_imagesPalette.Load (uiImagesPaletteResID);

		BITMAP bmp;
		GetObject (m_imagesPalette.GetImageWell (), sizeof (BITMAP), &bmp);

		m_imagesPalette.SetImageSize (
			CSize (cxPaletteImage, bmp.bmHeight), TRUE);
	}
	else
	{
		ASSERT (cxPaletteImage == m_imagesPalette.GetImageSize ().cx);
		m_imagesPalette.Load (uiImagesPaletteResID, NULL, TRUE);
	}

	m_nIcons = m_imagesPalette.GetCount ();
	RemoveAll ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::AddGroup (
		LPCTSTR lpszGroupName,
		CBCGPToolBarImages& imagesGroup)
{
	ASSERT_VALID (this);

	if (m_bIsOwnerDraw)
	{
		ASSERT (FALSE);
		return;
	}

	m_arGroupNames.Add (lpszGroupName);
	m_arGroupLen.Add (m_imagesPalette.GetCount ());

	if (m_imagesPalette.GetCount () == 0)
	{
		imagesGroup.CopyTo (m_imagesPalette);
	}
	else
	{
		ASSERT (CSize (imagesGroup.GetImageSize ()) == m_imagesPalette.GetImageSize ());
		m_imagesPalette.AddImage (imagesGroup.GetImageWell ());
	}

	m_nIcons = m_imagesPalette.GetCount ();
	RemoveAll ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::AddGroup (
		LPCTSTR lpszGroupName,
		int	nIconsNum)
{
	ASSERT_VALID (this);

	if (!m_bIsOwnerDraw)
	{
		ASSERT (FALSE);
		return;
	}

	m_arGroupNames.Add (lpszGroupName);
	m_arGroupLen.Add (m_nIcons);

	m_nIcons += nIconsNum;
	RemoveAll ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::SetGroupName (int nGroupIndex, LPCTSTR lpszGroupName)
{
	ASSERT_VALID (this);

	m_arGroupNames [nGroupIndex] = lpszGroupName;

	if (m_arIcons.GetSize () == 0)
	{
		return;
	}

	CBCGPRibbonLabel* pLabel = DYNAMIC_DOWNCAST (CBCGPRibbonLabel,
		m_arIcons [m_arGroupLen [nGroupIndex]]);
	if (pLabel == NULL)
	{
		return;
	}

	ASSERT_VALID (pLabel);

	pLabel->SetText (lpszGroupName);

	CBCGPRibbonPanelMenu* pPanelMenu = DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenu, m_pPopupMenu);
	if (pPanelMenu != NULL)
	{
		ASSERT_VALID (pPanelMenu);

		if (pPanelMenu->GetPanel () != NULL)
		{
			CBCGPBaseRibbonElement* pMenuElem = pPanelMenu->GetPanel ()->FindByData ((DWORD_PTR) pLabel);
			
			if (pMenuElem != NULL)
			{
				pMenuElem->SetText (lpszGroupName);
				pMenuElem->Redraw ();
			}
		}
	}
}
//***************************************************************************
LPCTSTR CBCGPRibbonPaletteButton::GetGroupName (int nGroupIndex) const
{
	ASSERT_VALID (this);
	return m_arGroupNames [nGroupIndex];
}
//***************************************************************************
void CBCGPRibbonPaletteButton::SetPalette (CBCGPToolBarImages& imagesPalette)
{
	ASSERT_VALID (this);

	if (m_bIsOwnerDraw)
	{
		ASSERT (FALSE);
		return;
	}

	Clear ();
	imagesPalette.CopyTo (m_imagesPalette);

	m_nIcons = m_imagesPalette.GetCount ();

	CreateIcons ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::SetPalette (UINT uiImagesPaletteResID, int cxPaletteImage)
{
	ASSERT_VALID (this);
	ASSERT (uiImagesPaletteResID != 0);
	ASSERT (cxPaletteImage != 0);

	if (m_bIsOwnerDraw)
	{
		ASSERT (FALSE);
		return;
	}

	Clear ();

	m_imagesPalette.Load (uiImagesPaletteResID);

	BITMAP bmp;
	GetObject (m_imagesPalette.GetImageWell (), sizeof (BITMAP), &bmp);

	m_imagesPalette.SetImageSize (
		CSize (cxPaletteImage, bmp.bmHeight), TRUE);

	m_nIcons = m_imagesPalette.GetCount ();

	CreateIcons ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::Clear ()
{
	ASSERT_VALID (this);
	
	m_mapSelectedItems.RemoveKey (m_nPaletteID == 0 ? m_nID : m_nPaletteID);

	RemoveAll ();

	m_arGroupNames.RemoveAll ();
	m_arGroupLen.RemoveAll ();
	m_arToolTips.RemoveAll ();
	m_imagesPalette.Clear ();

	m_nScrollOffset = 0;
	m_nScrollTotal = 0;
	m_nIcons = 0;
}
//***************************************************************************
void CBCGPRibbonPaletteButton::RedrawIcons ()
{
	ASSERT_VALID (this);

	if (m_pPopupMenu != NULL &&
		m_pPopupMenu->GetMenuBar () != NULL)
	{
		m_pPopupMenu->GetMenuBar ()->RedrawWindow ();
		return;
	}

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		m_arIcons [i]->Redraw ();
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::RemoveAll ()
{
	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		delete m_arIcons [i];
	}

	m_arIcons.RemoveAll ();
}
//*****************************************************************************
void CBCGPRibbonPaletteButton::AddSubItem (CBCGPBaseRibbonElement* pSubItem, int nIndex,
										   BOOL bOnTop)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pSubItem);

	pSubItem->m_bIsOnPaletteTop = bOnTop;

	CBCGPRibbonButton::AddSubItem (pSubItem, nIndex);
}
//***************************************************************************
void CBCGPRibbonPaletteButton::CommonInit ()
{
	m_bIsDefaultCommand = FALSE;
	m_bIsButtonMode = FALSE;
	m_nImagesInRow = 0;
	m_nImagesInColumn = 0;
	m_bSmallIcons = FALSE;
	m_nScrollOffset = 0;
	m_nScrollTotal = 0;
	m_nSelected = 0;
	m_bEnableMenuResize = FALSE;
	m_bMenuResizeVertical = FALSE;
	m_nIconsInRow = -1;
	m_nPaletteID = 0;
	m_bNotifyPaletteID = FALSE;
	m_nPanelColumns = 6;
	m_bIsOwnerDraw = FALSE;
	m_bDefaultButtonStyle = TRUE;
	m_bMenuSideBar = FALSE;
	m_bIsCollapsed = FALSE;
	m_nIcons = 0;
	m_bResetColumns = FALSE;
	m_pParentControl = FALSE;
}
//***************************************************************************
CSize CBCGPRibbonPaletteButton::GetCompactSize (CDC* pDC)
{
	ASSERT_VALID (this);

	if (IsButtonLook ())
	{
		return CBCGPRibbonButton::GetCompactSize (pDC);
	}

	return CBCGPRibbonButton::GetRegularSize (pDC);
}
//***************************************************************************
CSize CBCGPRibbonPaletteButton::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);

	const CSize sizeImage = GetIconSize ();
	CSize sizePanelSmallImage (16, 16);

	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		sizePanelSmallImage = m_pParent->GetImageSize (FALSE);
	}

	m_bSmallIcons = (sizeImage.cx <= sizePanelSmallImage.cx * 3 / 2);

	if (m_bResetColumns && !m_bSmallIcons)
	{
		m_nPanelColumns = 6;

		if (m_pParentMenu != NULL && m_pParentMenu->GetCategory () == NULL)
		{
			// From the default panel button
			m_nPanelColumns = 3;
		}
	}

	m_bResetColumns = FALSE;

	if (IsButtonLook ())
	{
		return CBCGPRibbonButton::GetRegularSize (pDC);
	}

	if (m_arIcons.GetSize () == 0)
	{
		CreateIcons ();
	}

	ASSERT_VALID (m_pParent);

	const CSize sizePanelLargeImage = m_pParent->GetImageSize (TRUE);

	CSize size (0, 0);

	if (m_bSmallIcons)
	{
		size.cx = sizeImage.cx * m_nPanelColumns;

		int nRows = 3;

		if (sizePanelLargeImage != CSize (0, 0) && sizeImage.cy != 0)
		{
			nRows = max (nRows, sizePanelLargeImage.cy * 2 / sizeImage.cy);
		}

		size.cy = nRows * sizeImage.cy + 2 * nBorderMarginY;
	}
	else
	{
		size.cx = (sizeImage.cx + 2 * nImageMargin) * m_nPanelColumns;
		size.cy = sizeImage.cy + 3 * nImageMargin + 2 * nBorderMarginY;
	}

	//---------------------------------------
	// Add space for menu and scroll buttons:
	//---------------------------------------
	size.cx += GetDropDownImageWidth () + 3 * nImageMargin;

	return size;
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);

	if (IsButtonLook ())
	{
		CBCGPRibbonButton::OnDraw (pDC);
		return;
	}

	CRect rectBorder = m_rect;
	rectBorder.DeflateRect (nBorderMarginX, nBorderMarginY);
	rectBorder.right -= 2 * nBorderMarginX;

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonPaletteBorder (
		pDC, this, rectBorder);

	CRect rectImages = m_rect;
	const CSize sizeImage = GetIconSize ();

	CBCGPDrawState ds;

	if (m_imagesPalette.GetCount () > 0)
	{
		m_imagesPalette.SetTransparentColor (globalData.clrBtnFace);
		m_imagesPalette.PrepareDrawImage (ds, sizeImage);
	}

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		m_arIcons [i]->OnDraw (pDC);
	}

	if (m_imagesPalette.GetCount () > 0)
	{
		m_imagesPalette.EndDrawImage (ds);
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::OnAfterChangeRect (pDC);

	m_nScrollTotal = 0;
	m_nScrollOffset = 0;

	const CSize sizeImage = GetIconSize ();

	if (sizeImage.cx == 0 || sizeImage.cy == 0 || IsButtonLook ())
	{
		m_nImagesInRow = 0;
		m_nImagesInColumn = 0;

		RebuildIconLocations ();

		return;
	}

	const int cxMenu = GetDropDownImageWidth () + 6;

	CRect rectImages = m_rect;

	int nMargin = m_bSmallIcons ? 0 : nImageMargin;
	rectImages.DeflateRect (0, nMargin);

	rectImages.right -= cxMenu;

	m_nImagesInRow = rectImages.Width () / (sizeImage.cx + 2 * nMargin);
	m_nImagesInColumn = rectImages.Height () / (sizeImage.cy + 2 * nMargin);

	if (m_nImagesInRow == 0)
	{
		m_nScrollTotal = 0;
	}
	else
	{
		m_nScrollTotal = m_nIcons / m_nImagesInRow - m_nImagesInColumn;

		if (m_nIcons % m_nImagesInRow)
		{
			m_nScrollTotal++;
		}
	}

	RebuildIconLocations ();

	CRect rectBorder = m_rect;
	rectBorder.DeflateRect (nBorderMarginX, nBorderMarginY);
	rectBorder.right -= 2 * nBorderMarginX;

	const int cyMenu = rectBorder.Height () / 3;

	int yButton = rectBorder.top;

	CRect rectButtons = rectBorder;
	rectButtons.left = rectButtons.right - cxMenu;

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, m_arIcons [i]);
		if (pIcon == NULL)
		{
			continue;
		}

		ASSERT_VALID (pIcon);

		if (pIcon->m_nIndex < 0)	// Scroll button
		{
			int yBottom = yButton + cyMenu;

			if (i == m_arIcons.GetSize () - 1)
			{
				yBottom = rectBorder.bottom;
			}

			pIcon->m_rect = CRect (
				rectButtons.left,
				yButton,
				rectButtons.right,
				yBottom);
			
			yButton = yBottom;
		}
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnDrawPaletteIcon (CDC* pDC, CRect rectIcon, 
												  int nIconIndex, CBCGPRibbonPaletteIcon* /*pIcon*/,
												  COLORREF /*clrText*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT (nIconIndex >= 0);
	ASSERT (nIconIndex < m_nIcons);

	if (m_bIsOwnerDraw)
	{
		// You should implement OnDrawPaletteIcon in your
		// CBCGPRibbonPaletteButton-derived class!
		ASSERT (FALSE);
		return;
	}

	if (!m_bSmallIcons)
	{
		rectIcon.DeflateRect (nImageMargin, nImageMargin);
	}

	m_imagesPalette.Draw (pDC, rectIcon.left, rectIcon.top,
		nIconIndex, FALSE, IsDisabled ());
}
//***************************************************************************
void CBCGPRibbonPaletteButton::CreateIcons ()
{
	ASSERT_VALID (this);

	int nGroupIndex = 0;
	
	for (int i = 0; i < m_nIcons; i++)
	{
		if (nGroupIndex < m_arGroupLen.GetSize () &&
			i == m_arGroupLen [nGroupIndex])
		{
			CString strLabel = m_arGroupNames [nGroupIndex++];
			if (!strLabel.IsEmpty ())
			{
				strLabel = _T("   ") + strLabel;
			}

			CBCGPRibbonLabel* pLabel = new CBCGPRibbonLabel (strLabel);
			pLabel->SetData ((DWORD_PTR) pLabel);

			m_arIcons.Add (pLabel);
		}

		CBCGPRibbonPaletteIcon* pIcon = new CBCGPRibbonPaletteIcon (this, i);

		if (i == m_nSelected)
		{
			pIcon->m_bIsChecked = TRUE;
		}

		m_arIcons.Add (pIcon);
	}

	m_arIcons.Add (new CBCGPRibbonPaletteIcon (this, nScrollUpID));
	m_arIcons.Add (new CBCGPRibbonPaletteIcon (this, nScrollDownID));
	m_arIcons.Add (new CBCGPRibbonPaletteIcon (this, nMenuID));
}
//***************************************************************************
void CBCGPRibbonPaletteButton::RebuildIconLocations ()
{
	ASSERT_VALID (this);

	CRect rectImages = m_rect;

	const CSize sizeImage = GetIconSize ();

	int nMargin = m_bSmallIcons ? 0 : nImageMargin;
	rectImages.DeflateRect (0, nMargin);

	int yOffset = max (0, 
		(rectImages.Height () - (sizeImage.cy + 2 * nMargin) * m_nImagesInColumn) / 2);

	int nRow = 0;
	int nColumn = 0;

	CSize sizeIcon (sizeImage.cx + 2 * nMargin, sizeImage.cy + 2 * nMargin);

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, m_arIcons [i]);
		if (pIcon == NULL)
		{
			continue;
		}

		ASSERT_VALID (pIcon);

		pIcon->m_bIsFirstInRow = FALSE;
		pIcon->m_bIsLastInRow = FALSE;
		pIcon->m_bIsFirstInColumn = FALSE;
		pIcon->m_bIsLastInColumn = FALSE;

		pIcon->m_pParentMenu = m_pParentMenu;

		if (pIcon->m_nIndex < 0)	// Scroll button
		{
			if (pIcon->m_nIndex == nScrollUpID)
			{
				pIcon->m_bIsDisabled = (m_nScrollOffset == 0);
			}
			else if (pIcon->m_nIndex == nScrollDownID)
			{
				pIcon->m_bIsDisabled = (m_nScrollOffset >= m_nScrollTotal);
			}

			continue;
		}

		if (nRow - m_nScrollOffset >= m_nImagesInColumn || 
			nRow < m_nScrollOffset)
		{
			pIcon->m_rect.SetRectEmpty ();
		}
		else
		{
			CRect rectIcon (
				CPoint (
					rectImages.left + sizeIcon.cx * nColumn + nImageMargin / 2,
					rectImages.top + sizeIcon.cy * (nRow - m_nScrollOffset) + yOffset),
				sizeIcon);

			pIcon->m_rect = rectIcon;

			pIcon->m_bIsFirstInRow = (nColumn == 0);
			pIcon->m_bIsLastInRow = (nColumn == m_nImagesInRow - 1);
			pIcon->m_bIsFirstInColumn = (nRow - m_nScrollOffset == 0);
			pIcon->m_bIsLastInColumn = (nRow - m_nScrollOffset == m_nImagesInColumn - 1);
		}

		nColumn++;
		
		if (nColumn == m_nImagesInRow)
		{
			nColumn = 0;
			nRow++;
		}
	}
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::HitTest (CPoint point)
{
	ASSERT_VALID (this);

	if (IsDisabled ())
	{
		return NULL;
	}

	if (IsButtonLook ())
	{
		return CBCGPRibbonButton::HitTest (point);
	}

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		if (m_arIcons [i]->GetRect ().PtInRect (point))
		{
			return m_arIcons [i];
		}
	}

	return NULL;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::GetPressed ()
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arIcons [i];
		ASSERT_VALID (pButton);

		CBCGPBaseRibbonElement* pElem = pButton->GetPressed ();
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}
	
	return NULL;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::GetHighlighted ()
{
	ASSERT_VALID (this);

	if (IsButtonLook ())
	{
		return CBCGPRibbonButton::GetHighlighted ();
	}

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arIcons [i];
		ASSERT_VALID (pButton);

		CBCGPBaseRibbonElement* pElem = pButton->GetHighlighted ();
		if (pElem != NULL)
		{
			ASSERT_VALID (pElem);
			return pElem;
		}
	}
	
	return NULL;
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnClickPaletteIcon (CBCGPRibbonPaletteIcon* pIcon)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pIcon);

	switch (pIcon->m_nIndex)
	{
	case nScrollUpID:
		m_nScrollOffset = max (0, m_nScrollOffset - 1);
		RebuildIconLocations ();
		Redraw ();
		break;

	case nScrollDownID:
		m_nScrollOffset = min (m_nScrollTotal, m_nScrollOffset + 1);
		RebuildIconLocations ();
		Redraw ();
		break;

	case nMenuID:
		// Already shown in CBCGPRibbonPaletteIcon::OnLButtonDown
		break;

	default:
		{
			int nIconIndex = 0;

			for (int i = 0; i < m_arIcons.GetSize (); i++)
			{
				CBCGPRibbonPaletteIcon* pListIcon = DYNAMIC_DOWNCAST (
					CBCGPRibbonPaletteIcon, m_arIcons [i]);
				if (pListIcon == NULL)
				{
					continue;
				}

				ASSERT_VALID (pListIcon);

				if (pListIcon->m_bIsChecked)
				{
					pListIcon->m_bIsChecked = FALSE;
				}

				if (pListIcon == pIcon)
				{
					m_nSelected = nIconIndex;
					pIcon->m_bIsChecked = TRUE;

					if (pIcon->m_rect.IsRectEmpty () && m_nImagesInRow > 0)
					{
						m_nScrollOffset = nIconIndex / m_nImagesInRow;
						m_nScrollOffset = min (m_nScrollTotal, m_nScrollOffset);
						RebuildIconLocations ();
					}
				}

				nIconIndex++;
			}
		}

		Redraw ();

		m_mapSelectedItems.SetAt (m_nPaletteID == 0 ? m_nID : m_nPaletteID, pIcon->m_nIndex);
	}
}
//*****************************************************************************
void CBCGPRibbonPaletteButton::CopyFrom (const CBCGPBaseRibbonElement& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::CopyFrom (s);

	if (!s.IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteButton)))
	{
		return;
	}

	CBCGPRibbonPaletteButton& src = (CBCGPRibbonPaletteButton&) s;

	RemoveAll ();

	src.m_imagesPalette.CopyTo (m_imagesPalette);
	m_bSmallIcons = src.m_bSmallIcons;
	m_nSelected = src.m_nSelected;
	m_bEnableMenuResize = src.m_bEnableMenuResize;
	m_bMenuResizeVertical = src.m_bMenuResizeVertical;
	m_nPaletteID = src.m_nPaletteID;
	m_bIsButtonMode = src.m_bIsButtonMode;
	m_nIconsInRow = src.m_nIconsInRow;
	m_nPanelColumns = src.m_nPanelColumns;
	m_bIsOwnerDraw = src.m_bIsOwnerDraw;
	m_bDefaultButtonStyle = src.m_bDefaultButtonStyle;
	m_nIcons = src.m_nIcons;
	m_bMenuSideBar = src.m_bMenuSideBar;

	ASSERT (src.m_arGroupNames.GetSize () == src.m_arGroupLen.GetSize ());

	m_arGroupNames.RemoveAll ();
	m_arGroupLen.RemoveAll ();

	int i = 0;

	for (i = 0; i < src.m_arGroupNames.GetSize (); i++)
	{
		m_arGroupNames.Add (src.m_arGroupNames [i]);
		m_arGroupLen.Add (src.m_arGroupLen [i]);
	}

	m_arToolTips.RemoveAll ();

	for (i = 0; i < src.m_arToolTips.GetSize (); i++)
	{
		m_arToolTips.Add (src.m_arToolTips [i]);
	}

	CreateIcons ();
}
//*****************************************************************************
void CBCGPRibbonPaletteButton::SetParentCategory (CBCGPRibbonCategory* pParent)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::SetParentCategory (pParent);

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		ASSERT_VALID (m_arIcons [i]);
		m_arIcons [i]->SetParentCategory (pParent);
	}
}
//*****************************************************************************
void CBCGPRibbonPaletteButton::OnShowPopupMenu ()
{
	ASSERT_VALID (this);

	if (m_pParentControl != NULL && m_pRibbonBar == NULL)
	{
		CWnd* pWndFrame = AfxGetMainWnd ();		
		ASSERT_VALID (pWndFrame);

		if (pWndFrame->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
		{
			m_pRibbonBar = ((CBCGPFrameWnd*) pWndFrame)->GetRibbonBar ();
		}
		else if (pWndFrame->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
		{
			m_pRibbonBar = ((CBCGPMDIFrameWnd*) pWndFrame)->GetRibbonBar ();
		}

		if (m_pRibbonBar == NULL || !m_pRibbonBar->IsWindowVisible ())
		{
			ASSERT(FALSE);
			return;
		}
	}

	CWnd* pWndParent = m_pParentControl == NULL ? GetParentWnd () : m_pParentControl;
	if (pWndParent->GetSafeHwnd () == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	CBCGPRibbonBar* pTopLevelRibbon = GetTopLevelRibbonBar ();
	if (pTopLevelRibbon->GetSafeHwnd () == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	CBCGPBaseRibbonElement::OnShowPopupMenu ();

	const BOOL bIsRTL = (pTopLevelRibbon->GetExStyle () & WS_EX_LAYOUTRTL);

	if (m_arIcons.GetSize () == 0)
	{
		CreateIcons ();
	}

	int nSel = GetLastSelectedItem (m_nPaletteID == 0 ? m_nID : m_nPaletteID);
	if (nSel >= 0)
	{
		SelectItem (nSel);
	}

	CBCGPBaseRibbonElement* pMenuButton = IsButtonLook () ? this : m_arIcons [m_arIcons.GetSize () - 1];

	CWnd* pWndOwner = pTopLevelRibbon->GetSafeOwner ();

	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (this);

	pMenu->SetParentRibbonElement (pMenuButton);
	pMenu->SetMenuMode ();
	
	CRect rectBtn = GetRect ();
	if (m_pParentControl != NULL)
	{
		m_pParentControl->GetClientRect (rectBtn);
	}

	pWndParent->ClientToScreen (&rectBtn);

	int nMargin = m_bSmallIcons ? 0 : nImageMargin;
	const CSize sizeImage = GetIconSize ();

	CSize sizeIcon (sizeImage.cx + 2 * nMargin, sizeImage.cy + 2 * nMargin);

	int x = bIsRTL ? rectBtn.right : rectBtn.left;
	int y = rectBtn.bottom;

	if (IsMenuMode ())
	{
		x = bIsRTL ? rectBtn.left : rectBtn.right;
		y = rectBtn.top;
	}

	if (!IsButtonLook ())
	{
		x = bIsRTL ? rectBtn.right : rectBtn.left;
		y = rectBtn.top + nBorderMarginY;
	}

	if (m_nIconsInRow > 0)
	{
		pMenu->SetPreferedSize (CSize (m_nIconsInRow * sizeIcon.cx, 0));
	}
	else
	{
		const int nPanelColumns = pMenuButton == this ? 4 : m_nPanelColumns;
		const int nIconsInRow = m_bSmallIcons ? 10 : max (nPanelColumns, 4);

		pMenu->SetPreferedSize (CSize (nIconsInRow * sizeIcon.cx, 0));
	}

	pMenu->Create (pWndOwner, x, y, (HMENU) NULL);
	pMenuButton->SetDroppedDown (pMenu);

	if (pMenu->HasBeenResized ())
	{
		pMenu->TriggerResize ();
	}
}
//**************************************************************************
void CBCGPRibbonPaletteButton::SelectItem (int nItemIndex)
{
	ASSERT_VALID (this);

	m_nSelected = nItemIndex;

	int nCurrIndex = 0;

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, m_arIcons [i]);

		if (pIcon == NULL)
		{
			continue;
		}

		ASSERT_VALID (pIcon);

		if (pIcon->m_bIsChecked)
		{
			pIcon->m_bIsChecked = FALSE;
		}

		if (nCurrIndex == nItemIndex)
		{
			pIcon->m_bIsChecked = TRUE;
		}

		nCurrIndex++;
	}

	m_mapSelectedItems.SetAt (m_nPaletteID == 0 ? m_nID : m_nPaletteID, m_nSelected);

	Redraw ();
}
//**************************************************************************
void CBCGPRibbonPaletteButton::SetItemToolTip (int nItemIndex, LPCTSTR lpszToolTip)
{
	ASSERT_VALID (this);

	if (nItemIndex < 0)
	{
		ASSERT (FALSE);
		return;
	}

	if (nItemIndex >= m_arToolTips.GetSize ())
	{
		m_arToolTips.SetSize (nItemIndex + 1);
	}

	m_arToolTips [nItemIndex] = lpszToolTip == NULL ? _T("") : lpszToolTip;
}
//**************************************************************************
LPCTSTR CBCGPRibbonPaletteButton::GetItemToolTip (int nItemIndex) const
{
	ASSERT_VALID (this);

	if (nItemIndex < 0 || nItemIndex >= m_arToolTips.GetSize ())
	{
		ASSERT (FALSE);
		return NULL;
	}

	return m_arToolTips [nItemIndex];
}
//**************************************************************************
void CBCGPRibbonPaletteButton::RemoveItemToolTips ()
{
	ASSERT_VALID (this);
	m_arToolTips.RemoveAll ();
}
//**************************************************************************
CString CBCGPRibbonPaletteButton::GetIconToolTip (const CBCGPRibbonPaletteIcon* pIcon) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pIcon);

	int nIndex = pIcon->m_nIndex;

	CString strTip;

	switch (nIndex)
	{
	case nScrollUpID:
	case nScrollDownID:
		{
			CBCGPLocalResource locaRes;

			if (m_nImagesInColumn == 1)
			{
				strTip.Format (IDS_BCGBARRES_GALLERY_ROW1_FMT, m_nScrollOffset + 1, m_nScrollTotal + m_nImagesInColumn);
			}
			else
			{
				strTip.Format (IDS_BCGBARRES_GALLERY_ROW2_FMT, m_nScrollOffset + 1, m_nScrollOffset + m_nImagesInColumn, m_nScrollTotal + m_nImagesInColumn);
			}
			return strTip;
		}

	case nMenuID:
		{
			CBCGPLocalResource locaRes;
			strTip.LoadString (IDS_BCGBARRES_MORE);
		}
		return strTip;
	}

	if (nIndex < 0 || nIndex >= m_arToolTips.GetSize ())
	{
		return _T("");
	}

	return m_arToolTips [nIndex];
}
//**************************************************************************
CString CBCGPRibbonPaletteButton::GetIconDescription (const CBCGPRibbonPaletteIcon* pIcon) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pIcon);

	int nIndex = pIcon->m_nIndex;

	switch (nIndex)
	{
	case nScrollUpID:
	case nScrollDownID:
	case nMenuID:
		return m_strDescription;
	}

	return _T("");
}
//**************************************************************************
int CBCGPRibbonPaletteButton::GetLastSelectedItem (UINT uiCmdID)
{
	int nIndex = -1;

	m_mapSelectedItems.Lookup (uiCmdID, nIndex);
	return nIndex;
}
//**************************************************************************
void CBCGPRibbonPaletteButton::GetMenuItems (
	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons)
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arSubItems.GetSize (); i++)
	{
		arButtons.Add (m_arSubItems [i]);
	}

	for (i = 0; i < m_arIcons.GetSize () - 3 /* Scroll buttons */; i++)
	{
		arButtons.Add (m_arIcons [i]);
	}
}
//***************************************************************************
int CBCGPRibbonPaletteButton::GetMenuRowHeight () const
{
	ASSERT_VALID (this);

	int nMargin = m_bSmallIcons ? 0 : nImageMargin;
	const CSize sizeImage = GetIconSize ();
	
	return sizeImage.cy + 2 * nMargin;
}
//***************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPaletteButton::GetDroppedDown ()
{
	ASSERT_VALID (this);

	if (m_arIcons.GetSize () > 0)
	{
		CBCGPBaseRibbonElement* pMenuButton = m_arIcons [m_arIcons.GetSize () - 1];
		ASSERT_VALID (pMenuButton);

		if (pMenuButton->IsDroppedDown ())
		{
			return pMenuButton;
		}
	}

	return CBCGPRibbonButton::GetDroppedDown ();
}
//***************************************************************************
void CBCGPRibbonPaletteButton::OnEnable (BOOL bEnable)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::OnEnable (bEnable);

	for (int i = 0; i < m_arIcons.GetSize (); i++)
	{
		ASSERT_VALID (m_arIcons [i]);
		m_arIcons [i]->m_bIsDisabled = !bEnable;
	}
}
//***************************************************************************
void CBCGPRibbonPaletteButton::SetNotifyParentID (BOOL bSet)
{
	m_bNotifyPaletteID = bSet;

	if (m_pOriginal != NULL)
	{
		ASSERT_VALID (m_pOriginal);

		CBCGPRibbonPaletteButton* pOriginal = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteButton, m_pOriginal);
		if (pOriginal != NULL)
		{
			pOriginal->m_bNotifyPaletteID = bSet;
		}
	}
}
//***************************************************************************
BOOL CBCGPRibbonPaletteButton::OnKey (BOOL /*bIsMenuKey*/)
{
	ASSERT_VALID (this);

	return CBCGPRibbonButton::OnKey (TRUE);
}
//***************************************************************************
CRect CBCGPRibbonPaletteButton::GetKeyTipRect (CDC* pDC, BOOL bIsMenu)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (IsButtonLook ())
	{
		return CBCGPRibbonButton::GetKeyTipRect (pDC, bIsMenu);
	}

	CSize sizeKeyTip = GetKeyTipSize (pDC);
	CRect rectKeyTip (0, 0, 0, 0);

	if (sizeKeyTip == CSize (0, 0) || m_rect.IsRectEmpty ())
	{
		return rectKeyTip;
	}

	rectKeyTip.left = m_rect.right - sizeKeyTip.cx / 2;
	rectKeyTip.top = m_rect.bottom - sizeKeyTip.cy / 2;

	// Find 'nMenuID' button:
	if (m_arIcons.GetSize () > 0)
	{
		CBCGPRibbonPaletteIcon* pMenuButton = DYNAMIC_DOWNCAST (CBCGPRibbonPaletteIcon, m_arIcons [m_arIcons.GetSize () - 1]);
		if (pMenuButton != NULL)
		{
			ASSERT_VALID (pMenuButton);

			const CRect rectMenu = pMenuButton->GetRect ();

			if (pMenuButton->m_nIndex == nMenuID && !rectMenu.IsRectEmpty ())
			{
				rectKeyTip.left = rectMenu.CenterPoint ().x;
				rectKeyTip.top = rectMenu.bottom - 3;
			}
		}
	}

	rectKeyTip.right = rectKeyTip.left + sizeKeyTip.cx;
	rectKeyTip.bottom = rectKeyTip.top + sizeKeyTip.cy;

	return rectKeyTip;
}
//***************************************************************************
CSize CBCGPRibbonPaletteButton::GetIconSize () const
{
	ASSERT_VALID (this);

	CSize sizeImage = m_imagesPalette.GetImageSize ();

	if (globalData.GetRibbonImageScale () != 1.)
	{
		const double dblScale = 1. + (globalData.GetRibbonImageScale () - 1.) / 2;

		sizeImage.cx = (int)(.5 + dblScale * sizeImage.cx);
		sizeImage.cy = (int)(.5 + dblScale * sizeImage.cy);
	}

	return sizeImage;
}
//**************************************************************************************
void CBCGPRibbonPaletteButton::OnRTLChanged (BOOL bIsRTL)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::OnRTLChanged (bIsRTL);

	m_imagesPalette.Mirror ();
}
//**************************************************************************************
void CBCGPRibbonPaletteButton::OnSetFocus (BOOL bSet)
{
	ASSERT_VALID (this);

	CBCGPRibbonButton::OnSetFocus (bSet);

	for (int i = (int)m_arIcons.GetSize () - 1; i >= 0; i--)
	{
		CBCGPRibbonPaletteIcon* pIcon = DYNAMIC_DOWNCAST (
			CBCGPRibbonPaletteIcon, m_arIcons [i]);
		if (pIcon != NULL)
		{
			ASSERT_VALID (pIcon);

			if (pIcon->m_nIndex == nMenuID)
			{
				pIcon->m_bIsFocused = bSet;
				pIcon->Redraw ();
				break;
			}
		}
	}
}
//**************************************************************************************
BOOL CBCGPRibbonPaletteButton::SetACCData (CWnd* pParent, CBCGPAccessibilityData& data)
{
	CBCGPRibbonButton::SetACCData (pParent, data);
	data.m_nAccRole = IsCommandAreaHighlighted () ? ROLE_SYSTEM_SPLITBUTTON : ROLE_SYSTEM_BUTTONDROPDOWNGRID;

	return TRUE;
}

////////////////////////////////////////////////
// CBCGPRibbonPaletteMenuButton

IMPLEMENT_DYNCREATE(CBCGPRibbonPaletteMenuButton, CBCGPToolbarMenuButton)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonPaletteMenuButton::CBCGPRibbonPaletteMenuButton()
{
	CommonInit ();
}
//**************************************************************************************
CBCGPRibbonPaletteMenuButton::CBCGPRibbonPaletteMenuButton(UINT uiID, int iImage, LPCTSTR lpszText,
		CBCGPToolBarImages& imagesPalette) :
	CBCGPToolbarMenuButton (uiID, NULL, iImage, lpszText),
	m_paletteButton (0, _T(""), -1, -1, imagesPalette)
{
	CommonInit ();
}
//**************************************************************************************
CBCGPRibbonPaletteMenuButton::CBCGPRibbonPaletteMenuButton (UINT uiID, int iImage, LPCTSTR lpszText,
		UINT uiImagesPaletteResID, int cxPaletteImage) :
	CBCGPToolbarMenuButton (uiID, NULL, iImage, lpszText),
	m_paletteButton (0, _T(""), -1, -1, uiImagesPaletteResID, cxPaletteImage)
{
	CommonInit ();
}
//**************************************************************************************
CBCGPRibbonPaletteMenuButton::~CBCGPRibbonPaletteMenuButton ()
{
}
//**************************************************************************************
void CBCGPRibbonPaletteMenuButton::CommonInit ()
{
	CBCGPRibbonBar* pRibbon = NULL;

	CFrameWnd* pParentFrame = m_pWndParent == NULL ?
		DYNAMIC_DOWNCAST (CFrameWnd, AfxGetMainWnd ()) :
		BCGCBProGetTopLevelFrame (m_pWndParent);

	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pParentFrame);
	if (pMainFrame != NULL)
	{
		pRibbon = pMainFrame->GetRibbonBar ();
	}
	else	// Maybe, SDI frame...
	{
		CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pParentFrame);
		if (pFrame != NULL)
		{
			pRibbon = pFrame->GetRibbonBar ();
		}
	}

	if (pRibbon != NULL)
	{
		ASSERT_VALID (pRibbon);
		m_paletteButton.SetParentRibbonBar (pRibbon);
	}
	else
	{
		ASSERT (FALSE);	// Main farme should have the ribbon bar!
	}
}
//**************************************************************************************
CBCGPPopupMenu* CBCGPRibbonPaletteMenuButton::CreatePopupMenu ()
{
	ASSERT_VALID (this);

	m_paletteButton.SetID (m_nID);

	m_paletteButton.CBCGPBaseRibbonElement::OnShowPopupMenu ();

	if (m_paletteButton.m_nIcons == 0)
	{
		TRACE(_T("The palette is not initialized! You should add palette icons first.\n"));
		ASSERT (FALSE);
		return NULL;
	}

	if (m_paletteButton.m_arIcons.GetSize () == 0)
	{
		m_paletteButton.CreateIcons ();
	}

	m_paletteButton.SelectItem (CBCGPRibbonPaletteButton::GetLastSelectedItem (
		m_paletteButton.m_nPaletteID == 0 ? m_paletteButton.m_nID : m_paletteButton.m_nPaletteID));

	for (int i = 0; i < m_paletteButton.m_arSubItems.GetSize (); i++)
	{
		ASSERT_VALID (m_paletteButton.m_arSubItems [i]);
		m_paletteButton.m_arSubItems [i]->SetParentRibbonBar (m_paletteButton.m_pRibbonBar);
	}

	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (&m_paletteButton);

	pMenu->SetMenuMode ();
	
	int nMargin = m_paletteButton.m_bSmallIcons ? 0 : nImageMargin;
	const CSize sizeImage = m_paletteButton.GetIconSize ();
	CSize sizeIcon (sizeImage.cx + 2 * nMargin, sizeImage.cy + 2 * nMargin);

	int nIconsInRow = m_paletteButton.m_nIconsInRow > 0 ? 
		m_paletteButton.m_nIconsInRow : m_paletteButton.m_bSmallIcons ? 10 : 4;

	pMenu->SetPreferedSize (CSize (nIconsInRow * sizeIcon.cx, 0));
	pMenu->EnableCustomizeMenu (FALSE);

	return pMenu;
}
//**************************************************************************************
void CBCGPRibbonPaletteMenuButton::CopyFrom (const CBCGPToolbarButton& s)
{
	CBCGPToolbarMenuButton::CopyFrom (s);

	const CBCGPRibbonPaletteMenuButton& src = (const CBCGPRibbonPaletteMenuButton&) s;

	m_paletteButton.CopyFrom (src.m_paletteButton);
}

#endif // BCGP_EXCLUDE_RIBBON
