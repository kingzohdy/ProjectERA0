#include <assert.h>
#include "StdAfx.h"
#include "VersionVerifier.h"
#include "LauncherDlg.h"
#include "cs_net.h"
#include "Launcher.h"
#include "des.h"
#include "CIniFile.h"
#include "LauncherLayerDlg.h"
#include <WinInet.h>
#include "parseXML.h"

const int VER_DISCONNECT_FLAG	= -1;
const string DIRCONFIG_PATH		= "DirSvr.xml";

VersionVerifier::VersionVerifier(void)
{
	m_versionInfo.m_nIsUp = VERSION_NOT_UPDATA;
	m_hNetCltHandle = NULL;
}

unsigned int VersionVerifier::GetFileVersion(const char* szFileName)
{
	char* pszDecryptBuffer	= getDecryptFileBuffer( szFileName, CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return 0;
	}

	char szVersion[512];
	CIniFile iniNewVer;
	iniNewVer.Prepare( pszDecryptBuffer );
	iniNewVer.GetString( "version", "ver", "0", szVersion, 512 );
	iniNewVer.Close();

	delete[] pszDecryptBuffer;

	return version_to_uint( ( char* )szVersion ); 
}

unsigned int VersionVerifier::getLocalVersion()
{
	// 先打开已经加密的文件
	string strEncryptFilePath	=/* string( curdir ) + */"version.data";
	unsigned int ver = GetFileVersion( strEncryptFilePath.c_str() );
	return ver;
}

static std::string versionIp;
static int versionPort;

DWORD WINAPI ConnectVersionFunction( void* data )
{
	VersionVerifier* pVer = (VersionVerifier*)data;
	if(	net_clt_han_create_mem(CLauncherDlg::getMetaBuf(), (char*)versionIp.c_str(), versionPort, &pVer->m_hNetCltHandle) == 0 )
	{
		if( pVer->m_hNetCltHandle != NULL )
		{
			tagCSPkgBody body;
			memset( &body, 0, sizeof(tagCSPkgBody) );
			body.VersionClt.VersionNum = pVer->getLocalVersion();
			pVer->m_versionInfo.m_nVersionLatest = body.VersionClt.VersionNum;
			CSPKG pkg;
			pkg.Head.Magic = PROTOCOL_MAGIC;
			pkg.Head.Cmd = VERSION_CLT;
			pkg.Body = body;
			net_clt_send( pVer->m_hNetCltHandle, &pkg, 10 );
			CLauncherLayerDlg* pDlg = (CLauncherLayerDlg*) theApp.m_pMainWnd;
			pDlg->m_dlg.SetTimer( UI_NET_VERSION_DISPATCH,100,NULL );
		}
	}else{
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_FAIL_VER );
	}
	return 0;
}

void VersionVerifier::OnTime()
{
	if( m_hNetCltHandle )
	{
		CSPKG	pkg;
		int ret = net_clt_recv(m_hNetCltHandle, &pkg, 0);
		if(ret <= 0)
		{
			return;
		}
		HandleNetMsg(  pkg.Head.Cmd, pkg.Body );
		net_clt_han_destroy( &m_hNetCltHandle );
		CLauncherLayerDlg* pDlg = (CLauncherLayerDlg*) theApp.m_pMainWnd;
		pDlg->m_dlg.KillTimer( UI_NET_VERSION_DISPATCH );
	}
}

