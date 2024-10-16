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

#include "stdafx.h"
#include "BCGCBPro.h"
#include "bcgglobals.h"
#include "BCGPVisualManager.h"
#include "BCGPKeyboardManager.h"
#include "MenuHash.h"
#include "BCGPToolBar.h"
#include "MenuImages.h"
#include "BCGPMiniframeWnd.h"

extern CObList	gAllToolbars;

BOOL CBCGPMemDC::m_bUseMemoryDC = TRUE;

static const CString strOfficeFontName	= _T("Tahoma");
static const CString strDefaultFontName = _T("MS Sans Serif");
static const CString strVertFontName	= _T("Arial");
static const CString strMarlettFontName	= _T("Marlett");

static int CALLBACK FontFamalyProcFonts (const LOGFONT FAR* lplf,
									const TEXTMETRIC FAR* /*lptm*/,
									ULONG /*ulFontType*/,
									LPARAM /*lParam*/)
{
	ASSERT (lplf != NULL);

	CString strFont = lplf->lfFaceName;
	return strFont.CollateNoCase (strOfficeFontName) == 0 ? 0 : 1;
}
/////////////////////////////////////////////////////////////////////////////
// Cached system metrics, etc

BCGCBPRODLLEXPORT GLOBAL_DATA globalData;

