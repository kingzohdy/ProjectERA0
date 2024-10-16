
#ifndef __GameLoginManager_H__
#define __GameLoginManager_H__
//////////////////////////////////////////////////////////////////////////
// 登录系统
//////////////////////////////////////////////////////////////////////////
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "cs_net.h"
#include "GameDefineTolua.h"
#include <string>
#include <map>

//tolua_begin
#pragma pack( 1 )

enum
{
	CLIENT_MAX_BIRTH_ITEM = MAX_BIRTH_ITEM,
};

#define MAX_SAVE_NAME_NUM 3

struct tagBirthItemData_ToLua
{
	unsigned int ItemID;                         
	unsigned char ItemNum;                        
};

struct tagBirthItemDef_ToLua
{
	unsigned short Career;                         
	unsigned short Gender;                          	/*   性别 */
	tagBirthItemData_ToLua Items[MAX_BIRTH_ITEM];          
};

struct tagCSWorldBossInfoOne_ToLua
{
	int MonID;                          
	int Stat;                            	/*   1-已出现 */
	int FreshTime;                       	/*   预计刷新时间 */
};

struct tagCSWorldBossInfoRes_ToLua
{
	int Num;                            
	tagCSWorldBossInfoOne_ToLua One[MAX_WORLD_BOSS_NUM];        
};

#pragma pack()

struct LoginRole
{
	char name[32];
	int race; 
	int gender;
	int level;
	tdr_ulonglong roleid;
	int face;
	int hair;
	int head;          
	int map;
    int lastlogin;
	tagRoleAvatarProf_ToLua armInfo;
	int delTime;
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
	unsigned short AuthPort;                        /*  Ver.166 连接auth_svr Port */
	int  authIP_Bak1;
	int  authIP_Bak2;
	char mudIp[48];
	unsigned short	mudPort;
	int  MudIp_Bak1;
	int  MudIp_Bak2;
};

enum
{
	CLIENT_DIR_NOT_WORK = 0, 		/* 维护状态 */
	CLIENT_DIR_EXCELLENCE = 1, 		/* 很好 */
	CLIENT_DIR_NORMAL = 2, 			/* 正常 */
	CLIENT_DIR_BUSY = 3, 			/* 繁忙 */
	CLIENT_DIR_FULL = 4, 			/* 爆满 */
};

struct tagClientSelectBirth
{
	int BirthMapNum;                    
	int MapIDList[MAX_BIRTH_MAP];       
	int Busy[MAX_BIRTH_MAP];
};

enum tagCHOOSE_TYPE_TOLUA 
{
	CHOOSE_TYPE_CLOSE_TOLUA = 0, 	/* 关闭,未选择 */
	CHOOSE_TYPE_YES_TOLUA = 1, 	/* 选择了yes */
	CHOOSE_TYPE_NO_TOLUA = 2, 	/* 选择了no */
};

enum tagBIND_TYPE_TOLUA 
{
	BIND_TYPE_UNKNOWN_TOLUA = 0, 	/* 未知 */
	BIND_TYPE_NPC_TOLUA = 1, 	/* 脚本绑定了npc */
	BIND_TYPE_ITEM_TOLUA = 2, 	/* 脚本绑定了物品 */
	BIND_TYPE_NAME_TOLUA = 3, 	/* 脚本绑定了脚本名 */
};

#define ROLE_USER_NUM_TOLUA ROLE_USER_NUM

enum
{
	CLIENT_NET_AREA_DIAN_XIN	= REGIONSTATUSBIT_DIANXIN,
	CLIENT_NET_AREA_WANG_TONG	= REGIONSTATUSBIT_WANGTONG,
	CLIENT_NET_AREA_SHUANG_XIAN	= REGIONSTATUSBIT_SHUANGXIAN,
};

#define DEL_ROLE_PROTECT_TM_TOLUA (DEL_ROLE_PROTECT_TM * 60)
#define PRECREATE_GIFT_VERIFY_TIMEOUT_TOLUA PRECREATE_GIFT_VERIFY_TIMEOUT

struct tagPeopleInfo_ToLua
{
	char RoleName[ROLE_NAME_LEN];         	/*   角色名 */
	char Gender;                          	/*   性别 */
	unsigned char Level;                          
};

struct tagCSNewPeopleInfo_ToLua
{
	int Num;                             	/*   数量 */
	tagPeopleInfo_ToLua PeopleInfo[MAX_NEW_PEOPLE_NUM];  	/*   新人栏具体信息 */
};

#define MAX_NEW_PEOPLE_NUM_TOLUA MAX_NEW_PEOPLE_NUM
//tolua_end

const int MAX_USER_NAME_LENGTH = 64;
class DirBigWorldInfo//tolua_export
{//tolua_export
public:
	DirBigWorldInfo();

	//tolua_begin
	const char *getName();
	int getNumWorld();
	DirWorldInfo &getIthWorld(int i);

	bool isDianXinArea();
	bool isWangTongArea();
	bool isShuangXianArea();

	bool isNewBigWorld();
	bool isRecommendBigWorld();
	//tolua_end
	unsigned int getFlag();

