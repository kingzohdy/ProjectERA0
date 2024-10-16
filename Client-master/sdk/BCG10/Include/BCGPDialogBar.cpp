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

// BCGPDialogBar.cpp : implementation file
//

#include "stdafx.h"
#if _MSC_VER >= 1300
	#include <afxocc.h>
#else
	#include <../src/occimpl.h>
#endif

#include "BCGPDialogBar.h"
#include "BCGPGlobalUtils.h"
#include "BCGPVisualManager.h"

#ifndef _BCGSUITE_
#include "BCGPDockManager.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialogBar

IMPLEMENT_SERIAL(CBCGPDialogBar, CBCGPDockingControlBar, VERSIONABLE_SCHEMA | 1)

#pragma warning (disable : 4355)

CBCGPDialogBar::CBCGPDialogBar() :
	m_Impl (*this)
{
#ifndef _AFX_NO_OCC_SUPPORT
	m_pOccDialogInfo = NULL;
#endif
}

#pragma warning (default : 4355)

CBCGPDialogBar::~CBCGPDialogBar()
{

}

/////////////////////////////////////////////////////////////////////////////
// CBCGPDialogBar message handlers

BOOL CBCGPDialogBar::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, BOOL bHasGripper, 
						   UINT nIDTemplate, UINT nStyle, UINT nID)
{ 
	return Create(lpszWindowName, pParentWnd, bHasGripper, MAKEINTRESOURCE(nIDTemplate), nStyle, nID); 
}
//****************************************************************************************
BOOL CBCGPDialogBar::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, BOOL bHasGripper, 
						   LPCTSTR lpszTemplateName, UINT nStyle, UINT nID, 
						   DWORD dwTabbedStyle, DWORD dwBCGStyle)
{
	m_lpszBarTemplateName = (LPTSTR) lpszTemplateName;

	if (!CBCGPDockingControlBar::Create(lpszWindowName, pParentWnd, CSize (0, 0), bHasGripper, 
						   nID, nStyle, dwTabbedStyle, dwBCGStyle))
	{
		return FALSE;
	}

	m_lpszBarTemplateName = NULL;
	SetOwner (BCGCBProGetTopLevelFrame (this));
	//m_ahSlideMode = BCGP_AHSM_STRETCH;
	return TRUE;
}
//****************************************************************************************
void CBCGPDialogBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CBCGPDockingControlBar::OnUpdateCmdUI (pTarget, bDisableIfNoHndler);
}

BEGIN_MESSAGE_MAP(CBCGPDialogBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CBCGPDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()

//*****************************************************************************************
LRESULT CBCGPDialogBar::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	CBCGPBaseControlBar::HandleInitDialog(wParam, lParam);

#ifndef _AFX_NO_OCC_SUPPORT
	Default();  // allow default to initialize first (common dialogs/etc)

	// create OLE controls
	COccManager* pOccManager = afxOccManager;
	if ((pOccManager != NULL) && (m_pOccDialogInfo != NULL))
	{
		if (!pOccManager->CreateDlgControls(this, m_lpszBarTemplateName,
			m_pOccDialogInfo))
		{
			TRACE (_T("Warning: CreateDlgControls failed during dialog bar init.\n"));
			return FALSE;
		}
	}
#endif //!_AFX_NO_OCC_SUPPORT

	if (IsVisualManagerStyle ())
	{
		m_Impl.EnableVisualManagerStyle (TRUE);
		m_Impl.m_bTransparentStaticCtrls = FALSE;
	}

	return TRUE;
}

#ifndef _AFX_NO_OCC_SUPPORT
//*****************************************************************************************
BOOL CBCGPDialogBar::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo)
{
	m_pOccDialogInfo = pOccDialogInfo;
	return TRUE;
}
#endif //!_AFX_NO_OCC_SUPPORT
//*****************************************************************************************
BOOL CBCGPDialogBar::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);

	if (IsVisualManagerStyle () &&
		CBCGPVisualManager::GetInstance ()->OnFillDialog (pDC, this, rectClient))
	{
		return TRUE;
	}

	pDC->FillRect (rectClient, &globalData.brBtnFace);
	return TRUE;
}
//*****************************************************************************************
void CBCGPDialogBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CPoint ptScr = point;
	ClientToScreen (&ptScr);

	int nHitTest = HitTest (ptScr, TRUE);

	if (nHitTest == HTCAPTION)
	{
		CBCGPDockingControlBar::OnLButtonDblClk(nFlags, point);
	}
	else
	{
		CWnd::OnLButtonDblClk(nFlags, point);
	}
}
//*****************************************************************************************
void CBCGPDialogBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint ptScr = point;
	ClientToScreen (&ptScr);

	int nHitTest = HitTest (ptScr, TRUE);

	if (nHitTest == HTCAPTION || nHitTest == HTCLOSE_BCG || 
		nHitTest == HTMAXBUTTON || nHitTest == HTMINBUTTON)
	{
		CBCGPDockingControlBar::OnLButtonDown(nFlags, point);
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
}
//*****************************************************************************************
void CBCGPDialogBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CBCGPDockingControlBar::OnWindowPosChanging(lpwndpos);
	
	if (!CanBeResized ())
	{
		CSize sizeMin; 
		GetMinSize (sizeMin);
		
		if (IsHorizontal () && lpwndpos->cy < sizeMin.cy)
		{
			lpwndpos->cy = sizeMin.cy;
			lpwndpos->hwndInsertAfter = HWND_BOTTOM;
		}
		else if (!IsHorizontal () && lpwndpos->cx < sizeMin.cx)
		{
			lpwndpos->cx = sizeMin.cx;
			lpwndpos->hwndInsertAfter = HWND_BOTTOM;
		}
	}
}
//*****************************************************************************************
void CBCGPDialogBar::EnableVisualManagerStyle (BOOL bEnable)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable);
}
//***************************************************************************************
HBRUSH CBCGPDialogBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (IsVisualManagerStyle ())
	{
		HBRUSH hbr = m_Impl.OnCtlColor (pDC, pWnd, nCtlColor);
		if (hbr != NULL)
		{
			return hbr;
		}
	}	

	return CBCGPDockingControlBar::OnCtlColor(pDC, pWnd, nCtlColor);
}
//***************************************************************************************
void CBCGPDialogBar::OnDestroy() 
{
	m_Impl.OnDestroy ();
	CBCGPDockingControlBar::OnDestroy();
}
//***************************************************************************************
void CBCGPDialogBar::OnEraseNCBackground (CDC* pDC, CRect rectBar)
{
#ifndef _BCGSUITE_
	if (IsVisualManagerStyle () &&
		CBCGPVisualManager::GetInstance ()->OnFillDialog (pDC, this, rectBar))
	{
		return;
	}

	CBCGPDockingControlBar::OnEraseNCBackground (pDC, rectBar);
#endif
}