// Initialization code
GLOBAL_DATA::GLOBAL_DATA()
{
	//---------------------------------------------------------
	// Cached system values (updated in CWnd::OnSysColorChange)
	//---------------------------------------------------------
	hbrBtnShadow = NULL;
	hbrBtnHilite = NULL;
	hbrWindow = NULL;

	UpdateSysColors();

	m_hcurStretch = NULL;
	m_hcurStretchVert = NULL;
	m_hcurHand = NULL;
	m_hcurSizeAll = NULL;
	m_hiconTool = NULL;

	UpdateFonts();

	//-----------------------
	// Shell icon sizes:
	//-----------------------
	CoInitialize (NULL);
	SHFILEINFO	sfi;
	HIMAGELIST	hil = NULL;
	hil	= (HIMAGELIST) ::SHGetFileInfo(_T(""), FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof sfi, SHGFI_SHELLICONSIZE | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
	if (hil != NULL)
	{
		int	cxIcon, cyIcon;
		VERIFY(ImageList_GetIconSize(hil, &cxIcon, &cyIcon));
		m_sizeSmallIcon.cx	= cxIcon;
		m_sizeSmallIcon.cy	= cyIcon;
	}
	else
	{
		m_sizeSmallIcon.cx	= ::GetSystemMetrics (SM_CXSMICON);
		m_sizeSmallIcon.cy	= ::GetSystemMetrics (SM_CYSMICON);
	}

	CoUninitialize ();

	//-----------------------
	// Detect the kind of OS:
	//-----------------------
	OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	::GetVersionEx (&osvi);
	bIsWindowsNT4 = ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
					(osvi.dwMajorVersion < 5));

	bIsWindows9x = (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);

	m_nDragFrameThiknessFloat = 4;  // pixels
	m_nDragFrameThiknessDock = 3;   // pixels

	m_nAutoHideToolBarSpacing = 14; // pixels
	m_nAutoHideToolBarMargin  = 4;  // pixels

	m_nCoveredMainWndClientAreaPercent = 50; // percents

	bIsRemoteSession = FALSE;

	if ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
		(osvi.dwMajorVersion >= 5))
	{
		bIsRemoteSession = GetSystemMetrics (0x1000 /* SM_REMOTESESSION */);
	}

	m_hinstUXThemeDLL = NULL;

	m_nMaxToolTipWidth = -1;
}
//*******************************************************************************************
GLOBAL_DATA::~GLOBAL_DATA()
{
	CleanUp ();
}
//************************************************************************************
void GLOBAL_DATA::UpdateFonts()
{
	if (fontRegular.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontRegular.Detach ());
	}

	if (fontTooltip.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontTooltip.Detach ());
	}

	if (fontBold.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontBold.Detach ());
	}

	if (fontDefaultGUIBold.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontDefaultGUIBold.Detach ());
	}

	if (fontUnderline.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontUnderline.Detach ());
	}

	if (fontDefaultGUIUnderline.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontDefaultGUIUnderline.Detach ());
	}

	if (fontVert.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontVert.Detach ());
	}

	if (fontVertCaption.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontVertCaption.Detach ());
	}

	if (fontMarlett.GetSafeHandle () != NULL)
	{
		::DeleteObject (fontMarlett.Detach ());
	}

	//------------------
	// Initialize fonts:
	//------------------
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	::SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);

	LOGFONT lf;
	memset (&lf, 0, sizeof (LOGFONT));

	CWindowDC dc (NULL);
	lf.lfCharSet = (BYTE) GetTextCharsetInfo (dc.GetSafeHdc (), NULL, 0);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	//////////////////////////////////////////////
	// 	// Check if we should use system font
	//--------------------------------------------
	_tcscpy (lf.lfFaceName, info.lfMenuFont.lfFaceName);

	BOOL fUseSystemFont = (info.lfMenuFont.lfCharSet > SYMBOL_CHARSET);
	if (!fUseSystemFont)
	{
		//----------------------------------
		// Check for "Tahoma" font existance:
		//----------------------------------
		if (::EnumFontFamilies (dc.GetSafeHdc (), NULL, FontFamalyProcFonts, 0) == 0)
		{
			//---------------------------
			// Found! Use MS Office font!
			//---------------------------
			_tcscpy (lf.lfFaceName, strOfficeFontName);
		}
		else
		{
			//-----------------------------
			// Not found. Use default font:
			//-----------------------------
			_tcscpy (lf.lfFaceName, strDefaultFontName);
		}
	}
	//--------------------------------------------
	//////////////////////////////////////////////

	fontRegular.CreateFontIndirect (&lf);

	//---------------------
	// Create tooltip font:
	//---------------------
	NONCLIENTMETRICS ncm;
	memset (&ncm, 0, sizeof (NONCLIENTMETRICS));
	ncm.cbSize = sizeof (NONCLIENTMETRICS);

	::SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof (NONCLIENTMETRICS),
		&ncm, 0);

	lf.lfItalic = ncm.lfStatusFont.lfItalic;
	lf.lfWeight = ncm.lfStatusFont.lfWeight;
	fontTooltip.CreateFontIndirect (&lf);

	lf.lfItalic = info.lfMenuFont.lfItalic;
	lf.lfWeight = info.lfMenuFont.lfWeight;

	//-------------------------
	// Create "underline" font:
	//-------------------------
	lf.lfUnderline = TRUE;
	fontUnderline.CreateFontIndirect (&lf);
	lf.lfUnderline = FALSE;

	//------------------
	// Create bold font:
	//------------------
	lf.lfWeight = FW_BOLD;
	fontBold.CreateFontIndirect (&lf);

	//---------------------
	// Create Marlett font:
	//---------------------
	BYTE bCharSet = lf.lfCharSet;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfCharSet = SYMBOL_CHARSET;
	lf.lfWeight = 0;
	lf.lfHeight = ::GetSystemMetrics (SM_CYMENUCHECK) - 1;
	_tcscpy (lf.lfFaceName, strMarlettFontName);

	fontMarlett.CreateFontIndirect (&lf);

	lf.lfCharSet = bCharSet;	// Restore charset

	//----------------------
	// Create vertical font:
	//----------------------
	CFont font;
	if (font.CreateStockObject (DEFAULT_GUI_FONT))
	{
		if (font.GetLogFont (&lf) != 0)
		{
			lf.lfOrientation = 900;
			lf.lfEscapement = 2700;

			lf.lfHeight = info.lfMenuFont.lfHeight;
			lf.lfWeight = info.lfMenuFont.lfWeight;
			lf.lfItalic = info.lfMenuFont.lfItalic;
			
			{
				_tcscpy (lf.lfFaceName, strVertFontName);
			}
			//--------------------------------------------
			//////////////////////////////////////////////

			fontVert.CreateFontIndirect (&lf);

			lf.lfEscapement = 900;
			fontVertCaption.CreateFontIndirect (&lf);
		}
	}

	//----------------------------------------
	// Create dialog underline and bold fonts:
	//----------------------------------------
	CFont* pDefaultGUIFont = CFont::FromHandle ((HFONT) GetStockObject (DEFAULT_GUI_FONT));
	ASSERT_VALID (pDefaultGUIFont);
	pDefaultGUIFont->GetLogFont (&lf);

	lf.lfUnderline = TRUE;
	fontDefaultGUIUnderline.CreateFontIndirect (&lf);
	lf.lfUnderline = FALSE;

	lf.lfWeight = FW_BOLD;
	fontDefaultGUIBold.CreateFontIndirect (&lf);

	UpdateTextMetrics();

	//-------------------------------------
	// Notify toolbars about font changing:
	//-------------------------------------
	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);

			pToolBar->OnGlobalFontsChanged ();
		}
	}
}
//*******************************************************************************************
void GLOBAL_DATA::UpdateSysColors()
{
	clrBarFace = clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
	clrBarShadow = clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
	clrBarDkShadow = clrBtnDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
	clrBarLight = clrBtnLight = ::GetSysColor(COLOR_3DLIGHT);
	clrBarHilite = clrBtnHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	clrBarText = clrBtnText = ::GetSysColor(COLOR_BTNTEXT);
	clrGrayedText = ::GetSysColor (COLOR_GRAYTEXT);
	clrWindowFrame = ::GetSysColor(COLOR_WINDOWFRAME);

	clrHilite = ::GetSysColor(COLOR_HIGHLIGHT);
	clrTextHilite = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

	clrBarWindow = clrWindow = ::GetSysColor (COLOR_WINDOW);
	clrWindowText = ::GetSysColor (COLOR_WINDOWTEXT);

	clrCaptionText = ::GetSysColor (COLOR_CAPTIONTEXT);
	clrMenuText = ::GetSysColor (COLOR_MENUTEXT);

	clrActiveCaption = ::GetSysColor (COLOR_ACTIVECAPTION);
	clrInactiveCaption = ::GetSysColor (COLOR_INACTIVECAPTION);

	clrActiveBorder = ::GetSysColor (COLOR_ACTIVEBORDER);
	clrInactiveBorder = ::GetSysColor (COLOR_INACTIVEBORDER);

	clrInactiveCaptionText = ::GetSysColor (COLOR_INACTIVECAPTIONTEXT);

#ifdef COLOR_HOTLIGHT

	OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	::GetVersionEx (&osvi);
	if (osvi.dwMajorVersion >= 5)
	{
		clrHotText = ::GetSysColor (COLOR_HOTLIGHT);
	}
	else
#endif
	{
		clrHotText = RGB (0, 0, 255);	// Light blue
	}

	hbrBtnShadow = ::GetSysColorBrush (COLOR_BTNSHADOW);
	ASSERT(hbrBtnShadow != NULL);

	hbrBtnHilite = ::GetSysColorBrush (COLOR_BTNHIGHLIGHT);
	ASSERT(hbrBtnHilite != NULL);

	hbrWindow = ::GetSysColorBrush (COLOR_WINDOW);
	ASSERT(hbrWindow != NULL);

	brBtnFace.DeleteObject ();
	brBtnFace.CreateSolidBrush (clrBtnFace);

	brBarFace.DeleteObject ();
	brBarFace.CreateSolidBrush (clrBarFace);

	brActiveCaption.DeleteObject ();
	brActiveCaption.CreateSolidBrush (clrActiveCaption);

	brInactiveCaption.DeleteObject ();
	brInactiveCaption.CreateSolidBrush (clrInactiveCaption);

	brHilite.DeleteObject ();
	brHilite.CreateSolidBrush (clrHilite);

	brBlack.DeleteObject ();
	brBlack.CreateSolidBrush (clrBtnDkShadow);

	brWindow.DeleteObject ();
	brWindow.CreateSolidBrush (clrWindow);

	penHilite.DeleteObject ();
	penHilite.CreatePen (PS_SOLID, 1, globalData.clrHilite);

	penBarFace.DeleteObject ();
	penBarFace.CreatePen (PS_SOLID, 1, globalData.clrBarFace);

	penBarShadow.DeleteObject ();
	penBarShadow.CreatePen (PS_SOLID, 1, globalData.clrBarShadow);

	CWindowDC dc (NULL);
	m_nBitsPerPixel = dc.GetDeviceCaps (BITSPIXEL);

	if (brLight.GetSafeHandle ())
	{
		brLight.DeleteObject ();
	}
	
	if (m_nBitsPerPixel > 8)
	{
		COLORREF clrLight = RGB (
			GetRValue(clrBtnFace) + ((GetRValue(clrBtnHilite) -
				GetRValue(clrBtnFace)) / 2 ),
			GetGValue(clrBtnFace) + ((GetGValue(clrBtnHilite) -
				GetGValue(clrBtnFace)) / 2),
			GetBValue(clrBtnFace) + ((GetBValue(clrBtnHilite) -
				GetBValue(clrBtnFace)) / 2)
			);

		brLight.CreateSolidBrush (clrLight);
	}
	else
	{
		HBITMAP hbmGray = CreateDitherBitmap (dc.GetSafeHdc ());
		ASSERT (hbmGray != NULL);

		CBitmap bmp;
		bmp.Attach (hbmGray);

		brLight.CreatePatternBrush (&bmp);
	}

	CMenuImages::CleanUp ();
}
//************************************************************************************
BOOL GLOBAL_DATA::SetMenuFont (LPLOGFONT lpLogFont, BOOL bHorz)
{
	ASSERT (lpLogFont != NULL);

	if (bHorz)
	{
		//---------------------
		// Create regular font:
		//---------------------
		fontRegular.DeleteObject ();
		if (!fontRegular.CreateFontIndirect (lpLogFont))
		{
			ASSERT (FALSE);
			return FALSE;
		}

		//-----------------------
		// Create underline font:
		//-----------------------
		lpLogFont->lfUnderline = TRUE;
		fontUnderline.DeleteObject ();
		fontUnderline.CreateFontIndirect (lpLogFont);
		lpLogFont->lfUnderline = FALSE;

		//---------------------------------------------------
		// Create bold font (used in the default menu items):
		//---------------------------------------------------
		long lSavedWeight = lpLogFont->lfWeight;
		lpLogFont->lfWeight = 700;

		fontBold.DeleteObject ();
		BOOL bResult = fontBold.CreateFontIndirect (lpLogFont);

		lpLogFont->lfWeight = lSavedWeight;	// Restore weight

		if (!bResult)
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}
	else	// Vertical font
	{
		fontVert.DeleteObject ();
		if (!fontVert.CreateFontIndirect (lpLogFont))
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	UpdateTextMetrics();
	return TRUE;
}
//************************************************************************************
void GLOBAL_DATA::UpdateTextMetrics ()
{
	CWindowDC dc (NULL);

	CFont* pOldFont = dc.SelectObject (&fontRegular);
	ASSERT (pOldFont != NULL);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	m_nTextHeightHorz = tm.tmHeight + 2;

	dc.SelectObject (&fontVert);
	dc.GetTextMetrics (&tm);

	m_nTextHeightVert = tm.tmHeight + 2;

	dc.SelectObject (pOldFont);
}
//*******************************************************************************
HBITMAP GLOBAL_DATA::CreateDitherBitmap (HDC hDC)
{
	struct  // BITMAPINFO with 16 colors
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD      bmiColors[16];
	} 
	bmi;
	memset(&bmi, 0, sizeof(bmi));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = 8;
	bmi.bmiHeader.biHeight = 8;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 1;
	bmi.bmiHeader.biCompression = BI_RGB;

	COLORREF clr = globalData.clrBtnFace;

	bmi.bmiColors[0].rgbBlue = GetBValue(clr);
	bmi.bmiColors[0].rgbGreen = GetGValue(clr);
	bmi.bmiColors[0].rgbRed = GetRValue(clr);

	clr = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	bmi.bmiColors[1].rgbBlue = GetBValue(clr);
	bmi.bmiColors[1].rgbGreen = GetGValue(clr);
	bmi.bmiColors[1].rgbRed = GetRValue(clr);

	// initialize the brushes
	long patGray[8];
	for (int i = 0; i < 8; i++)
	   patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;

	HBITMAP hbm = CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT,
		(LPBYTE)patGray, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS);
	return hbm;
}
//*************************************************************************************
void GLOBAL_DATA::CleanUp ()
{
	if (brLight.GetSafeHandle ())
	{
		brLight.DeleteObject ();
	}
	
	// cleanup fonts:
	fontRegular.DeleteObject ();
	fontBold.DeleteObject ();
	fontUnderline.DeleteObject ();
	fontVert.DeleteObject ();
	fontVertCaption.DeleteObject ();
	fontTooltip.DeleteObject ();

	if (m_hinstUXThemeDLL != NULL)
	{
		::FreeLibrary (m_hinstUXThemeDLL);
		m_hinstUXThemeDLL = NULL;
	}
}
//*************************************************************************************
BCGCBPRODLLEXPORT void BCGCBProCleanUp ()
{
	globalData.CleanUp ();

	g_menuHash.CleanUp ();

	CBCGPToolBar::CleanUpImages ();
	CMenuImages::CleanUp ();

	if (GetCmdMgr () != NULL)
	{
		GetCmdMgr ()->CleanUp ();
	}

	CBCGPKeyboardManager::CleanUp ();

	// Destroy visualization manager:
	CBCGPVisualManager* pVisManager = CBCGPVisualManager::GetInstance ();
	if (pVisManager != NULL && pVisManager->IsAutoDestroy ())
	{
		delete pVisManager;
	}
}
//****************************************************************************************
BOOL GLOBAL_DATA::DrawParentBackground (CWnd* pWnd, CDC* pDC, LPRECT rectClip)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pWnd);

	BOOL bRes = FALSE;

	CRgn rgn;
	if (rectClip != NULL)
	{
		rgn.CreateRectRgnIndirect (rectClip);
		pDC->SelectClipRgn (&rgn);
	}

	CWnd* pParent = pWnd->GetParent ();
	ASSERT_VALID (pParent);

	// In Windows XP, we need to call DrawThemeParentBackground function to implement
	// transparent controls
	if (m_hinstUXThemeDLL == NULL)
	{
		m_hinstUXThemeDLL = LoadLibrary (_T("UxTheme.dll"));
	}

	if (m_hinstUXThemeDLL != NULL)
	{
		typedef HRESULT (__stdcall * DRAWTHEMEPARENTBACKGROUND)(HWND hWnd, HDC hdc,const RECT *pRec);
		DRAWTHEMEPARENTBACKGROUND m_pfDrawThemeBackground = 
			(DRAWTHEMEPARENTBACKGROUND)::GetProcAddress (m_hinstUXThemeDLL, "DrawThemeParentBackground");

		if (m_pfDrawThemeBackground != NULL)
		{
			bRes = (*m_pfDrawThemeBackground) (pWnd->GetSafeHwnd (), 
				pDC->GetSafeHdc (), rectClip) == S_OK;
		}
	}

	if (!bRes)
	{
		CPoint pt (0, 0);
		pWnd->MapWindowPoints (pParent, &pt, 1);
		pt = pDC->OffsetWindowOrg (pt.x, pt.y);

		bRes = (BOOL) pParent->SendMessage (WM_ERASEBKGND, (WPARAM)pDC->m_hDC);

		pDC->SetWindowOrg(pt.x, pt.y);
	}

	pDC->SelectClipRgn (NULL);

	return bRes;
}

