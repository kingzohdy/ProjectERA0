// LauncherSelectView.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include <map>
#include <assert.h>
#include <string>
#include "Launcher.h"
#include "LauncherSelectView.h"
#include "tinyxml.h"
#include "util/utils.h"
#include "LauncherDlg.h"
#include "DirServeSelector.h"
#include "LauncherDlg.h"
using namespace std;

enum
{
	UI_SELECT_WORLD_BUTTON,
	UI_SELECT_SERVE_BUTTON,
	UI_SELECT_CONFIRM_BUTTON,
	UI_SELECT_CLOSE,
};

const int NONE_SELECT  = -1;
// CLauncherSelectView dialog

std::string	CLauncherSelectView::m_strPingInfo;

IMPLEMENT_DYNAMIC(CLauncherSelectView, CDialog)

CLauncherSelectView::CLauncherSelectView(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherSelectView::IDD, pParent), m_nCurSelectArea( NONE_SELECT ), m_nCurSelectServe( NONE_SELECT )
// 		m_pPing( NULL )
{
	
	m_vecAreaBtns.push_back( &m_btnBigWorld1 );
	m_vecAreaBtns.push_back( &m_btnBigWorld2 );
	m_vecAreaBtns.push_back( &m_btnBigWorld3 );
	m_vecAreaBtns.push_back( &m_btnBigWorld4 );
	
	m_vecServeBtns.push_back( &m_btnServe1 );
	m_vecServeBtns.push_back( &m_btnServe2 );
	m_vecServeBtns.push_back( &m_btnServe3 );
	m_vecServeBtns.push_back( &m_btnServe4 );
	m_vecServeBtns.push_back( &m_btnServe5 );
	m_vecServeBtns.push_back( &m_btnServe6 );
	m_vecServeBtns.push_back( &m_btnServe7 );
	for ( LstImageButtons::iterator iter = m_vecAreaBtns.begin(), end = m_vecAreaBtns.end();
			iter != end; ++iter )
	{
		( *iter )->SetFontStyle( "宋体" );
	}

	for ( LstImageButtons::iterator iter = m_vecServeBtns.begin(), end = m_vecServeBtns.end();
		iter != end; ++iter )
	{
		( *iter )->SetFontStyle( "宋体" );
	}

	m_vecFunctionBtns.push_back( &m_btnConfirmSelect );
	m_vecFunctionBtns.push_back( &m_btnClose );

	// 一开始默认选中上次登录的区和服务器
	char szPath[128] = { '\0' };
	GetFilePath( "UserInfoLoginRecord.ini", szPath, sizeof( szPath ) );
	/*m_nCurSelectArea	= ::GetPrivateProfileInt( "Area",		"AreaID",	0,	szPath );
	m_nCurSelectServe	= ::GetPrivateProfileInt( "LoginWorld",	"ID",		0,	szPath );*/
}

CLauncherSelectView::~CLauncherSelectView()
{
// 	SafeDelete( m_pPing );
}

