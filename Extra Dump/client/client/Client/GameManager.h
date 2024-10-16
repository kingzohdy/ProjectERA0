
#ifndef __GameManager_H__
#define __GameManager_H__

#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "GameMsgPrint.h"
#include "SystemSettingConfig.h"
#include "GameClan_BaseWar.h"


typedef void (*BTN_FUN)(bool);

class MailConfig : public Ogre::Singleton<MailConfig>
{
protected:
	MailConfig(){};
public:
	static MailConfig& getSingleton()
	{
		static MailConfig instance;
		return instance;
	}
	bool bHasConfig;

	char ftp_address[255];
	char ftp_pwd[255];
	char ftp_user[255];
	unsigned int ftp_port;
};

class SCTDamageManager;
class GameBackground;
class GameFollowManager;
class PworldDifficulty;
class PworldScore;
class BulletManager;
class GameManager //tolua_export
	: public Ogre::Singleton<GameManager>, public IGameMsgPrint
{//tolua_export
public:
	GameManager();
	~GameManager();

	bool Create(const char *config_file, void *hinst,const char *pszdirectmud);
	void SetCreated (bool bCreated); // 启动画面销毁，主程序显示时，才算真的创建好。防止启动加载时，KissFocus降低加载速度。
	bool IsCreated ();
	void Destroy();
	void DoFrame();
	int OnWindowMsg(void *hwnd, unsigned int msgid, unsigned int wparam, unsigned int lparam);
	void InitDB();


	//tolua_begin

	// 打印Log
	void		MakeAttach();
	void		MakeMD5();
	void		 ScriptLog(const char *pinfo);
	virtual void SendUIEvent(const char *pevent);
	void		 QuitGame();
	void		 returnLogin();
	//void		 BeginPureClient(int mapid, int player_resid);
	unsigned int getTickTime();
	void		 cancelNextSelect();
	void		HighlightNewItem( int nMemID );
	int			getLastHighlightItemID();

	int			getLastHighStallActorID();
	void		HighlightStallActor( int nActorID );

	bool		isMapLoaded();
	void		mapLoadFaded();

	void        setScreenMode(bool fullscreen, int w, int h);

	// 播放镜头动画
	void		 playCameraAnim( const char* szOmodPath );
	// 是否是在播放镜头动画
	bool		 isInPlayCamerAnim();

	// 中断相机动画
	void		 stopPlayCameraAnim();
	void		ShowLoading(bool show);
	
	void setFashionChangeAngle(float pullDist);
	void resetPlayAngle();
	
	SystemSettingConfig&	getUserSystemConfig();
	SystemSettingConfig&	getLatestUserSystemConfig();
	void			ApplyDefaultUserConfig();
	void			ApplyNewUserConfig( SystemSettingConfig& latestSystemSetting );
	void			setGlobalSoundVolume(float fVolume);
	void			setGlobalMusicVolume(float volume);

	void			changeShowUI();

	void			openWebSite( const char* szUrl );
	void			sendEMail( const char* szTitle,const char* szContent,int mailType );
	void			playVideo(const char *filename);
	bool			isVideoPlaying();
	void			skipVideo();
	void			stopVideo();
	bool			isFirstRun();
	void			setFirstRun(bool b);
	void			setVideoTag();
	void			clearVideoTag();
	bool			IsLastVideoError();

	void			reloadSystemConfig();

	void			sendAllLogToFtp();

	void			ScreenSnap();

	void			SendHttpRequest( const char* szUrl );

	void			setTestMode( bool b );
	bool			IsTestMode();
	void			SetMaxLevel( int maxLevel );
	int				GetMaxLevel();

	void			SetLevelFeng( int nLv) ;
	int				GetLevelFeng() ;

	const char*		GetUtf8Uri( const char* src );
	const char*		GetVersionName();
	bool			CanFindFile( const char* path );
	//tolua_end

	INetworkManager *GetNetworkMgr()
	{
		return m_pNetMgr;
	}

	GameUI*	GetGameUI()
	{
		return m_pGameUI;
	}

	SkillManager* GetSkillMgr()
	{
		return m_pGameSkillMgr;
	}

	ChatManager* getChatManager()
	{
		return m_pChatMgr;
	}

	GameMsgPrint* getGameMsgPrint()
	{
		return m_pGameMsgPrint;
	}

	GameMapShower*	getGameMapShower()
	{ 
		return m_pGameMapShower;
	}

	GameConfig&	GetConfig()
	{
		return m_Config;
	}

	void GetViewport(int &w, int &h)
	{
		w = m_ViewportWidth;
		h = m_ViewportHeight;
	}

	Ogre::SceneRenderer *GetMainSceneRenderer()
	{
		return m_pMainRenderer;
	}

	Ogre::ScriptVM*	getScriptVM()
	{
		return m_pScriptVM;
	}
	void setFromMud(bool bfrom)
	{
		m_bStartFromMud = bfrom;
	}
	
	void addRightBtnCallBack( BTN_FUN p );
	void addLeftBtnCallBack( BTN_FUN p );
	void unRightBtnCallBack( BTN_FUN p );
	void unLeftBtnCallBack( BTN_FUN p );

	void PrintRunningInfo();

	void FlashWindow();
	void StopFlashWindow();

	void sendFileToFtp( const char* srcFile, const char* destFilePath, const char* destFileName );
	void registerSelfHandler( bool bReg = false );

	bool checkInstanceNum();
	void InitMachineID();
	const char* getMachineID();

public:
	void*					m_hWnd; // 将窗口句柄暴露
	void*					m_hWebWnd;
private:
	bool LoadConfigFile(Ogre::XMLData *pxmldata);
	void LoadServerListConfig(const char* fileName);
	bool InitEngine();
	void onWindowSize(int width, int height);

	void loadAdditionalScript();

	void sendDataToFtp( const char* data, const char* destFilePath, const char* destFileName , unsigned int length );

	void getMacString( char* szMacAdress );
	void getCpuId( char* szCPUId );
	void getHdSerialNum( char* serialNum );
	void createDownloadAdThread();

protected:
	void*					m_hInstance;
	GameConfig				m_Config;
	SystemSettingConfig		m_SystemSettingConfig;
	SystemSettingConfig		m_LatestSystemSettingConfig;
	Ogre::UIRenderer*		m_pUIRenderer;
	Ogre::SceneRenderer*	m_pMainRenderer;
	GameBackground *        m_pBackground;
	Ogre::ScriptVM*			m_pScriptVM;
	GameUI*					m_pGameUI;
	FrameTick*				m_pFrameTick;
	int						m_CurMapID;
	INetworkManager*		m_pNetMgr;
	LoginManager*			m_pLoginMgr;
	ChatManager*			m_pChatMgr;
	GameMsgPrint*			m_pGameMsgPrint;
	GameUIMsg*				m_pGameUIMsg;
	GameShortCut*			m_pGameShortCut;
	GameMapShower*          m_pGameMapShower;
	GameNpcDialog*			m_pNpcDialog;
	GameActionManager*		m_pActionManager;
	GameTeamManager*		m_pTeamManager;
	AutoTeamManager*		m_pAutoTeamManager;
	GameNpcShop*			m_pNpcShop;
	GameQuest*				m_pQuest;
	GameWizard*				m_pGameWizard;
	GameCollect*			m_pGameCollect;
	GameSkillLearn*			m_pGameSkillLearn;
	GameFriendManager*		m_pGameFriendManager;
	GameExchange*			m_pGameExchange;
	GameDynPWorld*			m_pGameDynPWorld;
	GameClanManager*		m_pClanManager;	
	GameMapSendingManager*	m_pMapSendingManager;
	GameTrust*				m_pTrust;
	GameMailManager*		m_pGameMailManager;
	GameStallManager*		m_pGameStallManager;
	GameFollowManager*		m_pGameFollowManager;		
	GameArmProce*			m_pGameArmProce;
	GamePromotion*			m_pGamePromotion;
	GameWatchPlayer*		m_pGameWatchPlayer;
	GameDesignation*		m_pGameDesignation;
    GameRespact*            m_pGameRespact;
	CMarketManager*			m_pGameMarket;
	GameUserConfig*			m_pGameUserConfig;
	ResoucePreLoadManager*	m_pPreLoadMgr;	//资源预加载类
	GameRankManager*		m_pGameRankMgr;
	SecondarySkillMgr*		m_pSecondarySkillMgr;
	GameBattle*				m_pGameBattle;
	GameWarfogManager*      m_pGameWarfogMgr;
	GameRollManager*		m_pRollMgr;
	GameRuneManager*		m_pRuneMgr;

	ActorManager*			m_pActorMgr;
	SCTDamageManager*		m_pSCTMgr;
	GamePlayManager*		m_pGamePlayMgr;
	SkillManager*			m_pGameSkillMgr;	
	DBDataManager*			m_pDBMgr;
	Ogre::InputManager*		m_pInputMgr;
	Ogre::MCIWrap*			m_pMCIWrap;
	GameSimulateManager*	m_pSimulateMgr;
	PworldDifficulty *		m_pPworldDifficulty;
	PworldScore *			m_pPworldScore;
	BulletManager *			m_pBulletMgr;
	DreamLandManager *		m_pDreamLandMgr;
	AutoAssist *			m_pGameAutoAssist;
	GamePetLand*			m_pGamePetLand;
	GameClan_BaseWar *		m_pGameClan_BaseWar;
	GameIntelliRaceMgr *	m_pGameIntelliRaceMgr;
	AutoPworldManager*		m_pAutoPworldManager;
	MagicSmelterManager*	m_pMagicSmelterManager;
	GameGetAccountAllRole*	m_pGameGetAccountAllRole;
	GameGeniusManager*		m_pGeniusManager;
	GameBusyManager*		m_pBusyManager;
	GamePoly*				m_pGamePoly;
	GameVipManager*			m_pVipManager;
	GameClanBoon*			m_pClanBoon;
	GameSparta*				m_pGameSparta;
	GameSchool*				m_pGameSchool;
	GameCardBox*			m_pGameCardBox;
	GameStoreManager*		m_pGameStoreManager;
	GameExerciseManager*	m_pGameExerciseMgr;
	ExtraActorMgr*			m_pGameExActorMgr;
	GameTopCareer*			m_pGameTopCareer;
	GameClan_SpanBaseWar*		m_pGameSpanBaseWar;
	GameHomelandManager*	m_pGameHomelandManager;
	GameShiKong*				m_pGameShiKong;

	int						m_ViewportWidth;
	int						m_ViewportHeight;

	bool m_bFocus;
	bool m_isFirstRun;

	bool m_lastVideoPlayed;
	bool m_bStartFromMud;
	std::list<BTN_FUN> m_lRightBtnCallBack;
	std::list<BTN_FUN> m_lLeftBtnCallBack;
	static SystemSettingConfig	m_sDefaultSysSetting;
	void*					m_hMutex;
	void*					m_hInstMutex;
	char					m_szMachineID[128];

	bool m_bIsCreated;
	bool m_bTestMode;

	int	m_nMaxLevel;
	int m_nLevelFeng;							//等级封印
};//tolua_export

#endif
