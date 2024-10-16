// BCGColorDialog.cpp : implementation file
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//

#include "stdafx.h"

#ifndef BCG_NO_COLOR

#include "BCGCBPro.h"
#include "BCGPLocalResource.h"
#include "BCGPColorDialog.h"
#include "PropertySheetCtrl.h"
#include "BCGGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPColorDialog dialog

CBCGPColorDialog::CBCGPColorDialog (COLORREF clrInit, DWORD /*dwFlags - reserved */, 
					CWnd* pParentWnd, HPALETTE hPal)
	: CDialog(CBCGPColorDialog::IDD, pParentWnd)
{
	//{{AFX_DATA_INIT(CBCGPColorDialog)
	//}}AFX_DATA_INIT

	m_pColourSheetOne = NULL;
	m_pColourSheetTwo = NULL;

	m_CurrentColor = m_NewColor = clrInit;
	m_pPropSheet = NULL;
	m_bIsMyPalette = TRUE;
	m_pPalette = NULL;
	
	if (hPal != NULL)
	{
		m_pPalette = CPalette::FromHandle (hPal);
		m_bIsMyPalette = FALSE;
	}
}

CBCGPColorDialog::~CBCGPColorDialog ()
{
	if (m_pColourSheetOne != NULL)
	{
		delete m_pColourSheetOne;
	}

	if (m_pColourSheetTwo != NULL)
	{
		delete m_pColourSheetTwo;
	}
}

void CBCGPColorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBCGPColorDialog)
	DDX_Control(pDX, IDC_BCGBARRES_STATICPLACEHOLDER, m_wndStaticPlaceHolder);
	DDX_Control(pDX, IDC_BCGBARRES_COLOURPLACEHOLDER, m_wndColors);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBCGPColorDialog, CDialog)
	//{{AFX_MSG_MAP(CBCGPColorDialog)
	ON_WM_DESTROY()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPColorDialog message handlers

BOOL CBCGPColorDialog::OnInitDialog()
{
	CBCGPLocalResource locaRes;

	CDialog::OnInitDialog();

	if (globalData.m_nBitsPerPixel < 8)	// 16 colors, call standard dialog
	{
		CColorDialog dlg (m_CurrentColor, CC_FULLOPEN | CC_ANYCOLOR);
		int nResult = (int) dlg.DoModal ();
		m_NewColor = dlg.GetColor ();
		EndDialog (nResult);

		return TRUE;
	}

	if (m_pPalette == NULL)
	{
		m_pPalette = new CPalette ();
		RebuildPalette ();
	}

	m_wndColors.SetType (CBCGPColorPickerCtrl::CURRENT);
	m_wndColors.SetPalette (m_pPalette);

	m_wndColors.SetOriginalColor (m_CurrentColor);
	m_wndColors.SetRGB (m_NewColor);

	// Create property sheet.
	m_pPropSheet = new CPropertySheetCtrl(_T(""), this);
	ASSERT(m_pPropSheet);

	m_pColourSheetOne = new CColorPage1;
	m_pColourSheetTwo = new CColorPage2;

	// Set parent dialog.
	m_pColourSheetOne->m_pDialog = this;
	m_pColourSheetTwo->m_pDialog = this;

	m_pPropSheet->AddPage (m_pColourSheetOne);
	m_pPropSheet->AddPage (m_pColourSheetTwo);

	// Retrieve the location of the window
	CRect rectListWnd;
	m_wndStaticPlaceHolder.GetWindowRect(rectListWnd);
	ScreenToClient(rectListWnd);
	
	if (!m_pPropSheet->Create(this, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0))
	{
		TRACE0 ("CBCGPColorDialog::OnInitDialog(): Can't create the property sheet.....\n");
	}

	m_pPropSheet->SetWindowPos(NULL, rectListWnd.left, rectListWnd.top, rectListWnd.Width(),
				rectListWnd.Height(), SWP_NOZORDER | SWP_NOACTIVATE);

	SetPageOne (GetRValue (m_CurrentColor), 
				GetGValue (m_CurrentColor),
				GetBValue (m_CurrentColor));

	SetPageTwo (GetRValue (m_CurrentColor),
				GetGValue (m_CurrentColor),
				GetBValue (m_CurrentColor));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBCGPColorDialog::SetCurrentColor(COLORREF rgb)
{
	m_CurrentColor = rgb;
}

void CBCGPColorDialog::SetPageOne(BYTE R, BYTE G, BYTE B)
{
	m_pColourSheetOne->m_hexpicker.SelectCellHexagon(R, G, B);
	m_pColourSheetOne->m_hexpicker_greyscale.SelectCellHexagon(R, G, B);
}

void CBCGPColorDialog::SetPageTwo(BYTE R, BYTE G, BYTE B)
{
	m_pColourSheetTwo->Setup (R, G, B);
}

INT_PTR CBCGPColorDialog::DoModal() 
{
	CBCGPLocalResource locaRes;
	return CDialog::DoModal();
}

void CBCGPColorDialog::OnDestroy() 
{
	if (m_bIsMyPalette && m_pPalette != NULL)
	{
		m_pPalette->DeleteObject ();
		delete m_pPalette;
		m_pPalette = NULL;
	}

	CDialog::OnDestroy();
}

void CBCGPColorDialog::SetNewColor (COLORREF rgb)
{
	m_NewColor = rgb;

	if (globalData.m_nBitsPerPixel == 8) // 256 colors
	{
		ASSERT (m_pPalette != NULL);

		UINT uiPalIndex = m_pPalette->GetNearestPaletteIndex (rgb);
		m_wndColors.SetRGB (PALETTEINDEX (uiPalIndex));
	}
	else
	{
		m_wndColors.SetRGB (rgb);
	}

	m_wndColors.Invalidate ();
	m_wndColors.UpdateWindow ();
}

void CBCGPColorDialog::OnSysColorChange() 
{
	CDialog::OnSysColorChange();

	globalData.UpdateSysColors ();
	
	if (m_bIsMyPalette)
	{
		if (globalData.m_nBitsPerPixel < 8)	// 16 colors, call standard dialog
		{
			ShowWindow (SW_HIDE);

			CColorDialog dlg (m_CurrentColor, CC_FULLOPEN | CC_ANYCOLOR);
			int nResult = (int) dlg.DoModal ();
			m_NewColor = dlg.GetColor ();
			EndDialog (nResult);
		}
		else
		{
			::DeleteObject (m_pPalette->Detach ());
			RebuildPalette ();

			Invalidate ();
			UpdateWindow ();
		}
	}
}

void CBCGPColorDialog::RebuildPalette ()
{
	ASSERT (m_pPalette->GetSafeHandle () == NULL);

	// Create palette:
	CClientDC dc (this);

	int nColors = 256;	// Use 256 first entries
	UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
	LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

	pLP->palVersion = 0x300;
	pLP->palNumEntries = (USHORT) nColors;

	::GetSystemPaletteEntries (dc.GetSafeHdc (), 0, nColors, pLP->palPalEntry);

	m_pPalette->CreatePalette (pLP);

	delete[] pLP;
}

#endif // BCG_NO_COLOR
