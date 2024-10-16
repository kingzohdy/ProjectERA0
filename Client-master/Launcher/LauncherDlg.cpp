// LauncherDlg.cpp : implementation file
//
#include "stdafx.h"
#include <utility>
#include <stdlib.h>
#include <queue>

#include "Launcher.h"
#include "LauncherDlg.h"
#include "VersionVerifier.h"
#include "DirServeSelector.h"
#include "util/utils.h"
#include "GraphSettingLayerDlg.h"
#include "cs_net.h"
//#include "CIniFile.h"
#include "GamePackageFile.h"
#include "OgrePackageFile.h"
#include <Psapi.h>
#include <math.h>
#include "XMLData.h"
#include "des.h"
#include <MsHTML.h>
#include <mshtmhst.h>
#include <mmsystem.h>
#include <Wininet.h>
#include "CIniFile.h"
#include "OgreStringUtil.h"

using namespace Launcher;

using namespace std;

#define MB (1024*1024)
#define KB 1024

#define IDC_SELFDOWNLOAD	1001
#define IDC_UPDTAE			1002
#define IDC_ENTERGAME		1003
#define IDC_RESETEFFECT		1004
#define IDC_REGISTER		1005
#define IDC_UPDATELOG		1006
#define IDC_GUIDER			1007
#define IDC_CUSTOMERSERVICE 1008
#define IDC_OFFICEBBS		1009
#define IDC_OFFICESITE		1010
#define IDC_CANCELUPDATE	1011
#define IDC_CLOSE			1012
#define IDC_MINIMIZE		1013
#define IDC_EXPSERVER		1014
#define IDC_NORMALSERVER	1015
#define IDC_ACTIVE			1016

const int ACP_TYPE = 936;
static char *def_imgDownloadProgress= "resource/JinDuTiao.bmp";
static char *def_imgUpdateProgress	= "resource/JinDuTiao.bmp";

static POINT def_ptBtnUpdate    = { 812, 539 };
static POINT def_ptSelectArea   = { 48, 465 };
static POINT def_ptEnterGame	= { 812, 539 };
static POINT def_ptCancelUpdate	= { 812, 539 };
static POINT def_ptSelfDownload	= { 689, 597 };
static POINT def_ptResetEffect  = { 998, 250 };

static POINT def_ptOfficeSite		= { 652, 284 };
static POINT def_ptRegister			= { 464, 284 };
static POINT def_ptUpdateLog		= { 689, 557 };
static POINT def_ptGuider			= { 746, 284 };
static POINT def_ptOfficebbs		= { 934, 284 };
static POINT def_ptCustomerService	= { 840, 284 };
static POINT def_ptClose			= { 961, 250 };
static POINT def_ptMinimize			= { 924, 250 };
static POINT def_ptExpServer		= { 806, 512 };
static POINT def_ptNormalServer		= { 806, 512 };
static POINT def_ptActive			= { 558, 284 };

static POINT def_ptDownloadProgress	= { 411, 569 };
static RECT	def_rtProgressPos		= { 411,569,687,580 };

static RECT def_rtCurVersionInfo	= { 447, 594, 558, 605 };
static RECT def_rtLatestVersionInfo	= { 575, 594, 686, 605 };
static RECT def_rtWaitingInfo		= { 413, 529, 687, 542 };

static RECT def_rtDownloadInfo		= { 413,  545, 687, 558 };
static COLORREF def_crDownloadInfo	= RGB( 208,214,218 );

static RECT def_rtFontSelectArea	= { 90L,   380L,  190L,  395L };
static RECT def_rtFontSelectServe	= { 275L,   380L,  370L,  395L };

static COLORREF def_crSelectAreaColor	= RGB( 224, 103, 5 );
static COLORREF def_crSelectServeColor	= RGB( 224, 103, 5 );

static RECT def_rtSpeedBegin			= {413,513,493,535};
static RECT def_rtSpeed					= {473,513,543,535};
static RECT def_rtTimeLeftBegin			= {543,513,603,535};
static RECT def_trTime					= {603,513,673,535};
static RECT def_rtFileSizeBegin			= {413,535,473,557};
static RECT def_rtFileSize				= {473,535,573,557};

const string CLIENT_CFG_PATH	= "serverlist.data";
const string CLIENT_CFG_PATH2	= "client.cfg";
const string CLIENT_EXP_CFG_PATH = "serverlist_tmp.data";
const string CLIENT_EXP_SERVER_CONFIG = "expconfig.data";

#define ATTACH_DOWNLOAD_STEP 6

BOOL Kill(char* pName)//杀进程函数
{
	DWORD aProcesses[1024],dwSize,dwSize2;    
	char szProcessName[MAX_PATH]={0};
	DWORD Pid=0;
	unsigned int i;    
	if( !EnumProcesses(aProcesses,sizeof(aProcesses),&dwSize) )
	{ 
		return FALSE;    
	}
	dwSize2=dwSize/sizeof(DWORD);
	for( i=0;i<dwSize2;i++ )
	{    
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,aProcesses[i]);     

		if( hProcess )
		{    
			HMODULE hMod;    
			DWORD dwSize;    
			if( EnumProcessModules(hProcess,&hMod,sizeof(hMod),&dwSize) )
			{
				GetModuleBaseName(hProcess,hMod,szProcessName,sizeof(szProcessName));
			}
			if( strcmp(szProcessName,pName)==0 )
			{ 
				Pid=aProcesses[i];
				CloseHandle(hProcess);   
				break;
			}
		}
		CloseHandle(hProcess);    
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,Pid);
	if(NULL!=hProcess)       
	{ 
		TerminateProcess(hProcess,0); 
	} 
	CloseHandle(hProcess);   
	return TRUE;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const string VERSION_PATH	= "version.data";

const int MAX_CONNECT_TIMES = 3;
const int NONE_SELECT  = -1;
static DWORD s_nDownloadThread = 0;
const int DisplayChars = 21;


// 除了一下目录的外其他全部解压
const int MAXUNCOMPATH = 11;
static string strUnCompress[MAXUNCOMPATH] = {
	"character\\",
	"db\\",
	"effect\\",
	"maps\\",
	"scene\\",
	"script\\",
	"shaders\\",
	"texture\\",
	"toolres\\",
	"uires\\",
	"sounds\\"
};

bool isInUnCompressPath( string& str )
{
	for( int i = 0; i< MAXUNCOMPATH; i++ )
	{
		if( str.find( strUnCompress[i].c_str() ) == 0 )
		{
			return true;
		}
	}

	return false;
}

DWORD WINAPI ThreadDownloadPatch( LPVOID param )
{
	CLauncherDlg* pdlg = ( CLauncherDlg* ) param;

	const tagVersionInfo& curVersionInfo = pdlg->m_pversionVerifier->getLatestVersionInfo();
	// 当更新完毕后
	if ( pdlg->DownloadPatch( curVersionInfo.m_strUpAddress.c_str(), curVersionInfo.m_strMD5.c_str() ) )
	{
		// 当下载完一个更新包,将更新包安装到exe上后,然后再验证
		pdlg->ApplyPatch( pdlg->GetPatchFilePath() );
		// 再次验证版本信息
		pdlg->PostMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_RECHECK_VERSION );
	}else if( pdlg->GetDownload()->GetResult() == CURLE_OK )
	{
		//md5 错误，不重新下载
		pdlg->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_ERROR_UPDATE );
	}else{
		//::MessageBoxW( pdlg->GetSafeHwnd(), L"更新失败", L"提示", MB_OK );
		pdlg->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_BEGIN_DOWNLOAD );
	}

	return 0;
}

DWORD WINAPI ThreadDownloadAttach( LPVOID param )
{
	CLauncherDlg* pdlg = ( CLauncherDlg* ) param;

	const tagVersionInfo& curVersionInfo = pdlg->m_pversionVerifier->getLatestVersionInfo();
	char szPath[128] = { '\0' };
	GetFilePath( "patch", szPath, sizeof( szPath ) );
	pdlg->SetUrl( pdlg->m_SmallClientUrl );
	pdlg->SetLocalPath( szPath );
	pdlg->PostMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_DOWNLOAD_ATTACH );
	// 当更新完毕后
	if ( pdlg->DownloadPatch("attach.pkg", pdlg->m_SmallClientAttachMD5) )
	{
		// 移动过去
		string	path( szPath );
		string	fileName("attach.pkg");
		string postFileName;
		string::size_type pos = fileName.find_last_of( "/\\");
		if ( pos != string::npos )
		{
			postFileName = fileName.substr( pos );
		}
		else
		{
			postFileName = string( "\\" )+ fileName;
		}

		path = string( szPath ) + postFileName;
		if (MoveFile(path.c_str(), "data\\attach.pkg"))
		{
			DeleteFile("data\\protocol.pkg");
		}
		pdlg->SendMessage( WM_PATCH, CbUpdateDone, 0 );
		// 再次验证版本信息
		pdlg->PostMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_LATEST_VERSION );
	}
	else
	{
// 		::MessageBoxW( pdlg->GetSafeHwnd(), L"更新失败", L"提示", MB_OK );
		pdlg->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_BEGIN_DOWNLOAD );
	}

	return 0;
}

// CLauncherDlg dialog
DirServeSelector*		CLauncherDlg::m_pdirServeSelector	= NULL;
VersionVerifier*		CLauncherDlg::m_pversionVerifier	= NULL;
unsigned char*			CLauncherDlg::m_pCSMetaBuf			= NULL;

GameNetCfg		CLauncherDlg::m_NetCfg;

CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLauncherDlg::IDD, pParent),  m_pDonwloader( NULL  ),
m_bDownloading( false ), m_bUpdatingPatch( false ), m_bDownloadAttach(false),m_totalLength( 0 ),m_bNeedUpdateSelf(false),m_bConnectDirServer(false), m_SmallClient(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pdirServeSelector	= new DirServeSelector;
	m_pversionVerifier	= new VersionVerifier;

	m_szPatchFile[0] = 0;
	m_tmDlNotify = 0;
	m_ExpServerCfg.expFlag = 0;
	m_ExpServerCfg.path = "";
	m_TotalCopySize = 0;
	m_CopyedSize = 0;
	m_bIsCopyFile = false;
}

