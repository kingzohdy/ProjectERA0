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

// BCGPButton.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBPro.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPButton.h"
#include "BCGGlobals.h"
#include "BCGPToolBarImages.h"
#include "BCGPPopupMenu.h"
#include "BCGPVisualManager.h"
#include "BCGPDrawManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int nImageHorzMargin = 10;
static const int nVertMargin = 5;
static const COLORREF clrDefault = (COLORREF) -1;
static const UINT IdAutoCommand = 1;

BOOL CBCGPButton::m_bWinXPTheme = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CBCGPButton

IMPLEMENT_DYNCREATE(CBCGPButton, CButton)

CBCGPButton::CBCGPButton()
{
	m_bPushed			= FALSE;
	m_bClickiedInside	= FALSE;
	m_bHighlighted		= FALSE;
	m_bCaptured			= FALSE;
	m_nFlatStyle		= BUTTONSTYLE_3D;
	m_nAlignStyle		= ALIGN_CENTER;
	m_sizeImage			= CSize (0, 0);
	m_nStdImageId		= (CMenuImages::IMAGES_IDS) -1;
	m_nStdImageDisabledId = (CMenuImages::IMAGES_IDS) -1;
	m_StdImageState		= CMenuImages::ImageBlack;
	m_bFullTextTooltip	= FALSE;
	m_bRighImage		= FALSE;
	m_hCursor			= NULL;
	m_sizePushOffset	= CSize (2, 2);
	m_bHover			= FALSE;
	m_clrRegular		= clrDefault;
	m_clrHover			= clrDefault;
	m_clrFace			= (COLORREF)-1;
	m_bDrawFocus		= TRUE;
	m_bTransparent		= FALSE;
	m_hFont				= NULL;
	m_bDelayFullTextTooltipSet = FALSE;
	m_bGrayDisabled		= TRUE;
	m_bChecked			= FALSE;
	m_bCheckButton		= FALSE;
	m_bRadioButton		= FALSE;
	m_bAutoCheck		= FALSE;
	m_bHighlightChecked	= TRUE;
	m_nAutoRepeatTimeDelay = 0;
	m_bResponseOnButtonDown = FALSE;
}
//****************************************************************************
CBCGPButton::~CBCGPButton()
{
	CleanUp ();
}
//****************************************************************************
void CBCGPButton::CleanUp ()
{
	m_nStdImageId = (CMenuImages::IMAGES_IDS) -1;
	m_nStdImageDisabledId = (CMenuImages::IMAGES_IDS) -1;

	m_sizeImage = CSize (0, 0);

	m_Image.DeleteImageList ();
	m_ImageHot.DeleteImageList ();
	m_ImageDisabled.DeleteImageList ();

	m_ImageChecked.DeleteImageList ();
	m_ImageCheckedHot.DeleteImageList ();
	m_ImageCheckedDisabled.DeleteImageList ();
}


