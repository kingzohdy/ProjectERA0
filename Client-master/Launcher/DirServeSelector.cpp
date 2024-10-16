#include <assert.h>
#include <afxsock.h>
#include <string>
#include <stdio.h>
#include <list>
#include "Launcher.h"
#include "StdAfx.h"
#include "DirServeSelector.h"
#include "LauncherDlg.h"
#include "LauncherLayerDlg.h"
#include "cs_net.h"
#include "XMLData.h"
#include <mmsystem.h>
#include <WinInet.h>
#include <algorithm>
#include "parseXML.h"

using std::string;
using namespace Launcher;

const string DIRCONFIG_PATH		= "DirSvr.xml";

#define MAX_PING_NUM	0xFFFFFFFE

LauncherDirBigWorldInfo::LauncherDirBigWorldInfo()
{
	m_Name[0] = 0;
}

const char* LauncherDirBigWorldInfo::getName()
{
	return m_Name;
}

int LauncherDirBigWorldInfo::getNumWorld()
{
	return (int)m_Worlds.size();
}

LauncherDirWorldInfo &LauncherDirBigWorldInfo::getIthWorld(int i)
{
	assert(size_t(i) < m_Worlds.size());
	return m_Worlds[i];
}

void LauncherDirBigWorldInfo::addWorldInfo(const CSDIRWORLDINFO &src)
{
	LauncherDirWorldInfo info;
	strncpy_s(info.name, src.WorldName,WORLD_NAME_LEN);
	strcpy_s(info.ip, src.IP);
	info.recommend	= (src.StatFlag>>4)!=0;	
	info.busy		= (src.StatFlag&0xf);
	info.port		= NTOHS(src.Port);
	info.nWorldID	= src.WorldID;
	m_Worlds.push_back(info);
}

void LauncherDirBigWorldInfo::addWorldInfo( const LauncherDirWorldInfo& src )
{
	m_Worlds.push_back( src );
}

void LauncherDirBigWorldInfo::updateWorldInfo( const CSDIRLATESTINFO& src )
{
	for( size_t i = 0, size = m_Worlds.size(); i < size; i++ )
	{
		if( m_Worlds[i].nWorldID == src.WorldID )
		{
			m_Worlds[i].recommend	= (src.StatFlag>>4)!=0;	
			m_Worlds[i].busy		= (src.StatFlag&0xf);
			strcpy_s( m_Worlds[i].ip, src.IP );
			m_Worlds[i].port		= NTOHS(src.Port);
			return;
		}
	}
}

DirServeSelector::DirServeSelector(void)
{
	m_nBestPing = MAX_PING_NUM;
}

void DirServeSelector::initDirInfo(const CSDIRWORLDLIST &dirinfo)
{
	if (m_BigWorlds.size() != 0)
	{
		m_BigWorlds.clear();
	}
	for(int i=0; i<dirinfo.BigWorldNum; i++)
	{
		LauncherDirBigWorldInfo pbiginfo;
		const CSDIRBIGWORLD& svrBigWorldInfo = dirinfo.BigWorldList[i];
		strncpy(pbiginfo.m_Name, svrBigWorldInfo.BigWorldName,64);
		pbiginfo.m_strUrl = svrBigWorldInfo.URL;
		pbiginfo.m_uStatusFlag = svrBigWorldInfo.StatFlag;
		for(int j=0; j<dirinfo.WorldNum; j++)
		{
			const CSDIRWORLDINFO &src = dirinfo.ListInfoList[j];
			if(src.BigWorldIdx == i) pbiginfo.addWorldInfo(src);
		}
		m_BigWorlds.push_back(pbiginfo);
		if( !pbiginfo.m_strUrl.empty() )
		{
			m_vecUrl.push_back( pbiginfo.m_strUrl );
		}		
	}
	m_vecUrl.resize(std::unique(m_vecUrl.begin(),m_vecUrl.end()) - m_vecUrl.begin() );

	if( m_vecUrl.size() == 1 )
	{
		m_strBestUrl = m_vecUrl[0];
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG,UI_COMMAND_CONNECT_DISERVE_SUCC );
	}else
	{
		m_strBestUrl = m_vecUrl[0];
		m_nBestPing = MAX_PING_NUM;
		for( size_t i = 0 ; i < m_vecUrl.size() ; i++ )
		{
			CPing* ping = new CPing( m_vecUrl[i].c_str(),this );
			m_vecPing.push_back( ping );
			ping->Open();
		}
	}
	SaveLocalSvr();
}

