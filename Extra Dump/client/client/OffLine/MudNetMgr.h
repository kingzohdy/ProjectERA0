#pragma once
#include <map>
#include <string>

class Ogre::XMLData;

#define WM_NET_NOTIFY (WM_USER+1)


enum eNotifyType
{
	eUpdateDirList,	//更新服务器列表
	eLoginRet,		//登陆结果返回
	eRoleList,		//获得人物列表
	eRoleLogin,		//玩家进入游戏
	eOnline,		//其他玩家上线下线
	eChatMsg,		//聊天消息
	eRelation,		//好友关系
	eEnterRoom,		//进入房间
	eLeaveRoom,		//离开房间


	eError,			//错误提示
};

enum eErrorCode
{
	eErrorGetDirError,		//获取DIR出错
	eErrorLoginError,		//登陆出错
	eErrorLoginRepeat,		//短时间内连续登陆
	eErrorDirectLoginError,	// 快速登陆失败
	eErrorAlreadyLogin,		//已经登陆了
	eErrorKickOff,			//被服务器踢下线
};

struct DirWorldInfo
{
	char name[32];
	bool recommend;
	int busy; //tagDirBusyDegree
	char ip[32];
	unsigned short port;
	unsigned short worldid; 
	unsigned char RegionID;                        	/*  Ver.166 属于哪个大区 */
	char AuthIP[32];                          		/*  Ver.166 连接auth_svr IP */
	unsigned short AuthPort;                        /*  Ver.166 连接auth_svr Port */
	char mudIp[32];
	unsigned short mudPort;
};

struct DirBigWorldInfo
{
	DirBigWorldInfo()
	{
		m_nWorldNum = 0;
	}
	const char *getName();
	int getNumWorld();
	DirWorldInfo &getIthWorld(int i);

	void addWorldInfo(const CSDIRWORLDINFO &srcinfo);
	void addWorldInfo( const DirWorldInfo& src );
	void updateWorldInfo( const CSDIRLATESTINFO& src );
	std::string getUrl();

	char			m_Name[64];
	std::string		m_strUrl;
	int				m_nWorldNum;
	DirWorldInfo	m_Worlds[MAX_WORLD_NUM];

};
class AutoLock
{
public:
	explicit AutoLock(LPCRITICAL_SECTION pSection)
	{
		m_pSection = pSection;
		EnterCriticalSection(pSection);
	}
	~AutoLock()
	{
		LeaveCriticalSection(m_pSection);
	}
	LPCRITICAL_SECTION m_pSection;
};

struct FriendInfo
{
	tdr_longlong RoleID;
	char	roleName[NAME_LEN];
	char	Emotion[EMOTION_LEN];
	int		iOnline;	//0 下线，1游戏在线 2 挂机在线
};

class MudNetMgr:public NetMsgHandler,public Ogre::Singleton<MudNetMgr>
{
public:
	MudNetMgr(void);
	~MudNetMgr(void);

public:

	void				startDispatch();
	void				Dispatch();
	static MudNetMgr*	Create(Ogre::XMLData* pData);

	virtual int			HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
	void				initNetMgr();

	//////////////////////////////////////////////////////////////////////////
	//请求服务器
	//////////////////////////////////////////////////////////////////////////
	//登陆服务器
	bool				requestLoginGameServer( const char *ip, unsigned short port, const char *username, 
							const char *passwd, int worldId, const char* worldName);
	//登陆游戏
	void				requestEnterGameWorld(tdr_longlong RoleID);
	//角色退出
	void				requestRoleLogout();
	//账号退出
	void				requestLogout();
	//请求Dir列表
	void				requestDirInfo();

	void				requestSendChat( const char* toName, const char* chatMsg,bool bIsFriend);

	void				requestHeartHeat();

	void				requestLoginServerDirect();

	void				addMessageHwnd(HWND hWnd);

	//////////////////////////////////////////////////////////////////////////
	//获取数据
	int					getBigWorldNum(){return m_nBigWorldNum;};
	DirBigWorldInfo&	getBidWorldInfo(int nIndex);

	int					getRoleNum(){ return m_RoleList.RoleNum; };
	ROLEINFO&			getRoleInfo(int nIndex){return m_RoleList.RoleList[nIndex]; };

	int					getChatRoomNum() { return m_ChatRoomInfo.PlayerNum; };
	CSROOMROLEINFO&		getRoomRoleInfo(int nIndex){ return m_ChatRoomInfo.ChatRoomRoleInfo[nIndex]; };

	CSMUDROLEDETAIL&	getRoleDetail(){return m_RoleDetail;};

	CSONLINESVR			getOnlineInfo(int nIndex);

	tdr_longlong		getLoginRoleID(){return m_loginRoleID;};


	const char*			getAttrName(int attrID);

	int					getFriendNum();
	FriendInfo&			getFriendInfo(int nIndex);
private:
	bool				LoadLocalSvr();
	void				SaveLocalSvr();
	void				initDirInfo(const CSDIRINFO& res);
	void				onDirInfoResponse(const CSDIRRES& res);
	void				updateDirInfo(const CSDIRLATEST& latestinfo);
	void				onRelationResponse( const CSRELATIONSVR& relation );
	void				onOnlineResponse( const CSONLINESVR& online );
	void				onChatResponse(const CSCHATRES& chat);

	void				notifyGui(eNotifyType type,LPARAM lParam);


private:
	INetworkManager*				m_pNetMgr;
	int								m_nVersion;

	DirBigWorldInfo					m_BigWorlds[MAX_BIGWORLD_NUM];
	int								m_nBigWorldNum;

	CSROLELIST						m_RoleList;		//玩家角色列表

	CSCHATROOMINFO					m_ChatRoomInfo;	//房间玩家信息

	CSMUDROLEDETAIL					m_RoleDetail;	//主角信息

	tdr_longlong					m_loginRoleID;

	HANDLE							m_hThread;

	CSONLINESVR						m_OnlineInfo[MAX_CHAT_ROOM_PLAYER];


public:
	GameConfig						m_config;
	int								m_DirNetCL;
	int								m_GameNetCL;
	CRITICAL_SECTION				m_CriticalSection;

	std::vector<HWND>				m_DialogHwnd;
	std::vector<FriendInfo>			m_vFriends;

	char							m_UserName[64];

	std::map<int,std::string> m_attrNameMap;
};
