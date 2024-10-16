// BCGPDockingControlBar.cpp : implementation file
//

#include "stdafx.h"

#include "BCGGlobals.h"
#include "BCGPGlobalUtils.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPMultiMiniFrameWnd.h"
#include "BCGPBaseControlBar.h"
#include "BCGPDockBarRow.h"
#include "BCGPTabbedControlBar.h"
#include "BCGPDrawManager.h"
#include "BCGPAutoHideButton.h"
#include "BCGPAutoHideToolBar.h"
#include "BCGPAutoHideDockBar.h"
#include "BCGPSlider.h"
#include "BCGPLocalResource.h"
#include "BCGProRes.h"
#include "BCGPDockingControlBar.h"
#include "BCGPBarContainerManager.h"
#include "BCGPOutlookBar.h"
#include "BCGPMultiMiniFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int  CBCGPDockingControlBar::m_nTimeOutBeforeAutoHide	= 700;
int  CBCGPDockingControlBar::m_nSlideDefaultTimeOut		= 1;
BOOL CBCGPDockingControlBar::m_bHideInAutoHideMode		= FALSE;
int  CBCGPDockingControlBar::m_nSlideSteps				= 12;


static int g_nCloseButtonMargin = 1;
static int g_nCaptionVertMargin = 2;
static int g_nCaptionHorzMargin = 2;

CSize CBCGPDockingControlBar::m_sizeDragSencitivity = CSize (GetSystemMetrics (SM_CXDRAG), 
															 GetSystemMetrics (SM_CYDRAG));

BOOL CBCGPDockingControlBar::m_bCaptionText = FALSE;
BOOL CBCGPDockingControlBar::m_bHideDisabledButtons = TRUE;
BOOL CBCGPDockingControlBar::m_bDisableAnimation = FALSE;

IMPLEMENT_SERIAL(CBCGPDockingControlBar, CBCGPControlBar, VERSIONABLE_SCHEMA | 2)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPDockingControlBar::CBCGPDockingControlBar()
{
	m_bPrepareToFloat = false;	
	m_bReadyToFloat = false;

	m_pTabbedControlBarRTC = RUNTIME_CLASS (CBCGPTabbedControlBar);

	m_hDefaultSlider = NULL;
	m_cyGripper = 0;
	m_bHasGripper = FALSE;
	m_nBorderSize = 0;
	m_dwSCBStyle = 0;
	m_bMaximized = FALSE;
	m_bActive = FALSE;

	m_bEnableAutoHideAll = TRUE;

	m_bPinState = FALSE;
	m_nAutoHideConditionTimerID = 0;
	m_nSlideTimer = 0;
	m_nSlideStep = 0;
	m_nSlideDelta = 0;
	m_pAutoHideButton = NULL;
	m_pAutoHideBar = NULL;

	m_ahSlideMode = CBCGPDockManager::m_ahSlideModeGlobal;

	m_bIsSliding = FALSE;
	m_bIsHiding = FALSE;
	m_bIsResizing = FALSE;

	m_nLastPercent = 100;

	m_rectRedraw.SetRectEmpty ();
	m_rectRestored.SetRectEmpty ();

	m_nHot = HTNOWHERE;
	m_nHit = HTNOWHERE;
	m_bCaptionButtonsCaptured = FALSE;

	m_hRestoredDefaultSlider = NULL;
}

CBCGPDockingControlBar::~CBCGPDockingControlBar()
{
}

BEGIN_MESSAGE_MAP(CBCGPDockingControlBar, CBCGPControlBar)
	//{{AFX_MSG_MAP(CBCGPDockingControlBar)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_NCMOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETTINGCHANGE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnNeedTipText)
END_MESSAGE_MAP()

