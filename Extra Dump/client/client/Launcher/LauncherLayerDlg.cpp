// LauncherLayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherLayerDlg.h"
#include "des.h"


// CLauncherLayerDlg dialog

IMPLEMENT_DYNAMIC(CLauncherLayerDlg, CDialog)

CLauncherLayerDlg::CLauncherLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherLayerDlg::IDD, pParent)
{

}

CLauncherLayerDlg::~CLauncherLayerDlg()
{
}

void CLauncherLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLauncherLayerDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CLauncherLayerDlg message handlers

BOOL CLauncherLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 若是没有写入过CompleteCheck，则先写一次
	if ( !IsSelfComplete() )
	{
		if ( ::MessageBoxW( this->GetSafeHwnd(), L"文件有损坏，要现在重新安装客户端吗？" , L"提示" , MB_YESNO ) == IDYES )
		{
			::ShellExecute( NULL, "open", "http://www.ljy0.com", NULL, NULL, SW_SHOWNORMAL );
		}	
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	if( FAILED( m_bkgImg.Load("./resource/DengLuMianBanDiBan.png") ) )
	{
		if ( ::MessageBoxW( this->GetSafeHwnd(), L"文件有损坏，要现在重新安装客户端吗？" , L"提示" , MB_YESNO ) == IDYES )
		{
			::ShellExecute( NULL, "open", "http://www.ljy0.com", NULL, NULL, SW_SHOWNORMAL );
		}		
		EndDialog(0);
	}
	SetWindowPos( NULL, 0,0, m_bkgImg.GetWidth(), m_bkgImg.GetHeight(), SWP_NOMOVE|SWP_NOZORDER );
	CenterWindow();
	SetAlphaWindow();
	CRect	rc;
	GetWindowRect( rc );

	m_dlg.Create( CLauncherDlg::IDD, this );
	m_dlg.SetWindowPos( NULL, rc.left,rc.top, m_bkgImg.GetWidth(), m_bkgImg.GetHeight(), SWP_NOZORDER );
	m_dlg.ShowWindow(SW_SHOW);

	m_flashDlg.Create( CLauncherFlashDialog::IDD, this );
	m_flashDlg.SetWindowPos( NULL, rc.left + 596,rc.top + 647, 388, 75, SWP_NOZORDER );
	m_flashDlg.ShowWindow(SW_SHOW);

	if ( GetACP() == 936 )
	{
		::SetWindowText( GetSafeHwnd(), "零纪元登录程序" );
	}
	else
	{
		::SetWindowText( GetSafeHwnd(), "Era Zero Launcher" );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLauncherLayerDlg::SetAlphaWindow()
{
	ModifyStyleEx( 0, WS_EX_LAYERED );
	CDC dcScreen;
	CDC dcMemory;
	dcScreen.Attach( ::GetDC(NULL) );
	dcMemory.CreateCompatibleDC( &dcScreen );
	PreMultiplyRGBChannels((BYTE*) m_bkgImg.GetBits(), m_bkgImg.GetPitch(), m_bkgImg.GetWidth(), m_bkgImg.GetHeight());
	CBitmap	bmp;
	bmp.Attach( (HGDIOBJ)(m_bkgImg) );
	CBitmap *pOldBitmap= dcMemory.SelectObject( &bmp );
	BITMAP	bmpInfo;
	bmp.GetBitmap( &bmpInfo );
	CRect rc;
	GetWindowRect( rc );
	BLENDFUNCTION	blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;
	CPoint	ptWindow( rc.TopLeft() );
	CSize	szWindow( bmpInfo.bmWidth, bmpInfo.bmHeight );
	CPoint	ptSrc(0, 0);
	UpdateLayeredWindow( &dcScreen, &ptWindow, &szWindow, &dcMemory, &ptSrc, RGB(255,255,255), &blend, ULW_ALPHA );
	bmp.Detach();
}

LRESULT CLauncherLayerDlg::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}	

void CLauncherLayerDlg::OnMove(int x, int y)
{
	if( m_dlg.GetSafeHwnd() )
	{
		m_dlg.SetWindowPos( NULL, x,y,0,0, SWP_NOSIZE|SWP_NOZORDER );
	}
	if( m_flashDlg.GetSafeHwnd() )
	{
		m_flashDlg.SetWindowPos( NULL, x+596,y+647,0,0, SWP_NOSIZE|SWP_NOZORDER );
	}
	CDialog::OnMove(x, y);
}

BOOL CLauncherLayerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if( wParam == CUSTOM_BUTTON_MSG )
	{
		return m_dlg.OnCommand(wParam,lParam);
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CLauncherLayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CLauncherLayerDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dlg.OnEnterGameClick();
	CDialog::OnOK();
}

void CLauncherLayerDlg::OnCancel()
{
	m_dlg.SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_CLOSE );
}
