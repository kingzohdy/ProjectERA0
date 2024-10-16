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

// BCGPToolBarImages.cpp: implementation of the CBCGPToolBarImages class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "BCGGlobals.h"
#include "BCGPToolBarImages.h"
#include "BCGPToolBar.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPVisualManager.h"
#include "BCGPDrawManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static BOOL WriteDIB( LPCTSTR szFile, HANDLE hDIB);
static HANDLE DDBToDIB (HBITMAP bitmap, DWORD dwCompression);

BOOL CBCGPToolBarImages::m_bDisableTrueColorAlpha = TRUE;
HINSTANCE CBCGPToolBarImages::m_hinstMSIMGDLL = NULL;
ALPHABLEND CBCGPToolBarImages::m_pfAlphaBlend = NULL;
BYTE CBCGPToolBarImages::m_nDisabledImageAlpha = 127;

// globals for fast drawing (shared globals)
static HDC hDCGlyphs = NULL;
static HDC hDCMono = NULL;

/*
	DIBs use RGBQUAD format:
		0xbb 0xgg 0xrr 0x00

	Reasonably efficient code to convert a COLORREF into an
	RGBQUAD is byte-order-dependent, so we need different
	code depending on the byte order we're targeting.
*/

#define RGB_TO_RGBQUAD(r,g,b)   (RGB(b,g,r))
#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))
#define RGBQUAD_TO_CLR(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

// Raster Ops
#define ROP_DSPDxax  0x00E20746L
#define ROP_PSDPxax  0x00B8074AL

/////////////////////////////////////////////////////////////////////////////
// Init / Term

void CBCGPToolBarImages::CleanUp ()
{
	if (hDCMono != NULL)
	{
		::DeleteDC (hDCMono);
		hDCMono = NULL;
	}

	if (hDCGlyphs != NULL)
	{
		::DeleteDC (hDCGlyphs);
		hDCGlyphs = NULL;
	}

	if (m_hinstMSIMGDLL == NULL)
	{
		::FreeLibrary (m_hinstMSIMGDLL);
		m_hinstMSIMGDLL = NULL;
	}

	m_pfAlphaBlend = NULL;
}

// a special struct that will cleanup automatically
struct _AFX_TOOLBAR_TERM
{
	~_AFX_TOOLBAR_TERM()
	{
		CBCGPToolBarImages::CleanUp ();
	}
};

