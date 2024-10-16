// GraphSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "GraphSetting.h"
#include "GraphSettingLayerDlg.h"


// CGraphSettingDlg dialog

IMPLEMENT_DYNAMIC(CGraphSettingLayerDlg, CDialog)

CGraphSettingLayerDlg::CGraphSettingLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphSettingLayerDlg::IDD, pParent)
{

}

CGraphSettingLayerDlg::~CGraphSettingLayerDlg()
{
}

void CGraphSettingLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphSettingLayerDlg, CDialog)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CGraphSettingDlg message handlers
void CGraphSettingLayerDlg::SetAlphaWindow()
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
	//CPoint	ptWindow( rc.TopLeft() );
	CPoint	ptWindow( rc.TopLeft().x + 45, rc.TopLeft().y );
	CSize	szWindow( bmpInfo.bmWidth, bmpInfo.bmHeight );
	CPoint	ptSrc(0, 0);
	UpdateLayeredWindow( &dcScreen, &ptWindow, &szWindow, &dcMemory, &ptSrc, RGB(255,255,255), &blend, ULW_ALPHA );
	bmp.Detach();
}


BOOL CGraphSettingLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_imgBack.Load("./resource/TanChuKuangDiBan.png");
	SetWindowPos( NULL, 0,0, m_imgBack.GetWidth(), m_imgBack.GetHeight(), SWP_NOMOVE|SWP_NOZORDER );
	CenterWindow();
	SetAlphaWindow();
	CRect	rc;
	GetWindowRect( rc );
	m_dlg.Create( CGraphSetting::IDD, this );
	m_dlg.SetWindowPos( NULL, rc.left,rc.top, m_imgBack.GetWidth(), m_imgBack.GetHeight(), SWP_NOZORDER );

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CGraphSettingLayerDlg::OnMove(int x, int y)
{
	if( m_dlg.GetSafeHwnd() )
	{
		m_dlg.SetWindowPos( NULL, x,y,0,0, SWP_NOSIZE|SWP_NOZORDER );
	}
	CDialog::OnMove(x, y);
}

void CGraphSettingLayerDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}
