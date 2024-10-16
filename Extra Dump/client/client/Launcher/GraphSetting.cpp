// GraphSetting.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "GraphSetting.h"
#include "tinyxml.h"
#include "util/utils.h"
#include "GraphSettingLayerDlg.h"
#include <algorithm>

const int MIN_WIDTH		= 800;
const int MIN_HEIGHT	= 600;

#define MAX_SOLUTION_OPTION 6

bool operator< ( const DEVMODE& lhs, const DEVMODE& rhs )
{
	return lhs.dmPelsWidth < rhs.dmPelsWidth;
}

bool operator== ( const DEVMODE& lhs, const DEVMODE& rhs )
{
	return ( lhs.dmPelsWidth == rhs.dmPelsWidth ) && ( lhs.dmPelsHeight == rhs.dmPelsHeight );
}


const int CUSTOM_SET_BUTTON_MSG = 0xDEBA;

std::vector<DEVMODE>	CGraphSetting::s_AllModes;
DEVMODE					CGraphSetting::s_DeskMode;//当前桌面模式
std::vector<DEVMODE>		CGraphSetting::s_FullScreenModes;
std::vector<DEVMODE>		CGraphSetting::s_WideScreenModes;


enum
{
	UI_SET_CANCEL,
};

// CGraphSetting dialog

IMPLEMENT_DYNAMIC(CGraphSetting, CDialog)

CGraphSetting::CGraphSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphSetting::IDD, pParent)
{

}

CGraphSetting::~CGraphSetting()
{
}

void CGraphSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combSample);
	DDX_Control(pDX, IDC_CHECK1, m_chkWindowMode);
	DDX_Control(pDX, IDC_CHECK2, m_chkFullScreen);
	DDX_Control(pDX, IDC_CHECK3, m_chkWideScreen);
	DDX_Control(pDX, IDC_BUTTON4, m_btnLowPreSetting);
	DDX_Control(pDX, IDC_BUTTON5, m_btnMediumPreSetting);
	DDX_Control(pDX, IDC_BUTTON6, m_btnHighPreSetting);
	DDX_Control(pDX, IDC_BUTTON7, m_btnDefaultSetting);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BUTTON8, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON9, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON1, m_btnDefaultPreSetting);
	DDX_Control(pDX, IDC_BUTTON2, m_btnMusic);
	DDX_Control(pDX, IDC_BUTTON3, m_btnSound);
	DDX_Control(pDX, IDC_SLIDER_MUSIC,m_bitmapSliderMusic);
	DDX_Control(pDX, IDC_SLIDER_EFFECT,m_bitmapSliderEffect);
}

