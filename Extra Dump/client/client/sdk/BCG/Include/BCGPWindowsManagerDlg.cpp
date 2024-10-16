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
//
// This code is based on Ivan Zhakov (vanya@mail.axon.ru)'s
// MDI Windows Manager dialog 
// http://codeguru.developer.com/doc_view/WindowsManager.shtml
//

// BCGPWindowsManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBPro.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPWindowsManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPWindowsManagerDlg dialog
UINT WM_BCGWINDOW_HELP		= ::RegisterWindowMessage(_T("WINDOW_HELP"));//WDW

CBCGPWindowsManagerDlg::CBCGPWindowsManagerDlg(CBCGPMDIFrameWnd* pMDIFrame, BOOL bHelpButton)
	: CDialog(CBCGPWindowsManagerDlg::IDD, pMDIFrame),
	m_pMDIFrame (pMDIFrame),
	m_bHelpButton (bHelpButton)
{
	//{{AFX_DATA_INIT(CBCGPWindowsManagerDlg)
	//}}AFX_DATA_INIT
	ASSERT_VALID (m_pMDIFrame);

	m_bMDIActions = TRUE;
}


void CBCGPWindowsManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCGPWindowsManagerDlg)
	DDX_Control(pDX, IDC_BCGBARRES_LIST, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBCGPWindowsManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CBCGPWindowsManagerDlg)
	ON_BN_CLICKED(IDC_BCGBARRES_ACTIVATE, OnActivate)
	ON_BN_CLICKED(IDC_BCGBARRES_SAVE, OnSave)
	ON_BN_CLICKED(IDC_BCGBARRES_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_BCGBARRES_CASCADE, OnCascade)
	ON_BN_CLICKED(IDC_BCGBARRES_TILEHORZ, OnTilehorz)
	ON_BN_CLICKED(IDC_BCGBARRES_TILEVERT, OnTilevert)
	ON_BN_CLICKED(IDC_BCGBARRES_MINIMIZE, OnMinimize)
	ON_LBN_SELCHANGE(IDC_BCGBARRES_LIST, OnSelchangeBcgbarresList)
	ON_WM_DRAWITEM()
	ON_LBN_DBLCLK(IDC_BCGBARRES_LIST, OnActivate)
	ON_BN_CLICKED(ID_HELP, OnBcgbarresWindowHelp)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPWindowsManagerDlg message handlers