CLauncherDlg::~CLauncherDlg()
{
	SafeDelete( m_pdirServeSelector );
	SafeDelete( m_pversionVerifier );
	SafeDelete( m_pDonwloader );
	if( m_hInstMutex )
	{
		CloseHandle( m_hInstMutex );
		m_hInstMutex = NULL;
	}
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_ctrlWeb);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_DOWNLOADER,	&CLauncherDlg::OnSetDownloadStatus)		
	ON_MESSAGE(WM_PATCH,		&CLauncherDlg::OnSetPatchStatus)
	ON_WM_SYSCOMMAND()
	//	ON_WM_DRAWITEM()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_SELFDOWNLOAD,&CLauncherDlg::OnSelfDownloadClick)
	ON_BN_CLICKED(IDC_UPDTAE,&CLauncherDlg::OnUpdateClick)
	ON_BN_CLICKED(IDC_ENTERGAME,&CLauncherDlg::OnEnterGameClick)
	ON_BN_CLICKED(IDC_RESETEFFECT,&CLauncherDlg::OnResetEffectClick)
	ON_BN_CLICKED(IDC_REGISTER,&CLauncherDlg::OnRegisterClick)
	ON_BN_CLICKED(IDC_UPDATELOG,&CLauncherDlg::OnUpdateLogClick)
	ON_BN_CLICKED(IDC_GUIDER,&CLauncherDlg::OnGuildClick)
	ON_BN_CLICKED(IDC_CUSTOMERSERVICE,&CLauncherDlg::OnCustomerServiceClick)
	ON_BN_CLICKED(IDC_OFFICEBBS,&CLauncherDlg::OnOfficeBbsClick)
	ON_BN_CLICKED(IDC_OFFICESITE,&CLauncherDlg::OnOfficeSiteClick)
	ON_BN_CLICKED(IDC_CANCELUPDATE,&CLauncherDlg::onCanelUpdateClick)
	ON_BN_CLICKED(IDC_CLOSE,&CLauncherDlg::onCloseClick)
	ON_BN_CLICKED(IDC_MINIMIZE,&CLauncherDlg::onMinimizeClick)
	ON_BN_CLICKED(IDC_EXPSERVER,&CLauncherDlg::onExpServerClick)
	ON_BN_CLICKED(IDC_NORMALSERVER,&CLauncherDlg::onNormalServerClick)
	ON_BN_CLICKED(IDC_ACTIVE,&CLauncherDlg::onActiveClick)
END_MESSAGE_MAP()


// CLauncherDlg message handlers

BOOL CLauncherDlg::sendDumpFile()
{
// 	//没有配置 ftp
// 	if( m_FtpConfig.strFtpServer == "" )
// 	{
// 		return FALSE;
// 	}
// 	//打包dump
// 	CFileFind finder;
// 	BOOL bWorking = finder.FindFile("log\\*.dmp");
// 	if( bWorking )
// 	{
// 		//开启上传工具
// 		::ShellExecute( NULL, "open", "BugReport.exe", "launcher", NULL, SW_HIDE );
// 	}
	return TRUE;
}

bool readBuffer( char* pHashFileName, const char* filename )
{
	if( CLauncherDlg::m_pCSMetaBuf != NULL )
	{
		delete CLauncherDlg::m_pCSMetaBuf;
		CLauncherDlg::m_pCSMetaBuf = NULL;
	}
	char buffer[256];
	ValidateFileName(buffer, 256, pHashFileName );

	unsigned int key1 = StringUtil::hash(buffer, 1);
	unsigned int key2 = StringUtil::hash(buffer, 2);

	GameFilePackage desPackage;
	if ( !desPackage.open( filename, true ) )
	{
		return false;
	}

	for ( size_t i = 0, nFileNum = desPackage.getNumFiles(); i < nFileNum; ++i )
	{
		const GamePkgFileInfo& fileInfo = desPackage.getFileInfo( i );

		if ( key1 == fileInfo.key1 && key2 == fileInfo.key2 )
		{
			auto_ptr<DataStream> pStream( desPackage.openFile( (int)i ) );

			if ( pStream.get() != NULL )
			{
				long	len		= fileInfo.originsize;
				CLauncherDlg::m_pCSMetaBuf	= new unsigned char[len];
				pStream->read( CLauncherDlg::m_pCSMetaBuf, len );
				re_confuse( CLauncherDlg::m_pCSMetaBuf, len );
				desPackage.close();
				return true;
			}
		}
	}
	desPackage.close();

	return false;
}

void LoadMetaFile()
{
	StringUtil::init();
	if( readBuffer( "db\\proto_cs.meta","data\\update.pkg" ) )
	{
		return;
	}
	if( readBuffer( "db\\proto_cs.meta","data\\attach.pkg" ) )
	{
		return;
	}
	if( readBuffer( "db\\proto_cs.meta","data\\protocol.pkg" ) )
	{
		return;
	}
	if( readBuffer( "db\\proto_cs.meta", "data\\common.pkg" ) )
	{
		return;
	}
	FILE* pfile = fopen("db\\proto_cs.meta","rb");
	if( pfile )
	{
		fseek(pfile, 0, SEEK_END);
		size_t fsize = ftell(pfile);
		fseek(pfile, 0, SEEK_SET);
		CLauncherDlg::m_pCSMetaBuf = (unsigned char *)malloc(fsize);
		fread(CLauncherDlg::m_pCSMetaBuf, 1,fsize,pfile);		
		re_confuse(CLauncherDlg::m_pCSMetaBuf, (int)fsize);
		fclose(pfile);
	}	
}

BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	ModifyStyleEx( 0, WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB(255,0,255), 255, LWA_ALPHA | LWA_COLORKEY );
	// 加载控件的贴图
	InitGuiStyle();
	m_ctrlWeb.SetWindowPos( NULL, 0,0, 0,0, SWP_NOZORDER );
	//m_ctrlWeb2.SetWindowPos( NULL, 0,0, 0,0, SWP_NOZORDER );
	// 查看是否需要自更新
	CString updateCmdLine( theApp.m_lpCmdLine );
	if( updateCmdLine.CompareNoCase("-updated") == 0 )
	{
		m_bUpdated = true;
	}else{
		m_bUpdated = false;
	}
	if ( updateCmdLine.CompareNoCase("-helper") == 0 )
	{
		SelfUpdateLauncher();
		exit(0);
		return TRUE;
	}
	else
	{
		//在删除helper文件前，先停止helper进程。
		Kill(HELPER_UPDATE_LAUNCHER_NAME);
		::DeleteFile(HELPER_UPDATE_LAUNCHER_NAME);
	}
	Kill(SMART_DOWNLOADER_NAME);
	CheckSingleInstance();

	// TODO: 获取更新补丁的URL
	strcpy_s( m_szSfxPatchUrl, "http://www.ljy0.com/download/" );
	m_strLauncherUrl = "http://www.ljy0.com/new_launcher_2013_5/";
	m_strHomePageUrl = "http://www.ljy0.com";
	m_strRegisterUrl = "http://passport.wanyouyl.com/register.php";

	if( !loadLocalCfg() )
	{
		return TRUE;
	}
	m_nLastIndex = 0;

	char szPath[128] = { '\0' };	

	// 当version serve连接成功后,初始化下载器
	m_pDonwloader	= new CDownloader( this->GetSafeHwnd(), WM_DOWNLOADER );

	::BringWindowToTop(this->m_hWnd);	

	LoadMetaFile();

	this->SetTimer( UI_NET_CONNECT,100,NULL );
	if ( GetACP() == ACP_TYPE )
	{
		::SetWindowText( GetSafeHwnd(), "零纪元登录程序" );
	}
	else
	{
		::SetWindowText( GetSafeHwnd(), "Era Zero Launcher" );
	}

	XMLData data;
	if (data.loadFile(CLIENT_CFG_PATH2))
	{
		XMLNode	root = data.getRootNode();
		if (root.hasChild("SmartClient"))
		{
			XMLNode node	= root.getChild("SmartClient");
			m_SmallClient	= node.attribToBool("flag");
		}
		if( root.hasChild("HomePageUrl") )
		{
			XMLNode node = root.getChild("HomePageUrl");
			m_strHomePageUrl = node.attribToString( "url" );
		}
		if( root.hasChild( "RegisterUrl" ) )
		{
			XMLNode node = root.getChild( "RegisterUrl" );
			m_strRegisterUrl = node.attribToString( "url" );
		}
	}
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLauncherDlg::OnPaint()
{
	CPaintDC dc( this );
	HDC hdc = dc.GetSafeHdc();

	// 绘制当前版本号
	CBrush	brush( RGB(255,0,255) );
	dc.FillRect( &def_rtCurVersionInfo, &brush );
	char szVer[100];
	sprintf( szVer, "当前版本：%s", uint_to_version( m_pversionVerifier->getLocalVersion() ) );
	DrawFont( hdc, RGB( 208,214,218 ), szVer, def_rtCurVersionInfo, 12 );
	// 绘制最新版本号
	dc.FillRect( &def_rtLatestVersionInfo, &brush );
	if( m_pversionVerifier->getLatestVersionInfo().m_nVersionLatest == 0 )
	{
		DrawFont( hdc, RGB( 208,214,218 ), "最新版本：无法获得版本信息",	
			def_rtLatestVersionInfo, 12 );

	}else{
		sprintf( szVer, "最新版本：%s", uint_to_version( m_pversionVerifier->getLatestVersionInfo().m_nVersionLatest ) );
		DrawFont( hdc, RGB( 208,214,218 ), szVer, def_rtLatestVersionInfo, 12 );
	}

	if( !m_bDownloadAttach )
	{
		if( !m_bIsCopyFile )
		{
			dc.FillRect(&def_rtWaitingInfo,&brush);	
			if( m_bDownloading || m_bUpdatingPatch )
			{
				DrawFont(hdc,RGB( 208,214,218 ),"下载信息：正在更新最新版本中...请稍候",def_rtWaitingInfo,12);
			}
			else if( m_bConnectDirServer )
			{
				DrawFont(hdc,RGB( 208,214,218 ),"下载信息：正在获取下载地址...请稍候",def_rtWaitingInfo,12);
			}
		}else{
			DrawFont(hdc,RGB( 208,214,218 ),"下载信息：正在拷贝客户端文件...请稍候",def_rtWaitingInfo,12);
		}
	}else{
		dc.FillRect(&def_rtWaitingInfo,&brush);
		char buf[256];
		sprintf( buf,"下载信息：正在下载核心包（%d/%d）请稍候", m_nLastIndex, ATTACH_DOWNLOAD_STEP );
		DrawFont(hdc,RGB( 208,214,218 ),buf,def_rtWaitingInfo,12);
	}
	CDialog::OnPaint( );
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLauncherDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_CLOSE );
	exit(0);
}

void CLauncherDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if ( nIDEvent == UI_NET_VERSION_DISPATCH )
	{
		m_pversionVerifier->OnTime();
	}else if( nIDEvent == UI_NET_DIR_DISPATCH )
	{
		m_pdirServeSelector->onTime();
	}else if( nIDEvent == UI_NET_CONNECT )
	{
		this->KillTimer(nIDEvent);
		ConnectVersionServe();
		m_ctrlWeb.Navigate( "about:blank", NULL, NULL, NULL, NULL );
		//m_ctrlWeb2.Navigate( "about:blank", NULL, NULL, NULL, NULL );
	}
	CDialog::OnTimer(nIDEvent);
}


void CLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnSysCommand(nID, lParam);
}
LRESULT CLauncherDlg::OnSetDownloadStatus( WPARAM wParam, LPARAM lParam )
{
	CClientDC dc( this );
	CRect rect( def_rtDownloadInfo );
	HDC hdc = dc.GetSafeHdc();

	switch( wParam )
	{
	case CDownloader::CbDownloadTotal:
		{
			m_totalLength = (unsigned int)lParam;
		}
		break;

	case CDownloader::CbBreakDownload:	
		break;

	case CDownloader::CbStartupDownload:
		{
			m_bDownloading = true;
			m_tmDlStartup	= GetTickCount();
			m_tmDlNotify    = GetTickCount();
			m_nDlBytes      = 0;
		}
		break;
	case CDownloader::CbEndDownload:
		{	
			m_bDownloading = false;
			m_bDownloadAttach = false;
			m_tmDlStartup	= GetTickCount();
			m_tmDlNotify    = GetTickCount();
			m_nDlBytes      = 0;

			CBrush	brush( RGB(255,0,255) );
			dc.FillRect(&def_rtSpeedBegin,&brush);
			dc.FillRect(&def_rtSpeed,&brush);
			dc.FillRect(&def_rtTimeLeftBegin,&brush);
			dc.FillRect(&def_trTime,&brush);
			dc.FillRect(&def_rtFileSizeBegin,&brush);
			dc.FillRect(&def_rtFileSize,&brush);

		}
		break;

	case CDownloader::CbDownloadProgress:
		{
			if( m_bUpdatingPatch )
			{
				break;
			}
			time_t passtime = GetTickCount() - m_tmDlNotify + 1;

			CBrush	brush( RGB(255,0,255) );
			char buf[128] = { '\0' };
			m_nDlBytes      = (unsigned int) lParam;
			int nProgress = 0;
			if( m_totalLength != 0 )
			{
				if( m_bDownloadAttach )
				{
					int nIndex = ( 100 * m_nDlBytes * ATTACH_DOWNLOAD_STEP /m_totalLength ) / 100 + 1;
					nProgress = ( 100 * m_nDlBytes * ATTACH_DOWNLOAD_STEP /m_totalLength ) % 100;
					if( m_nLastIndex != nIndex && m_bDownloading)
					{
						m_nLastIndex = nIndex;
						dc.FillRect(&def_rtWaitingInfo,&brush);	
						char buf[256];
						sprintf( buf,"正在下载核心包（%d/%d）请稍候", nIndex, ATTACH_DOWNLOAD_STEP );
						DrawFont(hdc,RGB( 208,214,218 ),buf,def_rtWaitingInfo,12);			
					}
				}else{
					nProgress = (int)ceil( ( double )( 100 * m_nDlBytes/m_totalLength) );
				}
			}
			if( passtime > 100 )
			{
				CBrush	brush( RGB(255,0,255) );
				dc.FillRect(&def_rtSpeedBegin,&brush);
				DrawFont(hdc,RGB( 208,214,218 ),"下载速度：",def_rtSpeedBegin,12);
				dc.FillRect(&def_rtTimeLeftBegin,&brush);
				if( !m_bDownloadAttach )
				{
					DrawFont(hdc,RGB( 208,214,218 ),"预计时间：",def_rtTimeLeftBegin,12);
				}

// 				long bytes	= static_cast<unsigned int>( lParam ) - m_nDlBytes;
// 				float speed = ( static_cast<float>( bytes ) ) / passtime * 1000.0f * 1024.0f ;
				double speed = m_pDonwloader->getDownloadSpeed();
				if( speed > MB )
				{
					sprintf_s(buf,"%.1fMB/S",speed/MB);
				}else if( speed > KB )
				{
					sprintf_s(buf,"%.1fKB/S",speed/KB);
				}else{
					sprintf_s(buf,"%.1fB/S",speed);
				}
				dc.FillRect(&def_rtSpeed,&brush);
				DrawFont(hdc,RGB( 208,214,218 ),buf,def_rtSpeed,12);

				time_t nRemainedTime = 0;
				if( speed > 0 )
				{
					nRemainedTime = (time_t)((m_totalLength - lParam)*KB/speed);
				}
				if( nRemainedTime >= 0 )
				{
					if( nRemainedTime <= 60 )
					{
						sprintf_s(buf,"< 1分钟");
					}else
					{
						int nHour = nRemainedTime/(60*60);
						int nMin = (nRemainedTime/60)%60;
						if( nHour > 0 )
						{
							sprintf_s(buf,"%d小时%d分钟",nHour,nMin);
						}else{
							sprintf_s(buf,"%d分钟",nMin);
						}
					}
				}
				dc.FillRect(&def_trTime,&brush);
				if( !m_bDownloadAttach )
				{
					DrawFont(hdc,RGB( 208,214,218 ),buf,def_trTime,12);
				}				

				m_tmDlNotify    = GetTickCount();
				dc.FillRect( rect, &brush );
				dc.FillRect(&def_rtProgressPos,&brush);
				m_downloadBarProgress.SetPercent( (BYTE)nProgress );
				sprintf_s( buf, "下载进度：%d%%", nProgress );
				DrawFont( hdc, def_crDownloadInfo, buf, def_rtDownloadInfo, 12 );

				dc.FillRect(&def_rtFileSizeBegin,&brush);
				if( !m_bDownloadAttach )
				{
					DrawFont(hdc,RGB( 208,214,218 ),"文件大小：",def_rtFileSizeBegin,12);
				}

				float downLoaded = (float)m_nDlBytes/KB;
				float total = (float)m_totalLength/KB;
				sprintf_s(buf,"%.2fMB/%.2fMB",downLoaded,total);
				dc.FillRect(&def_rtFileSize,&brush);
				if( !m_bDownloadAttach )
				{
					DrawFont(hdc,RGB( 208,214,218 ),buf,def_rtFileSize,12);
				}
			}
		}
		break;

	case CDownloader::CbDownloadDiskFull:
		{
			ShowError( L"下载补丁文件失败，磁盘空间已满！请清理磁盘文件后再更新游戏！", GetSafeHwnd( ) );
		}
		break;

	default:
		assert( !"未定义消息事件!" );
		break;

	}
	return 0;
}

LRESULT	CLauncherDlg::OnSetPatchStatus(WPARAM wParam, LPARAM lParam)
{
	switch( wParam )
	{
	case CbUpdateStartup:
	case CbUpdateDone:
		{
			m_bUpdatingPatch = false;
			CClientDC dc( this );
			CBrush	brush( RGB(255,0,255) );
			dc.FillRect( &def_rtProgressPos, &brush );
			dc.FillRect( &def_rtDownloadInfo, &brush );
			m_downloadBarProgress.SetPercent( 0 );
			m_updateBarProgress.SetPercent( 0 );
		}
		break;

	case CbUpdateProgress:
		{
			char buf[50] = { '\0' };
			sprintf_s( buf, "更新进度：%d%%", lParam );
			CClientDC dc( this );
			CRect rect( def_rtDownloadInfo );
			HDC hdc = dc.GetSafeHdc( );
			CBrush	brush( RGB(255,0,255) );
			dc.FillRect( &def_rtProgressPos, &brush );
			dc.FillRect( rect, &brush );
			m_downloadBarProgress.SetPercent( 0 );
			m_updateBarProgress.SetPercent( (BYTE)lParam );
			DrawFont( hdc, def_crDownloadInfo, buf , def_rtDownloadInfo, 12 );
		}
		break;

	case CbUpdateDiskFull:
		{
			CClientDC dc( this );
			CBrush	brush( RGB(255,0,255) );
			dc.FillRect( &def_rtProgressPos, &brush );
			m_updateBarProgress.SetPercent( 0 );
			ShowError( L"更新补丁文件失败，磁盘空间已满！请清理磁盘文件后再更新游戏！", GetSafeHwnd( ) );
		}
		break;

	default:
		assert( !"未定义消息事件!" );
		break;
	}
	return 0;
}

BOOL CLauncherDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )   
	{
		this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_CLOSE );
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);   
}

