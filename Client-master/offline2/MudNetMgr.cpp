#include "MudNetMgr.h"
#include "cs_net.h"
#include "tinyxml.h"
#include "OgreXMLData.h"
#include "ServerTimer.h"
#include "../UILib/ui_gameui.h"
#include <MMSystem.h>
#include "OgreScriptLuaVM.h"
#include "OfflineManager.h"
#include "OfflineMainPlayer.h"
#include "RelationManager.h"
#include <iptypes.h>
#include <iphlpapi.h>
#include <WinInet.h>
#include "parseXML.h"
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment( lib, "Wininet.lib" )

using namespace std;
using namespace Ogre;

#define DIRCONFIG_PATH		"DirSvr.xml"
#define CONNECT_SERTIME		500

void getMacString( char* szMacAdress )
{
	sprintf(szMacAdress,"");
	PIP_ADAPTER_INFO pAdapterInfo;
	DWORD dwRetVal = 0;
	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		sprintf(szMacAdress,"%02x-%02x-%02x-%02x-%02x-%02x",pAdapterInfo->Address[0],pAdapterInfo->Address[1],pAdapterInfo->Address[2],
			pAdapterInfo->Address[3],pAdapterInfo->Address[4],pAdapterInfo->Address[5]);
	}
	free(pAdapterInfo);
}

void OfflineSendMessage(int msgid, const tagCSPkgBody &msgbody)
{
	SendNetMessage( 0, msgid, msgbody);
}

const char *DirBigWorldInfo::getName()
{
	return m_Name;
}

int DirBigWorldInfo::getNumWorld()
{
	return m_Worlds.size();
}

unsigned int DirBigWorldInfo::getFlag()
{
	return m_uStatusFlag;
}

DirWorldInfo &DirBigWorldInfo::getIthWorld(int i)
{
	return m_Worlds[i];
}

void DirBigWorldInfo::addWorldInfo(const CSDIRWORLDINFO &src)
{
	DirWorldInfo info;
	strcpy_s(info.name, src.WorldName);
	strcpy_s(info.ip, src.IP );
	info.recommend	= (src.MudStatFlag>>4)!=0;	
	info.busy		= (src.MudStatFlag&0xf);
	info.port		= NTOHS(src.Port);
	info.worldid	= src.WorldID;
	info.RegionID	= src.RegionID;
	strcpy_s(info.AuthIP, src.AuthIP);
	info.AuthPort	= NTOHS(src.AuthPort);
	strcpy_s(info.mudIp,src.MudIP);
	info.mudPort	= NTOHS(src.MudPort);
	m_Worlds.push_back(info);
}

void DirBigWorldInfo::addWorldInfo( const DirWorldInfo& src )
{
	m_Worlds.push_back(src);
}

MudNetMgr::MudNetMgr(HWND hwnd)
{
	m_hwnd					= hwnd;
	m_nBigWorldSelectindex	= -1;
	m_nWorldSelectindex		= -1;
	m_nRfreshTime			= 0;
	m_nOffline				= false;
	m_BigWorlds.clear();
}

MudNetMgr::~MudNetMgr(void)
{
	delete m_pNetMgr;
}

void MudNetMgr::Dispatch( unsigned int dtick ) 
{
	if( m_pNetMgr != NULL )
	{
		m_pNetMgr->Dispatch();
	}
	if (OfflineMainPlayer::getSingleton().getGameState() == LOGIN_STATE  )
	{
		m_nRfreshTime = m_nRfreshTime + dtick;
		if ( m_nRfreshTime > 30000 )
		{
			ReFreshServer();
			m_nRfreshTime = 0;
		}
	}
	else
	{
		static unsigned int HeartBeatTick=0;
		HeartBeatTick = HeartBeatTick + dtick;
		if ( HeartBeatTick >= 30000)
		{
			if ( !m_nOffline )
			{
				requestHeartHeat();
				HeartBeatTick = 0;
				m_nOffline	  = true;
			}
			else
			{
				OfflineManager::getSingleton().SendUIEvent("GE_OFFLINE");
			}
		}
	}
}


