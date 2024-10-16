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

// BCGPToolBox.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPToolBox.h"
#include "BCGPOutlookWnd.h"
#include "BCGPButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBoxPage

#define TOOLBOX_IMAGE_MARGIN 6

IMPLEMENT_DYNCREATE(CBCGPToolBoxPage, CBCGPControlBar)

CBCGPToolBoxPage::CBCGPToolBoxPage()
{
	m_nCheckedButton = -1;
	m_sizeImage = CSize (0, 0);
	m_pToolBox = NULL;
	m_nVertScrollOffset = 0;
	m_nVertScrollSize = 0;
}

CBCGPToolBoxPage::~CBCGPToolBoxPage()
{
	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		delete m_arButtons [i];
	}
}

BEGIN_MESSAGE_MAP(CBCGPToolBoxPage, CBCGPControlBar)
	//{{AFX_MSG_MAP(CBCGPToolBoxPage)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBCGPToolBoxPage::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (&rectClient);

	CBCGPVisualManager::GetInstance ()->OnFillBarBackground (pDC, this,
		rectClient, rectClient);
	return  TRUE;
}
//****************************************************************************************
void CBCGPToolBoxPage::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPControlBar::OnSize(nType, cx, cy);

	m_nVertScrollOffset = 0;
	ReposButtons ();
}
//****************************************************************************************
void CBCGPToolBoxPage::ReposButtons ()
{
	CRect rc;
	GetClientRect (&rc);

	int iTop = rc.top + TOOLBOX_IMAGE_MARGIN;
	const int iHeight = max (globalData.GetTextHeight (), m_sizeImage.cy + TOOLBOX_IMAGE_MARGIN);

	m_nVertScrollSize = 0;

	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		CBCGPButton* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		if (i < m_nVertScrollOffset)
		{
			pButton->SetWindowPos(NULL,
				0, 0,
				0, 0,
				SWP_NOACTIVATE | SWP_NOZORDER);
		}
		else
		{
			if (iTop + iHeight <= rc.bottom)
			{
				pButton->SetWindowPos(NULL,
					rc.left, iTop,
					rc.Width(), iHeight,
					SWP_NOACTIVATE | SWP_NOZORDER);
			}
			else
			{
				pButton->SetWindowPos(NULL,
					0, 0,
					0, 0,
					SWP_NOACTIVATE | SWP_NOZORDER);
			}

			iTop += iHeight;

			if (iTop >= rc.bottom)
			{
				m_nVertScrollSize++;
			}
		}

		pButton->RedrawWindow ();
	}
}
//***************************************************************************************
BOOL CBCGPToolBoxPage::InitPage (UINT uiBmpResID, int nImageWidth, 
								 const CStringList& lstLabels,
								 CRuntimeClass* pButtonClass)
{
	ASSERT_VALID (this);

	HBITMAP hBitmap = (HBITMAP) ::LoadImage (
		AfxGetResourceHandle (),
		MAKEINTRESOURCE (uiBmpResID),
		IMAGE_BITMAP,
		0, 0,
		LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

	if (hBitmap == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BITMAP bmp;
	::GetObject (hBitmap, sizeof (BITMAP), (LPVOID) &bmp);

	m_sizeImage.cx = nImageWidth;
	m_sizeImage.cy = bmp.bmHeight;

	UINT nFlags = ILC_MASK;

	switch (bmp.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		break;
	}

	CImageList images;
	images.Create (nImageWidth, bmp.bmHeight, nFlags, 0, 0);

	HBITMAP hbmpCopy = (HBITMAP) ::CopyImage (hBitmap, IMAGE_BITMAP, 0, 0, 0);
	images.Add (CBitmap::FromHandle (hbmpCopy), globalData.clrBtnFace);
	::DeleteObject (hbmpCopy);

	int nImages = bmp.bmWidth / nImageWidth;
	ASSERT (nImages == lstLabels.GetCount ());

	for (int i = 0; i < nImages; i++)
	{
		CBCGPButton* pButton = DYNAMIC_DOWNCAST (CBCGPButton, pButtonClass->CreateObject ());
		if (pButton == NULL)
		{
			ASSERT (FALSE);
			return FALSE;
		}

		pButton->Create (_T(""), WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
			CRect (0, 0, 0, 0),this, (UINT)-1);

		pButton->m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
		pButton->m_nAlignStyle = CBCGPButton::ALIGN_LEFT;
		pButton->m_bDrawFocus = FALSE;
		pButton->m_bResponseOnButtonDown = TRUE;
		pButton->m_bTransparent = TRUE;

		pButton->SetImage (images.ExtractIcon (i));

		POSITION posLabel = lstLabels.FindIndex (i);
		if (posLabel != NULL)
		{
			pButton->SetWindowText (lstLabels.GetAt (posLabel));
		}

		if (i == 0)
		{
			m_nCheckedButton = 0;
			pButton->SetCheck (TRUE);
		}

		m_arButtons.Add (pButton);
	}

	return TRUE;
}
//*****************************************************************************************
BOOL CBCGPToolBoxPage::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int nButtonIndex = FindButton ((HWND) lParam);
	
	if (nButtonIndex < 0)
	{
		return CBCGPControlBar::OnCommand(wParam, lParam);
	}

	if (m_nCheckedButton != -1)
	{
		m_arButtons [m_nCheckedButton]->SetCheck (FALSE);
	}

	m_nCheckedButton = nButtonIndex;

	if (m_nCheckedButton < 0)
	{
		return CBCGPControlBar::OnCommand(wParam, lParam);
	}

	m_arButtons [m_nCheckedButton]->SetCheck (TRUE);

	if (m_pToolBox != NULL)
	{
		ASSERT_VALID (m_pToolBox);
		int nPage = m_pToolBox->GetPageNumber (this);

		m_pToolBox->OnClickTool (nPage, m_nCheckedButton);
	}

	return TRUE;
}
//*****************************************************************************************
int CBCGPToolBoxPage::FindButton (HWND hwndButton) const
{
	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		CBCGPButton* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		if (pButton->GetSafeHwnd () == hwndButton)
		{
			return i;
		}
	}
	
	return -1;
}
//*****************************************************************************************
void CBCGPToolBoxPage::OnNcDestroy() 
{
	CBCGPControlBar::OnNcDestroy();

	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBox

IMPLEMENT_DYNCREATE(CBCGPToolBox, CBCGPDockingControlBar)

CBCGPToolBox::CBCGPToolBox()
{
}

CBCGPToolBox::~CBCGPToolBox()
{
}

BEGIN_MESSAGE_MAP(CBCGPToolBox, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CBCGPToolBox)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBox message handlers

int CBCGPToolBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy (0, 0, 0, 0);

	// Create ToolBox pane:
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT;
	DWORD dwBCGStyle = 0;

	if (!m_wndOutlook.Create (_T(""), this, rectDummy, 
		0, dwStyle, dwBCGStyle))
	{
		TRACE0("Failed to create ToolBox window\n");
		return -1;      // fail to create
	}

    m_wndOutlook.SetBarStyle (CBRS_ALIGN_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY |
								CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndOutlook.EnableSetCaptionTextToTabName (FALSE);

	return 0;
}
//****************************************************************************************
void CBCGPToolBox::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	
	m_wndOutlook.SetWindowPos (NULL,
            -1, -1,
            cx, cy,
            SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	OnActivatePage (GetActivePage ());
}
//****************************************************************************************
BOOL CBCGPToolBox::AddToolsPage (LPCTSTR lpszPageName, UINT uiBmpResID, int nImageWidth,
								 LPCTSTR lpszLabels,
								 CRuntimeClass* pPageClass,
								 CRuntimeClass* pButtonClass)
{
	ASSERT (lpszLabels != NULL);

	CString strLabels = lpszLabels;
	CStringList lstLabels;

	for (int i = 0; i < strLabels.GetLength ();)
	{
		CString strLabel;
			
		int iNextWord = strLabels.Find (_T('\n'), i);
		if (iNextWord == -1)
		{
			lstLabels.AddTail (strLabels.Mid (i));
			break;
		}

		lstLabels.AddTail (strLabels.Mid (i, iNextWord - i));
		i = iNextWord + 1;
	}

	return AddToolsPage (lpszPageName, uiBmpResID, nImageWidth,
								 lstLabels, pPageClass, pButtonClass);
}
//****************************************************************************************
BOOL CBCGPToolBox::AddToolsPage (LPCTSTR lpszPageName, UINT uiBmpResID, int nImageWidth,
								 const CStringList& lstLabels, 
								 CRuntimeClass* pPageClass,
								 CRuntimeClass* pButtonClass)
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);
	ASSERT (pPageClass != NULL);
	ASSERT (pPageClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPToolBoxPage)));
	ASSERT (pButtonClass != NULL);
	ASSERT (pButtonClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPButton)));
	ASSERT (lpszPageName != NULL);

	CBCGPToolBoxPage* pPage = DYNAMIC_DOWNCAST (
		CBCGPToolBoxPage, pPageClass->CreateObject ());
	if (pPage == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CRect rectDummy (0, 0, 0, 0);
	pPage->Create (NULL, WS_VISIBLE | WS_CHILD, rectDummy, this, 0);

	pPage->SetWindowText (lpszPageName);
	if (!pPage->InitPage (uiBmpResID, nImageWidth, lstLabels, pButtonClass))
	{
		delete pPage;
		return FALSE;
	}

	m_wndOutlook.AddTab (pPage, TRUE, TRUE, FALSE);
	pPage->m_pToolBox = this;

	OnActivatePage (m_wndOutlook.GetTabsNum () - 1);

	return TRUE;
}
//*****************************************************************************************
int CBCGPToolBox::GetActivePage () const
{
	ASSERT_VALID (this);

	CBCGPBaseTabWnd* pTab = GetTabWnd ();
	if (pTab == NULL)
	{
		ASSERT (FALSE);
		return  -1;
	}

	ASSERT_VALID (pTab);

	return pTab->GetActiveTab ();
}
//*****************************************************************************************
int CBCGPToolBox::GetLastClickedTool (int nPage) const
{
	ASSERT_VALID (this);

	CBCGPBaseTabWnd* pTab = GetTabWnd ();
	if (pTab == NULL)
	{
		ASSERT (FALSE);
		return  -1;
	}

	ASSERT_VALID (pTab);

	if (nPage < 0 || nPage >= pTab->GetTabsNum ())
	{
		ASSERT (FALSE);
		return -1;
	}

	CBCGPToolBoxPage* pPage = DYNAMIC_DOWNCAST (CBCGPToolBoxPage, pTab->GetTabWnd (nPage));
	if (pPage == NULL)
	{
		ASSERT (FALSE);
		return -1;
	}

	ASSERT_VALID (pPage);
	return pPage->m_nCheckedButton;
}
//******************************************************************************************
int CBCGPToolBox::GetPageNumber (CBCGPToolBoxPage* pPage) const
{
	ASSERT_VALID (this);
	ASSERT_VALID (pPage);

	CBCGPBaseTabWnd* pTab = GetTabWnd ();
	if (pTab == NULL)
	{
		ASSERT (FALSE);
		return  -1;
	}

	ASSERT_VALID (pTab);

	for (int i = 0; i < pTab->GetTabsNum (); i++)
	{
		CBCGPToolBoxPage* pListPage = DYNAMIC_DOWNCAST (CBCGPToolBoxPage, pTab->GetTabWnd (i));
		if (pListPage->GetSafeHwnd () == pPage->GetSafeHwnd ())
		{
			return i;
		}
	}

	return -1;
}
//****************************************************************************************
CBCGPToolBoxPage* CBCGPToolBox::GetPage (int nPage) const
{
	ASSERT_VALID (this);

	CBCGPBaseTabWnd* pTab = GetTabWnd ();
	if (pTab == NULL)
	{
		ASSERT (FALSE);
		return  NULL;
	}

	ASSERT_VALID (pTab);

	return DYNAMIC_DOWNCAST (CBCGPToolBoxPage, pTab->GetTabWnd (nPage));
}
//****************************************************************************************
void CBCGPToolBox::OnClickTool (int /*nPage*/, int /*nIndex*/)
{
	ASSERT_VALID (this);

	CWnd* pOwner = GetOwner ();
	if (pOwner != NULL)
	{
		pOwner->PostMessage (	WM_COMMAND,
								MAKEWPARAM (GetDlgCtrlID (), BN_CLICKED),
								(LPARAM) m_hWnd);
	}
}
//*****************************************************************************************
CBCGPBaseTabWnd* CBCGPToolBox::GetTabWnd () const
{
	ASSERT_VALID (this);
	return ((CBCGPToolBox*) this)->m_wndOutlook.GetUnderlinedWindow ();
}
//***************************************************************************************
void CBCGPToolBoxBar::OnActivateTab (int iTabNum)
{
	ASSERT_VALID (this);

	CBCGPOutlookBar::OnActivateTab (iTabNum);

	CBCGPToolBox* pParentBar = DYNAMIC_DOWNCAST (CBCGPToolBox, GetParent ());
	if (pParentBar == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	pParentBar->OnActivatePage (iTabNum);
}
//****************************************************************************************
void CBCGPToolBoxBar::OnScroll (BOOL bDown)
{
	ASSERT_VALID (this);

	CBCGPOutlookBar::OnScroll (bDown);

	CBCGPToolBox* pParentBar = DYNAMIC_DOWNCAST (CBCGPToolBox, GetParent ());
	if (pParentBar == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	pParentBar->OnScroll (bDown);
}
//****************************************************************************************
void CBCGPToolBox::OnActivatePage (int /*nPage*/)
{
	ASSERT_VALID (this);

	CBCGPOutlookWnd* pOlWnd = DYNAMIC_DOWNCAST (CBCGPOutlookWnd, GetTabWnd ());
	ASSERT_VALID (pOlWnd);

	CBCGPToolBoxPage* pPage = GetPage (GetActivePage ());
	if (pPage == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	pOlWnd->EnableScrollButtons (TRUE, FALSE, pPage->m_nVertScrollSize > 0);
}
//***************************************************************************************
void CBCGPToolBox::OnScroll (BOOL bDown)
{
	ASSERT_VALID (this);

	CBCGPOutlookWnd* pOlWnd = DYNAMIC_DOWNCAST (CBCGPOutlookWnd, GetTabWnd ());
	ASSERT_VALID (pOlWnd);

	CBCGPToolBoxPage* pPage = GetPage (GetActivePage ());
	if (pPage == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	if (bDown)
	{
		pPage->m_nVertScrollOffset++;
	}
	else
	{
		pPage->m_nVertScrollOffset--;
	}

	pPage->ReposButtons ();
	pOlWnd->EnableScrollButtons (TRUE, pPage->m_nVertScrollOffset > 0, pPage->m_nVertScrollSize > 0);
}