BOOL CLauncherSelectView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if ( wParam == CUSTOM_SELECT_BUTTON_MSG )
	{
		switch ( lParam % 10 )
		{
		case UI_SELECT_WORLD_BUTTON:
			{
				// 显示服务器列表
				m_nCurSelectArea	= (int)lParam / 10 - 1;
				m_nCurSelectServe	= 0;
				
				this->InvalidateRect( NULL, FALSE );
				this->UpdateWindow();
				
				CLauncherDlg* pMainWnd = static_cast<CLauncherDlg*>( theApp.m_pMainWnd );
				pMainWnd->InvalidateRect( NULL, FALSE );
				pMainWnd->UpdateWindow();
			}
			break;
		
		case UI_SELECT_SERVE_BUTTON:
			{
				m_nCurSelectServe = (int)lParam / 10 - 1;
				// 同时向主窗口发送
				LauncherDirBigWorldInfo* pBigWorldInfo	= CLauncherDlg::m_pdirServeSelector->getBigWorld( m_nCurSelectArea );
				LauncherDirWorldInfo&	 pWorldInfo		= pBigWorldInfo->getIthWorld( m_nCurSelectServe ); 
				// 先清空原来被ping的服务器,确保只ping一个服务器
// 				m_pPing->ClearHost();
				//m_strCurSelectIP = string( "http://" ) + string( pWorldInfo.ip );
				// TODO:
				m_strCurSelectIP = "192.168.0.77";
// 				m_pPing->AddPing( 5, m_strCurSelectIP.c_str(), 0 );
// 				m_pPing->Start();

				this->InvalidateRect( NULL, FALSE );
				this->UpdateWindow();

				CLauncherDlg* pMainWnd = static_cast<CLauncherDlg*>( theApp.m_pMainWnd );
				pMainWnd->InvalidateRect( NULL, FALSE );
				pMainWnd->UpdateWindow();

			}
			break;

		case UI_SELECT_CONFIRM_BUTTON:
			{
				CLauncherDlg* pMainWnd = static_cast<CLauncherDlg*>( theApp.m_pMainWnd );
				pMainWnd->SetCurSelectArea( m_nCurSelectArea );
				pMainWnd->SetUrl( CLauncherDlg::m_pdirServeSelector->getBigWorld( 
									m_nCurSelectArea )->m_strUrl.c_str() );
				pMainWnd->SetCurSelectWorld( m_nCurSelectServe );
				pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_CHANGE_SERVE );
				this->EndDialog( IDCLOSE );
			}
			break;

		case UI_SELECT_CLOSE:
			this->EndDialog( IDCLOSE );
			break;

		default:
			assert( !"未定义事件!" );
			break;
		}

		return TRUE;
	}
	else
	{
		return CDialog::OnCommand(wParam, lParam);
	}
}

void CLauncherSelectView::SetCurSelectArea( int nArea )
{
	m_nCurSelectArea = nArea;
}

void CLauncherSelectView::SetCurSelectServe( int nServe )
{
	m_nCurSelectServe = nServe;
}


void CLauncherSelectView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLauncherSelectView, CDialog)
//	ON_WM_PAINT()
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_NCHITTEST()
ON_WM_CLOSE()
END_MESSAGE_MAP()

static char *def_imgBkgnd		= "大区面板底板.bmp";

static char *def_imgBigWorld1	= "公告更新.bmp";
static char *def_mskBigWorld1	= "公告更新mask.bmp";
static char *def_imgBigWorld2	= "公告更新.bmp";
static char *def_mskBigWorld2	= "公告更新mask.bmp";
static char *def_imgBigWorld3	= "公告更新.bmp";
static char *def_mskBigWorld3	= "公告更新mask.bmp";
static char *def_imgBigWorld4	= "公告更新.bmp";
static char *def_mskBigWorld4	= "公告更新mask.bmp";
static char *def_imgConfirmSelect= "确认选择.bmp";

static char *def_imgServe1	= "公告更新.bmp";
static char *def_mskServe1	= "公告更新mask.bmp";
static char *def_imgServe2	= "公告更新.bmp";
static char *def_mskServe2	= "公告更新mask.bmp";
static char *def_imgServe3	= "公告更新.bmp";
static char *def_mskServe3	= "公告更新mask.bmp";
static char *def_imgServe4	= "公告更新.bmp";
static char *def_mskServe4	= "公告更新mask.bmp";
static char *def_imgServe5	= "公告更新.bmp";
static char *def_mskServe5	= "公告更新mask.bmp";
static char *def_imgServe6	= "公告更新.bmp";
static char *def_mskServe6	= "公告更新mask.bmp";
static char *def_imgServe7	= "公告更新.bmp";
static char *def_mskServe7	= "公告更新mask.bmp";
static char *def_imgClose	= "关闭.bmp";

static POINT def_ptBigWorld1     = { 53,   160  };
static POINT def_ptBigWorld2     = { 153,  160  };
static POINT def_ptBigWorld3     = { 253,  160 };
static POINT def_ptBigWorld4     = { 292,  160 };
static POINT def_ptBtnUpdate	 = { 376, 60 };

static POINT def_ptBtnServe1	 = { 53,  282  };
static POINT def_ptBtnServe2	 = { 153, 282  };
static POINT def_ptBtnServe3	 = { 253, 282 };
static POINT def_ptBtnServe4	 = { 352, 282 };
static POINT def_ptBtnServe5	 = { 452, 282 };
static POINT def_ptBtnServe6	 = { 555, 282 };
static POINT def_ptBtnServe7	 = { 654, 282 };
static POINT def_ptClose		 = { 665, 5  };
static POINT def_ptConfirmSelect = { 515, 395  };

