
#ifndef __NetMessage_H__
#define __NetMessage_H__

#include "OgreSingleton.h"
#include "GameEngineTypes.h"

// struct ACCGETROLESVR;

class INetworkManager : public Ogre::Singleton<INetworkManager>
{
public:
	static INetworkManager *Create();
	virtual ~INetworkManager()
	{
	}
	// 连接DirServer不加密
	virtual int		ConnectDirServer( const char *addr, unsigned short port ) = 0;
	virtual void	ReleaseDirServer() = 0;
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
								   const char*		pszMac = NULL ) = 0;
	// 激活游戏账户
	virtual int		ActiveAccount(	const char*		authConnIP, 
										unsigned short	authPort, 
										const char*		addr, 
										unsigned short	port,
										char* szActiveCode,
										const char* pszMac,
										int iCltType,
										int* pRet
									) = 0;
	virtual void	Release() = 0;

	virtual int		Dispatch() = 0;

	virtual bool	SetBusy(bool busy) = 0;
	
	virtual void	clearMsg() = 0;

	// 发送DirServer消息
	virtual int		SendDirNetMessage( int msgid, const tagCSPkgBody &msgbody ) = 0;
	// 发送加密消息
	virtual int		SendNetMessage( int netcl_index, int msgid, const tagCSPkgBody &msgbody ) = 0;
	virtual void	RegisterMsgHandler( int msgid, NetMsgHandler *phandler ) = 0;
	virtual void	RegisterDisConnectHandler( NetDisConnectHandler* phandler ) = 0;
	virtual int		DirectConnectServer(char* authInfo,const char* ip,unsigned short port,const char* pszMac,int iCltType,int* pRet) = 0;
	virtual void	unRegisterMsgHander( int msgid ) = 0;
	// 游戏中 快速试玩注册
	virtual int	CreateUserInGame(char*	pszAccountLoginName, 
										char*	pszPasswd,
										char*	CertID,
										char*	RealName,
										char*	eMail,
										char*	Mobile,
										int	iCltType ) = 0;

	//验证密保卡
	virtual int	AuthPasswdCard(	const char*		pszMac,
								int				iCltType,
								void*			info,
								int*			pRet) = 0;

	virtual unsigned char* getMetaBuf() = 0;
	virtual bool getSockAddr( void* addr ) = 0;
	virtual const char* getConnectWorldIp() = 0;

	virtual bool GetAccountRole(const char* AuthIp, unsigned short AuthPort,const char* LoginName, void* pRole ) = 0;
	virtual int QuickLoginServer( const char* szIp, unsigned short port, const char* pszMac, int iCltType, int* pRet ) = 0;
	virtual int AuthHeartHeat() = 0;
	virtual void SaveTempImage( int nIdx, const char* szPath) = 0;
};

inline int SendDirNetMessage( int msgid, const tagCSPkgBody &msgbody )
{
	return INetworkManager::getSingleton().SendDirNetMessage( msgid, msgbody );
}

inline int SendNetMessage(int netcl_index, int msgid, const tagCSPkgBody &msgbody)
{
	return INetworkManager::getSingleton().SendNetMessage(netcl_index, msgid, msgbody);
}

inline int SendGameServerMsg(int msgid, const tagCSPkgBody &msgbody)
{
	return INetworkManager::getSingleton().SendNetMessage(-1, msgid, msgbody);
}
#endif