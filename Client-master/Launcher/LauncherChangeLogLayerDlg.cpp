// LauncherChangeLogLayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherChangeLogLayerDlg.h"


// LauncherChangeLogLayerDlg dialog

IMPLEMENT_DYNAMIC(LauncherChangeLogLayerDlg, CDialog)

LauncherChangeLogLayerDlg::LauncherChangeLogLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LauncherChangeLogLayerDlg::IDD, pParent)
{

}

LauncherChangeLogLayerDlg::~LauncherChangeLogLayerDlg()
{
}

void LauncherChangeLogLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LauncherChangeLogLayerDlg, CDialog)
	ON_WM_MOVE()
END_MESSAGE_MAP()

void LauncherChangeLogLayerDlg::SetAlphaWindow()
{
	ModifyStyleEx( 0, WS_EX_LAYERED );
	CDC dcScreen;
	CDC dcMemory;
	dcScreen.Attach( ::GetDC(NULL) );
	dcMemory.CreateCompatibleDC( &dcScreen );
	PreMultiplyRGBChannels((BYTE*) m_imgBack.GetBits(), m_imgBack.GetPitch(), m_imgBack.GetWidth(), m_imgBack.GetHeight());
	CBitmap	bmp;
	bmp.Attach( (HGDIOBJ)(m_imgBack) );
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
	CPoint	ptWindow( rc.TopLeft().x + 45, rc.TopLeft().y );
	CSize	szWindow( bmpInfo.bmWidth, bmpInfo.bmHeight );
	CPoint	ptSrc(0, 0);
	UpdateLayeredWindow( &dcScreen, &ptWindow, &szWindow, &dcMemory, &ptSrc, RGB(255,255,255), &blend, ULW_ALPHA );
	bmp.Detach();
}



// LauncherChangeLogLayerDlg message handlers

BOOL LauncherChangeLogLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_imgBack.Load("./resource/GengXinRiZhiTanChuKuang.png");
	SetWindowPos( NULL, 0,0, m_imgBack.GetWidth(), m_imgBack.GetHeight(), SWP_NOMOVE|SWP_NOZORDER );
	CenterWindow();
	SetAlphaWindow();

	CRect	rc;
	GetWindowRect( rc );
	m_dlg.Create( LauncherChangeLogDlg::IDD, this );
	m_dlg.SetWindowPos( NULL, rc.left,rc.top, m_imgBack.GetWidth(), m_imgBack.GetHeight(), SWP_NOZORDER );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LauncherChangeLogLayerDlg::OnMove(int x, int y)
{
	if( m_dlg.GetSafeHwnd() )
	{
		m_dlg.SetWindowPos( NULL, x,y,0,0, SWP_NOSIZE|SWP_NOZORDER );
	}

	CDialog::OnMove(x, y);
}

void LauncherChangeLogLayerDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	TCHAR wndName[MAX_PATH] = {0};
	if ( GetACP() == 936 )
	{ 
		lstrcat( wndName, "Áã¼ÍÔªµÇÂ¼³ÌÐò" );
	}
	else
	{
		lstrcat( wndName, "Era Zero Launcher" );
	}

	CWnd* pWnd = FindWindow(NULL,wndName);
	if ( pWnd != NULL )
	{
		pWnd->EnableWindow();
	}
	CDialog::OnOK();
}

void LauncherChangeLogLayerDlg::setVersionInfo( const string& ver, const string& newVer )
{
	m_dlg.m_strVer = ver;
	m_dlg.m_strNewVer = newVer;
}