//***********************************************************************
BOOL CBCGPDockingControlBar::Create(LPCTSTR lpszCaption, 
									CWnd* pParentWnd, 
									const RECT& rect, 
									BOOL bHasGripper, 
									UINT nID, 
									DWORD dwStyle, 
									DWORD dwTabbedStyle,
									DWORD dwBCGStyle,
									CCreateContext* pContext)
{
	ASSERT_VALID (this);
	return CBCGPDockingControlBar::CreateEx (0, lpszCaption, pParentWnd, rect, 
											 bHasGripper, nID, dwStyle, dwTabbedStyle, 
											 dwBCGStyle, pContext);
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::Create(LPCTSTR lpszWindowName, 
									CWnd* pParentWnd,
									CSize sizeDefault, 
									BOOL bHasGripper,
									UINT nID, 
									DWORD dwStyle, 
									DWORD dwTabbedStyle, 
									DWORD dwBCGStyle)
{
	ASSERT_VALID (this);
	CRect rect (0, 0, sizeDefault.cx, sizeDefault.cy);
	return CBCGPDockingControlBar::CreateEx (0, lpszWindowName, pParentWnd, rect, 
											 bHasGripper, nID, dwStyle, dwTabbedStyle, 
											 dwBCGStyle, NULL);
}
//***********************************************************************
BOOL CBCGPDockingControlBar::CreateEx (DWORD dwStyleEx, 
									   LPCTSTR lpszCaption, 
									   CWnd* pParentWnd, 
									   const RECT& rect, 
									   BOOL bHasGripper, 
									   UINT nID, 
									   DWORD dwStyle, 
									   DWORD dwTabbedStyle, 
									   DWORD dwBCGStyle,
									   CCreateContext* pContext)
{
	ASSERT_VALID (this);

	if (dwStyle & CBRS_FLOAT_MULTI)
	{
		m_pMiniFrameRTC = RUNTIME_CLASS (CBCGPMultiMiniFrameWnd);
	}

	if (dwTabbedStyle & CBRS_BCGP_OUTLOOK_TABS)
	{
		m_pTabbedControlBarRTC = RUNTIME_CLASS (CBCGPOutlookBar);
	}
	else if (dwTabbedStyle & CBRS_BCGP_REGULAR_TABS)
	{
		m_pTabbedControlBarRTC = RUNTIME_CLASS (CBCGPTabbedControlBar);
	}

	if (dwStyle & WS_CAPTION || bHasGripper)
	{
		m_bHasGripper = bHasGripper = TRUE;
		dwStyle &= ~WS_CAPTION;
	}


	if (!CBCGPControlBar::CreateEx (dwStyleEx, NULL, dwStyle, rect, pParentWnd, nID, 
											 dwBCGStyle, pContext))
	{
		return FALSE;
	}
	
	m_rectRestored = rect;

	if (m_sizeDialog != CSize (0, 0))
	{
		m_rectRestored.right = m_rectRestored.left + m_sizeDialog.cx;
		m_rectRestored.bottom = m_rectRestored.top + m_sizeDialog.cy;
	}

	SetBarAlignment (dwStyle & CBRS_ALIGN_ANY);
	EnableGripper (bHasGripper);
	
	if (lpszCaption != NULL)
	{
		SetWindowText (lpszCaption);
	}

	return TRUE;
}
//***********************************************************************
void CBCGPDockingControlBar::EnableGripper (BOOL bEnable)
{
	if (bEnable && m_bHasGripper)
	{
		m_cyGripper = globalData.GetTextHeight () + g_nCaptionVertMargin * 2 + 1;
	}
	else
	{
		m_cyGripper = 0;
	}

	SetWindowPos (NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | 
									SWP_NOACTIVATE | SWP_FRAMECHANGED);
}
//***********************************************************************
int CBCGPDockingControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetCaptionButtons ();

	m_ToolTip.Create (NULL, TTS_ALWAYSTIP);
	m_ToolTip.Activate (TRUE);
	if(globalData.m_nMaxToolTipWidth != -1)
	{
		m_ToolTip.SetMaxTipWidth(globalData.m_nMaxToolTipWidth);
	}

	for (int i = 0; i < CBCGSIZINGCONTROLBAR_BUTTONS_NUM; i ++)
	{
		CBCGPLocalResource locaRes;

		CRect rectDummy;
		rectDummy.SetRectEmpty ();

		CString strTooltip;
		strTooltip.LoadString (i == 0 ? IDS_BCGBARRES_CLOSEBAR : IDS_BCGBARRES_AUTOHIDEBAR);

		m_ToolTip.AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, i + 1);
	}

	return 0;
}
//***********************************************************************
BOOL CBCGPDockingControlBar::IsDocked () const
{
	ASSERT_VALID (this);
	CBCGPMiniFrameWnd* pParent = GetParentMiniFrame ();
	
	if (pParent != NULL && pParent->GetControlBarCount () == 1)
	{
		return FALSE;
	}

	return TRUE;
}
//***********************************************************************
void CBCGPDockingControlBar::OnAfterDock  (CBCGPBaseControlBar* /*pBar*/, LPCRECT /*lpRect*/, BCGP_DOCK_METHOD /*dockMethod*/) 
{
	if (!CBCGPDockManager::m_bRestoringDockState)
	{
		SetFocus ();
	}
}
//***********************************************************************
void CBCGPDockingControlBar::OnBeforeChangeParent (CWnd* pWndNewParent, BOOL bDelay)
{
	ASSERT_VALID (this);

	if (pWndNewParent != NULL)
	{
		// is being floated or tabbed
		if (pWndNewParent->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)) ||
			pWndNewParent->IsKindOf (RUNTIME_CLASS (CBCGPTabWnd)))
		{
			UnDockControlBar (bDelay);
		}

		CBCGPControlBar::OnBeforeChangeParent (pWndNewParent);
	}
}
//***********************************************************************
void CBCGPDockingControlBar::RemoveFromDefaultSlider ()
{
	ASSERT_VALID (this);

	if (m_hDefaultSlider != NULL)
	{
		// slider will be deleted here (by delete this) if it was a last
		// control bar registered with the slider
		SetDefaultSlider (NULL);
	}
}
//***********************************************************************
void CBCGPDockingControlBar::OnAfterChangeParent  (CWnd* pWndOldParent)
{
	ASSERT_VALID (this);
	CBCGPControlBar::OnAfterChangeParent (pWndOldParent);

	CBCGPMiniFrameWnd* pMiniFrameParent = GetParentMiniFrame ();
	if (pMiniFrameParent != NULL)
	{
		pMiniFrameParent->AddRemoveBarFromGlobalList (this, TRUE);
	}
}
//***********************************************************************
void CBCGPDockingControlBar::RecalcLayout ()
{
	ASSERT_VALID (this);

	CBCGPControlBar::RecalcLayout ();

	
}
//***********************************************************************
void CBCGPDockingControlBar::UpdateTooltips ()
{
    CRect rcBar;
    GetWindowRect(rcBar);
	ScreenToClient (rcBar);

	for (int i = 0; i < m_arrButtons.GetSize () && i < m_ToolTip.GetToolCount( ); i ++)
	{
		CBCGPCaptionButton* pbtn = m_arrButtons [i];
		ASSERT_VALID (pbtn);

		if (m_ToolTip.GetSafeHwnd () != NULL)
		{
			CRect rectTT = pbtn->GetRect ();
			rectTT.OffsetRect (rcBar.TopLeft());
			m_ToolTip.SetToolRect (this, i + 1, rectTT);
		}
	}
}
//***********************************************************************
void CBCGPDockingControlBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	ASSERT_VALID (this);
	CBCGPControlBar::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (IsFloating ())
	{
		for (int i = 0; i < m_arrButtons.GetSize (); i ++)
		{
			CBCGPCaptionButton* pbtn = m_arrButtons [i];
			ASSERT_VALID (pbtn);

			pbtn->m_bHidden = TRUE;
		}

		return;
	}

	CRect rcClient = lpncsp->rgrc[0];
	rcClient.DeflateRect (0, m_cyGripper, 0, 0);

	// "hide" and "expand" buttons positioning:
	CSize sizeButton = CBCGPCaptionButton::GetSize ();
    CPoint ptOrgBtnRight = CPoint (rcClient.right - sizeButton.cx - g_nCaptionHorzMargin,
		rcClient.top - m_cyGripper - m_nBorderSize + (m_cyGripper - sizeButton.cy) / 2);
    CPoint ptOrgBtnLeft = CPoint (rcClient.left + g_nCaptionHorzMargin, ptOrgBtnRight.y);

	CRect rcBar;
    GetWindowRect(rcBar);
	ScreenToClient (rcBar);

	BOOL bHidePinBtn = TRUE;

	CWnd* pParentWnd = GetParent ();

	if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
	{
		pParentWnd = pParentWnd->GetParent ();
	}

	CBCGPSlider* pDefaultSlider = GetDefaultSlider ();

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (pParentWnd);
	if (pDockManager != NULL &&
		pDefaultSlider != NULL && 
		(pDefaultSlider->GetCurrentAlignment () & 
		 pDockManager->GetEnabledAutoHideAlignment ()) &&
		 CanAutoHide ())
	{
       bHidePinBtn = FALSE;
	}

	if (m_cyGripper > 0)
	{
		for (int i = 0; i < m_arrButtons.GetSize (); i ++)
		{
			CBCGPCaptionButton* pbtn = m_arrButtons [i];
			ASSERT_VALID (pbtn);

			UINT unHit = pbtn->GetHit ();

			BOOL bHide = FALSE;
			if (m_bHideDisabledButtons)
			{
				bHide = bHidePinBtn && unHit == HTMAXBUTTON || 
						!CanBeClosed () && unHit == HTCLOSE_BCG;
			}

			pbtn->m_bFocused = pbtn->m_bPushed = FALSE;

			if (pbtn->m_bLeftAlign)
			{
				pbtn->Move (ptOrgBtnLeft - CRect (lpncsp->rgrc[0]).TopLeft (), bHide);

				if (!bHide)
				{
					ptOrgBtnLeft.Offset (sizeButton.cx + 2, 0);
				}
			}
			else
			{
				pbtn->Move (ptOrgBtnRight - CRect (lpncsp->rgrc[0]).TopLeft (), bHide);

				if (!bHide)
				{
					ptOrgBtnRight.Offset (- sizeButton.cx - 2, 0);
				}
			}
		}

		// Hide left aligned buttons if there is no room for them:
		for (i = 0; i < m_arrButtons.GetSize (); i ++)
		{
			CBCGPCaptionButton* pbtn = m_arrButtons [i];
			ASSERT_VALID (pbtn);

			if (pbtn->m_bLeftAlign)
			{
				pbtn->m_bHidden = CRect (lpncsp->rgrc[0]).left + pbtn->GetRect ().left >= ptOrgBtnRight.x;
			}
		}
	}
	else
	{
		for (int i = 0; i < m_arrButtons.GetSize (); i ++)
		{
			CBCGPCaptionButton* pbtn = m_arrButtons [i];
			ASSERT_VALID (pbtn);

			pbtn->m_bHidden = TRUE;
		}
	}

	rcClient.right = max(rcClient.right, rcClient.left);
	rcClient.bottom = max(rcClient.bottom, rcClient.top);

    lpncsp->rgrc[0] = rcClient;
	
	UpdateTooltips ();
}
//***********************************************************************
void CBCGPDockingControlBar::OnNcPaint() 
{
	if (m_bMultiThreaded)
	{
		g_cs.Lock ();
	}

	ASSERT_VALID (this);
	
	// get window DC that is clipped to the non-client area
    CWindowDC dcPaint (this);

	CRect rectUpd;
	GetUpdateRect (rectUpd);

    CRect rcClient, rcBar;
    GetClientRect(rcClient);
    ClientToScreen(rcClient);
    GetWindowRect(rcBar);

    rcClient.OffsetRect(-rcBar.TopLeft());
    rcBar.OffsetRect(-rcBar.TopLeft());

	if (rectUpd.top > rcClient.top)
	{
		if (m_bMultiThreaded)
		{
			g_cs.Unlock ();
		}

		return;
	}

	CDC*		pDC = &dcPaint;
	BOOL		m_bMemDC = FALSE;
	CDC			dcMem;
	CBitmap		bmp;
	CBitmap*	pOldBmp = NULL;

	if (dcMem.CreateCompatibleDC (&dcPaint) &&
		bmp.CreateCompatibleBitmap (&dcPaint, rcBar.Width (),
								  rcBar.Height ()))
	{
		//-------------------------------------------------------------
		// Off-screen DC successfully created. Better paint to it then!
		//-------------------------------------------------------------
		m_bMemDC = TRUE;
		pOldBmp = dcMem.SelectObject (&bmp);
		pDC = &dcMem;
	}

    // client area is not our bussiness :)
    dcPaint.ExcludeClipRect(rcClient);

	CRgn rgn;
	if (!m_rectRedraw.IsRectEmpty ())
	{
		rgn.CreateRectRgnIndirect (m_rectRedraw);
		dcPaint.SelectClipRgn (&rgn);
	}

    // erase parts not drawn
    dcPaint.IntersectClipRect(rcBar);

    // erase NC background the hard way
	CBCGPVisualManager::GetInstance ()->OnFillBarBackground (pDC, this, rcBar, rcBar, 
		TRUE /* NC area */);

    if (m_cyGripper && !IsFloating())
	{
		// Paint caption and buttons:
		CRect rectCaption;
		
		GetWindowRect (&rectCaption);
		ScreenToClient (&rectCaption);

		rectCaption.OffsetRect (-rectCaption.left, -rectCaption.top);
		rectCaption.DeflateRect (0, 1);

		rectCaption.left = rcClient.left;
		rectCaption.top --;
		rectCaption.bottom = rectCaption.top + m_cyGripper - 2;

		DrawCaption (pDC, rectCaption);

		for (int i = 0; i < m_arrButtons.GetSize (); i ++)
		{
			CBCGPCaptionButton* pbtn = m_arrButtons [i];
			ASSERT_VALID (pbtn);

			BOOL bIsMax = FALSE;
			if (pbtn->GetHit () == HTMAXBUTTON)
			{
				bIsMax = m_bPinState;
			}

			pbtn->OnDraw (pDC, m_bActive, IsHorizontal (), bIsMax);
			pbtn->m_clrForeground = (COLORREF)-1;
		}
	}

	if (m_bMemDC)
	{
		//--------------------------------------
		// Copy the results to the on-screen DC:
		//-------------------------------------- 
		dcPaint.BitBlt (rcBar.left, rcBar.top, rcBar.Width(), rcBar.Height(),
					   &dcMem, rcBar.left, rcBar.top, SRCCOPY);

		dcMem.SelectObject(pOldBmp);
	}

	dcPaint.SelectClipRgn (NULL);

	if (m_bMultiThreaded)
	{
		g_cs.Unlock ();
	}
}
//***********************************************************************
void CBCGPDockingControlBar::OnDrawDragRect (LPCRECT lprectNew, LPCRECT lprectOld)
{
	ASSERT_VALID (this);
	CWindowDC dcWnd (GetDesktopWindow ());
	dcWnd.DrawDragRect (lprectNew, CSize (1, 1), lprectOld, CSize (1, 1));
}
//***********************************************************************
UINT CBCGPDockingControlBar::OnNcHitTest(CPoint point) 
{
	ASSERT_VALID (this);
	UINT nHitTest = HitTest (point);
	if (nHitTest != HTERROR)
	{
		return nHitTest;
	}
	return CBCGPControlBar::OnNcHitTest(point);
}
//***********************************************************************
int CBCGPDockingControlBar::HitTest (CPoint point, BOOL bDetectCaption)
{
	ASSERT_VALID (this);
	CRect rectWnd;
	GetWindowRect (&rectWnd);

	if (!rectWnd.PtInRect (point))
	{
		return HTNOWHERE;
	}

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetDockSite ());
	ASSERT (pDockManager != NULL || globalUtils.m_bDialogApp);	

	// should return hite test of client or caption only in the lock update mode
	if (pDockManager != NULL && !pDockManager->m_bLockUpdate)
	{
		for (int i = 0; i < m_arrButtons.GetSize (); i ++)
		{
			CBCGPCaptionButton* pbtn = m_arrButtons [i];
			ASSERT_VALID (pbtn);

			CRect rc = pbtn->GetRect();
			rc.OffsetRect(rectWnd.TopLeft());
			if (rc.PtInRect(point))
			{
				return pbtn->GetHit ();
			}
		}
	}

	CRect rectClient;
	GetClientRect (&rectClient);
	ClientToScreen (&rectClient);

	if (rectClient.PtInRect (point))
	{
		return HTCLIENT;
	}

	if (IsDocked ())
	{
		CRect rect;
		int nBorderWidth  = 0;
		int nBorderHeight = 1;
		// caption
		rect.SetRect (rectWnd.left + nBorderWidth, rectWnd.top + nBorderHeight, 
							 rectWnd.right - nBorderWidth, 
							 rectWnd.top + nBorderHeight + m_cyGripper);
		if (rect.PtInRect (point))
		{
			return bDetectCaption ? HTCAPTION : HTCLIENT;
		}
	}

	return HTERROR;
}
//***********************************************************************
CSize CBCGPDockingControlBar::CalcFixedLayout(BOOL /*bStretch*/, BOOL /*bHorz*/)
{
	ASSERT_VALID (this);
	CRect rectWnd;
	GetWindowRect (&rectWnd);
	CSize size = rectWnd.Size ();
	return size;
}
//***********************************************************************
void CBCGPDockingControlBar::OnPaint() 
{
	ASSERT_VALID (this);
	CPaintDC dc(this); // device context for painting
}
//***********************************************************************
BCGP_CS_STATUS CBCGPDockingControlBar::IsChangeState (int nOffset, 
													CBCGPBaseControlBar** ppTargetBar) const
{
	ASSERT_VALID (this);
	ASSERT (ppTargetBar != NULL);

	CPoint ptMouse;
	GetCursorPos (&ptMouse);

	CWnd* pParentWnd = GetParent ();

	if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
	{
		CBCGPMiniFrameWnd* pMiniFrame = DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pParentWnd);
		pParentWnd = pMiniFrame->GetParent ();
	}

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (pParentWnd);
	
	if (pDockManager == NULL)
	{
		return CS_NOTHING;
	}

	return pDockManager->DetermineControlBarAndStatus (ptMouse, nOffset, 
				GetEnabledAlignment (), ppTargetBar, this, this);
}
//***********************************************************************
void CBCGPDockingControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT_VALID (this);

	if (m_nHot != HTNOWHERE)
	{
		CBCGPCaptionButton* pBtn = FindButtonByHit (m_nHot);
		if (pBtn != NULL)
		{
			m_nHit = m_nHot;
			pBtn->m_bPushed = TRUE;
			RedrawButton (pBtn);
			return;
		}
	}
	else
	{
		CWnd* pWndChild = GetWindow (GW_CHILD);
		CWnd* pWndFirstChild = NULL;
		int nCount = 0;

		while (pWndChild != NULL)
		{
			pWndFirstChild = pWndChild;
			pWndChild = pWndChild->GetNextWindow ();
			nCount++;
		}

		if (nCount == 1)
		{
			pWndFirstChild->SetFocus ();
		}
	}

	if (!IsAutoHideMode () && !IsTabbed ())
	{
		if (CanFloat ())
		{
			m_bPrepareToFloat = true;
		}
		CBCGPControlBar::OnLButtonDown(nFlags, point);
	}
	
	SetFocus ();
}
//***********************************************************************
void CBCGPDockingControlBar::StoreRecentDockInfo ()
{
	CBCGPMiniFrameWnd* pParentMiniFrame = GetParentMiniFrame ();

	CBCGPDockingControlBar* pBarToSave = this;

	if (IsTabbed ())
	{
		CBCGPBaseTabWnd* pTabWnd = 
			DYNAMIC_DOWNCAST (CBCGPBaseTabWnd, GetParent ());
		if (pTabWnd != NULL)
		{
			pBarToSave = 
				DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pTabWnd->GetParent ());	
		}
	}			

	CBCGPSlider* pDefaultSlider = pBarToSave->GetDefaultSlider ();

	if (pParentMiniFrame != NULL)
	{
		pParentMiniFrame->StoreRecentDockInfo (pBarToSave);
	}
	else if (pDefaultSlider != NULL)
	{
		pDefaultSlider->StoreRecentDockInfo (pBarToSave);
	}
}
//***********************************************************************
void CBCGPDockingControlBar::StoreRecentTabRelatedInfo ()
{
	if (!IsTabbed ())
	{
		return;
	}

	CBCGPDockingControlBar* pParentTabbedBar = NULL;

	CBCGPBaseTabWnd* pTabWnd = 
			DYNAMIC_DOWNCAST (CBCGPBaseTabWnd, GetParent ());
	if (pTabWnd != NULL)
	{
		pParentTabbedBar = 
			DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pTabWnd->GetParent ());	
	}

	if (pParentTabbedBar == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	CBCGPMiniFrameWnd* pParentMiniFrame = GetParentMiniFrame ();
	CBCGPSlider* pDefaultSlider = pParentTabbedBar->GetDefaultSlider ();

	if (pParentMiniFrame != NULL)
	{
		pParentMiniFrame->StoreRecentTabRelatedInfo (this, pParentTabbedBar);
	}
	else if (pDefaultSlider != NULL)
	{
		pDefaultSlider->StoreRecentTabRelatedInfo (this, pParentTabbedBar); 
	}
}
//***********************************************************************
void CBCGPDockingControlBar::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus ();
	CBCGPPopupMenu::UpdateAllShadows ();
	
	CBCGPControlBar::OnRButtonDown(nFlags, point);
}
//***********************************************************************
void CBCGPDockingControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	ASSERT_VALID (this);
	CPoint ptMouse;
	GetCursorPos (&ptMouse);

	if (GetDockMode () == DT_IMMEDIATE)
	{
		if ((!m_bCaptured && GetCapture () == this ||
			m_bCaptured && GetCapture () != this ||
			(GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0)
			&& !m_bCaptionButtonsCaptured)
		{
			ReleaseCapture ();
			m_bCaptured = false;
			m_bPrepareToFloat = false;
		}
		if (m_bPrepareToFloat)
		{
			CRect rectBar;
			GetWindowRect (rectBar);

			if (!m_bReadyToFloat)
			{
				m_bReadyToFloat = rectBar.PtInRect (ptMouse) == TRUE;
			}

			CRect rectLast = m_rectDragImmediate;

			CPoint ptOffset = ptMouse - m_dragFrameImpl.m_ptHot;
			m_dragFrameImpl.m_ptHot = ptMouse;

			UpdateVirtualRect (ptOffset);
		

			if (IsTabbed ())
			{
				CBCGPBaseTabWnd* pParentTab = 
						DYNAMIC_DOWNCAST (CBCGPBaseTabWnd, GetParent ());
				pParentTab->GetParent ()->GetWindowRect (&rectBar);
			}

			rectBar.InflateRect (m_sizeDragSencitivity.cx, 
								 m_sizeDragSencitivity.cy);


			if (!rectBar.PtInRect (ptMouse) && m_bReadyToFloat)
			{
				if (IsTabbed ())
				{
					CBCGPBaseTabWnd* pParentTab = 
						DYNAMIC_DOWNCAST (CBCGPBaseTabWnd, GetParent ());
					if (pParentTab != NULL)
					{
						pParentTab->DetachTab (DM_MOUSE);
					}
				}
				else
				{
					FloatControlBar (m_recentDockInfo.m_rectRecentFloatingRect, DM_MOUSE);			
				}
				m_bPrepareToFloat = false;
				m_bReadyToFloat = false;
			}
			return;
		}
	}
	else if (GetDockMode () == DT_STANDARD && m_bPrepareToFloat)
	{
		CBCGPControlBar::OnMouseMove(nFlags, point);
		return;
	}

	CPoint ptScreen = point;
	ClientToScreen (&ptScreen);

	OnTrackCaptionButtons (ptScreen);
}
//***********************************************************************
void CBCGPDockingControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ASSERT_VALID (this);

	if (m_bPrepareToFloat)
	{
		m_bPrepareToFloat = false;
	}

	if (m_nHit != HTNOWHERE)
	{
		CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetDockSite ());
		ASSERT (pDockManager != NULL || globalUtils.m_bDialogApp);		

		UINT nHot = m_nHot;
		UINT nHit = m_nHit;

		StopCaptionButtonsTracking ();

		CBCGPSlider* pDefaultSlider = GetDefaultSlider ();

		if (nHot == nHit)
		{
			switch (nHit)
			{
			case HTCLOSE_BCG:
				{
					BOOL bCanClose = TRUE;
					CFrameWnd* pWndMain = BCGCBProGetTopLevelFrame (this);
					if (pWndMain != NULL)
					{
						CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pWndMain);
						if (pMainFrame != NULL)
						{
							bCanClose = pMainFrame->OnCloseDockingBar (this);
						}
						else	// Maybe, SDI frame...
						{
							CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pWndMain);
							if (pFrame != NULL)
							{
								bCanClose = pFrame->OnCloseDockingBar (this);
							}
							else	// Maybe, OLE frame...
							{
								CBCGPOleIPFrameWnd* pOleFrame = 
									DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, pWndMain);
								if (pOleFrame != NULL)
								{
									bCanClose = pOleFrame->OnCloseDockingBar (this);
								}
								else
								{
									CBCGPOleDocIPFrameWnd* pOleDocFrame = 
										DYNAMIC_DOWNCAST (CBCGPOleDocIPFrameWnd, pWndMain);
									if (pOleDocFrame != NULL)
									{
										bCanClose = pOleDocFrame->OnCloseDockingBar (this);
									}
								}
							}
						}
					}

					if (bCanClose)
					{
						OnPressCloseButton ();
					}
					break;
				}

			case HTMAXBUTTON:
				if (GetAsyncKeyState (VK_CONTROL) && IsAutohideAllEnabled ())
				{
					m_pDockSite->SetRedraw (FALSE);
					if (!m_bPinState)
					{
						CObList lstBars;
						pDefaultSlider->GetControlBars (lstBars);
						
						for (POSITION pos = lstBars.GetHeadPosition (); pos != NULL;)
						{
							CBCGPDockingControlBar* pBar = 
								DYNAMIC_DOWNCAST (CBCGPDockingControlBar, lstBars.GetNext (pos));
							if (pBar->IsAutohideAllEnabled ())
							{
								pBar->SetAutoHideMode (TRUE, 
									pDefaultSlider->GetCurrentAlignment (), NULL, FALSE);	
							}
						}
					}
					else
					{
						CBCGPAutoHideDockBar* pParentDockBar = 
							DYNAMIC_DOWNCAST (CBCGPAutoHideDockBar, m_pAutoHideBar->GetParentDockBar ());

						if (pParentDockBar != NULL)
						{
							pParentDockBar->UnSetAutoHideMode (NULL);
						}
					}

					m_pDockSite->SetRedraw (TRUE);
					m_pDockSite->RedrawWindow (NULL, NULL, 
							RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
				}
				else
				{
					if (pDockManager != NULL && pDefaultSlider != NULL && 
						(pDefaultSlider->GetCurrentAlignment () & 
						pDockManager->GetEnabledAutoHideAlignment ()))
					{
						SetAutoHideMode (!m_bPinState, 
							pDefaultSlider->GetCurrentAlignment ());
					}
				}
				return;				

			default:
				OnPressButtons (nHit);
			}
		}

		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	CBCGPControlBar::OnLButtonUp(nFlags, point);
}
//***********************************************************************
void CBCGPDockingControlBar::OnPressCloseButton ()
{
	if (IsAutoHideMode ())
	{
		m_bPinState	= FALSE;	
		SetAutoHideMode (m_bPinState, GetCurrentAlignment ());
	}
	ShowControlBar (FALSE, FALSE, FALSE);
	AdjustDockingLayout ();
}
//***********************************************************************
void CBCGPDockingControlBar::EnterDragMode (BOOL bChangeHotPoint)
{
	m_bPrepareToFloat = true;
	CBCGPControlBar::EnterDragMode (bChangeHotPoint);
}
//***********************************************************************
CBCGPAutoHideToolBar* CBCGPDockingControlBar::SetAutoHideMode (BOOL bMode, DWORD dwAlignment, 
																CBCGPAutoHideToolBar* pCurrAutoHideBar, 
																BOOL bUseTimer)
{
	ASSERT_VALID (this);
	ASSERT (dwAlignment & CBRS_ALIGN_ANY);

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetDockSite ());
	ASSERT_VALID (pDockManager);

	if (bMode)
	{
		m_bPinState = TRUE;

		CRect rectBeforeUndock;
		GetWindowRect (rectBeforeUndock);
		GetDockSite ()->ScreenToClient (rectBeforeUndock);

		StoreRecentDockInfo ();

		// set autohide mode
		UnDockControlBar (FALSE);

		CBCGPSlider* pDefaultSlider = GetDefaultSlider ();
		ASSERT (pDefaultSlider == NULL);
		pDefaultSlider = CreateDefaultSlider (dwAlignment, GetDockSite ());

		if (pDefaultSlider == NULL)
		{
			TRACE0 ("Failed to create default slider\n");
			DockControlBar (this, NULL, DM_DBL_CLICK);
			return NULL;
		}

		m_hDefaultSlider = pDefaultSlider->m_hWnd;

		pDefaultSlider->SetAutoHideMode (TRUE);
		pDefaultSlider->AddControlBar (this);

		SetBarAlignment (dwAlignment);
		pDefaultSlider->SetBarAlignment (dwAlignment);
		
		pCurrAutoHideBar = pDockManager->AutoHideBar (this, pCurrAutoHideBar);

		if (IsBarVisible ())
		{
			pDefaultSlider->RedrawWindow (NULL, NULL,
					RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
			RedrawWindow (NULL, NULL,
					RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

			GetDockSite ()->RedrawWindow (rectBeforeUndock,  NULL,
							RDW_INVALIDATE | RDW_UPDATENOW |  RDW_ALLCHILDREN);
		}
		else
		{
			ShowWindow (SW_SHOW);
		}

		if (bUseTimer)
		{
			m_nAutoHideConditionTimerID = SetTimer (ID_CHECK_AUTO_HIDE_CONDITION, 
													m_nTimeOutBeforeAutoHide, NULL);	
		}
		else
		{
			Slide (FALSE, FALSE);
		}
	}
	else if (m_pAutoHideBar != NULL)
	{
		CBCGPAutoHideDockBar* pParentDockBar = 
			DYNAMIC_DOWNCAST (CBCGPAutoHideDockBar, m_pAutoHideBar->GetParentDockBar ());

		if (pParentDockBar != NULL)
		{
			pParentDockBar->UnSetAutoHideMode (m_pAutoHideBar);
		}
	}

	return pCurrAutoHideBar;
}
//***********************************************************************
void CBCGPDockingControlBar::UnSetAutoHideMode (CBCGPDockingControlBar* pFirstBarInGroup)
{
	m_bPinState = FALSE;

	if (m_nAutoHideConditionTimerID != 0)
	{
		KillTimer (m_nAutoHideConditionTimerID);
	}

	if (m_nSlideTimer != 0)
	{
		KillTimer (m_nSlideTimer);
	}

	BOOL bWasActive = m_pAutoHideBar->m_bActiveInGroup;

	m_pAutoHideBar->RemoveAutoHideWindow (this);

	RemoveFromDefaultSlider ();		
	// unset autohide mode - make it docked back
	if (pFirstBarInGroup == NULL)
	{
		if (!DockControlBar (this, NULL, DM_DBL_CLICK))
		{
			return;
		}
	}
	else
	{
		AttachToTabWnd (pFirstBarInGroup, DM_SHOW, bWasActive);
	}
	ShowControlBar (TRUE, FALSE, bWasActive);
	AdjustDockingLayout ();
}
//***********************************************************************
void CBCGPDockingControlBar::OnTimer(UINT nIDEvent) 
{
	BOOL bSlideDirection = FALSE;

	switch (nIDEvent)
	{
	case ID_CHECK_AUTO_HIDE_CONDITION:
		if (CheckAutoHideCondition ())
		{
			KillTimer (m_nAutoHideConditionTimerID);
			m_nAutoHideConditionTimerID = 0;
		}
		return;
	case BCGP_AUTO_HIDE_SLIDE_OUT_EVENT:
		bSlideDirection = TRUE;
		m_bIsHiding = FALSE;
		break;
	case BCGP_AUTO_HIDE_SLIDE_IN_EVENT:
		bSlideDirection = FALSE;
		m_bIsHiding = TRUE;
		break;
	default:
		CBCGPControlBar::OnTimer(nIDEvent);
		return;
	}

	OnSlide (bSlideDirection);

	if (CheckStopSlideCondition (bSlideDirection))
	{
		KillTimer (m_nSlideTimer);

		m_bIsSliding = FALSE;
		m_nSlideTimer = 0;
		m_nSlideStep = 0;

		if (bSlideDirection) // slide out - show
		{
		
			RedrawWindow (NULL, NULL,
				RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | 
				RDW_ALLCHILDREN);

			::RedrawWindow (m_hDefaultSlider, NULL, NULL, RDW_INVALIDATE);
			// one second time out to give the user ability to move the mouse over
			// miniframe
			if (m_nAutoHideConditionTimerID != 0)
			{
				KillTimer (m_nAutoHideConditionTimerID);
			}

			m_nAutoHideConditionTimerID = SetTimer (ID_CHECK_AUTO_HIDE_CONDITION, 
													m_nTimeOutBeforeAutoHide, NULL);
		}
		else
		{
			ShowWindow (SW_HIDE);
			CBCGPSlider* pDefaultSlider = GetDefaultSlider ();
			ASSERT_VALID (pDefaultSlider);
			pDefaultSlider->ShowWindow (SW_HIDE);
		}
	}

	CBCGPControlBar::OnTimer(nIDEvent);
}
//***********************************************************************
// Returns TRUE when the dock bar should be hidden (strats slide in)
//***********************************************************************
BOOL CBCGPDockingControlBar::CheckAutoHideCondition ()
{
	if (m_bActive || m_bIsResizing || !IsAutoHideMode ())
	{
		return FALSE;
	}

	if (m_ToolTip.IsWindowVisible()) 
	{
		return FALSE;
	}

	ASSERT_VALID (m_pAutoHideButton);
	ASSERT_VALID (m_pAutoHideBar);

	CRect rectAutoHideBtn = m_pAutoHideButton->GetRect ();
	m_pAutoHideBar->ClientToScreen (&rectAutoHideBtn);

	CPoint ptCursor;
	GetCursorPos (&ptCursor);

	CWnd* pWndFromPoint = WindowFromPoint (ptCursor);
	BOOL bCursorOverThisWindow = FALSE; // and this is topmost window
	while (pWndFromPoint != NULL)
	{
		if (pWndFromPoint == this ||
			pWndFromPoint->m_hWnd == m_hDefaultSlider)
		{
			bCursorOverThisWindow = TRUE;
			break;
		}
		pWndFromPoint = pWndFromPoint->GetParent ();
	}

	CRect rectWnd;
	GetWindowRect (rectWnd);
	CRect rectSlider;
	::GetWindowRect (m_hDefaultSlider, &rectSlider);

	rectWnd.UnionRect (rectWnd, rectSlider);

	if (rectWnd.PtInRect (ptCursor) &&  bCursorOverThisWindow ||
		rectAutoHideBtn.PtInRect (ptCursor))
	{
		return FALSE;
	}

	Slide (FALSE);

	return TRUE;
}
//***********************************************************************
BOOL CBCGPDockingControlBar::CheckStopSlideCondition (BOOL bDirection)
{
	if (!IsAutoHideMode ())
	{
		return TRUE;
	}

	CRect rectWnd;
	GetWindowRect (rectWnd);

	GetDockSite ()->ScreenToClient (rectWnd);
 
	CRect rectAutoHideDockBar;
	m_pAutoHideBar->GetParentDockBar ()->GetWindowRect (rectAutoHideDockBar);
	GetDockSite ()->ScreenToClient (rectAutoHideDockBar);

	BOOL bStop = FALSE;
	switch (GetCurrentAlignment ())
	{

	case CBRS_ALIGN_RIGHT:
		if (m_ahSlideMode == BCGP_AHSM_MOVE)
		{
			bStop = bDirection ? rectWnd.right <= rectAutoHideDockBar.left : 
								 rectWnd.left >= rectAutoHideDockBar.left;
		}
		else
		{
			bStop = bDirection ? rectWnd.Width () >= m_rectRestored.Width () : 
							 rectWnd.Width () <= 0;
		}
		break;
	case CBRS_ALIGN_LEFT:
		if (m_ahSlideMode == BCGP_AHSM_MOVE)
		{
			bStop = bDirection ? rectWnd.left >= rectAutoHideDockBar.right :
								 rectWnd.right <= rectAutoHideDockBar.right;
		}
		else
		{
			bStop = bDirection ? rectWnd.Width () >= m_rectRestored.Width () : 
							 rectWnd.Width () <= 0;
		}
		break;
	case CBRS_ALIGN_TOP:	
		if (m_ahSlideMode == BCGP_AHSM_MOVE)
		{
			bStop = bDirection ? rectWnd.top >= rectAutoHideDockBar.bottom :
								 rectWnd.bottom <= rectAutoHideDockBar.bottom;
		}
		else
		{

		}
		break;
	case CBRS_ALIGN_BOTTOM:	
		if (m_ahSlideMode == BCGP_AHSM_MOVE)
		{
			bStop = bDirection ? rectWnd.bottom <= rectAutoHideDockBar.top :
								 rectWnd.top >= rectAutoHideDockBar.top;
		}
		else
		{
			bStop = bDirection ? rectWnd.Height () >= m_rectRestored.Height () : 
							 rectWnd.Height () <= 0;
		}
		break;
	}

	return bStop;
}
//***********************************************************************
void CBCGPDockingControlBar::OnSlide (BOOL bSlideDirection)
{
	if (!IsAutoHideMode () && !IsWindow (m_hDefaultSlider))
	{
		return;
	}

	m_nSlideStep++;

	CRect rect;
	GetWindowRect (&rect);
	GetDockSite ()->ScreenToClient (&rect); 

	CRect rectSlider;
	::GetWindowRect (m_hDefaultSlider, &rectSlider);
	GetDockSite ()->ScreenToClient (&rectSlider); 

	if (m_ahSlideMode == BCGP_AHSM_MOVE)
	{
		OffsetRectForSliding (rect, bSlideDirection);
		OffsetRectForSliding (rectSlider, bSlideDirection);
		if (bSlideDirection)
		{
			CPoint pt = CalcCorrectOffset (rect);
			rect.OffsetRect (pt);
			rectSlider.OffsetRect (pt);
		}
	}
	else
	{
		CalcRectForSliding (rect, rectSlider, bSlideDirection);
	}


	SetWindowPos (NULL, rect.left, rect.top,
					rect.Width (), rect.Height (),
					SWP_NOZORDER | SWP_NOACTIVATE);

	::SetWindowPos (m_hDefaultSlider, NULL, rectSlider.left, rectSlider.top,
					rectSlider.Width (), rectSlider.Height (), SWP_NOZORDER | SWP_NOACTIVATE);

}
//***********************************************************************
void CBCGPDockingControlBar::OffsetRectForSliding (CRect& rect, BOOL bSlideDirection)
{
	if (!IsAutoHideMode ())
	{
		return;
	}

	switch (GetCurrentAlignment ())
	{
	case CBRS_ALIGN_LEFT:
		bSlideDirection ? rect.OffsetRect (m_nSlideDelta, 0) : 
						  rect.OffsetRect (-m_nSlideDelta, 0);	
		break;

	case CBRS_ALIGN_RIGHT:
		bSlideDirection ? rect.OffsetRect (-m_nSlideDelta, 0) : 
						  rect.OffsetRect (m_nSlideDelta, 0);
		break;

	case CBRS_ALIGN_TOP:
		bSlideDirection ? rect.OffsetRect (0, m_nSlideDelta) : 
						  rect.OffsetRect (0, -m_nSlideDelta);
		break;

	case CBRS_ALIGN_BOTTOM:
		bSlideDirection ? rect.OffsetRect (0, -m_nSlideDelta) : 
						  rect.OffsetRect (0, m_nSlideDelta);
		break;

	}
}
//***********************************************************************
void CBCGPDockingControlBar::CalcRectForSliding (CRect& rect, CRect& rectSlider, BOOL bSlideDirection)
{
	if (!IsAutoHideMode ())
	{
		return;
	}
	
	switch (GetCurrentAlignment ())
	{
	case CBRS_ALIGN_LEFT:
		if (bSlideDirection)   
		{
			rect.right += m_nSlideDelta;
			if (rect.Width () > m_rectRestored.Width ())
			{
				rect.right = rect.left + m_rectRestored.Width ();
			}
		}
		else
		{
			rect.right -= m_nSlideDelta;	
			if (rect.right < rect.left)
			{
				rect.right = rect.left;
			}
		}
		{
			int nSliderWidth = rectSlider.Width ();
			rectSlider.left = rect.right;
			rectSlider.right = rectSlider.left + nSliderWidth;
		}
		break;

	case CBRS_ALIGN_RIGHT:
		if (bSlideDirection)   
		{
			rect.left -= m_nSlideDelta;
			if (rect.Width () > m_rectRestored.Width ())
			{
				rect.left = rect.right - m_rectRestored.Width ();
			}
		}
		else
		{
			rect.left += m_nSlideDelta;	
			if (rect.left > rect.right)
			{
				rect.left = rect.right;
			}
		}
		{
			int nSliderWidth = rectSlider.Width ();
			rectSlider.right = rect.left;
			rectSlider.left = rectSlider.right - nSliderWidth;
		}
		break;

	case CBRS_ALIGN_TOP:
		if (bSlideDirection)   
		{
			rect.bottom += m_nSlideDelta;
			if (rect.Height () > m_rectRestored.Height ())
			{
				rect.bottom = rect.top + m_rectRestored.Height ();
			}
		}
		else
		{
			rect.bottom -= m_nSlideDelta;	
			if (rect.bottom < rect.top)
			{
				rect.bottom = rect.top;
			}
		}
		{
			int nSliderHeight = rectSlider.Height ();
			rectSlider.top = rect.bottom;
			rectSlider.bottom = rectSlider.top + nSliderHeight;
		}
		break;

	case CBRS_ALIGN_BOTTOM:
		if (bSlideDirection)   
		{
			rect.top -= m_nSlideDelta;
			if (rect.Height () > m_rectRestored.Height ())
			{
				rect.top = rect.bottom - m_rectRestored.Height ();
			}
		}
		else
		{
			rect.top += m_nSlideDelta;	
			if (rect.top > rect.bottom)
			{
				rect.top = rect.bottom;
			}
		}
		{
			int nSliderHeight = rectSlider.Height ();
			rectSlider.bottom = rect.top;
			rectSlider.top = rectSlider.bottom - nSliderHeight;
		}
		break;

	}
}
//***********************************************************************
CPoint CBCGPDockingControlBar::CalcCorrectOffset (CRect rect)
{
	CRect rectAutoHideDockBar;
	m_pAutoHideBar->GetParentDockBar ()->GetWindowRect (rectAutoHideDockBar);
	GetDockSite ()->ScreenToClient (rectAutoHideDockBar);
	
	switch (GetCurrentAlignment ())
	{
	case CBRS_ALIGN_LEFT:
		if (rect.left > rectAutoHideDockBar.right)
		{
			return CPoint (rectAutoHideDockBar.right - rect.left, 0);
		}
		break;
	case CBRS_ALIGN_RIGHT:
		if (rect.right < rectAutoHideDockBar.left)
		{
			return CPoint (rectAutoHideDockBar.left - rect.right, 0);
		}
		break;
	case CBRS_ALIGN_TOP:
		if (rect.top > rectAutoHideDockBar.bottom)
		{
			return CPoint (0, rectAutoHideDockBar.bottom - rect.top);
		}
		break;
	case CBRS_ALIGN_BOTTOM:
		if (rect.bottom < rectAutoHideDockBar.top)
		{
			return CPoint (0, rectAutoHideDockBar.top - rect.bottom);
		}
		break;
	}
	return CPoint (0, 0);
}
//***********************************************************************
void CBCGPDockingControlBar::Slide (BOOL bSlideOut, BOOL bUseTimer)
{
	ASSERT_VALID (this);

	if (!IsAutoHideMode ())
	{
		return;
	}

	if (m_nSlideTimer != 0)
	{
		KillTimer (m_nSlideTimer);
	}

	if (m_nAutoHideConditionTimerID != 0)
	{
		KillTimer (m_nAutoHideConditionTimerID);
		m_nAutoHideConditionTimerID = 0;
	}

	CRect rectWnd;
	GetWindowRect (rectWnd);

	if (!bUseTimer || m_bDisableAnimation || globalData.bIsRemoteSession)
	{
		m_nSlideDelta = IsHorizontal () ? rectWnd.Height () : rectWnd.Width ();
	}

	if (!bUseTimer)
	{
		m_rectRestored = rectWnd;
		// just move out from the screen
		
		OnSlide (FALSE);
		ShowWindow (SW_HIDE);
		::ShowWindow (m_hDefaultSlider, SW_HIDE);
		return;
	}

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetDockSite ());
	ASSERT_VALID (pDockManager);	

	if (bSlideOut)		
	{
		pDockManager->HideAutoHideBars (this);
		pDockManager->AlignAutoHideBar (GetDefaultSlider (), FALSE);
		ShowWindow (SW_SHOW);
		::ShowWindow (m_hDefaultSlider, SW_SHOW);
	}

	
	BringWindowToTop ();
	::BringWindowToTop (m_hDefaultSlider);

	if (m_ahSlideMode == BCGP_AHSM_MOVE)
	{
		pDockManager->BringBarsToTop ();
	}

	m_nSlideTimer = SetTimer (bSlideOut ? BCGP_AUTO_HIDE_SLIDE_OUT_EVENT : 
										  BCGP_AUTO_HIDE_SLIDE_IN_EVENT, 
										  m_nSlideDefaultTimeOut, NULL);

	
	if (!m_bDisableAnimation && !globalData.bIsRemoteSession)
	{
		if (m_ahSlideMode == BCGP_AHSM_MOVE)
		{
			GetDockSite ()->ScreenToClient (rectWnd);
			m_nSlideDelta = max (1, ((GetCurrentAlignment () & CBRS_ORIENT_HORZ) ?
					rectWnd.Height () : rectWnd.Width ()) / m_nSlideSteps);

		}
		else if (m_ahSlideMode == BCGP_AHSM_STRETCH)
		{
			if (!bSlideOut && !m_bIsSliding)
			{
				m_rectRestored = rectWnd;
				GetDockSite ()->ScreenToClient (m_rectRestored);
			}
			m_nSlideDelta = max (1, ((GetCurrentAlignment () & CBRS_ORIENT_HORZ) ?
					m_rectRestored.Height () : m_rectRestored.Width ()) / m_nSlideSteps);
		}	
	}

	m_nSlideStep = 0;
	m_bIsSliding = TRUE;
}
//***********************************************************************
void CBCGPDockingControlBar::SetAutoHideParents (CBCGPAutoHideToolBar* pToolBar, 
												 CBCGPAutoHideButton* pBtn)
{
	ASSERT_VALID (pToolBar);
	ASSERT_VALID (pBtn);

	m_pAutoHideBar		= pToolBar;
	m_pAutoHideButton	= pBtn;
}
//***********************************************************************
void CBCGPDockingControlBar::SetResizeMode (BOOL bResize) 
{ 
	m_bIsResizing = bResize;
}
//***********************************************************************
CBCGPSlider* CBCGPDockingControlBar::CreateDefaultSlider (DWORD dwAlignment, CWnd* pParent, 
															CRuntimeClass* pSliderRTC)
{
	CRect rectSlider (0, 0, CBCGPSlider::GetDefaultWidth (), CBCGPSlider::GetDefaultWidth ());
	WORD dwSliderStyle = CBCGPSlider::SS_HORZ;

	if (dwAlignment & CBRS_ALIGN_LEFT || dwAlignment & CBRS_ALIGN_RIGHT)
	{
		dwSliderStyle = CBCGPSlider::SS_VERT;
	}

	// create a slider with a control bar container
	CBCGPSlider* pSlider = NULL; 
	if (pSliderRTC != NULL)
	{
		pSlider = (CBCGPSlider*) pSliderRTC->CreateObject ();
		pSlider->SetDefaultMode (TRUE);
	}
	else
	{
		pSlider = new CBCGPSlider (TRUE); 
	}

	ASSERT_VALID (pSlider);

	if (!pSlider->CreateEx (0, dwSliderStyle | WS_VISIBLE, 
							rectSlider, pParent, (UINT) -1, NULL))
	{
		TRACE0 ("Can't create default slider while docking\n");
		delete pSlider;
		return NULL;
	}

	pSlider->SetBarAlignment (dwAlignment);

	return pSlider;
}
//***********************************************************************
void CBCGPDockingControlBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CPoint ptScreen = point;
	ClientToScreen (&ptScreen);

	CBCGPCaptionButton* pBtn = FindButton (ptScreen);
	if (pBtn != NULL)
	{
		CWnd::OnLButtonDblClk(nFlags, point);
		return;
	}

	if (!IsAutoHideMode ())
	{
		CBCGPDockingControlBar* pBarToDock = this;
		if (IsTabbed ())
		{
			CBCGPBaseTabWnd* pTabWnd = 
				DYNAMIC_DOWNCAST (CBCGPBaseTabWnd, GetParent ());
			if (pTabWnd != NULL)
			{
				pBarToDock = 
					DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pTabWnd->GetParent ());	
			}
		}

		CBCGPMultiMiniFrameWnd* pParentMiniFrame = 
				DYNAMIC_DOWNCAST (CBCGPMultiMiniFrameWnd, GetParentMiniFrame ());

		if (pParentMiniFrame != NULL)
		{
			OnProcessDblClk ();
			pParentMiniFrame->DockRecentControlBarToMainFrame (pBarToDock);
		}
		else if (IsWindow (m_hDefaultSlider))
		{
			// currently docked at main frame
			CBCGPMultiMiniFrameWnd* pRecentMiniFrame = 
				DYNAMIC_DOWNCAST (CBCGPMultiMiniFrameWnd,
					CWnd::FromHandlePermanent (m_recentDockInfo.m_hRecentMiniFrame));
			
			if (pRecentMiniFrame != NULL && 
				(m_recentDockInfo.GetRecentContainer (FALSE) != NULL ||
				 m_recentDockInfo.GetRecentTabContainer (FALSE) != NULL))
			{
				OnProcessDblClk ();
				UnDockControlBar ();
				pRecentMiniFrame->AddRecentControlBar (pBarToDock);
				if (CanFocus ())
				{
					pRecentMiniFrame->SetFocus ();
				}
			}
			else
			{
				CBCGPControlBar::OnLButtonDblClk(nFlags, point);
			}
		}
		else
		{
			OnProcessDblClk ();
		}
	}
	else
	{
		CWnd::OnLButtonDblClk(nFlags, point);
	}
}
//***********************************************************************
BOOL CBCGPDockingControlBar::OnBeforeFloat (CRect& rectFloat, BCGP_DOCK_METHOD dockMethod)
{
	ASSERT_VALID (this);
	BOOL bResult = CBCGPControlBar::OnBeforeFloat (rectFloat, dockMethod);
	
	if (dockMethod == DM_MOUSE)
	{
		// prevent drawing of the drag rectangle on mouse up
		m_bPrepareToFloat = false;
	}

	return bResult;
}
//***********************************************************************
void CBCGPDockingControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	ASSERT_VALID (this);

	if (!IsDocked ())
	{
		CBCGPControlBar::OnNcLButtonDown(nHitTest, point);
	}
}
//***********************************************************************
void CBCGPDockingControlBar::OnClose() 
{
	ASSERT_VALID (this);
	DestroyWindow ();
}
//***********************************************************************
CBCGPDockingControlBar* CBCGPDockingControlBar::AttachToTabWnd (CBCGPDockingControlBar* pTabControlBarAttachTo, 
																BCGP_DOCK_METHOD dockMethod,
																BOOL bSetActive, 
																CBCGPDockingControlBar** ppTabbedControlBar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pTabControlBarAttachTo);

	if (ppTabbedControlBar != NULL)
	{
		*ppTabbedControlBar = NULL;
	}

	if (!pTabControlBarAttachTo->CanBeAttached () || !CanBeAttached ())
	{
		return NULL; // invalid attempt to attach non-attachable control bar
	}

	// check whether pTabBar is derived from CBCGPTabbedControlBar. If so, we 
	// can attach this bar to it immediately. Otherwise, we need to create a 
	// new tabbed control bar and replace pTabControlBarAttachTo with it.
	CBCGPBaseTabbedBar* pTabbedBarAttachTo = 
		DYNAMIC_DOWNCAST (CBCGPBaseTabbedBar, pTabControlBarAttachTo);

	BOOL bBarAttachToIsFloating = (pTabControlBarAttachTo->GetParentMiniFrame () != NULL);

	CWnd* pOldParent = GetParent ();
	CRect rectWndTab; rectWndTab.SetRectEmpty ();
	if (pTabbedBarAttachTo == NULL)
	{
		CWnd* pTabParent = pTabControlBarAttachTo->GetParent ();
		if (DYNAMIC_DOWNCAST (CBCGPBaseTabWnd, pTabParent) != NULL)
		{
			pTabParent = pTabParent->GetParent ();
		}

		pTabbedBarAttachTo = DYNAMIC_DOWNCAST (CBCGPBaseTabbedBar, pTabParent);

		if (pTabbedBarAttachTo == NULL)
		{
			pTabControlBarAttachTo->StoreRecentDockInfo ();

			pTabControlBarAttachTo->GetWindowRect (rectWndTab);
			pTabControlBarAttachTo->GetParent ()->ScreenToClient (&rectWndTab);

			pTabbedBarAttachTo = pTabControlBarAttachTo->CreateTabbedControlBar (); 
			ASSERT_VALID (pTabbedBarAttachTo);

			pTabControlBarAttachTo->InsertControlBar (pTabbedBarAttachTo, pTabControlBarAttachTo);

			if (!pTabControlBarAttachTo->ReplaceControlBar (pTabbedBarAttachTo, dockMethod))
			{
				if (!bBarAttachToIsFloating)
				{
					RemoveControlBarFromDockManager (pTabbedBarAttachTo);
				}
				ASSERT (FALSE);
				TRACE0 ("Failed to replace resizable control bar by tabbed control bar. \n");
				delete pTabbedBarAttachTo;
				return NULL;
			}

			pTabbedBarAttachTo->
				EnableDocking (pTabControlBarAttachTo->GetEnabledAlignment ());
			pTabbedBarAttachTo->
				SetBarAlignment (pTabControlBarAttachTo->GetCurrentAlignment ());
			
			pTabControlBarAttachTo->UnDockControlBar (TRUE);
			pTabbedBarAttachTo->AddTab (pTabControlBarAttachTo, TRUE, bSetActive);
			pTabControlBarAttachTo->EnableGripper (FALSE);
		}
	}

	if (ppTabbedControlBar != NULL)
	{
		*ppTabbedControlBar = pTabbedBarAttachTo;
	}

	EnableGripper (FALSE);
	
	// send before dock notification without guarantee that the bar will 
	// be attached to another dock bar
	OnBeforeDock ((CBCGPBaseControlBar**)pTabbedBarAttachTo, NULL, dockMethod);
	// reassign the parentship to the tab bar
	OnBeforeChangeParent (pTabbedBarAttachTo, TRUE);

	// remove from miniframe
	RemoveFromMiniframe (pTabbedBarAttachTo, dockMethod);

	// AddTab returns TRUE only if this pointer is not tabbed control bar
	// (tabbed control bar is destroyed by AddTab and its tab windows are copied
	// to pTabbedBarAttachTo tabbed window)
	BOOL bResult = pTabbedBarAttachTo->AddTab (this, TRUE, bSetActive);
	if (bResult)
	{
		OnAfterChangeParent (pOldParent);
		OnAfterDock (pTabbedBarAttachTo, NULL, dockMethod);
	}

	if (!rectWndTab.IsRectEmpty ())
	{
		pTabbedBarAttachTo->SetWindowPos (NULL, rectWndTab.left, rectWndTab.top, 
			rectWndTab.Width (), rectWndTab.Height (), 
			SWP_NOZORDER | SWP_NOACTIVATE);
		
		if (bResult)
		{
			AdjustDockingLayout ();
		}
	}

	pTabbedBarAttachTo->RecalcLayout ();

	return bResult ? this : pTabbedBarAttachTo;
}
//***********************************************************************
BOOL CBCGPDockingControlBar::ReplaceControlBar (CBCGPDockingControlBar* pBarToReplaceWith, 
												BCGP_DOCK_METHOD /*dockMethod*/,
												BOOL bRegisterWithFrame)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBarToReplaceWith);
	
	CBCGPMiniFrameWnd* pParentMiniFrame = GetParentMiniFrame ();
	
	if (pParentMiniFrame != NULL)
	{
		// this is tabbed control bar that should be replaced by docking control bar
		// within miniframe
		
		ASSERT_VALID (pParentMiniFrame);
		pParentMiniFrame->ReplaceControlBar (this, pBarToReplaceWith);
		return TRUE;
	}
	else if (m_hDefaultSlider != NULL)
	{
		CBCGPSlider* pDefaultSlider = GetDefaultSlider ();

		if (pDefaultSlider != NULL && 
			pDefaultSlider->ReplaceControlBar (this, pBarToReplaceWith))
		{
			// unregister from parent frame/dock manager the bar that is being replaced (this)
			RemoveControlBarFromDockManager (this, FALSE);
			// register with the parent frame the bar we're replacing with
			if (bRegisterWithFrame)
			{
				AddControlBar (pBarToReplaceWith);
			}
			
			return TRUE;
		}
	}
	return FALSE;
}
//***********************************************************************
CBCGPTabbedControlBar* CBCGPDockingControlBar::CreateTabbedControlBar ()
{
	ASSERT_VALID (this);
	CRect rectTabBar;
	GetWindowRect (&rectTabBar);
	ASSERT_VALID (GetParent ());
	GetParent ()->ScreenToClient (&rectTabBar);

	CBCGPTabbedControlBar* pTabbedBar = 
			(CBCGPTabbedControlBar*) m_pTabbedControlBarRTC->CreateObject ();
	ASSERT_VALID (pTabbedBar);

	pTabbedBar->SetAutoDestroy (TRUE);

	if (!pTabbedBar->Create (_T (""), 
							GetParent (), 
							rectTabBar, 
							TRUE, 
							(UINT) -1, 
							GetStyle () | CBRS_FLOAT_MULTI))
	{
		TRACE0 ("Failed to create tabbed control bar\n");
		return NULL;
	}

	// override recent floating/docking info

	pTabbedBar->m_recentDockInfo.m_recentMiniFrameInfo.m_rectDockedRect = 
		m_recentDockInfo.m_recentMiniFrameInfo.m_rectDockedRect;
	pTabbedBar->m_recentDockInfo.m_recentSliderInfo.m_rectDockedRect = 
		m_recentDockInfo.m_recentSliderInfo.m_rectDockedRect;
	pTabbedBar->m_recentDockInfo.m_rectRecentFloatingRect = 
		m_recentDockInfo.m_rectRecentFloatingRect;

	return pTabbedBar;
}
//***********************************************************************
BOOL CBCGPDockingControlBar::Dock (CBCGPBaseControlBar* pTargetBar, LPCRECT lpRect, 
								  BCGP_DOCK_METHOD dockMethod)
{
	CFrameWnd* pParentFrame = DYNAMIC_DOWNCAST (CFrameWnd, BCGPGetParentFrame (this));
	ASSERT_VALID (pParentFrame);

	if (pTargetBar != NULL && !pTargetBar->CanAcceptBar (this) && 
		pTargetBar != this)
	{
		return FALSE;
	}

	if (dockMethod == DM_RECT && lpRect == NULL)
	{
		TRACE0 ("Docking control bar must be docked by rect or by mouse!");
		ASSERT (FALSE);
		return FALSE;
	}

	m_bPrepareToFloat = false;

	if (dockMethod == DM_DBL_CLICK || dockMethod == DM_SHOW)
	{
		CBCGPBarContainer* pRecentTabContainer = 
			m_recentDockInfo.GetRecentTabContainer (TRUE);

		ShowWindow (SW_HIDE);

		RemoveFromMiniframe (BCGPGetParentFrame (this), dockMethod);
		SetBarAlignment (m_recentDockInfo.m_dwRecentAlignmentToFrame);

		CBCGPSlider* pRecentDefaultSlider = m_recentDockInfo.GetRecentDefaultSlider ();
		if (pRecentDefaultSlider != NULL)
		{
			SetDefaultSlider (pRecentDefaultSlider->m_hWnd);
		}

		if (pRecentTabContainer != NULL)
		{
			BOOL bRecentLeftBar = m_recentDockInfo.IsRecentLeftBar (TRUE);
			CBCGPDockingControlBar* pTabbedBar = (CBCGPDockingControlBar*) (bRecentLeftBar ? 
				pRecentTabContainer->GetLeftBar () : pRecentTabContainer->GetRightBar ());
			if (pTabbedBar != NULL)
			{
				BOOL bResult = (AttachToTabWnd (pTabbedBar, DM_DBL_CLICK) != NULL);
				ShowWindow (SW_SHOW);
				AdjustDockingLayout ();
				return bResult;
			}
		}

		if (pRecentDefaultSlider != NULL)
		{
			EnableGripper (TRUE);
			InsertControlBar (this, pRecentDefaultSlider, FALSE);

			ShowWindow (SW_SHOW);
			CBCGPDockingControlBar* pAddedControlBar = 
				pRecentDefaultSlider->AddRecentControlBar (this);
			if (pAddedControlBar == this)
			{
				AdjustDockingLayout ();
				return TRUE;
			}
			else if (pAddedControlBar != NULL)
			{
				pAddedControlBar->AdjustDockingLayout ();
				return FALSE;
			}
		}
		else
		{
			ShowWindow (SW_SHOW);
			return DockToFrameWindow (m_recentDockInfo.m_dwRecentAlignmentToFrame, 
				(lpRect == NULL) ? 
						&m_recentDockInfo.m_recentSliderInfo.m_rectDockedRect : lpRect);
		}
		return FALSE;
	}

	CPoint ptMouse (0, 0);
	if (dockMethod == DM_MOUSE || dockMethod == DM_STANDARD)
	{
		GetCursorPos (&ptMouse);
	}


	DWORD dwAlignment = 0;
	if (pTargetBar == NULL)
	{
		// insert the resizable bar as first resizable if it crosses the outer edge
		// IsPointNearDockBar will return this information
		BOOL bOuterEdge = FALSE;
	
		if (dockMethod == DM_MOUSE  || dockMethod == DM_STANDARD)
		{
			CPoint ptMouse;
			GetCursorPos (&ptMouse);
			if (!IsPointNearDockBar (ptMouse, dwAlignment, bOuterEdge))
			{
				return FALSE;
			}
			return DockToFrameWindow (dwAlignment, NULL, DT_DOCK_LAST, 
										NULL, -1, bOuterEdge);
		}
		else if (lpRect != NULL)
		{

		}
		
	}
	else
	{
		ASSERT_VALID (pTargetBar);

		if (dockMethod == DM_MOUSE || dockMethod == DM_STANDARD)
		{
			if (!globalUtils.CheckAlignment (ptMouse, pTargetBar, 
											CBCGPDockManager::m_nDockSencitivity,
											FALSE, dwAlignment))
			{
				return FALSE;
			}

			return DockToWindow ((CBCGPDockingControlBar*) pTargetBar, dwAlignment, NULL);
		}
		else if (lpRect != NULL)
		{
			return DockToWindow ((CBCGPDockingControlBar*) pTargetBar, 0, lpRect);
		}
	}

	return FALSE;
}
//***********************************************************************
BOOL CBCGPDockingControlBar::DockToFrameWindow (DWORD dwAlignment, 
												LPCRECT lpRect, 
												DWORD /*dwDockFlags*/, 
												CBCGPBaseControlBar* /*pRelativeBar*/, 
												int /*nRelativeIndex*/, 
												BOOL bOuterEdge)
{
	ASSERT_VALID (this);
	ASSERT (dwAlignment & CBRS_ALIGN_ANY);

	RemoveFromMiniframe (BCGPGetParentFrame (this), DM_UNKNOWN);
	
	if (m_hDefaultSlider != NULL && IsWindow (m_hDefaultSlider))
	{
		UnDockControlBar (FALSE);
	}

	CBCGPSlider* pDefaultSlider = NULL;
	// create a slider with a control bar container
	if ((pDefaultSlider = CreateDefaultSlider (dwAlignment, GetDockSite ())) == NULL)
	{
		TRACE0 ("Failde to create default slider");
		ShowWindow (SW_SHOW);
		return FALSE;
	}

	m_hDefaultSlider = pDefaultSlider->m_hWnd;

	CRect rectBar;
	GetWindowRect (rectBar);
	
	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetDockSite ());
	ASSERT_VALID (pDockManager);

	if (pDockManager->AdjustRectToClientArea (rectBar, dwAlignment))
	{
		SetWindowPos (NULL, 0, 0, rectBar.Width (), rectBar.Height (),  
							SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
	
	pDefaultSlider->AddControlBar (this);

	// register this docking bar and slider with the frame's window dock manager
	if (!bOuterEdge)
	{
		AddControlBar (this);
		AddControlBar (pDefaultSlider);
	}
	else
	{
		pDockManager->AddControlBar (pDefaultSlider, !bOuterEdge, FALSE, bOuterEdge);
		pDockManager->AddControlBar (this, !bOuterEdge, FALSE, bOuterEdge);
	}

	SetBarAlignment (dwAlignment);
	pDefaultSlider->SetBarAlignment (GetCurrentAlignment ());
	m_recentDockInfo.m_dwRecentAlignmentToFrame = GetCurrentAlignment ();

	EnableGripper (TRUE);

	if (lpRect != NULL)
	{
		CRect rect (lpRect);
		SetWindowPos (NULL, 0, 0, rect.Width (), rect.Height (),  
					SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}

	AdjustDockingLayout ();
	OnAfterDock (this, NULL, DM_UNKNOWN);
	return TRUE;
}
//***********************************************************************
BOOL CBCGPDockingControlBar::DockToWindow (CBCGPDockingControlBar* pTargetWindow, 
										  DWORD dwAlignment, 
										  LPCRECT lpRect)
{

	ASSERT_VALID (this);
	ASSERT_VALID (pTargetWindow);
	ASSERT (dwAlignment & CBRS_ALIGN_ANY || lpRect != NULL);
	ASSERT_KINDOF (CBCGPDockingControlBar, pTargetWindow);

	CBCGPSlider* pSlider = pTargetWindow->GetDefaultSlider ();

	if (pSlider == NULL)
	{
		ShowWindow (SW_SHOW);
		return FALSE;
	}


	if (m_hDefaultSlider != NULL && IsWindow (m_hDefaultSlider))
	{
		UnDockControlBar (FALSE);
	}

	RemoveFromMiniframe (BCGPGetParentFrame (this), DM_UNKNOWN);
	
	if (pSlider->InsertControlBar (this, pTargetWindow, dwAlignment, lpRect))
	{
		// the bar was successfully inserted into slider's container. Now, we need 
		// to register it with the frame
		InsertControlBar (this, pTargetWindow, TRUE);
		m_hDefaultSlider = pSlider->m_hWnd;

		EnableGripper (TRUE);
		// force NcCalcSize to recalculate and draw the caption (gripper)
		SetWindowPos (NULL, 0, 0, 0, 0, 
						SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | 
						SWP_NOREDRAW | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		AdjustDockingLayout ();
		OnAfterDock (this, NULL, DM_UNKNOWN);
		return TRUE;
	}
	
	return FALSE;
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::DockContainer (CBCGPBarContainerManager& barContainerManager, 
											DWORD dwAlignment, BCGP_DOCK_METHOD /*dockMethod*/)
{
	if (m_hDefaultSlider != NULL && IsWindow (m_hDefaultSlider))
	{
		CObList lstControlBars; 
		barContainerManager.AddControlBarsToList (&lstControlBars, NULL);

		for (POSITION pos = lstControlBars.GetHeadPosition (); pos != NULL;)
		{
			CBCGPDockingControlBar* pBar = 
				DYNAMIC_DOWNCAST (CBCGPDockingControlBar, lstControlBars.GetNext (pos));

			InsertControlBar (pBar, this, TRUE);
			pBar->SetDefaultSlider (m_hDefaultSlider); 
			pBar->SetBarAlignment (GetCurrentAlignment ());
		}

		CBCGPSlider* pDefaultSlider = GetDefaultSlider ();
		if (pDefaultSlider != NULL)
		{
			return pDefaultSlider->AddContainer (this, barContainerManager, dwAlignment);
		}
	}

	return FALSE;
}
//*****************************************************************************************
void CBCGPDockingControlBar::DrawCaption (CDC* pDC, CRect rectCaption)
{
	ASSERT_VALID (pDC);

	CRect rcbtnRight = CRect (rectCaption.BottomRight (), CSize (0, 0));
	for (int i = (int) m_arrButtons.GetUpperBound (); i >= 0; i --)
	{
		if (!m_arrButtons [i]->m_bLeftAlign && !m_arrButtons [i]->m_bHidden)
		{
			rcbtnRight = m_arrButtons [i]->GetRect();
			break;
		}
	}

	CRect rcbtnLeft = CRect (rectCaption.TopLeft (), CSize (0, 0));
	for (i = (int) m_arrButtons.GetUpperBound (); i >= 0; i --)
	{
		if (m_arrButtons [i]->m_bLeftAlign && !m_arrButtons [i]->m_bHidden)
		{
			rcbtnLeft = m_arrButtons [i]->GetRect();
			break;
		}
	}

	COLORREF clrCptnText = CBCGPVisualManager::GetInstance ()->OnDrawControlBarCaption (
		pDC, this, m_bActive, rectCaption, rcbtnRight);

	for (i = 0; i < m_arrButtons.GetSize (); i ++)
	{
		CBCGPCaptionButton* pbtn = m_arrButtons [i];
		ASSERT_VALID (pbtn);

		pbtn->m_clrForeground = clrCptnText;
	}

    int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
    COLORREF clrOldText = pDC->SetTextColor (clrCptnText);

    CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);
	ASSERT (pOldFont != NULL);

    CString strTitle;
    GetWindowText (strTitle);

	rectCaption.right = rcbtnRight.left;
	rectCaption.left = rcbtnLeft.right;
	rectCaption.top++;
	rectCaption.DeflateRect (g_nCaptionHorzMargin * 2, 0);

	pDC->DrawText (strTitle, rectCaption, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    pDC->SelectObject(pOldFont);
    pDC->SetBkMode(nOldBkMode);
    pDC->SetTextColor(clrOldText);
}
//*****************************************************************************************
void CBCGPDockingControlBar::RedrawButton (const CBCGPCaptionButton* pButton)
{
	if (pButton == NULL /*|| GetParentMiniFrame (TRUE) != NULL*/)
	{
		return;
	}

	if (!pButton->m_bEnabled)
	{
		return;
	}

	m_rectRedraw = pButton->GetRect ();
	SendMessage (WM_NCPAINT);
	m_rectRedraw.SetRectEmpty ();

	UpdateWindow ();
}
//*****************************************************************************************
void CBCGPDockingControlBar::SetCaptionStyle (BOOL bDrawText, BOOL /*bForceGradient*/,
											BOOL bHideDisabledButtons)
{
	m_bCaptionText = bDrawText;
	m_bHideDisabledButtons = bHideDisabledButtons;
}
//*****************************************************************************************
void CBCGPDockingControlBar::ShowControlBar (BOOL bShow, BOOL bDelay, BOOL bActivate)
{
	if (IsAutoHideMode ())
	{
		if (IsHideInAutoHideMode ())
		{
			if (IsBarVisible () && !bShow)
			{
				m_pAutoHideButton->ShowAttachedWindow (FALSE);
			}
			m_pAutoHideBar->ShowAutoHideWindow(this, bShow, bDelay);
		}
		else
		{
			m_pAutoHideButton->ShowAttachedWindow (TRUE);
			if (bShow && bActivate)
			{
				SetFocus ();
				m_bActive = TRUE;
			}
		}
	}
	else if (IsFloating () || IsTabbed ())
	{
		// standard procedure - show/hide bar and its miniframe
		CBCGPControlBar::ShowControlBar (bShow, bDelay, bActivate);
		CBCGPMiniFrameWnd* pMiniFrame = GetParentMiniFrame ();
		if (pMiniFrame != NULL)
		{
			pMiniFrame->OnShowControlBar (this, bShow);
		}
	}
	else
	{
		
		ShowWindow (bShow ? SW_SHOW : SW_HIDE);
		CBCGPSlider* pDefaultSlider = GetDefaultSlider ();
		CBCGPMiniFrameWnd* pMiniFrame = GetParentMiniFrame ();

		if (pMiniFrame != NULL)
		{
			pMiniFrame->OnShowControlBar (this, bShow);
		}
		else if (pDefaultSlider != NULL)
		{
			if (bShow)
			{
				int nLastPercent = GetLastPercentInContainer ();
				if (nLastPercent >= 50)
				{
					SetLastPercentInContainer (50);
				}
				else
				{
					SetLastPercentInContainer (nLastPercent + 1);
				}
			}
			
			// docked at main frame - notify to adjust container
			pDefaultSlider->OnShowControlBar (this, bShow);
			if (!bDelay)
			{
				AdjustDockingLayout ();
			}
		}
		else 
		{
			// flotaing with other bars on miniframe  - notify to adjust container
			
		}
		
	}

	if (IsTabbed () && bShow && bActivate)
	{
		CBCGPBaseTabWnd* pParentTab = DYNAMIC_DOWNCAST (CBCGPBaseTabWnd, GetParent ());
		if (pParentTab == NULL)
		{
			ASSERT (FALSE);
			return;
		}

		ASSERT_VALID (pParentTab);
		pParentTab->SetActiveTab (pParentTab->GetTabFromHwnd (GetSafeHwnd ()));
	}
}
//*****************************************************************************************
void CBCGPDockingControlBar::UnDockControlBar (BOOL bDelay)
{
	CBCGPMiniFrameWnd* pMiniFrame = 
		DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, GetParentMiniFrame ());

	if (pMiniFrame == NULL)
	{
		RemoveFromDefaultSlider ();
		// remove from dock site		
		RemoveControlBarFromDockManager (this, FALSE, !bDelay);

		if (!bDelay && !IsFloating ())
		{
			AdjustDockingLayout ();
		}
	}
	else
	{
		pMiniFrame->RemoveControlBar (this);
	}
}
//*****************************************************************************************
void CBCGPDockingControlBar::OnDestroy() 
{
	RemoveCaptionButtons ();

	if (GetParentMiniFrame () != NULL)
	{
		RemoveFromMiniframe (NULL, DM_UNKNOWN);
	}
	else
	{
		UnDockControlBar (TRUE);
	}	

	if (m_ToolTip.GetSafeHwnd () != NULL)
	{
		m_ToolTip.DestroyWindow ();
	}

	CBCGPControlBar::OnDestroy();
}
//*************************************************************************************
void CBCGPDockingControlBar::OnTrackCaptionButtons (CPoint point)
{
	UINT nHot = m_nHot;

	CBCGPCaptionButton* pBtn = FindButton (point);
	if (pBtn != NULL)
	{
		m_nHot = pBtn->GetHit ();

		if (m_nHit == HTNOWHERE || m_nHit == m_nHot)
		{
			pBtn->m_bFocused = TRUE;
		}
	}
	else
	{
		m_nHot = HTNOWHERE;
	}

	if (m_nHot != nHot)
	{
		RedrawButton (pBtn);

		CBCGPCaptionButton* pBtnOld = FindButtonByHit (nHot);
		if (pBtnOld != NULL)
		{
			pBtnOld->m_bFocused = FALSE;
			RedrawButton (pBtnOld);
		}
	}

	if (m_nHit == HTNOWHERE)
	{
		if (nHot != HTNOWHERE && m_nHot == HTNOWHERE)
		{
			::ReleaseCapture();
			m_bCaptionButtonsCaptured = FALSE;
		}
		else if (nHot == HTNOWHERE && m_nHot != HTNOWHERE)
		{
			SetCapture ();
			m_bCaptionButtonsCaptured = TRUE;
		}
	}
}
//*************************************************************************************
void CBCGPDockingControlBar::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	if (!m_bPrepareToFloat /*&& GetParentMiniFrame (TRUE) == NULL*/)
	{
		OnTrackCaptionButtons (point);
	}

	CBCGPControlBar::OnNcMouseMove(nHitTest, point);
}
//************************************************************************************
void CBCGPDockingControlBar::StopCaptionButtonsTracking ()
{
	if (m_nHit != HTNOWHERE)
	{
		CBCGPCaptionButton* pBtn = FindButtonByHit (m_nHit);
		m_nHit = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bPushed = FALSE;
			RedrawButton (pBtn);
		}
	}

	if (m_nHot != HTNOWHERE)
	{
		CBCGPCaptionButton* pBtn = FindButtonByHit (m_nHot);
		m_nHot = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bFocused = FALSE;
			RedrawButton (pBtn);
		}
	}
	m_bCaptionButtonsCaptured = FALSE;
}
//*************************************************************************************
void CBCGPDockingControlBar::OnCancelMode() 
{
	StopCaptionButtonsTracking ();
	if (m_bPrepareToFloat)
	{
		m_bPrepareToFloat = false;
	}
	CBCGPControlBar::OnCancelMode();
}
//*************************************************************************************
CBCGPCaptionButton* CBCGPDockingControlBar::FindButton (CPoint point) const
{
	ASSERT_VALID (this);

	CRect rcBar;
    GetWindowRect(rcBar);
	ScreenToClient (rcBar);

	for (int i = 0; i < m_arrButtons.GetSize (); i ++)
	{
		CBCGPCaptionButton* pbtn = m_arrButtons [i];
		ASSERT_VALID (pbtn);

		CRect rectBtn = pbtn->GetRect ();
		rectBtn.OffsetRect (rcBar.TopLeft());

		ClientToScreen (rectBtn);

		if (rectBtn.PtInRect (point))
		{
			return pbtn;
		}
	}

	return NULL;
}
//*****************************************************************************************
CBCGPCaptionButton* CBCGPDockingControlBar::FindButtonByHit (UINT nHit) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arrButtons.GetSize (); i ++)
	{
		CBCGPCaptionButton* pbtn = m_arrButtons [i];
		ASSERT_VALID (pbtn);

		if (pbtn->GetHit () == nHit)
		{
			return pbtn;
		}
	}

	return NULL;
}
//*****************************************************************************************
void CBCGPDockingControlBar::EnableButton (UINT nHit, BOOL bEnable/* = TRUE*/)
{
	ASSERT_VALID (this);

	CBCGPCaptionButton* pButton = FindButtonByHit (nHit);
	if (pButton == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	pButton->m_bEnabled = bEnable;
}
//******************************************************************************
BOOL CBCGPDockingControlBar::IsButtonEnabled (UINT nHit) const
{
	ASSERT_VALID (this);

	CBCGPCaptionButton* pButton = FindButtonByHit (nHit);
	if (pButton == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	return pButton->m_bEnabled;
}
//******************************************************************************
void CBCGPDockingControlBar::OnUpdateCmdUI(class CFrameWnd *pTarget, int bDisableIfNoHndler)
{
    UpdateDialogControls(pTarget, bDisableIfNoHndler);

    CWnd* pFocus = GetFocus();
    BOOL bActiveOld = m_bActive;

    m_bActive = (pFocus->GetSafeHwnd () != NULL && 
		(IsChild (pFocus) || pFocus->GetSafeHwnd () == GetSafeHwnd ()));

    if (m_bActive != bActiveOld)
	{
        SendMessage (WM_NCPAINT);
	}
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::IsVisible () const
{
	if (IsAutoHideMode ())
	{
		if (!IsHideInAutoHideMode ())
		{
			return FALSE;
		}
		return m_pAutoHideBar->IsVisible ();
	}
	return CBCGPControlBar::IsVisible ();
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::PreTranslateMessage(MSG* pMsg) 
{
   	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCRBUTTONUP:
	case WM_NCMBUTTONUP:
	case WM_MOUSEMOVE:
		if (m_ToolTip.GetSafeHwnd () != NULL)
		{
			m_ToolTip.RelayEvent(pMsg);
		}
		break;
	}

	if (pMsg->message == WM_KEYDOWN && GetDockMode () == DT_STANDARD && m_bPrepareToFloat &&
		pMsg->wParam == VK_ESCAPE)
	{
		PostMessage (WM_CANCELMODE);
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && IsTabbed () && pMsg->wParam == VK_ESCAPE)
	{
		CBCGPBaseTabbedBar* pParentBar = GetParentTabbedBar ();
		CBCGPMiniFrameWnd* pParentMiniFrame = pParentBar->GetParentMiniFrame ();
		if (pParentBar != NULL && 
			(pParentBar->IsTracked () || 
			 pParentMiniFrame != NULL && pParentMiniFrame->IsCaptured ()))
			
		{
			if (pParentMiniFrame != NULL)
			{
				pParentMiniFrame->PostMessage (WM_CANCELMODE);
			}
			else
			{
				pParentBar->PostMessage (WM_CANCELMODE);
			}
			return TRUE;
		}
	}

	return CBCGPControlBar::PreTranslateMessage(pMsg);
}
//*****************************************************************************************
void CBCGPDockingControlBar::SetDefaultSlider (HWND hSliderWnd)  
{
	if (m_hDefaultSlider != hSliderWnd)
	{
		CBCGPSlider* pDefaultSlider = GetDefaultSlider ();
		if (pDefaultSlider != NULL)
		{
			pDefaultSlider->RemoveControlBar (this);
		}
	}
	m_hDefaultSlider = hSliderWnd;
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::LoadState (LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	return CBCGPControlBar::LoadState (lpszProfileName, nIndex, uiID);
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::SaveState (LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	return CBCGPControlBar::SaveState (lpszProfileName, nIndex, uiID);
}
//*****************************************************************************************
void CBCGPDockingControlBar::Serialize (CArchive& ar)
{
	CBCGPControlBar::Serialize (ar);
	if (ar.IsLoading ())
	{
		ar >> m_recentDockInfo.m_rectRecentFloatingRect;
		ar >> m_rectSavedDockedRect;
		m_recentDockInfo.m_recentSliderInfo.m_rectDockedRect = m_rectSavedDockedRect;
		ar >> m_bRecentFloatingState;
	}
	else
	{
		BOOL bFloating = IsFloating ();

		if (bFloating)
		{
			CBCGPMiniFrameWnd* pMiniFrame = GetParentMiniFrame ();
			if (pMiniFrame != NULL)
			{
				pMiniFrame->GetWindowRect (m_recentDockInfo.m_rectRecentFloatingRect);
			}
		}
		else
		{
			CalcRecentDockedRect ();
		}

		ar << m_recentDockInfo.m_rectRecentFloatingRect;
		ar << m_recentDockInfo.m_recentSliderInfo.m_rectDockedRect;
		ar << m_bRecentFloatingState;
	}
}
//*****************************************************************************************
void CBCGPDockingControlBar::GetRecentSiblingBarInfo (CList<UINT, UINT&>& /*lstBarIDs*/)
{
}
//*****************************************************************************************
LRESULT CBCGPDockingControlBar::OnSetText(WPARAM, LPARAM lParam) 
{
	LRESULT	lRes = Default();

	if (!lRes)
	{
		return lRes;
	}
	
	CBCGPMiniFrameWnd* pParentMiniFrame = NULL;

	if (IsTabbed())
	{
		// If we are docked on a tabbed control bar, we have to update the tab label
		CBCGPBaseTabWnd* pParentTabWnd	= 
			DYNAMIC_DOWNCAST(CBCGPBaseTabWnd, GetParent());

		ASSERT_VALID (pParentTabWnd);	

		CWnd* pWndTabbedControlBar = 
			DYNAMIC_DOWNCAST(CBCGPBaseTabbedBar, pParentTabWnd->GetParent());

		if (pWndTabbedControlBar != NULL)
		{
			LPCTSTR	lpcszTitle	= reinterpret_cast<LPCTSTR> (lParam);
			int		iTab	= pParentTabWnd->GetTabFromHwnd(GetSafeHwnd());
			CString	strLabel;
			if (iTab >= 0 && iTab < pParentTabWnd->GetTabsNum ())
			{
				VERIFY(pParentTabWnd->GetTabLabel(iTab, strLabel));
				if (strLabel != lpcszTitle)
				{
					VERIFY(pParentTabWnd->SetTabLabel(iTab, lpcszTitle));
				}
			}
		}
	}
	else if ((pParentMiniFrame = GetParentMiniFrame ()) != NULL)
	{
		pParentMiniFrame->SetWindowPos (NULL, 0, 0, 0, 0, 
				SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | 
				SWP_FRAMECHANGED);
	}
	else if (IsAutoHideMode ())
	{
		ASSERT_VALID (m_pAutoHideBar);
		m_pAutoHideBar->RedrawWindow ();
		AdjustDockingLayout ();
	}
	else 
	{
		SetWindowPos (NULL, 0, 0, 0, 0, 
				SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | 
				SWP_FRAMECHANGED);
	}


	return lRes;
}
//*****************************************************************************************
CBCGPControlBar* CBCGPDockingControlBar::DockControlBarStandard (BOOL& bWasDocked)
{
	CBCGPBaseControlBar* pTargetBar = NULL;
	BCGP_CS_STATUS status = IsChangeState (CBCGPDockManager::m_nDockSencitivity, 
													&pTargetBar);

	CBCGPDockingControlBar* pTargetDockingBar = 
		DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pTargetBar);

	if (pTargetDockingBar == this || GetAsyncKeyState (VK_CONTROL) < 0)
	{
		return NULL;
	}

	CBCGPMultiMiniFrameWnd* pTargetMiniFrame = pTargetDockingBar != NULL ? 
		DYNAMIC_DOWNCAST (CBCGPMultiMiniFrameWnd, pTargetDockingBar->GetParentMiniFrame ()) : NULL;

	if (status == CS_DELAY_DOCK) // status returned by resizable control bar
	{
	
		if (pTargetMiniFrame != NULL)
		{
			if ((GetBarStyle () & CBRS_FLOAT_MULTI) == 0)
			{
				return NULL;
			}
			else if (pTargetBar != NULL)
			{
				bWasDocked = !pTargetMiniFrame->DockBar (this);
				return this;
			}
		}

		bWasDocked = DockControlBar (pTargetDockingBar, NULL, DM_STANDARD);
	}
	else if (status == CS_DELAY_DOCK_TO_TAB && pTargetDockingBar != NULL && 
			 pTargetDockingBar->CanAcceptBar (this) && CanBeAttached ())
	{
		UnDockControlBar (FALSE);
		CBCGPDockingControlBar* pBar = AttachToTabWnd (pTargetDockingBar, DM_STANDARD);
		bWasDocked = (pBar != NULL);
		return pBar;
	}

	return NULL;
}
//*****************************************************************************************
CBCGPSlider* CBCGPDockingControlBar::GetDefaultSlider () const 
{
	return DYNAMIC_DOWNCAST (CBCGPSlider, CWnd::FromHandlePermanent (m_hDefaultSlider));
}
//*****************************************************************************************
BCGP_CS_STATUS CBCGPDockingControlBar::GetDockStatus (CPoint pt, int nSencitivity) 
{
	ASSERT_VALID (this);

	// detect caption
	UINT nHitTest = HitTest (pt, TRUE);

	CRect rectTabAreaTop;
	CRect rectTabAreaBottom;
	GetTabArea (rectTabAreaTop, rectTabAreaBottom);

	if (nHitTest == HTCAPTION || rectTabAreaTop.PtInRect (pt) ||
		rectTabAreaBottom.PtInRect (pt))
	{
		// need to display "ready to create detachable tab" status
		return CS_DELAY_DOCK_TO_TAB;
	}
	else
	{
		CRect rectBar;
		GetWindowRect (&rectBar);
		
		rectBar.top += GetCaptionHeight ();
		rectBar.top += rectTabAreaTop.Height (); 
		rectBar.bottom -= rectTabAreaBottom.Height (); 

		rectBar.DeflateRect (nSencitivity, nSencitivity);
		if (!rectBar.PtInRect (pt))
		{
			// mouse over an edge
			return CS_DELAY_DOCK;
		}
	}
	return CS_NOTHING;
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::CanAcceptMiniFrame (CBCGPMiniFrameWnd* pMiniFrame) const
{
	return pMiniFrame->CanBeDockedToBar (this);
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::IsFloatingMulti () const
{
	CBCGPMiniFrameWnd* pMiniFrame = GetParentMiniFrame ();
	if (pMiniFrame != NULL)
	{	
		return pMiniFrame->IsKindOf (RUNTIME_CLASS (CBCGPMultiMiniFrameWnd));
	}
	return FALSE;
}
//*****************************************************************************************
void CBCGPDockingControlBar::SetCaptionButtons ()
{
	RemoveCaptionButtons ();

	m_arrButtons.Add (new CBCGPCaptionButton (HTCLOSE_BCG));
	m_arrButtons.Add (new CBCGPCaptionButton (HTMAXBUTTON));
}
//*****************************************************************************************
void CBCGPDockingControlBar::RemoveCaptionButtons ()
{
	for (int i = 0; i < m_arrButtons.GetSize (); i++)
	{
		delete m_arrButtons[i];
	}
	m_arrButtons.RemoveAll ();
}
//*****************************************************************************************
void CBCGPDockingControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CBCGPControlBar::OnSettingChange(uFlags, lpszSection);
	if (m_cyGripper > 0)
	{
		m_cyGripper = 0;
		EnableGripper (TRUE);
	}
}
//*****************************************************************************************
BOOL CBCGPDockingControlBar::OnNeedTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/)
{
	static CString strTipText;

	if (m_ToolTip.GetSafeHwnd () == NULL)
	{
		return FALSE;
	}

	SHORT state = GetAsyncKeyState (VK_CONTROL);
	UINT nAutoHideRes = IDS_BCGBARRES_AUTOHIDEBAR;
	
	if ((state & 0x8000) && IsAutohideAllEnabled ())
	{
		nAutoHideRes = IDS_BCGBARRES_AUTOHIDE_ALL;
	}
	
	LPNMTTDISPINFO	pTTDispInfo	= (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	{
		CBCGPLocalResource locaRes;
		strTipText.LoadString (pNMH->idFrom == 1 ? IDS_BCGBARRES_CLOSEBAR : nAutoHideRes);
	}

	pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR) strTipText);

	return TRUE;
}

void CBCGPDockingControlBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CBCGPControlBar::OnWindowPosChanging(lpwndpos);
}

void CBCGPDockingControlBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (IsTracked ())
	{
		return;
	}
	CBCGPControlBar::OnContextMenu (pWnd, point);
	return; 
}
void CBCGPDockingControlBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPControlBar::OnSetFocus(pOldWnd);

	CBCGPMultiMiniFrameWnd* pParentMiniFrame = 
		DYNAMIC_DOWNCAST (CBCGPMultiMiniFrameWnd, GetParentMiniFrame ());
	if (pParentMiniFrame != NULL)
	{
		pParentMiniFrame->SetLastFocusedBar (GetSafeHwnd ());
	}
}
