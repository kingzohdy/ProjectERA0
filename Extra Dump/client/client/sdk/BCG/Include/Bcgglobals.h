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

#ifndef __GLOBAL_DATA_H
#define __GLOBAL_DATA_H

#include "BCGCBPro.h"

/////////////////////////////////////////////////////////////////////////////
// Auxiliary System/Screen metrics

typedef enum BCGP_DOCK_TYPE
{
	DT_UNDEFINED,  // inherit from application
	DT_IMMEDIATE,  // control bar torn off immediately and follows the mouse
	DT_STANDARD	   // user drags a frame
};

// autohide sliding modes
static const UINT BCGP_AHSM_MOVE		= 1;
static const UINT BCGP_AHSM_STRETCH		= 2;


struct BCGCBPRODLLEXPORT GLOBAL_DATA
{
	// solid brushes with convenient gray colors and system colors
	HBRUSH hbrBtnHilite, hbrBtnShadow;

	HBRUSH hbrWindow;

	// color values of system colors used for CToolBar
	COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF clrBtnText, clrWindowFrame;
	COLORREF clrBtnDkShadow, clrBtnLight;
	COLORREF clrGrayedText;
	COLORREF clrHilite;
	COLORREF clrTextHilite;
	COLORREF clrHotText;

	COLORREF clrBarWindow;
	COLORREF clrBarFace;
	COLORREF clrBarShadow, clrBarHilite;
	COLORREF clrBarDkShadow, clrBarLight;
	COLORREF clrBarText;

	COLORREF clrWindow;
	COLORREF clrWindowText;

	COLORREF clrCaptionText;

	COLORREF clrMenuText;

	COLORREF clrActiveCaption;
	COLORREF clrInactiveCaption;

	COLORREF clrInactiveCaptionText;

	COLORREF clrActiveBorder;
	COLORREF clrInactiveBorder;

	CBrush	brBtnFace;
	CBrush	brHilite;
	CBrush	brLight;
	CBrush	brBlack;
	CBrush	brActiveCaption;
	CBrush	brInactiveCaption;
	CBrush	brWindow;

	CBrush brBarFace;

	CPen	penHilite;
	CPen	penBarFace;
	CPen	penBarShadow;

	HCURSOR	m_hcurStretch;
	HCURSOR	m_hcurStretchVert;
	HCURSOR	m_hcurHand;
	HCURSOR	m_hcurSizeAll;

	HICON	m_hiconTool;

	// Shell icon sizes:
	CSize	m_sizeSmallIcon;

	// Toolbar and menu fonts:
	CFont				fontRegular;
	CFont				fontTooltip;
	CFont				fontBold;
	CFont				fontDefaultGUIBold;
	CFont				fontUnderline;
	CFont				fontDefaultGUIUnderline;
	CFont				fontVert;
	CFont				fontVertCaption;
						
	CFont				fontMarlett;	// Standard Windows menu symbols
						
	BOOL				bIsWindowsNT4;
	BOOL				bIsWindows9x;
	int					m_nBitsPerPixel;

	BOOL				bIsRemoteSession;

	int					m_nDragFrameThiknessFloat;
	int					m_nDragFrameThiknessDock;
						
	int					m_nAutoHideToolBarSpacing;
	int					m_nAutoHideToolBarMargin;
						
	int					m_nCoveredMainWndClientAreaPercent;

	int					m_nMaxToolTipWidth;
						
// Implementation
	GLOBAL_DATA();
	~GLOBAL_DATA();

	void UpdateSysColors();
	void UpdateFonts();

	BOOL SetMenuFont (LPLOGFONT lpLogFont, BOOL bHorz);

	int GetTextHeight (BOOL bHorz = TRUE)
	{
		return bHorz ? m_nTextHeightHorz : m_nTextHeightVert;
	}

	BOOL DrawParentBackground (CWnd* pWnd, CDC* pDC, LPRECT lpRect = NULL);
	void CleanUp ();

	COLORREF GetColor (int nColor);

protected:

	void UpdateTextMetrics ();
	HBITMAP CreateDitherBitmap (HDC hDC);
	
	int	m_nTextHeightHorz;
	int	m_nTextHeightVert;

	HINSTANCE m_hinstUXThemeDLL;
};

extern BCGCBPRODLLEXPORT GLOBAL_DATA globalData;



#define IMAGE_MARGIN	4

// MFC Control bar compatibility 
#define CX_BORDER   1
#define CY_BORDER   1

#define CX_GRIPPER  3
#define CY_GRIPPER  3
#define CX_BORDER_GRIPPER 2
#define CY_BORDER_GRIPPER 2


/////////////////////////////////////////////////////////////////////////////

#endif // __GLOBAL_DATA_H