static RECT def_rtFontExcellent	= { 30L,   355,  130L,  375L };
static RECT def_rtFontNormal	= { 140L,  355L, 240L,  375L };
static RECT def_rtFontBusy		= { 250L,  355L, 350L,  375L };
static RECT def_rtFontFull		= { 360L,  355L, 460L,  375L };
static RECT def_rtFontNotWork	= { 470L,  355L, 570L,  375L };
static RECT def_rtPingInfo		= { 300L,  290L, 400L,  310L };
static RECT def_rtAreaListTitle	= { 300L,  10L,  500L,  70L };
static RECT def_rtFontSelectArea	= { 90L,   380L,  190L,  395L };
static RECT def_rtFontSelectServe	= { 275L,   380L,  370L,  395L };
static RECT def_rtCurVersionInfo	= { 535L, 37L, 620L,  50L };
static RECT def_rtLatestVersionInfo	= { 640L, 37L, 700L,  50L };


static COLORREF def_crExcellentColor	= RGB( 236,  218, 60 );
static COLORREF def_crNormalColor		= RGB( 236,  218, 60 );
static COLORREF def_crBusyColor			= RGB( 253,  215, 61 );
static COLORREF def_crFullColor			= RGB( 214,  74,  51 );
static COLORREF def_crNotWorkColor		= RGB( 255,  255, 255 );
static COLORREF def_crPingInfoColor		= RGB( 0,    255, 0 );
static COLORREF def_crAreaListTitle		= RGB( 255, 255, 0 );

static COLORREF def_crSelectAreaColor	= RGB( 224, 103, 5 );
static COLORREF def_crSelectServeColor	= RGB( 224, 103, 5 );

extern std::map<int, COLORREF>	stateColor;

typedef struct 
{
	char    imgBkgnd[MAX_PATH];
	
	// 大区
	char    mskBigWorld1[MAX_PATH];
	char    imgBigWorld1[MAX_PATH];
	char    mskBigWorld2[MAX_PATH];
	char    imgBigWorld2[MAX_PATH];
	char    mskBigWorld3[MAX_PATH];
	char    imgBigWorld3[MAX_PATH];
	char    mskBigWorld4[MAX_PATH];
	char    imgBigWorld4[MAX_PATH];

	char    mskServe1[MAX_PATH];
	char    imgServe1[MAX_PATH];
	char    mskServe2[MAX_PATH];
	char    imgServe2[MAX_PATH];
	char    mskServe3[MAX_PATH];
	char    imgServe3[MAX_PATH];
	char    mskServe4[MAX_PATH];
	char    imgServe4[MAX_PATH];
	char    mskServe5[MAX_PATH];
	char    imgServe5[MAX_PATH];
	char    mskServe6[MAX_PATH];
	char    imgServe6[MAX_PATH];
	char    mskServe7[MAX_PATH];
	char    imgServe7[MAX_PATH];
	char    imgClose[MAX_PATH];
	char    imgConfirmSelect[MAX_PATH];

	POINT   ptBigWorld1;
	POINT   ptBigWorld2;
	POINT   ptBigWorld3;
	POINT   ptBigWorld4;

	POINT	ptBtnServe1;
	POINT	ptBtnServe2;
	POINT	ptBtnServe3;
	POINT	ptBtnServe4;
	POINT	ptBtnServe5;
	POINT	ptBtnServe6;
	POINT	ptBtnServe7;
	POINT	ptClose;
	POINT	ptConfirmSelect;	

	RECT		rtFontExcellent;
	COLORREF	crExcellentColor;

	RECT		rtFontNormal;
	COLORREF	crNormalColor;

	RECT		rtFontBusy;
	COLORREF	crBusyColor;

	RECT		rtFontFull;
	COLORREF	crFullColor;

	RECT		rtFontNotWork;	
	COLORREF	crNotWorkColor;

} TSkinConfig;