static const _AFX_TOOLBAR_TERM toolbarTerm;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPToolBarImages::CBCGPToolBarImages()
{
	m_bModified = FALSE;
	m_bReadOnly = FALSE;
	m_bIsTemporary = FALSE;
	m_iCount = 0;

	m_hbmImageWell = NULL;
	m_hbmImageLight = NULL;

	m_bUserImagesList = FALSE;

	// initialize the toolbar drawing engine
	static BOOL bInitialized;
	if (!bInitialized)
	{
		hDCGlyphs = CreateCompatibleDC(NULL);

		// Mono DC and Bitmap for disabled image
		hDCMono = ::CreateCompatibleDC(NULL);

		if (hDCGlyphs == NULL || hDCMono == NULL)
			AfxThrowResourceException();

		if (!globalData.bIsWindows9x && !globalData.bIsWindowsNT4)
		{
			if ((m_hinstMSIMGDLL = LoadLibrary (_T("msimg32.dll"))) != NULL)
			{
				m_pfAlphaBlend = 
					(ALPHABLEND)::GetProcAddress (m_hinstMSIMGDLL, "AlphaBlend");
			}
		}

		bInitialized = TRUE;
	}

	m_clrTransparent = (COLORREF) -1;

	// UISG standard sizes
	m_sizeImage = CSize (16, 15);
	m_sizeImageDest = CSize (0, 0);
	m_bStretch = FALSE;
	m_pBmpOriginal = NULL;

	m_bFadeInactive = FALSE;
	m_nBitsPerPixel = 0;

	OnSysColorChange ();
}
//*******************************************************************************
CBCGPToolBarImages::~CBCGPToolBarImages()
{
	ASSERT (m_dcMem.GetSafeHdc () == NULL);
	ASSERT (m_bmpMem.GetSafeHandle () == NULL);
	ASSERT (m_pBmpOriginal == NULL);

	if (!m_bIsTemporary)
	{
		AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
		AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
	}
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Load (UINT uiResID, HINSTANCE hinstRes, BOOL bAdd)
{
	if (m_bIsTemporary)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	LPCTSTR lpszResourceName = MAKEINTRESOURCE (uiResID);
	ASSERT(lpszResourceName != NULL);

	if (!bAdd)
	{
		AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);     // get rid of old one
		m_lstOrigResIds.RemoveAll ();
		m_lstOrigResInstances.RemoveAll ();
		m_mapOrigResOffsets.RemoveAll ();
	}
	else if (m_lstOrigResIds.Find (uiResID) != NULL)	// Already loaded, do nothing
	{
		return TRUE;
	}

	if (hinstRes == NULL)
	{
		hinstRes = AfxFindResourceHandle (lpszResourceName, RT_BITMAP);
	}

	HBITMAP hbmp = (HBITMAP) ::LoadImage (
		hinstRes,
		lpszResourceName,
		IMAGE_BITMAP,
		0, 0,
		LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

	if (hbmp == NULL)
	{
		TRACE(_T("Can't load bitmap: %x. GetLastError() = %x\n"), 
			uiResID,
			GetLastError ());
		return FALSE;
	}

	BITMAP bmp;
	if (::GetObject (hbmp, sizeof (BITMAP), &bmp) == 0)
	{
		ASSERT (FALSE);
		::DeleteObject (hbmp);
		return FALSE;
	}

	if (bmp.bmBitsPixel >= 32)
	{
		PreMultiplyAlpha (hbmp);
	}

	m_nBitsPerPixel = max (m_nBitsPerPixel, bmp.bmBitsPixel);

	if (bAdd)
	{
		m_mapOrigResOffsets.SetAt (uiResID, m_iCount);

		AddImage (hbmp);
		m_lstOrigResIds.AddTail (uiResID);
		m_lstOrigResInstances.AddTail (hinstRes);

		::DeleteObject (hbmp);
	}
	else
	{
		m_hbmImageWell = hbmp;
	}

	if (m_nBitsPerPixel > 8 && m_nBitsPerPixel < 32)
	{
		//------------------------------------------------
		// LR_LOADMAP3DCOLORS don't support > 8bpp images,
		// we should convert it now:
		//------------------------------------------------
		MapTo3dColors (FALSE);
	}

	UpdateCount ();
	UpdateLightImage ();

	return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Load (LPCTSTR lpszBmpFileName)
{
	if (m_bIsTemporary)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	ASSERT (lpszBmpFileName != NULL);

	AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);     // get rid of old one

	CString strPath = lpszBmpFileName;

	//-----------------------------------------------------------------------
	// If the image path is not defined, try to open it in the EXE directory:
	//-----------------------------------------------------------------------
	if (strPath.Find (_T("\\")) == -1 &&
		strPath.Find (_T("/")) == -1 &&
		strPath.Find (_T(":")) == -1)
	{
		TCHAR lpszFilePath [_MAX_PATH];
		if (::GetModuleFileName (NULL, lpszFilePath, _MAX_PATH) > 0)
		{
			TCHAR path_buffer[_MAX_PATH];   
			TCHAR drive[_MAX_DRIVE];   
			TCHAR dir[_MAX_DIR];
			TCHAR fname[_MAX_FNAME];   
			TCHAR ext[_MAX_EXT];

			_tsplitpath (lpszFilePath, drive, dir, NULL, NULL);
			_tsplitpath (lpszBmpFileName, NULL, NULL, fname, ext);

			_tmakepath (path_buffer, drive, dir, fname, ext);
			strPath = path_buffer;
		}
	}

	//--------------------------------
	// Load images from the disk file:
	//--------------------------------
	m_hbmImageWell = (HBITMAP) ::LoadImage (
		AfxGetInstanceHandle (),
		strPath,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

	if (m_hbmImageWell == NULL)
	{
		TRACE(_T("Can't load bitmap: %s. GetLastError() = %x\r\n"), 
			lpszBmpFileName,
			GetLastError ());
		return FALSE;
	}

	BITMAP bmp;
	if (::GetObject (m_hbmImageWell, sizeof (BITMAP), &bmp) == 0)
	{
		ASSERT (FALSE);
		::DeleteObject (m_hbmImageWell);
		m_hbmImageWell = NULL;
		return FALSE;
	}

	m_bUserImagesList = TRUE;
	m_strUDLPath = strPath;

	if (::GetFileAttributes (strPath) & FILE_ATTRIBUTE_READONLY)
	{
		m_bReadOnly = TRUE;
	}

	m_nBitsPerPixel = bmp.bmBitsPixel;
	if (m_nBitsPerPixel > 8 && m_nBitsPerPixel < 32)
	{
		//------------------------------------------------
		// LR_LOADMAP3DCOLORS don't support > 8bpp images,
		// we should convert it now:
		//------------------------------------------------
		MapTo3dColors (FALSE);
	}

	UpdateCount ();
	UpdateLightImage ();
	return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::PrepareDrawImage (CBCGPDrawState& ds,
										  CSize sizeImageDest,
										  BOOL bFadeInactive)
{
	if (m_hbmImageWell == NULL)
	{
		return FALSE;
	}

	m_bStretch = FALSE;

	if (m_hbmImageLight == NULL ||
		m_nBitsPerPixel > 4 || m_nBitsPerPixel == 0)
	{
		// Down't fade 256+ or unknown bitmaps
		bFadeInactive = FALSE;
	}

	m_bFadeInactive = bFadeInactive;

	ASSERT(m_hbmImageWell != NULL);
	ASSERT (m_dcMem.GetSafeHdc () == NULL);
	ASSERT (m_bmpMem.GetSafeHandle () == NULL);
	ASSERT (m_pBmpOriginal == NULL);

	// We need to kick-start the bitmap selection process.
	ds.hbmOldGlyphs = (HBITMAP)SelectObject (hDCGlyphs, 
		bFadeInactive ? m_hbmImageLight : m_hbmImageWell);

	ds.hbmMono = CreateBitmap (m_sizeImage.cx + 2, m_sizeImage.cy + 2,
					1, 1, NULL);
	ds.hbmMonoOld = (HBITMAP)SelectObject(hDCMono, ds.hbmMono);
	if (ds.hbmOldGlyphs == NULL || ds.hbmMono == NULL || ds.hbmMonoOld == NULL)
	{
		TRACE0("Error: can't draw toolbar.\r\n");
		AfxDeleteObject((HGDIOBJ*)&ds.hbmMono);
		return FALSE;
	}

	if (sizeImageDest.cx <= 0 || sizeImageDest.cy <= 0)
	{
		m_sizeImageDest = m_sizeImage;
	}
	else
	{
		m_sizeImageDest = sizeImageDest;
	}

	COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
		(COLORREF) -1 : m_clrTransparent;

	if (m_sizeImageDest != m_sizeImage || clrTransparent != (COLORREF) -1)
	{
		CWindowDC dc (NULL);

		m_bStretch = (m_sizeImageDest != m_sizeImage);

		m_dcMem.CreateCompatibleDC (NULL);	// Assume display!
		m_bmpMem.CreateCompatibleBitmap (&dc, m_sizeImage.cx + 2, m_sizeImage.cy + 2);

		m_pBmpOriginal = m_dcMem.SelectObject (&m_bmpMem);
		ASSERT (m_pBmpOriginal != NULL);
	}

	return TRUE;
}
//*******************************************************************************
void CBCGPToolBarImages::EndDrawImage (CBCGPDrawState& ds)
{
	SelectObject(hDCMono, ds.hbmMonoOld);
	AfxDeleteObject((HGDIOBJ*)&ds.hbmMono);
	SelectObject(hDCGlyphs, ds.hbmOldGlyphs);

	m_sizeImageDest = CSize (0, 0);

	COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
		(COLORREF) -1 : m_clrTransparent;

	if (m_bStretch || clrTransparent != (COLORREF) -1)
	{
		ASSERT (m_pBmpOriginal != NULL);

		m_dcMem.SelectObject (m_pBmpOriginal);
		m_pBmpOriginal = NULL;

		::DeleteObject (m_bmpMem.Detach ());
		::DeleteDC (m_dcMem.Detach ());
	}

	m_bFadeInactive = FALSE;
}
//*******************************************************************************
void CBCGPToolBarImages::CreateMask(int iImage, BOOL bHilite, BOOL bHiliteShadow)
{
	// initalize whole area with 0's
	PatBlt(hDCMono, 0, 0, m_sizeImage.cx + 2, m_sizeImage.cy + 2, WHITENESS);

	COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
		(COLORREF) -1 : m_clrTransparent;

	// create mask based on color bitmap
	// convert this to 1's
	SetBkColor (hDCGlyphs, 
		clrTransparent != -1 ? clrTransparent : globalData.clrBtnFace);

	::BitBlt(hDCMono, 0, 0, m_sizeImage.cx, m_sizeImage.cy,
		hDCGlyphs, iImage * m_sizeImage.cx, 0, SRCCOPY);

	if (bHilite)
	{
		// convert this to 1's
		SetBkColor(hDCGlyphs, globalData.clrBtnHilite);

		// OR in the new 1's
		::BitBlt(hDCMono, 0, 0, m_sizeImage.cx, m_sizeImage.cy,
			hDCGlyphs, iImage * m_sizeImage.cx, 0, SRCPAINT);

		if (bHiliteShadow)
		{
			::BitBlt(hDCMono, 1, 1, m_sizeImage.cx + 1, m_sizeImage.cy + 1,
				hDCMono, 0, 0, SRCAND);
		}
	}
}
//********************************************************************************
BOOL CBCGPToolBarImages::Draw (CDC* pDCDest, 
				int xDest, int yDest,
				int iImage,
				BOOL bHilite,
				BOOL bDisabled,
				BOOL bIndeterminate,
				BOOL bShadow,
				BOOL bInactive)
{
	if (iImage < 0 || iImage >= m_iCount)
	{
		return FALSE;
	}

	if (bShadow && globalData.m_nBitsPerPixel <= 8)
	{
		return TRUE;
	}

	if (m_bStretch)
	{
		bHilite = FALSE;
		bIndeterminate = FALSE;
	}

	HBITMAP hBmpOriginal = NULL;
	if (!bInactive && m_bFadeInactive)
	{
		hBmpOriginal = (HBITMAP) SelectObject (hDCGlyphs, m_hbmImageWell);
	}

	COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
		(COLORREF) -1 : m_clrTransparent;

	BOOL bIsTransparent =	(clrTransparent != (COLORREF) -1);

	CDC* pDC = m_bStretch || bIsTransparent ? &m_dcMem : pDCDest;
	ASSERT_VALID(pDC);

	int x = m_bStretch || bIsTransparent ? 0 : xDest;
	int y = m_bStretch || bIsTransparent ? 0 : yDest;

	if (m_bStretch || bIsTransparent)
	{
		CRect rectImage (CPoint (0, 0), m_sizeImage);

		if (bIsTransparent && clrTransparent != globalData.clrBtnFace)
		{
			CBrush brBackgr (clrTransparent);
			pDC->FillRect (rectImage, &brBackgr);
		}
		else
		{
			pDC->FillRect (rectImage, &globalData.brBtnFace);
		}
	}

	BOOL bDisabledTrueColor = FALSE;

	if (bDisabled && m_nBitsPerPixel >= 24)
	{
		bDisabled = FALSE;
		bDisabledTrueColor = TRUE;
	}

	BOOL bShadowTrueColor = FALSE;

	if (bShadow && m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL)
	{
		bShadow = FALSE;
		bShadowTrueColor = TRUE;
	}

	if (!bHilite && !bDisabled && !bShadow)
	{
		BOOL bIsReady = FALSE;

		if (m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL)
		{
			BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 
				255, 1 /*AC_SRC_ALPHA*/ };

			if (bDisabledTrueColor)
			{
				pixelblend.SourceConstantAlpha = m_nDisabledImageAlpha;
			}

			ASSERT (m_pfAlphaBlend != NULL);

			bIsReady = (*m_pfAlphaBlend) (pDC->m_hDC, x, y,
				m_sizeImage.cx, m_sizeImage.cy, 
				hDCGlyphs, iImage * m_sizeImage.cx, 0, 
				m_sizeImage.cx, m_sizeImage.cy, pixelblend);
		}
		
		if (!bIsReady)
		{
			//----------------------
			// normal image version:
			//----------------------
			::BitBlt(pDC->m_hDC, x, y,
				m_sizeImage.cx, m_sizeImage.cy,
				hDCGlyphs, iImage * m_sizeImage.cx, 0, SRCCOPY);

			if (bDisabledTrueColor)
			{
				CBCGPDrawManager dm (*pDC);
				dm.GrayRect (CRect (0, 0, m_sizeImage.cx + 2, m_sizeImage.cy + 2), -1, clrTransparent);
			}
		}
	}
	else
	{
		if (bDisabled || bIndeterminate || bShadow)
		{
			// disabled or indeterminate version
			CreateMask(iImage, TRUE, FALSE);

			pDC->SetTextColor(bShadow ? m_clrImageShadow : 0L); // 0's in mono -> 0 (for ROP)
			pDC->SetBkColor((COLORREF)0x00FFFFFFL); // 1's in mono -> 1

			if (bDisabled && CBCGPVisualManager::GetInstance ()->IsEmbossDisabledImage ())
			{
				// disabled - draw the hilighted shadow
				HGDIOBJ hbrOld = pDC->SelectObject (globalData.hbrBtnHilite);
				if (hbrOld != NULL)
				{
					// draw hilight color where we have 0's in the mask
					::BitBlt(pDC->m_hDC, x + 1, y + 1,
						m_sizeImage.cx + 2, m_sizeImage.cy + 2,
						hDCMono, 0, 0, ROP_PSDPxax);

					pDC->SelectObject(hbrOld);
				}
			}

			//BLOCK: always draw the shadow
			{
				HGDIOBJ hbrOld = pDC->SelectObject(globalData.hbrBtnShadow);
				if (hbrOld != NULL)
				{
					// draw the shadow color where we have 0's in the mask
					::BitBlt(pDC->m_hDC, 
						x, y,
						m_sizeImage.cx + 2, m_sizeImage.cy + 2,
						hDCMono, 0, 0, ROP_PSDPxax);

					pDC->SelectObject(hbrOld);
				}
			}
		}

		// if it is checked do the dither brush avoiding the glyph
		if (bHilite || bIndeterminate)
		{
			CBrush* pBrOld = pDC->SelectObject(&globalData.brLight);
			if (pBrOld != NULL)
			{
				CreateMask(iImage, !bIndeterminate, bDisabled);

				pDC->SetTextColor(0L);              // 0 -> 0
				pDC->SetBkColor((COLORREF)0x00FFFFFFL); // 1 -> 1

				// only draw the dither brush where the mask is 1's
				::BitBlt(pDC->m_hDC, x, y, 
					m_sizeImage.cx, m_sizeImage.cy,
					hDCMono, 0, 0, ROP_DSPDxax);

				pDC->SelectObject(pBrOld);
			}
		}
	}

	if (m_bStretch)
	{
		TransparentBlt (pDCDest->GetSafeHdc (), xDest, yDest, 
						m_sizeImage.cx, m_sizeImage.cy, 
						pDC, 0, 0, 
						bIsTransparent ? clrTransparent : globalData.clrBtnFace,
						m_sizeImageDest.cx, m_sizeImageDest.cy);
	}
	else if (bIsTransparent)
	{
		TransparentBlt (pDCDest->GetSafeHdc (), xDest, yDest, 
						m_sizeImage.cx, m_sizeImage.cy, 
						pDC, 0, 0, clrTransparent);
	}

	if (hBmpOriginal != NULL)
	{
		SelectObject (hDCGlyphs, hBmpOriginal);
	}

	return TRUE;
}
//********************************************************************************
void CBCGPToolBarImages::FillDitheredRect (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID(pDC);
	pDC->FillRect (&rect, &globalData.brLight);
}
//********************************************************************************
void CBCGPToolBarImages::OnSysColorChange()
{
	if (m_bIsTemporary)
	{
		return;
	}

	int iOldCount = m_iCount;

	// re-color bitmap for toolbar
	if (m_hbmImageWell != NULL)
	{
		if (m_bUserImagesList)
		{
			Load (m_strUDLPath);
		}
		else
		{
			// Image was buit from the resources...
			if (m_lstOrigResIds.IsEmpty ())
			{
				return;
			}

			ASSERT (m_lstOrigResInstances.GetCount () == m_lstOrigResIds.GetCount ());

			AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);     // get rid of old one

			POSITION posInst = m_lstOrigResInstances.GetHeadPosition ();

			for (POSITION pos = m_lstOrigResIds.GetHeadPosition (); 
				pos != NULL;)
			{
				ASSERT (posInst != NULL);

				UINT uiResId = m_lstOrigResIds.GetNext (pos);
				ASSERT (uiResId > 0);

				HINSTANCE hInst = m_lstOrigResInstances.GetNext (posInst);
				ASSERT (hInst != NULL);

				HBITMAP hbmp = (HBITMAP) ::LoadImage (
					hInst,
					MAKEINTRESOURCE (uiResId),
					IMAGE_BITMAP,
					0, 0,
					LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

				AddImage (hbmp);
				::DeleteObject (hbmp);
			}
		}
	}

	UpdateCount ();
	ASSERT (iOldCount == m_iCount);

	if (m_nBitsPerPixel > 8 && m_nBitsPerPixel < 32)
	{
		//------------------------------------------------
		// LR_LOADMAP3DCOLORS don't support > 8bpp images,
		// we should convert it now:
		//------------------------------------------------
		MapTo3dColors (FALSE);
	}

	UpdateLightImage ();

	m_clrImageShadow = globalData.clrBtnShadow;
}
//********************************************************************************
void CBCGPToolBarImages::UpdateCount ()
{
	if (m_hbmImageWell == NULL)
	{
		m_iCount = 0;
		return;
	}

	BITMAP bmp;
	if (::GetObject (m_hbmImageWell, sizeof (BITMAP), &bmp) == 0)
	{
		m_iCount = 0;
		return;
	}

	int iWidth = bmp.bmWidth;
	m_iCount = iWidth / m_sizeImage.cx;
}

//////////////////////////////////////////
// Image editing methods:
//////////////////////////////////////////

int CBCGPToolBarImages::AddImage (HBITMAP hbmp)
{
	if (m_bIsTemporary)
	{
		ASSERT (FALSE);
		return -1;
	}

	//-------------------------------------------------------
	// Create memory source DC and select an original bitmap:
	//-------------------------------------------------------
	CDC memDCSrc;
	memDCSrc.CreateCompatibleDC (NULL);

	HBITMAP hOldBitmapSrc = NULL;

	int iBitmapWidth;
	int iBitmapHeight;
	int iNewBitmapWidth;

	BITMAP bmp;
	if (::GetObject (hbmp, sizeof (BITMAP), &bmp) == 0)
	{
		return -1;
	}

	iNewBitmapWidth = bmp.bmWidth;
	iBitmapHeight = bmp.bmHeight;

	if (m_hbmImageWell != NULL)
	{
		//-------------------------------
		// Get original bitmap attrbutes:
		//-------------------------------
		if (::GetObject (m_hbmImageWell, sizeof (BITMAP), &bmp) == 0)
		{
			return -1;
		}

		hOldBitmapSrc = (HBITMAP) memDCSrc.SelectObject (m_hbmImageWell);
		if (hOldBitmapSrc == NULL)
		{
			return -1;
		}

		iBitmapWidth = bmp.bmWidth;
		iBitmapHeight = bmp.bmHeight;
	}
	else
	{
		iBitmapWidth = 0;

		hOldBitmapSrc = (HBITMAP) memDCSrc.SelectObject (hbmp);
		if (hOldBitmapSrc == NULL)
		{
			return -1;
		}
	}

	//----------------------------------------------------------
	// Create a new bitmap compatibel with the source memory DC
	// (original bitmap SHOULD BE ALREADY SELECTED!):
	//----------------------------------------------------------
	HBITMAP hNewBitmap = (HBITMAP) ::CreateCompatibleBitmap (memDCSrc,
									iBitmapWidth + iNewBitmapWidth,
									iBitmapHeight);
	if (hNewBitmap == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		return -1;
	}

	//------------------------------------------------------
	// Create memory destination DC and select a new bitmap:
	//------------------------------------------------------
	CDC memDCDst;
	memDCDst.CreateCompatibleDC (&memDCSrc);
	
	HBITMAP hOldBitmapDst = (HBITMAP) memDCDst.SelectObject (hNewBitmap);
	if (hOldBitmapDst == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		::DeleteObject (hNewBitmap);
		return -1;
	}

	if (m_hbmImageWell != NULL)
	{
		//-----------------------------
		// Copy original bitmap to new:
		//-----------------------------
		memDCDst.BitBlt (0, 0, iBitmapWidth, iBitmapHeight,
					&memDCSrc, 0, 0, SRCCOPY);
	}
		
	//--------------------------------
	// Select a new image and copy it:
	//--------------------------------
	if (memDCSrc.SelectObject (hbmp) == NULL)
	{
		memDCDst.SelectObject (hOldBitmapDst);
		memDCSrc.SelectObject (hOldBitmapSrc);

		::DeleteObject (hNewBitmap);
		return -1;
	}

	memDCDst.BitBlt (iBitmapWidth, 0, iNewBitmapWidth, iBitmapHeight,
				&memDCSrc, 0, 0, SRCCOPY);

	memDCDst.SelectObject (hOldBitmapDst);
	memDCSrc.SelectObject (hOldBitmapSrc);

	if (m_hbmImageWell != NULL)
	{
		::DeleteObject (m_hbmImageWell);
	}

	m_hbmImageWell = hNewBitmap;
	m_bModified = TRUE;

	UpdateCount ();
	UpdateLightImage ();

	return m_iCount - 1;
}
//********************************************************************************
int CBCGPToolBarImages::AddIcon (HICON hIcon)
{
	CWindowDC dc (NULL);

	CDC dcMem;
	dcMem.CreateCompatibleDC (NULL);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap (&dc, m_sizeImage.cx, m_sizeImage.cy);

	CBitmap* pBmpOriginal = dcMem.SelectObject (&bmpMem);

	dcMem.FillRect (CRect (0, 0, m_sizeImage.cx, m_sizeImage.cy), &globalData.brBtnFace);

	if (hIcon != NULL)
	{
		dcMem.DrawState (CPoint (0, 0), m_sizeImage, hIcon, DSS_NORMAL, (CBrush*) NULL);
	}

	dcMem.SelectObject (pBmpOriginal);

	return AddImage (bmpMem);
}
//*******************************************************************************
BOOL CBCGPToolBarImages::UpdateImage (int iImage, HBITMAP hbmp)
{
	if (m_bIsTemporary)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (!m_bUserImagesList)	// Only user images can be edited!
	{
		return FALSE;
	}

	CWindowDC	dc (NULL);
	CBitmap 	bitmap;
	CDC 		memDCSrc;
	CDC 		memDCDst;

	memDCSrc.CreateCompatibleDC (&dc);
	memDCDst.CreateCompatibleDC (&dc);
	
	HBITMAP hOldBitmapDst = (HBITMAP) memDCDst.SelectObject (m_hbmImageWell);
	HBITMAP hOldBitmapSrc = (HBITMAP) memDCSrc.SelectObject (hbmp);

	memDCDst.BitBlt (m_sizeImage.cx * iImage, 0, m_sizeImage.cx, m_sizeImage.cy,
				&memDCSrc, 0, 0, SRCCOPY);

	memDCDst.SelectObject (hOldBitmapDst);
	memDCSrc.SelectObject (hOldBitmapSrc);

	m_bModified = TRUE;

	UpdateLightImage ();
	return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::DeleteImage (int iImage)
{
	if (m_bIsTemporary)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (!m_bUserImagesList)	// Only user images can be edited!
	{
		return FALSE;
	}

	if (iImage < 0 || iImage >= GetCount ())	// Wrong index
	{
		return FALSE;
	}

	//-------------------------------
	// Get original bitmap attrbutes:
	//-------------------------------
	BITMAP bmp;
	if (::GetObject (m_hbmImageWell, sizeof (BITMAP), &bmp) == 0)
	{
		return FALSE;
	}

	//-------------------------------------------------------
	// Create memory source DC and select an original bitmap:
	//-------------------------------------------------------
	CDC memDCSrc;
	memDCSrc.CreateCompatibleDC (NULL);

	HBITMAP hOldBitmapSrc = (HBITMAP) memDCSrc.SelectObject (m_hbmImageWell);
	if (hOldBitmapSrc == NULL)
	{
		return FALSE;
	}

	//----------------------------------------------------------
	// Create a new bitmap compatibel with the source memory DC
	// (original bitmap SHOULD BE ALREADY SELECTED!):
	//----------------------------------------------------------
	HBITMAP hNewBitmap = (HBITMAP) ::CreateCompatibleBitmap (memDCSrc,
									bmp.bmWidth - m_sizeImage.cx,
									bmp.bmHeight);
	if (hNewBitmap == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		return FALSE;
	}

	//------------------------------------------------------
	// Create memory destination DC and select a new bitmap:
	//------------------------------------------------------
	CDC memDCDst;
	memDCDst.CreateCompatibleDC (&memDCSrc);
	
	HBITMAP hOldBitmapDst = (HBITMAP) memDCDst.SelectObject (hNewBitmap);
	if (hOldBitmapDst == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		::DeleteObject (hNewBitmap);
		return FALSE;
	}

	//-----------------------------
	// Copy original bitmap to new:
	//-----------------------------

	if (iImage != 0)
	{
		memDCDst.BitBlt (0, 0, 
					m_sizeImage.cx * iImage, bmp.bmHeight,
					&memDCSrc, 0, 0, SRCCOPY);
	}
	
	if (iImage != m_iCount - 1)
	{
		memDCDst.BitBlt (m_sizeImage.cx * iImage, 0, 
					bmp.bmWidth - (m_iCount - iImage - 1) * m_sizeImage.cx, bmp.bmHeight,
					&memDCSrc, 
					m_sizeImage.cx * (iImage + 1), 0, SRCCOPY);
	}

	memDCDst.SelectObject (hOldBitmapDst);
	memDCSrc.SelectObject (hOldBitmapSrc);

	::DeleteObject (m_hbmImageWell);

	m_hbmImageWell = hNewBitmap;
	m_bModified = TRUE;

	UpdateCount ();
	UpdateLightImage ();

	return TRUE;
}
//*******************************************************************************
COLORREF CBCGPToolBarImages::MapToSysColor (COLORREF color, BOOL bUseRGBQUAD)
{
	struct COLORMAP
	{
		// use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
		DWORD rgbqFrom;
		int iSysColorTo;
	};
	static const COLORMAP sysColorMap[] =
	{
		// mapping from color in DIB to system color
		{ RGB_TO_RGBQUAD(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
		{ RGB_TO_RGBQUAD(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark grey
		{ RGB_TO_RGBQUAD(0xC0, 0xC0, 0xC0),  COLOR_BTNFACE },       // bright grey
		{ RGB_TO_RGBQUAD(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
	};
	const int nMaps = 4;

	// look for matching RGBQUAD color in original
	for (int i = 0; i < nMaps; i++)
	{
		if (color == sysColorMap[i].rgbqFrom)
		{
			return bUseRGBQUAD ? 
				CLR_TO_RGBQUAD (globalData.GetColor (sysColorMap[i].iSysColorTo)) :
				globalData.GetColor (sysColorMap[i].iSysColorTo);
		}
	}

	return color;
}
//********************************************************************************
COLORREF CBCGPToolBarImages::MapToSysColorAlpha (COLORREF color)
{
	BYTE r = GetRValue (color);
	BYTE g = GetGValue (color);
	BYTE b = GetBValue (color);

	const int nDelta = 10;

	if (abs (r - b) > nDelta || abs (r - g) > nDelta || abs (b - g) > nDelta)
	{
		return color;
	}

	return CBCGPDrawManager::PixelAlpha (globalData.clrBarFace,
		1. + ((double) r - 192) / 255,
		1. + ((double) g - 192) / 255,
		1. + ((double) b - 192) / 255);
}
//********************************************************************************
COLORREF CBCGPToolBarImages::MapFromSysColor (COLORREF color, BOOL bUseRGBQUAD)
{
	struct COLORMAP
	{
		// use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
		DWORD rgbTo;
		int iSysColorFrom;
	};
	static const COLORMAP sysColorMap[] =
	{
		// mapping from color in DIB to system color
		{ RGB(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
		{ RGB(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark grey
		{ RGB(0xC0, 0xC0, 0xC0),  COLOR_BTNFACE },       // bright grey
		{ RGB(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
	};
	const int nMaps = 4;

	// look for matching RGBQUAD color in original
	for (int i = 0; i < nMaps; i++)
	{
		COLORREF clrSystem = globalData.GetColor (sysColorMap[i].iSysColorFrom);

		if (bUseRGBQUAD)
		{
			if (color == CLR_TO_RGBQUAD (clrSystem))
			{
				return CLR_TO_RGBQUAD (sysColorMap[i].rgbTo);
			}
		}
		else
		{
			if (color == clrSystem)
			{
				return sysColorMap[i].rgbTo;
			}
		}
	}

	return color;
}
//************************************************************************************
BOOL CBCGPToolBarImages::Save (LPCTSTR lpszBmpFileName)
{
	if (!m_bUserImagesList ||	// Only user-defined bitmaps can be saved!
		m_hbmImageWell == NULL)	// Not loaded yet!
	{
		return FALSE;
	}

	if (m_bReadOnly)
	{
		return FALSE;
	}

	CString strFile;
	if (lpszBmpFileName == NULL)
	{
		strFile = m_strUDLPath;
	}
	else
	{
		strFile = lpszBmpFileName;
	}

	if (!m_bModified && strFile == m_strUDLPath)
	{
		return TRUE;
	}

	HANDLE hDib = DDBToDIB (m_hbmImageWell, 0);
	if (hDib == NULL)
	{
		TRACE(_T("CBCGPToolBarImages::Save Can't convert DDB to DIB\n"));
		return FALSE;
	}

	BOOL bSuccess = WriteDIB (strFile, hDib);
	::GlobalFree (hDib);

	if (!bSuccess)
	{
		return FALSE;
	}

	m_bModified = FALSE;
	return TRUE;
}
//************************************************************************************
static BOOL WriteDIB( LPCTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if( !file.Open (szFile, CFile::modeWrite | CFile::modeCreate))
	{
		return FALSE;
	}

	lpbi = (LPBITMAPINFOHEADER) hDIB;

	int nColors = 1 << lpbi->biBitCount;
	if (nColors > 256 || lpbi->biBitCount == 32)
		nColors = 0;

	// Fill in the fields of the file header 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= (DWORD) (GlobalSize (hDIB) + sizeof (BITMAPFILEHEADER));
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	// Write the file header 
	file.Write( &hdr, sizeof(hdr) );

	// Write the DIB header and the bits 
	file.Write( lpbi, (UINT) GlobalSize(hDIB) );

	return TRUE;
}
//********************************************************************************
static HANDLE DDBToDIB (HBITMAP bitmap, DWORD dwCompression) 
{
	BITMAP				bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD				dwLen;
	HANDLE				hDIB;
	HANDLE				handle;
	HDC 				hDC;
	HPALETTE			hPal;

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS)
		return NULL;

	hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	::GetObject(bitmap, sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize			= sizeof(BITMAPINFOHEADER);
	bi.biWidth			= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= (WORD) (bm.bmPlanes * bm.bmBitsPixel);
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if (nColors > 256 || bi.biBitCount == 32)
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::CreateCompatibleDC (NULL);
	if (hDC == NULL)
	{
		return FALSE;
	}

	HBITMAP bmp = ::CreateBitmap (1, 1, 1, bi.biBitCount, NULL);
	if (bmp == NULL)
	{
		::DeleteDC(hDC);
		return NULL;
	}

	HBITMAP hOldBitmap = (HBITMAP)::SelectObject (hDC, bmp);

	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB)
	{
		::SelectPalette (hDC,hPal,FALSE);

		if (hOldBitmap != NULL)
		{
			::SelectObject (hDC, hOldBitmap);
		}

		::DeleteObject (bmp);
		::DeleteDC(hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, bitmap, 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE);
	if (handle != NULL)
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		if (hOldBitmap != NULL)
		{
			::SelectObject (hDC, hOldBitmap);
		}
		::DeleteObject (bmp);
		::DeleteDC(hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, bitmap,
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		
		SelectPalette(hDC,hPal,FALSE);
		if (hOldBitmap != NULL)
		{
			::SelectObject (hDC, hOldBitmap);
		}
		::DeleteObject (bmp);
		::DeleteDC(hDC);
		return NULL;
	}

	// Convert color table to the standard 3-d colors:
	DWORD* pColorTable = (DWORD*)(((LPBYTE)lpbi) + (UINT) lpbi->biSize);
	for (int iColor = 0; iColor < nColors; iColor ++)
	{
		pColorTable[iColor] = CBCGPToolBarImages::MapFromSysColor (pColorTable[iColor]);
	}

	SelectPalette(hDC,hPal,FALSE);

	if (hOldBitmap != NULL)
	{
		::SelectObject (hDC, hOldBitmap);
	}

	::DeleteObject (bmp);
	::DeleteDC(hDC);

	return hDIB;
}


/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBarImages diagnostics

#ifdef _DEBUG
void CBCGPToolBarImages::AssertValid() const
{
	CObject::AssertValid();

	ASSERT(m_hbmImageWell != NULL);
}

void CBCGPToolBarImages::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "\nm_bUserImagesList = " << m_bUserImagesList;
	dc << "\nm_sizeImage = " << m_sizeImage;

	if (m_bUserImagesList)
	{
		dc << "\nm_strUDLPath = " << m_strUDLPath;
	}

	if (dc.GetDepth() > 0)
	{
	}

	dc << "\n";
}

#endif

BOOL CBCGPToolBarImages::CopyImageToClipboard (int iImage)
{
	CBCGPLocalResource locaRes;

	try
	{
		CWindowDC dc (NULL);

		//----------------------
		// Create a bitmap copy:
		//----------------------
		CDC memDCDest;
		memDCDest.CreateCompatibleDC (NULL);
		
		CBitmap bitmapCopy;
		if (!bitmapCopy.CreateCompatibleBitmap (&dc, m_sizeImage.cx, m_sizeImage.cy))
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			return FALSE;
		}

		CBitmap* pOldBitmapDest = memDCDest.SelectObject (&bitmapCopy);

		memDCDest.FillRect (CRect (0, 0, m_sizeImage.cx, m_sizeImage.cy),
			&globalData.brBtnFace);

		CBCGPDrawState ds;
		PrepareDrawImage (ds, FALSE);
	
		Draw (&memDCDest, 0, 0, iImage);
		EndDrawImage (ds);

		memDCDest.SelectObject (pOldBitmapDest);

		if (!AfxGetMainWnd ()->OpenClipboard ())
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			return FALSE;
		}

		if (!::EmptyClipboard ())
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			::CloseClipboard ();
			return FALSE;
		}


		HANDLE hclipData = ::SetClipboardData (CF_BITMAP, bitmapCopy.Detach ());
		if (hclipData == NULL)
		{
			AfxMessageBox (IDP_BCGBARRES_CANT_COPY_BITMAP);
			TRACE (_T("CBCGToolBar::OnBcgbarresCopyImage error. Error code = %x\n"), GetLastError ());
		}

		::CloseClipboard ();
		return TRUE;
	}
	catch (...)
	{
		AfxMessageBox (IDP_BCGBARRES_INTERLAL_ERROR);
	}

	return FALSE;
}
//****************************************************************************************
BOOL CBCGPToolBarImages::CopyTo (CBCGPToolBarImages& dest)
{
	if (dest.m_bIsTemporary)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (dest.IsValid ())
	{
		dest.Clear ();
	}

	dest.m_sizeImage = m_sizeImage;        
	dest.m_sizeImageDest = m_sizeImageDest;	
	dest.m_bUserImagesList = m_bUserImagesList;	
	dest.m_strUDLPath = m_strUDLPath;		
	dest.m_bModified = m_bModified;		
	dest.m_iCount = m_iCount;			
	dest.m_clrTransparent = m_clrTransparent;	
	dest.m_bReadOnly = m_bReadOnly;

	dest.m_hbmImageWell = (HBITMAP) ::CopyImage (m_hbmImageWell, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	POSITION pos;
	for (pos = m_lstOrigResIds.GetHeadPosition (); pos != NULL;)
	{
		UINT uiResId = m_lstOrigResIds.GetNext (pos);

		dest.m_lstOrigResIds.AddTail (uiResId);

		int iOffset = -1;
		if (m_mapOrigResOffsets.Lookup (uiResId, iOffset))
		{
			dest.m_mapOrigResOffsets.SetAt (uiResId, iOffset);
		}
	}

	for (POSITION posInst = m_lstOrigResInstances.GetHeadPosition (); pos != NULL;)
	{
		HINSTANCE hInst = m_lstOrigResInstances.GetNext (posInst);
		dest.m_lstOrigResInstances.AddTail (hInst);
	}

	return TRUE;
}
//****************************************************************************************
void CBCGPToolBarImages::Clear ()
{
	if (m_bIsTemporary)
	{
		ASSERT (FALSE);
		return;
	}

	AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
	m_hbmImageWell = NULL;
	AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
	m_hbmImageLight = NULL;

	m_lstOrigResIds.RemoveAll();
	m_mapOrigResOffsets.RemoveAll ();
	m_lstOrigResInstances.RemoveAll ();
	m_strUDLPath.Empty();

	m_bUserImagesList = FALSE;
	m_iCount = 0;
	m_bModified = FALSE;
}
//*************************************************************************************
void CBCGPToolBarImages::TransparentBlt( HDC hdcDest, int nXDest, int nYDest, int nWidth, 
			int nHeight, CDC* pDcSrc, int nXSrc, int nYSrc,
			COLORREF colorTransparent,
			int nWidthDest/* = -1*/, int nHeightDest/* = -1*/)
{
	CDC dc, memDC, maskDC;
	dc.Attach( hdcDest );
	maskDC.CreateCompatibleDC(&dc);
	CBitmap maskBitmap;

	int cx = nWidthDest == -1 ? nWidth : nWidthDest;
	int cy = nHeightDest == -1 ? nHeight : nHeightDest;
	
	//add these to store return of SelectObject() calls
	CBitmap* pOldMemBmp = NULL;
	CBitmap* pOldMaskBmp = NULL;
	
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmpImage;
	bmpImage.CreateCompatibleBitmap( &dc, cx, cy);
	pOldMemBmp = memDC.SelectObject( &bmpImage );

	if (nWidthDest == -1 || (nWidthDest == nWidth && nHeightDest == nHeight))
	{
		memDC.BitBlt( 0,0,nWidth, nHeight, pDcSrc, nXSrc, nYSrc, SRCCOPY);
	}
	else
	{
		memDC.StretchBlt (0,0, nWidthDest, nHeightDest, pDcSrc, 
						nXSrc, nYSrc, nWidth, nHeight, SRCCOPY);
	}
	
	// Create monochrome bitmap for the mask
	maskBitmap.CreateBitmap (cx, cy, 1, 1, NULL );
	pOldMaskBmp = maskDC.SelectObject( &maskBitmap );
	memDC.SetBkColor( colorTransparent );
	
	// Create the mask from the memory DC
	maskDC.BitBlt (0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);
	
	// Set the background in memDC to black. Using SRCPAINT with black 
	// and any other color results in the other color, thus making 
	// black the transparent color
	memDC.SetBkColor(RGB(0,0,0));
	memDC.SetTextColor(RGB(255,255,255));
	memDC.BitBlt(0, 0, cx, cy, &maskDC, 0, 0, SRCAND);
	
	// Set the foreground to black. See comment above.
	dc.SetBkColor(RGB(255,255,255));
	dc.SetTextColor(RGB(0,0,0));

	dc.BitBlt (nXDest, nYDest, cx, cy, &maskDC, 0, 0, SRCAND);
		
	// Combine the foreground with the background
	dc.BitBlt(nXDest, nYDest, cx, cy, &memDC, 
		0, 0, SRCPAINT);
	
	if (pOldMaskBmp)
		maskDC.SelectObject( pOldMaskBmp );
	if (pOldMemBmp)
		memDC.SelectObject( pOldMemBmp );
	
	dc.Detach();
}
//**********************************************************************************
BOOL CBCGPToolBarImages::MapTo3dColors (	BOOL bUseRGBQUAD/* = TRUE*/,
										COLORREF clrSrc/* = (COLORREF)-1*/,
										COLORREF clrDest/* = (COLORREF)-1*/)
{
	if (m_hbmImageWell == NULL)
	{
		return FALSE;
	}

	if (clrSrc != (COLORREF)-1 && clrDest == (COLORREF)-1)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	//-------------------------------------------------------
	// Create memory source DC and select an original bitmap:
	//-------------------------------------------------------
	CDC memDCSrc;
	memDCSrc.CreateCompatibleDC (NULL);

	HBITMAP hOldBitmapSrc = NULL;

	int iBitmapWidth;
	int iBitmapHeight;

	//-------------------------------
	// Get original bitmap attrbutes:
	//-------------------------------
	BITMAP bmp;
	if (::GetObject (m_hbmImageWell, sizeof (BITMAP), &bmp) == 0)
	{
		return FALSE;
	}

	hOldBitmapSrc = (HBITMAP) memDCSrc.SelectObject (m_hbmImageWell);
	if (hOldBitmapSrc == NULL)
	{
		return FALSE;
	}

	iBitmapWidth = bmp.bmWidth;
	iBitmapHeight = bmp.bmHeight;

	//----------------------------------------------------------
	// Create a new bitmap compatibel with the source memory DC:
	// (original bitmap SHOULD BE ALREADY SELECTED!):
	//----------------------------------------------------------
	HBITMAP hNewBitmap = (HBITMAP) ::CreateCompatibleBitmap (memDCSrc,
									iBitmapWidth,
									iBitmapHeight);
	if (hNewBitmap == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		return FALSE;
	}

	//------------------------------
	// Create memory destination DC:
	//------------------------------
	CDC memDCDst;
	memDCDst.CreateCompatibleDC (&memDCSrc);

	HBITMAP hOldBitmapDst = (HBITMAP) memDCDst.SelectObject (hNewBitmap);
	if (hOldBitmapDst == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		::DeleteObject (hNewBitmap);
		return FALSE;
	}

	//-----------------------------
	// Copy original bitmap to new:
	//-----------------------------
	memDCDst.BitBlt (0, 0, iBitmapWidth, iBitmapHeight,
				&memDCSrc, 0, 0, SRCCOPY);
		
	//------------------------------------
	// Change a specific colors to system:
	//------------------------------------
	for (int x = 0; x < iBitmapWidth; x ++)
	{
		for (int y = 0; y < iBitmapHeight; y ++)
		{
			COLORREF clrOrig = ::GetPixel (memDCDst, x, y);

			if (clrSrc != (COLORREF)-1)
			{
				if (clrOrig == clrSrc)
				{
					::SetPixel (memDCDst, x, y, clrDest);
				}
			}
			else
			{
				COLORREF clrNew = m_nBitsPerPixel == 24 && !m_bDisableTrueColorAlpha ?
					MapToSysColorAlpha (clrOrig) :
					MapToSysColor (clrOrig, bUseRGBQUAD);

				if (clrOrig != clrNew)
				{
					::SetPixel (memDCDst, x, y, clrNew);
				}
			}
		}
	}
	
	memDCDst.SelectObject (hOldBitmapDst);
	memDCSrc.SelectObject (hOldBitmapSrc);

	::DeleteObject (m_hbmImageWell);
	m_hbmImageWell = hNewBitmap;

	return TRUE;
}
//*******************************************************************************
void CBCGPToolBarImages::CopyTemp (CBCGPToolBarImages& imagesDest)
{
	imagesDest.Clear ();
	imagesDest.m_bIsTemporary = TRUE;

	imagesDest.m_sizeImage = m_sizeImage;
	imagesDest.m_sizeImageDest = m_sizeImageDest;
	imagesDest.m_hbmImageWell = m_hbmImageWell;
	imagesDest.m_bUserImagesList = m_bUserImagesList;
	imagesDest.m_iCount = m_iCount;
	imagesDest.m_bReadOnly = TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::UpdateLightImage ()
{
	AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
	m_hbmImageLight = NULL;

	if (m_hbmImageWell == NULL)
	{
		return TRUE;
	}

	OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	::GetVersionEx (&osvi);

	if (globalData.m_nBitsPerPixel <= 8 ||
		osvi.dwPlatformId != VER_PLATFORM_WIN32_NT)
	{
		return TRUE;
	}

	//-------------------------------------------------------
	// Create memory source DC and select an original bitmap:
	//-------------------------------------------------------
	CDC memDCSrc;
	memDCSrc.CreateCompatibleDC (NULL);

	BITMAP bmp;
	if (::GetObject (m_hbmImageWell, sizeof (BITMAP), &bmp) == 0)
	{
		return FALSE;
	}

	int iBitmapWidth = bmp.bmWidth;
	int iBitmapHeight = bmp.bmHeight;

	HBITMAP hOldBitmapSrc = (HBITMAP) memDCSrc.SelectObject (m_hbmImageWell);
	if (hOldBitmapSrc == NULL)
	{
		return FALSE;
	}

	//------------------------------------------------------
	// Create memory destination DC and select a new bitmap:
	//------------------------------------------------------
	CDC memDCDst;
	memDCDst.CreateCompatibleDC (&memDCSrc);
	
	BITMAPINFO bi;

	// Fill in the BITMAPINFOHEADER
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = iBitmapWidth;
	bi.bmiHeader.biHeight = iBitmapHeight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = iBitmapWidth * iBitmapHeight;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	COLORREF* pBits = NULL;
	m_hbmImageLight = CreateDIBSection (
		memDCDst.m_hDC, &bi, DIB_RGB_COLORS, (void **)&pBits,
		NULL, NULL);

	if (m_hbmImageLight == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		return FALSE;
	}

	HBITMAP hOldBitmapDst = (HBITMAP) memDCDst.SelectObject (m_hbmImageLight);
	if (hOldBitmapDst == NULL)
	{
		memDCSrc.SelectObject (hOldBitmapSrc);
		::DeleteObject (m_hbmImageLight);
		m_hbmImageLight = NULL;
		return FALSE;
	}

	//-----------------------------
	// Copy original bitmap to new:
	//-----------------------------
	COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
		(COLORREF) -1 : m_clrTransparent;

	memDCDst.BitBlt (0, 0, iBitmapWidth, iBitmapHeight,
				&memDCSrc, 0, 0, SRCCOPY);
		
	CBCGPDrawManager dm (memDCDst);

	dm.HighlightRect (CRect (0, 0, iBitmapWidth, iBitmapHeight),
		130,
		clrTransparent == -1 ? globalData.clrBtnFace : clrTransparent);

	memDCDst.SelectObject (hOldBitmapDst);
	memDCSrc.SelectObject (hOldBitmapSrc);

	return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::PreMultiplyAlpha (HBITMAP hbmp)
{
	DIBSECTION ds;
	if (::GetObject (hbmp, sizeof (DIBSECTION), &ds) == 0)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (ds.dsBm.bmBitsPixel != 32)
	{
		return FALSE;
	}

	RGBQUAD* pBits = (RGBQUAD*) ds.dsBm.bmBits;

	//----------------------------------------------------------------
	// Premultiply the R,G and B values with the Alpha channel values:
	//----------------------------------------------------------------
	for (int i = 0; i < ds.dsBm.bmWidth * ds.dsBm.bmHeight; i++)
	{
		RGBQUAD* pBit = pBits + i;
		pBit->rgbRed = (BYTE) (pBit->rgbRed * pBit->rgbReserved / 255);
		pBit->rgbGreen = (BYTE) (pBit->rgbGreen * pBit->rgbReserved / 255);
		pBit->rgbBlue = (BYTE) (pBit->rgbBlue * pBit->rgbReserved / 255);
	}

	return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::CreateFromImageList (const CImageList& imageList)
{
	ASSERT_VALID (this);
	ASSERT (imageList.GetSafeHandle () != NULL);
	ASSERT (imageList.GetImageCount () > 0);

	Clear ();

	IMAGEINFO info;
	imageList.GetImageInfo (0, &info);

	CRect rectImage = info.rcImage;
	m_sizeImage = rectImage.Size ();

	for (int i = 0; i < imageList.GetImageCount (); i++)
	{
		HICON hIcon = ((CImageList&) imageList).ExtractIcon (i);
		ASSERT (hIcon != NULL);

		AddIcon (hIcon);

		::DestroyIcon (hIcon);
	}

	return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Is32BitTransparencySupported ()
{
	return !globalData.bIsWindows9x && !globalData.bIsWindowsNT4;
}