BOOL CLauncherDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CPaintDC dc( this );
	HDC hdc = dc.GetSafeHdc();
	RECT rect;
	this->GetClientRect( &rect );

	if ( wParam == CUSTOM_BUTTON_MSG )
	{
		switch ( lParam )
		{
		case UI_COMMAND_CONNECT_DISERVE_SUCC:
			{
				m_bConnectDirServer = false;
 				m_bDownloading = true;
				// 使用下载速度最快的url
 				SetUrl( m_pdirServeSelector->getBestUrl() );
 				assert( m_pDonwloader != NULL );
 
 				char szPath[128] = { '\0' };
 				GetFilePath( "patch", szPath, sizeof( szPath ) );
 				m_pDonwloader->SetLocalPath( szPath );
 				m_btnCancelUpdate.ShowWindow(SW_SHOW);
 				string strFileName = m_pversionVerifier->getLatestVersionInfo().m_strUpAddress;
 				string postFileName;
 				string::size_type pos = strFileName.find_last_of( "/\\");
 				if ( pos != string::npos )
 				{
 					postFileName = strFileName.substr( pos );
 				}else{
 					postFileName = string( "\\" ) + strFileName;
 				}
 				strFileName = string( szPath ) + postFileName;
 				strcpy_s( m_szPatchFile, strFileName.c_str() );
 
 				this->InvalidateRect( &rect, FALSE );
 				this->UpdateWindow();
 				this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_BEGIN_DOWNLOAD );
			}
			break;

		case UI_COMMAND_NEED_UPDATE:
			{
				m_bConnectDirServer = true;
				ConnectDirServe(); 
				this->InvalidateRect( &rect, FALSE );
				this->UpdateWindow();
				m_bUpdated = true;
			}
			break;

		case UI_COMMAND_OLD_VERSION:
			m_btnUpdate.ShowWindow(SW_HIDE);
			m_btnEnterGame.ShowWindow(SW_SHOW);
			m_btnEnterGame.EnableWindow(FALSE);
			m_btnCancelUpdate.ShowWindow(SW_HIDE);
			m_btnEnterGame.EnableWindow(FALSE);

			this->InvalidateRect( &rect, FALSE );
			this->UpdateWindow();
			::MessageBoxW( GetSafeHwnd(), L"版本太老,无法更新,请到官方网站重新下载!!", L"错误", MB_OK );
			break;

		case UI_COMMAND_DOWNLOAD_ATTACH:
			{
				m_btnCancelUpdate.ShowWindow(SW_SHOW);
			}
			break;

		case UI_COMMAND_LATEST_VERSION:
			{
				m_bDownloading = false;
				m_btnUpdate.ShowWindow(SW_HIDE);
				m_btnEnterGame.ShowWindow(SW_SHOW);
				m_btnCancelUpdate.ShowWindow(SW_HIDE);
				CheckExpServer();
				if (m_SmallClient && !PathFileExists("data\\common.pkg") && !PathFileExists("data\\attach.pkg"))
				{
					m_bDownloadAttach = true;
					if ( m_pDonwloader->IsPaused() )
					{
						m_pDonwloader->ContinueDownload();
					}
					else
					{
						DWORD dwThreadId;
						HANDLE hThread = ::CreateThread(
							NULL,
							0, 
							ThreadDownloadAttach, 
							this, 
							0, 
							&dwThreadId);

						CloseHandle( hThread );
					}
				}
				else
				{
					m_btnEnterGame.EnableWindow(TRUE);
				}
				if( m_bUpdated )
				{
					this->InvalidateRect( &rect, FALSE );
					this->UpdateWindow();
					RECT rc;
					m_dlgLog.Create( LauncherChangeLogLayerDlg::IDD, this );
					m_dlgLog.setVersionInfo( m_strPatchVer, uint_to_version( m_pversionVerifier->getLatestVersionInfo().m_nVersionLatest ) );
					m_dlgLog.SetWindowPos( NULL, rc.left + 333,rc.top + 162, 469, 365, SWP_NOMOVE|SWP_NOZORDER );
					m_dlgLog.ShowWindow(SW_SHOW);
					this->EnableWindow(FALSE);
					m_bUpdated = false;
					break;
				}
			}
			this->InvalidateRect( &rect, FALSE );
			this->UpdateWindow();
			break;

		case UI_COMMAND_BEGIN_DOWNLOAD:
			{
				m_btnUpdate.ShowWindow(SW_HIDE);
				m_btnEnterGame.ShowWindow(SW_SHOW);
				m_btnEnterGame.EnableWindow(FALSE);
				m_btnCancelUpdate.ShowWindow(SW_SHOW);

				this->InvalidateRect( &rect, FALSE );
				this->UpdateWindow();
				unsigned int from = m_pversionVerifier->getLatestVersionInfo().m_nVersionFrom;
				unsigned int to = m_pversionVerifier->getLatestVersionInfo().m_nVersionTo;

				m_strPatchVer = uint_to_version(from) + string("-") + string(uint_to_version(to));
				// 首先检测当前下载器是否处于中断状态,若是则无需重新创建线程
				// TODO: 查看CURL库如何断开连接,目前是出于中断状态,不知道curl_easy_pause是否会一直占用连接.
				//		 若CURL库一直占用连接,则考虑如何释放连接,以减少不必要的资源消耗
				if ( m_pDonwloader->IsPaused() )
				{
					m_bDownloading = true;
					m_pDonwloader->ContinueDownload();
				}
				else
				{
					DWORD dwThreadId;
					HANDLE hThread = ::CreateThread(
						NULL,
						0, 
						ThreadDownloadPatch, 
						this, 
						0, 
						&dwThreadId);

					CloseHandle( hThread );
				}
			}
			break;

		case UI_COMMAND_CANCEL_DOWNLOAD:
			{
				// 若当前处于安装补丁状态,则不能取消当前操作
				if ( m_bUpdatingPatch )
				{
					::MessageBoxW( GetSafeHwnd(), L"当前处于安装补丁包状态,不能进行取消操作!", L"提示", MB_OK );
				}
				else
				{
					m_bDownloading = false;
// 					m_bDownloadAttach = false;
					m_btnUpdate.ShowWindow(SW_SHOW);
					m_btnEnterGame.ShowWindow(SW_HIDE);
					m_btnCancelUpdate.ShowWindow(SW_HIDE);
					m_btnUpdate.EnableWindow(TRUE);
					m_pDonwloader->StopDownload();

					this->InvalidateRect( &rect, FALSE );
					this->UpdateWindow();
					m_nLastIndex = 0;
				}
			}
			break;

		case UI_COMMAND_ERROR_UPDATE:
			m_btnUpdate.ShowWindow(SW_SHOW);
			m_btnUpdate.EnableWindow(TRUE);
			m_btnEnterGame.ShowWindow(SW_HIDE);
			m_btnCancelUpdate.ShowWindow(SW_HIDE);

			this->InvalidateRect( &rect, FALSE );
			this->UpdateWindow();
			break;

		case UI_COMMAND_ENTER_GAME:
			{
				// 在这儿启动游戏
#ifdef _DEBUG
				::ShellExecute( NULL, "open", "Client_d.exe", "launcher", NULL, SW_SHOWNORMAL );
#else
				::ShellExecute( NULL, "open", "Client.exe", "launcher", NULL, SW_SHOWNORMAL );
#endif
				::exit( 0 );
			}
			break;

		case UI_COMMAND_RESET_EFFECT:
			{
				// 设置游戏的显示画面的效果
				CGraphSettingLayerDlg dlg;
				dlg.DoModal();
			}
			break;

		case UI_COMMAND_CLOSE:
			{
				// 检查若处于安装补丁包过程,则不能进行关闭操作
				if ( !m_bUpdatingPatch )
				{
					// 若当前处于下载补丁状态,则不能取消下载
					if ( m_bDownloading )
					{
						if (::MessageBoxW( GetSafeHwnd(), L"当前正在下载补丁,是否取消下载？", L"提示", MB_YESNO ) == IDYES )
						{
							exit(0);
						}
					}else if( m_bIsCopyFile )
					{
						if(::MessageBoxW( GetSafeHwnd(), L"当前正在拷贝客户端,是否取消拷贝？", L"提示", MB_YESNO ) == IDYES)
						{
							exit(0);
						}
					}else
					{
						exit(0);
					}
				}
				else
				{
					::MessageBoxW( GetSafeHwnd(), L"当前处于安装补丁包状态,不能进行取消操作!", L"提示", MB_OK );
				}
			}
			break;

		case UI_COMMAND_MINIMIZE:
			GetParent()->ShowWindow(SW_SHOWMINIMIZED);
			break;

		case UI_COMMAND_FAIL_DIR:
			{
				//::MessageBoxW( GetSafeHwnd(), L"连接服务器失败,无法获取服务器列表,请尝试重新连接!", L"提示",MB_SYSTEMMODAL );
			}

			break;

		case UI_COMMAND_FAIL_VER:
			{
// 				MessageBoxW( GetSafeHwnd(), L"连接服务器失败,无法验证版本信息,请尝试重新连接!", L"提示",
// 							MB_SYSTEMMODAL );
				//连接版本服务器失败时，继续重新链接
				static int count = 1;
				if( count < 4 )
				{
					ConnectVersionServe();
					++count;
				}else{
					CheckExpServer();
				}
			}
			break;

		case UI_COMMAND_RECHECK_VERSION:
			{
				ConnectVersionServe();
			}
			break;

		case UI_COMMAND_OPEN_UPDATE_URL:
			{
				// 若当前处于下载状态, 则取消下载
				if ( m_bDownloading )
				{
					this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_CANCEL_DOWNLOAD );
				}

				OpenUrl( m_szSfxPatchUrl );
			}
			break;
		case UI_COMMAND_BEGIN_PATCH:
			m_bUpdatingPatch = true;
			break;

		case UI_COMMAND_UPDATE_PATCH_SUCCESS:
			{
				if ( NeedSelfUpdateLauncher() )
				{
					exit(0);
					m_bUpdatingPatch = false;
				}else{
					m_bUpdatingPatch = false;
				}

				UpdateSelfCompleteCheckInfo();
			}
			break;
		case UI_COMMAND_COPYFILE_PROGRESS:
			{
				int nProgress = 0;
				nProgress = (int)ceil( ( double )( 100 * m_CopyedSize/m_TotalCopySize) );
				m_updateBarProgress.SetPercent( nProgress );
				this->InvalidateRect( &rect, FALSE );
				this->UpdateWindow();
			}
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