static bool GetSkinConfig( TSkinConfig& config )
{
	SURFACE_LOADPATH( config, imgBkgnd,		path );
	SURFACE_LOADPATH( config, mskBigWorld1, path );
	SURFACE_LOADPATH( config, imgBigWorld1, path );
	SURFACE_LOADPATH( config, mskBigWorld2, path );
	SURFACE_LOADPATH( config, imgBigWorld2, path );
	SURFACE_LOADPATH( config, mskBigWorld3, path );
	SURFACE_LOADPATH( config, imgBigWorld3, path );
	SURFACE_LOADPATH( config, mskBigWorld4, path );
	SURFACE_LOADPATH( config, imgBigWorld4, path );

	SURFACE_LOADPATH( config, mskServe1, path );
	SURFACE_LOADPATH( config, imgServe1, path );
	SURFACE_LOADPATH( config, mskServe2, path );
	SURFACE_LOADPATH( config, imgServe2, path );
	SURFACE_LOADPATH( config, mskServe3, path );
	SURFACE_LOADPATH( config, imgServe3, path );
	SURFACE_LOADPATH( config, mskServe4, path );
	SURFACE_LOADPATH( config, imgServe4, path );
	SURFACE_LOADPATH( config, mskServe5, path );
	SURFACE_LOADPATH( config, imgServe5, path );
	SURFACE_LOADPATH( config, mskServe6, path );
	SURFACE_LOADPATH( config, imgServe6, path );
	SURFACE_LOADPATH( config, mskServe7, path );
	SURFACE_LOADPATH( config, imgServe7, path );
	SURFACE_LOADPATH( config, imgClose,	 path );
	SURFACE_LOADPATH( config, imgConfirmSelect,	 path );

	SKINXML_LOADPOINT( config, ptBigWorld1, node );
	SKINXML_LOADPOINT( config, ptBigWorld2, node);
	SKINXML_LOADPOINT( config, ptBigWorld3, node );
	SKINXML_LOADPOINT( config, ptBigWorld4, node );

	SKINXML_LOADPOINT( config, ptBtnServe1, node );
	SKINXML_LOADPOINT( config, ptBtnServe2, node );
	SKINXML_LOADPOINT( config, ptBtnServe3, node );
	SKINXML_LOADPOINT( config, ptBtnServe4, node );
	SKINXML_LOADPOINT( config, ptBtnServe5, node );
	SKINXML_LOADPOINT( config, ptBtnServe6, node );
	SKINXML_LOADPOINT( config, ptBtnServe7, node );
	SKINXML_LOADPOINT( config, ptClose,		node );
	SKINXML_LOADPOINT( config, ptConfirmSelect,		node );

	return true;
}

