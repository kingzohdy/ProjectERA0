// BCGPTabbedToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPTabbedToolbar.h"
#include "BCGPTabWnd.h"
#include "BCGPToolBar.h"
#include "BCGPMiniFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CBCGPTabbedToolbar, CBCGPToolBar, VERSIONABLE_SCHEMA | 2)
/////////////////////////////////////////////////////////////////////////////
// CBCGPTabbedToolbar

CBCGPTabbedToolbar::CBCGPTabbedToolbar()
{
	m_pTabWnd = NULL;
	m_cyTopBorder = m_cyBottomBorder = m_cxLeftBorder = m_cxRightBorder = 0;
}

CBCGPTabbedToolbar::~CBCGPTabbedToolbar()
{
}
//************************************************************************************
BOOL CBCGPTabbedToolbar::Create  (CWnd* pParentWnd, DWORD dwStyle, UINT nID, DWORD dwBCGStyle)
{
	if (dwBCGStyle & CBRS_BCGP_FLOAT)
	{
		dwStyle |= CBRS_GRIPPER;
	}

	dwStyle &= ~(CBRS_LEFT | CBRS_RIGHT);

	if (pParentWnd != NULL)
	{
		ASSERT_VALID (pParentWnd);   // must have a parent
	}

	SetBarAlignment (m_dwStyle);

	//----------------------------
	// Initialize common controls:
	//----------------------------
	VERIFY (AfxDeferRegisterClass (AFX_WNDCOMMCTLS_REG));

	//-----------------
	// Create the HWND:
	//-----------------
	CRect rect;
	rect.SetRectEmpty();

	//-----------------------------
	// Register a new window class:
	//-----------------------------
	HINSTANCE hInst = AfxGetInstanceHandle();
	UINT uiClassStyle = CS_DBLCLKS;
	HCURSOR hCursor = ::LoadCursor (NULL, IDC_ARROW);
	HBRUSH hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	CString strClassName;
	strClassName.Format (_T("BCGToolBar:%x:%x:%x:%x"), 
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

	return CBCGPBaseToolBar::Create (strClassName, dwStyle, rect, pParentWnd, nID, dwBCGStyle);
}
//************************************************************************************
BEGIN_MESSAGE_MAP(CBCGPTabbedToolbar, CBCGPToolBar)
	//{{AFX_MSG_MAP(CBCGPTabbedToolbar)
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPTabbedToolbar message handlers

int CBCGPTabbedToolbar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPBaseToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectClient (0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	ASSERT (m_pTabWnd == NULL);

	m_pTabWnd = new CBCGPTabWnd;

	CBCGPTabWnd* pTabWnd = (CBCGPTabWnd*) m_pTabWnd;

	// Create tabs window:
	if (!pTabWnd->Create (CBCGPTabWnd::STYLE_3D, rectClient, this, 101, 
							CBCGPTabWnd::LOCATION_TOP))
	{
		TRACE0("Failed to create tab window\n");
		delete m_pTabWnd;
		m_pTabWnd = NULL;
		return -1;      // fail to create
	}

	pTabWnd->AutoDestroyWindow (FALSE);
	pTabWnd->HideSingleTab (FALSE);

	pTabWnd->m_bEnableWrapping = FALSE;
	pTabWnd->EnableTabSwap (FALSE);

	return 0;
}
//************************************************************************************
void CBCGPTabbedToolbar::OnNcDestroy() 
{
	CBCGPBaseToolBar::OnNcDestroy();
	
	if (m_pTabWnd != NULL)
	{
		delete m_pTabWnd;
		m_pTabWnd = NULL;
	}
}
//************************************************************************************
LRESULT CBCGPTabbedToolbar::OnChangeActiveTab (WPARAM, LPARAM)
{
	return 0;
}
//************************************************************************************
void CBCGPTabbedToolbar::RecalcLayout ()
{
	if (m_pTabWnd != NULL)
	{
		CRect rect;
		CSize size = m_pTabWnd->GetMaxWindowSize ();


		rect.top = rect.left = 0;
		rect.bottom = size.cy + m_pTabWnd->GetTabsHeight () + 
						m_pTabWnd->GetTabBorderSize () * 2 + 2;
		rect.right = size.cx + m_pTabWnd->GetTabBorderSize () * 2;

		CRect rectTabWndOld;
		m_pTabWnd->GetWindowRect (rectTabWndOld);

		m_pTabWnd->SetWindowPos (NULL, rect.left, rect.top, rect.Width (), rect.Height (), SWP_NOZORDER  | SWP_NOACTIVATE);
		m_pTabWnd->RecalcLayout ();

		HDWP hdwp = NULL;
		int nStretchSize = rect.Width () - rectTabWndOld.Width ();
		StretchBar (nStretchSize, hdwp);
		UpdateVirtualRect ();
		if (GetParentMiniFrame () != NULL)
		{
			GetParentMiniFrame ()->SizeToContent ();
		}
		else if (BCGPGetParentFrame (this) != NULL)
		{
			BCGPGetParentFrame (this)->RecalcLayout ();
		}
	}	

	CBCGPBaseToolBar::RecalcLayout ();
}
//************************************************************************************
CSize CBCGPTabbedToolbar::CalcFixedLayout(BOOL /*bStretch*/, BOOL /*bHorz*/)
{
	CSize sizeResult (0, 0);
	CRect rectTabWnd;

	m_pTabWnd->GetWindowRect (rectTabWnd);

	sizeResult = rectTabWnd.Size ();

	CRect rect; 
	rect.SetRectEmpty();

	CalcInsideRect(rect, LM_HORZ);

	sizeResult.cy -= rect.Height();
	sizeResult.cx -= rect.Width();

	return sizeResult;
}
//************************************************************************************
BOOL CBCGPTabbedToolbar::AddTab (CBCGPControlBar* pNewBar, BOOL bVisible, 
								 BOOL bSetActive, BOOL bDetachable)
{
	if (m_pTabWnd == NULL)
	{
		return FALSE;
	}

	pNewBar->SetParent (m_pTabWnd);
	pNewBar->SetOwner (BCGPGetParentFrame (this));

	CString strText; 
	pNewBar->GetWindowText (strText);

	DWORD dwStyle = pNewBar->GetBarStyle ();
	pNewBar->SetBarStyle (dwStyle & ~CBRS_GRIPPER);

	m_pTabWnd->AddTab (pNewBar, strText, (UINT) -1, bDetachable);

	int nTabCount = m_pTabWnd->GetTabsNum ();

	if (bSetActive)
	{
		m_pTabWnd->SetActiveTab (nTabCount - 1);
	}

	
	m_pTabWnd->ShowTab (nTabCount - 1, bVisible);
	RecalcLayout ();

	return TRUE;
}


