
#include "CSNetManager.h"
#include "OgreLog.h"
#include "OgreDataStream.h"
#include "OgreResourceManager.h"
#include "OgreSceneManager.h"


CSNetManager::CSNetManager() : m_pCSMetaBuf(NULL), m_pDirServerNet(NULL), m_isBusy(false),m_Port(0)
{
	Ogre::DataStream *fp = Ogre::ResourceManager::getSingleton().openFileStream("db\\proto_cs.meta", true);
	if(fp)
	{
		size_t fsize = fp->size();
		m_pCSMetaBuf = (unsigned char *)malloc(fsize);
		fp->read(m_pCSMetaBuf, fsize);
		delete fp;

		re_confuse(m_pCSMetaBuf, (int)fsize);
	}

	strncpy(m_TdrPath, "db\\proto_cs.meta", sizeof(m_TdrPath));
	m_DisConnectHandlers = NULL;
	memset( m_Handlers, 0, sizeof(m_Handlers) );
	memset( m_pNetCLTable, 0, sizeof(m_pNetCLTable));
	m_IP[0] = '\0';
}

CSNetManager::~CSNetManager()
{
	Release();
	free(m_pCSMetaBuf);
}

int CSNetManager::dispatchDirNetCL(NETCLTHANDLE *pnetcl)
{
	CSPKG	pkg;
	int ret = net_clt_recv(pnetcl, &pkg, 0);

	if(ret <= 0)
	{
		return ret==0?0:-1;
	}

	// 打印消息日志
#ifndef BUILD_DEPLOY
	unsigned char kbuf[256];
	GetKeyboardState(kbuf);
	if( kbuf[VK_NUMLOCK] & 1)
	{
		static char szBuf[1024*4];
		static char buf[256];
		ZeroMemory( szBuf, 1024*4 );
		ZeroMemory( buf, 256 );
		if( net_clt_sprintf( pnetcl, &pkg, szBuf, 1024*4 ) >= 0 )
		{
			SYSTEMTIME st;
			GetLocalTime( &st );
			sprintf( buf, "s->c: %d-%d-%d %d:%d:%d:%d", 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
			LOG_SEVEREMSG(buf);
			LOG_SEVEREMSG(szBuf);
		}
	}
#endif
	//////////////////////////////////////////////////////////////////////////

	if( pkg.Head.Cmd > 256 )
	{
		LOG_WARNING("msg cmd error:%d", pkg.Head.Cmd);
		return -2;
	}

	if( m_Handlers[pkg.Head.Cmd] == NULL )
	{
		LOG_WARNING("recv wrong msg:%d", pkg.Head.Cmd);
		return -2;
	}

	if( m_Handlers[pkg.Head.Cmd]->HandleNetMsg(pkg.Head.Cmd, pkg.Body) < 0 )
	{
		LOG_WARNING("handle msg failed");
		return -2;
	}

	return 1;
}

int CSNetManager::dispatchNetCL(NETCLTAUTHHANDLE *pnetcl)
{
	CSPKG	pkg;
	int ret = net_clt_auth_recv( pnetcl, &pkg, 0 );

	if(ret <= 0)
	{
		return ret==0?0:-1;
	}
	
	// 打印消息日志
#ifndef BUILD_DEPLOY
	unsigned char kbuf[256];
	GetKeyboardState(kbuf);
	if( kbuf[VK_NUMLOCK] & 1)
	{
		static char szBuf[1024*8];
		static char buf[256];
		ZeroMemory( szBuf, sizeof(szBuf) );
		ZeroMemory( buf, 256 );
		if( net_clt_auth_sprintf( pnetcl, &pkg, szBuf, 1024*4 ) >= 0 )
		{
			SYSTEMTIME st;
			GetLocalTime( &st );
			sprintf( buf, "s->c: %d-%d-%d %d:%d:%d:%d", 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
			LOG_SEVEREMSG(buf);
			LOG_SEVEREMSG(szBuf);
		}
	}
#endif
	//////////////////////////////////////////////////////////////////////////

	if( pkg.Head.Cmd > 256 )
	{
		LOG_WARNING("msg cmd error:%d", pkg.Head.Cmd);
		return -1;
	}
	
	if( m_Handlers[pkg.Head.Cmd] == NULL )
	{
		LOG_WARNING("recv wrong msg:%d", pkg.Head.Cmd);
		return -1;
	}

	if( HEARTBEAT_RES != pkg.Head.Cmd )
	{
		if (m_isBusy)
		{
			m_lstCollecting.resize(m_lstCollecting.size() + 1);
			m_lstCollecting.back().first = pkg.Head.Cmd;
			m_lstCollecting.back().second = pkg.Body;
			return 1;
		}
	}

	if( m_Handlers[pkg.Head.Cmd]->HandleNetMsg(pkg.Head.Cmd, pkg.Body) < 0 )
	{
		LOG_WARNING("handle msg failed");
		return -1;
	}

	return 1;
}

int CSNetManager::ConnectDirServer( const char *addr, unsigned short port )
{
	if( m_pDirServerNet == NULL )
	{
		int count;
		for( count=0; count<2; count++ )//尝试2次
		{
			LOG_INFO("try to connect server %s:%d", addr, port);
			if( net_clt_han_create_mem( m_pCSMetaBuf, (char*)addr, port, &m_pDirServerNet ) >= 0 )
			{
				LOG_INFO("connected server %s:%d", addr, port);
				return 1;
			}
		}
	}
	LOG_WARNING("net_clt_han_create failed");
	return -1;
}

void CSNetManager::ReleaseDirServer()
{
	if( m_pDirServerNet != NULL ) 
	{
		net_clt_han_destroy( &m_pDirServerNet );
		m_pDirServerNet = NULL;
	}
}

int CSNetManager::ConnectServer( const char*		authConnIP, 
								unsigned short		authPort, 
								const char*		addr, 
								unsigned short		port, 
								char*				pszAccountLoginName, 
								char*				pszPasswd, 
								int*				pRet,
								int				iCltType,
								int				iAuthFlag,
								const char*		CertID,
								const char*		RealName,
								const char*		eMail,
								const char*		Mobile,
								const char*		pszMac )
{
	strcpy(m_IP,addr);
	m_Port = port;
	for(int i=0; i<MAX_NETCL; i++)
	{
		if( m_pNetCLTable[i] != NULL )
		{
			continue;
		}

		int count;

		for(count=0; count<2; count++)//尝试2次
		{
			LOG_INFO("try to connect server %s:%d", addr, port);
			*pRet = net_clt_auth_han_create_c( (char*)m_pCSMetaBuf, 
				(char*)authConnIP, 
				authPort, 
				(char *)addr, 
				port, 
				pszAccountLoginName,
				pszPasswd,
				getLocalVersion(),
				&m_pNetCLTable[i],
				(char*)pszMac, (char*)CertID, (char*)RealName, (char*)eMail, (char*)Mobile, iCltType, iAuthFlag );
			// 注册成功
			if( *pRet == 0 || *pRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES)
			{
				LOG_INFO("connected server %s:%d", addr, port);
				return i;
			}
			else if( *pRet == AUTH_REASON_NEED_PASSWD_CARD_ERROR )
			{
				return i;
			}else if( *pRet != AUTH_ERR_NET )
			{//只有在网络有问题的时候才重试
				return -1;
			}
		}

		LOG_WARNING("net_clt_han_create failed");
		return -1;
	}
	assert(0);
	return -1;
}

int CSNetManager::DirectConnectServer(char* authInfo,const char* ip,unsigned short port,const char* pszMac,int iCltType,int* pRet)
{
	strcpy(m_IP,ip);
	m_Port = port;
	for(int i=0; i<MAX_NETCL; i++)
	{
		if( m_pNetCLTable[i] != NULL )
		{
			continue;
		}

		int count;

		for(count=0; count<3; count++)//尝试3次
		{
			LOG_INFO("try to connect server %s:%d", ip, port);
			*pRet = net_clt_direct_conn_c((char*)m_pCSMetaBuf,
				(NetCltAuthInfo*)authInfo,
				(char*)ip,port,getLocalVersion(),&m_pNetCLTable[i],(char*)pszMac,iCltType);
			if( *pRet == 0 )
			{
				LOG_INFO("connected server %s:%d", ip, port);
				return i;
			}
			else if( *pRet > 0 )
			{
				return -1;
			}
		}
		LOG_WARNING("net_clt_han_create failed");
		return -1;
	}
	assert(0);
	return -1;
}

// 激活游戏账户
int	CSNetManager::ActiveAccount(	const char*		authConnIP, 
								unsigned short	authPort, 
								const char*		addr, 
								unsigned short	port,
								char* szActiveCode, 
								const char* pszMac,
								int iCltType,
								int* pRet )
{
	strcpy(m_IP,addr);
	m_Port = port;
	for(int i=0; i<MAX_NETCL; i++)
	{
		if( m_pNetCLTable[i] != NULL )
		{
			continue;
		}

		for ( int count=0; count<2; count++)//尝试2次
		{
			LOG_INFO("try to connect Active server %s:%d", addr, port);
			*pRet = net_clt_auth_han_active_c( (char*)m_pCSMetaBuf, 
				(char*)authConnIP, 
				authPort, 
				(char *)addr, 
				port, 
				szActiveCode,
				&m_pNetCLTable[i],(char*)pszMac,iCltType, NULL );
			//注册成功
			if ( *pRet == 0 || *pRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES )
			{
				LOG_INFO("connected Active server %s:%d", addr, port);
				return i;
			}
			else if ( *pRet > 0 )
			{
				return -1;
			}
		}

		LOG_WARNING("net_clt_auth_han_active failed");
		return -1;
	}
	assert(0);
	return -1;
}

void CSNetManager::Release()
{
	for( int i=0; i<MAX_NETCL; i++ )
	{
		if( m_pNetCLTable[i] == NULL ) 
		{
			continue;
		}
		net_clt_han_auth_destroy( &m_pNetCLTable[i] );
	}
}

int CSNetManager::Dispatch()
{
	if (!m_isBusy)
	{
		while (!m_lstSending.empty())
		{
			SendNetMessage(m_lstSending.front().first, m_lstSending.front().second.first, m_lstSending.front().second.second);
			m_lstSending.pop_front();
		}
		while (!m_lstCollecting.empty())
		{
			if (m_isBusy)
			{
				break;
			}
			if( m_Handlers[m_lstCollecting.front().first]->HandleNetMsg(m_lstCollecting.front().first, m_lstCollecting.front().second) < 0 )
			{
				LOG_WARNING("handle msg failed");
			}
			m_lstCollecting.pop_front();
		}
	}
	int ret = -1;
	if( m_pDirServerNet != NULL )
	{
		do
		{
			ret = dispatchDirNetCL( m_pDirServerNet );
		}while(ret>0);

		if( ret == -1 && m_pDirServerNet != NULL && m_pDirServerNet->iErr == AUTH_ERR_NET )
		{
			ret = AUTH_ERR_NET;
		}
	}else{
		for(int i=0; i<MAX_NETCL; i++)
		{
			if( m_pNetCLTable[i] == NULL ) 
			{
				continue;
			}

			do
			{
				ret = dispatchNetCL( m_pNetCLTable[i] );
			}while(ret>0);
			if( ret == -1 && m_pNetCLTable[i] != NULL && m_pNetCLTable[i]->iErr == AUTH_ERR_NET )
			{
				ret = AUTH_ERR_NET;
			}
		}
	}
	
	return ret;
}

int CSNetManager::SendDirNetMessage( int msgid, const tagCSPkgBody &msgbody )
{
	CSPKG pkg;
	pkg.Head.Magic = PROTOCOL_MAGIC;
	pkg.Head.Cmd = msgid;
	pkg.Body = msgbody;

	NETCLTHANDLE* pnetcl = m_pDirServerNet;

	if( pnetcl == NULL )
	{
		LOG_WARNING("net_clt_send failed");
		return -1;
	}

	// 打印发送消息日志
#ifndef BUILD_DEPLOY
	unsigned char kbuf[256];
	GetKeyboardState(kbuf);
	if( kbuf[VK_NUMLOCK] & 1)
	{
		static char szBuf[1024*4];
		static char buf[256];
		ZeroMemory( szBuf, 1024*4 );
		ZeroMemory( buf, 256 );
		if( net_clt_sprintf( pnetcl, &pkg, szBuf, 1024*4 ) >= 0 )
		{
			SYSTEMTIME st;
			GetLocalTime( &st );
			sprintf( buf, "c->s: %d-%d-%d %d:%d:%d:%d", 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
			LOG_SEVEREMSG(buf);
			LOG_SEVEREMSG(szBuf);
		}
	}
#endif

	if( 0 > net_clt_send( pnetcl, &pkg, 10 ) )
	{
		LOG_WARNING("net_clt_send failed");
		return -1;
	}
	return 0;
}

int CSNetManager::SendNetMessage(int netcl_index, int msgid, const tagCSPkgBody &msgbody)
{
	if( HEARTBEAT_REQ != msgid )
	{
		if( m_isBusy )
		{
			m_lstSending.resize(m_lstSending.size() + 1);
			m_lstSending.back().first = netcl_index;
			m_lstSending.back().second.first = msgid;
			m_lstSending.back().second.second = msgbody;
			return 0;
		}
	}

	CSPKG pkg;
	pkg.Head.Magic	= PROTOCOL_MAGIC;
	pkg.Head.Cmd	= msgid;
	pkg.Body		= msgbody;

	if( netcl_index < 0 ) 
	{
		netcl_index = 0;
	}
	assert(netcl_index>=0 && netcl_index<MAX_NETCL);
	NETCLTAUTHHANDLE* pnetcl = m_pNetCLTable[netcl_index];
	
	if( pnetcl == NULL )
	{
		LOG_WARNING("net_clt_send failed");
		return -1;
	}

	// 打印发送消息日志
	//////////////////////////////////////////////////////////////////////////
#ifndef BUILD_DEPLOY
	unsigned char kbuf[256];
	GetKeyboardState(kbuf);
	if( kbuf[VK_NUMLOCK] & 1)
	{
		static char szBuf[1024*4];
		static char buf[256];
		ZeroMemory( szBuf, 1024*4 );
		ZeroMemory( buf, 256 );
		if( net_clt_auth_sprintf( pnetcl, &pkg, szBuf, 1024*4 ) >= 0 )
		{
			SYSTEMTIME st;
			GetLocalTime( &st );
			sprintf( buf, "c->s: %d-%d-%d %d:%d:%d:%d", 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
			LOG_SEVEREMSG(buf);
			LOG_SEVEREMSG(szBuf);
		}
	}
#endif

	if( 0 > net_clt_auth_send( pnetcl, &pkg, 10 ) )
	{
		LOG_WARNING("net_clt_send failed");
		return -1;
	}
	
	/*
	CSPKG res;
	if (0 >= net_clt_recv(pnetcl, &res, 2000))
	{
		return -1;
	}*/

	return 0;
}

void CSNetManager::RegisterMsgHandler(int msgid, NetMsgHandler *phandler)
{
	assert(msgid>=0 && msgid<256);
	m_Handlers[msgid] = phandler;
}

void CSNetManager::unRegisterMsgHander( int msgid )
{
	m_Handlers[msgid] = NULL;
}

void CSNetManager::RegisterDisConnectHandler( NetDisConnectHandler* phandler )
{
	m_DisConnectHandlers = phandler;
}

bool CSNetManager::SetBusy(bool busy)
{
	bool old = m_isBusy;
	m_isBusy = busy;
	return old;
}

void CSNetManager::clearMsg()
{
	m_lstSending.clear();
	m_lstCollecting.clear();
}

INetworkManager *INetworkManager::Create()
{
	return new CSNetManager;
}

unsigned int DelayNetSender::expire()
{
	SendGameServerMsg(id, body);
	id = -1;
	return halt();
}

bool DelayNetSender::complete()
{
	return id == -1;
}

void DelayNetSender::setMsg(int msgid, const tagCSPkgBody &msgbody)
{
	if (complete())
	{
		return;
	}
	id = msgid;
	memcpy(&body, &msgbody, sizeof(body));
}

unsigned char* CSNetManager::getMetaBuf()
{
	return m_pCSMetaBuf;
}

int CSNetManager::CreateUserInGame( char*	pszAccountLoginName, 
									 char*	pszPasswd,
									 char*	CertID,
									 char*	RealName,
									 char*	eMail,
									 char*	Mobile,
									 int	iCltType )
{
	int netcl_index = 0;
	CSPKG stpkg;
	RSA *pstRsa;
	net_clt_auth_msg_create((LPTDRMETA)m_pCSMetaBuf,
		pstRsa,
		pszAccountLoginName,
		pszPasswd,
		NULL,
		0,
		0,
		CertID,
		RealName,
		eMail,
		Mobile,
		iCltType,
		AUTH_FLAG_DEQUICK,
		&stpkg, NULL);

	NETCLTAUTHHANDLE* pnetcl = m_pNetCLTable[netcl_index];

	if( pnetcl == NULL )
	{
		LOG_WARNING("net_clt_send failed");
		return -1;
	}

	if( 0 > net_clt_auth_send( pnetcl, &stpkg, 10 ) )
	{
		LOG_WARNING("send create user message failed");
		return -1;
	}
	return 0;
}

bool CSNetManager::getSockAddr( void* addr )
{
	if( m_pNetCLTable[0] != NULL )
	{
		SOCKET s = (SOCKET) m_pNetCLTable[0]->s;
		int size = sizeof( SOCKADDR_IN );
		return ( getpeername( s, (SOCKADDR*)addr, &size ) == 0 );
	}
	return false;
}

const char* CSNetManager::getConnectWorldIp()
{
	return m_IP;
}

bool CSNetManager::GetAccountRole( const char* AuthIp, unsigned short AuthPort, const char* LoginName, void* pRole )
{
	int ret = net_clt_acc_get_role((char*)m_pCSMetaBuf, (char*)AuthIp, AuthPort,(char*) LoginName, (ACCGETROLESVR*)pRole );
	return ( ret == 0 );
}

int CSNetManager::QuickLoginServer(const char* szIp, unsigned short port, const char* pszMac, int iCltType, int* pRet)
{
	strcpy(m_IP,szIp);
	m_Port = port;
	for(int i=0; i<MAX_NETCL; i++)
	{
		if( m_pNetCLTable[i] != NULL )
		{
			continue;
		}
		NetCltAuthInfo pAuth;
		get_clt_auth_han_info(&pAuth);
		int count;
		for(count=0; count<3; count++)//尝试3次
		{
			LOG_INFO("try to connect server %s:%d", szIp, port);
			*pRet = net_clt_direct_conn_c((char*)m_pCSMetaBuf,
				&pAuth,
				(char*)szIp,port,getLocalVersion(),&m_pNetCLTable[i],(char*)pszMac,iCltType);
			if( *pRet == 0 )
			{
				LOG_INFO("connected server %s:%d", szIp, port);
				return i;
			}
			else if( *pRet > 0 )
			{
				return -1;
			}
		}
		LOG_WARNING("net_clt_han_create failed");
		return -1;
	}
	assert(0);
	return -1;
}

int CSNetManager::AuthPasswdCard(	const char*		pszMac,
									int				iCltType,
									void*			info,
									int*			pRet )
{
	for(int i=0; i<MAX_NETCL; i++)
	{
		if( m_pNetCLTable[i] != NULL )
		{
			continue;
		}

		*pRet = net_clt_auth_han_active_c( (char*)m_pCSMetaBuf, 
			NULL,0,(char *)m_IP,m_Port,"",
			&m_pNetCLTable[i],(char*)pszMac,iCltType, (PASSWDCARDAUTHINFO*)info );
		//注册成功
		if ( *pRet == 0 || *pRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES )
		{
			LOG_INFO("connected server %s:%d use password card", m_IP, m_Port);
			return i;
		}
		else if ( *pRet > 0 )
		{
			return -1;
		}
		LOG_WARNING("net_clt_auth_han_active password card failed");
		return -1;
	}
	return -1;
}

int CSNetManager::AuthHeartHeat()
{
	return net_clt_auth_heart_beat( (char*)m_pCSMetaBuf );
}

void CSNetManager::SaveTempImage(int nIdx, const char* szPath)
{
	NetCltAuthInfo pAuth;
	get_clt_auth_han_info(&pAuth);
	if( nIdx <= pAuth.stPasswdCardAuthSvr.ImageNum )
	{
		FILE* f = fopen( szPath, "wb" );
		if( f != NULL )
		{
			fwrite( pAuth.stPasswdCardAuthSvr.Image[nIdx-1].Data,pAuth.stPasswdCardAuthSvr.Image[nIdx-1].Len,1,f );
			fclose(f);
		}
	}
}