bool DirServeSelector::tryRequestDirInfo()
{
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it == NULL )
	{
		DWORD dw = GetLastError();
		return false;
	}
	HINTERNET it2 = InternetOpenUrl( it,CLauncherDlg::getNetConfig().m_DirUrl.c_str(),NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
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

	CSDIRWORLDLIST dirInfo;
	if( !parseXmlData((char*)CLauncherDlg::getMetaBuf(),"CSDirWorldList",buf,(char*)&dirInfo,sizeof(CSDIRWORLDLIST) ) )
	{
		delete buf;
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}
	initDirInfo(dirInfo);
	delete buf;
	InternetCloseHandle(it);
	InternetCloseHandle(it2);
	return true;
}

bool DirServeSelector::requestDirInfo()
{
	if( tryRequestDirInfo() )
	{
		return true;
	}
	if( LoadLocalSvr() )
	{
		return true;
	}
	tryLoadDirFromDirSever();
	return true;
}

bool DirServeSelector::LoadLocalSvr()
{
	m_BigWorlds.clear();
	m_strBestUrl.clear();
	m_vecUrl.clear();
	XMLData xmldata;
	if( !xmldata.loadFile( DIRCONFIG_PATH ) )
	{
		return false;
	}

	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.iterateChild();
	while( !pChildNode.isNull() )
	{
		if( stricmp(pChildNode.getName(),"BestUrl") == 0 )
		{
			if( pChildNode.attribToString("Url") != NULL )
			{
				m_strBestUrl = pChildNode.attribToString("Url");
			}
		}
		if( _stricmp( pChildNode.getName(), "BigWorld" ) == 0 )
		{
			LauncherDirBigWorldInfo biginfo;
			strncpy_s( biginfo.m_Name, pChildNode.attribToString("name"), 64 );
			biginfo.m_uStatusFlag = pChildNode.attribToInt("flag");
			XMLNode pBigWorldChild = pChildNode.iterateChild();
			int num = pChildNode.attribToInt( "count" );
			biginfo.m_strUrl = pChildNode.attribToString( "url" );
			for ( int i = 0; i < num; i++ )
			{
				if ( _stricmp( pBigWorldChild.getName(), "DirWorld" ) == 0 )
				{
					LauncherDirWorldInfo worldInfo;
					worldInfo.busy		= pBigWorldChild.attribToInt("busy");
					worldInfo.port		= pBigWorldChild.attribToInt("port");
					worldInfo.recommend	= pBigWorldChild.attribToInt("recommend")==0?false:true;
					worldInfo.nWorldID	= pBigWorldChild.attribToInt("worldid");
					strncpy_s( worldInfo.name, pBigWorldChild.attribToString("name"), WORLD_NAME_LEN );
					strncpy_s( worldInfo.ip, pBigWorldChild.attribToString("ip"), 32 );
					biginfo.addWorldInfo( worldInfo );
				}
				pBigWorldChild = pChildNode.iterateChild(pBigWorldChild);
			}
			m_BigWorlds.push_back( biginfo );
			if( !biginfo.m_strUrl.empty() )
			{
				m_vecUrl.push_back( biginfo.m_strUrl );
			}
		}		
		pChildNode = pRootNode.iterateChild(pChildNode);
	}

	m_vecUrl.resize(std::unique(m_vecUrl.begin(),m_vecUrl.end()) - m_vecUrl.begin() );
	if( m_vecUrl.size() == 1 )
	{
		m_strBestUrl = m_vecUrl[0];
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG,UI_COMMAND_CONNECT_DISERVE_SUCC );
	}else
	{
		m_strBestUrl = m_vecUrl[0];
		m_nBestPing = MAX_PING_NUM;
		for( size_t i = 0 ; i < m_vecUrl.size() ; i++ )
		{
			CPing* ping = new CPing( m_vecUrl[i].c_str(),this );
			m_vecPing.push_back( ping );
			ping->Open();
		}
	}
	return true;
}