void MudNetMgr::initNetMgr()
{
	m_pNetMgr = INetworkManager::Create();
	//注册消息
	m_pNetMgr->RegisterMsgHandler( DIR_RES,this );
	m_pNetMgr->RegisterMsgHandler( ACC_LOGIN_RES,this );
	m_pNetMgr->RegisterMsgHandler(MUD_CHAT_CHANNEL_SET_RES,this);
	m_pNetMgr->RegisterMsgHandler(MUD_OFFLINE_EXP_FULL_INFO,this);
	m_pNetMgr->RegisterMsgHandler(MUD_OFFLINE_EXP_TAKE_RES,this);
	m_pNetMgr->RegisterMsgHandler(SYS_TIP,this);
	m_pNetMgr->RegisterMsgHandler(KICKOFF_INFO,this);
	m_pNetMgr->RegisterMsgHandler(HEARTBEAT_RES,this);
	m_bStartfb=false;
}

int MudNetMgr::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	string strtemp;
	switch( msgid )
	{
	case DIR_RES:
		{
			const CSDIRRES& res = msgbody.DirRes;
			onDirInfoResponse(res);
		}
		break;
	case ACC_LOGIN_RES:
		{
			if( msgbody.AccLoginRes.Result != 0 )
			{
				m_nLoginResult = msgbody.AccLoginRes.Result;
				OfflineManager::getSingleton().SendUIEvent("GE_LOGIN_RESULT");
				//登陆失败
				return -1;
			}
		}
		break;
	case MUD_OFFLINE_EXP_FULL_INFO:
		{
			if (!RelationManager::getSingleton().IsFriend(msgbody.MudOfflineExpFullInfo.RoleName))
			{
				break;
			}
			if (msgbody.MudOfflineExpFullInfo.Type == 1)
			{
				strtemp=msgbody.MudOfflineExpFullInfo.RoleName;
				strtemp=strtemp+"的经验已经满了，要偷的赶紧啊！";
			}
			else
			{
				strtemp=msgbody.MudOfflineExpFullInfo.RoleName;
				strtemp=strtemp+"的经验已经被偷掉咯";
			}
			m_tiplist.push_back(strtemp);
		}
		break;
	case MUD_OFFLINE_EXP_TAKE_RES:
		{
			if (msgbody.MudOfflineExpTakeRes.Result==0)
			{
				strtemp="偷取经验成功";
			}
			else
			{
			   strtemp="偷取经验失败";
			}
			m_tiplist.push_back(strtemp);
		}
		break;
	case SYS_TIP:
		{
		}
		break;
	case SYS_MSG:
		{
			strtemp=msgbody.SysMsg.Msg;
			m_tiplist.push_back(strtemp);
		}
		break;
	case KICKOFF_INFO:
		{
			OfflineManager::getSingleton().SendUIEvent("GE_KICK_OFF");
			addMessagestr( msgbody.KickoffInfo.Msg, 1 );
			return -1;

		}
		break;
	case MUD_CHAT_CHANNEL_SET_RES:
		{
		}
		break;
	case HEARTBEAT_RES:
		{
			m_nOffline = false;
		}
		break;
	default:
		break;
	}
	return 0;
}