void CLauncherDlg::ApplyPatch( const char* szEXEPath )
{
	this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_BEGIN_PATCH );

	char szPath[128] = { '\0' };
	::GetCurrentDirectory( 128, szPath );

	// 创建补丁目录
	string strPatchPath = /*string( szPath ) + */string( "Patch" );
	CreateMultipleDirectory( CString( strPatchPath.c_str() ) );
	// 现在补丁包释放到补丁目录
	int ret = m_SelfExtractor.ExtractAll( CString( strPatchPath.c_str() ), CString( szEXEPath ), NULL, NULL );
	if( ret != SUCCESS )
	{
		CStringW message;
		message.Format( L"解压补丁失败(%d)，请重试",ret );
		::MessageBoxW( GetSafeHwnd(),message, L"更新失败", MB_OK );
		exit(0);
	}

	string strOrgPatch		= /*string( szPath ) + */string( "data\\update.pkg" );
	string strDesPatch		= /*string( szPath ) + */string( "data\\update_tmp.pkg" );
	string strExtractPatch	= strPatchPath + string( "\\update.pkg" );

	// 先删除原来可能已经存在的update_tmp.pkg文件
	DeleteFile( strDesPatch.c_str( ) );

	// 版本比较

	string newVerFile = strPatchPath + string( "\\" ) + VERSION_PATH;
	string oldVerFile = /*string( szPath ) + string( "\\" ) +*/ VERSION_PATH;
	unsigned int newVer = m_pversionVerifier->GetFileVersion(newVerFile.c_str());
	unsigned int oldVer = m_pversionVerifier->GetFileVersion(oldVerFile.c_str());
	//
	if( oldVer == 0 )
	{
		char szVersion[50];
		GetPrivateProfileString("version","ver","",szVersion,50, oldVerFile.c_str() );
		oldVer = version_to_uint( szVersion );
	}
	if( newVer == 0 )
	{
		char szVersion[50];
		GetPrivateProfileString("version","ver","",szVersion,50, newVerFile.c_str() );
		newVer = version_to_uint( szVersion );
	}
	if ( oldVer > newVer )
	{
		::MessageBoxW( GetSafeHwnd(), L"客户端版本已经是最新的无需更新", L"更新失败", MB_OK );
		//EndDialog( TRUE );
		this->SendMessage( WM_PATCH, CbUpdateDone, 0 );
		return;
	}

	// 至少要有data目录
	string strDataPath = /*string( szPath ) + */string( "data" );
	CreateMultipleDirectory( CString( strDataPath.c_str() ) );

	// 拷贝补丁文件先往拷贝打
	CopyFile( strOrgPatch.c_str(), strDesPatch.c_str(), true );

	FilePackage package;
	GameFilePackage desPackage;
	if ( package.open( strExtractPatch.c_str(), true ) && desPackage.open( strDesPatch.c_str(), false ) )
	{
		int nFileNum = (int)package.getNumFiles();

		for ( int i = 0; i < nFileNum; i++ )
		{
			DataStream* pStream = package.openFile( i );
			const PkgFileInfo& fileInfo = package.getFileInfo( i );

			if( !isInUnCompressPath( string( fileInfo.szPath ) ) )
			{
				if( _stricmp( fileInfo.szPath, "Launcher.exe" ) == 0 )
				{
					//跳过launcher
					m_bNeedUpdateSelf = true;
					FILE* fp = fopen( HELPER_UPDATE_LAUNCHER_NAME,"wb" );
					if( fp != NULL )
					{
						long	len		= fileInfo.originsize;
						char*	pbuf	= new char[len];
						pStream->read( pbuf, len );
						fwrite( pbuf, len, 1, fp );
						fclose( fp );
						delete pbuf;
					}
					continue;
				}else if( _stricmp( fileInfo.szPath, VERSION_PATH.c_str() ) == 0 )
				{
					continue;
				}
				string strPath( fileInfo.szPath );
				if( string( fileInfo.szPath ).find_last_of("\\") != string::npos )
				{
					strPath = strPath;
					string createPath = strPath.substr( 0, strPath.find_last_of("\\") );
					CreateMultipleDirectory( CString( createPath.c_str() ) );
				}
				string unComPath = string( fileInfo.szPath ) ;
				char tmpBuffer[256];
				_snprintf_s( tmpBuffer, sizeof( tmpBuffer ), "%s", fileInfo.szPath );
				_snprintf_s( tmpBuffer, sizeof( tmpBuffer ), "%s", unComPath.c_str() );
				FILE* fp = fopen( unComPath.c_str() , "wb" );
				if ( fp != NULL )
				{
					long	len		= fileInfo.originsize;
					char*	pbuf	= new char[len];
					pStream->read( pbuf, len );
					fwrite( pbuf, len, 1, fp );
					fclose( fp );
					delete pbuf;
				}
				else
				{
					::MessageBoxW( GetSafeHwnd(), L"请关闭游戏客户端再执行更新程序", L"更新失败", MB_OK );
					exit(0);
				}
			}
			else
			{
				// 其他的文件直接打包到update.pkg包里
				long	len		= fileInfo.originsize;
				char*	pbuf	= new char[len];
				pStream->read( pbuf, len );
				desPackage.appendOrReplaceFile( fileInfo.key1, fileInfo.key2, 1, pbuf, fileInfo.originsize, 0 );
				delete  pbuf;
			}

			int f = (int)( (i+1)/(float)nFileNum * 100);
			this->SendMessage( WM_PATCH, CbUpdateProgress, (LPARAM)f );
			delete pStream;
		}
	}
	else
	{
		::MessageBoxW( GetSafeHwnd(), L"请选择正确的游戏目录",  L"更新失败", MB_OK );
		package.close();
		desPackage.close();
		this->SendMessage( WM_PATCH, CbUpdateDone, 0 );
		exit(0);
		return;
	}

	package.close();
	desPackage.close();

	DeleteFile(strOrgPatch.c_str());
	DeleteFile( strExtractPatch.c_str() );

	if( rename( strDesPatch.c_str(), strOrgPatch.c_str() ) != 0 )
	{
		::MessageBoxW( GetSafeHwnd(), L"更新失败，请重试", L"更新失败", MB_OK );
		exit(0);
	}

	// 最后拷贝版本信息文件
	DeleteFile( oldVerFile.c_str() );
	::CopyFile( newVerFile.c_str(), oldVerFile.c_str(), true );
	DeleteFile( newVerFile.c_str() );

	/*	这个先去掉
	//补丁安装次数统计
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it != NULL )
	{
		char szBuf[256];
		sprintf( szBuf,"http://ljy.the9.com/count/auto_patch_setup_ok.html/?version=%s",uint_to_version(newVer) );
		HINTERNET it2 = InternetOpenUrl( it,szBuf,NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
		InternetCloseHandle(it2);
	}
	InternetCloseHandle(it);
	*/

	this->SendMessage( WM_PATCH, CbUpdateDone, 0 );
	this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_UPDATE_PATCH_SUCCESS );
}