BEGIN_MESSAGE_MAP(CGraphSetting, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK2, &CGraphSetting::OnBnClickedFullCheck)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CGraphSetting::OnCbnSelchangeSampleCombo)
	ON_BN_CLICKED(IDC_BUTTON1, &CGraphSetting::OnBnClickedPreSetButton)
	ON_BN_CLICKED(IDC_CHECK1, &CGraphSetting::OnBnClickedWindowCheck)
	ON_BN_CLICKED(IDC_CHECK3, &CGraphSetting::OnBnClickedWideCheck)
	ON_BN_CLICKED(IDC_BUTTON4, &CGraphSetting::OnBnClickedLowButton)
	ON_BN_CLICKED(IDC_BUTTON5, &CGraphSetting::OnBnClickedMediumButton)
	ON_BN_CLICKED(IDC_BUTTON6, &CGraphSetting::OnBnClickedHighButton)
	ON_BN_CLICKED(IDC_BUTTON7, &CGraphSetting::OnBnClickedDefaultButton)
	ON_BN_CLICKED(IDC_BUTTON8, &CGraphSetting::OnBnClickedConfirmButton)
	ON_BN_CLICKED(IDC_BUTTON9, &CGraphSetting::OnBnClickedCancelButton)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE,&CGraphSetting::OnBnClickedCloseButton)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVED, OnBitmapSliderMoved)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnBitmapSliderMoving)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CGraphSetting::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 2)
	{
		this->EndDialog(IDCANCEL);
		((CGraphSettingLayerDlg*)GetParent())->EndDialog(IDCANCEL);
	}
	if ( wParam == CUSTOM_SET_BUTTON_MSG )
	{
		switch ( lParam )
		{
		case UI_SET_CANCEL:
			this->EndDialog( IDCANCEL );
			break;

		default:
			break;
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}

// CGraphSetting message handlers

BOOL CGraphSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyleEx( 0, WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB(255,0,255), 255, LWA_ALPHA | LWA_COLORKEY );
	InitDisplayMode();
	m_SystemSettingConfig.LoadSystemSettingFile();	
	m_combSample.ResetContent();
	// 设置音效
 	m_bitmapSliderEffect.SetRangeMax( 100 );
 	m_bitmapSliderEffect.SetRangeMin( 0 );
 	m_bitmapSliderEffect.SetPos( (int)(m_SystemSettingConfig.m_fGlobalSoundVolume * m_bitmapSliderEffect.GetRangeMax()) );

	// 设置音乐
 	m_bitmapSliderMusic.SetRangeMax( 100 );
 	m_bitmapSliderMusic.SetRangeMin( 0 );
 	m_bitmapSliderMusic.SetPos( (int)(m_SystemSettingConfig.m_fGlobalMusicVolume * m_bitmapSliderMusic.GetRangeMax()) );

	m_bitmapSliderMusic.SetWindowPos(this,151,162,278,17,SWP_NOZORDER);
	m_bitmapSliderEffect.SetWindowPos(this,151,210,278,17,SWP_NOZORDER);

	m_bitmapSliderMusic.SetBitmapChannel(IDB_BITMAP3,NULL,FALSE);
	m_bitmapSliderMusic.SetBitmapThumb(IDB_BITMAP2,NULL,NULL);

	m_bitmapSliderEffect.SetBitmapChannel(IDB_BITMAP4,NULL,FALSE);
	m_bitmapSliderEffect.SetBitmapThumb(IDB_BITMAP2,NULL,NULL);

	m_combSample.SetWindowPos(this,23,56,110,80,SWP_NOZORDER);

	if ( m_SystemSettingConfig.m_bFullScreen )
	{
		m_chkWindowMode.SetCheck( BST_UNCHECKED );
		m_chkWideScreen.SetCheck( BST_UNCHECKED );
		m_chkFullScreen.SetCheck( BST_CHECKED );
		//这里显示 所有全屏的分辨率
		CString str;
		for( size_t i = 0 ; i < s_FullScreenModes.size() ; i++ )
		{
			str.Format("%d*%d",s_FullScreenModes[i].dmPelsWidth,s_FullScreenModes[i].dmPelsHeight);
			m_combSample.InsertString(-1,str);
		}
	 	int nSelectIndex = 0;
	 	for ( size_t i = 0 ; i < s_FullScreenModes.size() ; i++  )
	 	{
	 		if ( m_SystemSettingConfig.m_nWinWidth == s_FullScreenModes[i].dmPelsWidth && 
	 				m_SystemSettingConfig.m_nWinHeight == s_FullScreenModes[i].dmPelsHeight )
	 		{
	 			nSelectIndex = i;
	 		}
	 	}
		m_combSample.SetCurSel(nSelectIndex);
	}else{
		m_chkWindowMode.SetCheck( BST_CHECKED );
		m_chkWideScreen.SetCheck( BST_UNCHECKED );
		m_chkFullScreen.SetCheck( BST_UNCHECKED );

		CString str;
		for( size_t i = 0 ; i < s_AllModes.size() ; i++ )
		{
			str.Format("%d*%d",s_AllModes[i].dmPelsWidth,s_AllModes[i].dmPelsHeight);
			m_combSample.InsertString(-1,str);
		}
		int nSelectIndex = 0;
		for ( size_t i = 0 ; i < s_AllModes.size() ; i++  )
		{
			if ( m_SystemSettingConfig.m_nWinWidth == s_AllModes[i].dmPelsWidth && 
				m_SystemSettingConfig.m_nWinHeight == s_AllModes[i].dmPelsHeight )
			{
				nSelectIndex = i;
			}
		}
		m_combSample.SetCurSel(nSelectIndex);
	}

	m_imgcheck[0].Load("./resource/GouXuan1.png");
	m_imgcheck[1].Load("./resource/GouXuan2.png");
	m_chkWindowMode.SetBitmaps(m_imgcheck[0],0,m_imgcheck[1],0,m_imgcheck[0],0);
	m_chkWindowMode.SizeToContent();
	m_chkWindowMode.SetWindowPos(this,174,57,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_chkFullScreen.SetBitmaps(m_imgcheck[0],0,m_imgcheck[1],0,m_imgcheck[0],0);
	m_chkFullScreen.SizeToContent();
	m_chkFullScreen.SetWindowPos(this,268,57,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_chkWideScreen.SetBitmaps(m_imgcheck[0],0,m_imgcheck[1],0,m_imgcheck[0],0);
	m_chkWideScreen.SizeToContent();
	m_chkWideScreen.SetWindowPos(this,32,60,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_imgLowPreSetting[0].Load("./resource/Di1.png");
	m_imgLowPreSetting[1].Load("./resource/Di2.png");
	m_imgLowPreSetting[2].Load("./resource/Di3.png");
	m_btnLowPreSetting.SetBitmaps(m_imgLowPreSetting[0],0,m_imgLowPreSetting[1],0,m_imgLowPreSetting[2],0);
	m_btnLowPreSetting.SizeToContent();
	m_btnLowPreSetting.SetWindowPos(this,153,104,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_imgMediumPreSetting[0].Load("./resource/Zhong1.png");
	m_imgMediumPreSetting[1].Load("./resource/Zhong2.png");
	m_imgMediumPreSetting[2].Load("./resource/Zhong3.png");
	m_btnMediumPreSetting.SetBitmaps(m_imgMediumPreSetting[0],0,m_imgMediumPreSetting[1],0,m_imgMediumPreSetting[2],0);
	m_btnMediumPreSetting.SizeToContent();
	m_btnMediumPreSetting.SetWindowPos(this,210,104,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_imgHighPreSetting[0].Load("./resource/Gao1.png");
	m_imgHighPreSetting[1].Load("./resource/Gao2.png");
	m_imgHighPreSetting[2].Load("./resource/Gao3.png");
	m_btnHighPreSetting.SetBitmaps(m_imgHighPreSetting[0],0,m_imgHighPreSetting[1],0,m_imgHighPreSetting[2],0);
	m_btnHighPreSetting.SizeToContent();
	m_btnHighPreSetting.SetWindowPos(this,267,104,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgDefaultPreSetting[0].Load("./resource/HuaMianYuShe  GaoLiang.png");
	m_imgDefaultPreSetting[1].Load("./resource/HuaMianYuShe  DaiJi.png");
	m_imgDefaultPreSetting[2].Load("./resource/HuaMianYuShe  AnXia.png");
	m_btnDefaultPreSetting.SetBitmaps(m_imgDefaultPreSetting[0],0,m_imgDefaultPreSetting[1],0,m_imgDefaultPreSetting[2],0);
	m_btnDefaultPreSetting.SizeToContent();
	m_btnDefaultPreSetting.SetWindowPos(this,29,182,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_btnMusic.SetWindowPos(this,27,110,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW);

	m_btnSound.SetWindowPos(this,27,146,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW);

	m_btnDefaultSetting.SetWindowPos(this,20,182,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW);

	m_btnClose.SetWindowPos(this,375,5,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW);

	m_imgOk[0].Load("./resource/QueRen1.png");
	m_imgOk[1].Load("./resource/QueRen2.png");
	m_imgOk[2].Load("./resource/QueRen3.png");
	m_btnOk.SetBitmaps(m_imgOk[0],0,m_imgOk[1],0,m_imgOk[2],0);
	m_btnOk.SizeToContent();
	m_btnOk.SetWindowPos(this,273,246,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgCancel[0].Load("./resource/QuXiao1.png");
	m_imgCancel[1].Load("./resource/QuXiao2.png");
	m_imgCancel[2].Load("./resource/QuXiao3.png");
	m_btnCancel.SetBitmaps(m_imgCancel[0],0,m_imgCancel[1],0,m_imgCancel[2],0);
	m_btnCancel.SizeToContent();
	m_btnCancel.SetWindowPos(this,357,246,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_btnLowPreSetting.SetCheck( BST_UNCHECKED );
	m_btnMediumPreSetting.SetCheck( BST_UNCHECKED );
	m_btnHighPreSetting.SetCheck( BST_UNCHECKED );
	//设置画质
	if( m_SystemSettingConfig.m_bWaterReflect == true && m_SystemSettingConfig.m_bDistort == true)
	{
		m_btnHighPreSetting.SetCheck( BST_CHECKED );
	}else if( m_SystemSettingConfig.m_bDistort == true )
	{
		m_btnMediumPreSetting.SetCheck( BST_CHECKED );
	}else{
		m_btnLowPreSetting.SetCheck( BST_CHECKED );
	}

	return TRUE;
}

void CGraphSetting::InitDisplayMode()
{
	DISPLAY_DEVICE device;
	DEVMODE devMode;
	device.cb = sizeof(DISPLAY_DEVICE);
	int nIndex = 0;
	s_AllModes.clear();
	s_FullScreenModes.clear();
	while( EnumDisplayDevices(NULL,nIndex,&device,0) )
	{
		if( device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP )
		{
			EnumDisplaySettings(device.DeviceName, ENUM_CURRENT_SETTINGS, &s_DeskMode);
			int modeExist = EnumDisplaySettings(device.DeviceName, 0, &devMode);
			if(modeExist == 0)
			{
				break;
			}
			for(int i=0; modeExist; i++)
			{
				if ( devMode.dmBitsPerPel == s_DeskMode.dmBitsPerPel && 
					devMode.dmPelsHeight >= MIN_HEIGHT && devMode.dmPelsWidth >= MIN_WIDTH )//刷新频率和颜色深度相同，比例一致就保存起来
				{
					s_AllModes.push_back(devMode);
				}
				modeExist = EnumDisplaySettings(NULL, i, &devMode);
			}
		}
		nIndex ++;
	}

	std::sort( s_AllModes.begin(),s_AllModes.end() );
 	s_AllModes.resize(std::unique(s_AllModes.begin(),s_AllModes.end()) - s_AllModes.begin() );
	if( s_AllModes.size() > MAX_SOLUTION_OPTION - 1 )
	{
		s_AllModes.resize(MAX_SOLUTION_OPTION - 1);
	}
	devMode.dmPelsWidth = GetSystemMetrics( SM_CXSCREEN );
	devMode.dmPelsHeight = GetSystemMetrics( SM_CYSCREEN );
	std::vector<DEVMODE>::iterator it = s_AllModes.begin();
	while( it != s_AllModes.end() )
	{
		if( it->dmPelsWidth > devMode.dmPelsWidth || it->dmPelsHeight > devMode.dmPelsHeight )
		{
			it = s_AllModes.erase( it );
		}else{
			++it;
		}
	}

	if( std::find(s_AllModes.begin(),s_AllModes.end(),devMode) == s_AllModes.end() )
	{
		s_AllModes.push_back(devMode);
	}
	s_FullScreenModes.push_back( devMode );
}

void CGraphSetting::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	HDC hdc = dc.GetSafeHdc();
	if ( hdc == NULL )
	{
		return;
	}
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CGraphSetting::OnBnClickedFullCheck()
{
	// TODO: Add your control notification handler code here
	m_SystemSettingConfig.m_bFullScreen	= true;
	m_chkWindowMode.SetCheck( BST_UNCHECKED );
	m_chkFullScreen.SetCheck( BST_CHECKED );
	m_chkWideScreen.SetCheck( BST_UNCHECKED );
	CString str;
	m_combSample.ResetContent();
	for( size_t i = 0 ; i < s_FullScreenModes.size() ; i++ )
	{
		str.Format("%d*%d",s_FullScreenModes[i].dmPelsWidth,s_FullScreenModes[i].dmPelsHeight);
		m_combSample.InsertString(-1,str);
	}
	m_combSample.SetCurSel(0);
	OnCbnSelchangeSampleCombo();
}

void CGraphSetting::OnBnClickedWindowCheck()
{
	// TODO: Add your control notification handler code here
	m_SystemSettingConfig.m_bFullScreen = false;
	m_chkWindowMode.SetCheck( BST_CHECKED );
	m_chkFullScreen.SetCheck( BST_UNCHECKED );
	m_chkWideScreen.SetCheck( BST_UNCHECKED );
	CString str;
	m_combSample.ResetContent();
	for( size_t i = 0 ; i < s_AllModes.size() ; i++ )
	{
		str.Format("%d*%d",s_AllModes[i].dmPelsWidth,s_AllModes[i].dmPelsHeight);
		m_combSample.InsertString(-1,str);
	}
	m_combSample.SetCurSel(0);
	OnCbnSelchangeSampleCombo();
}

void CGraphSetting::OnBnClickedWideCheck()
{
	// TODO: Add your control notification handler code here
	m_SystemSettingConfig.m_bFullScreen	= false;
	m_combSample.SetCurSel(0);
	m_chkWindowMode.SetCheck( BST_UNCHECKED );
	m_chkFullScreen.SetCheck( BST_UNCHECKED );
	m_chkWideScreen.SetCheck( BST_CHECKED );
	CString str;
	m_combSample.ResetContent();
	for( size_t i = 0 ; i < s_WideScreenModes.size() ; i++ )
	{
		str.Format("%d*%d",s_WideScreenModes[i].dmPelsWidth,s_WideScreenModes[i].dmPelsHeight);
		m_combSample.InsertString(-1,str);
	}
	m_combSample.SetCurSel(0);
	OnCbnSelchangeSampleCombo();
}

void CGraphSetting::OnCbnSelchangeSampleCombo()
{
	// TODO: Add your control notification handler code here
	int nIndex	= m_combSample.GetCurSel();
	int nCount	= m_combSample.GetCount();
#ifdef _DEBUG
	assert( nIndex < nCount );
#else
	if ( nIndex >= nCount )
	{
		return;
	}
#endif

	if( m_chkWindowMode.GetCheck() == BST_CHECKED )
	{
		m_SystemSettingConfig.m_nWinWidth = s_AllModes[nIndex].dmPelsWidth;
		m_SystemSettingConfig.m_nWinHeight = s_AllModes[nIndex].dmPelsHeight;
	}else if( m_chkFullScreen.GetCheck() == BST_CHECKED )
	{
		m_SystemSettingConfig.m_nWinWidth = s_FullScreenModes[nIndex].dmPelsWidth;
		m_SystemSettingConfig.m_nWinHeight = s_FullScreenModes[nIndex].dmPelsHeight;
	}else if( m_chkWideScreen.GetCheck() == BST_CHECKED )
	{
		m_SystemSettingConfig.m_nWinWidth = s_WideScreenModes[nIndex].dmPelsWidth;
		m_SystemSettingConfig.m_nWinHeight = s_WideScreenModes[nIndex].dmPelsHeight;
	}
}

void CGraphSetting::OnBnClickedPreSetButton()
{
	// TODO: Add your control notification handler code here
}

void CGraphSetting::OnBnClickedLowButton()
{
	// TODO: Add your control notification handler code here
	m_SystemSettingConfig.m_bBloom				= false;
	m_SystemSettingConfig.m_bDistort			= false;
	m_SystemSettingConfig.m_bWaterReflect		= false;
	m_SystemSettingConfig.m_nShadowmapSize		= 0;
	m_SystemSettingConfig.m_nMultiSampleLevel	= 0;
	m_btnLowPreSetting.SetCheck( BST_CHECKED );
	m_btnMediumPreSetting.SetCheck( BST_UNCHECKED );
	m_btnHighPreSetting.SetCheck( BST_UNCHECKED );
}

void CGraphSetting::OnBnClickedMediumButton()
{
	// TODO: Add your control notification handler code here
	m_SystemSettingConfig.m_bBloom				= false;
	m_SystemSettingConfig.m_bDistort			= true;
	m_SystemSettingConfig.m_bWaterReflect		= false;
	m_SystemSettingConfig.m_nShadowmapSize		= 2048;
	m_SystemSettingConfig.m_nMultiSampleLevel	= 0;
	m_btnLowPreSetting.SetCheck( BST_UNCHECKED );
	m_btnMediumPreSetting.SetCheck( BST_CHECKED );
	m_btnHighPreSetting.SetCheck( BST_UNCHECKED );
}

void CGraphSetting::OnBnClickedHighButton()
{
	// TODO: Add your control notification handler code here
	m_SystemSettingConfig.m_bBloom				= false;
	m_SystemSettingConfig.m_bDistort			= true;
	m_SystemSettingConfig.m_bWaterReflect		= true;
	m_SystemSettingConfig.m_nShadowmapSize		= 2048;
	m_SystemSettingConfig.m_nMultiSampleLevel	= 0;
	m_btnLowPreSetting.SetCheck( BST_UNCHECKED );
	m_btnMediumPreSetting.SetCheck( BST_UNCHECKED );
	m_btnHighPreSetting.SetCheck( BST_CHECKED );
}

void CGraphSetting::OnBnClickedDefaultButton()
{
	// TODO: Add your control notification handler code here
// 	m_SystemSettingConfig = SystemSettingConfig();
// 	m_chkWindowMode.SetCheck( BST_UNCHECKED );
// 	m_chkWideScreen.SetCheck( BST_UNCHECKED );
// 	m_chkFullScreen.SetCheck( BST_CHECKED );
	OnBnClickedFullCheck();
	OnBnClickedLowButton();

	m_combSample.SetCurSel( 0 );
// 	m_SystemSettingConfig.m_bFullScreen	= true;
// 	m_SystemSettingConfig.m_nWinWidth = 1024;
// 	m_SystemSettingConfig.m_nWinHeight = 768;
	m_SystemSettingConfig.m_fGlobalMusicVolume = 0.5f;
	m_SystemSettingConfig.m_fGlobalSoundVolume = 0.5f;
	m_bitmapSliderMusic.SetPos( (int)(m_SystemSettingConfig.m_fGlobalMusicVolume * m_bitmapSliderMusic.GetRangeMax()) );
	m_bitmapSliderEffect.SetPos( (int)(m_SystemSettingConfig.m_fGlobalSoundVolume * m_bitmapSliderEffect.GetRangeMax()) );
}

void CGraphSetting::OnBnClickedConfirmButton()
{
	// TODO: Add your control notification handler code here
	m_SystemSettingConfig.SaveSystemSettingFile();
	OnOK();
	((CGraphSettingLayerDlg*)GetParent())->OnCancel();
}

void CGraphSetting::OnBnClickedCancelButton()
{
	OnCancel();
	((CGraphSettingLayerDlg*)GetParent())->OnCancel();
}

void CGraphSetting::OnBnClickedCloseButton()
{
	OnCancel();
	((CGraphSettingLayerDlg*)GetParent())->OnCancel();
}

BOOL CGraphSetting::OnEraseBkgnd(CDC* pDC)
{
// 	CDC memdc;
// 	CRect rect;
// 	GetClientRect(&rect);
// 	memdc.CreateCompatibleDC(pDC);
// 	memdc.SelectObject(&m_Background);
// 	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memdc,0,0,SRCCOPY);
	CBrush	brush( RGB(255,0,255) );
	CRect	rc;
	GetClientRect( rc );
	pDC->FillRect( rc, &brush );
	return FALSE;

	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CGraphSetting::OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam)
{
	switch( wParam )
	{
	case IDC_SLIDER_MUSIC:
		m_SystemSettingConfig.m_fGlobalMusicVolume = static_cast<float>( m_bitmapSliderMusic.GetPos() ) 
		 													/ m_bitmapSliderMusic.GetRangeMax();

		break;

	case IDC_SLIDER_EFFECT :
		m_SystemSettingConfig.m_fGlobalSoundVolume = static_cast<float>( m_bitmapSliderEffect.GetPos() ) 
			 													/ m_bitmapSliderEffect.GetRangeMax();
		break;
	}
	return 0;
}

LRESULT CGraphSetting::OnBitmapSliderMoving(WPARAM wParam, LPARAM lParam)
{
	switch( wParam )
	{
	case IDC_SLIDER_MUSIC:
		m_SystemSettingConfig.m_fGlobalMusicVolume = static_cast<float>( m_bitmapSliderMusic.GetPos() ) 
			/ m_bitmapSliderMusic.GetRangeMax();

		break;

	case IDC_SLIDER_EFFECT :
		m_SystemSettingConfig.m_fGlobalSoundVolume = static_cast<float>( m_bitmapSliderEffect.GetPos() ) 
			/ m_bitmapSliderEffect.GetRangeMax();
		break;
	}
	return 0;
}
