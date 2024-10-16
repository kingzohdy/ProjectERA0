#pragma once
#include <map>
#include <string>

class Ogre::XMLData;

#define WM_NET_NOTIFY (WM_USER+1)


enum eNotifyType
{
	eUpdateDirList,	//���·������б�
	eLoginRet,		//��½�������
	eRoleList,		//��������б�
	eRoleLogin,		//��ҽ�����Ϸ
	eOnline,		//���������������
	eChatMsg,		//������Ϣ
	eRelation,		//���ѹ�ϵ
	eEnterRoom,		//���뷿��
	eLeaveRoom,		//�뿪����


	eError,			//������ʾ
};

enum eErrorCode
{
	eErrorGetDirError,		//��ȡDIR����
	eErrorLoginError,		//��½����
	eErrorLoginRepeat,		//��ʱ����������½
	eErrorDirectLoginError,	// ���ٵ�½ʧ��
	eErrorAlreadyLogin,		//�Ѿ���½��
	eErrorKickOff,			//��������������
};

struct DirWorldInfo
{
	char name[32];
	bool recommend;
	int busy; //tagDirBusyDegree
	char ip[32];
	unsigned short port;
	unsigned short worldid; 
	unsigned char RegionID;                        	/*  Ver.166 �����ĸ����� */
	char AuthIP[32];                          		/*  Ver.166 ����auth_svr IP */
	unsigned short AuthPort;                        /*  Ver.166 ����auth_svr Port */
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
	int		iOnline;	//0 ���ߣ�1��Ϸ���� 2 �һ�����
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
	//���������
	//////////////////////////////////////////////////////////////////////////
	//��½������
	bool				requestLoginGameServer( const char *ip, unsigned short port, const char *username, 
							const char *passwd, int worldId, const char* worldName);
	//��½��Ϸ
	void				requestEnterGameWorld(tdr_longlong RoleID);
	//��ɫ�˳�
	void				requestRoleLogout();
	//�˺��˳�
	void				requestLogout();
	//����Dir�б�
	void				requestDirInfo();

	void				requestSendChat( const char* toName, const char* chatMsg,bool bIsFriend);

	void				requestHeartHeat();

	void				requestLoginServerDirect();

	void				addMessageHwnd(HWND hWnd);

	//////////////////////////////////////////////////////////////////////////
	//��ȡ����
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

	CSROLELIST						m_RoleList;		//��ҽ�ɫ�б�

	CSCHATROOMINFO					m_ChatRoomInfo;	//���������Ϣ

	CSMUDROLEDETAIL					m_RoleDetail;	//������Ϣ

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
