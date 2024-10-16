//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2008 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
 //*******************************************************************************

#include "stdafx.h"

#include "BCGPDialog.h"

#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#endif

#include "BCGPVisualManager.h"
#include "BCGPLocalResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBCGPDialog, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog dialog

#pragma warning (disable : 4355)

CBCGPDialog::CBCGPDialog() :
	m_Impl (*this)
{
	CommonConstruct ();
}
//*************************************************************************************
CBCGPDialog::CBCGPDialog (UINT nIDTemplate, CWnd *pParent/*= NULL*/) : 
				CDialog (nIDTemplate, pParent),
				m_Impl (*this)
{
	CommonConstruct ();
}
//*************************************************************************************
CBCGPDialog::CBCGPDialog (LPCTSTR lpszTemplateName, CWnd *pParentWnd/*= NULL*/) : 
				CDialog(lpszTemplateName, pParentWnd),
				m_Impl (*this)
{
	CommonConstruct ();
}

#pragma warning (default : 4355)

//*************************************************************************************
void CBCGPDialog::CommonConstruct ()
{
	m_hBkgrBitmap = NULL;
	m_sizeBkgrBitmap = CSize (0, 0);
	m_BkgrLocation = (BackgroundLocation) -1;
	m_bAutoDestroyBmp = FALSE;
	m_bIsLocal = FALSE;
	m_pLocaRes = NULL;
}
//*************************************************************************************
void CBCGPDialog::SetBackgroundColor (COLORREF color, BOOL bRepaint)
{
	if (m_brBkgr.GetSafeHandle () != NULL)
	{
		m_brBkgr.DeleteObject ();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush (color);
	}

	if (bRepaint && GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//*************************************************************************************
void CBCGPDialog::SetBackgroundImage (HBITMAP hBitmap, 
								BackgroundLocation location,
								BOOL bAutoDestroy,
								BOOL bRepaint)
{
	if (m_bAutoDestroyBmp && m_hBkgrBitmap != NULL)
	{
		::DeleteObject (m_hBkgrBitmap);
	}

	m_hBkgrBitmap = hBitmap;
	m_BkgrLocation = location;
	m_bAutoDestroyBmp = bAutoDestroy;

	if (hBitmap != NULL)
	{
		BITMAP bmp;
		::GetObject (hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

		m_sizeBkgrBitmap = CSize (bmp.bmWidth, bmp.bmHeight);
	}
	else
	{
		m_sizeBkgrBitmap = CSize (0, 0);
	}

	if (bRepaint && GetSafeHwnd () != NULL)
	{
		Invalidate ();
		UpdateWindow ();
	}
}
//*************************************************************************************
BOOL CBCGPDialog::SetBackgroundImage (UINT uiBmpResId,
									BackgroundLocation location,
									BOOL bRepaint)
{
	HBITMAP hBitmap = NULL;

	if (uiBmpResId != 0)
	{
		hBitmap = ::LoadBitmap (AfxGetResourceHandle (), 
										MAKEINTRESOURCE (uiBmpResId));
		if (hBitmap == NULL)
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	SetBackgroundImage (hBitmap, location, TRUE /* Autodestroy */, bRepaint);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CBCGPDialog, CDialog)
	//{{AFX_MSG_MAP(CBCGPDialog)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DWMCOMPOSITIONCHANGED, OnDWMCompositionChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialog message handlers

void CBCGPDialog::OnActivate(UINT nState, CWnd *pWndOther, BOOL /*bMinimized*/) 
{
	m_Impl.OnActivate (nState, pWndOther);
}
//*************************************************************************************
BOOL CBCGPDialog::OnNcActivate(BOOL bActive) 
{
	m_Impl.OnNcActivate (bActive);

	//-----------------------------------------------------------
	// Do not call the base class because it will call Default()
	// and we may have changed bActive.
	//-----------------------------------------------------------
	return (BOOL) DefWindowProc (WM_NCACTIVATE, bActive, 0L);
}
//**************************************************************************************
BOOL CBCGPDialog::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = TRUE;

	if (m_brBkgr.GetSafeHandle () == NULL && m_hBkgrBitmap == NULL &&
		!IsVisualManagerStyle ())
	{
		bRes = CDialog::OnEraseBkgnd (pDC);
	}
	else
	{
		ASSERT_VALID (pDC);

		CRect rectClient;
		GetClientRect (rectClient);

		if (m_BkgrLocation != BACKGR_TILE || m_hBkgrBitmap == NULL)
		{
			if (m_brBkgr.GetSafeHandle () != NULL)
			{
				pDC->FillRect (rectClient, &m_brBkgr);
			}
			else if (IsVisualManagerStyle ())
			{
				if (!CBCGPVisualManager::GetInstance ()->OnFillDialog (pDC, this, rectClient))
				{
					CDialog::OnEraseBkgnd (pDC);
				}
			}
			else
			{
				CDialog::OnEraseBkgnd (pDC);
			}
		}

		if (m_hBkgrBitmap != NULL)
		{
			ASSERT (m_sizeBkgrBitmap != CSize (0, 0));

			if (m_BkgrLocation != BACKGR_TILE)
			{
				CPoint ptImage = rectClient.TopLeft ();

				switch (m_BkgrLocation)
				{
				case BACKGR_TOPLEFT:
					break;

				case BACKGR_TOPRIGHT:
					ptImage.x = rectClient.right - m_sizeBkgrBitmap.cx;
					break;

				case BACKGR_BOTTOMLEFT:
					ptImage.y = rectClient.bottom - m_sizeBkgrBitmap.cy;
					break;

				case BACKGR_BOTTOMRIGHT:
					ptImage.x = rectClient.right - m_sizeBkgrBitmap.cx;
					ptImage.y = rectClient.bottom - m_sizeBkgrBitmap.cy;
					break;
				}

				pDC->DrawState (ptImage, m_sizeBkgrBitmap, m_hBkgrBitmap, DSS_NORMAL);
			}
			else
			{
				// Tile background image:
				for (int x = rectClient.left; x < rectClient.Width (); x += m_sizeBkgrBitmap.cx)
				{
					for (int y = rectClient.top; y < rectClient.Height (); y += m_sizeBkgrBitmap.cy)
					{
						pDC->DrawState (CPoint (x, y), m_sizeBkgrBitmap, m_hBkgrBitmap, DSS_NORMAL);
					}
				}
			}
		}
	}

	m_Impl.ClearAeroAreas (pDC);
	return bRes;
}
//**********************************************************************************
void CBCGPDialog::OnDestroy() 
{
	if (m_bAutoDestroyBmp && m_hBkgrBitmap != NULL)
	{
		::DeleteObject (m_hBkgrBitmap);
		m_hBkgrBitmap = NULL;
	}

	m_Impl.OnDestroy ();

	CDialog::OnDestroy();
}
//***************************************************************************************
HBRUSH CBCGPDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (m_brBkgr.GetSafeHandle () != NULL || m_hBkgrBitmap != NULL ||
		IsVisualManagerStyle ())
	{
		HBRUSH hbr = m_Impl.OnCtlColor (pDC, pWnd, nCtlColor);
		if (hbr != NULL)
		{
			return hbr;
		}
	}	

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
//**************************************************************************************
BOOL CBCGPDialog::PreTranslateMessage(MSG* pMsg) 
{
	if (m_Impl.PreTranslateMessage (pMsg))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
//**************************************************************************************
void CBCGPDialog::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
	m_Impl.SetActiveMenu (pMenu);
}
//*************************************************************************************
BOOL CBCGPDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (m_Impl.OnCommand (wParam, lParam))
	{
		return TRUE;
	}

	return CDialog::OnCommand(wParam, lParam);
}
//*************************************************************************************
INT_PTR CBCGPDialog::DoModal() 
{
	if (m_bIsLocal)
	{
		m_pLocaRes = new CBCGPLocalResource ();
	}

	return CDialog::DoModal();
}
//*************************************************************************************
void CBCGPDialog::PreInitDialog()
{
	if (m_pLocaRes != NULL)
	{
		delete m_pLocaRes;
		m_pLocaRes = NULL;
	}
}
//*************************************************************************************
void CBCGPDialog::OnSysColorChange() 
{
	CDialog::OnSysColorChange();
	
	if (AfxGetMainWnd () == this)
	{
		globalData.UpdateSysColors ();
	}
}
//*************************************************************************************
void CBCGPDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	
	if (AfxGetMainWnd () == this)
	{
		globalData.OnSettingChange ();
	}
}
//*************************************************************************************
void CBCGPDialog::EnableVisualManagerStyle (BOOL bEnable)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable);
}
//*************************************************************************************
BOOL CBCGPDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (IsVisualManagerStyle ())
	{
		m_Impl.EnableVisualManagerStyle (TRUE);
	}

	if (m_Impl.HasAeroMargins ())
	{
		m_Impl.EnableAero (m_Impl.m_AeroMargins);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//*************************************************************************************
BOOL CBCGPDialog::EnableAero (BCGPMARGINS& margins)
{
	return m_Impl.EnableAero (margins);
}
//*************************************************************************************
void CBCGPDialog::GetAeroMargins (BCGPMARGINS& margins) const
{
	m_Impl.GetAeroMargins (margins);
}
//***************************************************************************
LRESULT CBCGPDialog::OnDWMCompositionChanged(WPARAM,LPARAM)
{
	m_Impl.OnDWMCompositionChanged ();
	return 0;
}