bool MudNetMgr::LoadLocalSvr()
{
	XMLData xmldata;
	if( !xmldata.loadFile( DIRCONFIG_PATH ) )
	{
		return false;
	}
	// 将旧的内容清空 Add by Kevin [11/24/2009]
	if (m_BigWorlds.size() != 0)
	{
		m_BigWorlds.clear();
	}

	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.iterateChild();
	while( !pChildNode.isNull() )
	{
		if( stricmp( pChildNode.getName(), "BigWorld" ) == 0 )
		{
			DirBigWorldInfo pbiginfo;
			strncpy( pbiginfo.m_Name, pChildNode.attribToString("name"), 64 );
			pbiginfo.m_uStatusFlag = pChildNode.attribToInt("flag");			
			if ( pChildNode.attribToString( "url" ) == NULL )
			{
				return false;
			}
			pbiginfo.m_strUrl = pChildNode.attribToString( "url" );
			if ( pChildNode.attribToString( "server" ) != NULL )
			{
				XMLNode pBigWorldChild = pChildNode.iterateChild();
				int num = pChildNode.attribToInt("count");
				for ( int i = 0; i < num; i++ )
				{
					if( stricmp( pBigWorldChild.getName(), "DirWorld" ) == 0 )
					{
						DirWorldInfo worldInfo;
						worldInfo.busy		= 0;
						worldInfo.port		= pBigWorldChild.attribToInt("port");
						worldInfo.recommend	= pBigWorldChild.attribToInt("recommend") == 0 ? false:true;
						worldInfo.worldid	= pBigWorldChild.attribToInt("worldid");
						strncpy( worldInfo.name, pBigWorldChild.attribToString("name"), WORLD_NAME_LEN );
						strncpy( worldInfo.ip, pBigWorldChild.attribToString("ip"), 32 );
						pbiginfo.addWorldInfo( worldInfo );
					}
					pBigWorldChild = pChildNode.iterateChild(pBigWorldChild);
				}
				m_BigWorlds.push_back(pbiginfo);				
			}			
		}		
		pChildNode = pRootNode.iterateChild(pChildNode);
	}
	return true;
}

void MudNetMgr::initDirInfo( const CSDIRWORLDLIST &dirinfo )
{
	if (m_BigWorlds.size() != 0)
	{
		m_BigWorlds.clear();
	}
	for(int i=0; i<dirinfo.BigWorldNum; i++)
	{
		DirBigWorldInfo pbiginfo;
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
	}
}

void MudNetMgr::onDirInfoResponse(const CSDIRRES& res)
{
	if( res.Result == DIR_SUCC )
	{
		OfflineManager::getSingleton().SendUIEvent("GE_LOGIN_DIR");
	}
}

void MudNetMgr::requestSendChat( int channel, const char* szName, const char* szContent,int onlinetype)
{
	
	tagCSPkgBody msgbody;
	memset( &msgbody,0, sizeof(msgbody));
	msgbody.ChatReq.Type = channel;
	msgbody.ChatReq.OnlineType= MUD_ONLINE;
	if( msgbody.ChatReq.Type == CHAT_FRIEND || msgbody.ChatReq.Type == CHAT_PRIVATE )
	{
		strncpy( msgbody.ChatReq.ToName, szName, sizeof(msgbody.ChatReq.ToName) );		
		msgbody.ChatReq.OnlineType = RelationManager::getSingleton().getOnlineinfo( msgbody.ChatReq.ToName );		
	}else{
		msgbody.ChatReq.ToName[0] = 0;
	}
	strncpy( msgbody.ChatReq.Content, szContent, sizeof(msgbody.ChatReq.Content) );
	msgbody.ChatReq.LinkLen=0;
	OfflineSendMessage( CHAT_REQ, msgbody);	
}