void CLauncherSelectView::SetupGuiStyle()
{
	TSkinConfig config;
	if ( !GetSkinConfig( "./resource", config ) /*!GetSkinConfig( m_szSkinPath, config )*/ )
	{
		char buf[256] = { '\0' };
		sprintf_s( buf, "无法打开/载入界面资源文件: %s/surface.xml", "" );
		ShowError( buf, GetSafeHwnd() );
	}

	//////////////////////////////////////////////////////////////////////////
	// GUI 按钮 surface
	CDC*	pCDC	= GetDC();
	HDC		hdc		= pCDC->GetSafeHdc();

	LOAD_IMAGE( m_imgBkgnd,		config.imgBkgnd );
	LOAD_IMAGE( m_mskBigWorld1,	config.mskBigWorld1 );
	LOAD_IMAGE( m_imgBigWorld1,	config.imgBigWorld1 );
	LOAD_IMAGE( m_mskBigWorld2,	config.mskBigWorld2 );
	LOAD_IMAGE( m_imgBigWorld2,	config.imgBigWorld2 );
	LOAD_IMAGE( m_mskBigWorld3,	config.mskBigWorld3 );
	LOAD_IMAGE( m_imgBigWorld3,	config.imgBigWorld3 );
	LOAD_IMAGE( m_mskBigWorld4,	config.mskBigWorld4 );
	LOAD_IMAGE( m_imgBigWorld4,	config.imgBigWorld4 );
	LOAD_IMAGE( m_mskServe1,	config.mskServe1 );
	LOAD_IMAGE( m_imgServe1,	config.imgServe1 );
	LOAD_IMAGE( m_mskServe2,	config.mskServe2 );
	LOAD_IMAGE( m_imgServe2,	config.imgServe2 );
	LOAD_IMAGE( m_mskServe3,	config.mskServe3 );
	LOAD_IMAGE( m_imgServe3,	config.imgServe3 );
	LOAD_IMAGE( m_mskServe4,	config.mskServe4 );
	LOAD_IMAGE( m_imgServe4,	config.imgServe4 );
	LOAD_IMAGE( m_mskServe5,	config.mskServe5 );
	LOAD_IMAGE( m_imgServe5,	config.imgServe5 );
	LOAD_IMAGE( m_mskServe6,	config.mskServe6 );
	LOAD_IMAGE( m_imgServe6,	config.imgServe6 );
	LOAD_IMAGE( m_mskServe7,	config.mskServe7 );
	LOAD_IMAGE( m_imgServe7,	config.imgServe7 );
	LOAD_IMAGE( m_imgClose,		config.imgClose );
	LOAD_IMAGE( m_imgConfirmSelect,		config.imgConfirmSelect );

	RECT rt;
	GetWindowRect( &rt );
	SIZE* size = m_imgBkgnd.Size();
	// 若没有为按钮设置贴图,则将窗口设置为默认大小
	if ( size->cx == 0 || size->cy == 0 )
	{
		size->cx = rt.right	 - rt.left;
		size->cy = rt.bottom - rt.top;
	}

	m_btnBigWorld1.SetPos( config.ptBigWorld1 );
	m_btnBigWorld1.SetDestDC( hdc );
	m_btnBigWorld1.SetImage( &m_imgBigWorld1 );
	m_btnBigWorld1.SetFontWidth( 10 );
	m_btnBigWorld1.SetClientID( 1 );

	m_btnBigWorld2.SetPos( config.ptBigWorld2 );
	m_btnBigWorld2.SetDestDC( hdc );
	m_btnBigWorld2.SetImage( &m_imgBigWorld2 );
	m_btnBigWorld2.SetFontWidth( 10 );
	m_btnBigWorld2.SetClientID( 2 );

	m_btnBigWorld3.SetPos( config.ptBigWorld3 );
	m_btnBigWorld3.SetDestDC( hdc );
	m_btnBigWorld3.SetImage( &m_imgBigWorld3 );
	m_btnBigWorld3.SetFontWidth( 10 );
	m_btnBigWorld3.SetClientID( 3 );

	m_btnBigWorld4.SetPos( config.ptBigWorld4 );
	m_btnBigWorld4.SetDestDC( hdc );
	m_btnBigWorld4.SetImage( &m_imgBigWorld4 );
	m_btnBigWorld4.SetFontWidth( 10 );
	m_btnBigWorld4.SetClientID( 4 );

	m_btnServe1.SetPos( config.ptBtnServe1 );
	m_btnServe1.SetDestDC( hdc );
	m_btnServe1.SetImage( &m_imgServe1 );

	m_btnServe2.SetPos( config.ptBtnServe2 );
	m_btnServe2.SetDestDC( hdc );
	m_btnServe2.SetImage( &m_imgServe2 );
	
	m_btnServe3.SetPos( config.ptBtnServe3 );
	m_btnServe3.SetDestDC( hdc );
	m_btnServe3.SetImage( &m_imgServe3 );
	
	m_btnServe4.SetPos( config.ptBtnServe4 );
	m_btnServe4.SetDestDC( hdc );
	m_btnServe4.SetImage( &m_imgServe4 );

	m_btnServe5.SetPos( config.ptBtnServe5 );
	m_btnServe5.SetDestDC( hdc );
	m_btnServe5.SetImage( &m_imgServe5 );

	m_btnServe6.SetPos( config.ptBtnServe6 );
	m_btnServe6.SetDestDC( hdc );
	m_btnServe6.SetImage( &m_imgServe6 );

	m_btnServe7.SetPos( config.ptBtnServe7 );
	m_btnServe7.SetDestDC( hdc );
	m_btnServe7.SetImage( &m_imgServe7 );

	m_btnClose.Show();
	m_btnClose.SetPos( config.ptClose );
	m_btnClose.SetDestDC( hdc );
	m_btnClose.SetImage( &m_imgClose );

	m_btnConfirmSelect.Show();
	m_btnConfirmSelect.SetPos( config.ptConfirmSelect );
	m_btnConfirmSelect.SetDestDC( hdc );
	m_btnConfirmSelect.SetImage( &m_imgConfirmSelect );
	//m_btnConfirmSelect.SetCaption( "确认选择" );

	m_btnBigWorld1.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_WORLD_BUTTON + 1 * 10 );
	m_btnBigWorld2.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_WORLD_BUTTON + 2 * 10 );
	m_btnBigWorld3.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_WORLD_BUTTON + 3 * 10 );
	m_btnBigWorld4.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_WORLD_BUTTON + 4 * 10 );

	m_btnServe1.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + 1 * 10 );
	m_btnServe2.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + 2 * 10 );
	m_btnServe3.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + 3 * 10 );
	m_btnServe4.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + 4 * 10 );
	m_btnServe5.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + 5 * 10 );
	m_btnServe6.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + 6 * 10 );
	m_btnServe7.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + 7 * 10 );
	m_btnClose.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_CLOSE );
	m_btnConfirmSelect.SetNotify(	m_hWnd, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_CONFIRM_BUTTON );
}


