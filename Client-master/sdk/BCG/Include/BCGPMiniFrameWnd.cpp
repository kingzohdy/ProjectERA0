// BCGPMiniFrameWnd.cpp : implementation file
//

#include "stdafx.h"

#include "bcgprores.h"
#include "BCGGlobals.h"
#include "BCGPGlobalUtils.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPOleCntrFrameWnd.h"

#include "BCGPControlBar.h"
#include "BCGPDockingControlBar.h"

#include "BCGPToolBar.h"
#include "BCGPToolbarButton.h"
#include "BCGPMiniFrameWnd.h"

#include "BCGPDockBar.h"
#include "BCGPDockManager.h"

#include "BCGPTabbedControlBar.h"
#include "BCGPVisualManager.h"

#include "BCGPToolbarMenuButton.h"
#include "BCGPTabbedToolbar.h"


#include "BCGPCaptionMenuButton.h"
#include "CustomizeButton.h"
#include "BCGPLocalResource.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CBCGPMiniFrameWnd::m_nRollTimeOut = 250;
BOOL CBCGPMiniFrameWnd::m_bUseSaveBits = TRUE;

UINT BCGPM_CHECKEMPTYMINIFRAME		= ::RegisterWindowMessage (_T("BCGPMSG_CHECKEMPTYMINIFRAME"));


IMPLEMENT_SERIAL(CBCGPMiniFrameWnd, CWnd, VERSIONABLE_SCHEMA | 2)

/////////////////////////////////////////////////////////////////////////////
// CBCGPMiniFrameWnd

CMap<UINT,UINT,HWND,HWND> CBCGPMiniFrameWnd::m_mapFloatingBars;
CList<HWND, HWND> CBCGPMiniFrameWnd::m_lstFrames;
CFrameWnd*	CBCGPMiniFrameWnd::m_pParentWndForSerialize = NULL;

CBCGPMiniFrameWnd::CBCGPMiniFrameWnd()	
{
	m_bActive = FALSE;
	m_bCaptured = false;
	m_bBlockMove = false; 
	m_bNoDelayedDestroy = FALSE;

	m_preDockStateCurr = PDS_NOTHING;
	m_pPreDockBar = NULL;
	m_bTabDragRectDisplayed = false;

	RecalcCaptionHeight ();

	m_nHit = HTNOWHERE;
	m_nHot = HTNOWHERE;
	m_rectRedraw.SetRectEmpty ();
	m_nDockTimerID			= 0;
	m_nRollTimerID			= 0;
	m_bRolledUp				= FALSE;
	m_nHeightBeforeRollUp	= 0;
	m_bPinned				= FALSE;

	m_hEmbeddedBar = NULL;

	m_nRestoredEmbeddedBarID = 0;

	m_dwCaptionButtons = 0;
	m_rectRecentFloatingRect.SetRect (0, 0, 100, 100);

	m_hParentWnd = NULL;
	m_hWndToDestroyOnRelease = NULL;
	m_hLastFocusWnd = NULL;
}


CBCGPMiniFrameWnd::~CBCGPMiniFrameWnd()
{
	RemoveAllCaptionButtons ();
}