//////////////////////////////////////////////////////////////////////////
void MudNetMgr::requestLoginGameServer( const char *ip, unsigned short port, const char *username, 
								   const char *passwd, int worldId, const char* worldName)
{
	// 同上次链接时间间隔需超过6秒
	static int nStartTime = 0;
	if( timeGetTime() - nStartTime < CONNECT_SERTIME )
	{
		Messagestruct Messagetemp;
		Messagetemp.style	= 0;
		Messagetemp.content = "两次连续登陆时间的最小间隔为6秒！";
		m_messageboxlist.push_back( Messagetemp );

		return;
	}else{
		nStartTime = timeGetTime();
	}

	m_pNetMgr->Release();

	int nDirIdx  = -1;
	int nRet = 0;
	char szMac[128];
	
	getMacString(szMac);
	for( int i = 0; i < m_BigWorlds.size(); i++ )
	{
		for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
		{
			if( stricmp( m_BigWorlds[i].getIthWorld( j ).mudIp, ip ) == 0 && 
				m_BigWorlds[i].getIthWorld( j ).mudPort == port )
			{
				char szName[NAME_LEN];
				strcpy( szName, username );
				int nRet = m_pNetMgr->ConnectServer( m_BigWorlds[i].getIthWorld( j ).AuthIP, 
					m_BigWorlds[i].getIthWorld( j ).AuthPort,
					ip, 
					port,
					szName, 
					const_cast<char*>( passwd ),
					&nRet, 
					NULL,
					NULL,
					NULL,
					NULL,
					szMac);
				if( nDirIdx >= 0 )
				{					
					break;
				}
			}
		}
		if( nDirIdx >= 0 )
		{
			break;
		}
		OfflineManager::getSingleton().m_nLastSelectWorldId = worldId;

		if(nDirIdx < 0 ) 
		{
			char szScript[256];
			sprintf( szScript, "LoginError(%d)", nRet );
			OfflineManager::getSingleton().getScriptVM()->callString( szScript );
		}		
	}
}

struct MudData
{
	NetCltAuthInfo authInfo;
	char		   ip[64];
	unsigned short port;
	unsigned short mudport;
	tdr_longlong RoleID;
};

bool MudNetMgr::requestLoginServerDirect(const char* pdirectstart)
{
	MudData data;
	FILE* f = NULL;
	if  (access(pdirectstart,0) != 0)
	{
		return false;
	}
	f = fopen(pdirectstart,"rb");
	fread(&data,sizeof(MudData),1,f);
	fclose(f);
	int nRet = 0;
	char szMac[128];
	getMacString(szMac);
	int nGameNetIdx	= m_pNetMgr->DirectConnectServer( (char*)&data.authInfo,data.ip,data.mudport,szMac,NULL,&nRet );
	if( nRet != 0 || nGameNetIdx < 0 )
	{
		return false;
	}
	m_DirectloginRoleID	=	data.RoleID;
	strncpy(m_szLastLoginIp,data.ip,32);
	m_uLastLoginPort = data.port;
	m_uLastLoginMudPort = data.mudport;
	DeleteFile(pdirectstart);
	m_bStartfb=true;
	return true;
}

void MudNetMgr::startZone()
{
	MudData data;
	get_clt_auth_han_info(&data.authInfo);
	strncpy(data.ip,m_szLastLoginIp,32);
	data.port		= m_uLastLoginPort;
	data.mudport	= m_uLastLoginMudPort; 
	data.RoleID	= OfflineMainPlayer::getSingleton().getRoleId();
	char pCtemp[100];
	strcpy(pCtemp,"directmud");
	strcat(pCtemp,data.authInfo.AccountLoginName);
	FILE* f = NULL;
	if (access(pCtemp,0) == 0)
	{
		DeleteFile(pCtemp);
	}
	f = fopen(pCtemp,"wb");
	fwrite(&data,sizeof(MudData),1,f);
	fclose(f);
	OfflineManager::getSingleton().end();
	SHELLEXECUTEINFO info;
	memset(&info,0,sizeof(SHELLEXECUTEINFO));
	/*info.lpVerb = "runas";
	
	info.nShow = SW_SHOW;*/
	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.lpParameters = (LPCSTR)pCtemp;
	info.lpFile = "Client.exe";
	
	if( ShellExecuteEx(&info) )
	{
		OfflineManager::getSingleton().end1();
	}else
	{
		DeleteFile(pCtemp);
	}
}


void MudNetMgr::requestLogout()
{
	tagCSPkgBody msgbody;
	msgbody.AccLogoutReq.Uin = atoi( OfflineMainPlayer::getSingleton().getName() );
	OfflineSendMessage( ACC_LOGOUT_REQ, msgbody);
}