BCGCBPRODLLEXPORT CFrameWnd* BCGPGetParentFrame (const CWnd* pWnd)
{
	if (pWnd->GetSafeHwnd () == NULL)
	{
		return NULL;
	}
	ASSERT_VALID (pWnd);

	const CWnd* pParentWnd = pWnd;

	while (pParentWnd != NULL)
	{
		if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
		{
			CBCGPMiniFrameWnd* pMiniFrame = DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pParentWnd);
			pParentWnd = pMiniFrame->GetParent ();
		}
		else
		{
			pParentWnd = pParentWnd->GetParent ();
		}

		if (pParentWnd == NULL)
		{
			return NULL;
		}
		if (pParentWnd->IsFrameWnd ())
		{
			return (CFrameWnd*)pParentWnd;
		}
	}

	return NULL;
}
//***********************************************************************************
COLORREF GLOBAL_DATA::GetColor (int nColor)
{
	switch (nColor)
	{
	case COLOR_BTNFACE:             return clrBtnFace;
	case COLOR_BTNSHADOW:           return clrBtnShadow;			
	case COLOR_3DDKSHADOW:          return clrBtnDkShadow;			
	case COLOR_3DLIGHT:             return clrBtnLight;				
	case COLOR_BTNHIGHLIGHT:        return clrBtnHilite;			
	case COLOR_BTNTEXT:             return clrBtnText;				
	case COLOR_GRAYTEXT:            return clrGrayedText;			
	case COLOR_WINDOWFRAME:         return clrWindowFrame;			
	                          
	case COLOR_HIGHLIGHT:           return clrHilite;				
	case COLOR_HIGHLIGHTTEXT:       return clrTextHilite;			
	                          
	case COLOR_WINDOW:				return clrWindow;				
	case COLOR_WINDOWTEXT:			return clrWindowText;			
	                          
	case COLOR_CAPTIONTEXT:			return clrCaptionText;			
	case COLOR_MENUTEXT:			return clrMenuText;				
	                          
	case COLOR_ACTIVECAPTION:		return clrActiveCaption;		
	case COLOR_INACTIVECAPTION:		return clrInactiveCaption;		
	                          
	case COLOR_ACTIVEBORDER:		return clrActiveBorder;			
	case COLOR_INACTIVEBORDER:		return clrInactiveBorder;
	                          
	case COLOR_INACTIVECAPTIONTEXT:	return clrInactiveCaptionText;
	}

	return ::GetSysColor (nColor);
}