void DirServeSelector::SaveLocalSvr()
{
	TiXmlDocument doc;
	std::list< TiXmlElement* > ListElemnt;

	TiXmlElement* root = new TiXmlElement( "LoclDirSvr" );
	doc.LinkEndChild( root );
	ListElemnt.push_back( root );

	for( size_t i = 0; i < m_BigWorlds.size(); i++ )
	{
		TiXmlElement* pBigWorldElement = new TiXmlElement( "BigWorld");
		root->LinkEndChild(pBigWorldElement);
		pBigWorldElement->SetAttribute( "flag",		m_BigWorlds[i].m_uStatusFlag );
		pBigWorldElement->SetAttribute( "name",		m_BigWorlds[i].getName() );
		pBigWorldElement->SetAttribute( "count",	m_BigWorlds[i].getNumWorld() );
		pBigWorldElement->SetAttribute( "url",		m_BigWorlds[i].m_strUrl.c_str() );
		ListElemnt.push_back( pBigWorldElement );

		for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
		{
			LauncherDirWorldInfo& DirWorld = m_BigWorlds[i].getIthWorld( j );
			TiXmlElement* pDirWorldElement = new TiXmlElement( "DirWorld");
			pBigWorldElement->LinkEndChild(pDirWorldElement);

			pDirWorldElement->SetAttribute( "name",			string( DirWorld.name ).c_str() );
			pDirWorldElement->SetAttribute( "recommend",	DirWorld.recommend );
			pDirWorldElement->SetAttribute( "busy",			DirWorld.busy );
			pDirWorldElement->SetAttribute( "ip",			string( DirWorld.ip ).c_str() );
			pDirWorldElement->SetAttribute( "port",			DirWorld.port );
			pDirWorldElement->SetAttribute( "worldid",		DirWorld.nWorldID );
			ListElemnt.push_back( pDirWorldElement );
		}
	}
	doc.SaveFile( DIRCONFIG_PATH.c_str() );
}

DirServeSelector::~DirServeSelector(void)
{
	for( size_t i = 0 ; i < m_vecPing.size() ; i++ )
	{
		if( m_vecPing[i] != NULL )
		{
			delete m_vecPing[i];
			m_vecPing[i] = NULL;
		}
	}
	m_BigWorlds.clear();
}

const char* DirServeSelector::getBestUrl()
{
	return m_strBestUrl.c_str();
}

void DirServeSelector::OnResult( LPCSTR szHost, DWORD dwIntervel )
{
	LockFunctor lock( &m_Mutex );
	if( dwIntervel >= 0 && dwIntervel < m_nBestPing )
	{
		m_nBestPing = dwIntervel;
		m_strBestUrl = szHost;
	}

	std::vector<std::string>::iterator it = m_vecUrl.begin();
	for( ; it != m_vecUrl.end() ; it ++ )
	{
		if( *it == szHost )
		{
			m_vecUrl.erase( it );
			break;
		}
	}

	if( m_vecUrl.empty() )
	{
		theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG,UI_COMMAND_CONNECT_DISERVE_SUCC );
	}
}

int net_clt_han_create_memEx(unsigned char *pmetabuffer,const char *pszConndIP,  unsigned short unPort, LPNETCLTHANDLE *ppNetCltHandle)
{
	return net_clt_han_create_mem(pmetabuffer, (char*)pszConndIP, unPort, ppNetCltHandle);
}

int SendDirNetMessage( NETCLTHANDLE *pnetcl,int msgid, const tagCSPkgBody &msgbody )
{
	CSPKG pkg;
	pkg.Head.Magic = PROTOCOL_MAGIC;
	pkg.Head.Cmd = msgid;
	pkg.Body = msgbody;

	if( pnetcl == NULL )
	{
		return -1;
	}

	if( 0 > net_clt_send( pnetcl, &pkg, 10 ) )
	{
		return -1;
	}
	return 0;
}