BOOL CLauncherSelectView::OnInitDialog()
{	
	CDialog::OnInitDialog();
	SetupGuiStyle();
	CRect rect;
	GetParent()->GetWindowRect( &rect );
	MoveWindow( rect.left, rect.top, m_imgBkgnd.Size()->cx, m_imgBkgnd.Size()->cy );
	// 根据配置文件来选择会被自动选择的区
	char szPath[128] = { '\0' };
	GetFilePath( "UserInfoLoginRecord.ini", szPath, sizeof( szPath ) );
	/*m_nCurSelectArea	= ::GetPrivateProfileInt( "Area",		"AreaID",	0,	szPath );
	m_nCurSelectServe	= ::GetPrivateProfileInt( "LoginWorld",	"ID",		0,	szPath );*/

	// 发送默认选中某个大区
	/*this->SendMessage( WM_COMMAND, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_WORLD_BUTTON + ( m_nCurSelectArea + 1 ) * 10 );
	this->PostMessage( WM_COMMAND, CUSTOM_SELECT_BUTTON_MSG, UI_SELECT_SERVE_BUTTON + ( m_nCurSelectServe + 1 ) * 10 );*/

	return TRUE;
}

void CLauncherSelectView::SetPingInfo( const char* pszInfo )
{
	m_strPingInfo = pszInfo;
}


