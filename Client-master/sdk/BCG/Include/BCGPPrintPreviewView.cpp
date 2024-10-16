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
//

#include "stdafx.h"
#include "BCGPPrintPreviewView.h"
#include "BCGPStatusBar.h"
#include "bcgprores.h"
#include "BCGPLocalResource.h"
#include "BCGPGlobalUtils.h"
#include "BCGPDockManager.h"

IMPLEMENT_DYNCREATE(CBCGPPrintPreviewView, CPreviewView)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int iSimplePaneIndex = 255;

/////////////////////////////////////////////////////////////////////////////
// CBCGPPrintPreviewToolBar

IMPLEMENT_DYNAMIC(CBCGPPrintPreviewToolBar, CBCGPToolBar)

BEGIN_MESSAGE_MAP(CBCGPPrintPreviewToolBar, CBCGPToolBar)
	//{{AFX_MSG_MAP(CBCGPPrintPreviewToolBar)
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBCGPPrintPreviewToolBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*pos*/)
{
	// Prevent print preview toolbar context menu appearing
}

int CBCGPPrintPreviewToolBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	CBCGPLocalResource locaRes;
	return CBCGPToolBar::OnToolHitTest (point, pTI);
}

void CBCGPPrintPreviewToolBar::OnDestroy() 
{
	CFrameWnd* pParentFrame = BCGPGetParentFrame (this);
	ASSERT_VALID (pParentFrame);

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (pParentFrame);
	if (pDockManager != NULL)
	{
		pDockManager->RemoveControlBarFromDockManager (this, FALSE, FALSE);
	}

	CBCGPToolBar::OnDestroy();
}
/////////////////////////////////////////////////////////////////////////////
// CBCGPPrintPreviewView

CBCGPPrintPreviewView::CBCGPPrintPreviewView()
{
	m_iPagesBtnIndex = -1;
	m_iOnePageImageIndex = -1;
	m_iTwoPageImageIndex = -1;
	m_pWndStatusBar = NULL;
	m_bIsStatusBarSimple = FALSE;
	m_nSimpleType = 0;
	m_nCurrentPage = 1;
}
//*********************************************************************************
CBCGPPrintPreviewView::~CBCGPPrintPreviewView()
{
	if (m_pWndStatusBar != NULL)
	{
		//----------------------------------
		// Restore previous StatusBar state:
		//----------------------------------
		m_pWndStatusBar->SetPaneText (iSimplePaneIndex, NULL);
	}
}


BEGIN_MESSAGE_MAP(CBCGPPrintPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CBCGPPrintPreviewView)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, OnUpdatePreviewNumPage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPPrintPreviewView message handlers

int CBCGPPrintPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPreviewView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPLocalResource locaRes;

	ASSERT_VALID (m_pToolBar);

	if (!m_wndToolBar.Create (m_pToolBar) ||
		!m_wndToolBar.LoadToolBar(IDR_BCGRES_PRINT_PREVIEW, 0, 0, TRUE /* Locked */))
	{
		TRACE0("Failed to create print preview toolbar\n");
		return FALSE;      // fail to create
	}

	m_wndToolBar.SetOwner (this);

	//-------------------------------------------
	// Remember One Page/Two pages image indexes:
	//-------------------------------------------
	m_iPagesBtnIndex = m_wndToolBar.CommandToIndex (AFX_ID_PREVIEW_NUMPAGE);
	ASSERT (m_iPagesBtnIndex >= 0);
	
	CBCGPToolbarButton* pButton= m_wndToolBar.GetButton (m_iPagesBtnIndex);
	ASSERT_VALID (pButton);

	m_iOnePageImageIndex = pButton->GetImage ();

	int iIndex = m_wndToolBar.CommandToIndex (ID_BCGRES_TWO_PAGES_DUMMY);
	ASSERT (iIndex >= 0);
	
	pButton= m_wndToolBar.GetButton (iIndex);
	ASSERT_VALID (pButton);

	m_iTwoPageImageIndex = pButton->GetImage ();

	//---------------------------------
	// Remove dummy "Two pages" button:
	//---------------------------------
	m_wndToolBar.RemoveButton (iIndex);

	//------------------------------------
	// Set "Print" button to image + text:
	//------------------------------------
	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (AFX_ID_PREVIEW_PRINT));

	//---------------------------------
	// Set "Close" button to text only:
	//---------------------------------
	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (AFX_ID_PREVIEW_CLOSE),
		NULL, TRUE, FALSE);

	CFrameWnd* pParentFrame = BCGPGetParentFrame (this);
	ASSERT_VALID (pParentFrame);

	CBCGPDockManager* pDockManager = globalUtils.GetDockManager (pParentFrame);
	ASSERT_VALID (pDockManager);			
	pDockManager->AddControlBar (&m_wndToolBar, FALSE);

	//-------------------------
	// Change the Toolbar size:
	//-------------------------
	SetToolbarSize ();

	//-------------------------------------------
	// Set Application Status Bar to Simple Text:
	//-------------------------------------------
	
	m_pWndStatusBar = DYNAMIC_DOWNCAST (CBCGPStatusBar,
		pParentFrame->GetDlgItem (AFX_IDW_STATUS_BAR));

	if (m_pWndStatusBar != NULL)
	{
	
		//-------------------------------------
		// Set Simple Pane Style to No Borders:
		//-------------------------------------
		m_pWndStatusBar->SetPaneText (iSimplePaneIndex, NULL);
	}

	return 0;
}
//*********************************************************************************
void CBCGPPrintPreviewView::OnUpdatePreviewNumPage(CCmdUI *pCmdUI) 
{
	CPreviewView::OnUpdateNumPageChange(pCmdUI);

	//--------------------------------------------------
	// Change the Icon of AFX_ID_PREVIEW_NUMPAGE button:
	//--------------------------------------------------
	CBCGPToolbarButton* pButton = m_wndToolBar.GetButton (m_iPagesBtnIndex);
	ASSERT_VALID (pButton);

	UINT nPages = m_nZoomState == ZOOM_OUT ? m_nPages : m_nZoomOutPages;
	pButton->SetImage (nPages == 1 ? m_iTwoPageImageIndex : m_iOnePageImageIndex);

	m_wndToolBar.InvalidateRect (pButton->Rect ());
}
//*********************************************************************************
void CBCGPPrintPreviewView::OnDisplayPageNumber (UINT nPage, UINT nPagesDisplayed)
{
	ASSERT (m_pPreviewInfo != NULL);

	CFrameWnd* pParentFrame = BCGPGetParentFrame (this);
	ASSERT_VALID (pParentFrame);

	int nSubString = (nPagesDisplayed == 1) ? 0 : 1;

	CString s;
	if (AfxExtractSubString (s, m_pPreviewInfo->m_strPageDesc, nSubString))
	{
		CString strPage;

		if (nSubString == 0)
		{
			strPage.Format (s, nPage);
		}
		else
		{
			UINT nEndPage = nPage + nPagesDisplayed - 1;
			strPage.Format (s, nPage, nEndPage);
		}

		if (m_pWndStatusBar != NULL)
		{
			m_pWndStatusBar->SetPaneText (iSimplePaneIndex, strPage);
		}
		else
		{
			pParentFrame->SendMessage (WM_SETMESSAGESTRING, 0, 
										(LPARAM)(LPCTSTR) strPage);
		}
	}
	else
	{
		TRACE1("Malformed Page Description string. Could not get string %d.\n",
			nSubString);
	}
}
//*********************************************************************************
BCGCBPRODLLEXPORT void BCGPPrintPreview (CView* pView)
{
	ASSERT_VALID (pView);

	CPrintPreviewState *pState= new CPrintPreviewState;

	CBCGPLocalResource locaRes;

	if (!pView->DoPrintPreview (IDD_BCGBAR_RES_PRINT_PREVIEW, pView, 
		RUNTIME_CLASS (CBCGPPrintPreviewView), pState))
	{
		TRACE0("Error: OnFilePrintPreview failed.\n");
		AfxMessageBox (AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
}
//*******************************************************************************
void CBCGPPrintPreviewView::OnSize(UINT nType, int cx, int cy) 
{
	CPreviewView::OnSize(nType, cx, cy);
	
	//-------------------------
	// Change the Toolbar size:
	//-------------------------
	SetToolbarSize ();
}
//******************************************************************************
void CBCGPPrintPreviewView::SetToolbarSize ()
{
	ASSERT_VALID (m_pToolBar);

	CSize szSize = m_wndToolBar.CalcFixedLayout (TRUE, TRUE);

	//----------------------------------------------------------------------
	// Print toolbar should occupy the whole width of the mainframe (Win9x):
	//----------------------------------------------------------------------
	CFrameWnd* pParent = BCGPGetParentFrame (this);
	ASSERT_VALID (pParent);

	CRect rectParent;
	pParent->GetClientRect (rectParent);
	szSize.cx = rectParent.Width ();

	m_pToolBar->SetWindowPos (NULL, 0, 0, szSize.cx, szSize.cy, 
				SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOZORDER);

	m_wndToolBar.SetWindowPos (NULL, 0, 0, szSize.cx, szSize.cy, 
				SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOZORDER);

	//----------------------------------------------------
	// Adjust parent toolbar (actually - dialog bar) size:
	//----------------------------------------------------
	m_pToolBar->m_sizeDefault.cy = szSize.cy;
	
	pParent->RecalcLayout();            // position and size everything
	pParent->UpdateWindow();
}