void CBCGPWindowsManagerDlg::MDIMessage (UINT uMsg, WPARAM flag)
{
	CWaitCursor wait;
	CBCGPDisableMDIAnimation	no_animation;

	int nItems = m_wndList.GetCount();

	if (nItems != LB_ERR && nItems>0)
	{
		HWND	hMDIClient=m_pMDIFrame->m_hWndMDIClient;
		::LockWindowUpdate(hMDIClient);
		for(int i=nItems-1;i>=0;i--)
		{
			HWND hWnd=(HWND) m_wndList.GetItemData(i);
			if (m_wndList.GetSel(i)>0)	::ShowWindow(hWnd,SW_RESTORE);
			else						::ShowWindow(hWnd,SW_MINIMIZE);
		}
		::SendMessage(hMDIClient,uMsg, flag,0);	
		::LockWindowUpdate(NULL);
	}
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnActivate() 
{
	GetParent ()->SetRedraw (FALSE);

	if (m_wndList.GetSelCount() == 1)
	{
		int	index;	
		if (m_wndList.GetSelItems (1, &index) == 1)
		{						 
			DWORD_PTR dw = m_wndList.GetItemData (index);
			if (dw != (DWORD_PTR) LB_ERR)
			{
				WINDOWPLACEMENT	wndpl;
				wndpl.length = sizeof(WINDOWPLACEMENT);
				::GetWindowPlacement ((HWND) dw,&wndpl);

				if (wndpl.showCmd == SW_SHOWMINIMIZED)
				{
					::ShowWindow((HWND) dw,SW_RESTORE);
				}

				::SendMessage(m_pMDIFrame->m_hWndMDIClient,WM_MDIACTIVATE,(WPARAM) dw,0);
				EndDialog(IDOK);
			}
		}
	}

	GetParent ()->SetRedraw (TRUE);
	GetParent ()->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnSave() 
{
	int		nItems=m_wndList.GetCount();
	if (nItems!=LB_ERR && nItems>0)
	{
		for(int i=0;i<nItems;i++)
		{
			if (m_wndList.GetSel(i)>0)
			{
				HWND		hWnd=(HWND) m_wndList.GetItemData(i);
				CWnd		*pWnd=CWnd::FromHandle(hWnd);
				CFrameWnd	*pFrame=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
				if (pFrame != NULL)
				{
					CDocument *pDoc=pFrame->GetActiveDocument();
					if (pDoc != NULL)	pDoc->SaveModified();
				}
			}
		}
	}

	FillWindowList();
	SelActive();
	UpdateButtons();
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnClose() 
{
	int		nItems=m_wndList.GetCount();
	if (nItems!=LB_ERR && nItems>0)
	{
		HWND	hMDIClient=m_pMDIFrame->m_hWndMDIClient;
		
		m_wndList.SetRedraw(FALSE);

		for(int i=nItems-1;i>=0;i--)
		{
			if (m_wndList.GetSel(i)>0)
			{					   
				HWND hWnd=(HWND) m_wndList.GetItemData(i);
				::SendMessage(hWnd,WM_CLOSE,(WPARAM) 0, (LPARAM) 0);
				if (::GetParent(hWnd)==hMDIClient) break;
			}				  
		}
		m_wndList.SetRedraw(TRUE);
	}

	FillWindowList();
	SelActive();
	UpdateButtons();
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnCascade() 
{
	MDIMessage(WM_MDICASCADE,0);
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnTilehorz() 
{
	MDIMessage(WM_MDITILE,MDITILE_HORIZONTAL);	
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnTilevert() 
{
	MDIMessage(WM_MDITILE,MDITILE_VERTICAL);	
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnMinimize() 
{
	CBCGPDisableMDIAnimation	no_animation;

	int		nItems=m_wndList.GetCount();
	if (nItems!=LB_ERR && nItems>0)
	{
		m_wndList.SetRedraw(FALSE);

		for(int i=nItems-1;i>=0;i--)
		{
			if (m_wndList.GetSel(i)>0)
			{
				HWND hWnd=(HWND) m_wndList.GetItemData(i);
				::ShowWindow(hWnd,SW_MINIMIZE);
			}
		}
		m_wndList.SetRedraw(TRUE);
	}

	FillWindowList();
	SelActive();
	UpdateButtons();
}
//**************************************************************************************
BOOL CBCGPWindowsManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	FillWindowList();

	// If no MDI actions are availible, hide all MDI-related buttons:

	if (!m_bMDIActions)
	{
		GetDlgItem(IDC_BCGBARRES_TILEHORZ)->ShowWindow (SW_HIDE);
		GetDlgItem(IDC_BCGBARRES_TILEVERT)->ShowWindow (SW_HIDE);
		GetDlgItem(IDC_BCGBARRES_CASCADE)->ShowWindow (SW_HIDE);
		GetDlgItem(IDC_BCGBARRES_MINIMIZE)->ShowWindow (SW_HIDE);
	}

	SelActive();
	UpdateButtons();

	CWnd* pBtnHelp = GetDlgItem (ID_HELP);
	if (pBtnHelp != NULL)
	{
		pBtnHelp->ShowWindow (m_bHelpButton ? SW_SHOW : SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//**************************************************************************************
void CBCGPWindowsManagerDlg::OnSelchangeBcgbarresList() 
{
	UpdateButtons();
}

// Enables/Disables states of buttons
void CBCGPWindowsManagerDlg::UpdateButtons()
{						   
	int	nSel = m_wndList.GetSelCount ();

	BOOL bClose = nSel > 0;

	for (int i= 0; bClose && i < m_wndList.GetCount (); i++)
	{
		if (m_wndList.GetSel(i) > 0)
		{					   
			HWND hWnd= (HWND) m_wndList.GetItemData (i);
			if (m_lstCloseDisabled.Find (hWnd))
			{
				bClose = FALSE;
			}
		}
	}

	GetDlgItem(IDC_BCGBARRES_CLOSE)->EnableWindow (bClose);

	GetDlgItem(IDC_BCGBARRES_SAVE)->EnableWindow(nSel>0);
	GetDlgItem(IDC_BCGBARRES_TILEHORZ)->EnableWindow(m_bMDIActions && nSel>=2);
	GetDlgItem(IDC_BCGBARRES_TILEVERT)->EnableWindow(m_bMDIActions && nSel>=2);
	GetDlgItem(IDC_BCGBARRES_CASCADE)->EnableWindow(m_bMDIActions && nSel>=2);
	GetDlgItem(IDC_BCGBARRES_MINIMIZE)->EnableWindow(m_bMDIActions && nSel>0);

	GetDlgItem(IDC_BCGBARRES_ACTIVATE)->EnableWindow(nSel==1);
}

// Selects currently active window in listbox
void CBCGPWindowsManagerDlg::SelActive()
{
	int		nItems=m_wndList.GetCount();
	if (nItems != LB_ERR && nItems>0)
	{
	
		m_wndList.SetRedraw(FALSE);
		m_wndList.SelItemRange(FALSE,0,nItems-1);
		
		HWND	hwndActive=(HWND) ::SendMessage(m_pMDIFrame->m_hWndMDIClient,WM_MDIGETACTIVE,0,0);
	
		for(int i=0;i<nItems;i++) {
			if ((HWND) m_wndList.GetItemData(i)==hwndActive)  {
				m_wndList.SetSel(i);
				break;
			}
		}
		m_wndList.SetRedraw(TRUE);
	}
}

// Refresh windows list
void CBCGPWindowsManagerDlg::FillWindowList(void)
{
	m_wndList.SetRedraw(FALSE);
	m_wndList.ResetContent();

	m_bMDIActions = TRUE;
	m_lstCloseDisabled.RemoveAll ();

	HWND hwndT = ::GetWindow(m_pMDIFrame->m_hWndMDIClient, GW_CHILD);
	while (hwndT != NULL)
	{
		TCHAR	szWndTitle[256];
		::GetWindowText(hwndT,szWndTitle,sizeof(szWndTitle)/sizeof(szWndTitle[0]));

		int index=m_wndList.AddString(szWndTitle);
		m_wndList.SetItemData(index,(DWORD_PTR) hwndT);

		DWORD dwStyle = ::GetWindowLong (hwndT, GWL_STYLE);
		if ((dwStyle & WS_SYSMENU) == 0)
		{
			m_bMDIActions = FALSE;
		}
		else
		{
			HMENU hSysMenu = ::GetSystemMenu (hwndT, FALSE);
			if (hSysMenu == NULL)
			{
				m_bMDIActions = FALSE;
			}
			else
			{
				MENUITEMINFO menuInfo;
				ZeroMemory(&menuInfo,sizeof(MENUITEMINFO));
				menuInfo.cbSize = sizeof(MENUITEMINFO);
				menuInfo.fMask = MIIM_STATE;

				if (!::GetMenuItemInfo(hSysMenu, SC_CLOSE, FALSE, &menuInfo) ||
					(menuInfo.fState & MFS_GRAYED) || 
					(menuInfo.fState & MFS_DISABLED))
				{
					m_lstCloseDisabled.AddTail (hwndT);
				}
			}
		}

		hwndT=::GetWindow(hwndT,GW_HWNDNEXT);
	}

	m_wndList.SetRedraw(TRUE);
}


void CBCGPWindowsManagerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	if (nIDCtl == IDC_BCGBARRES_LIST)
	{
		if (lpDIS->itemID == LB_ERR) return; 
  
		HBRUSH		brBackground;
		RECT		rcTemp = lpDIS->rcItem; 
		HDC			hDC=lpDIS->hDC;

		COLORREF	clText; 

		if (lpDIS->itemState & ODS_SELECTED)  { 
			brBackground = GetSysColorBrush (COLOR_HIGHLIGHT); 
			clText = globalData.clrTextHilite; 
		} else { 
			brBackground = GetSysColorBrush (COLOR_WINDOW); 
			clText = globalData.clrWindowText;
		} 

		if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)) 	FillRect(hDC,&rcTemp, brBackground); 
	
		int			OldBkMode=::SetBkMode(hDC,TRANSPARENT); 
		COLORREF	clOldText=::SetTextColor(hDC,clText); 

		TCHAR		szBuf[1024];
		::SendMessage(lpDIS->hwndItem,LB_GETTEXT,(WPARAM) lpDIS->itemID,(LPARAM) szBuf);		
		
		int h=rcTemp.bottom-rcTemp.top;
		rcTemp.left+=h+4;
		DrawText(hDC,szBuf,-1,&rcTemp,DT_LEFT|DT_VCENTER|	
			DT_NOPREFIX| DT_SINGLELINE);

		HICON hIcon = (HICON)(LONG_PTR) GetClassLongPtr ((HWND) lpDIS->itemData, GCLP_HICONSM);
		rcTemp.left=lpDIS->rcItem.left;
		::DrawIconEx(hDC,rcTemp.left+2,rcTemp.top,			
			hIcon,h,h,0,0,DI_NORMAL);		
		
		::SetTextColor(hDC,clOldText);
		::SetBkMode(hDC,OldBkMode);
  
		if (lpDIS->itemAction & ODA_FOCUS)   DrawFocusRect(hDC,&lpDIS->rcItem); 
		return;		
	}
	CDialog::OnDrawItem(nIDCtl, lpDIS);
}

void CBCGPWindowsManagerDlg::OnBcgbarresWindowHelp() 
{
	CWnd* pParentFrame = AfxGetMainWnd();
	pParentFrame->SendMessage(WM_BCGWINDOW_HELP, 0, (LPARAM) this);	
}

BOOL CBCGPWindowsManagerDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	pHelpInfo->iCtrlId;
	CWnd* pParentFrame = AfxGetMainWnd();
	pParentFrame->SendMessage(WM_BCGWINDOW_HELP, 0, (LPARAM) this);	
	return FALSE;
}