void CLauncherSelectView::OnPaint()
{
	// TODO: Add your message handler code here
	//CWindowDC dc(this);
	CPaintDC dc(this); // device context for painting
	HDC hdc = dc.GetSafeHdc();
	if ( hdc == NULL )
	{
		return;
	}
	
	m_imgBkgnd.Blt( hdc, 0, 0 );
	//m_mskBkgnd.Blt( hdc, 0, 0 );

	// 绘制区域按钮
	int nAreaNum = min( CLauncherDlg::m_pdirServeSelector->getNumBigWorld(), (int)m_vecAreaBtns.size() );
	for ( int i = 0; i < nAreaNum; ++i )
	{
		CClient_Button* pbtn = ( m_vecAreaBtns[i] );
		if ( pbtn->m_hParentWnd != NULL )
		{
			pbtn->SetCaption( ( CLauncherDlg::m_pdirServeSelector->getBigWorld( i ) )->m_Name, RGB( 255, 255, 0 ), 12 );
			pbtn->Show();
			pbtn->OnPaint( hdc );
		}
		// 同时根据它的ping值来调整显示颜色
	}

	for ( size_t i = nAreaNum, total = m_vecAreaBtns.size(); i < total; ++i )
	{
		CClient_Button* pbtn = m_vecAreaBtns[i];
		if ( pbtn->m_hParentWnd )
		{
			pbtn->SetCaption( "" );
			pbtn->Hide();
			pbtn->OnPaint( hdc );
		}
	}
	
	LauncherDirBigWorldInfo* pBigWorldInfo = CLauncherDlg::m_pdirServeSelector->getBigWorld( m_nCurSelectArea );
	int nServeNum = min( pBigWorldInfo->getNumWorld(), (int)m_vecServeBtns.size() );
	for ( size_t i = 0, size = m_vecServeBtns.size(); i < size; ++i )
	{
		m_vecServeBtns[i]->Hide();
		m_vecServeBtns[i]->OnPaint( hdc );
	}

	// 绘制服务器按钮 
	for ( int i = 0 ; i < nServeNum; ++i )
	{
		LauncherDirWorldInfo& worldInfo = pBigWorldInfo->getIthWorld( i );
		m_vecServeBtns[i]->SetCaption( worldInfo.name, RGB( 255, 255, 0 ), 12 );
		m_vecServeBtns[i]->SetTextColor( stateColor[worldInfo.busy] );
		m_vecServeBtns[i]->Show();
		m_vecServeBtns[i]->OnPaint( hdc );
		// 红色表示繁忙,蓝色表示空闲,黄色表示一般
	}

	// 绘制功能性的按钮
	for ( size_t i = 0, size = m_vecFunctionBtns.size(); i < size; ++i )
	{
		m_vecFunctionBtns[i]->OnPaint( hdc );
	}

	// 绘制表示繁忙程度的字体
	/*DrawFont( hdc, def_crExcellentColor,	TEXT( "绿色表示：极佳" ),	def_rtFontExcellent,	12 );
	DrawFont( hdc, def_crNormalColor,		TEXT( "浅色表示：良好" ),	def_rtFontNormal,		12 );
	DrawFont( hdc, def_crBusyColor,			TEXT( "橙色表示：繁忙" ),	def_rtFontBusy,			12 );
	DrawFont( hdc, def_crFullColor,			TEXT( "红色表示：爆满" ),	def_rtFontFull,			12 );
	DrawFont( hdc, def_crNotWorkColor,		TEXT( "灰色表示：维护" ),	def_rtFontNotWork,		12 );
	DrawFont( hdc, def_crPingInfoColor,		m_strPingInfo.c_str(),		def_rtPingInfo,			12 );*/
	//DrawFont( hdc, def_crAreaListTitle,		TEXT( "服务器列表" ),		def_rtAreaListTitle,	20 );

	if ( CLauncherDlg::m_pdirServeSelector->getNumBigWorld() != 0 && m_nCurSelectServe >= 0 && 
			m_nCurSelectArea >= 0 )
	{
		LauncherDirBigWorldInfo* pBigWorldInfo = CLauncherDlg::m_pdirServeSelector->getBigWorld( m_nCurSelectArea );
		string strAreaName		= pBigWorldInfo->m_Name;
		// 将这个button索引转换为各个大区对应的世界索引
		LauncherDirWorldInfo& worldInfo = pBigWorldInfo->getIthWorld( m_nCurSelectServe );
		string strWorldName	= worldInfo.name;
		DrawFont( hdc, def_crSelectAreaColor, TEXT( strAreaName.c_str() ), def_rtFontSelectArea, 14 );

		DrawFont( hdc, stateColor[worldInfo.busy],	TEXT( strWorldName.c_str() ),def_rtFontSelectServe, 14 );
	}
	CLauncherDlg* pdlg = (CLauncherDlg*)theApp.m_pMainWnd;
	DrawFont( hdc, RGB( 255, 255, 255 ), uint_to_version( pdlg->m_pversionVerifier->getLocalVersion() ),	
		def_rtCurVersionInfo, 10 );
	DrawFont( hdc, RGB( 255, 255, 255 ), uint_to_version( pdlg->m_pversionVerifier->getLatestVersionInfo().m_nVersionTo ),	
		def_rtLatestVersionInfo, 10 );
	CDialog::OnPaint();
}

void CLauncherSelectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// 先让所有的按钮check状态去除
	CClient_Button* pbtn	= NULL;
	for ( LstImageButtons::iterator iter = m_vecAreaBtns.begin(), end = m_vecAreaBtns.end(); iter != end; ++iter )
	{
		(*iter)->DisChecked();
	}

	for ( LstImageButtons::iterator iter = m_vecServeBtns.begin(), end = m_vecServeBtns.end(); iter != end; ++iter )
	{
		(*iter)->DisChecked();
	}

	for ( int i = 0, size = (int)m_vecServeBtns.size(); i < size; ++i )
	{
		pbtn = m_vecServeBtns[i];
		if ( pbtn->OnLButtonDown( point.x, point.y ) )
		{
			pbtn->Checked();
			m_nCurSelectServe = i;
			return;
		}
	}

	for ( int i = 0, size = (int)m_vecAreaBtns.size(); i < size; ++i )
	{
		pbtn = m_vecAreaBtns[i];
		if ( pbtn->OnLButtonDown( point.x, point.y ) )
		{
			pbtn->Checked();
			m_nCurSelectArea = i;
			return;
		}
	}

	for ( size_t i = 0, size = m_vecFunctionBtns.size(); i < size; ++i )
	{
		pbtn = m_vecFunctionBtns[i];
		if ( pbtn->OnLButtonDown( point.x, point.y ) )
		{
			return;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CLauncherSelectView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	for ( LstImageButtons::iterator iter = m_vecServeBtns.begin(), end = m_vecServeBtns.end(); iter != end; ++iter )
	{
		if ( ( *iter )->OnLButtonUp( point.x, point.y ) )
		{
			return;
		}
	}

	for ( LstImageButtons::iterator iter = m_vecAreaBtns.begin(), end = m_vecAreaBtns.end(); iter != end; ++iter )
	{
		if ( ( *iter )->OnLButtonUp( point.x, point.y ) )
		{
			return;
		}
	}
	
	for ( LstImageButtons::iterator iter = m_vecFunctionBtns.begin(), end = m_vecFunctionBtns.end(); iter != end; ++iter )
	{
		if ( ( *iter )->OnLButtonUp( point.x, point.y ) )
		{
			return;
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CLauncherSelectView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	for ( LstImageButtons::iterator iter = m_vecServeBtns.begin(), end = m_vecServeBtns.end(); iter != end; ++iter )
	{
		if ( ( *iter )->OnMouseMove( point.x, point.y ) )
		{
			return;
		}
	}

	for ( LstImageButtons::iterator iter = m_vecAreaBtns.begin(), end = m_vecAreaBtns.end(); iter != end; ++iter )
	{
		if ( ( *iter )->OnMouseMove( point.x, point.y ) )
		{
			return;
		}
	}

	for ( LstImageButtons::iterator iter = m_vecFunctionBtns.begin(), end = m_vecFunctionBtns.end(); iter != end; ++iter )
	{
		if ( ( *iter )->OnMouseMove( point.x, point.y ) )
		{
			return;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CLauncherSelectView::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	RECT rtClient;
	//POINT topleftPoint;
	this->GetClientRect( &rtClient );
	POINT ptClientPoint = point;
	this->ScreenToClient( &ptClientPoint );

	// 检测点是否在客户区,若不在客户区,则使用默认处理方式
	if ( ptClientPoint.x <0 || ptClientPoint.y < 0 )
	{
		return CDialog::OnNcHitTest( point );
	}

	CPoint pt( ptClientPoint.x - rtClient.left, ptClientPoint.y - rtClient.top );
	
	// 当点击区域在某个按钮上,则按照默认鼠标点击测试方式来处理
	for ( LstImageButtons::iterator iter = m_vecAreaBtns.begin(), end = m_vecAreaBtns.end(); iter != end; ++iter )
	{
		CClient_Button *pbtn = ( *iter );
		if ( !pbtn->IsShown() || pbtn->GetClientID() == ( 1 + m_nCurSelectArea ) )
		{
			continue;
		}

		pbtn->Rest( pt );

		if ( pbtn->IsPointIn( pt.x, pt.y ) )
		{
			return CDialog::OnNcHitTest( point );
		}
	}

	for ( LstImageButtons::iterator iter = m_vecServeBtns.begin(), end = m_vecServeBtns.end(); iter != end; ++iter )
	{
		CClient_Button *pbtn = ( *iter );
		if ( !pbtn->IsShown() )
		{
			continue;
		}

		pbtn->Rest( pt );

		if ( pbtn->IsPointIn( pt.x, pt.y ) )
		{
			return CDialog::OnNcHitTest( point );
		}
	}

	for ( LstImageButtons::iterator iter = m_vecFunctionBtns.begin(), end = m_vecFunctionBtns.end(); iter != end; ++iter )
	{
		CClient_Button *pbtn = ( *iter );
		if ( !pbtn->IsShown() )
		{
			continue;
		}

		pbtn->Rest( pt );

		if ( pbtn->IsPointIn( pt.x, pt.y ) )
		{
			return CDialog::OnNcHitTest( point );
		}
	}
	
	return HTCLIENT;
	//return HTCAPTION;
}

void CLauncherSelectView::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
