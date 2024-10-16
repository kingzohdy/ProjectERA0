// BCGPSlider.cpp : implementation file
//

#include "stdafx.h"

#include "BCGGlobals.h"
#include "BCGPSlider.h"
#include "BCGPDockingControlBar.h"
#include "BCGPBarContainerManager.h"
#include "BCGPBarContainer.h"
#include "BCGPGlobalUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CBCGPSlider,CBCGPBaseControlBar)

int CBCGPSlider::m_nDefaultWidth = 4;

/////////////////////////////////////////////////////////////////////////////
// CBCGPSlider
//--------------------------------------------------------------------------//
CBCGPSlider::CBCGPSlider (BOOL bDefaultSlider, CWnd* pParentWnd) : m_nID ((UINT)-1), m_dwSliderStyle (0), 
												 m_nWidth (0), 
												 m_bCaptured (false), 
												 m_pContainerManager (NULL),
												 m_bDefaultSlider (bDefaultSlider)
{
	m_rectLastDragRect.SetRectEmpty ();	
	m_rectDragBounds.SetRectEmpty ();
	m_nMinOffset = 0;  
	m_nMaxOffset = 0; 
	m_nStep		 = -1;
	m_bAutoHideMode = FALSE;
	m_pParentWndForSerialize = pParentWnd;
	
}
//--------------------------------------------------------------------------//
CBCGPSlider::~CBCGPSlider()
{
	
}
//--------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBCGPSlider, CBCGPBaseControlBar)
	//{{AFX_MSG_MAP(CBCGPSlider)
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPSlider message handlers
//--------------------------------------------------------------------------//
BOOL CBCGPSlider::Create (DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
						 CCreateContext* pContext) 
{
	ASSERT_VALID (this);

	
	return CBCGPSlider::CreateEx(0L, dwStyle, rect, pParentWnd, nID, pContext);
}
//--------------------------------------------------------------------------//
BOOL CBCGPSlider::CreateEx(DWORD dwStyleEx, DWORD dwStyle, const RECT& rect, 
						  CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	ASSERT_VALID (this);

	//-----------------------------
	// Register a new window class:
	//-----------------------------
	HINSTANCE hInst = AfxGetInstanceHandle();
	UINT uiClassStyle = CS_DBLCLKS;
	HCURSOR hCursor = ::LoadCursor (NULL, IDC_ARROW);
	HBRUSH hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	CString strClassName;
	strClassName.Format (_T("BCGSlider:%x:%x:%x:%x"), 
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

	m_nID = nID;
	m_dwSliderStyle = dwStyle;

	if (m_dwSliderStyle & SS_VERT)
	{
		m_nWidth = rect.right - rect.left;
	}
	else if (m_dwSliderStyle & SS_HORZ)
	{
		m_nWidth = rect.bottom - rect.top;
	}

	DWORD dwSliderStyle = m_dwSliderStyle | WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (m_bDefaultSlider)
	{
		m_pContainerManager = new CBCGPBarContainerManager;
		m_pContainerManager->Create (pParentWnd, this);
	}

	m_pDockSite = DYNAMIC_DOWNCAST (CFrameWnd, pParentWnd);

	if (m_pDockSite == NULL)
	{
		m_pDockSite = BCGPGetParentFrame (pParentWnd);
	}

	if (m_pDockSite == NULL)
	{
		ASSERT (FALSE);
	}

	return CWnd::CreateEx (dwStyleEx, strClassName, NULL, dwSliderStyle, rect, pParentWnd, nID, pContext);
}
//--------------------------------------------------------------------------//
void CBCGPSlider::Serialize (CArchive& ar)
{
	ASSERT_VALID (this);
	CBCGPBaseControlBar::Serialize (ar);

	CRect rect;
	
	if (ar.IsStoring ())
	{
		GetWindowRect (rect);
		GetParent ()->ScreenToClient (rect);

		ar << m_nID;
		ar << m_nStep;
		ar << rect;
		ar << IsWindowVisible ();	
		ar << m_dwSliderStyle;
		ar << m_nWidth;
		ar << m_bDefaultSlider;
		ar << m_nMinOffset;
		ar << m_nMaxOffset;
	}
	else
	{
		BOOL bVisible = FALSE;

		ar >> m_nID;
		ar >> m_nStep;
		ar >> rect;
		ar >> bVisible;
		ar >> m_dwSliderStyle;
		ar >> m_nWidth;
		ar >> m_bDefaultSlider;
		ar >> m_nMinOffset;
		ar >> m_nMaxOffset;

		if (bVisible)
		{
			m_dwSliderStyle |= WS_VISIBLE;
		}
		else
		{
			m_dwSliderStyle &= ~WS_VISIBLE;
		}

		if (!CreateEx (0, m_dwSliderStyle, rect, m_pParentWndForSerialize, m_nID, NULL))
		{
			TRACE0 ("Unable to create slider from archive");
		}
	}

	if (m_pContainerManager != NULL && m_bDefaultSlider)
	{
		m_pContainerManager->Serialize (ar);
	}

}
//--------------------------------------------------------------------------//
CBCGPDockingControlBar* CBCGPSlider::FindTabbedBar (UINT nID)
{
	ASSERT_VALID (this);
	if (m_pContainerManager != NULL)
	{
		return m_pContainerManager->FindTabbedBar (nID);
	}
	return NULL;
}
//--------------------------------------------------------------------------//
void CBCGPSlider::OnSize(UINT nType, int cx, int cy) 
{
	ASSERT_VALID (this);
	CWnd::OnSize(nType, cx, cy);
}
//--------------------------------------------------------------------------//
BOOL CBCGPSlider::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	ASSERT_VALID (this);

	switch (nHitTest)
	{
	case HTCLIENT:
		if (m_dwSliderStyle & SS_HORZ)
		{
			SetCursor (globalData.m_hcurStretchVert);
		}
		else if (m_dwSliderStyle & SS_VERT) 
		{
			SetCursor (globalData.m_hcurStretch);
		}
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
//--------------------------------------------------------------------------//
void CBCGPSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT_VALID (this);
	if (!m_bCaptured)
	{
		m_bCaptured = true;
		SetCapture ();

		CRect rectSlider;
		GetWindowRect (rectSlider);
		CSize size (m_nWidth / 2, m_nWidth / 2);
		CWindowDC dc (GetDesktopWindow ());
		dc.DrawDragRect (&rectSlider, size, NULL, size);

		m_rectLastDragRect = rectSlider;

		m_pContainerManager->GetMinMaxOffset (this, m_nMinOffset, m_nMaxOffset, m_nStep);
		m_rectDragBounds = rectSlider;
		if (IsHorizontal ())
		{
			m_rectDragBounds.top = rectSlider.top + m_nMinOffset;
			m_rectDragBounds.bottom = rectSlider.bottom + m_nMaxOffset;
		}
		else
		{
			m_rectDragBounds.left = rectSlider.left + m_nMinOffset;
			m_rectDragBounds.right = rectSlider.right + m_nMaxOffset;
		}

		if (m_pContainerManager != NULL)
		{
			m_pContainerManager->SetResizeMode (TRUE);
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}
//--------------------------------------------------------------------------//
void CBCGPSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ASSERT_VALID (this);
	
	StopTracking (TRUE);
	CWnd::OnLButtonUp(nFlags, point);
}
//--------------------------------------------------------------------------//
void CBCGPSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	ASSERT_VALID (this);
	if (m_bCaptured)
	{
		CRect rectNew = m_rectLastDragRect;

		CPoint ptNew;
		GetCursorPos (&ptNew);

		if (m_dwSliderStyle & SS_VERT)
		{
			rectNew.left = ptNew.x - m_nWidth / 2;
			rectNew.right = rectNew.left + m_nWidth;
			if (rectNew.left < m_rectDragBounds.left)
			{
				rectNew.left = m_rectDragBounds.left;
				rectNew.right = rectNew.left + m_rectLastDragRect.Width ();
			}

			if (rectNew.right > m_rectDragBounds.right)
			{
				rectNew.right = m_rectDragBounds.right;
				rectNew.left = rectNew.right - m_rectLastDragRect.Width ();
			}
		}
		else
		{
			rectNew.top = ptNew.y - m_nWidth / 2;
			rectNew.bottom = rectNew.top + m_nWidth;
			if (rectNew.top < m_rectDragBounds.top)
			{
				rectNew.top = m_rectDragBounds.top;
				rectNew.bottom = rectNew.top + m_nWidth;
			}

			if (rectNew.bottom > m_rectDragBounds.bottom)
			{
				rectNew.bottom = m_rectDragBounds.bottom;
				rectNew.top = rectNew.bottom - m_nWidth;
			}
		}
		
		CSize size (m_nWidth / 2, m_nWidth / 2);
		CWindowDC dc (GetDesktopWindow ());
		dc.DrawDragRect (&rectNew, size, &m_rectLastDragRect, size);
		m_rectLastDragRect = rectNew;
	}
	
	CWnd::OnMouseMove(nFlags, point);
}
//--------------------------------------------------------------------------//
void CBCGPSlider::OnCancelMode ()
{
	StopTracking (FALSE);
	CBCGPBaseControlBar::OnCancelMode ();
}
//--------------------------------------------------------------------------//
void CBCGPSlider::StopTracking (BOOL bMoveSlider)
{
	if (m_bCaptured)
	{
		CRect rectSlider;
		GetWindowRect (rectSlider);

		CPoint ptOffset = m_rectLastDragRect.TopLeft () - rectSlider.TopLeft ();

		CRect rectEmpty;
		rectEmpty.SetRectEmpty ();	

		CWindowDC dc (NULL);
		CSize size (m_nWidth / 2, m_nWidth / 2);
		dc.DrawDragRect (&rectEmpty, size, &m_rectLastDragRect, size);
		
		if (bMoveSlider)
		{
			MoveSlider (ptOffset);	
		}
		
		m_rectLastDragRect.SetRectEmpty ();

		ReleaseCapture ();
		m_bCaptured = false;

		if (m_pContainerManager != NULL)
		{
			m_pContainerManager->SetResizeMode (FALSE);
		}
	}
}
//--------------------------------------------------------------------------//
void CBCGPSlider::OnPaint() 
{
	ASSERT_VALID (this);
	CPaintDC dc(this); // device context for painting
	CBCGPMemDC memDC (dc, this);

	CRect rectClient;
	GetClientRect (rectClient);
	CBCGPVisualManager::GetInstance ()->OnDrawSlider (&memDC.GetDC (), this, rectClient, m_bAutoHideMode);
}
//--------------------------------------------------------------------------//
void CBCGPSlider::MoveSlider (CPoint& ptOffset, BOOL /*bAdjustLayout*/)
{
	ASSERT_VALID (this);
	
	CRect rectSlider;
	GetWindowRect (rectSlider);

	int nOffset = 0;
	if (m_dwSliderStyle & SS_VERT)
	{
		nOffset = ptOffset.x;
		rectSlider.OffsetRect (nOffset, 0);
	}
	else if (m_dwSliderStyle & SS_HORZ)
	{
		nOffset = ptOffset.y;
		rectSlider.OffsetRect (0, nOffset);
	}
	else
	{
		return;
	}

	HDWP hdwp = BeginDeferWindowPos (50);
	if (m_pContainerManager != NULL)
	{
		m_pContainerManager->OnSliderMove (this, 0, nOffset, hdwp);
	}
	EndDeferWindowPos (hdwp);
	
	// it moves the slider
	AdjustDockingLayout ();
	

	GetParent ()->ScreenToClient (rectSlider);
	// move the slider by ourself
	SetWindowPos (NULL, rectSlider.left, rectSlider.top, 
					rectSlider.Width (), rectSlider.Height (), SWP_NOZORDER  | SWP_NOACTIVATE);

	
}
//--------------------------------------------------------------------------//
void CBCGPSlider::AddControlBar  (CBCGPDockingControlBar* pBar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);
	ASSERT_KINDOF (CBCGPDockingControlBar, pBar);

	m_pContainerManager->AddControlBar (pBar);
	CheckVisibility ();
}
//--------------------------------------------------------------------------//
CBCGPDockingControlBar*  CBCGPSlider::AddRecentControlBar (CBCGPDockingControlBar* pBar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);
	ASSERT_KINDOF (CBCGPDockingControlBar, pBar);

	CBCGPDockingControlBar* pAddedBar = NULL;
	CBCGPBarContainer* pRecentContainer = pBar->m_recentDockInfo.GetRecentContainer (TRUE);
	CBCGPBarContainer* pRecentTabContainer = pBar->m_recentDockInfo.GetRecentTabContainer (TRUE);
	if (pRecentContainer != NULL)
	{
		pAddedBar = m_pContainerManager->AddControlBarToRecentContainer (pBar, pRecentContainer);
		CheckVisibility ();
	}
	else if (pRecentTabContainer != NULL)
	{
		pAddedBar = m_pContainerManager->AddControlBarToRecentContainer (pBar, pRecentTabContainer);
		CheckVisibility ();
	}
	else
	{
		ASSERT (FALSE);
	}
	return pAddedBar;
}
//--------------------------------------------------------------------------//
BOOL CBCGPSlider::InsertControlBar	(CBCGPDockingControlBar* pBarToInsert, 
									 CBCGPDockingControlBar* pTargetBar, 
									 DWORD dwAlignment, 
									 LPCRECT lpRect)
{
	ASSERT_VALID (this);
	ASSERT_KINDOF (CBCGPDockingControlBar, pBarToInsert);
	ASSERT_KINDOF (CBCGPDockingControlBar, pTargetBar);

	BOOL bResult = FALSE;
	if (m_pContainerManager != NULL)
	{
		bResult = m_pContainerManager->InsertControlBar (pBarToInsert, pTargetBar, 
														dwAlignment, lpRect);
		CheckVisibility ();
	}
	return bResult;
}
//--------------------------------------------------------------------------//
const CBCGPBaseControlBar* CBCGPSlider::GetFirstBar () const
{
	ASSERT_VALID (this);
	if (m_pContainerManager != NULL)
	{
		return m_pContainerManager->GetFirstBar ();
	}
	return NULL;
}
//--------------------------------------------------------------------------//
void CBCGPSlider::RemoveControlBar (CBCGPDockingControlBar* pBar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);
	ASSERT_KINDOF (CBCGPDockingControlBar, pBar);

	if (m_pContainerManager != NULL)
	{
		// remove the bar from container
		m_pContainerManager->RemoveControlBarFromContainer (pBar);
		// remove the bar from docksite (do not destroy!!!)
		CBCGPBaseControlBar::RemoveControlBarFromDockManager (pBar, FALSE, FALSE, m_bAutoHideMode);
		if (m_pContainerManager->IsEmpty () && 
			m_pContainerManager->GetTotalRefCount () == 0 &&
			pBar->m_recentDockInfo.GetRecentDefaultSlider () != this)
		{
			// it was the last control bar in the container - 
			// remove and DESTROY  the slider as well
			CBCGPBaseControlBar::RemoveControlBarFromDockManager (this, TRUE, FALSE, m_bAutoHideMode);
		}
		else
		{
			if (!CheckVisibility ())
			{
				ShowWindow (SW_HIDE);
			}
		}
	}
}
//--------------------------------------------------------------------------//
BOOL CBCGPSlider::ReplaceControlBar	(CBCGPDockingControlBar* pBarToReplace, 
									 CBCGPDockingControlBar* pBarToReplaceWith)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBarToReplace);
	ASSERT_VALID (pBarToReplaceWith);
	ASSERT_KINDOF (CBCGPDockingControlBar, pBarToReplaceWith);

	if (m_pContainerManager == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	pBarToReplaceWith->SetDefaultSlider (m_hWnd);
	
	BOOL bResult = m_pContainerManager->ReplaceControlBar (pBarToReplace, pBarToReplaceWith);
	CheckVisibility ();
	return bResult;
}
//--------------------------------------------------------------------------//
BOOL CBCGPSlider::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//*******************************************************************************
CSize CBCGPSlider::CalcFixedLayout(BOOL /*bStretch*/, BOOL /*bHorz*/)
{
	ASSERT_VALID (this);
	CRect rectWnd;
	GetWindowRect (&rectWnd);
	CSize size = rectWnd.Size ();

	CRect rectContainer;
	if (m_pContainerManager != NULL)
	{
		m_pContainerManager->GetWindowRect (rectContainer);
		size += rectContainer.Size ();
	}
	return size;
}
//*******************************************************************************
void CBCGPSlider::RepositionBars	(CRect& rectNew, HDWP& hdwp)
{
	CRect rectWnd;
	GetWindowRect (&rectWnd);

	CRect rectContainer = rectNew; 
	CRect rectNewSlider = rectNew;
	
	DWORD dwAlignment = GetCurrentAlignment ();

	switch (dwAlignment)
	{
	case CBRS_ALIGN_LEFT:
		rectNewSlider.left = rectNew.right - m_nWidth;
		rectContainer.right = rectNewSlider.left;
		rectContainer.top = rectNewSlider.top;
		rectContainer.bottom = rectNewSlider.bottom;
		break;
	case CBRS_ALIGN_RIGHT:
		rectNewSlider.right = rectNew.left + m_nWidth;
		rectContainer.left = rectNewSlider.right;
		rectContainer.top = rectNewSlider.top;
		rectContainer.bottom = rectNewSlider.bottom;
		break;
	case CBRS_ALIGN_TOP:
		rectNewSlider.top = rectNew.bottom - m_nWidth;
		rectContainer.bottom = rectNewSlider.top;
		rectContainer.left = rectNewSlider.left;
		rectContainer.right = rectNewSlider.right;
		break;
	case CBRS_ALIGN_BOTTOM:
		rectNewSlider.bottom = rectNew.top + m_nWidth;
		rectContainer.top = rectNewSlider.bottom;
		rectContainer.left = rectNewSlider.left;
		rectContainer.right = rectNewSlider.right;
		break;
	}

	CWnd* pParentWnd = GetParent ();
	ASSERT_VALID (pParentWnd);

	pParentWnd->ScreenToClient (rectNew);
	pParentWnd->ScreenToClient (rectNewSlider);
	pParentWnd->ScreenToClient (rectContainer);
	hdwp = MoveWindow (rectNewSlider, TRUE, hdwp);

	if (m_pContainerManager != NULL)
	{
		m_pContainerManager->ResizeBarContainers (rectContainer, hdwp);
	}
}
//*******************************************************************************
void CBCGPSlider::OnDestroy() 
{

	CBCGPBaseControlBar::OnDestroy();
}
//*******************************************************************************
void CBCGPSlider::OnNcDestroy() 
{
	if (m_pContainerManager != NULL)
	{
		m_pContainerManager->RemoveSlider (this);
	}

	if (m_pContainerManager != NULL && m_bDefaultSlider)
	{
		delete m_pContainerManager;
		m_pContainerManager = NULL;
	}

	CBCGPBaseControlBar::OnNcDestroy();
	delete this;		
}
//*******************************************************************************
void CBCGPSlider::ShowWindow (int nCmdShow)
{
	CWnd::ShowWindow (nCmdShow);
}
//*******************************************************************************
int CBCGPSlider::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPBaseControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (globalData.m_hcurStretch == NULL)
	{
		globalData.m_hcurStretch = AfxGetApp ()->LoadCursor (AFX_IDC_HSPLITBAR);
	}

	if (globalData.m_hcurStretchVert == NULL)
	{
		globalData.m_hcurStretchVert = AfxGetApp ()->LoadCursor (AFX_IDC_VSPLITBAR);
	}
	
	return 0;
}
//*******************************************************************************
void CBCGPSlider::StoreRecentDockInfo (CBCGPDockingControlBar* pBar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);

	if (m_pContainerManager != NULL)
	{
		m_pContainerManager->StoreRecentDockInfo (pBar);
	}
}
//*******************************************************************************
void CBCGPSlider::StoreRecentTabRelatedInfo (CBCGPDockingControlBar* pDockingBar, 
												CBCGPDockingControlBar* pTabbedBar)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDockingBar);
	ASSERT_VALID (pTabbedBar);

	if (m_pContainerManager != NULL)
	{
		BOOL bLeftBar = FALSE;
		CBCGPBarContainer* pTabbedContainer = 
			m_pContainerManager->FindContainer (pTabbedBar, bLeftBar);
		if (pTabbedContainer != NULL)
		{
			pDockingBar->
				m_recentDockInfo.StoreDockInfo (pTabbedContainer, pTabbedBar);
		}
		else
		{
			ASSERT (FALSE);
		}
	}
}
//*******************************************************************************
CBCGPBarContainer* CBCGPSlider::FindContainer (CBCGPDockingControlBar* pBar, 
											   BOOL& bLeftBar)
{
	ASSERT_VALID (this);

	return m_pContainerManager->FindContainer (pBar, bLeftBar);
}
//*******************************************************************************
BOOL CBCGPSlider::AddContainer (CBCGPBarContainerManager& barContainerManager, 
								BOOL bOuterEdge)	
{
	BOOL bResult = FALSE;
	if (m_pContainerManager != NULL)
	{
		bResult = m_pContainerManager->AddContainerManager (barContainerManager, bOuterEdge);
		CheckVisibility ();
	}
	return bResult;
}
//*******************************************************************************
BOOL CBCGPSlider::AddContainer	(CBCGPDockingControlBar* pTargetBar, 
								 CBCGPBarContainerManager& barContainerManager, 
								 DWORD dwAlignment)
{
	BOOL bResult = FALSE;
	if (m_pContainerManager != NULL)
	{
		bResult = m_pContainerManager->AddContainerManager (pTargetBar, dwAlignment, 
																 barContainerManager,
																 TRUE);
		CheckVisibility ();
	}
	return bResult;
}
//*******************************************************************************
void CBCGPSlider::OnShowControlBar (CBCGPDockingControlBar* pBar, BOOL bShow)
{
	if (m_pContainerManager != NULL && !IsAutoHideMode	())
	{
		BOOL bNewVisibleState = m_pContainerManager->OnShowControlBar (pBar, bShow);
		if (!bShow)
		{
			// actual only for hide, because when bShow is TRUE slider must be always
			// visible
			ShowWindow (bNewVisibleState ? SW_SHOW : SW_HIDE);
			BOOL bLeftBar = FALSE;
			CBCGPBarContainer* pContainer = m_pContainerManager->FindContainer (pBar, bLeftBar);
			if (pContainer != NULL)
			{
				pContainer->OnShowControlBar (pBar, bShow);
			}
		}
		else
		{
			ShowWindow (SW_SHOW);
		}
	}
}
//*******************************************************************************
BOOL CBCGPSlider::CheckVisibility ()
{
	if (m_bDefaultSlider && !IsAutoHideMode	() && m_pContainerManager != NULL)
	{
		BOOL bIsRootContainerVisible = m_pContainerManager->IsRootContainerVisible ();
		ShowWindow (bIsRootContainerVisible ? SW_SHOW : SW_HIDE);
		return bIsRootContainerVisible;
	}

	return FALSE;
}
//*******************************************************************************
BOOL CBCGPSlider::DoesContainFloatingBar ()
{
	if (m_pContainerManager != NULL)
	{
		return m_pContainerManager->DoesContainFloatingBar ();
	}
	return FALSE;
}
//*******************************************************************************
BOOL CBCGPSlider::DoesAllowDynInsertBefore () const
{
	if (m_pContainerManager != NULL)
	{
		return m_pContainerManager->DoesAllowDynInsertBefore ();
	}
	return TRUE;
}
//*******************************************************************************
void CBCGPSlider::CalcExpectedDockedRect (CWnd* pWndToDock, CPoint ptMouse, 
										  CRect& rectResult, BOOL& bDrawTab, 
										  CBCGPDockingControlBar** ppTargetBar)
{
	CBCGPGlobalUtils globalUtils;
	if (m_pContainerManager != NULL)
	{
		globalUtils.CalcExpectedDockedRect (*m_pContainerManager, pWndToDock, 
											ptMouse, rectResult, bDrawTab, ppTargetBar);
	}
}
//*******************************************************************************
void CBCGPSlider::NotifyAboutRelease ()
{
	if (m_pContainerManager->IsEmpty () && 
		m_pContainerManager->GetTotalRefCount () == 0)
	{
		// it was the last control bar in the container - 
		// remove and DESTROY  the slider as well
		CBCGPBaseControlBar::RemoveControlBarFromDockManager (this, TRUE, FALSE, m_bAutoHideMode);
	}	
}
//*******************************************************************************
void CBCGPSlider::GetControlBars (CObList& lstBars) 
{
	if (m_pContainerManager != NULL)
	{
		m_pContainerManager->AddControlBarsToList(&lstBars, NULL);
	}
}
//*******************************************************************************
void CBCGPSlider::GetSliders (CObList& lstSliders)
{
	if (m_pContainerManager != NULL)
	{
		m_pContainerManager->AddControlBarsToList(NULL, &lstSliders);
	}
}