bool VersionVerifier::tryDownloadVersionInfo()
{
	if( CLauncherDlg::getNetConfig().m_VersionUrl.empty() )
	{
		return false;
	}
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it == NULL )
	{
		DWORD dw = GetLastError();
		return false;
	}
	HINTERNET it2 = InternetOpenUrl( it,CLauncherDlg::getNetConfig().m_VersionUrl.c_str(),NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
	if( it2 == NULL )
	{
		InternetCloseHandle(it);
		return false;
	}
	size_t nTotalSize = 0;
	DWORD nLength = sizeof(nTotalSize);
	if( !HttpQueryInfo(it2, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &nTotalSize,
		&nLength, NULL) )
	{
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}
	char* buf = new char[ nTotalSize+1 ];
	buf[nTotalSize] = '\0';
	if( !InternetReadFile(it2,buf,nTotalSize,&nLength) || nTotalSize != nLength )
	{
		delete buf;
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}
	VERSION_XML* pVersion = new VERSION_XML;
	if( !parseXmlData((char*)CLauncherDlg::getMetaBuf(),"version_xml",buf,(char*)pVersion,sizeof(VERSION_XML) ) )
	{
		delete buf;
		delete pVersion;
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}
	delete buf;
	InternetCloseHandle(it);
	InternetCloseHandle(it2);

	if( 0 == pVersion->MinVersion || 0 == pVersion->NewVersion )
	{
		delete pVersion;
		return false;
	}	
	unsigned int ver = getLocalVersion();
	m_versionInfo.m_nVersionLatest = ver;
	if( ver < pVersion->MinVersion )
	{
		m_versionInfo.m_nIsUp = VERSION_OLD;
		m_versionInfo.m_nVersionLatest = pVersion->NewVersion;
		delete pVersion;
		return true;
	}
	
	m_versionInfo.m_nIsUp = VERSION_NOT_UPDATA;
	if( ver >= pVersion->NewVersion )
	{
		delete pVersion;
		return true;
	}

	bool bFound = false;
	// 优先查找跳包
	for(int i=0; i<(int)(pVersion->JumpCount); i++)
	{
		if ( ver >= version_to_uint(pVersion->JumpPack[i].Version_From) &&
			ver <= version_to_uint(pVersion->JumpPack[i].JumpMaxVer))
		{
			bFound = true;
			m_versionInfo.m_nIsUp = VERSION_UPDATA;
			m_versionInfo.m_nUpSize = pVersion->JumpPack[i].Version_Size;
			m_versionInfo.m_nVersionFrom = version_to_uint(pVersion->JumpPack[i].Version_From);
			m_versionInfo.m_nVersionTo = version_to_uint(pVersion->JumpPack[i].Version_Jump);
			m_versionInfo.m_strMD5 = pVersion->JumpPack[i].MD5;
			m_versionInfo.m_strUpName = pVersion->JumpPack[i].Version_Name;
			m_versionInfo.m_strUpAddress = pVersion->JumpPack[i].UpAddr;
			m_versionInfo.m_nVersionLatest = pVersion->NewVersion;
			break;
		}
	}

	if( !bFound )
	{
		for (int i=0; i<(int)(pVersion->Count); i++)
		{
			unsigned int iVersion = version_to_uint(pVersion->UpPack[i].Version_From);
			if (iVersion == (unsigned int)(-1))
			{
				continue;
			}

			// 要更新的版本
			if(iVersion == ver)
			{
				bFound = true;
				m_versionInfo.m_nIsUp = VERSION_UPDATA;
				m_versionInfo.m_nUpSize = pVersion->UpPack[i].Version_Size;
				m_versionInfo.m_nVersionFrom = iVersion;
				m_versionInfo.m_nVersionTo = version_to_uint(pVersion->UpPack[i].Version_To);
				m_versionInfo.m_strMD5 = pVersion->UpPack[i].MD5;
				m_versionInfo.m_strUpName = pVersion->UpPack[i].Version_Name;
				m_versionInfo.m_strUpAddress = pVersion->UpPack[i].UpAddr;
				m_versionInfo.m_nVersionLatest = pVersion->NewVersion;
				break;
			}
		}
	}
	delete pVersion;
	return bFound;
}

void VersionVerifier::tryRequestVersionInfo()
{
	if( tryDownloadVersionInfo() )
	{
		OnGetVersionInfo();
	}else{
		const LauncherNetConfigData& verConfig = CLauncherDlg::getNetConfig().m_verConfig;
		hostent* remoteHost;
		remoteHost = gethostbyname( verConfig.m_strIp.c_str() );
		if( remoteHost != NULL )
		{
			versionIp = inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list);
			versionPort = verConfig.m_nPort ;
			for( size_t i = 0 ; i < verConfig.m_vData.size() ; i ++ )
			{
				if( versionIp == verConfig.m_vData[i].ip )
				{
					versionPort = verConfig.m_vData[i].port;
					break;
				}
			}
		}else if( !verConfig.m_vData.empty() ){
			size_t idx = rand()%( verConfig.m_vData.size() );
			versionIp = verConfig.m_vData[idx].ip;
			versionPort = verConfig.m_vData[idx].port;		
		}

		DWORD threadid;
		HANDLE threadHandle = CreateThread(NULL, 0, ConnectVersionFunction, this, 0, &threadid);
		CloseHandle( threadHandle );
	}
	return;
}

bool VersionVerifier::requestVersionInfo()
{
	tryRequestVersionInfo();
	return true;
}

int VersionVerifier::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	int	nRet = 0;
	if ( msgid == VERSION_SVR )
	{
		const CSVERSIONSVR& launchData	= msgbody.VersionSvr;
		m_versionInfo.m_nIsUp			= launchData.IsUp;
		m_versionInfo.m_nUpSize			= launchData.UpSize;
		m_versionInfo.m_nVersionFrom	= launchData.VersionFrom;
		m_versionInfo.m_nVersionTo		= launchData.VersionTo;
		m_versionInfo.m_strMD5			= launchData.MD5;
		m_versionInfo.m_strUpName		= launchData.UpName;
		m_versionInfo.m_strUpAddress	= launchData.UpAddr;
		m_versionInfo.m_nVersionLatest	= launchData.VersionNew;

		OnGetVersionInfo();
	}
	return nRet;
}

VersionVerifier::~VersionVerifier(void)
{
}

const tagVersionInfo& VersionVerifier::getLatestVersionInfo() const
{
	return m_versionInfo;
}

void VersionVerifier::OnGetVersionInfo()
{
	//将版本信息写到本地客户端的配置文件中
	switch ( m_versionInfo.m_nIsUp )
	{
	case VERSION_UPDATA:
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_NEED_UPDATE );
		break;

	case VERSION_NOT_UPDATA:
		m_versionInfo.m_nVersionLatest = getLocalVersion();
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_LATEST_VERSION );
		break;

	case VERSION_OLD:
		// TODO: 加一个跳转到更新网页的操作
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG, UI_COMMAND_OLD_VERSION );
		break;

	default:
		assert( !"非法信息!" );
		break;
	}
}