BEGIN_MESSAGE_MAP(CBCGPMiniFrameWnd, CWnd)
	//{{AFX_MSG_MAP(CBCGPMiniFrameWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVING()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_SIZING()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCCREATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	ON_WM_CANCELMODE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_CANCELMODE()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FLOATSTATUS, OnFloatStatus)
	ON_REGISTERED_MESSAGE(BCGPM_CHECKEMPTYMINIFRAME, OnCheckEmptyState)
	ON_MESSAGE( WM_EXITSIZEMOVE, OnExitSizeMove )
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnNeedTipText)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPMiniFrameWnd message handlers
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBCGPMiniFrameWnd::CreateEx(0, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::CreateEx(DWORD dwStyleEx, LPCTSTR lpszWindowName, 
								 DWORD dwStyle, const RECT& rect, 
								 CWnd* pParentWnd, CCreateContext* pContext)
{
	//-----------------------------
	// Register a new window class:
	//-----------------------------
	HINSTANCE hInst = AfxGetInstanceHandle();
	UINT uiClassStyle = CS_DBLCLKS;
	if (m_bUseSaveBits)
	{
		uiClassStyle |= CS_SAVEBITS;
	}
	HCURSOR hCursor = ::LoadCursor (NULL, IDC_ARROW);
	HBRUSH hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	CString strClassName;
	strClassName.Format (_T("BCGMiniWrame:%x:%x:%x:%x"), 
		(UINT_PTR)hInst, uiClassStyle, (UINT_PTR)hCursor, (UINT_PTR)hbrBackground);

	//---------------------------------
	// See if the class already exists:
	//---------------------------------
	WNDCLASS wndcls;
	if (::GetClassInfo (hInst, strClassName, &wndcls))
	{
		//-----------------------------------------------
		// Already registered, assert everything is good:
		//-----------------------------------------------
		ASSERT (wndcls.style == uiClassStyle);
	}
	else
	{
		//-------------------------------------------
		// Otherwise we need to register a new class:
		//-------------------------------------------
		wndcls.style = uiClassStyle;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = hCursor;
		wndcls.hbrBackground = hbrBackground;
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = strClassName;
		
		if (!AfxRegisterClass (&wndcls))
		{
			AfxThrowResourceException();
		}
	}

	dwStyle |= WS_POPUP;

	m_hParentWnd = pParentWnd != NULL ? pParentWnd->m_hWnd : NULL;


	if (!CWnd::CreateEx (dwStyleEx, strClassName, lpszWindowName, dwStyle, rect, 
						 pParentWnd, 0, pContext))
	{
		return FALSE;
	}

	if (pParentWnd != NULL)
	{
		if (DYNAMIC_DOWNCAST (CFrameWnd, pParentWnd) == NULL)
		{
			TRACE0 ("Minframe parent must be derived from CFrameWnd. Creation failed.\n");
			return FALSE;
		}

		// register with dock manager
		CBCGPDockManager* pDockManager = globalUtils.GetDockManager (pParentWnd);
		ASSERT_VALID (pDockManager);

		if (pDockManager == NULL)
		{
			TRACE0 ("Minframe parent must be connected to dock manager. Creation failed.\n");
			return FALSE;
		}

		pDockManager->AddMiniFrame (this);
	}

	m_dragFrameImpl.Init (this);
	return TRUE;
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::SaveRecentFloatingState ()
{
	GetWindowRect (m_rectRecentFloatingRect);
	// save the recent floating rect
	if (m_hEmbeddedBar != NULL)
	{
		CBCGPControlBar* pContainedBar = DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
		if (pContainedBar != NULL) 
		{
			pContainedBar->m_recentDockInfo.m_rectRecentFloatingRect = m_rectRecentFloatingRect;
		}

		CPoint ptClientHotSpot;
		GetCursorPos (&ptClientHotSpot);
		pContainedBar->ScreenToClient (&ptClientHotSpot);
		pContainedBar->SetClientHotSpot (ptClientHotSpot);

	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_nHot != HTNOWHERE)
	{
		CBCGPCaptionButton* pBtn = FindButton (m_nHot);
		if (pBtn != NULL)
		{
			m_nHit = m_nHot;
			pBtn->m_bPushed = TRUE;
			RedrawCaptionButton (pBtn);
		}

		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	EnterDragMode ();

	SaveRecentFloatingState ();

	CBCGPControlBar* pContainedBar = DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pContainedBar != NULL)
	{
		pContainedBar->m_bWasFloatingBeforeMove = TRUE;
		if (!pContainedBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
		{
			SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
	}
	else
	{
		SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}
//--------------------------------------------------------------------------------------//
int CBCGPMiniFrameWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	if (m_hEmbeddedBar != NULL && IsWindow (m_hEmbeddedBar))
	{
		CWnd* pWnd = CWnd::FromHandle (m_hEmbeddedBar);
		CPoint pt;
		GetCursorPos (&pt);
		ScreenToClient (&pt);
		UINT nHit = HitTest (pt, TRUE);
		if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)) &&
			nHit != HTCLIENT)
		{
			SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
			return MA_NOACTIVATE;
		}
	}

	return CWnd::OnMouseActivate (pDesktopWnd, nHitTest, message);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::EnterDragMode (HWND hWndToDestroyOnRelease)
{
	if (!m_bCaptured)
	{
		SetCapture ();
		if (m_hWndToDestroyOnRelease == NULL)
		{
			m_hWndToDestroyOnRelease = hWndToDestroyOnRelease;
		}

		m_bCaptured = true;

		OnCapture (TRUE);

		GetCursorPos (&m_dragFrameImpl.m_ptHot);

		if (GetDockMode () == DT_IMMEDIATE)
		{
			SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZEALL));
		}
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// m_bCaptured is true when the miniframe is being dragged
	if (m_nHit != HTNOWHERE && !m_bCaptured)
	{
		UINT nHot = m_nHot;
		UINT nHit = m_nHit;

		StopCaptionButtonsTracking ();

		if (nHot == nHit)
		{
			switch (nHit)
			{
			case HTCLOSE:
				if (OnCloseMiniFrame ())
				{
					CloseMiniFrame ();
					return;
				}
				break;

			case HTMAXBUTTON:
				m_bPinned = !m_bPinned;
				break;
				
			default:
				if (!CBCGPToolBar::IsCustomizeMode())
				{
					OnPressButtons (nHit);
				}
			}
		}

		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	if (m_bCaptured)	
	{
		ReleaseCapture ();
		m_bCaptured = false;
		OnCapture (FALSE);
		BOOL bWasDocked = FALSE;
		if (DockControlBar (bWasDocked) || bWasDocked)
		{
			return;
		}
		m_dragFrameImpl.m_rectDrag.SetRectEmpty ();
	}

	CWnd::OnLButtonUp(nFlags, point);
}
//--------------------------------------------------------------------------------------//
CBCGPDockingControlBar* CBCGPMiniFrameWnd::DockControlBar (BOOL& bWasDocked)
{
	CBCGPDockingControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPDockingControlBar, GetControlBar ());

	bWasDocked = FALSE;

	if (GetDockMode () == DT_IMMEDIATE)
	{
		if (m_preDockStateCurr == PDS_DOCK_REGULAR)
		{
			ASSERT_VALID (pBar);
			bWasDocked = pBar->DockByMouse (m_pPreDockBar);
		}
		else if (m_preDockStateCurr == PDS_DOCK_TO_TAB && m_pPreDockBar != NULL )
		{
			CBCGPDockingControlBar* pTabControlBarAttachTo = DYNAMIC_DOWNCAST (CBCGPDockingControlBar, m_pPreDockBar);
			if (pBar == NULL || pTabControlBarAttachTo == NULL)
			{
				ASSERT (FALSE);
				TRACE0 ("Attempt to attach a control bar that is not derived from CBCGPDockingControlBar to tab window. \r\n");
			}
			else
			{
				bWasDocked = TRUE;
				return pBar->AttachToTabWnd (pTabControlBarAttachTo, DM_MOUSE);
			}
		}

		m_preDockStateCurr = PDS_NOTHING;
		return pBar;
	}
	else if (GetDockMode () == DT_STANDARD)
	{
		CRect rectFinal = m_dragFrameImpl.m_rectDrag;
		m_dragFrameImpl.EndDrawDragFrame ();

		CFrameWnd* pFrameWnd = BCGPGetParentFrame (this);

		BOOL	bPrevDisableRecalcLayout	= CBCGPDockManager::m_bDisableRecalcLayout;
		CBCGPDockManager::m_bDisableRecalcLayout = TRUE;
		CBCGPDockingControlBar* pDockedBar = DockControlBarStandard (bWasDocked);
		CBCGPDockManager::m_bDisableRecalcLayout = bPrevDisableRecalcLayout;

		if (pFrameWnd != NULL)
		{
			pFrameWnd->RecalcLayout ();
		}
		if (!rectFinal.IsRectEmpty () && 
			(pDockedBar == NULL || pDockedBar->GetParentMiniFrame () == this) &&
			!bWasDocked)
		{
			MoveWindow (rectFinal);
			if (!IsWindowVisible () && GetControlBarCount () > 0)
			{
				ShowWindow (SW_SHOW);
			}
		}
		return pDockedBar;
	}
	return NULL;
}
//--------------------------------------------------------------------------------------//
CBCGPDockingControlBar* CBCGPMiniFrameWnd::DockControlBarStandard (BOOL& bWasDocked)
{
	CBCGPDockingControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPDockingControlBar, GetControlBar ());

	if (pBar == NULL || !OnBeforeDock ())
	{
		return NULL;
	}
	
	if (!pBar->IsWindowVisible () && pBar->GetDockMode () == DT_STANDARD)
	{
		pBar->ShowWindow (SW_SHOW);
	}

	return DYNAMIC_DOWNCAST (CBCGPDockingControlBar, 
								pBar->DockControlBarStandard (bWasDocked));
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bBlockMove)
	{
		m_bBlockMove = false;
		return;
	}

	if (m_bCaptured)
	{
		globalUtils.m_bIsDragging = TRUE;

		if (GetDockMode () == DT_STANDARD)
		{
			m_dragFrameImpl.MoveDragFrame ();			
		}
		else if (GetDockMode () == DT_IMMEDIATE)
		{
			if (MoveMiniFrame ())
			{
				CPoint ptMouse;
				GetCursorPos (&ptMouse);

				CPoint ptOffset = ptMouse - m_dragFrameImpl.m_ptHot;
		
				CRect rect;
				GetWindowRect (&rect);
				rect.OffsetRect (ptOffset);
				MoveWindow (rect);

				m_dragFrameImpl.m_ptHot = ptMouse;
			}

			SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZEALL));
		}

		globalUtils.m_bIsDragging = FALSE;
		return;
	}

	CPoint ptScreen = point;
	ClientToScreen (&ptScreen);

	OnTrackCaptionButtons (ptScreen);
	CWnd::OnMouseMove(nFlags, point);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::MoveDragFrame ()
{
	m_dragFrameImpl.MoveDragFrame (TRUE);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnMoving(fwSide, pRect);
	MoveMiniFrame ();
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnCheckRollState ()
{
	ASSERT_VALID (this);

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetParent ());
	ASSERT_VALID (pDockManager);	

	if (pDockManager->m_bLockUpdate)
	{
		return;
	}

	CRect rectClient;
	CRect rectWnd;

	GetWindowRect (rectWnd);
	GetClientRect (rectClient);

	BOOL bRollDown = IsRollDown ();
	BOOL bRollUp   = IsRollUp ();

	if (m_bRolledUp && bRollDown)
	{
		rectWnd.bottom = rectWnd.top + m_nHeightBeforeRollUp;
		SetWindowPos (NULL, rectWnd.left,  rectWnd.top, rectWnd.Width (), rectWnd.Height (),
					  SWP_NOZORDER  | SWP_NOACTIVATE);
		m_bRolledUp = FALSE;
	}
	
	if (!m_bRolledUp && bRollUp)
	{
		m_nHeightBeforeRollUp = rectWnd.Height ();
		rectWnd.bottom -= rectClient.Height ();
		SetWindowPos (NULL, rectWnd.left,  rectWnd.top, rectWnd.Width (), rectWnd.Height (),
					  SWP_NOZORDER  | SWP_NOACTIVATE);
		m_bRolledUp = TRUE;
	}
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::IsRollDown () const
{
	CPoint ptMouse;
	GetCursorPos (&ptMouse);

	CRect rectWnd;
	GetWindowRect (rectWnd);

	const CWnd* pWnd = WindowFromPoint (ptMouse);
	return rectWnd.PtInRect (ptMouse) && (pWnd->GetSafeHwnd () == GetSafeHwnd ());
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::IsRollUp () const
{
	CPoint ptMouse;
	GetCursorPos (&ptMouse);

	CRect rectWnd;
	GetWindowRect (rectWnd);

	return !rectWnd.PtInRect (ptMouse) && !m_bPinned;
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::AddControlBar (CBCGPBaseControlBar* pWnd)
{
	ASSERT_VALID (pWnd);

	if (m_hEmbeddedBar != pWnd->GetSafeHwnd ())
	{
		m_hEmbeddedBar = pWnd->GetSafeHwnd ();

		CString strText;
		pWnd->GetWindowText (strText);
		SetWindowText (strText);

		SetIcon (pWnd->GetIcon (FALSE), FALSE);
		SetIcon (pWnd->GetIcon (TRUE), TRUE);

		AddRemoveBarFromGlobalList (pWnd, TRUE);
		if (pWnd->CanBeClosed ())
		{
			if((pWnd->IsKindOf(RUNTIME_CLASS (CBCGPToolBar))))
			{
				CBCGPToolBar* pWndToolBar = (CBCGPToolBar*)pWnd;
				if(pWndToolBar->IsExistCustomizeButton() && pWndToolBar->IsAddRemoveQuickCustomize())
				{
					SetCaptionButtons (BCGP_CAPTION_BTN_CLOSE|BCGP_CAPTION_BTN_CUSTOMIZE);
				}else
				{
					SetCaptionButtons (BCGP_CAPTION_BTN_CLOSE); 
				}

			}else
			{
				SetCaptionButtons (BCGP_CAPTION_BTN_CLOSE); 
			}
		}

		if(pWnd->IsKindOf(RUNTIME_CLASS (CBCGPMenuBar)))
		{
			CBCGPToolBar* pWndToolBar = (CBCGPToolBar*)pWnd;
			if(pWndToolBar->IsExistCustomizeButton())
			{
				SetCaptionButtons (BCGP_CAPTION_BTN_CUSTOMIZE);
			}

		}
		OnSetRollUpTimer ();
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnSetRollUpTimer ()
{
	CBCGPBaseControlBar* pBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, GetControlBar ());
	if (pBar != NULL && pBar->GetBCGStyle () & CBRS_BCGP_AUTO_ROLLUP)
	{
		SetRollUpTimer ();	
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnKillRollUpTimer ()
{
	CBCGPBaseControlBar* pBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, GetControlBar ());

	if (pBar != NULL && (pBar->GetBCGStyle () & CBRS_BCGP_AUTO_ROLLUP) == 0 ||
		pBar == NULL)
	{
		KillRollupTimer ();
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::SetRollUpTimer ()
{
	if (m_nRollTimerID == 0)
	{
		m_nRollTimerID = (UINT) SetTimer (BCGP_CHECK_ROLL_STATE, m_nRollTimeOut, NULL);
		SetCaptionButtons (m_dwCaptionButtons | BCGP_CAPTION_BTN_PIN);
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::KillRollupTimer ()
{
	if (m_nRollTimerID != 0)
	{
		KillTimer (m_nRollTimerID);
		m_nRollTimerID = 0;
		DWORD dwButtons = m_dwCaptionButtons & (~BCGP_CAPTION_BTN_PIN);
		SetCaptionButtons (dwButtons);
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::RemoveControlBar (CBCGPBaseControlBar* pWnd, BOOL bDestroy,
										  BOOL bNoDelayedDestroy)
{
	ASSERT_VALID (this);

	m_bNoDelayedDestroy = bNoDelayedDestroy;

	AddRemoveBarFromGlobalList (pWnd, FALSE);

	pWnd->OnRemoveFromMiniFrame (this);

	if (m_hEmbeddedBar == pWnd->GetSafeHwnd ())
	{
		m_hEmbeddedBar = NULL;
	}

	OnKillRollUpTimer ();

	if (GetControlBarCount () == 0)
	{
		if (bDestroy)
		{
			DestroyWindow ();
		}
		else
		{
			PostMessage (BCGPM_CHECKEMPTYMINIFRAME);
		}
	}
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::AddRemoveBarFromGlobalList (CBCGPBaseControlBar* pWnd, BOOL bAdd)
{
	ASSERT_VALID (pWnd);

	int nID = pWnd->GetDlgCtrlID ();

	if (nID != -1)
	{
		bAdd ? m_mapFloatingBars.SetAt (nID, pWnd->GetSafeHwnd ()) : m_mapFloatingBars.RemoveKey (nID);
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPBaseTabbedBar)))
	{
		CBCGPBaseTabbedBar* pTabbedBar = 
			DYNAMIC_DOWNCAST (CBCGPBaseTabbedBar, pWnd);

		int nTabsNum = pTabbedBar->GetTabsNum ();

		for (int i = 0; i < nTabsNum; i++)
		{
			CWnd* pNextWnd = pTabbedBar->FindBarByTabNumber (i, TRUE);
			ASSERT_VALID (pNextWnd);

			int nBarID = pNextWnd->GetDlgCtrlID ();
			
			if (nBarID == -1)
			{
				TRACE0 ("Tabbed control bar contains a bar with ID = -1\n");
				ASSERT (FALSE);
			}
			else
			{
				bAdd ?  m_mapFloatingBars.SetAt (nBarID, pNextWnd->GetSafeHwnd ()) : 
						m_mapFloatingBars.RemoveKey (nBarID);
			}
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------------------------------------------//

void CBCGPMiniFrameWnd::ReplaceControlBar (CBCGPBaseControlBar* pBarOrg, 
										   CBCGPBaseControlBar* pBarReplaceWith)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBarOrg);
	ASSERT_VALID (pBarReplaceWith);
	ASSERT (pBarOrg != pBarReplaceWith);

	AddRemoveBarFromGlobalList (pBarOrg, FALSE);

	if (pBarOrg->GetSafeHwnd () == m_hEmbeddedBar)
	{
		m_hEmbeddedBar = pBarReplaceWith->GetSafeHwnd ();
	}

	AddRemoveBarFromGlobalList (pBarReplaceWith, TRUE);

	OnSetRollUpTimer ();
}
//--------------------------------------------------------------------------------------//
CWnd* CBCGPMiniFrameWnd::GetControlBar () const
{
	ASSERT_VALID (this);
	
	return CWnd::FromHandlePermanent (m_hEmbeddedBar);
}
//--------------------------------------------------------------------------------------//
CWnd* CBCGPMiniFrameWnd::GetFirstVisibleBar () const
{
	if (GetVisibleBarCount () == 1)
	{
		return GetControlBar ();
	}
	return NULL;
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CRect rectBorderSize;
	CalcBorderSize (rectBorderSize);

	lpncsp->rgrc[0].top += m_nCaptionHeight + rectBorderSize.top;
	lpncsp->rgrc[0].bottom -= rectBorderSize.bottom;
	lpncsp->rgrc[0].left += rectBorderSize.left;
	lpncsp->rgrc[0].right -= rectBorderSize.right;
	
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnNcPaint() 
{
	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetParent ());
	if (pDockManager == NULL)
	{
		return;
	}

	ASSERT_VALID (pDockManager);

	if (pDockManager->m_bLockUpdate)
	{
		return;
	}


	CWindowDC dc(this); // device context for painting
	
	CDC*		pDC = &dc;
	BOOL		m_bMemDC = FALSE;
	CDC			dcMem;
	CBitmap		bmp;
	CBitmap*	pOldBmp = NULL;

	CRect rectWindow;
	GetWindowRect (rectWindow);
	CRect rect;
	rect.SetRect (0, 0, rectWindow.Width(), rectWindow.Height());

	if (dcMem.CreateCompatibleDC (&dc) &&
		bmp.CreateCompatibleBitmap (&dc, rect.Width (),
								  rect.Height ()))
	{
		//-------------------------------------------------------------
		// Off-screen DC successfully created. Better paint to it then!
		//-------------------------------------------------------------
		m_bMemDC = TRUE;
		pOldBmp = dcMem.SelectObject (&bmp);
		pDC = &dcMem;
	}
	
    // client area is not our bussiness :)
    CRect rcClient, rcBar;
    GetWindowRect(rcBar);

    GetClientRect(rcClient);
    ClientToScreen(rcClient);
    rcClient.OffsetRect(-rcBar.TopLeft());

    dc.ExcludeClipRect (rcClient);
	
	CRgn rgn;
	if (!m_rectRedraw.IsRectEmpty ())
	{
		rgn.CreateRectRgnIndirect (m_rectRedraw);
		dc.SelectClipRgn (&rgn);
	}

	// draw border
	OnDrawBorder (pDC);

	CRect rectCaption;
	GetCaptionRect (rectCaption);

	pDockManager = globalUtils.GetDockManager (GetParent ());
	ASSERT_VALID (pDockManager);	
	if (pDockManager->m_bLockUpdate)
	{
		rectCaption.SetRectEmpty ();	
	}

	// draw caption:
	GetCaptionRect (rectCaption);

	COLORREF clrText = CBCGPVisualManager::GetInstance ()->OnFillMiniFrameCaption (
							pDC, rectCaption, this, m_bActive);

	int xBtnsLeft = -1;
	int xBtnsRight = -1;
	for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
		ASSERT_VALID (pBtn);

		if (pBtn->m_bLeftAlign)
		{
			if (xBtnsRight == -1)
			{
				xBtnsRight = pBtn->GetRect ().right + 2;
			}
			else
			{
				xBtnsRight = max (xBtnsRight, pBtn->GetRect ().right + 2);
			}
		}
		else
		{
			if (xBtnsLeft == -1)
			{
				xBtnsLeft = pBtn->GetRect ().left;
			}
			else
			{
				xBtnsLeft = min (xBtnsLeft, pBtn->GetRect ().left);
			}
		}
	}

	// Paint caption text:
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (clrText);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontBold);
	ASSERT_VALID (pOldFont);

	CString strCaption = GetCaptionText ();

	CRect rectText = rectCaption;
	if (xBtnsLeft != -1)
	{
		rectText.right = xBtnsLeft;
	}
	if (xBtnsRight != -1)
	{
		rectText.left = xBtnsRight;
	}

	rectText.DeflateRect (2, 0);

	pDC->DrawText (strCaption, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
	pDC->SelectClipRgn (NULL);

	// Paint caption buttons:
	OnDrawCaptionButtons (pDC);

	if (m_bMemDC)
	{
		//--------------------------------------
		// Copy the results to the on-screen DC:
		//-------------------------------------- 
		CRect rectClip;
		int nClipType = dc.GetClipBox (rectClip);
		if (nClipType != NULLREGION)
		{
			if (nClipType != SIMPLEREGION)
			{
				rectClip = rect;
			}

			dc.BitBlt (rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(),
						   &dcMem, rectClip.left, rectClip.top, SRCCOPY);
		}

		dcMem.SelectObject(pOldBmp);
	}

	CWnd::OnNcPaint ();
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnDrawBorder (CDC* pDC)
{
	ASSERT_VALID (pDC);
	
	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetParent ());
	if (pDockManager == NULL)
	{
		return;
	}

	ASSERT_VALID (pDockManager);

	if (pDockManager->m_bLockUpdate)
	{
		return;
	}

	CRect rectWnd;
	GetWindowRect (&rectWnd);
	ScreenToClient (&rectWnd);

	CRect rectBorderSize;
	CalcBorderSize (rectBorderSize);
	
	rectWnd.OffsetRect (rectBorderSize.left, m_nCaptionHeight + rectBorderSize.top);

	CRect rectBorder = rectWnd;

	CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, GetControlBar ());

	if (pToolBar != NULL)
	{
		pDC->FillRect (rectBorder, &globalData.brBtnFace);

		CBCGPVisualManager::GetInstance ()->OnDrawFloatingToolbarBorder (
			pDC, pToolBar, rectBorder, rectBorderSize);
		return;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawMiniFrameBorder (
		pDC, this, rectBorder, rectBorderSize);

}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnDrawCaptionButtons (CDC* pDC)
{
	ASSERT_VALID (pDC);

	// Paint caption buttons:
	for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
		ASSERT_VALID (pBtn);

		BOOL bMaximized = TRUE;
		if (pBtn->GetHit () == HTMAXBUTTON && m_bPinned)
		{
			bMaximized = FALSE;
		}

		BOOL bEnabled = TRUE;
		if(CBCGPToolBar::IsCustomizeMode())
		{
			if ((pBtn->GetHit () == HTCLOSE) || (pBtn->GetHit () == HTCLOSE_BCG))
			{
				bEnabled = TRUE;
			}
			pBtn->m_bEnabled = bEnabled;
		}

		pBtn->OnDraw (pDC, TRUE, TRUE, bMaximized, !bEnabled);
	}
}
//--------------------------------------------------------------------------------------//
UINT CBCGPMiniFrameWnd::OnNcHitTest(CPoint point) 
{
	return HitTest (point, FALSE);
}
//--------------------------------------------------------------------------------------//
UINT CBCGPMiniFrameWnd::HitTest (CPoint point, BOOL bDetectCaption)
{
		// in cust. mode allow mouse processing only for floating toolbar
	if (IsCustModeAndNotFloatingToolbar ())
	{
		return HTNOWHERE;
	}

	CRect rectWnd;
	GetWindowRect (&rectWnd);

	if (!rectWnd.PtInRect (point))
	{
		return HTNOWHERE;
	}

	CRect rectClient;
	GetClientRect (&rectClient);
	ClientToScreen (&rectClient);

	if (rectClient.PtInRect (point))
	{
		return HTCLIENT;
	}

	CRect rectBorderSize;
	CalcBorderSize (rectBorderSize);

	int nCursorWidth  = GetSystemMetrics (SM_CXCURSOR) / 2;
	int nCursorHeight = GetSystemMetrics (SM_CYCURSOR) / 2;
	
	CRect rectCaption (rectWnd.left + rectBorderSize.left, rectWnd.top + rectBorderSize.top, 
						 rectWnd.right - rectBorderSize.right, 
						 rectWnd.top + rectBorderSize.top + m_nCaptionHeight);

	if (rectCaption.PtInRect (point))
	{
		if (bDetectCaption)
		{
			return HTCAPTION;
		}

		for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL;)
		{
			CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
			ASSERT_VALID (pBtn);

			// the button's rectangle has offset relative to caption, we need to offset it
			// by caption's topleft corner to get client coordinates
			CRect rectBtn = pBtn->GetRect ();
			rectBtn.OffsetRect (rectCaption.TopLeft ());

			if (rectBtn.PtInRect (point))
			{
				return pBtn->GetHit ();
			}
		}

		return HTCLIENT;
	}

	BOOL bEnableCornerArrows = TRUE;

	CWnd* pWndEmbedded  = CWnd::FromHandlePermanent (m_hEmbeddedBar);
	if (pWndEmbedded != NULL && pWndEmbedded->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
	{
		bEnableCornerArrows = FALSE;
	}

	// no corner arrows in sliding mode
	bEnableCornerArrows = bEnableCornerArrows;

	CRect rectBorder;

	if (bEnableCornerArrows)
	{
		// top left corner - border
		rectBorder.SetRect (rectWnd.left, rectWnd.top, rectWnd.left + nCursorWidth, rectWnd.top + nCursorHeight);
		if (rectBorder.PtInRect (point))
		{
			return HTTOPLEFT;
		}

		// top border
		rectBorder.SetRect (rectWnd.left + nCursorWidth, rectWnd.top, 
					  rectWnd.right - nCursorWidth, rectWnd.top + rectBorderSize.top);
		if (rectBorder.PtInRect (point))
		{
			return HTTOP;
		}

		// top right border
		rectBorder.SetRect (rectWnd.right - nCursorWidth, rectWnd.top, 
					  rectWnd.right, rectWnd.top + nCursorHeight);
		if (rectBorder.PtInRect (point))
		{
			return HTTOPRIGHT;
		}

		// right border
		rectBorder.SetRect (rectWnd.right - rectBorderSize.right, rectWnd.top + nCursorHeight, 
					  rectWnd.right, rectWnd.bottom - nCursorHeight);
		if (rectBorder.PtInRect (point))
		{
			return HTRIGHT;
		}

		// bottom right
		rectBorder.SetRect (rectWnd.right - nCursorWidth, rectWnd.bottom - nCursorHeight, 
					  rectWnd.right, rectWnd.bottom);
		if (rectBorder.PtInRect (point))
		{
			return HTBOTTOMRIGHT;
		}

		// bottom
		rectBorder.SetRect (rectWnd.left + nCursorWidth, rectWnd.bottom - rectBorderSize.bottom, 
					  rectWnd.right - nCursorWidth, rectWnd.bottom);
		if (rectBorder.PtInRect (point))
		{
			return HTBOTTOM;
		}

		// bottom left
		rectBorder.SetRect (rectWnd.left, rectWnd.bottom - nCursorHeight, 
					  rectWnd.left + nCursorWidth, rectWnd.bottom);
		if (rectBorder.PtInRect (point))
		{
			return HTBOTTOMLEFT;
		}

		// left
		rectBorder.SetRect (rectWnd.left, rectWnd.top + nCursorHeight, 
					  rectWnd.left + rectBorderSize.left, rectWnd.bottom - nCursorHeight);

		if (rectBorder.PtInRect (point))
		{
			return HTLEFT;
		}
	}
	else
	{
		// in sliding mode only one side enabled for sliding
		UINT uEnabledSizingSide = 0;

		// top border
		rectBorder.SetRect (rectWnd.left, rectWnd.top, rectWnd.right, 
					  rectWnd.top + rectBorderSize.top);
		if (rectBorder.PtInRect (point))
		{
			if (uEnabledSizingSide != 0 && uEnabledSizingSide != HTTOP)
			{
				return HTBORDER; // no sizing
			}
			return HTTOP;
		}

		// left
		rectBorder.SetRect (rectWnd.left, rectWnd.top, 
					  rectWnd.left + rectBorderSize.left, rectWnd.bottom);

		if (rectBorder.PtInRect (point))
		{
			if (uEnabledSizingSide != 0 && uEnabledSizingSide != HTLEFT)
			{
				return HTBORDER; // no sizing
			}
			return HTLEFT;
		}

		// bottom
		rectBorder.SetRect (rectWnd.left, rectWnd.bottom - rectBorderSize.bottom, 
					  rectWnd.right, rectWnd.bottom);
		if (rectBorder.PtInRect (point))
		{
			if (uEnabledSizingSide != 0 && uEnabledSizingSide != HTBOTTOM)
			{
				return HTBORDER; // no sizing
			}
			return HTBOTTOM;
		}

		// right border
		rectBorder.SetRect (rectWnd.right - rectBorderSize.right, rectWnd.top, 
					  rectWnd.right, rectWnd.bottom);
		if (rectBorder.PtInRect (point))
		{
			if (uEnabledSizingSide != 0 && uEnabledSizingSide != HTRIGHT)
			{
				return HTBORDER; // no sizing
			}
			return HTRIGHT;
		}
	}
	return CWnd::OnNcHitTest(point);
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (nHitTest)
	{
	case HTTOPLEFT:
	case HTBOTTOMRIGHT:
		SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZENWSE));
		return TRUE;

	case HTTOP:
	case HTBOTTOM:
		SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZENS));
		return TRUE;

	case HTRIGHT:
	case HTLEFT:
		SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZEWE));
		return TRUE;

	case HTTOPRIGHT:
	case HTBOTTOMLEFT:
		SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZENESW));
		return TRUE;

	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnSizing(fwSide, pRect);

	if (m_bRolledUp)
	{
		// can't be resized when rolled up
		return;
	}

	CRect rectOldWnd;
	GetWindowRect (rectOldWnd);

	CBCGPTabbedToolbar* pTabbedBar = DYNAMIC_DOWNCAST (CBCGPTabbedToolbar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pTabbedBar != NULL)
	{
		*pRect = rectOldWnd;
		// can't be resized when contains tabbed toolbar
		return;
	}

	
	CRect rect (pRect);

	CRect rectOldClient;
	GetClientRect (rectOldClient);

	BOOL bHorz = (fwSide == WMSZ_LEFT || fwSide == WMSZ_RIGHT);

	if (bHorz && rect.Width () == rectOldWnd.Width () ||
		!bHorz && rect.Height () == rectOldWnd.Height ())
	{
		return;
	}

	CBCGPToolBar* pNextBar = DYNAMIC_DOWNCAST (CBCGPToolBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pNextBar == NULL)
	{
		return;
	}

	CRect rectBorderSize;
	CalcBorderSize (rectBorderSize);

	int nNewHeight = rect.Height () - 
		rectBorderSize.top - rectBorderSize.bottom - m_nCaptionHeight;

	int nNewWidth  = rect.Width () - rectBorderSize.left - rectBorderSize.right;

	CSize sizeBarOld = pNextBar->CalcSize (FALSE);

	CSize sizeBar = 
		pNextBar->StretchControlBar (bHorz ? nNewWidth  : nNewHeight, !bHorz);

	int nXBarDelta = sizeBar.cx - sizeBarOld.cx;
	int nYBarDelta = sizeBar.cy - sizeBarOld.cy;

	if (nXBarDelta == 0 && nYBarDelta == 0 && pNextBar->IsKindOf (RUNTIME_CLASS (CBCGPBaseToolBar)))
	{
		*pRect = rectOldWnd;
		return;
	}

	if (nXBarDelta != 0)
	{
		if (fwSide == WMSZ_RIGHT)
		{
			pRect->right = pRect->left + rectOldWnd.Width () + nXBarDelta;
		}
		else
		{
			pRect->left = pRect->right - rectOldWnd.Width () - nXBarDelta;
		}
	}
	else
	{
		fwSide == WMSZ_RIGHT ? 
			pRect->right = rectOldWnd.right : pRect->left = rectOldWnd.left;
	}

	if (nYBarDelta != 0)
	{
		if (fwSide == WMSZ_BOTTOM || fwSide == WMSZ_RIGHT || fwSide == WMSZ_LEFT)
		{
			pRect->bottom = pRect->top + rectOldWnd.Height() + nYBarDelta;
		}
		else
		{
			pRect->top = pRect->bottom - rectOldWnd.Height() - nYBarDelta;
		}
	}
	else
	{
		fwSide == WMSZ_BOTTOM ? 
			pRect->bottom = rectOldWnd.bottom : pRect->top = rectOldWnd.top;
	}

	pNextBar->RecalcLayout ();
	
	BOOL bParam = FALSE;
	SystemParametersInfo (SPI_GETDRAGFULLWINDOWS, 0, &bParam, 0);
	if (!bParam && pNextBar->IsKindOf (RUNTIME_CLASS (CBCGPBaseToolBar)))
	{
		SetWindowPos (NULL, pRect->left, pRect->top, 
							pRect->right - pRect->left, pRect->bottom - pRect->top,
							SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CBCGPControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pBar != NULL)
	{
		pBar->RecalcLayout ();
	}	
	
	CRect rect;
	GetWindowRect (rect);

	globalUtils.AdjustRectToWorkArea (rect);
	SetWindowPos (NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	ArrangeCaptionButtons ();
	SendMessage (WM_NCPAINT);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::SizeToContent ()
{
	ASSERT_VALID (this);

	CBCGPControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pBar != NULL)
	{
		CSize sizeBar = pBar->CalcFixedLayout (FALSE, TRUE);

		CRect rectWnd;
		GetWindowRect (rectWnd);

		CRect rectClient;
		GetClientRect (rectClient);
		int nXDelta = rectClient.Width () - sizeBar.cx;
		int nYDelta = rectClient.Height () - sizeBar.cy;

		SetWindowPos (NULL, 0, 0, rectWnd.Width () - nXDelta, 
								  rectWnd.Height () - nYDelta, 
								  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnBarRecalcLayout ()
{
	CRect rect;
	GetClientRect (&rect);
	CBCGPControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pBar != NULL)
	{
		pBar->SetWindowPos (NULL, rect.left, rect.top, rect.Width (), rect.Height (), 
					  SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
		pBar->RedrawWindow ();
	}
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::SetPreDockState (BCGP_PREDOCK_STATE preDockState, 
										 CBCGPBaseControlBar* pBarToDock,
										 BCGP_DOCK_METHOD /*dockMethod*/)
{
	if (preDockState == PDS_NOTHING || 
		preDockState == PDS_DOCK_TO_TAB && 
		pBarToDock != NULL && 
		!pBarToDock->CanBeAttached ())
	{
		return TRUE;
	}
	
	if (pBarToDock != NULL && pBarToDock->GetParentMiniFrame () != NULL)
	{
		return TRUE;
	}

	BOOL bWasCaptured = m_bCaptured;
	if (m_bCaptured)	
	{
		ReleaseCapture ();
		m_bCaptured = false;
		OnCapture (FALSE);
	}

	CPoint ptScreen;
	GetCursorPos (&ptScreen);
	
	CPoint ptClientBar = ptScreen;

	CBCGPDockingControlBar* pWnd = (CBCGPDockingControlBar*) GetControlBar ();

	pWnd->ScreenToClient (&ptClientBar);		

	m_preDockStateCurr = preDockState;
	m_pPreDockBar = DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pBarToDock);

	// it will be different bar in case
	// of tab window
	BOOL bWasDocked = FALSE;
	CBCGPDockingControlBar* pDockedBar = DockControlBar (bWasDocked);
	
	if (pDockedBar != NULL)
	{
		pDockedBar->AdjustDockingLayout ();
		if (bWasCaptured)	
		{
			pDockedBar->EnterDragMode (FALSE);
		}
	}

	return FALSE;
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnMoveControlBar (CBCGPControlBar* /*pBar*/, CPoint ptOffset)
{
	CRect rectMiniFrame;
	GetWindowRect (rectMiniFrame);
	rectMiniFrame.OffsetRect (ptOffset);
	MoveWindow (rectMiniFrame);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags |= SWP_NOACTIVATE;
	if ((lpwndpos->flags & SWP_NOMOVE) == 0)
	{
		CRect rectWnd;
		GetWindowRect (&rectWnd);
		CRect rect;

		if (lpwndpos->flags & SWP_NOSIZE)
		{
			rect.SetRect (lpwndpos->x, lpwndpos->y, 
							lpwndpos->x + rectWnd.Width (), 
							lpwndpos->y + rectWnd.Height ());
		}
		else
		{
			rect.SetRect (lpwndpos->x, lpwndpos->y, 
							lpwndpos->x + lpwndpos->cx, 
							lpwndpos->y + lpwndpos->cy);
		}

		int captionHeight = GetCaptionHeight ();
		CRect rectDelta (captionHeight, captionHeight, captionHeight, 
							captionHeight);
		globalUtils.AdjustRectToWorkArea (rect, &rectDelta);
		lpwndpos->x = rect.left;
		lpwndpos->y = rect.top;
	}

	CWnd::OnWindowPosChanging(lpwndpos);
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::MoveMiniFrame ()
{
	CWnd* pParent = GetParent ();
	
	if (pParent == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	if (pParent->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		return ((CBCGPFrameWnd*) pParent)->OnMoveMiniFrame (this);
	}
	else if (pParent->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		return ((CBCGPMDIFrameWnd*) pParent)->OnMoveMiniFrame (this);
	}
	else if (pParent->IsKindOf (RUNTIME_CLASS (CBCGPOleIPFrameWnd)))
	{
		return ((CBCGPOleIPFrameWnd*) pParent)->OnMoveMiniFrame (this);
	}
	else if (pParent->IsKindOf (RUNTIME_CLASS (CBCGPOleDocIPFrameWnd)))
	{
		return ((CBCGPOleDocIPFrameWnd*) pParent)->OnMoveMiniFrame (this);
	}
	else if (pParent->IsKindOf (RUNTIME_CLASS (CBCGPMDIChildWnd)))
	{
		return ((CBCGPMDIChildWnd*) pParent)->OnMoveMiniFrame (this);
	}
	else if (pParent->IsKindOf (RUNTIME_CLASS (CBCGPOleCntrFrameWnd)))
	{
		return ((CBCGPOleCntrFrameWnd*) pParent)->OnMoveMiniFrame (this);
	}
	else 
	{
		ASSERT (FALSE);
	}
	return FALSE;
}
//--------------------------------------------------------------------------------------//
CString CBCGPMiniFrameWnd::GetCaptionText ()
{
	if (m_hEmbeddedBar == NULL)
	{
		return _T ("");
	}

	CString strCaption;
	CWnd* pEmbeddedWnd = CWnd::FromHandlePermanent (m_hEmbeddedBar);
	if (pEmbeddedWnd != NULL)
	{
		pEmbeddedWnd->GetWindowText (strCaption);
	}

	return strCaption;
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case BCGP_DOCK_EVENT:
		{
			CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetParent ());
			ASSERT_VALID (pDockManager);
			pDockManager->OnMoveMiniFrame (this);
			return;
		}
	case BCGP_CHECK_ROLL_STATE:
		OnCheckRollState ();
		break;
	}

	CWnd::OnTimer(nIDEvent);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::CalcBorderSize (CRect& rectBorderSize) const
{
	if (DYNAMIC_DOWNCAST (CBCGPToolBar, GetControlBar ()) != NULL)
	{
		rectBorderSize.SetRect (g_nToolbarBorderSize, g_nToolbarBorderSize, 
								g_nToolbarBorderSize, g_nToolbarBorderSize);
	}
	else
	{
		rectBorderSize.SetRect (GetSystemMetrics (SM_CXSIZEFRAME), 
								GetSystemMetrics (SM_CYSIZEFRAME),
								GetSystemMetrics (SM_CXSIZEFRAME),
								GetSystemMetrics (SM_CYSIZEFRAME));
	}
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::OnNcActivate(BOOL bActive) 
{
	if ((GetStyle() & MFS_SYNCACTIVE) == 0)
	{
		bActive = (GetFocus () == this);
		if (m_bActive != bActive)
		{
			m_bActive = bActive;
			
			CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetParent ());
			if (pDockManager != NULL)
			{
				SendMessage(WM_NCPAINT);
			}
		}
	}
	else if(m_nFlags & WF_KEEPMINIACTIVE)
	{
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::SetCaptionButtons (DWORD dwButtons)
{
	ASSERT_VALID (this);
	RemoveAllCaptionButtons ();

	if (dwButtons & BCGP_CAPTION_BTN_CLOSE)
	{
		CBCGPBaseControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPBaseControlBar, GetControlBar ());
		if (pBar != NULL && pBar->CanBeClosed ())
		{
			AddButton (HTCLOSE);
		}
	}

	if (dwButtons & BCGP_CAPTION_BTN_PIN)
	{
		AddButton (HTMAXBUTTON);
	}

	if (dwButtons & BCGP_CAPTION_BTN_MENU)
	{
		AddButton (HTMINBUTTON);
	}

	if(dwButtons & BCGP_CAPTION_BTN_CUSTOMIZE)
	{
		AddButton(HTMENU_BCG); 
	}


	m_dwCaptionButtons = dwButtons;
	SetCaptionButtonsToolTips ();

	ArrangeCaptionButtons ();
	SendMessage (WM_NCPAINT);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::AddButton (UINT nHit)
{
	ASSERT_VALID (this);

	CBCGPCaptionButton* pBtn = FindButton (nHit);

	if (pBtn == NULL)
	{
		switch (nHit) 
		{
			case HTMENU_BCG:
			{
				CBCGPCaptionMenuButton *pMenuBtn = new CBCGPCaptionMenuButton;
				pMenuBtn->m_bOSMenu = FALSE;
				pMenuBtn->m_nHit = HTMENU_BCG;
				m_lstCaptionButtons.AddHead (pMenuBtn);
				break;
			}

			default:
			pBtn = new CBCGPCaptionButton;
			m_lstCaptionButtons.AddHead (pBtn);
			pBtn->m_nHit = nHit;
			break;
		}
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::RemoveButton (UINT nHit)
{
	ASSERT_VALID (this);

	for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL;)
	{
		POSITION posSave = pos;

		CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
		ASSERT_VALID (pBtn);

		if (pBtn->m_nHit == nHit)
		{
			m_lstCaptionButtons.RemoveAt (posSave);
			delete pBtn;
			break;
		}
	}

	ArrangeCaptionButtons ();
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::ReplaceButton (UINT nHit, UINT nHitNew)
{
	ASSERT_VALID (this);

	CBCGPCaptionButton* pBtn = FindButton (nHit);
	if (pBtn != NULL)
	{
		pBtn->m_nHit = nHitNew;
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::ShowButton (UINT nHit, BOOL bShow)
{
	ASSERT_VALID (this);
	CBCGPCaptionButton* pBtn = FindButton (nHit);
	if (pBtn != NULL)
	{
		pBtn->m_bHidden = bShow;
		ArrangeCaptionButtons ();
	}
}
//--------------------------------------------------------------------------------------//
CBCGPCaptionButton* CBCGPMiniFrameWnd::FindButton (UINT uiHit) const
{
	ASSERT_VALID (this);

	for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
		ASSERT_VALID (pBtn);

		if (pBtn->GetHit () == uiHit)
		{
			return pBtn;
		}
	}

	return NULL;
}
//--------------------------------------------------------------------------------------//
CBCGPCaptionButton* CBCGPMiniFrameWnd::FindButton (CPoint point) const
{
	ASSERT_VALID (this);

	CRect rectWnd;
	GetWindowRect (&rectWnd);

	for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
		ASSERT_VALID (pBtn);

		CRect rectBtn = pBtn->GetRect ();
		rectBtn.OffsetRect (rectWnd.TopLeft ());

		if (rectBtn.PtInRect (point))
		{
			return pBtn;
		}
	}

	return NULL;
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::RemoveAllCaptionButtons ()
{
	ASSERT_VALID (this);

	m_dwCaptionButtons = 0;
	
	while (!m_lstCaptionButtons.IsEmpty ())
	{
		delete m_lstCaptionButtons.RemoveHead ();
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::UpdateTooltips ()
{
	// Update tool area for the tooltips:
	if (m_ToolTip.GetSafeHwnd() != NULL)
	{
		CRect rcBar;
		GetWindowRect(rcBar);
		ScreenToClient (rcBar);
		int i = 0;
		for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL; i++)
		{
			CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
			ASSERT_VALID (pBtn);

			if (i < m_ToolTip.GetToolCount())
			{
				CRect rectTT;
				rectTT.SetRectEmpty ();
				if (!pBtn->m_bHidden)
				{
					rectTT = pBtn->GetRect ();
					rectTT.OffsetRect (rcBar.TopLeft());
				}
				m_ToolTip.SetToolRect (this, i + 1, rectTT);
			}
		}
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::ArrangeCaptionButtons ()
{
	ASSERT_VALID (this);

	CRect rectCaption;
	GetCaptionRect (rectCaption);

	CSize btnSize = CBCGPCaptionButton::GetSize ();
	CPoint ptOrgRight (rectCaption.right - g_nCaptionButtonMargin, 
					rectCaption.top + (rectCaption.Height () - btnSize.cy) / 2);
	CPoint ptOrgLeft (rectCaption.left + g_nCaptionButtonMargin, 
					rectCaption.top + (rectCaption.Height () - btnSize.cy) / 2);

	int i = 0;
	for (POSITION pos = m_lstCaptionButtons.GetTailPosition (); pos != NULL; i++)
	{
		CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetPrev (pos);
		ASSERT_VALID (pBtn);

		if (!pBtn->m_bHidden)
		{
			if (pBtn->m_bLeftAlign)
			{
				pBtn->Move (ptOrgLeft);
				ptOrgLeft.x += btnSize.cx;
				ptOrgLeft.x += g_nCaptionButtonSpacing;
			}
			else
			{
				ptOrgRight.x -= btnSize.cx;
				pBtn->Move (ptOrgRight);
				ptOrgRight.x -= g_nCaptionButtonSpacing;
			}
		}
	}

	UpdateTooltips ();
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::RedrawCaptionButton (CBCGPCaptionButton* pBtn)
{
	ASSERT_VALID(this);

	if (pBtn ==	NULL)
	{
		return;
	}

	ASSERT_VALID (pBtn);

	m_rectRedraw = pBtn->GetRect ();
	SendMessage (WM_NCPAINT);
	m_rectRedraw.SetRectEmpty ();

	UpdateWindow ();
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::CloseMiniFrame ()
{
	ShowWindow (SW_HIDE);
	
	if (m_hEmbeddedBar != NULL)
	{
		CWnd* pEmbeddedWnd = CWnd::FromHandlePermanent (m_hEmbeddedBar);
		if (pEmbeddedWnd != NULL)
		{
			pEmbeddedWnd->ShowWindow (SW_HIDE);
		}
	}
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnNcLButtonDblClk(nHitTest, point);
}
//--------------------------------------------------------------------------------------//
void CBCGPMiniFrameWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	CPoint ptScreen = point;
	ClientToScreen (&ptScreen);

	CBCGPCaptionButton* pBtn = FindButton (ptScreen);
	if (pBtn != NULL)
	{
		CWnd::OnLButtonDblClk(nFlags, point);
		return;
	}

	CBCGPControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));	

	if (pBar != NULL && pBar->GetEnabledAlignment () & CBRS_ALIGN_ANY)
	{
		SaveRecentFloatingState ();

		if (pBar->DockControlBar (pBar, NULL, DM_DBL_CLICK))
		{
			// was destroyed if DockControlBar returned TRUE
			return;
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= MFS_SYNCACTIVE;
	return CWnd::PreCreateWindow(cs);
}
//--------------------------------------------------------------------------------------//
BOOL CBCGPMiniFrameWnd::OnNcCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!CWnd::OnNcCreate(lpCreateStruct))
		return FALSE;
	
	if (GetStyle() & MFS_SYNCACTIVE)
	{
		// syncronize activation state with top level parent
		CWnd* pParentWnd = GetTopLevelParent();
		ASSERT(pParentWnd != NULL);
		CWnd* pActiveWnd = GetForegroundWindow();
		BOOL bActive = (pParentWnd == pActiveWnd) ||
			(pParentWnd->GetLastActivePopup() == pActiveWnd &&
			 pActiveWnd->SendMessage(WM_FLOATSTATUS, FS_SYNCACTIVE) != 0);

		// the WM_FLOATSTATUS does the actual work
		SendMessage(WM_FLOATSTATUS, bActive ? FS_ACTIVATE : FS_DEACTIVATE);
	}
	
	return TRUE;
}
//--------------------------------------------------------------------------------------//
LRESULT CBCGPMiniFrameWnd::OnFloatStatus(WPARAM wParam, LPARAM)
{
	// these asserts make sure no conflicting actions are requested
	ASSERT(!((wParam & FS_SHOW) && (wParam & FS_HIDE)));
	ASSERT(!((wParam & FS_ENABLE) && (wParam & FS_DISABLE)));
	ASSERT(!((wParam & FS_ACTIVATE) && (wParam & FS_DEACTIVATE)));

	// FS_SYNCACTIVE is used to detect MFS_SYNCACTIVE windows
	LRESULT lResult = 0;
	if ((GetStyle() & MFS_SYNCACTIVE) && (wParam & FS_SYNCACTIVE))
		lResult = 1;

	if (wParam & (FS_SHOW|FS_HIDE))
	{
		SetWindowPos(NULL, 0, 0, 0, 0,
			((wParam & FS_SHOW) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW) | SWP_NOZORDER |
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}
	if (wParam & (FS_ENABLE|FS_DISABLE))
		EnableWindow((wParam & FS_ENABLE) != 0);

	if ((wParam & (FS_ACTIVATE|FS_DEACTIVATE)) &&
		GetStyle() & MFS_SYNCACTIVE)
	{
		ModifyStyle(MFS_SYNCACTIVE, 0);
		SendMessage(WM_NCACTIVATE, (wParam & FS_ACTIVATE) != 0);
		ModifyStyle(0, MFS_SYNCACTIVE);
	}

	return lResult;
}
//**************************************************************************************
BOOL CBCGPMiniFrameWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//**************************************************************************************
void CBCGPMiniFrameWnd::OnNcDestroy() 
{
	OnCancelMode();
	CWnd::OnNcDestroy();
	delete this;	
}
//**************************************************************************************
void CBCGPMiniFrameWnd::OnCancelMode() 
{
	StopCaptionButtonsTracking ();

	if (m_bCaptured)	
	{
		if (GetDockMode () == DT_STANDARD)
		{
			m_dragFrameImpl.EndDrawDragFrame ();
			if (!IsWindowVisible ())
			{
				// probably dragging control bar detached from tab window
				CBCGPControlBar* pBar = 
					DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));	
				if (pBar != NULL && pBar->GetParent () == this)
				{
					BOOL bResult = pBar->DockControlBar (pBar, NULL, DM_DBL_CLICK);
					pBar->ShowControlBar (TRUE, FALSE, TRUE);
					if (!bResult)
					{
						return;
					}
				}
			}
		}
		ReleaseCapture ();
		m_bCaptured = false;
		OnCapture (FALSE);
	}

	CWnd::OnCancelMode();
}
//**************************************************************************************
void CBCGPMiniFrameWnd::GetCaptionRect (CRect& rectCaption) const
{
	CRect rectBorderSize;
	rectBorderSize.SetRectEmpty (); 
	CalcBorderSize (rectBorderSize);

	CRect rectWnd;
	GetWindowRect (&rectWnd);
	ScreenToClient (&rectWnd);
	rectWnd.OffsetRect (rectBorderSize.left, m_nCaptionHeight + rectBorderSize.top);

	rectCaption = CRect (rectWnd.left + rectBorderSize.left, rectWnd.top + rectBorderSize.top, 
						 rectWnd.right - rectBorderSize.right, 
						 rectWnd.top + rectBorderSize.top + m_nCaptionHeight);

}
//**************************************************************************************
void CBCGPMiniFrameWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	if (!m_bBlockMove && !m_bCaptured)
	{
		OnTrackCaptionButtons (point);
	}
	
	CWnd::OnNcMouseMove(nHitTest, point);
}
//*************************************************************************************
void CBCGPMiniFrameWnd::OnTrackCaptionButtons (CPoint point)
{
	UINT nHot = m_nHot;

	CBCGPCaptionButton* pBtn = FindButton (point);
	if (pBtn != NULL && pBtn->m_bEnabled)
	{
		m_nHot = pBtn->GetHit ();
		pBtn->m_bFocused = TRUE;
	}
	else
	{
		m_nHot = HTNOWHERE;
	}

	if (m_nHot != nHot)
	{
		RedrawCaptionButton (pBtn);

		CBCGPCaptionButton* pBtnOld = FindButton (nHot);
		if (pBtnOld != NULL)
		{
			pBtnOld->m_bFocused = FALSE;
			RedrawCaptionButton (pBtnOld);
		}
	}

	if (m_nHit == HTNOWHERE)
	{
		if (nHot != HTNOWHERE && m_nHot == HTNOWHERE)
		{
			::ReleaseCapture();
		}
		else if (nHot == HTNOWHERE && m_nHot != HTNOWHERE)
		{
			SetCapture ();
		}
	}
}
//************************************************************************************
void CBCGPMiniFrameWnd::StopCaptionButtonsTracking ()
{
	if (m_nHit != HTNOWHERE)
	{
		CBCGPCaptionButton* pBtn = FindButton (m_nHit);
		m_nHit = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bPushed = FALSE;
			RedrawCaptionButton (pBtn);
		}
	}

	if (m_nHot != HTNOWHERE)
	{
		CBCGPCaptionButton* pBtn = FindButton (m_nHot);
		m_nHot = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bFocused = FALSE;
			RedrawCaptionButton (pBtn);
		}
	}
}
//************************************************************************************
BOOL CBCGPMiniFrameWnd::IsCustModeAndNotFloatingToolbar () const
{
	CWnd* pEmbeddedWnd = CWnd::FromHandlePermanent (m_hEmbeddedBar);
	if (CBCGPToolBar::IsCustomizeMode () && pEmbeddedWnd != NULL && 
		!pEmbeddedWnd->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
	{
		return TRUE;
	}
	return FALSE;
}
//************************************************************************************
int  CBCGPMiniFrameWnd::GetCaptionButtonTotalWidth () const
{
	int nTotalWidth = 0;
	for (POSITION pos = m_lstCaptionButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*) m_lstCaptionButtons.GetNext (pos);
		ASSERT_VALID (pBtn);
	
		CRect rectBtn = pBtn->GetRect ();
		nTotalWidth += rectBtn.Width ();
	}
	return nTotalWidth;
}
//************************************************************************************
void CBCGPMiniFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CBCGPControlBar* pContainedBar = 
		DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	
	if (pContainedBar != NULL)
	{
		CSize sizeBase;  
		pContainedBar->GetMinSize (sizeBase);

		CalcMinSize (sizeBase, lpMMI);
	}
	
	CWnd::OnGetMinMaxInfo(lpMMI);
}
//*****************************************************************************************
void CBCGPMiniFrameWnd::CalcMinSize (CSize& sizeBase, MINMAXINFO FAR* lpMMI)
{
	CRect rectBorderSize;
	CalcBorderSize (rectBorderSize);

	lpMMI->ptMinTrackSize.x = sizeBase.cx + 
									rectBorderSize.left + rectBorderSize.right + 
									GetCaptionButtonTotalWidth ();

	lpMMI->ptMinTrackSize.x = max (lpMMI->ptMinTrackSize.x, m_sizeMinSize.cx);

	lpMMI->ptMinTrackSize.y = sizeBase.cy + m_nCaptionHeight + 
								rectBorderSize.top + rectBorderSize.bottom;

	lpMMI->ptMinTrackSize.y = max (lpMMI->ptMinTrackSize.y, m_sizeMinSize.cy);
}
//*****************************************************************************************
void CBCGPMiniFrameWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	if (CBCGPToolBar::IsCustomizeMode () || m_bCaptured)
	{
		return;
	}

	CRect rectWnd;
	GetWindowRect (&rectWnd);

	CRect rectBorderSize;
	CalcBorderSize (rectBorderSize);

	CRect rectCaption (rectWnd.left + rectBorderSize.left, rectWnd.top + rectBorderSize.top, 
						 rectWnd.right - rectBorderSize.right, 
						 rectWnd.top + rectBorderSize.top + m_nCaptionHeight);

	if (!rectCaption.PtInRect (point))
	{
		return;
	}

	CWnd* pParent = GetParent ();
	if (pParent != NULL)
	{
		ASSERT_VALID (pParent);

		pParent->SendMessage (BCGM_TOOLBARMENU, (WPARAM) GetSafeHwnd (),
								MAKELPARAM(point.x, point.y));
	}
}
//*************************************************************************************
void CBCGPMiniFrameWnd::OnCapture (BOOL bOnOff)
{
	CBCGPControlBar* pBar = 
		DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));

	if (pBar != NULL)
	{
		pBar->SetDragMode (bOnOff);
	}

	if (IsWindow (m_hWndToDestroyOnRelease) && !bOnOff)
	{
		::DestroyWindow (m_hWndToDestroyOnRelease);
		m_hWndToDestroyOnRelease = NULL;
	}

	if (pBar != NULL && !bOnOff)
	{
		if (pBar->m_hwndMiniFrameToBeClosed != m_hWnd && 
			IsWindow (pBar->m_hwndMiniFrameToBeClosed))
		{
			::DestroyWindow (pBar->m_hwndMiniFrameToBeClosed);
		}
		pBar->m_hwndMiniFrameToBeClosed = NULL;
	}
}
//**************************************************************************************
void CBCGPMiniFrameWnd::OnDestroy() 
{
	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = false;
		OnCapture (FALSE);
	}

	KillDockTimer ();

	if (m_nRollTimerID != 0)
	{
		KillTimer (m_nRollTimerID);
	}

	// register with dock manager
	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetParent ());
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->RemoveMiniFrame (this);
	}

	POSITION pos = m_lstFrames.Find (GetSafeHwnd ());
	ASSERT (pos != NULL);
	m_lstFrames.RemoveAt (pos);

	CWnd::OnDestroy();
}
//**************************************************************************************
void CBCGPMiniFrameWnd::SetDockTimer (UINT nTimeOut)
{
	if (m_nDockTimerID != 0)
	{
		KillDockTimer ();
	}
	m_nDockTimerID = (UINT) SetTimer (BCGP_DOCK_EVENT, nTimeOut, NULL);
}
//**************************************************************************************
void CBCGPMiniFrameWnd::KillDockTimer ()
{
	if (m_nDockTimerID != 0)
	{
		KillTimer (m_nDockTimerID);
		m_nDockTimerID = 0;
	}
}
//*************************************************************************************
CBCGPBaseControlBar* CBCGPMiniFrameWnd::FindFloatingBarByID (UINT nID)
{
	HWND hWnd = NULL;
	if (!m_mapFloatingBars.Lookup (nID, hWnd))
	{
		return NULL;
	}

	CBCGPBaseControlBar* pBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, CWnd::FromHandlePermanent (hWnd));

	return pBar;
}
//*************************************************************************************
CBCGPMiniFrameWnd* CBCGPMiniFrameWnd::FrameFromPoint (CPoint pt, int nSencitivity, 
														CBCGPMiniFrameWnd* pFrameToExclude, 
														BOOL bFloatMultiOnly)
{
	for (POSITION pos = m_mapFloatingBars.GetStartPosition (); pos != NULL;)
	{
		UINT uID = 0; 
		HWND hWnd = NULL;
		
		m_mapFloatingBars.GetNextAssoc (pos, uID, hWnd);

		CBCGPBaseControlBar* pNextBar = 
			DYNAMIC_DOWNCAST (CBCGPBaseControlBar, CWnd::FromHandlePermanent (hWnd));

		if (pNextBar == NULL)
		{
			continue;
		}

		if (!pNextBar->IsWindowVisible ())
		{
			continue;
		}


		if ((bFloatMultiOnly && 
			((pNextBar->GetBarStyle () & CBRS_FLOAT_MULTI) || pNextBar->IsFloatingMulti ()) ||
			!bFloatMultiOnly) && pNextBar->GetParentMiniFrame () != pFrameToExclude)
		{
			CRect rectBar, rectBarInflated;
			pNextBar->GetWindowRect (rectBar);
			rectBarInflated = rectBar;

			if (rectBarInflated.PtInRect (pt))
			{
				// the point around bar
				rectBar.InflateRect (-nSencitivity, -nSencitivity);
				if (!rectBar.PtInRect (pt) || nSencitivity == 0)
				{
					// the point near  the floating bar edges - return its parent frame
					CBCGPMiniFrameWnd* pWnd =  
						DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pNextBar->GetParentMiniFrame ());

					if (pWnd != NULL && pWnd->IsWindowVisible ())
					{
						return pWnd;
					}
					return NULL;
				}
			}
		}
	}
	return NULL;
}
//*************************************************************************************
void CBCGPMiniFrameWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_bCaptured && nChar == VK_ESCAPE)
	{
		if (GetDockMode () == DT_STANDARD)
		{
			OnCancelMode ();
		}
		else
		{
			ReleaseCapture ();
			CBCGPControlBar* pBar = 
				DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));	

			if (pBar != NULL && 
				pBar->GetEnabledAlignment () & CBRS_ALIGN_ANY)
			{
				pBar->m_recentDockInfo.m_rectRecentFloatingRect = m_rectRecentFloatingRect;
				if (!pBar->DockControlBar (pBar, NULL, DM_DBL_CLICK))
				{
					// was destroyed
					return;
				}
			}
		}
	}	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}
