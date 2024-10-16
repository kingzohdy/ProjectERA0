
#ifndef __CSNETMANAGER_H__
#define __CSNETMANAGER_H__

#include "NetMessage.h"
#include "cs_net.h"
#include "GameEngineTypes.h"
#include "GameTickAction.h"
#include <list>
enum
{
	MAX_NETCL = 4
};

class CSNetManager : public INetworkManager
{
public:
	CSNetManager();
	~CSNetManager();
	
	// 连接DirServer不加密
	virtual int		ConnectDirServer( const char *addr, unsigned short port );
	virtual void	ReleaseDirServer();
	// 连接游戏服务器消息加密
	virtual int		ConnectServer( const char*		authConnIP, 
								   unsigned short	authPort, 
								   const char*		addr, 
								   unsigned short	port, 
								   char*			pszAccountLoginName, 
								   char*			pszPasswd,
								   int*				pRet,
								   int				iCltType,
								   int				iAuthFlag,
								   const char*		CertID = NULL,
								   const char*		RealName = NULL,
								   const char*		eMail = NULL,
								   const char*		Mobile = NULL,
								   const char*		pszMac = NULL );
	// 激活游戏账户
	virtual int		ActiveAccount(	const char*		authConnIP, 
		unsigned short	authPort, 
		const char*		addr, 
		unsigned short	port,
		char* szActiveCode,
		const char* pszMac,
		int iCltType,
		int* pRet );
	virtual void	Release();

	virtual int		Dispatch();
	virtual bool	SetBusy(bool busy);
	virtual void	clearMsg();
	virtual int		SendDirNetMessage( int msgid, const tagCSPkgBody &msgbody );
	virtual int		SendNetMessage(int netcl_index, int msgid, const tagCSPkgBody &msgbody);
	virtual void	RegisterMsgHandler(int msgid, NetMsgHandler *phandler);
	virtual void	RegisterDisConnectHandler( NetDisConnectHandler* phandler );
	virtual int		DirectConnectServer(char* authInfo,const char* ip,unsigned short port,const char* pszMac,int iCltType,int* pRet);
	virtual void	unRegisterMsgHander( int msgid );
	virtual int	CreateUserInGame(char*	pszAccountLoginName, 
										char*	pszPasswd,
										char*	CertID,
										char*	RealName,
										char*	eMail,
										char*	Mobile,
										int	iCltType );

	virtual int	AuthPasswdCard(	const char*		pszMac,
								int				iCltType,
								void*			info,
								int*			pRet );

	virtual unsigned char*	getMetaBuf();
	virtual bool getSockAddr( void* addr );
	virtual const char* getConnectWorldIp();
	virtual bool GetAccountRole( const char* AuthIp, unsigned short AuthPort,const char* LoginName, void* pRole );
	virtual int QuickLoginServer( const char* szIp, unsigned short port, const char* pszMac, int iCltType, int* pRet );
	virtual int AuthHeartHeat();
	virtual void SaveTempImage(int nIdx, const char* szPath);

private:
	NETCLTHANDLE*			m_pDirServerNet;
	NETCLTAUTHHANDLE*		m_pNetCLTable[MAX_NETCL];

	NetMsgHandler*			m_Handlers[256];
	NetDisConnectHandler*	m_DisConnectHandlers;
	char					m_TdrPath[128];
	unsigned char*			m_pCSMetaBuf;
	char					m_IP[48];
	unsigned short			m_Port;
	
	//1:收到数据， 0:没有数据， -1:链接错误， -2:协议错误
	int dispatchDirNetCL( NETCLTHANDLE* pnetcl );
	//1:收到数据， 0:没有数据， -1:链接错误， -2:协议错误
	int dispatchNetCL( NETCLTAUTHHANDLE* pnetcl );

	bool					m_isBusy;
	std::list< std::pair< int, std::pair<int, tagCSPkgBody> > >	m_lstSending;
	std::list< std::pair<int, tagCSPkgBody> >	m_lstCollecting;
};

class DelayNetSender : public ITickActionDelay
{
private:
	int					id;
	tagCSPkgBody		body;

public:
	DelayNetSender(unsigned int delay, int msgid, const tagCSPkgBody &msgbody) : ITickActionDelay(delay), id(msgid), body(msgbody) {}
	virtual ~DelayNetSender() {}
	bool	complete();
	void	setMsg(int msgid, const tagCSPkgBody &msgbody);
	virtual unsigned int expire();
};
#endif