DirBigWorldInfo& MudNetMgr::getBidWorldInfo(int nIndex)
{
	return m_BigWorlds[nIndex];
}

void MudNetMgr::requestHeartHeat()
{
	tagCSPkgBody body;
	CSTIME& serveTime = body.HeartBeatReq.SvrTime;
	ServerTimer::getSingleton().getServerTime( serveTime.BenchTimeSec, serveTime.BenchTimeUSec );
	OfflineSendMessage( HEARTBEAT_REQ, body);
}

int	MudNetMgr::getSevernums()
{
	return m_BigWorlds.size();
}

const char* MudNetMgr::getSeverName(int i)
{
	i--;
	if ( i>=m_BigWorlds.size() )
	{
		return NULL;
	}
	return m_BigWorlds[i].m_Name;
}

void MudNetMgr::setBigworldindex(const char* str)
{
	for (int i=0;i<m_BigWorlds.size();i++)
	{
		if (strcmp(m_BigWorlds[i].m_Name,str)==0)
		{
			m_nBigWorldSelectindex=i;
			return;
		}
	}
}

int	 MudNetMgr::getBigWorldindex()
{
	return m_nBigWorldSelectindex;
}

int MudNetMgr::getWorldNum(int i)//获取当前大区的下的服务器个数
{
	if ( i >= 0 && i < m_BigWorlds.size())
		return m_BigWorlds[i].m_Worlds.size();
	else
	return -1;
}

const char* MudNetMgr::getWorldName(int i,int j)
{
	return m_BigWorlds[i].m_Worlds[j-1].name;
}

int	MudNetMgr::getWorldState(int i,int j)
{
	return m_BigWorlds[i].m_Worlds[j-1].busy;
}

void MudNetMgr::setWorldindex(const char* str)
{
	int j=m_nBigWorldSelectindex;
	for (int i=0;i<m_BigWorlds[j].getNumWorld();i++)
	{
		if (strcmp(m_BigWorlds[j].m_Worlds[i].name,str)==0)
		{
			m_nWorldSelectindex=i;
			return;
		}
	}
}

int MudNetMgr::getWorldindex()
{
	return m_nWorldSelectindex;
}

bool MudNetMgr::isDirConnected()
{
	if ( m_BigWorlds.size() <= 0 )
	{
		return false;
	}

	if ( m_nBigWorldSelectindex < 0 )
	{
		return false;
	}

	if ( m_nBigWorldSelectindex > m_BigWorlds.size() -1 )
	{
		return false;
	}

	if ( m_BigWorlds[m_nBigWorldSelectindex].m_Worlds.size() <= 0)
	{
		return false;
	}

	if ( m_nWorldSelectindex < 0 )
	{
		return false;
	}

	if ( m_nWorldSelectindex > m_BigWorlds[m_nBigWorldSelectindex].m_Worlds.size() )
	{
		return false;
	}
	return true;
}

const char* MudNetMgr::getWorldIp()
{
	if ( isDirConnected() )
	{
		return m_BigWorlds[m_nBigWorldSelectindex].m_Worlds[m_nWorldSelectindex].mudIp;
	}
	return NULL;
}

int MudNetMgr::getWorldPort()
{
	if ( isDirConnected() )
	{
		return m_BigWorlds[m_nBigWorldSelectindex].m_Worlds[m_nWorldSelectindex].mudPort;	
	}
	return NULL;
}

int MudNetMgr::getWorldId()
{
	if ( isDirConnected() )
	{
		return m_BigWorlds[m_nBigWorldSelectindex].m_Worlds[m_nWorldSelectindex].worldid;
	}
	return NULL;
}

int	MudNetMgr::getSelectWorldState()
{
	if ( isDirConnected() )
	{
		return m_BigWorlds[m_nBigWorldSelectindex].m_Worlds[m_nWorldSelectindex].busy;
	}
	return -1;
}