void CLauncherDlg::InitGuiStyle()
{

	// GUI 按钮 surface
	CDC*	pCDC	= GetDC();
	HDC		hdc		= pCDC->GetSafeHdc();

	LOAD_IMAGE( m_imgDowndloadProgress,	def_imgDownloadProgress );
	LOAD_IMAGE( m_imgUpdateProgress,	def_imgUpdateProgress );
	RECT rc;

	m_imgSelfDownload[0].Load("./resource/ShouDongGengXin1.png");
	m_imgSelfDownload[1].Load("./resource/ShouDongGengXin2.png");
	m_imgSelfDownload[2].Load("./resource/ShouDongGengXin3.png");
	m_btnSelfDownload.Create("",BS_PUSHBUTTON,rc,this,IDC_SELFDOWNLOAD);
	m_btnSelfDownload.SetBitmaps(m_imgSelfDownload[1],RGB(255,0,255),m_imgSelfDownload[0],RGB(255,0,255),m_imgSelfDownload[2],RGB(255,0,255));
	m_btnSelfDownload.SizeToContent();
	m_btnSelfDownload.SetWindowPos(this,def_ptSelfDownload.x,def_ptSelfDownload.y,0,0,SWP_NOSIZE|SWP_NOZORDER );
	m_btnSelfDownload.ShowWindow(SW_SHOW);

	m_imgUpdate[0].Load("./resource/KaiShiGengXin1.png");
	m_imgUpdate[1].Load("./resource/KaiShiGengXin2.png");
	m_imgUpdate[2].Load("./resource/KaiShiGengXin3.png");
	m_btnUpdate.Create("",BS_PUSHBUTTON,rc,this,IDC_UPDTAE);
	m_btnUpdate.SetBitmaps(m_imgUpdate[1],0,m_imgUpdate[0],0,m_imgUpdate[2],0);
	m_btnUpdate.SizeToContent();
	m_btnUpdate.SetWindowPos(this,def_ptBtnUpdate.x,def_ptBtnUpdate.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_btnUpdate.ShowWindow(SW_HIDE);

	m_imgResetEffect[0].Load("./resource/SheZhi1.png");
	m_imgResetEffect[1].Load("./resource/SheZhi2.png");
	m_imgResetEffect[2].Load("./resource/SheZhi3.png");
	m_btnResetEffect.Create("",BS_PUSHBUTTON,rc,this,IDC_RESETEFFECT);
	m_btnResetEffect.SetBitmaps(m_imgResetEffect[1],0,m_imgResetEffect[0],0,m_imgResetEffect[2],0);
	m_btnResetEffect.SizeToContent();
	m_btnResetEffect.SetWindowPos(this,def_ptResetEffect.x,def_ptResetEffect.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgRegister[0].Load("./resource/ZhuCeZhangHao1.png");
	m_imgRegister[1].Load("./resource/ZhuCeZhangHao2.png");
	m_imgRegister[2].Load("./resource/ZhuCeZhangHao3.png");
	m_btnRegister.Create("",BS_PUSHBUTTON,rc,this,IDC_REGISTER);
	m_btnRegister.SetBitmaps(m_imgRegister[1],0,m_imgRegister[0],0,m_imgRegister[2],0);
	m_btnRegister.SizeToContent();
	m_btnRegister.SetWindowPos(this,def_ptRegister.x,def_ptRegister.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btnRegister.ShowWindow(SW_SHOW);

	m_imgUpdateLog[0].Load("./resource/GengXinRiZhi1.png");
	m_imgUpdateLog[1].Load("./resource/GengXinRiZhi2.png");
	m_imgUpdateLog[2].Load("./resource/GengXinRiZhi3.png");
	m_btnUpdateLog.Create("",BS_PUSHBUTTON,rc,this,IDC_UPDATELOG);
	m_btnUpdateLog.SetBitmaps(m_imgUpdateLog[1],0,m_imgUpdateLog[0],0,m_imgUpdateLog[2],0);
	m_btnUpdateLog.SizeToContent();
	m_btnUpdateLog.SetWindowPos(this,def_ptUpdateLog.x,def_ptUpdateLog.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btnUpdateLog.ShowWindow(SW_SHOW);

	m_imgGuider[0].Load("./resource/XinShouZhiNan1.png");
	m_imgGuider[1].Load("./resource/XinShouZhiNan2.png");
	m_imgGuider[2].Load("./resource/XinShouZhiNan3.png");
	m_btnGuider.Create("",BS_PUSHBUTTON,rc,this,IDC_GUIDER);;
	m_btnGuider.SetBitmaps(m_imgGuider[1],0,m_imgGuider[0],0,m_imgGuider[2],0);
	m_btnGuider.SizeToContent();
	m_btnGuider.SetWindowPos(this,def_ptGuider.x,def_ptGuider.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btnGuider.ShowWindow(SW_SHOW);

	m_imgCustomerService[0].Load("./resource/KeFuZhongXin1.png");
	m_imgCustomerService[1].Load("./resource/KeFuZhongXin2.png");
	m_imgCustomerService[2].Load("./resource/KeFuZhongXin3.png");
	m_btnCustomerService.Create("",BS_PUSHBUTTON,rc,this,IDC_CUSTOMERSERVICE);
	m_btnCustomerService.SetBitmaps(m_imgCustomerService[1],0,m_imgCustomerService[0],0,m_imgCustomerService[2],0);
	m_btnCustomerService.SizeToContent();
	m_btnCustomerService.SetWindowPos(this,def_ptCustomerService.x,def_ptCustomerService.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btnCustomerService.ShowWindow(SW_SHOW);

	m_imgOfficebbs[0].Load("./resource/GuanFangLunTan1.png");
	m_imgOfficebbs[1].Load("./resource/GuanFangLunTan2.png");
	m_imgOfficebbs[2].Load("./resource/GuanFangLunTan3.png");
	m_btnOfficebbs.Create("",BS_PUSHBUTTON,rc,this,IDC_OFFICEBBS);
	m_btnOfficebbs.SetBitmaps(m_imgOfficebbs[1],0,m_imgOfficebbs[0],0,m_imgOfficebbs[2],0);
	m_btnOfficebbs.SizeToContent();
	m_btnOfficebbs.SetWindowPos(this,def_ptOfficebbs.x,def_ptOfficebbs.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgOfficeSite[0].Load("./resource/GuanFangWangZhan1.png");
	m_imgOfficeSite[1].Load("./resource/GuanFangWangZhan2.png");
	m_imgOfficeSite[2].Load("./resource/GuanFangWangZhan3.png");
	m_btnOfficeSite.Create("",BS_PUSHBUTTON,rc,this,IDC_OFFICESITE);
	m_btnOfficeSite.SetBitmaps(m_imgOfficeSite[1],RGB(255,0,255),m_imgOfficeSite[0],RGB(255,0,255),m_imgOfficeSite[2],RGB(255,0,255));
	m_btnOfficeSite.SizeToContent();
	m_btnOfficeSite.SetWindowPos(this,def_ptOfficeSite.x,def_ptOfficeSite.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgEnterGame[0].Load("./resource/JinRuYouXi1.png");
	m_imgEnterGame[1].Load("./resource/JinRuYouXi2.png");
	m_imgEnterGame[2].Load("./resource/JinRuYouXi3.png");
	m_btnEnterGame.Create("",BS_PUSHBUTTON,rc,this,IDC_ENTERGAME);
	m_btnEnterGame.SetBitmaps(m_imgEnterGame[1],0,m_imgEnterGame[0],0,m_imgEnterGame[2],0);
	m_btnEnterGame.SizeToContent();
	m_btnEnterGame.SetWindowPos(this,def_ptEnterGame.x,def_ptEnterGame.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btnEnterGame.ShowWindow(SW_SHOW);
	m_btnEnterGame.EnableWindow(FALSE);

	m_imgCancelUpdate[0].Load("./resource/ZanTingGengXin1.png");
	m_imgCancelUpdate[1].Load("./resource/ZanTingGengXin2.png");
	m_imgCancelUpdate[2].Load("./resource/ZanTingGengXin3.png");
	m_btnCancelUpdate.Create("",BS_PUSHBUTTON,rc,this,IDC_CANCELUPDATE);
	m_btnCancelUpdate.SetBitmaps(m_imgCancelUpdate[1],0,m_imgCancelUpdate[0],0,m_imgCancelUpdate[2],0);
	m_btnCancelUpdate.SizeToContent();
	m_btnCancelUpdate.SetWindowPos(this,def_ptCancelUpdate.x,def_ptCancelUpdate.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btnCancelUpdate.ShowWindow(SW_HIDE);
	// 	m_btnCancelUpdate.EnableWindow(FALSE);

	m_imgClose[0].Load("./resource/GuanBi1.png");
	m_imgClose[1].Load("./resource/GuanBi2.png");
	m_imgClose[2].Load("./resource/GuanBi3.png");
	m_btnClose.Create("",BS_PUSHBUTTON,rc,this,IDC_CLOSE);
	m_btnClose.SetBitmaps(m_imgClose[1],0,m_imgClose[0],0,m_imgClose[2],0);
	m_btnClose.SizeToContent();
	m_btnClose.SetWindowPos(this,def_ptClose.x,def_ptClose.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgMinimize[0].Load("./resource/ZuiXiaoHua1.png");
	m_imgMinimize[1].Load("./resource/ZuiXiaoHua2.png");
	m_imgMinimize[2].Load("./resource/ZuiXiaoHua3.png");
	m_btnMinimize.Create("",BS_PUSHBUTTON,rc,this,IDC_MINIMIZE);
	m_btnMinimize.SetBitmaps(m_imgMinimize[1],0,m_imgMinimize[0],0,m_imgMinimize[2],0);
	m_btnMinimize.SizeToContent();
	m_btnMinimize.SetWindowPos(this,def_ptMinimize.x,def_ptMinimize.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_downloadBarProgress.SetPos( def_ptDownloadProgress );
	m_downloadBarProgress.SetDestDC( hdc );
	m_downloadBarProgress.SetHwnd( m_hWnd );
	m_downloadBarProgress.SetImage( &m_imgDowndloadProgress );	
	m_downloadBarProgress.m_pAlphaImage = NULL;

	m_downloadBarProgress.InitAlpha( );
	m_downloadBarProgress.SetPercent( 0 );

	m_updateBarProgress.SetPos( def_ptDownloadProgress );
	m_updateBarProgress.SetDestDC( hdc );
	m_updateBarProgress.SetHwnd( m_hWnd );
	m_updateBarProgress.SetImage( &m_imgUpdateProgress );
	m_updateBarProgress.m_pAlphaImage = NULL;

	m_updateBarProgress.InitAlpha( );
	m_updateBarProgress.SetPercent( 0 );

	m_imgExpServer[0].Load("./resource/TiYanFu1.png");
	m_imgExpServer[1].Load("./resource/TiYanFu2.png");
	m_imgExpServer[2].Load("./resource/TiYanFu3.png");
	m_btnExpServer.Create("",BS_PUSHBUTTON,rc,this,IDC_EXPSERVER);
	m_btnExpServer.SetBitmaps(m_imgExpServer[1],0,m_imgExpServer[0],0,m_imgExpServer[2],0);
	m_btnExpServer.SizeToContent();
	m_btnExpServer.SetWindowPos(this,def_ptExpServer.x,def_ptExpServer.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgNormalServer[0].Load("./resource/ZhengShiFu1.png");
	m_imgNormalServer[1].Load("./resource/ZhengShiFu2.png");
	m_imgNormalServer[2].Load("./resource/ZhengShiFu3.png");
	m_btnNormalServer.Create("",BS_PUSHBUTTON,rc,this,IDC_NORMALSERVER);
	m_btnNormalServer.SetBitmaps(m_imgNormalServer[1],0,m_imgNormalServer[0],0,m_imgNormalServer[2],0);
	m_btnNormalServer.SizeToContent();
	m_btnNormalServer.SetWindowPos(this,def_ptNormalServer.x,def_ptNormalServer.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_imgActive[0].Load("./resource/ZhangHaoChongZhi1.png");
	m_imgActive[1].Load("./resource/ZhangHaoChongZhi2.png");
	m_imgActive[2].Load("./resource/ZhangHaoChongZhi3.png");
	m_btnActive.Create("",BS_PUSHBUTTON,rc,this,IDC_ACTIVE);
	m_btnActive.SetBitmaps(m_imgActive[1],0,m_imgActive[0],0,m_imgActive[2],0);
	m_btnActive.SizeToContent();
	m_btnActive.SetWindowPos(this,def_ptActive.x,def_ptActive.y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_btnRegister.ShowWindow(SW_SHOW);
	m_btnUpdateLog.ShowWindow(SW_SHOW);
	m_btnCustomerService.ShowWindow(SW_SHOW);
	m_btnOfficeSite.ShowWindow(SW_SHOW);
	m_btnResetEffect.ShowWindow(SW_SHOW);
	m_btnOfficebbs.ShowWindow(SW_SHOW);
	m_btnClose.ShowWindow(SW_SHOW);
	m_btnMinimize.ShowWindow(SW_SHOW);
	m_btnExpServer.ShowWindow(SW_HIDE);
	m_btnNormalServer.ShowWindow(SW_HIDE);
	m_btnActive.ShowWindow(SW_SHOW);
}

bool CLauncherDlg::DownloadPatch( const char* url, const char *checksum )
{
	char szPath[128] = { '\0' };
	GetFilePath( "patch", szPath, sizeof( szPath ) );
	string	path( szPath );
	string	fileName( url );
	string	postFileName;
	string::size_type pos = fileName.find_last_of( "/\\");
	if ( pos != string::npos )
	{
		postFileName = fileName.substr( pos );
	}
	else
	{
		postFileName = string( "\\" )+ fileName;
	}

	path = string( szPath ) + postFileName;

contdownload:
	if ( file_exist( path.c_str() ) )
	{
		if( checksum[0] == 0 || _strnicmp(checksum,file_checksum(path.c_str()),32) == 0 )
		{
			if ( ::MessageBoxW( GetSafeHwnd(), L"已经存在对应的补丁包，是否重新下载补丁包？", 
					L"提示", MB_YESNO ) == IDYES )
			{
				// 完全下载
				if ( !m_pDonwloader->Download( url ) )
				{
					return false;
				}
			}else{
				return true;
			}
		}else{
			// 从当前文件大小处开始续传
			if ( !m_pDonwloader->Download( url, (unsigned int)file_size( path.c_str() ) ) )
			{
				return false;
			}
		}
	}
	else
	{
		// 完全下载
		if ( !m_pDonwloader->Download( url ) )
		{
			return false;
		}
	}

	MSG msg;
	while ( !m_pDonwloader->IsDone( ) )
	{
		while ( ::PeekMessage( &msg, theApp.GetMainWnd()->GetSafeHwnd(), 0, 0, PM_REMOVE ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
		//theApp.PumpMessage();
		Sleep( 1 );
	}

	// 下载完成后，md5校验还是不通过，则删除下载完的文件，下次启动更新工具后，重新下载
	if ( ( m_pDonwloader->GetResult( ) == CURLE_BAD_DOWNLOAD_RESUME) || m_pDonwloader->GetResult() == CURLE_PARTIAL_FILE 
		|| m_pDonwloader->GetResult() == CURLE_RANGE_ERROR ||
		( checksum[0] != 0 && _strnicmp( checksum, file_checksum( path.c_str() ), 32 ) != 0 ) )
	{
		SetFileAttributes( path.c_str(), FILE_ATTRIBUTE_NORMAL );
		DeleteFile( path.c_str() );
		const int MAX_BUFFER_LENGTH = 256;
		char buf[MAX_BUFFER_LENGTH] = { '\0' };
		if ( m_pDonwloader->GetResult( ) != CURLE_OK )
		{
			sprintf_s( buf, "下载%s失败(%d)，是否重新下载？", url, m_pDonwloader->GetResult() );
		}
		else
		{
			sprintf_s( buf, "MD5码效验失败,下载%s失败，是否重新下载？", url );
		}
		// TODO
		wchar_t wBuffer[MAX_BUFFER_LENGTH] = { L'\0' };
	
		MultiByteToWideChar(
			ACP_TYPE,
			MB_PRECOMPOSED,
			buf,
			-1,
			wBuffer,
			MAX_BUFFER_LENGTH );
		if ( ::MessageBoxW( GetSafeHwnd(), wBuffer, L"错误", MB_YESNO ) == IDYES )
		{
			goto contdownload;
		}

		return false;
	}

	return m_pDonwloader->GetResult( ) == CURLE_OK; 
}

void CLauncherDlg::SetUrl( const char* pszUrl )
{
	m_pDonwloader->SetUrl( pszUrl );
}

void CLauncherDlg::SetLocalPath(const char* szPath)
{
	m_pDonwloader->SetLocalPath( szPath );
}


const char* CLauncherDlg::GetPatchFilePath() const
{
	return m_szPatchFile;
}

// 自更新Launcher
bool CLauncherDlg::NeedSelfUpdateLauncher()
{
	// 这次全找文件
	if ( m_bNeedUpdateSelf )
	{
		OSVERSIONINFOEX osvi;
		BOOL bOsVersionInfoEx;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );
		bOsVersionInfoEx = GetVersionEx( ( OSVERSIONINFO* ) &osvi );

		if ( osvi.dwMajorVersion > 5 )
		{
			//vista
			SHELLEXECUTEINFO shExecInfo;

			shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

			shExecInfo.fMask		= NULL;
			shExecInfo.hwnd			= NULL;
			shExecInfo.lpVerb		= "open";
			shExecInfo.lpFile		= HELPER_UPDATE_LAUNCHER_NAME;
			shExecInfo.lpParameters = "-helper";
			shExecInfo.lpDirectory	= NULL;
			shExecInfo.nShow		= SW_NORMAL;
			shExecInfo.hInstApp		= NULL;
			ShellExecuteEx( &shExecInfo );
		}
		else
		{
			ShellExecute( NULL, "open", HELPER_UPDATE_LAUNCHER_NAME, "-helper", NULL, SW_SHOW );	
		}
		return true;
	}
	return false;
}

// 自更新
void CLauncherDlg::SelfUpdateLauncher()
{
	char szPath[128] = { '\0' };
	::GetCurrentDirectory( 128, szPath );
	// 创建补丁目录
	CString strDir				= "";
	CString strOldVersionFiles	= strDir + UPDATE_LAUNCHER_NAME;
	CString strNewVersionFiles	= strDir + HELPER_UPDATE_LAUNCHER_NAME;
	SetFileAttributes( strOldVersionFiles.GetBuffer(), FILE_ATTRIBUTE_NORMAL );
	//这里尝试3次
	for( int i = 0 ; i < 3 ; i ++ )
	{
		if( ::CopyFile( strNewVersionFiles.GetBuffer(), strOldVersionFiles.GetBuffer(), FALSE ) )
		{
			break;
		}
		Sleep(500);
	}
	//TODO HERE 更新更新包
	UpdateSelfCompleteCheckInfo();
	// 更新好后重启Launcher
	ShellExecute( NULL, "open", UPDATE_LAUNCHER_NAME, "-updated", NULL, SW_SHOW );	
	exit(0);
}

// 连接DirServe
void CLauncherDlg::ConnectDirServe()
{
	assert( m_pdirServeSelector != NULL );
	if ( m_pdirServeSelector == NULL )
	{
		return;
	}
	m_pdirServeSelector->requestDirInfo();
}


void CLauncherDlg::ConnectVersionServe()
{
	assert( m_pversionVerifier != NULL );
	if ( m_pversionVerifier == NULL )
	{
		return;
	}

	if ( !m_pversionVerifier->requestVersionInfo() )
	{
		this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_FAIL_VER );
	}
}

void CLauncherDlg::OpenUrl( const char* url )
{
	if ( ( url != NULL ) && ( strstr( url, ":/" ) ) )
	{
		::ShellExecute( NULL, "open", url, NULL, NULL, SW_SHOWNORMAL );
	}
}

BOOL CLauncherDlg::OnEraseBkgnd(CDC* pDC)
{
	CBrush	brush( RGB(255,0,255) );
	CRect	rc;
	GetClientRect( rc );
	pDC->FillRect( rc, &brush );
	return FALSE;
}

void CLauncherDlg::OnSelfDownloadClick()
{
	SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_OPEN_UPDATE_URL );
}
void CLauncherDlg::OnUpdateClick()
{
	SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_BEGIN_DOWNLOAD );
}
void CLauncherDlg::OnEnterGameClick()
{
	SendMessage(WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_ENTER_GAME );
}
void CLauncherDlg::onCanelUpdateClick()
{
	SendMessage(WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_CANCEL_DOWNLOAD );
}
void CLauncherDlg::onCloseClick()
{
	SendMessage(WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_CLOSE );
}
void CLauncherDlg::onMinimizeClick()
{
	SendMessage(WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_MINIMIZE );
}
void CLauncherDlg::OnResetEffectClick()
{
	SendMessage(WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_RESET_EFFECT);
}
void CLauncherDlg::OnOfficeBbsClick()
{
	OpenUrl("http://bbs.ljy0.com");
}
void CLauncherDlg::OnOfficeSiteClick()
{
	OpenUrl( m_strHomePageUrl.c_str() );
}
void CLauncherDlg::OnCustomerServiceClick()
{
	OpenUrl("http://service.wanyouyl.com");
}
void CLauncherDlg::OnGuildClick()
{
	OpenUrl("http://www.ljy0.com/guide/");
}
void CLauncherDlg::OnUpdateLogClick()
{
	LauncherChangeLogLayerDlg dlg;
	dlg.setVersionInfo( m_strPatchVer, uint_to_version( m_pversionVerifier->getLatestVersionInfo().m_nVersionLatest ) );
	dlg.DoModal();
}
void CLauncherDlg::OnRegisterClick()
{
	OpenUrl(m_strRegisterUrl.c_str());
}

void CLauncherDlg::onActiveClick()
{
	OpenUrl( "http://pay.wanyouyl.com/" );
}

bool CLauncherDlg::loadLocalCfg()
{
	char* pszDecryptBuffer	= getDecryptFileBuffer( CLIENT_CFG_PATH.c_str(), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return false;
	}
	XMLData xmldata;
	if( !xmldata.loadBuffer( pszDecryptBuffer, strlen( pszDecryptBuffer )) )
	{
		delete[] pszDecryptBuffer;
		return false;
	}

	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.iterateChild();
	while ( !pChildNode.isNull() )
	{
		if ( _stricmp( pChildNode.getName(), "VerServer" ) == 0 )
		{
			m_NetCfg.m_verConfig.m_vData.clear();
			m_NetCfg.m_verConfig.m_strIp = pChildNode.attribToString( "ip" );
			m_NetCfg.m_verConfig.m_nPort = pChildNode.attribToInt( "port" );
			XMLNode dirNode;
			dirNode = pChildNode.iterateChild(dirNode);
			while( !dirNode.isNull() )
			{
				if( _stricmp(dirNode.getName(),"iplist") == 0 )
				{
					DirServerData data;
					data.ip = dirNode.attribToString("ip");
					data.port = dirNode.attribToInt("port");
					m_NetCfg.m_verConfig.m_vData.push_back(data);
				}
				dirNode = pChildNode.iterateChild(dirNode);
			}
		}
		else if ( _stricmp( pChildNode.getName(), "DirServer" ) == 0 )
		{
			LauncherNetConfigData data;
			data.m_strIp = pChildNode.attribToString("address");
			data.m_nPort = pChildNode.attribToInt("port");
			XMLNode dirNode;
			dirNode = pChildNode.iterateChild(dirNode);
			while( !dirNode.isNull()  )
			{
				if( strcmp(dirNode.getName(),"iplist")==0 )
				{
					DirServerData dirData;
					dirData.ip = dirNode.attribToString("ip");
					dirData.port = dirNode.attribToInt("port");
					data.m_vData.push_back( dirData );
				}
				dirNode = pChildNode.iterateChild(dirNode);
			}
			m_NetCfg.m_vDirConfig.push_back( data );
		}
		else if( strcmp( pChildNode.getName(),"DirUrl" ) == 0 )
		{
			m_NetCfg.m_DirUrl = pChildNode.attribToString("address");
		}
		else if( strcmp( pChildNode.getName(),"SmartClient" ) == 0 )
		{
			m_SmallClientUrl = pChildNode.attribToString("server");
			if (pChildNode.hasAttrib("attach"))
			{
				m_SmallClientAttachMD5 = pChildNode.attribToString("attach");
			}
		}else if( strcmp( pChildNode.getName(), "PatchUrl" ) == 0 )
		{
			strcpy( m_szSfxPatchUrl, pChildNode.attribToString("url") );
		}else if( strcmp(pChildNode.getName(), "LauncherUrl") == 0 )
		{
			m_strLauncherUrl = pChildNode.attribToString("url");
		}else if( strcmp( pChildNode.getName(),"VersionUrl" ) == 0 )
		{
			m_NetCfg.m_VersionUrl = pChildNode.attribToString( "address" );
		}
		pChildNode = pRootNode.iterateChild( pChildNode );
	}
	delete[] pszDecryptBuffer;

	m_ExpServerCfg.expFlag = 1;
	m_ExpServerCfg.path = "";

	pszDecryptBuffer = getDecryptFileBuffer( CLIENT_EXP_SERVER_CONFIG.c_str(),CLIENT_JIEMI );
	if( pszDecryptBuffer == NULL )
	{
		return true;
	}
	CIniFile file;
	if( file.Prepare(pszDecryptBuffer) )
	{
		m_ExpServerCfg.expFlag = file.GetInteger("ExpServerConfig","ExpFlag",1);
		char buf[256];
		file.GetString("ExpServerConfig","Path","",buf,256);
		m_ExpServerCfg.path = buf;
	}
	delete[] pszDecryptBuffer;
	file.Close();

	return true;
}
BEGIN_EVENTSINK_MAP(CLauncherDlg, CDialog)
//DISPID_DOCUMENTCOMPLETE DISPID_NEWWINDOW3
ON_EVENT(CLauncherDlg, IDC_EXPLORER1, 259, CLauncherDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
ON_EVENT(CLauncherDlg, IDC_EXPLORER1, 273, CLauncherDlg::OnDocumentNewWindow1, VTS_DISPATCH VTS_PBOOL VTS_I4 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()

class DocHostUIHandler : public IDocHostUIHandler 
{ 
public: 
	DocHostUIHandler()
		: refcount(1)
	{
	}
	STDMETHOD_(ULONG, AddRef)()
	{
		::InterlockedIncrement(&refcount);
		return refcount;
	}
	STDMETHOD_(ULONG, Release)()
	{
		if(refcount)
			::InterlockedDecrement(&refcount);
		return refcount;
	}
	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj)
	{
		if(iid == IID_IUnknown)
		{
			*ppvObj = (IUnknown*) this;
			AddRef();
		}
		else if(iid == IID_IDocHostUIHandler)
		{
			*ppvObj = (IDocHostUIHandler*) this;
			AddRef();
		}
		else if(iid == IID_IDispatch)
		{
			*ppvObj = (IDispatch*) this;
			AddRef();
		}
		else
		{
			return E_NOINTERFACE;
		}
		return S_OK;
	}
	STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH)
	{
		return S_OK;
	}
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo)
	{
	 	pInfo->cbSize = sizeof(DOCHOSTUIINFO);   
	 	pInfo->dwFlags = DOCHOSTUIFLAG_DIALOG |    
	 		DOCHOSTUIFLAG_THEME  |    
	 		DOCHOSTUIFLAG_NO3DBORDER |    
	 		DOCHOSTUIFLAG_SCROLL_NO;
		pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	 	return S_OK;
	}
	STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT,
		LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW)
	{
		return S_OK;
	}
	STDMETHOD(HideUI)(void)
	{
		return S_OK;
	}
	STDMETHOD(UpdateUI)(void)
	{
		return S_OK;
	}
	STDMETHOD(EnableModeless)(BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnDocWindowActivate)(BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnFrameWindowActivate)(BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD)
	{
		return S_OK;
	}
	STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetExternal)(LPDISPATCH*)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*)
	{
		return E_NOTIMPL;
	}
	private:
		long refcount;
};
static DocHostUIHandler ms_DocHostUIHandler;

void CLauncherDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	CString str( URL->bstrVal );
	if( str != "about:blank" )
	{
		m_ctrlWeb.SetWindowPos( NULL, 392, 333, 614, 173, SWP_NOZORDER );
	}else{
		IHTMLDocument2* pDoc;
		IHTMLElement* pEl;
		IHTMLBodyElement * pBodyEl;	
		if (SUCCEEDED(m_ctrlWeb.get_Document()->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc)))
		{
			ICustomDoc *pCustomDoc;
			if (SUCCEEDED(pDoc->QueryInterface(IID_ICustomDoc, (void **) &pCustomDoc)))
			{
				pCustomDoc->SetUIHandler(&ms_DocHostUIHandler);
			}
			if (SUCCEEDED(pDoc->get_body(&pEl)))
			{
				if (SUCCEEDED(pEl->QueryInterface(IID_IHTMLBodyElement, (void**)&pBodyEl)))
				{
					pBodyEl->put_scroll(BSTR(L"no"));
					pBodyEl->Release();
				}
				pBodyEl->Release();
			}
			pDoc->Release();
			m_ctrlWeb.Navigate(m_strLauncherUrl.c_str(), NULL, NULL, NULL, NULL);
		}
	}
}

// CArkFileDialog

IMPLEMENT_DYNAMIC(CArkFileDialog, CFileDialog)
CArkFileDialog::CArkFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
							   DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}
BEGIN_MESSAGE_MAP(CArkFileDialog, CFileDialog)
END_MESSAGE_MAP()

// CArkFileDialog message handlers

INT_PTR CArkFileDialog::DoModal( CString& RootPath )
{
	// TODO: Add your specialized code here and/or call the base class
	INT_PTR re = CFileDialog::DoModal();
	if( re == IDOK )
		RootPath = m_FloderName;

	return re;
}

void CArkFileDialog::OnFolderChange()
{
	// TODO: Add your specialized code here and/or call the base class
	m_FloderName = GetFolderPath();

	CFileDialog::OnFolderChange();
}

void CLauncherDlg::CheckExpServer()
{
	m_btnExpServer.ShowWindow(SW_HIDE);
	m_btnNormalServer.ShowWindow(SW_HIDE);
	return;
	HANDLE hFile = CreateFile(CLIENT_EXP_CFG_PATH.c_str() , FILE_GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	//不存在这个文件
	if(hFile==INVALID_HANDLE_VALUE)
	{
		return;
	}
	CloseHandle(hFile);
	if( m_ExpServerCfg.expFlag == 1 )
	{
		m_btnExpServer.ShowWindow(SW_SHOW);
	}else if( m_ExpServerCfg.expFlag == 2 )
	{
		m_btnNormalServer.ShowWindow(SW_SHOW);
	}
}

void CLauncherDlg::onExpServerClick()
{
	m_btnExpServer.EnableWindow(FALSE);
	if( !m_ExpServerCfg.path.empty() )
	{
		//看看 那个目录的是否有 launcher
		string launcherPath = m_ExpServerCfg.path +"\\"+ "launcher.exe";
		HANDLE hFile = CreateFile(launcherPath.c_str(), FILE_GENERIC_READ, 
			FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);  
		if(hFile==INVALID_HANDLE_VALUE)
		{
			//MessageBox("无法找到终极测试服启动程序","错误",MB_OK);
			CreateNewClient();
			return;
		}
		CloseHandle(hFile);
		ShellExecute( NULL, "open", launcherPath.c_str(), "", m_ExpServerCfg.path.c_str(), SW_SHOWNORMAL );
		::exit(0);
	}else{
		CreateNewClient();
	}
}

void CLauncherDlg::onNormalServerClick()
{
	m_btnNormalServer.EnableWindow(FALSE);
	if( m_ExpServerCfg.path.empty() )
	{
		//MessageBox("无法找到正式服启动程序","错误",MB_OK);
		CreateNewClient();
	}else{
 		string launcherPath = m_ExpServerCfg.path + "\\" + "launcher.exe";
 		HANDLE hFile = CreateFile(launcherPath.c_str(), FILE_GENERIC_READ, 
 			FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
 		if(hFile==INVALID_HANDLE_VALUE)
 		{
 			//MessageBox("无法找到正式服启动程序","错误",MB_OK);
			CreateNewClient();
 			return;
 		}
		CloseHandle(hFile);
		ShellExecute( NULL, "open", launcherPath.c_str(), "", m_ExpServerCfg.path.c_str(), SW_SHOWNORMAL );
		::exit(0);
	}
}

void CLauncherDlg::CopyDirFiles()
{
	std::list<CopyFileInfo>::iterator it = m_lstCopyFiles.begin();
	for( ; it != m_lstCopyFiles.end() ; it ++ )
	{
		CopyFile(it->path, it->destPath, FALSE);
		m_CopyedSize += it->size;
		this->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_COPYFILE_PROGRESS );
	}
	m_lstCopyFiles.clear();
}

DWORD WINAPI ThreadCopyClientFunction( LPVOID param )
{
	//从当前的工作目录 拷贝到 目标目录
	CLauncherDlg* dlg = (CLauncherDlg*)param;
	// 计算大小
	dlg->m_TotalCopySize = 0;
	char dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,dir);
	dlg->CalcCopyFileSize( dir,dlg->m_ExpServerCfg.path.c_str() );
	dlg->m_CopyedSize = 0;
	dlg->PostMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_COPYFILE_PROGRESS );
	dlg->m_bIsCopyFile = true;
	dlg->CopyDirFiles();
	dlg->DoFinishWork();
	return 0;
}

void CLauncherDlg::CreateNewClient()
{
	char dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,dir);
	//对硬盘空间的判断

	CString gamePath = CString(dir) /*+ "\\" + _T("终极测试服")*/;
	if( m_ExpServerCfg.expFlag == 2 )
	{
		gamePath = gamePath + "\\" + _T("天空双线区");
	}else{
		gamePath = gamePath + "\\" + _T("终极测试区");
	}

	m_ExpServerCfg.path = gamePath.GetBuffer();
	CString newPath = gamePath + "\\" + "launcher.exe" ;
	HANDLE hFile = CreateFile( newPath, FILE_GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		m_btnEnterGame.EnableWindow(FALSE);
		DWORD dwThreadId;
		HANDLE hThread = ::CreateThread(
			NULL,
			0, 
			ThreadCopyClientFunction, 
			this, 
			0, 
			&dwThreadId);

		CloseHandle( hThread );
	}else{
		CloseHandle(hFile);
		m_ExpServerCfg.path = gamePath.GetBuffer();
		WriteSrcServerList();
		ShellExecute( NULL, "open", newPath, "", m_ExpServerCfg.path.c_str(), SW_SHOWNORMAL );
		::exit(0);
	}
}

void CLauncherDlg::CalcCopyFileSize( CString srcDir, CString destDir )
{
	HANDLE hFind;
	WIN32_FIND_DATA ffd;
	ZeroMemory( &ffd, sizeof(WIN32_FIND_DATA) );
	string strSeach = srcDir + "\\*.*";
	hFind = FindFirstFile( strSeach.c_str(), &ffd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		while( FindNextFile( hFind, &ffd ) )
		{
			if( destDir == srcDir + "\\" +ffd.cFileName )
			{
				continue;
			}
			if( strcmp( "..", ffd.cFileName ) && strcmp(".", ffd.cFileName ) )
			{
				if( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					CString destFile = destDir + "\\" + ffd.cFileName;
					CreateMultipleDirectory( destFile );
					CalcCopyFileSize(srcDir + "\\" + ffd.cFileName,destFile );
				}else{
					CopyFileInfo info;
					info.path = srcDir + "\\" + ffd.cFileName;
					info.destPath = destDir + "\\" + ffd.cFileName;
					info.size = (((__int64)ffd.nFileSizeHigh * (MAXDWORD+1)) + (__int64)ffd.nFileSizeLow);
					m_lstCopyFiles.push_back(info);
					m_TotalCopySize += (((__int64)ffd.nFileSizeHigh * (MAXDWORD+1)) + (__int64)ffd.nFileSizeLow);
				}
			}
		}
		FindClose( hFind );
	}
}

void CLauncherDlg::WriteSrcServerList()
{
	string destConfigFile = CLIENT_EXP_SERVER_CONFIG;
	CIniFile file;
	file.Create( destConfigFile.c_str() );
	if( file.Create( destConfigFile.c_str() ) )
	{
		file.WriteInteger( "ExpServerConfig","ExpFlag",m_ExpServerCfg.expFlag );
		file.WriteString("ExpServerConfig","Path",m_ExpServerCfg.path.c_str());
		file.Save();
		DesDecryptFile( destConfigFile.c_str(), destConfigFile.c_str(), CLIENT_JIAMI) ;
	}
	file.Close();
}

void CLauncherDlg::WriteDestServerList()
{
// 	//修改目标目录的launcher
	string destServerlist = m_ExpServerCfg.path + "\\" + CLIENT_CFG_PATH;
	string destServerlistTmp = m_ExpServerCfg.path + "\\" + CLIENT_EXP_CFG_PATH;
	CopyFile( destServerlist.c_str(), "serverlisttmp", FALSE );
	CopyFile( destServerlistTmp.c_str(), destServerlist.c_str(), FALSE);
	CopyFile( "serverlisttmp", destServerlistTmp.c_str(), FALSE );
 	DeleteFile( "serverlisttmp" );
 	string destConfigFile = m_ExpServerCfg.path + "\\" + CLIENT_EXP_SERVER_CONFIG;
	CIniFile file;
	if( file.Create( destConfigFile.c_str() ) )
	{
		if( m_ExpServerCfg.expFlag == 1 )
		{
			file.WriteInteger( "ExpServerConfig","ExpFlag",2 );
		}else if( m_ExpServerCfg.expFlag == 2 )
		{
			file.WriteInteger( "ExpServerConfig","ExpFlag",1 );
		}
		//当前目录
		char dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,dir);
		file.WriteString("ExpServerConfig","Path",dir);
		file.Save();
		DesDecryptFile( destConfigFile.c_str(), destConfigFile.c_str(), CLIENT_JIAMI) ;
	}
	file.Close();	 
}

void CLauncherDlg::DoFinishWork()
{
	m_btnEnterGame.EnableWindow( TRUE );
	WriteSrcServerList();
	WriteDestServerList();

	string launcherPath = m_ExpServerCfg.path +"\\"+ "launcher.exe";
	ShellExecute( NULL, "open", launcherPath.c_str(), "", m_ExpServerCfg.path.c_str(), SW_SHOWNORMAL );
	::exit(0);
	m_bIsCopyFile = false;
}

void CLauncherDlg::CheckSingleInstance()
{
	m_hInstMutex = CreateMutex(NULL, FALSE, "{AFFA60C6-9EE3-4a5c-AD1E-A86162E944DF}" );
	if (GetLastError() != ERROR_SUCCESS)
	{
		::MessageBoxW( this->GetSafeHwnd(), L"同时运行的一个游戏启动程序", L"提示", MB_OK );
		if (m_hInstMutex != NULL)
		{
			CloseHandle(m_hInstMutex);
			m_hInstMutex = NULL;
		}
		::exit(0);
	}
}

void CLauncherDlg::OnDocumentNewWindow1( LPDISPATCH* ppDisp, BOOL* cancel, unsigned long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl )
{
	OpenUrl(bstrUrl);
	*cancel = VARIANT_TRUE;
}