	void addWorldInfo(const CSDIRWORLDINFO &srcinfo);
	void addWorldInfo( const DirWorldInfo& src );
	void updateWorldInfo( const CSDIRLATESTINFO& src );
	void clearWorldInfo();
	std::string getUrl();

	char			m_Name[64];
	std::string		m_strUrl;
	unsigned int	m_uStatusFlag;
	int				m_nDirIdx;

private:
	std::vector<DirWorldInfo>m_Worlds;
};//tolua_export

struct DirConnectData
{
	DirConnectData()
	{
		m_nConfigIdx = 0;
		m_DirServerNet = NULL;
	};

	void Clear()
	{
		if( m_DirServerNet != NULL )
		{
			net_clt_han_destroy( &m_DirServerNet );
			m_DirServerNet = NULL;
		}		
	}

	int				m_nConfigIdx;
	NETCLTHANDLE*	m_DirServerNet;
};

class LoginManager //tolua_export
	: public NetMsgHandler, public Ogre::Singleton<LoginManager>
{//tolua_export
public:
	LoginManager();
	~LoginManager();

	void update(unsigned int dtick);

	static LoginManager* Create();
	void initDirInfo(const CSDIRWORLDLIST& dirinfo );
	void initDirInfo( const CSDIRINFO& dirinfo, int nIdx );
	// 	void updateDirInfo( const CSDIRLATEST& latestinfo );
	void onMapPreLoaded(GameMap *map, const CSENTERMAP& entermap);

	virtual int		HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

	//tolua_begin
	void keepAlive();
	void reqireHeartBeat();
 	bool requestDirInfo(const char *ip, unsigned short port);
	void releaseLoginGameServer();
	bool requestLoginGameServer( int RegionIdx, int WorldIdx, const char *username, const char *passwd );
	bool requestActiveAccount( int RegionIdx, int WorldIdx , const char *pszActiveCode );
	void requestCreateRole(const char* szNickName,int Gender,int Race,int Face,int Head,int Hair, int nClothes );
	void requestDeleteSelectedRole();
	void requestEnterWorld();
	void requestRoleList();
	void requestLogout();
	void requestCancelLogout();
	void requestSelectBirthLocation( int index );
	void requestCancelSelect();
	tagClientSelectBirth& getSelectBirthLocation();
	int	 getSelectBirthIndex();

	const char* getMapName(int mapId);
	bool dirRefurbish(const char *ip, unsigned short port);
	
	bool LoadLocalSvr();
	void SaveLocalSvr();

	int					getNumBigWorld();
	DirBigWorldInfo*	getBigWorld(int i);
	int					getNumRole();
	LoginRole&			getIthRole(int i);
	void				setAvatarAttrib(int i);
	void				selectRole(int i);
	void				beginRotatePreview(int speed);
	void				endRotatePreview();
	void				onEscPress();
	int					UseBackUpIP( const DirWorldInfo &IthWorld, int nAuth_Flag, const char *passwd, const char *szMac, int nRet );
	int					PickBackUpSvrIP( int bak1 ,int bak2 );

	void				getOptimalBakIp( DirWorldInfo * IthWorld );

	int					getDelRoleResult()
	{ 
		return m_nDelResult;   
	}	// 获取角色删除失败返回结果				
	int                 getLoginResult() { return m_nLoginResult; }              // 获取账号登录返回结果    Add by Kevin [11/16/2009]
	int                 getRoleNewResult() { return m_nRoleNewResult; }          // 获取创建角色返回结果    Add by Kevin [11/17/2009]
	bool                isRoleNumFull();                                         // 角色数量是否已满        Add by Kevin [11/17/2009]
	int                 getSelectRoleIndex() { return m_SelectRoleIndex; }       // 获取当前选择的角色Index
	const char*         getOffLineText() 
	{
		return m_szOffLineText.c_str(); 
	}     // 账号被逼下线时从服务器返回的信息  
	int					getMapId() { return m_MapID; }
	void                selectLastLoginRole();                                   // 选中最后一次登陆的角色

	bool				isBackFromInWorld();

	void				startMud();

	//判断主角是否在战场中
	bool				isInBattleWorld();

	void				saveLastUserName();
	void				clearLastUserName();
	int					getSaveUserCheck();
	const char*			getLastUserName();
	const char*			getUserNameByIdx( int idx );


	//创建 LJY 账户
	bool requestCreateUser( int RegionIdx, 
							int WorldIdx, 
							const char *username, 
							const char *passwd,
							const char* szCertID,
							const char* szRealName,
							const char* szEmail,
							const char* szMobile );

	//补填账户信息
	void continueRegisterUserInfo( const char* szCertID, const char* szRealName, const char* szEmail = "" );

	int getLastSelectArea();

	int getLastSelectWorld();
	
	bool getReadUserProtocol();
	void setReadUserProtocol( bool bAccept );
	void replyPlayerMisc( int nBindType,int nId,int nChooseType );

	void breakNetConnect();

	const char*			getUserName(){return m_UserName;}
	bool				requestLoginGameServerDirect(const char *pdirectmud);
	bool				QuickLoginServer( unsigned short regionId, unsigned short worldId );
	
	tagBirthItemDef_ToLua* getBirthItemDef( int nCareer, int nGender );

	tagCSNewPeopleInfo_ToLua* getNewPeopleInfo();

	void requestUnDelRole( int nIdx);
	void requestStartMud();

	const char* getWorldNameById(int nId);
	const char* getWorldName( unsigned short RegionId, unsigned short WorldId );

	//获得今天已经在线多久
	int getTodayOnTime();

	//快速试玩
	void requestEnterLoginWithoutUserName();
	//试玩注册账号
	void requestCreateUserInGame( char* szUserName, char* szPwd, char* szName, char* szCert, char* szMobile, char* szMail );

	bool isQuickStart();

	bool isTodayFirstLogin();
	bool isCloseLine(){return m_bCloseLine;}
	int GetPing();

	int getLastSelectWorldId(){ return m_nLastSelectWorldId; };
	int getLastSelectRegionId(){ return m_nLastSelectRegionId; };

	tagCSWorldBossInfoRes_ToLua& getBossInfo(){ return m_BossInfo; }

	//密保卡
	const char* getPasswdCardPath(int nIdx);
	void SetPasswdCardInfo( int nIdx, const char* szData );
	bool requestAuthPasswdCard();
	int GetIpMode();
	void SetIpMode( int nMode );
	//手机绑定
	void requestGetSMS( const char* szPhone );
	void requsetVerifySMS( int nCode );

	void sendFashionChangeMessage(unsigned char head, unsigned char face, unsigned char hair);
	void sendNewPeopleRequest();
	void clearNewPeopleInfo();
	//tolua_end
	void setFirstLoad(int mapid);
	bool isFirstLoad();

	void tryRequestDirInfo();
	void EnterWorldDirect();

// 	DirConnectData* getDirConnectDataByServerName( const char* szServerName );
// 	const char* getDirServerName( int nIdx );

 	void dispatchDirNetCL( NETCLTHANDLE *pnetcl, int nIdx );
 	void HandleDirServerMsg( int msgid, const tagCSPkgBody& msgbody, int nIdx );
 	void LoadDirFromDirSever();
	bool LoadDirFromUrl( const char* szUrl );

	void LoadLastUserName();
	void saveLastUserData();

	int getClientType();

	const char* getServerName() {return m_ServerName;}

	void createPingThread();
	void SetPing( int nPing );

	void TryGetOtherWorldData();

	void PingCheckStartFunction( int nRecTime, int nLostPack,int nMax, int nMin );
	int	 getPingCheckValue();
	bool getPingCheckFlag();
	void setPingCheckFlag( bool bPingTest );
	char* getDesIP() { return m_DesIp; }

	const char* GetSelectWorldIp( const DirWorldInfo& world );
	const char* GetSelectWorldAuthIp( const DirWorldInfo& world );
	void LoginManager::SetLastSelectData( int RegionId, int WorldId );

	void requestShortCutSkillClear();

public:
	int								m_MapID;
	int								m_GameNetCL;
	char							m_UserName[MAX_USER_NAME_LENGTH];
	
	int								m_nLoginResult;   
	int								m_nDelResult;   
	int								m_nRoleNewResult; 
	std::string						m_szOffLineText;

	unsigned int					m_HeartBeatTick;
	unsigned int					m_CurTick;

	std::vector<LoginRole>			m_Roles;
	int								m_SelectRoleIndex;
	tagClientSelectBirth			m_SelectBirth;
	int								m_SelectBirthIndex;
	unsigned int					m_uLastTime;

	tagCSNewPeopleInfo m_NewPeopleInfo;

	std::vector<DirBigWorldInfo>	m_BigWorlds;
	bool							m_bOffLine;
	char							m_szLastLoginIp[64];
	unsigned short					m_uLastLoginPort;
	unsigned short					m_uLastLoginMudPort;
	bool							m_isFirstLoad;

	tdr_longlong					m_DirectloginRoleID;//从mud启动客户端时选用的人物ID
	bool							m_bStartfmud;//是不是从mud启动的客户端

	std::vector<DirConnectData>		m_vecDir;
	bool							m_bIsAlive;
	char							m_IpBak[48];
	HANDLE							m_LastThread;

private:
	std::vector<tagBirthItemDef*>	m_vecBirthItemDef;
	char							m_SaveUserName[MAX_SAVE_NAME_NUM][MAX_USER_NAME_LENGTH];
	int								m_nLastSelectWorldId;
	int								m_nLastSelectRegionId;
	int								m_nSaveUserName;

	int								m_nLoginTime;

	DWORD							m_dwMainThreadId;

	char							m_ServerName[256];
	bool							m_bCloseLine;		//是否关闭分线

	volatile int					m_nPing;
	tagCSWorldBossInfoRes_ToLua		m_BossInfo;
	bool							m_bWaitPassCard;
	PASSWDCARDAUTHINFO				m_stPasswdCardInfo;

	bool							m_bPingTest;
	int								m_nPingCheck;
	char							m_SelfIp[48];
	char							m_DesIp[48];
	int								m_nIpMode;
	std::string						m_strLastWorldName;
};//tolua_export

#endif