const char* MudNetMgr::getSelectWorldName()
{
	if ( isDirConnected() )
	{
		return m_BigWorlds[m_nBigWorldSelectindex].m_Worlds[m_nWorldSelectindex].name;
	}
	return NULL;
}

const char* MudNetMgr::getSelectBigWorldName()
{
	if ( m_BigWorlds.size() >0 && m_nBigWorldSelectindex > 0 && m_nBigWorldSelectindex < m_BigWorlds.size() )
	{
		return m_BigWorlds[m_nBigWorldSelectindex].getName();
	}
	return NULL;
}

int	MudNetMgr::getChatmsgtype()
{
	return (int)m_ChatRes.Type;
}

const char*  MudNetMgr::getChatmsgrolename()
{
	return m_ChatRes.From;
}

const char*  MudNetMgr::getChatmsgContent()
{
	return m_ChatRes.Content;
}

void MudNetMgr::requestChannelSet(int channel, bool sign)
{
	tagCSPkgBody msgbody;
	msgbody.MudChatChannelSetReq.MudChatChannelInfo.ChatType=channel;
	msgbody.MudChatChannelSetReq.MudChatChannelInfo.Silence=sign;
	OfflineSendMessage( MUD_CHAT_CHANNEL_SET_REQ, msgbody);
}

int MudNetMgr::getTextTipsnums()
{
	return m_tiplist.size();
}

const char* MudNetMgr::getTextTip(int i)
{
	return m_tiplist[i-1].c_str();
}

void MudNetMgr::ClearTexttip()
{
	m_tiplist.clear();
}

const char* MudNetMgr::getMessagestr()
{
	return m_messageboxlist.begin()->content.c_str();
}

int MudNetMgr::getMessagestyle()
{
	return m_messageboxlist.begin()->style;
}

void MudNetMgr::addMessagestr(const char* str,int style)
{
	Messagestruct Messtemp;
	Messtemp.content = str;
	Messtemp.style   = style;
	m_messageboxlist.push_back(Messtemp);
}

int MudNetMgr::getMessageNums()
{
	return	m_messageboxlist.size();
}

void MudNetMgr::delMessage()
{
	m_messageboxlist.erase(m_messageboxlist.begin());
}

int	MudNetMgr::getLastSelectArea()
{
	int nWorldId = OfflineManager::getSingleton().m_nLastSelectWorldId;
	
	if( nWorldId != 0 )
	{
		for( size_t i = 0; i < m_BigWorlds.size(); i++ )
		{
			for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
			{
				if( m_BigWorlds[i].getIthWorld(j).worldid == nWorldId )
				{
					return i;
				}
			}
		}
	}
	return 0;
}

int	MudNetMgr::getLastSelectWorld()
{
	int nWorldId = OfflineManager::getSingleton().m_nLastSelectWorldId;
	
	if( nWorldId != 0 )
	{
		for( size_t i = 0; i < m_BigWorlds.size(); i++ )
		{
			for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
			{
				int bb = m_BigWorlds[i].getIthWorld(j).worldid;
				if( m_BigWorlds[i].getIthWorld(j).worldid == nWorldId )
				{
					return j;
				}
			}
		}
	}
	return 0;
}

void MudNetMgr::ReFreshServer()
{
	LoadDirFromUrl( m_cfg.dir_url );
}

bool MudNetMgr::LoadDirFromUrl( const char* szUrl )
{
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it == NULL )
	{
		return false;
	}
	HINTERNET it2 = InternetOpenUrl( it,szUrl,NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
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
	if( !parseXmlData( (char*)INetworkManager::getSingleton().getMetaBuf(),buf,(char*)&dirInfo,sizeof(CSDIRWORLDLIST) ) )
	{
		delete buf;
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}
	initDirInfo( dirInfo );

	OfflineManager::getSingleton().SendUIEvent("GE_LOGIN_DIR");
	delete buf;
	InternetCloseHandle(it);
	InternetCloseHandle(it2);
	return true;
}