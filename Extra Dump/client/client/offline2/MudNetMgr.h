#ifndef __MudNetMgr_H__
#define __MudNetMgr_H__

#include <map>
#include <string>
#include <winsock2.h>
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

class Ogre::XMLData;
// struct GameConfig;

#define WM_NET_NOTIFY (WM_USER+1)
//tolua_begin

enum ChannelType
{
	Colligation_Channel, 
	World_Channel,
	Private_Channel,
	Clan_Channel,
};

struct DirWorldInfo
{
	char name[32];
	bool recommend;
	int busy; //tagDirBusyDegree
	char ip[48];
	int  Ip_Bak1;
	int  Ip_Bak2;
	unsigned short port;
	unsigned short worldid; 
	unsigned char RegionID;                        	/*  Ver.166 属于哪个大区 */
	char AuthIP[48];                          		/*  Ver.166 连接auth_svr IP */
	int  authIP_Bak1;
	int  authIP_Bak2;
	unsigned short AuthPort;                        /*  Ver.166 连接auth_svr Port */
	char mudIp[48];
	unsigned short mudPort;
	int  MudIp_Bak1;
	int  MudIp_Bak2;
};

struct DirBigWorldInfo
{
	DirBigWorldInfo()
	{
		m_Worlds.clear();
	}
	const char *getName();
	int getNumWorld();
	unsigned int getFlag();
	DirWorldInfo &getIthWorld(int i);

	void addWorldInfo(const CSDIRWORLDINFO &srcinfo);
	void addWorldInfo( const DirWorldInfo& src );

	char						m_Name[64];
	std::string					m_strUrl;
	std::vector<DirWorldInfo>	m_Worlds;
	char						m_szServer[128];	//dir 服务器名
	unsigned int				m_uStatusFlag;

};

//tolua_end

struct Messagestruct
{
	int			style;
	std::string content;
};

void OfflineSendMessage(int msgid, const tagCSPkgBody &msgbody);

class MudNetMgr//tolua_export
	: public NetMsgHandler,public Ogre::Singleton<MudNetMgr>
{//tolua_export
public:
	MudNetMgr(HWND hwnd);
	~MudNetMgr(void);

public:
	
	void				Dispatch( unsigned int dtick );

	virtual int			HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
	void				initNetMgr();	

	//登陆游戏
	void				requestEnterGameWorld(tdr_longlong RoleID);

	void				requestHeartHeat();

	bool				requestLoginServerDirect(const char* pdirectstart);

	void                SetConfig(GameConfig	cfg)
	{
		m_cfg=cfg;
	}

	//////////////////////////////////////////////////////////////////////////
	//tolua_begin
	//获取数据
	int					getBigWorldNum(){return (int)m_BigWorlds.size();};
	DirBigWorldInfo&	getBidWorldInfo(int nIndex);

	bool				isDirConnected();	
	int					getSevernums();
	const char*         getSeverName(int i);
	int					getBigWorldindex();
	int					getWorldNum(int i);
	const char*			getWorldName(int i,int j);
	int					getWorldState(int i,int j);
	int					getWorldindex();
	const char*         getWorldIp();
	int					getWorldPort();
	int                 getWorldId();
	int					getSelectWorldState();
	const char*         getSelectWorldName();
	const char*         getSelectBigWorldName();
	int					getChatmsgtype();
	const char*         getChatmsgrolename();
	const char*         getChatmsgContent();
	int					getTextTipsnums();
	const char*			getTextTip(int i);
	int					getMessageNums();
	const char*			getMessagestr();
	int					getMessagestyle();
	int					getLastSelectArea();
	int					getLastSelectWorld();


	void				addMessagestr(const char* str,int style);

	void				delMessage();
	void				setBigworldindex(const char* str);
	void				setWorldindex(const char* str);;


	void				requestLoginGameServer( const char *ip, unsigned short port, const char *username, 
								const char *passwd, int worldId, const char* worldName);
	void				requestSendChat( int channel, const char* szName, const char* szContent,int onlinetype);
	void				requestChannelSet(int channel, bool sign);
	void				requestLogout();
	//请求Dir列表
	void				ClearTexttip();
	void				startZone();
	void				ResetRfreshTime()
	{
		m_nRfreshTime = 0;
	}
	int					getLoginResult()
	{
		return	m_nLoginResult;
	}
	void				ReFreshServer();
	//tolua_end
private:
	bool				LoadLocalSvr();
	void				initDirInfo(const CSDIRWORLDLIST& dirinfo );
	void				onDirInfoResponse(const CSDIRRES& res);
	void				onChatResponse(const CSCHATRES& chat);

	bool				LoadDirFromUrl( const char* szUrl );

private:
	INetworkManager*				m_pNetMgr;
	std::vector<DirBigWorldInfo>	m_BigWorlds;
	int								m_nBigWorldNum;

	CSCHATRES						m_ChatRes;
	char							m_szLastLoginIp[64];
	unsigned short					m_uLastLoginPort;
	unsigned short					m_uLastLoginMudPort;

public:
	HWND							m_hwnd;
	int								m_nBigWorldSelectindex;
	int								m_nWorldSelectindex;
	int								m_nLastRequstTime;
	char							m_DirIp[128];
	int								m_nDirPort;
	GameConfig						m_cfg;
	std::vector<std::string>        m_tiplist;
	std::vector<Messagestruct>		m_messageboxlist;
	bool							m_channelsheildstate[3];
	bool							m_bStartfb;//是否从客户端登录
	tdr_longlong					m_DirectloginRoleID;//从客户端直接启动mud用roleid
	int								m_nRfreshTime;
	int								m_nLoginResult;
	bool							m_nOffline;
};//tolua_export

#endif