void DirServeSelector::onTime()
{
	bool bHave = false;
	for( int i = 0 ; i < (int)m_vecNetCltHandle.size() ; i++ )
	{	
		if( m_vecNetCltHandle[i] != NULL )
		{
			bHave = true;
			dispatchDirNetCL( m_vecNetCltHandle[i], i );
		}		
	}
	if( !bHave )
	{
		m_vecNetCltHandle.clear();
		CLauncherLayerDlg* pDlg = (CLauncherLayerDlg*) theApp.m_pMainWnd;
		pDlg->m_dlg.KillTimer( UI_NET_DIR_DISPATCH );
		m_vecUrl.resize(std::unique(m_vecUrl.begin(),m_vecUrl.end()) - m_vecUrl.begin() );
		if( m_vecUrl.size() == 1 )
		{
			m_strBestUrl = m_vecUrl[0];
			theApp.m_pMainWnd->SendMessage( WM_COMMAND, CUSTOM_BUTTON_MSG,UI_COMMAND_CONNECT_DISERVE_SUCC );
		}else
		{
			m_strBestUrl = m_vecUrl[0];
			m_nBestPing = MAX_PING_NUM;
			for( size_t i = 0 ; i < m_vecUrl.size() ; i++ )
			{
				CPing* ping = new CPing( m_vecUrl[i].c_str(),this );
				m_vecPing.push_back( ping );
				ping->Open();
			}
		}
	}	
}

bool DirServeSelector::tryLoadDirFromDirSever()
{
	for( size_t i = 0 ; i < m_vecNetCltHandle.size() ; i++ )
	{
		if( m_vecNetCltHandle[i] != NULL )
		{
			net_clt_han_destroy( &m_vecNetCltHandle[i] );
			m_vecNetCltHandle[i] = NULL;
		}
	}
	m_vecNetCltHandle.clear();
	m_strBestUrl.clear();
	m_vecUrl.clear();

	const GameNetCfg& cfg = CLauncherDlg::getNetConfig();
	for( size_t i = 0 ; i < cfg.m_vDirConfig.size() ; i++ )
	{
		const LauncherNetConfigData& configData = cfg.m_vDirConfig[i];
		NETCLTHANDLE* pHandle;
		if( net_clt_han_create_memEx( CLauncherDlg::getMetaBuf(),
			configData.m_strIp.c_str(),
			configData.m_nPort, &pHandle ) < 0 )
		{
			for( int j = 0 ; j < configData.m_vData.size() ; j ++ )
			{
				if( net_clt_han_create_memEx( CLauncherDlg::getMetaBuf(),
					configData.m_vData[j].ip.c_str(),
					configData.m_vData[j].port, &pHandle ) >= 0 )
				{
					break;
				}
			}
		}
		if( pHandle != NULL )
		{
			tagCSPkgBody body;
			body.DirReq.LastVersion = 0;
			SendDirNetMessage( pHandle,DIR_REQ, body );
			m_vecNetCltHandle.push_back( pHandle );
			CLauncherLayerDlg* pDlg = (CLauncherLayerDlg*) theApp.m_pMainWnd;
			pDlg->m_dlg.SetTimer( UI_NET_DIR_DISPATCH,100,NULL );
		}
	}
	return true;
}

void DirServeSelector::dispatchDirNetCL( NETCLTHANDLE *pnetcl, int nIdx )
{
	CSPKG	pkg;
	int ret = net_clt_recv(pnetcl, &pkg, 0);
	if(ret <= 0)
	{
		return;
	}
	HandleDirServerMsg( pkg.Head.Cmd, pkg.Body, nIdx );
}

void DirServeSelector::HandleDirServerMsg( int msgid, const tagCSPkgBody& msgbody, int nIdx )
{
	if(DIR_RES == msgid)
	{
		switch( msgbody.DirRes.Result )
		{
		case DIR_SUCC:
			{
				const CSDIRWORLDLIST& worldList = msgbody.DirRes.DirInfo.WorldList;
				for( int i = 0 ;i < worldList.BigWorldNum ; i ++ )
				{
					m_vecUrl.push_back( worldList.BigWorldList[i].URL );
				}				
			}
			break;
		}
		if( m_vecNetCltHandle[nIdx] != NULL )
		{
			net_clt_han_destroy( &m_vecNetCltHandle[nIdx] );
			m_vecNetCltHandle[nIdx] = NULL;
		}
	}
}
