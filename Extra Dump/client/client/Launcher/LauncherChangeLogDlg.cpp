// LauncherChangeLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherChangeLogDlg.h"
#include "LauncherChangeLogLayerDlg.h"
#include "utils.h"
#include <MsHTML.h>
#include <mshtmhst.h>


// LauncherChangeLogDlg dialog

IMPLEMENT_DYNAMIC(LauncherChangeLogDlg, CDialog)

LauncherChangeLogDlg::LauncherChangeLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LauncherChangeLogDlg::IDD, pParent)
{

}

LauncherChangeLogDlg::~LauncherChangeLogDlg()
{
}

void LauncherChangeLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_CloseBtn);
	DDX_Control(pDX, IDOK, m_OKBtn);
	DDX_Control(pDX, IDC_EXPLORER1, m_web);
	DDX_Control(pDX, IDC_SCROLLBAR, m_scroll);
// 	DDX_Control(pDX, IDC_STATIC1, m_Text);
}


BEGIN_MESSAGE_MAP(LauncherChangeLogDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDOK, &LauncherChangeLogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &LauncherChangeLogDlg::OnBnClickedButtonClose)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// LauncherChangeLogDlg message handlers

BOOL LauncherChangeLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	ModifyStyleEx( 0, WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB(255,0,255), 255, LWA_ALPHA | LWA_COLORKEY );

	RECT rc = {0,0,0,0};
	m_imgOK[0].Load("./resource/GuanBiC1.png");
	m_imgOK[1].Load("./resource/GuanBiC2.png");
	m_imgOK[2].Load("./resource/GuanBiC3.png");
	m_OKBtn.SetBitmaps(m_imgOK[1],0,m_imgOK[0],0,m_imgOK[2],0);
	m_OKBtn.SizeToContent();
	m_OKBtn.SetWindowPos(this,183,314,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgClose[0].Load("./resource/GuanBiB1.png");
	m_imgClose[1].Load("./resource/GuanBiB2.png");
	m_imgClose[2].Load("./resource/GuanBiB3.png");
	m_CloseBtn.SetBitmaps(m_imgClose[1],0,m_imgClose[0],0,m_imgClose[2],0);
	m_CloseBtn.SizeToContent();
	m_CloseBtn.SetWindowPos(this,433,7,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_web.SetWindowPos( NULL, 0,0, 0,0, SWP_NOZORDER );
	m_web.Navigate( "about:blank", NULL, NULL, NULL, NULL );

	m_scroll.ShowWindow( SW_HIDE );
	m_scroll.SetWindowPos( this, 429, 68, 16, 239, SWP_NOZORDER );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL LauncherChangeLogDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush	brush( RGB(255,0,255) );
	CRect	rc;
	GetClientRect( rc );
	pDC->FillRect( rc, &brush );
	return FALSE;
}

BEGIN_EVENTSINK_MAP(LauncherChangeLogDlg, CDialog)
	ON_EVENT(LauncherChangeLogDlg, IDC_EXPLORER1, 259, LauncherChangeLogDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void LauncherChangeLogDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	CString str( URL->bstrVal );
	if( str != "about:blank" )
	{
		m_web.SetWindowPos( NULL, 17,66, 429,242, SWP_NOZORDER );
	}else{
		string ver = m_strNewVer;

		size_t pos = ver.find( '.' );
		while ( pos != ver.npos )
		{
			ver.replace(pos, 1, "_");
			pos = ver.find( '.' );
		}
		ver = "http://www.ljy0.com/new_launcher_2013_5/" + ver + string(".html");
		m_web.Navigate(ver.c_str(), NULL, NULL, NULL, NULL);
		return;
	}

	IHTMLElement* pEl;
	IHTMLBodyElement * pBodyEl;
	IHTMLTextContainer* pTextCon;
	IHTMLDocument2* pDoc = ( IHTMLDocument2* )m_web.get_Document();
	if(pDoc != NULL)
	{
		if (SUCCEEDED(pDoc->get_body(&pEl)))
		{
			if (SUCCEEDED(pEl->QueryInterface(IID_IHTMLBodyElement, (void**)&pBodyEl)))
			{
				pBodyEl->put_scroll((L"no"));
				pBodyEl->Release();
			}
			if( SUCCEEDED( pEl->QueryInterface(IID_IHTMLTextContainer, (void**)&pTextCon )))
			{
				long height;
				pTextCon->get_scrollHeight(&height);
				m_scroll.SetScrollRange( 0, height - 243 );
				pTextCon->Release();
			}
			pEl->Release();
		}
		pDoc->Release();
	}	
}

void LauncherChangeLogDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
// 	if(	pScrollBar->m_hWnd==m_scroll.m_hWnd)
// 	{
// 		SCROLLINFO si;
// 		si.fMask=SIF_ALL;
// 		m_scroll.GetScrollInfo(&si,SIF_ALL);
// 		switch(nSBCode)
// 		{
// 		case SB_LINEUP:
// 			si.nPos--;
// 			break;
// 		case SB_LINEDOWN:
// 			si.nPos++;
// 			break;
// 		case SB_PAGEUP:
// 			si.nPos-=si.nPage;
// 			break;
// 		case SB_PAGEDOWN:
// 			si.nPos+=si.nPage;
// 			break;
// 		case SB_THUMBTRACK:
// 			si.nPos=nPos;
// 			break;
// 		}
// 		TRACE("\nPos=%d",si.nPos);
// 		if(si.nPos>(int)(si.nMax-si.nMin-si.nPage+1)) si.nPos=si.nMax-si.nMin-si.nPage+1;
// 		if(si.nPos<si.nMin) si.nPos=si.nMin;
// 
// 		IHTMLDocument2* pDoc = ( IHTMLDocument2* )m_web.get_Document();
// 		IHTMLWindow2* pwindow;
// 		pDoc->get_parentWindow(& pwindow);
// 		pwindow->scrollTo( 0, si.nPos );
// 		pwindow->Release();
// 
// 		si.fMask=SIF_POS;
// 		m_scroll.SetScrollInfo(&si);
// 	}
// 
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void LauncherChangeLogDlg::OnBnClickedOk()
{
	OnOK();
	((LauncherChangeLogLayerDlg*)GetParent())->OnOK();
}

void LauncherChangeLogDlg::OnBnClickedButtonClose()
{
	OnBnClickedOk();
}
void LauncherChangeLogDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	OnBnClickedOk();
	CDialog::OnCancel();
}

void LauncherChangeLogDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnOK();
}

void LauncherChangeLogDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	HDC hdc = dc.GetSafeHdc();
	CBrush	brush( RGB(255,0,255) );
	RECT rect = { 30, 85, 409, 16 };
	
	dc.FillRect( &rect, &brush );
	DrawFont( hdc, RGB( 75, 75, 75 ), m_strVer.c_str(), rect, 12, true );
}