//*************************************************************************************
void CBCGPMiniFrameWnd::AdjustLayout ()
{
	CBCGPBaseControlBar* pBaseControlBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pBaseControlBar != NULL)
	{
		pBaseControlBar->RecalcLayout ();
		SizeToContent ();
	}
}
//*************************************************************************************
BOOL CBCGPMiniFrameWnd::SaveState (LPCTSTR lpszProfileName, UINT uiID)
{
	CBCGPBaseControlBar* pBaseControlBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pBaseControlBar != NULL)
	{
		return pBaseControlBar->SaveState (lpszProfileName, uiID);
	}
	return TRUE;
}
//*************************************************************************************
BOOL CBCGPMiniFrameWnd::LoadState (LPCTSTR lpszProfileName, UINT uiID)
{
	CBCGPBaseControlBar* pBaseControlBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));
	if (pBaseControlBar != NULL)
	{
		return pBaseControlBar->LoadState (lpszProfileName, uiID);
	}
	return TRUE;
}
//*************************************************************************************
void CBCGPMiniFrameWnd::Serialize (CArchive& ar)
{
	CWnd::Serialize (ar);

	if (ar.IsLoading ())
	{
		DWORD dwStyle = 0;
		CRect rect; rect.SetRectEmpty ();
		BOOL bIsVisible = FALSE;
		
		ar >> dwStyle;
		ar >> rect;
		ar >> bIsVisible;
		ar >> m_nRestoredEmbeddedBarID;
		ar >> m_dwCaptionButtons;
		ar >> m_bPinned;

		if (!Create (_T (""), dwStyle, rect, CBCGPMiniFrameWnd::m_pParentWndForSerialize))
		{
			throw new CArchiveException;
			return;
		}
		m_hParentWnd = CBCGPMiniFrameWnd::m_pParentWndForSerialize->m_hWnd;
	}
	else
	{
		CRect rect;
		GetWindowRect (rect);

		if (m_bRolledUp)
		{
			rect.bottom = rect.top + m_nHeightBeforeRollUp;
		}

		BOOL bIsVisible = IsWindowVisible ();

		ar << GetStyle ();
		ar << rect;
		ar << bIsVisible;

		CWnd* pEmbeddedWnd = CWnd::FromHandlePermanent (m_hEmbeddedBar);
		if (pEmbeddedWnd != 0)
		{
			ar << pEmbeddedWnd->GetDlgCtrlID ();
		}
		else
		{
			ar << 0;
		}
		ar << m_dwCaptionButtons;
		ar << m_bPinned;
	}
}
//*************************************************************************************
void CBCGPMiniFrameWnd::SetDockState (CBCGPDockManager* pDockManager)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDockManager);

	if (m_nRestoredEmbeddedBarID != NULL)
	{
		CBCGPBaseControlBar* pBar = pDockManager->FindBarByID (m_nRestoredEmbeddedBarID, TRUE);

		if (pBar != NULL && pBar->CanFloat () && ::IsWindow (pBar->m_hWnd))
		{

			if (pBar->IsTabbed ())
			{
				CBCGPBaseTabWnd* pTabWnd = (CBCGPBaseTabWnd*) pBar->GetParent ();
				CBCGPBaseTabbedBar* pTabBar = (CBCGPBaseTabbedBar*) pTabWnd->GetParent ();	
				ASSERT_VALID (pTabBar);
				// set belong to any parent
				pBar->SetParent (GetParent ());
				pTabBar->RemoveControlBar (pBar);
				if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPDockingControlBar)))
				{
					((CBCGPDockingControlBar*) pBar)->EnableGripper (TRUE);
				}

				pBar->ShowWindow (SW_SHOW);
			}

			if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPDockingControlBar)) && 
				((CBCGPDockingControlBar*) pBar)->IsAutoHideMode ())
			{
				((CBCGPDockingControlBar*) pBar)->SetAutoHideMode (FALSE, CBRS_ALIGN_ANY);
			}

			CRect rectDummy;
			pBar->GetWindowRect (rectDummy);

			if (pBar->GetParentMiniFrame () == NULL)
			{
				pBar->FloatControlBar (rectDummy, DM_SHOW, false);
			}

			CBCGPMiniFrameWnd* pParentMiniFrame = pBar->GetParentMiniFrame ();

			if (pParentMiniFrame != NULL)
			{
				pParentMiniFrame->RemoveControlBar (pBar, FALSE);
				pBar->SetParent (this);
				pBar->AdjustDockingLayout ();

				CRect rect;
				GetClientRect (rect);

				AddControlBar (pBar);

				//move control bar to the top left corner of the miniframe
				pBar->SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOSIZE  | SWP_NOACTIVATE);

				pBar->StretchControlBar (rect.Height (), TRUE);
				pBar->RecalcLayout ();
				SizeToContent ();

				SetWindowPos (NULL, 0, 0, 0, 0, 
								SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED  | SWP_NOACTIVATE);

				pBar->ShowControlBar (pBar->GetRecentVisibleState (), FALSE, FALSE);
				SetCaptionButtons (m_dwCaptionButtons);
				return;
			}
		}
	}

	// if we're here the miniframe is empty - destroy it
	DestroyWindow ();
}
//*************************************************************************************
LRESULT CBCGPMiniFrameWnd::OnCheckEmptyState (WPARAM, LPARAM)
{
	if (GetControlBarCount () == 0)
	{
		if (m_bNoDelayedDestroy)
		{
			ShowWindow (SW_HIDE);
		}
		else
		{
			DestroyWindow ();	
		}

	}

	return 0;
}
//*********************************************************************************
void CBCGPMiniFrameWnd::RedrawAll ()
{
	for (POSITION pos = m_lstFrames.GetHeadPosition (); pos != NULL;)
	{
		HWND hwndFrame = m_lstFrames.GetNext (pos);
		 if (CWnd::FromHandlePermanent (hwndFrame) != NULL)
		{
			::RedrawWindow (hwndFrame, NULL, NULL,
				RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
		 }
	}
}
//**********************************************************************************
int CBCGPMiniFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lstFrames.AddTail (GetSafeHwnd ());
	
	// Enable tooltips:
	m_ToolTip.Create (NULL, TTS_ALWAYSTIP);
	m_ToolTip.Activate (TRUE);
	if(globalData.m_nMaxToolTipWidth != -1)
	{
		m_ToolTip.SetMaxTipWidth(globalData.m_nMaxToolTipWidth);
	}

	return 0;
}
//**********************************************************************************
void CBCGPMiniFrameWnd::StoreRecentDockInfo (CBCGPControlBar* /*pBar*/)
{
}
//**********************************************************************************
void CBCGPMiniFrameWnd::StoreRecentTabRelatedInfo (CBCGPDockingControlBar* /*pDockingBar*/, 
													CBCGPDockingControlBar* /*pTabbedBar*/)
{
}
//**********************************************************************************
CBCGPBaseControlBar* CBCGPMiniFrameWnd::ControlBarFromPoint (CPoint point, int /*nSencitivity*/,
															 BOOL bCheckVisibility)
{
	CBCGPControlBar* pBar = 
			DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));	

	if (pBar != NULL)
	{
		CRect rect;
		pBar->GetWindowRect (rect);
		if (rect.PtInRect (point))
		{
			if (!pBar->IsWindowVisible () && bCheckVisibility)
			{
				return NULL;
			}
			return pBar;
		}
	}

	return NULL;
}
//**********************************************************************************
BOOL CBCGPMiniFrameWnd::CanBeDockedToBar (const CBCGPDockingControlBar* pDockingBar) const
{
	CBCGPControlBar* pBar = 
			DYNAMIC_DOWNCAST (CBCGPControlBar, CWnd::FromHandlePermanent (m_hEmbeddedBar));	

	return pDockingBar->CanAcceptBar (pBar);
}
//********************************************************************************
LRESULT CBCGPMiniFrameWnd::OnIdleUpdateCmdUI(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pFocus = GetFocus();
	BOOL bActiveOld = m_bActive;

	m_bActive = (pFocus->GetSafeHwnd () != NULL && 
		(IsChild (pFocus) || pFocus->GetSafeHwnd () == GetSafeHwnd ()));

	if (m_bActive != bActiveOld)
	{
		SendMessage (WM_NCPAINT);
	}

	SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0, TRUE, TRUE);

	return 0L;
}
//*********************************************************************************
BOOL CBCGPMiniFrameWnd::StartTearOff (CBCGPPopupMenu* pMenu)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pMenu);

	HWND hwndMenu = pMenu->GetSafeHwnd ();
	pMenu->ShowWindow (SW_HIDE);

	//----------------------
	// Redraw parent button:
	//----------------------
	CBCGPToolbarMenuButton* pParentBtn = pMenu->GetParentButton ();
	if (pParentBtn != NULL)
	{
		CWnd* pWndParent = pParentBtn->GetParentWnd ();
		if (pWndParent != NULL)
		{
			CRect rectBtn = pParentBtn->Rect ();
			rectBtn.InflateRect (4, 4);

			pWndParent->InvalidateRect (rectBtn);
			pWndParent->UpdateWindow ();
		}
	}

	CBCGPBaseControlBar* pBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, GetControlBar ());
	if (pBar == NULL)
	{
		return FALSE;
	}

	// handle pending WM_PAINT messages
	MSG msg;
	while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	{
		if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			return FALSE;
		DispatchMessage(&msg);
	}

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	// set capture to the window which received this message
	pBar->SetCapture();
	ASSERT(pBar == CWnd::GetCapture());

	BOOL bSuccess = FALSE;
	BOOL bStop = FALSE;

	// Move cirsor to the middle of the caption
	CRect rectFrame;
	GetWindowRect (rectFrame);

	int x = (rectFrame.left + rectFrame.right) / 2;
	int xOffset = x - rectFrame.left;

	int y = rectFrame.top + 5;
	int yOffset = y - rectFrame.top;

	::SetCursorPos (x, y);

	// get messages until capture lost or cancelled/accepted
	while (!bStop && CWnd::GetCapture() == pBar)
	{
		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int) msg.wParam);
			break;
		}

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			bStop = TRUE;
			bSuccess = TRUE;
			break;

		case WM_MOUSEMOVE:
			{
				SetWindowPos (NULL, 
					msg.pt.x - xOffset, msg.pt.y - yOffset,
					-1, -1,
					SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
			}
			break;

		case WM_KEYDOWN:
			if (msg.wParam == VK_ESCAPE)
			{
				bStop = TRUE;
			}
			break;

		case WM_RBUTTONDOWN:
			bStop = TRUE;
			break;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	ReleaseCapture();

	if (::IsWindow (hwndMenu))
	{
		if (bSuccess)
		{
			pMenu->SendMessage (WM_CLOSE);
			if (BCGCBProGetTopLevelFrame (this) != NULL)
			{
				BCGCBProGetTopLevelFrame (this)->SetFocus ();
			}
		}
		else
		{
			pMenu->ShowWindow (SW_SHOWNOACTIVATE);
		}
	}

	if (!bSuccess)
	{
		CFrameWnd* pWndMain = BCGCBProGetTopLevelFrame (this);
		if (pWndMain != NULL)
		{
			CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pWndMain);
			if (pMainFrame != NULL)
			{
				pMainFrame->m_Impl.RemoveTearOffToolbar (pBar);
			}
			else	// Maybe, SDI frame...
			{
				CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pWndMain);
				if (pFrame != NULL)
				{
					pFrame->m_Impl.RemoveTearOffToolbar (pBar);
				}
				else	// Maybe, OLE frame...
				{
					CBCGPOleIPFrameWnd* pOleFrame = 
						DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, pWndMain);
					if (pOleFrame != NULL)
					{
						pOleFrame->m_Impl.RemoveTearOffToolbar (pBar);
					}
					else
					{
						CBCGPOleDocIPFrameWnd* pOleDocFrame = 
							DYNAMIC_DOWNCAST (CBCGPOleDocIPFrameWnd, pWndMain);
						if (pOleDocFrame != NULL)
						{
							pOleDocFrame->m_Impl.RemoveTearOffToolbar (pBar);
						}
					}
				}
			}
		}

		pBar->DestroyWindow ();
		delete pBar;
	}

	return bSuccess;
}
//*********************************************************************************
BCGP_DOCK_TYPE CBCGPMiniFrameWnd::GetDockMode () const 
{
	CBCGPBaseControlBar* pBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseControlBar, GetControlBar ());
	if (pBar != NULL)
	{
		return pBar->GetDockMode ();
	}
	return CBCGPDockManager::m_dockModeGlobal;
}
//*********************************************************************************
void CBCGPMiniFrameWnd::RemoveNonValidBars ()
{
	CWnd* pWnd = DYNAMIC_DOWNCAST (CWnd, GetControlBar ());
	if (pWnd == NULL || !IsWindow (pWnd->GetSafeHwnd ()))
	{
		m_hEmbeddedBar = NULL;
		return;
	}
	
	if (pWnd->GetDlgCtrlID () != -1)
	{
		// the bar is ok
		return;
	}

	// check for empty tabbed bar with ID -1
	CBCGPBaseTabbedBar* pTabbedBar = 
		DYNAMIC_DOWNCAST (CBCGPBaseTabbedBar, GetControlBar ());

	if (pTabbedBar == NULL || pTabbedBar->GetTabsNum () == 0)
	{
		// notabbed control bar with ID -1 or tabbed control bar 
		// withot tabs - are not valid
		m_hEmbeddedBar = NULL;
		return;
	}
}
//*********************************************************************************
void CBCGPMiniFrameWnd::OnClose() 
{
	if (OnCloseMiniFrame ())
	{
		CloseMiniFrame ();
	}		
}
//*********************************************************************************
void CBCGPMiniFrameWnd::GetControlBarList (CObList& lstBars, CRuntimeClass* pRTCFilter)
{
	for (POSITION pos = CBCGPMiniFrameWnd::m_mapFloatingBars.GetStartPosition (); pos != NULL;)
	{
		UINT nID = (UINT) -1;
		HWND hWndBar = NULL;
		CBCGPMiniFrameWnd::m_mapFloatingBars.GetNextAssoc (pos, nID, hWndBar);

		CBCGPBaseControlBar* pBar = DYNAMIC_DOWNCAST (CBCGPBaseControlBar, 
														CWnd::FromHandle (hWndBar));

		if (pBar != NULL && 
			(pRTCFilter == NULL || pBar->GetRuntimeClass () == pRTCFilter))
		{
			lstBars.AddTail (pBar);
		}
	}
}
//*********************************************************************************
void CBCGPMiniFrameWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	m_hLastFocusWnd = NULL;
	if (m_hEmbeddedBar != NULL && IsWindow (m_hEmbeddedBar))
	{
		CWnd* pBar = CWnd::FromHandle (m_hEmbeddedBar);
		if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
		{
			m_hLastFocusWnd = pOldWnd->GetSafeHwnd ();
		}
	}

	CBCGPBaseControlBar* pFirstBar = DYNAMIC_DOWNCAST (CBCGPBaseControlBar, GetFirstVisibleBar ());
	if (pFirstBar != NULL && pFirstBar->CanFocus ())
	{
		pFirstBar->SetFocus ();
	}
	
	
	if (GetParentFrame () != NULL)
	{
		GetParentFrame ()->SetWindowPos (&wndTop, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
}
//*********************************************************************************
LRESULT CBCGPMiniFrameWnd::OnExitSizeMove (WPARAM, LPARAM)
{
	if (m_hLastFocusWnd != NULL && IsWindow (m_hLastFocusWnd))
	{
		::SetFocus (m_hLastFocusWnd);
		m_hLastFocusWnd = NULL;
	}
	return 0;
}
//*********************************************************************************
BOOL CBCGPMiniFrameWnd::OnBeforeDock ()
{
	if (GetKeyState (VK_CONTROL) < 0)
	{
		return FALSE;
	}

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (GetParent ());
	if (pDockManager != NULL)
	{
		CPoint ptMouse;
		GetCursorPos (&ptMouse);
		CRect rectExpected; rectExpected.SetRectEmpty ();
		BOOL bDrawTab;
		CBCGPDockingControlBar* pTargetBar = NULL;
		pDockManager->CalcExpectedDockedRect (this, ptMouse, 
							rectExpected, bDrawTab, &pTargetBar);	
		if (rectExpected.IsRectEmpty ())
		{
			return FALSE;
		}
	}

	return TRUE;
}
//*********************************************************************************
void CBCGPMiniFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CWnd::OnSettingChange(uFlags, lpszSection);
	RecalcCaptionHeight ();
}
//**********************************************************************************
void CBCGPMiniFrameWnd::RecalcCaptionHeight ()
{
	m_nCaptionHeight = GetSystemMetrics (SM_CYSMCAPTION);
	m_sizeMinSize.cx = m_sizeMinSize.cy = m_nCaptionHeight + 15;
}
//**********************************************************************************
BOOL CBCGPMiniFrameWnd::OnNeedTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/)
{
	static CString strTipText;

	if (m_ToolTip.GetSafeHwnd () == NULL)
	{
		return FALSE;
	}

	LPNMTTDISPINFO	pTTDispInfo	= (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	if (pNMH->idFrom > 0 && (int)pNMH->idFrom <= m_lstCaptionButtons.GetCount())
	{
		POSITION pos = m_lstCaptionButtons.FindIndex (pNMH->idFrom - 1);
		if (pos != NULL)
		{
			CBCGPCaptionButton* pBtn = (CBCGPCaptionButton*)m_lstCaptionButtons.GetAt (pos);
			ASSERT_VALID (pBtn);

			CBCGPLocalResource locaRes;

			switch (pBtn->GetHit ())
			{
			case HTCLOSE:
				strTipText.LoadString (IDS_BCGBARRES_CLOSE);
				pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR) strTipText);
				return TRUE;

			case HTMENU_BCG:
				strTipText.LoadString (IDS_BCGBARRES_TOOLBAR_OPTIONS);
				pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR) strTipText);
				return TRUE;

			case HTMAXBUTTON:
			case HTMINBUTTON:
				strTipText.LoadString (IDS_BCGBARRES_AUTOHIDEBAR);
				pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR) strTipText);
				return TRUE;
			}
		}
	}

	return FALSE;
}
//**********************************************************************************
BOOL CBCGPMiniFrameWnd::PreTranslateMessage(MSG* pMsg) 
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

	return CWnd::PreTranslateMessage(pMsg);
}
//**********************************************************************************
void CBCGPMiniFrameWnd::SetCaptionButtonsToolTips ()
{
	if (m_ToolTip.GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_lstCaptionButtons.GetCount() == m_ToolTip.GetToolCount())
	{
		return;
	}

	for (int i = 0 ; i < m_ToolTip.GetToolCount( ); i++)
	{
		m_ToolTip.DelTool (this, i + 1);
	}

	// Register tool for caption button's tooltip:
	for (i = m_ToolTip.GetToolCount(); i >= 0 && i < m_lstCaptionButtons.GetCount(); i ++)
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		m_ToolTip.AddTool (this, LPSTR_TEXTCALLBACK, &rectDummy, i + 1);
	}
}
//**********************************************************************************
void CBCGPMiniFrameWnd::OnPressButtons (UINT nHit)
{
	if (CBCGPToolBar::IsCustomizeMode() || (nHit != HTMENU_BCG))
	{
		return;
	}
	
	//-----------------------------
	//Prepare for Quick Customize Pane
	//----------------------------
	CBCGPCaptionMenuButton* pBtn = (CBCGPCaptionMenuButton *)FindButton(nHit);
	if (pBtn == NULL)
	{
		return;
	}

	CWnd* pWnd = GetControlBar();
	if (pWnd == NULL || pWnd->GetSafeHwnd() == NULL)
	{
		return;
	}

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CBCGPToolBar)))
	{
		return;
	}
						
	CBCGPToolBar* pTolBar = DYNAMIC_DOWNCAST(CBCGPToolBar, pWnd);

	CCustomizeButton* pCustBtn = pTolBar->GetCustomizeButton();
	if (!pCustBtn) //Not Enabled Customize Button
	{
		return;
	}

	if (pTolBar->IsAddRemoveQuickCustomize())
	{
		pBtn->m_bPushed = TRUE;

		//Get ToolBar's caption
		CString strCaption;
		pTolBar->GetWindowText(strCaption);

		strCaption.TrimLeft();
		strCaption.TrimRight();

		if (!strCaption.GetLength())
		{
			CBCGPLocalResource locaRes;
			strCaption.LoadString(IDS_BCGBARRES_UNTITLED_TOOLBAR);
		}
						
		CBCGPPopupMenu* pMenu = new CBCGPPopupMenu();

		//-----------------------
		// Insert Dummy Menu Item
		//-----------------------

		CBCGPPopupMenu* pMenuDummy = new CBCGPPopupMenu();
		pMenuDummy->InsertItem(CBCGPToolbarMenuButton(1, NULL, -1, _T("DUMMY")));
												
		CBCGPToolbarMenuButton btnToolCaption((UINT)-1, 
			pMenuDummy->GetMenuBar()->ExportToMenu(), -1, strCaption); 

		CBCGPToolbarMenuButton btnStandard(pCustBtn->GetCustomizeCmdId(), 
			NULL, -1, pCustBtn->GetCustomizeText());

		CBCGPPopupMenu* pMenuCustomize = new CBCGPPopupMenu();

		pMenuCustomize->InsertItem(btnToolCaption);
		pMenuCustomize->InsertItem(btnStandard);

		CString strLabel;

		{
			CBCGPLocalResource locaRes;
			strLabel.LoadString (IDS_BCGBARRES_ADD_REMOVE_BTNS);
		}

		CBCGPToolbarMenuButton btnAddRemove((UINT)-1, 
			pMenuCustomize->GetMenuBar()->ExportToMenu(), -1, strLabel);

		btnAddRemove.EnableQuickCustomize();

		delete pMenuDummy;
		delete pMenuCustomize;

		pMenu->InsertItem(btnAddRemove);
						
		CRect rc = pBtn->GetRect();					
		CPoint pt(rc.left, rc.top);
		ClientToScreen(&pt);
		CSize size = pBtn->GetSize();
											
		pMenu->Create(this, pt.x-2, pt.y-9, NULL);	
		pMenu->SetAutoDestroy();
		pMenu->SetOwner(this);
		pMenu->SetQuickMode(); // for Recently used mode
		pMenu->SetQuickCustomizeType(CBCGPPopupMenu::QUICK_CUSTOMIZE_ADDREMOVE);
	}
	
			
}
BOOL CBCGPMiniFrameWnd::OnCloseMiniFrame ()
{
	BOOL bCanClose = TRUE;
	CFrameWnd* pWndMain = BCGCBProGetTopLevelFrame (this);
	if (pWndMain != NULL)
	{
		CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pWndMain);
		if (pMainFrame != NULL)
		{
			bCanClose = pMainFrame->OnCloseMiniFrame (this);
		}
		else	// Maybe, SDI frame...
		{
			CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pWndMain);
			if (pFrame != NULL)
			{
				bCanClose = pFrame->OnCloseMiniFrame (this);
			}
			else	// Maybe, OLE frame...
			{
				CBCGPOleIPFrameWnd* pOleFrame = 
					DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, pWndMain);
				if (pOleFrame != NULL)
				{
					bCanClose = pOleFrame->OnCloseMiniFrame (this);
				}
				else
				{
					CBCGPOleDocIPFrameWnd* pOleDocFrame = 
						DYNAMIC_DOWNCAST (CBCGPOleDocIPFrameWnd, pWndMain);
					if (pOleDocFrame != NULL)
					{
						bCanClose = pOleDocFrame->OnCloseMiniFrame (this);
					}
				}
			}
		}
	}
	return bCanClose;
}