BEGIN_MESSAGE_MAP(CBCGPButton, CButton)
	//{{AFX_MSG_MAP(CBCGPButton)
	ON_WM_ERASEBKGND()
	ON_WM_CANCELMODE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ENABLE()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_SETIMAGE, OnSetImage)
	ON_MESSAGE(BM_GETIMAGE, OnGetImage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPButton message handlers

void CBCGPButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT (lpDIS != NULL);
	ASSERT (lpDIS->CtlType == ODT_BUTTON);

	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	CRect rectClient = lpDIS->rcItem;

	OnFillBackground (pDC, rectClient);
	OnDrawBorder (pDC, rectClient, lpDIS->itemState);

	//---------------------
	// Draw button content:
	//---------------------
	OnDraw (pDC, rectClient, lpDIS->itemState);

	if ((lpDIS->itemState & ODS_FOCUS) && m_bDrawFocus)
	{
		OnDrawFocusRect (pDC, rectClient);
	}
}
//****************************************************************************
void CBCGPButton::PreSubclassWindow() 
{
	InitStyle (GetStyle ());

	ModifyStyle (BS_DEFPUSHBUTTON, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}
//****************************************************************************
BOOL CBCGPButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	InitStyle (cs.style);

	cs.style |= BS_OWNERDRAW;
	cs.style &= ~BS_DEFPUSHBUTTON;

	return CButton::PreCreateWindow(cs);
}
//****************************************************************************
void CBCGPButton::InitStyle (DWORD dwStyle)
{
	switch (dwStyle & 0x0F) {
	case BS_CHECKBOX:
		m_bCheckButton = TRUE	;
		break	;
	case BS_AUTOCHECKBOX:
		m_bCheckButton =
			m_bAutoCheck = TRUE	;
		break	;
	case BS_RADIOBUTTON:
		m_bRadioButton = TRUE	;
		break	;
	case BS_AUTORADIOBUTTON:
		m_bRadioButton =
			m_bAutoCheck = TRUE	;
		break	;
	}
	if (m_bCheckButton || m_bRadioButton) {
		switch (dwStyle & BS_CENTER) {
		case BS_LEFT:
			m_nAlignStyle = CBCGPButton::ALIGN_LEFT	;
			break	;
		case BS_RIGHT:
			m_nAlignStyle = CBCGPButton::ALIGN_RIGHT	;
			break	;
		case BS_CENTER:
			m_nAlignStyle = CBCGPButton::ALIGN_CENTER	;
			break	;
		}
	}
}
//****************************************************************************
BOOL CBCGPButton::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//****************************************************************************
void CBCGPButton::OnFillBackground (CDC* pDC, const CRect& rectClient)
{
	if (m_bTransparent)
	{
		// Copy background from the parent window
		globalData.DrawParentBackground (this, pDC);
	}
	else
	{
		if (m_clrFace == (COLORREF)-1)
		{
			pDC->FillRect (rectClient, &globalData.brBtnFace);
		}
		else
		{
			pDC->FillSolidRect (rectClient, m_clrFace);
		}
	}

	if (m_bChecked && m_bHighlightChecked && !(m_bPushed && m_bHighlighted))
	{
		CBCGPDrawManager dm (*pDC);
		dm.HighlightRect (rectClient);
	}
}
//****************************************************************************
void CBCGPButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	CRect rectText = rect;
	CRect rectImage = rect;

	CString strText;
	GetWindowText (strText);

	if (m_sizeImage.cx != 0)
	{
		if (!strText.IsEmpty ())
		{
			if (m_bRighImage)
			{
				rectText.right -= m_sizeImage.cx + GetImageHorzMargin () / 2;
				rectImage.left = rectText.right;
				rectImage.right -= GetImageHorzMargin () / 2;
			}
			else
			{
				rectText.left +=  m_sizeImage.cx + GetImageHorzMargin () / 2;
				rectImage.left += GetImageHorzMargin () / 2;
				rectImage.right = rectText.left;
			}
		}

		// Center image:
		rectImage.DeflateRect ((rectImage.Width () - m_sizeImage.cx) / 2,
			max (0, (rect.Height () - m_sizeImage.cy) / 2));
	}
	else
	{
		rectImage.SetRectEmpty ();
	}

	//-----------
	// Draw text:
	//-----------
	CFont* pOldFont = SelectFont (pDC);
	ASSERT(pOldFont != NULL);

	pDC->SetBkMode (TRANSPARENT);
	COLORREF clrText = m_clrRegular == clrDefault ? 
		globalData.clrBtnText : m_clrRegular;
	
	if (m_bHighlighted && m_clrHover != clrDefault)
	{
		clrText = m_clrHover;
	}

	UINT uiDTFlags = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	switch (m_nAlignStyle)
	{
	case ALIGN_LEFT:
		uiDTFlags |= DT_LEFT;
		rectText.left += GetImageHorzMargin () / 2;
		break;

	case ALIGN_RIGHT:
		uiDTFlags |= DT_RIGHT;
		rectText.right -= GetImageHorzMargin () / 2;
		break;

	case ALIGN_CENTER:
		uiDTFlags |= DT_CENTER;
	}

	if ((uiState & ODS_DISABLED) && m_bGrayDisabled)
	{
		pDC->SetTextColor (globalData.clrBtnHilite);

		CRect rectShft = rectText;
		rectShft.OffsetRect (1, 1);
		pDC->DrawText (strText, rectShft, uiDTFlags);

		clrText = globalData.clrGrayedText;
	}

	pDC->SetTextColor (clrText);

	if (m_bDelayFullTextTooltipSet)
	{
		BOOL bIsFullText = pDC->GetTextExtent (strText).cx <= rectText.Width ();
		SetTooltip (bIsFullText ? NULL : (LPCTSTR) strText);
		m_bDelayFullTextTooltipSet = FALSE;
	}

	OnDrawText (pDC, rectText, strText, uiDTFlags, uiState);

	//------------
	// Draw image:
	//------------
	if (!rectImage.IsRectEmpty ())
	{
		if (m_nStdImageId != (CMenuImages::IMAGES_IDS) -1)
		{
			CMenuImages::IMAGES_IDS id = m_nStdImageId;

			if ((uiState & ODS_DISABLED) && m_bGrayDisabled &&
				m_nStdImageDisabledId != 0)
			{
				id = m_nStdImageDisabledId;
			}

			CMenuImages::Draw (pDC, id, rectImage.TopLeft (), m_StdImageState);
		}
		else
		{
			CImageList& imageChecked = 
				(((uiState & ODS_DISABLED) && m_bGrayDisabled && m_ImageCheckedDisabled.GetSafeHandle () != NULL) ?
				m_ImageCheckedDisabled :
				(m_bHighlighted && m_ImageCheckedHot.GetSafeHandle () != NULL) ?
				m_ImageCheckedHot : m_ImageChecked);

			CImageList& image = 
				(((uiState & ODS_DISABLED) && m_bGrayDisabled && m_ImageDisabled.GetSafeHandle () != NULL) ?
				m_ImageDisabled :
				(m_bHighlighted && m_ImageHot.GetSafeHandle () != NULL) ?
				m_ImageHot : m_Image);

			if (m_bChecked && imageChecked.GetSafeHandle () != NULL && imageChecked.GetImageCount () > 0)
			{
				imageChecked.Draw (pDC, 0, rectImage.TopLeft (), ILD_NORMAL);
			}
			else if (image.GetSafeHandle () != NULL && image.GetImageCount () > 0)
			{
				image.Draw (pDC, 0, rectImage.TopLeft (), ILD_NORMAL);
			}
		}
	}

	pDC->SelectObject (pOldFont);
}
//****************************************************************************
void CBCGPButton::OnDrawText (CDC* pDC, const CRect& rect, const CString& strText,
							 UINT uiDTFlags, UINT /*uiState*/)
{
	ASSERT_VALID (pDC);

	CRect rectText = rect;
	pDC->DrawText (strText, rectText, uiDTFlags);
}
//****************************************************************************
void CBCGPButton::SetImage (HICON hIconCold, BOOL bAutoDestroy, HICON hIconHot, HICON hIconDisabled)
{
	SetImageInternal (hIconCold, bAutoDestroy, hIconHot, FALSE /* Not checked */, hIconDisabled);
}
//****************************************************************************
void CBCGPButton::SetImage (HBITMAP hBitmapCold, BOOL bAutoDestroy, HBITMAP hBitmapHot, BOOL bMap3dColors,
							HBITMAP hBitmapDisabled)
{
	SetImageInternal (hBitmapCold, bAutoDestroy, hBitmapHot, bMap3dColors, FALSE /* Not checked */,
		hBitmapDisabled);
}
//****************************************************************************
void CBCGPButton::SetImage (UINT uiBmpResId, UINT uiBmpHotResId, UINT uiBmpDsblResID)
{
	SetImageInternal (uiBmpResId, uiBmpHotResId, FALSE /* Not checked */, uiBmpDsblResID);
}
//****************************************************************************
void CBCGPButton::SetCheckedImage (HICON hIconCold, BOOL bAutoDestroy, HICON hIconHot, HICON hIconDisabled)
{
	SetImageInternal (hIconCold, bAutoDestroy, hIconHot, TRUE /* Checked */, hIconDisabled);
}
//****************************************************************************
void CBCGPButton::SetCheckedImage (HBITMAP hBitmapCold, BOOL bAutoDestroy, HBITMAP hBitmapHot, BOOL bMap3dColors, HBITMAP hBitmapDisabled)
{
	SetImageInternal (hBitmapCold, bAutoDestroy, hBitmapHot, bMap3dColors, TRUE /* Checked */, hBitmapDisabled);
}
//****************************************************************************
void CBCGPButton::SetCheckedImage (UINT uiBmpResId, UINT uiBmpHotResId, UINT uiBmpDsblResID)
{
	SetImageInternal (uiBmpResId, uiBmpHotResId, TRUE /* Checked */, uiBmpDsblResID);
}
//****************************************************************************
void CBCGPButton::SetImageInternal (HICON hIconCold, BOOL bAutoDestroy, HICON hIconHot, BOOL bChecked, HICON hIconDisabled)
{
	ClearImages (bChecked);

	if (hIconCold == NULL)
	{
		return;
	}

	const int nCount = hIconDisabled == NULL ? 2 : 3;

	for (int i = 0; i < nCount; i++)
	{
		HICON hIcon = (i == 0) ? hIconCold : (i == 1) ? hIconHot : hIconDisabled;
		CImageList& image = bChecked ? ((i == 0) ? 
			m_ImageChecked : (i == 1) ? m_ImageCheckedHot : m_ImageCheckedDisabled) : 
			((i == 0) ? m_Image : (i == 1) ? m_ImageHot : m_ImageDisabled);

		if (hIcon == 0)
		{
			break;
		}

		ICONINFO info;
		::GetIconInfo (hIcon, &info);

		BITMAP bmp;
		::GetObject (info.hbmColor, sizeof (BITMAP), (LPVOID) &bmp);

		m_sizeImage.cx = bmp.bmWidth;
		m_sizeImage.cy = bmp.bmHeight;

		if (i == 0)
		{
			//--------------------------------------------
			// Create disabled image from the "cold" icon:
			//--------------------------------------------
			CDC dcMem;
			dcMem.CreateCompatibleDC (NULL);

			HBITMAP hBmp = (HBITMAP) ::CopyImage (info.hbmColor, IMAGE_BITMAP, 0, 0, 0);
			if (hBmp == NULL)
			{
				ASSERT (FALSE);
			}
			else
			{
				HBITMAP hOldBmp = (HBITMAP) dcMem.SelectObject (hBmp);

				dcMem.FillRect (CRect (0, 0, m_sizeImage.cx, m_sizeImage.cy), 
									&globalData.brBtnFace);

				::DrawIconEx (dcMem.GetSafeHdc (), 0, 0, hIcon, m_sizeImage.cx, m_sizeImage.cy,
								0, NULL, DI_NORMAL);

				dcMem.SelectObject (hOldBmp);

				if (hIconDisabled == NULL)
				{
					CreateDisabledImage (hBmp, bChecked);
				}

				::DeleteObject (hBmp);
			}
		}

		::DeleteObject (info.hbmColor);
		::DeleteObject (info.hbmMask);

		UINT nFlags = ILC_MASK;

		switch (bmp.bmBitsPixel)
		{
		case 4:
		default:
			nFlags |= ILC_COLOR4;
			break;

		case 8:
			nFlags |= ILC_COLOR8;
			break;

		case 16:
			nFlags |= ILC_COLOR16;
			break;

		case 24:
			nFlags |= ILC_COLOR24;
			break;

		case 32:
			nFlags |= ILC_COLOR32;
			break;
		}

		image.Create (bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);
		image.Add (hIcon);
	}

	if (bAutoDestroy)
	{
		if (hIconCold != NULL)
		{
			::DestroyIcon (hIconCold);
		}

		if (hIconHot != NULL)
		{
			::DestroyIcon (hIconHot);
		}
	}
}
//****************************************************************************
void CBCGPButton::SetImageInternal (HBITMAP hBitmapCold, BOOL bAutoDestroy, HBITMAP hBitmapHot, BOOL bMap3dColors, BOOL bChecked,
									HBITMAP hBitmapDisabled)
{
	ClearImages (bChecked);

	if (hBitmapCold == NULL)
	{
		return;
	}

	const int nCount = hBitmapDisabled == NULL ? 2 : 3;

	for (int i = 0; i < nCount; i++)
	{
		HBITMAP hBitmap = (i == 0) ? hBitmapCold : (i == 1) ? hBitmapHot : hBitmapDisabled;
		CImageList& image = bChecked ? ((i == 0) ? 
			m_ImageChecked : (i == 1) ? m_ImageCheckedHot : m_ImageCheckedDisabled) : 
			((i == 0) ? m_Image : (i == 1) ? m_ImageHot : m_ImageDisabled);

		if (hBitmap == NULL)
		{
			break;
		}

		BITMAP bmp;
		::GetObject (hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

		if (i == 0)
		{
			m_sizeImage.cx = bmp.bmWidth;
			m_sizeImage.cy = bmp.bmHeight;
		}
		else
		{
			// Hot and cold bitmaps should have the same size!
			ASSERT (m_sizeImage.cx == bmp.bmWidth);
			ASSERT (m_sizeImage.cy == bmp.bmHeight);
		}

		UINT nFlags = ILC_MASK;

		switch (bmp.bmBitsPixel)
		{
		case 4:
		default:
			nFlags |= ILC_COLOR4;
			break;

		case 8:
			nFlags |= ILC_COLOR8;
			break;

		case 16:
			nFlags |= ILC_COLOR16;
			break;

		case 24:
			nFlags |= ILC_COLOR24;
			break;

		case 32:
			nFlags |= ILC_COLOR32;
			break;
		}

		image.Create (bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);

		HBITMAP hbmpCopy = (HBITMAP) ::CopyImage (hBitmap, IMAGE_BITMAP, 0, 0, 0);
		image.Add (CBitmap::FromHandle (hbmpCopy), 
			bMap3dColors ? RGB (192, 192, 192) : globalData.clrBtnFace);
		::DeleteObject (hbmpCopy);
	}

	if (hBitmapDisabled == NULL)
	{
		CreateDisabledImage (hBitmapCold, bChecked);
	}

	if (bAutoDestroy)
	{
		if (hBitmapHot != NULL)
		{
			::DeleteObject (hBitmapHot);
		}

		if (hBitmapCold != NULL)
		{
			::DeleteObject (hBitmapCold);
		}
	}
}
//****************************************************************************
static HBITMAP ButtonLoadBitmap (UINT uiBmpResId)
{
	if (uiBmpResId == 0)
	{
		return NULL;
	}


	CBCGPToolBarImages images;
	if (!images.Load (uiBmpResId))
	{
		ASSERT (FALSE);
		return NULL;
	}

	return (HBITMAP) ::CopyImage (images.GetImageWell (), IMAGE_BITMAP, 0, 0, 0);
}
//********************************************************************************
void CBCGPButton::SetImageInternal (UINT uiBmpResId, UINT uiBmpHotResId, BOOL bChecked, UINT uiBmpDsblResID)
{
	ClearImages (bChecked);

	if (uiBmpResId == 0)
	{
		return;
	}

	HBITMAP hbmp = ButtonLoadBitmap (uiBmpResId);
	HBITMAP hbmpHot = ButtonLoadBitmap (uiBmpHotResId);
	HBITMAP hbmpDisabled = ButtonLoadBitmap (uiBmpDsblResID);

	SetImageInternal (hbmp, TRUE /* AutoDestroy */, hbmpHot, FALSE, bChecked, hbmpDisabled);
}
//****************************************************************************
void CBCGPButton::SetStdImage (CMenuImages::IMAGES_IDS id, CMenuImages::IMAGE_STATE state,
							   CMenuImages::IMAGES_IDS idDisabled)
{
	CleanUp ();

	m_sizeImage = CMenuImages::Size ();
	m_nStdImageId = id;
	m_nStdImageDisabledId = idDisabled;
	m_StdImageState = state;
}
//****************************************************************************
void CBCGPButton::OnCancelMode() 
{
	CButton::OnCancelMode();
	
	if (m_bCaptured)
	{
		ReleaseCapture ();

		m_bCaptured = FALSE;
		m_bPushed = FALSE;
		m_bClickiedInside = FALSE;
		m_bHighlighted = FALSE;
		m_bHover = FALSE;

		Invalidate ();
		UpdateWindow ();
	}

	if (m_nAutoRepeatTimeDelay >= 0)
	{
		KillTimer (IdAutoCommand);
	}
}
//****************************************************************************
void CBCGPButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_bHover = FALSE;

	if ((nFlags & MK_LBUTTON) || m_nFlatStyle != BUTTONSTYLE_3D ||
		m_bWinXPTheme)
	{
		BOOL bRedraw = FALSE;

		CRect rectClient;
		GetClientRect (rectClient);

		if (rectClient.PtInRect (point))
		{
			m_bHover = TRUE;

			if (!m_bHighlighted)
			{
				m_bHighlighted = TRUE;
				bRedraw = TRUE;
			}

			if ((nFlags & MK_LBUTTON) && !m_bPushed && m_bClickiedInside)
			{
				m_bPushed = TRUE;
				bRedraw = TRUE;
			}

			if (!m_bCaptured)
			{
				SetCapture ();
				m_bCaptured = TRUE;
				bRedraw = TRUE;
			}
		}
		else
		{
			if (nFlags & MK_LBUTTON)
			{
				if (m_bPushed)
				{
					m_bPushed = FALSE;
					bRedraw = TRUE;
				}
			}
			else if (m_bHighlighted)
			{
				m_bHighlighted = FALSE;
				bRedraw = TRUE;
			}

			if (m_bCaptured && (!nFlags & MK_LBUTTON))
			{
				ReleaseCapture ();
				m_bCaptured = FALSE;

				bRedraw = TRUE;
			}
		}

		if (bRedraw)
		{
			Invalidate ();
			UpdateWindow ();
		}
	}
	
	CButton::OnMouseMove(nFlags, point);
}
//****************************************************************************
void CBCGPButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bResponseOnButtonDown)
	{
		CWnd* pParent = GetParent ();
		if (pParent != NULL)
		{
			pParent->SendMessage (	WM_COMMAND,
									MAKEWPARAM (GetDlgCtrlID (), BN_CLICKED),
									(LPARAM) m_hWnd);
		}
	}
	else
	{
		m_bPushed = TRUE;
		m_bClickiedInside = TRUE;
		m_bHighlighted = TRUE;

		if (!m_bCaptured)
		{
			SetCapture ();
			m_bCaptured = TRUE;
		}

		Invalidate ();
		UpdateWindow ();

		if (m_nAutoRepeatTimeDelay > 0)
		{
			SetTimer (IdAutoCommand, m_nAutoRepeatTimeDelay, NULL);
		}
	}

	CButton::OnLButtonDown(nFlags, point);
}
//****************************************************************************
void CBCGPButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	BOOL bClicked = m_bPushed && m_bClickiedInside && m_bHighlighted;

	m_bPushed = FALSE;
	m_bClickiedInside = FALSE;
	m_bHighlighted = FALSE;

	if (bClicked && m_bAutoCheck)
	{
		if (m_bCheckButton)
		{
			m_bChecked = !m_bChecked;
		}
		else if (m_bRadioButton && !m_bChecked)
		{
			m_bChecked = TRUE;
			UncheckRadioButtonsInGroup ();
		}
	}

	RedrawWindow ();
	CButton::OnLButtonUp(nFlags, point);

	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;
	}

	if (m_nAutoRepeatTimeDelay > 0)
	{
		KillTimer (IdAutoCommand);
	}

	if (m_wndToolTip.GetSafeHwnd () != NULL)
	{
		m_wndToolTip.Pop ();

		CString str;
		m_wndToolTip.GetText (str, this);
		m_wndToolTip.UpdateTipText (str, this);
	}
}
//****************************************************************************
CSize CBCGPButton::SizeToContent (BOOL bCalcOnly)
{
	ASSERT (GetSafeHwnd () != NULL);

	CClientDC dc (this);

	CFont* pOldFont = SelectFont (&dc);
	ASSERT(pOldFont != NULL);

	CString strText;
	GetWindowText (strText);
	CSize sizeText = dc.GetTextExtent (strText);

	int cx = sizeText.cx + m_sizeImage.cx + GetImageHorzMargin ();
	if (sizeText.cx > 0)
	{
		cx += GetImageHorzMargin ();
	}

	int cy = max (sizeText.cy, m_sizeImage.cy) + GetVertMargin () * 2;

	if (!bCalcOnly)
	{
		SetWindowPos (NULL, -1, -1, cx, cy,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	dc.SelectObject (pOldFont);

	return CSize (cx, cy);
}
//****************************************************************************
BOOL CBCGPButton::PreTranslateMessage(MSG* pMsg) 
{
	if (m_wndToolTip.GetSafeHwnd () != NULL)
	{
		if (pMsg->message == WM_LBUTTONDOWN ||
			pMsg->message == WM_LBUTTONUP ||
			pMsg->message == WM_MOUSEMOVE)
		{
			m_wndToolTip.RelayEvent(pMsg);
		}
	}

	if (pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == VK_RETURN &&
		CBCGPPopupMenu::GetActiveMenu () == NULL)
	{
		CWnd* pParent = GetParent ();
		if (pParent != NULL)
		{
			pParent->SendMessage (	WM_COMMAND,
									MAKEWPARAM (GetDlgCtrlID (), BN_CLICKED),
									(LPARAM) m_hWnd);
			return TRUE;
		}
	}
	
	if (pMsg->message == WM_KEYDOWN && m_bAutoCheck && GetParent () != NULL)
	{
		switch (pMsg->wParam) 
		{
		case VK_SPACE:
			if (m_bCheckButton) 
			{
				m_bChecked = !m_bChecked;

				RedrawWindow ();

				CWnd* pWndParent = GetParent ();
				ASSERT_VALID (pWndParent);

				::SendMessage (pWndParent->GetSafeHwnd(), WM_COMMAND, 
					MAKELONG (::GetWindowLong(m_hWnd, GWL_ID), BN_CLICKED), 
					(LPARAM) m_hWnd);

				return TRUE	;
			}
			break;

		case VK_UP:
		case VK_LEFT:
			if (CheckNextPrevRadioButton (FALSE))
			{
				return TRUE;
			}
			break;

		case VK_DOWN:
		case VK_RIGHT:
			if (CheckNextPrevRadioButton (TRUE))
			{
				return TRUE;
			}
			break	;
		}
	}

	return CButton::PreTranslateMessage(pMsg);
}
//****************************************************************************
void CBCGPButton::SetTooltip (LPCTSTR lpszToolTipText)
{
	ASSERT (GetSafeHwnd () != NULL);

	if (lpszToolTipText == NULL)
	{
		if (m_wndToolTip.GetSafeHwnd () != NULL)
		{
			m_wndToolTip.Activate (FALSE);
		}
	}
	else
	{
		if (m_wndToolTip.GetSafeHwnd () != NULL)
		{
			m_wndToolTip.UpdateTipText (lpszToolTipText, this);
		}
		else
		{
			m_wndToolTip.Create (NULL, TTS_ALWAYSTIP);

			if (globalData.m_nMaxToolTipWidth != -1)
			{
				m_wndToolTip.SetMaxTipWidth (globalData.m_nMaxToolTipWidth);
			}

			m_wndToolTip.AddTool (this, lpszToolTipText);
		}

		m_wndToolTip.Activate (TRUE);
	}
}
//****************************************************************************
void CBCGPButton::SetMouseCursor (HCURSOR hcursor)
{
	m_hCursor = hcursor;
}
//****************************************************************************
void CBCGPButton::SetMouseCursorHand ()
{
	if (globalData.m_hcurHand == NULL)
	{
		CBCGPLocalResource locaRes;
		globalData.m_hcurHand = AfxGetApp ()->LoadCursor (IDC_BCGBARRES_HAND);
	}

	SetMouseCursor (globalData.m_hcurHand);
}
//*****************************************************************************
BOOL CBCGPButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor != NULL)
	{
		CRect rectClient;
		GetClientRect (rectClient);

		CPoint ptCursor;
		::GetCursorPos (&ptCursor);
		ScreenToClient (&ptCursor);

		if (rectClient.PtInRect (ptCursor))
		{
			::SetCursor (m_hCursor);
			return TRUE;
		}
	}
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}
//*****************************************************************************
void CBCGPButton::OnDrawFocusRect (CDC* pDC, const CRect& rectClient)
{
	ASSERT_VALID (pDC);

	CRect rectFocus = rectClient;
	rectFocus.DeflateRect (2, 2);

	COLORREF clrBckgr = (m_clrFace == (COLORREF)-1) ? globalData.clrBtnFace : m_clrFace;

	pDC->Draw3dRect (rectFocus, clrBckgr, clrBckgr);
	pDC->DrawFocusRect (rectFocus);
}
//******************************************************************************
BOOL CBCGPButton::CreateDisabledImage (HBITMAP hBitmapOrig, BOOL bChecked)
{
	CImageList& imagesDisabled = bChecked ? m_ImageCheckedDisabled : m_ImageDisabled;

	if (imagesDisabled.GetSafeHandle () != NULL)
	{
		imagesDisabled.DeleteImageList ();
	}

	//-------------------------------
	// Get original bitmap attrbutes:
	//-------------------------------
	BITMAP bmp;
	if (::GetObject (hBitmapOrig, sizeof (BITMAP), &bmp) == 0)
	{
		return FALSE;
	}

	int iBitmapWidth = bmp.bmWidth;
	int iBitmapHeight = bmp.bmHeight;

	//------------------------------
	// Create memory destination DC:
	//------------------------------
	CDC memDCDst;
	memDCDst.CreateCompatibleDC (NULL);
	
	HBITMAP hBitmapDisabled = (HBITMAP) ::CopyImage (hBitmapOrig, IMAGE_BITMAP, 0, 0, 0);
	if (hBitmapDisabled == NULL)
	{
		return FALSE;
	}

	HBITMAP hOldBitmapDst = (HBITMAP) memDCDst.SelectObject (hBitmapDisabled);
	if (hOldBitmapDst == NULL)
	{
		::DeleteObject (hBitmapDisabled);
		return FALSE;
	}

	memDCDst.FillRect (CRect (0, 0, iBitmapWidth, iBitmapHeight), 
		&globalData.brBtnFace);

	CBCGPToolBarImages images;
	images.MapTo3dColors ();
	images.SetImageSize (CSize (iBitmapWidth, iBitmapHeight));
	images.AddImage (hBitmapOrig);

	CBCGPDrawState ds;
	images.PrepareDrawImage (ds);
	images.Draw (&memDCDst, 0, 0, 0, FALSE, TRUE);
	images.EndDrawImage (ds);

	memDCDst.SelectObject (hOldBitmapDst);

	UINT nFlags = ILC_MASK;

	switch (bmp.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	imagesDisabled.Create (iBitmapWidth, iBitmapHeight, nFlags, 0, 0);
	imagesDisabled.Add (CBitmap::FromHandle (hBitmapDisabled), globalData.clrBtnFace);

	::DeleteObject (hBitmapDisabled);

	return TRUE;
}
//******************************************************************************
void CBCGPButton::OnEnable(BOOL bEnable) 
{
	// By Yury Goltsman
	if (!bEnable)
	{
		// control disabled
		m_bPushed = FALSE;
		m_bClickiedInside = FALSE;
		m_bHighlighted = FALSE;
		
		if (m_bCaptured)
		{
			ReleaseCapture ();
			m_bCaptured = FALSE;
		}
	}
	
	RedrawWindow ();
	CButton::OnEnable(bEnable);
}
//******************************************************************************
void CBCGPButton::SetFaceColor (COLORREF crFace, BOOL bRedraw)
{
	m_clrFace = crFace;

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//*****************************************************************************
CFont* CBCGPButton::SelectFont (CDC* pDC)
{
	ASSERT_VALID (pDC);

	CFont* pOldFont = m_hFont == NULL ?
		(CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT) :
		pDC->SelectObject (CFont::FromHandle (m_hFont));

	ASSERT(pOldFont != NULL);
	return pOldFont;
}
//*****************************************************************************
afx_msg LRESULT CBCGPButton::OnSetFont (WPARAM wParam, LPARAM lParam)
{
	BOOL bRedraw = (BOOL) LOWORD (lParam);

	m_hFont = (HFONT) wParam;

	if (bRedraw)
	{
		Invalidate ();
		UpdateWindow ();
	}

	return 0;
}
//*****************************************************************************
afx_msg LRESULT CBCGPButton::OnGetFont (WPARAM, LPARAM)
{
	return (LRESULT) m_hFont;
}
//*****************************************************************************
void CBCGPButton::EnableMenuFont (BOOL bOn, BOOL bRedraw)
{
	m_hFont = bOn ? (HFONT) globalData.fontRegular.GetSafeHandle () : NULL;

	if (bRedraw && GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//******************************************************************************
void CBCGPButton::EnableFullTextTooltip (BOOL bOn)
{
	m_bFullTextTooltip = bOn;
	m_bDelayFullTextTooltipSet = bOn;
}
//******************************************************************************
void CBCGPButton::OnSize(UINT nType, int cx, int cy) 
{
	m_bDelayFullTextTooltipSet = m_bFullTextTooltip;
	CButton::OnSize(nType, cx, cy);
}
//******************************************************************************
void CBCGPButton::OnDrawBorder (CDC* pDC, CRect& rectClient, UINT uiState)
{
	DrawBorder (pDC, rectClient, uiState);
}
//****************************************************************************************
void CBCGPButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;
	}
	
	m_bPushed = FALSE;
	m_bClickiedInside = FALSE;
	m_bHighlighted = FALSE;
	m_bHover = FALSE;

	Invalidate ();
	UpdateWindow ();
}
//*****************************************************************************************
void CBCGPButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_bPushed = TRUE;
	m_bClickiedInside = TRUE;
	m_bHighlighted = TRUE;

	Invalidate ();
	UpdateWindow ();

	CButton::OnLButtonDblClk(nFlags, point);
}
//***************************************************************************************
void CBCGPButton::EnableWinXPTheme (BOOL bEnable/* = TRUE*/)
{
	m_bWinXPTheme = bEnable;
}
//***************************************************************************************
LRESULT CBCGPButton::OnGetCheck(WPARAM, LPARAM)
{
	if (m_bCheckButton || m_bRadioButton)
	{
		return m_bChecked ? BST_CHECKED	: BST_UNCHECKED;
	}

	return 0;
}
//***************************************************************************************
LRESULT CBCGPButton::OnSetCheck(WPARAM fCheck, LPARAM)
{
	ASSERT (fCheck != BST_INDETERMINATE);

	if ((m_bCheckButton || m_bRadioButton) && (!m_bChecked) != (fCheck == BST_UNCHECKED)) 
	{
		m_bChecked = fCheck != BST_UNCHECKED;
		
		if (m_bRadioButton)
		{
			UncheckRadioButtonsInGroup ();
		}

		Invalidate();
		UpdateWindow();
	}

	return 0	;
}
//****************************************************************************************
void CBCGPButton::ClearImages (BOOL bChecked)
{
	m_nStdImageId = (CMenuImages::IMAGES_IDS) -1;
	m_nStdImageDisabledId = (CMenuImages::IMAGES_IDS) -1;
	m_sizeImage = CSize (0, 0);

	if (bChecked)
	{
		m_ImageChecked.DeleteImageList ();
		m_ImageCheckedHot.DeleteImageList ();
		m_ImageCheckedDisabled.DeleteImageList ();
	}
	else
	{
		m_Image.DeleteImageList ();
		m_ImageHot.DeleteImageList ();
		m_ImageDisabled.DeleteImageList ();
	}
}
//****************************************************************************************
BOOL CBCGPButton::CheckNextPrevRadioButton (BOOL bNext)
{
	ASSERT_VALID (this);

	if (!m_bRadioButton)
	{
		return FALSE;
	}

	CWnd* pWndParent = GetParent ();
	ASSERT_VALID (pWndParent);

	CBCGPButton* pBtn = NULL;

	for (CWnd* pWnd = pWndParent->GetNextDlgGroupItem (this, !bNext); 
		pWnd != this; 
		pWnd = pWndParent->GetNextDlgGroupItem (pWnd, !bNext))
	{
		if ((pBtn = DYNAMIC_DOWNCAST(CBCGPButton, pWnd)) != NULL &&
			pBtn->m_bRadioButton &&
			(pBtn->GetStyle() & (WS_DISABLED | WS_VISIBLE)) == WS_VISIBLE)
		{
			break;
		}
	}

	if (pBtn != NULL && pBtn != this && !pBtn->m_bChecked)
	{
		pBtn->SetCheck (TRUE);
		pBtn->SetFocus ();

		::SendMessage (pBtn->GetParent()->GetSafeHwnd(), WM_COMMAND,
			MAKELONG (::GetWindowLong(pBtn->m_hWnd, GWL_ID), BN_CLICKED),
			(LPARAM) pBtn->m_hWnd);
		return TRUE;
	}

	return FALSE;
}
//****************************************************************************************
void CBCGPButton::UncheckRadioButtonsInGroup ()
{
	CWnd* pWndParent = GetParent ();
	if (pWndParent == NULL)
	{
		return;
	}

	ASSERT_VALID (pWndParent);

	//--------------------------------------------------------
	// Walk through group and clear radio buttons check state
	//--------------------------------------------------------
	for (CWnd * pCtl = pWndParent->GetNextDlgGroupItem (this); 
		pCtl != this && pCtl != NULL;
		pCtl = pWndParent->GetNextDlgGroupItem (pCtl))
	{
		CBCGPButton* pBtn = DYNAMIC_DOWNCAST (CBCGPButton, pCtl);

		if (pBtn != NULL && pBtn->m_bRadioButton && pBtn->m_bChecked) 
		{
			pBtn->m_bChecked = FALSE;
			pBtn->RedrawWindow ();
		}
	}
}
//***********************************************************************************
void CBCGPButton::SetAutorepeatMode (int nTimeDelay)
{
	ASSERT (nTimeDelay >= 0);
	m_nAutoRepeatTimeDelay = nTimeDelay;
}
//***********************************************************************************
void CBCGPButton::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == IdAutoCommand)
	{
		if (m_bPushed && m_bHighlighted)
		{
			CWnd* pParent = GetParent ();
			if (pParent != NULL)
			{
				pParent->SendMessage (	WM_COMMAND,
										MAKEWPARAM (GetDlgCtrlID (), BN_CLICKED),
										(LPARAM) m_hWnd);
			}
		}
	}

	CButton::OnTimer(nIDEvent);
}
//****************************************************************************************
void CBCGPButton::DrawBorder (CDC* pDC, CRect& rectClient, UINT uiState)
{
	ASSERT_VALID (pDC);

	BOOL bBorderIsReady = FALSE;

	//----------------
	// Draw 3d border:
	//----------------
	if (m_nFlatStyle != BUTTONSTYLE_NOBORDERS)
	{
		if (m_bWinXPTheme &&
			CBCGPVisualManager::GetInstance ()->DrawPushButtonWinXP (pDC, rectClient, this, uiState))
		{
			bBorderIsReady = TRUE;
		}

		if (m_bPushed && m_bHighlighted || (uiState & ODS_SELECTED) || m_bChecked)
		{
			if (!bBorderIsReady)
			{
				pDC->Draw3dRect (rectClient,
							globalData.clrBtnDkShadow, globalData.clrBtnHilite);

				rectClient.DeflateRect (1, 1);

				if (m_nFlatStyle != BUTTONSTYLE_FLAT)
				{
					pDC->Draw3dRect (rectClient,
								globalData.clrBtnShadow, globalData.clrBtnLight);
				}

				rectClient.DeflateRect (1, 1);
			}
			else
			{
				rectClient.DeflateRect (2, 2);
			}

			rectClient.left += m_sizePushOffset.cx;
			rectClient.top += m_sizePushOffset.cy;
		}
		else if (!bBorderIsReady && (m_nFlatStyle != BUTTONSTYLE_FLAT || m_bHighlighted))
		{
			pDC->Draw3dRect (rectClient,
						globalData.clrBtnHilite, 
						globalData.clrBtnDkShadow);
			rectClient.DeflateRect (1, 1);

			if (m_nFlatStyle == BUTTONSTYLE_3D ||
				(m_nFlatStyle == BUTTONSTYLE_SEMIFLAT && m_bHighlighted))
			{
				pDC->Draw3dRect (rectClient,
							globalData.clrBtnLight, globalData.clrBtnShadow);
			}

			rectClient.DeflateRect (1, 1);
		}
		else
		{
			rectClient.DeflateRect (2, 2);
		}
	}
	else
	{
		rectClient.DeflateRect (2, 2);
	}
}
//*********************************************************************************
LRESULT CBCGPButton::OnSetImage (WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IMAGE_BITMAP:
		SetImage ((HBITMAP) lParam, FALSE);
		break;
		
	case IMAGE_ICON:
		SetImage ((HICON) lParam, FALSE);
		break;
		
	default:
		TRACE1 ("Error: unknown image type '%u'\n", (unsigned) wParam);
	}

	return 0;
}
//*********************************************************************************
LRESULT CBCGPButton::OnGetImage (WPARAM wParam, LPARAM)
{
	switch (wParam)
	{
	case IMAGE_BITMAP:
		{
			IMAGEINFO imgInfo;
			memset (&imgInfo, 0, sizeof (IMAGEINFO));

			if (!m_Image.GetImageInfo (0, &imgInfo))
			{
				return NULL;
			}

			return (LRESULT) imgInfo.hbmImage;
		}
		break;
		
	case IMAGE_ICON:
		return (LRESULT) m_Image.ExtractIcon (0);
		
	default:
		TRACE1 ("Error: unknown image type '%u'\n", (unsigned) wParam);
	}

	return 0;
}
//************************************************************************************
int CBCGPButton::GetImageHorzMargin () const
{ 
	return nImageHorzMargin; 
}
//************************************************************************************
int CBCGPButton::GetVertMargin () const
{ 
	return nVertMargin; 
}
