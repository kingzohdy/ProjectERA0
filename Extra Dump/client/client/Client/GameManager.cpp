#include "SMailer.h"
#include "WinSockHelper.h"
#include <memory>
#include "GameManager.h"
#include "FrameTick.h"
#include "GameLoginManager.h"
#include "GamePlayManager.h"
#include "GameChatManager.h"
#include "GameSkill_Mgr.h"
#include "GameEffect_Mgr.h"
#include "GameActor.h"
#include "GameDBData.h"
#include "GameCamera.h"
#include "GameUIMsg.h"
#include "GameShortCut.h"
#include "GameMaxMap.h"
#include "GameRuneManager.h"
#include "GameNpcDialog.h"
#include "GameActionManager.h"
#include "GameTeamManager.h"
#include "GameNpcShop.h"
#include "GameQuest.h"
#include "GameCollect.h"
#include "GameSkillLearn.h"
#include "GameFriendManager.h"
#include "GameBackgroundScene.h"
#include "Equip.h"
#include "GameFollowManager.h"
#include "GameTickAction.h"
#include "GameWarfogManager.h"
#include "ItemPicker.h"
#include "GameSimulateManager.h"
#include "PworldDifficulty.h"
#include "PworldScore.h"
#include "WorldCity.h"
#include "DreamLand.h"

#include "../UILib/ui_gameui.h"
#include "../UILib/ui_common.h"
#include "../UILib/ui_framemgr.h"

#include "OgreScriptLuaVM.h"
#include "OgreRoot.h"
#include "OgreLog.h"
#include "OgreXMLData.h"
#include "OgreUIRenderer.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreInputManager.h"
#include "OgreMCI.h"
#include "OgreOSUtility.h"
#include "OgreResourceManager.h"
#include "OgreCamera.h"
#include "OgreSoundSystem.h"
#include "OgreOSUtility.h"
#include "ShowGameMap.h"
#include "GameMsgPrint.h"
#include "GameStateManager.h"
#include "GameClientOpt.h"
#include "GameWizard.h"
#include "GameExchange.h"
#include "GameDynPWorld.h"
#include "GameClanManager.h"
#include "GameTrust.h"
#include "GameMailManager.h"
#include "GameStallManager.h"
#include "GameArmProce.h"
#include "GameSelectManager.h"
#include "GamePromotion.h"
#include "GameWatchPlayer.h"
#include "GameDesignation.h"
#include "GameRespact.h"
#include "GameSCTDamageManager.h"
#include "GameMarketManager.h"
#include "GameActor_PVP.h"
#include "GameUserConfig.h"
#include "GameDamageHealCounter.h"
#include "ResourcePreLoadManager.h"
#include "ResourceCheck.h"
#include "GameRankManager.h"
#include "PworldLimitInfo.h"
#include "BattleLimitInfo.h"
#include "AutoTeam.h"
#include "GameBCT.h"
#include "MapLineSelectManager.h"
#include "GameWheelLottery.h"
#include "SecondarySkillMgr.h"
#include "GameBattle.h"
#include "GameMainPlayer.h"
#include "GameKeyMoveOperate.h"
#include "GameMapSendingManager.h"
#include "GamePetLand.h"
#include "GameGetAccountAllRole.h"
#include "GameGeniusManager.h"
#include "GameVipManager.h"
#include "GameClanBoon.h"

#include "ScriptFunctions.h"
#include "ScriptTickAction.h"
#include "GameRollManager.h"
#include "Splash.h"
#include "des.h"

#include <Windows.h>
#include <string>

#include "OgreRenderSystem.h"
#include "OgreDeathScene.h"

#include "ogrebloom.h"
#include <direct.h>
#include "GameAutoAssist.h"
#include "GameIntelliRaceMgr.h"
#include "AutoPworld.h"
#include "MagicSmelter.h"
#include "GameBusyManager.h"
#include "GamePoly.h"
#include "GameSparta.h"
#include "GameSchool.h"
#include "GameCardBox.h"
#include "GameStoreManager.h"
#include "GameExerciseManager.h"
#include "GameExActorMgr.h"
#include "GameTopCareer.h"
#include "GameClan_SpanBaseWar.h"
#include "GameHomelandManager.h"
#include "GameShiKong.h"

#include "CallTime.h"
#include "BulletManager.h"
#include "MapOre.h"
#include "OgreStringUtil.h"

#include "ScreenCap.h"
#include "disphelper.h"
#include <Iphlpapi.h>
#include <WinInet.h>
#include "Ping.h"
#include <fstream>
#pragma comment(lib, "Iphlpapi.lib")
using namespace std;
using namespace Ogre;

extern int  tolua_GameEngineTolua_open (lua_State* tolua_S);
extern int  tolua_ClientTolua_open (lua_State* tolua_S);

static GameClientOpt g_GameOpt;

static void PreSceneRenderUI()
{
	FollowCamera *pcamera = GamePlayManager::getSingleton().getMainCamera();
	if( pcamera != NULL)
	{
		if( !pcamera->isPlayCameraAnim() && GamePlayManager::getSingleton().isMapLoadOK() )
		{
			ActorManager::getSingleton().drawActorTitles(pcamera);
			ExtraActorMgr::getSingleton().drawActorTitles(pcamera);
			WorldCityManager::getSingleton().draw(pcamera);
			GameTopCareer::getSingleton().draw(pcamera);
		}
		GameManager::getSingleton().GetGameUI()->Render();

	}
}

static void checkWindowSize( RECT& destRect )
{
	RECT rt;
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &rt, 0 );

	int nWorkAreaWidth	= rt.right - rt.left;
	int curWidth		= destRect.right - destRect.left;
	if ( curWidth > nWorkAreaWidth )
	{
		destRect.left	= 0;
		destRect.right	= nWorkAreaWidth;
	}

	int nWorkAreaHeight = rt.bottom - rt.top;
	int curHeight		= destRect.bottom - destRect.top;
	if ( curHeight > nWorkAreaHeight )
	{
		destRect.top	= 0;
		destRect.bottom	= nWorkAreaHeight;
	}
}

static void checkWindowPos( RECT& destRect )
{
	int nWidth	= destRect.right	- destRect.left;
	if ( destRect.left < 0 )
	{
		destRect.left	= 0;
		destRect.right	= nWidth;
	}
	
	RECT rt;
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &rt, 0 );

	int nWorkAreaWidth	= rt.right - rt.left;
	if ( destRect.right > nWorkAreaWidth )
	{
		destRect.right	= nWorkAreaWidth;
		destRect.left	= destRect.right - nWidth;
	}
	
	int nHeight	= destRect.bottom	- destRect.top;
	if ( destRect.top < 0 )
	{
		destRect.top	= 0;
		destRect.bottom	= nHeight;
	}
	
	int nWorkAreaHeight	= rt.bottom - rt.top;
	if ( destRect.bottom > nWorkAreaHeight )
	{
		destRect.bottom	= nWorkAreaHeight;
		destRect.top	= destRect.bottom - nHeight;
	}
}

inline FILE* OpenOSF( long h, const char* szFlag )
{
	int osfHandle = _open_osfhandle( h, _O_TEXT );

	FILE *file = _fdopen( osfHandle, szFlag );

	setvbuf(file, NULL, _IONBF, 0);
	return file;
}

SystemSettingConfig	GameManager::m_sDefaultSysSetting;

GameManager::GameManager()
	:	m_pScriptVM(NULL), 
		m_pGameUI(NULL), 
		m_pFrameTick(NULL), 
		m_CurMapID(-1), 
		m_pNetMgr(NULL), 
		m_pLoginMgr(NULL), 
		m_pChatMgr(NULL),
		m_pGameUIMsg(NULL),
		m_pActorMgr(NULL), 
		m_pSCTMgr(NULL),
		m_pInputMgr(NULL), 
		m_pDBMgr(NULL), 
		m_pUIRenderer(NULL), 
		m_pGamePlayMgr(NULL),
		m_pGameMapShower(NULL),
		m_pGamePromotion(NULL),
		m_pGameWatchPlayer(NULL),
		m_pGameDesignation(NULL),
		m_pGameRespact(NULL),
		m_pGameRankMgr(NULL),
		m_pSecondarySkillMgr(NULL),
		m_pRollMgr(NULL),
		m_bFocus(true),
		m_bIsCreated(false),
		m_lastVideoPlayed(false),
		m_bStartFromMud(false),
		m_hMutex(NULL),
		m_hInstMutex(NULL),
		m_pGamePetLand(NULL),
		m_bTestMode(false),
		m_nMaxLevel(0),
		m_nLevelFeng(0)
{
	m_szMachineID[0] = '\0';
}

GameManager::~GameManager()
{
}

static LONG s_DeskWorkW = 0;
static LONG s_DeskWorkH = 0;
static string config_filename;
bool GameManager::Create(const char *config_file, void *hinst,const char *pszdirectmud)
{
	config_filename = config_file;
	TraceTime::SetMainThreadID(GetCurrentThreadId());
	InitDisplayMode();
	InitMachineID();
	//RECT workarea;
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
	//s_DeskWorkW = workarea.right-workarea.left;
	//s_DeskWorkH = workarea.bottom-workarea.top;

	s_DeskWorkW = (LONG)::GetSystemMetrics(SM_CXSCREEN);
	s_DeskWorkH = (LONG)::GetSystemMetrics(SM_CYSCREEN);

	m_hInstance = hinst;
	Root *proot = new Root(config_file);
	if( proot->IsSmartClient() )
	{
		// 看看那个进程是否存在
		HANDLE hFileMapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, "{E7F8A607-89C0-4689-8C76-B7D28EA56E08}");
		if( hFileMapping != NULL )
		{
			//关闭后台下载
			struct MapData 
			{
				bool bStop;
				unsigned int processId;
			};
			unsigned int processId = -1;
			MapData* pBuf = (MapData*)MapViewOfFile(hFileMapping, FILE_MAP_WRITE|FILE_MAP_READ, 0, 0, sizeof(bool));
			if(pBuf != NULL)
			{
				pBuf->bStop = true;
				processId = pBuf->processId;
			}
			UnmapViewOfFile( pBuf );
			// 等待下载进程结束
			HANDLE process = OpenProcess( PROCESS_ALL_ACCESS , FALSE, processId );
			WaitForSingleObject( process, INFINITE );
			CloseHandle( process );
			CloseHandle( hFileMapping );
			hFileMapping = NULL;
		}
		m_hMutex = CreateMutex(NULL, FALSE, "{EF6C4BE0-ADCF-445e-8425-E883C5AA8B9C}" );
	}
	LoadServerListConfig("serverlist.data");
	proot->Initlize();

	createDownloadAdThread();

	char szMac[128];
	getMacString(szMac);
	LOG_INFO("start game, current version is %s,\t mac address is %s\n",uint_to_version( getLocalVersion() ),szMac);

	// 先加载游戏的默认设置和数据库方面的设置信息
	if ( !LoadConfigFile( &proot->m_Config ) )
	{
		return false;
	}


	// 再加载游戏的系统设置
	if ( !m_SystemSettingConfig.LoadSystemSettingFile( &proot->m_Config ) )
	{
		return false;
	}

	m_LatestSystemSettingConfig = m_SystemSettingConfig;
	
#ifdef _WEB
#else
	UpdateScreenSplash("初始化数据库...",40);
#endif
	//Sleep(20000);
	InitDB();
	m_pInputMgr			= new InputManager;	
	m_pMCIWrap			= new MCIWrap;
	m_pGameMsgPrint		= new GameMsgPrint;
	m_pNetMgr			= INetworkManager::Create();
	m_pLoginMgr			= LoginManager::Create();
	m_pChatMgr			= ChatManager::Create();
	m_pGameUIMsg		= GameUIMsg::Create();
	m_pGamePromotion	= GamePromotion::create("");

#ifdef _WEB
#else
	UpdateScreenSplash("初始化引擎...",60);
#endif
	
	if(!InitEngine())
	{
		MessageBox( NULL, TEXT( "创建设备失败，无法运行游戏"  ), NULL, 0 );
		return false;
	}

	NormalSceneRenderer *scenerenderer = new NormalSceneRenderer();

	//ColorQuad colorQ(118, 65, 26);
	ColorQuad colorQ(0, 0, 0);
	scenerenderer->setClearParams(CLEAR_TARGET|CLEAR_ZBUFFER, colorQ.c, 1.0f, 0);
	//scenerenderer->setClearParams(CLEAR_TARGET|CLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
	scenerenderer->setRenderTarget(RenderSystem::getSingleton().getMainWindow());
	scenerenderer->setCamera(NULL);
	scenerenderer->setRenderScene(NULL);
	m_pMainRenderer = scenerenderer;
	SceneManager::getSingleton().addSceneRenderer(0, scenerenderer);

	m_pUIRenderer = new UIRenderer;
	m_pUIRenderer->setPreRenderCallback(PreSceneRenderUI);
	SceneManager::getSingleton().addSceneRenderer(1, m_pUIRenderer);

	m_pBackground = new GameBackground(10);
	//m_pBackground = new GameBackground(0);

	m_pScriptVM = new ScriptVM;
	tolua_ClientTolua_open(m_pScriptVM->getLuaState());
	tolua_GameEngineTolua_open(m_pScriptVM->getLuaState());
	ScriptFunctions::getSingleton().registerToLua(m_pScriptVM->getLuaState());
	scriptTickRegisterToLua(m_pScriptVM->getLuaState());

	m_pScriptVM->setUserTypePointer("GameMgr",			"GameManager",		this );
	m_pScriptVM->setUserTypePointer("GameBackground",	"GameBackground",	m_pBackground);
	m_pScriptVM->setUserTypePointer("LoginMgr",			"LoginManager",		m_pLoginMgr);
	m_pScriptVM->setUserTypePointer("ChatManager",		"ChatManager",		m_pChatMgr);
	m_pScriptVM->setUserTypePointer("UIMSG",			"GameUIMsg",		m_pGameUIMsg);
	m_pScriptVM->setUserTypePointer("GamePromotion",	"GamePromotion",	m_pGamePromotion);

	m_pFrameTick = new FrameTick;

	m_pActorMgr				= new ActorManager;
	m_pSCTMgr				= new SCTDamageManager;
	m_pGamePlayMgr			= new GamePlayManager;
	m_pGameSkillMgr			= new SkillManager;
	m_pGameShortCut			= new GameShortCut;
	m_pGameMapShower		= new GameMapShower;
	m_pNpcDialog			= new GameNpcDialog;
	m_pActionManager		= new GameActionManager;
	m_pTeamManager			= new GameTeamManager;
	m_pAutoTeamManager		= new AutoTeamManager;
	m_pNpcShop				= new GameNpcShop;
	m_pQuest				= new GameQuest;
	m_pGameWizard			= new GameWizard;
	m_pGameCollect			= new GameCollect;
	m_pGameSkillLearn		= new GameSkillLearn;
	m_pGameFriendManager	= new GameFriendManager;
	m_pGameExchange			= new GameExchange;
	m_pGameDynPWorld		= new GameDynPWorld;
	m_pClanManager			= new GameClanManager;
	m_pTrust				= new GameTrust;
	m_pGameMailManager		= new GameMailManager;
	m_pGameStallManager		= new GameStallManager;
	m_pGameFollowManager	= new GameFollowManager;
	m_pGameArmProce			= new GameArmProce;
	m_pGameWatchPlayer		= new GameWatchPlayer;
	m_pGameDesignation		= new GameDesignation;
	m_pGameRespact          = new GameRespact;
	m_pGameMarket			= new CMarketManager;
	m_pGameMarket->create();
	m_pGameUserConfig		= new GameUserConfig;
	m_pGameRankMgr			= new GameRankManager;
	m_pSecondarySkillMgr	= new SecondarySkillMgr;
	m_pGameBattle			= new GameBattle;
	m_pGameWarfogMgr		= new GameWarfogManager;
	m_pRollMgr				= new GameRollManager;
	m_pRuneMgr				= new GameRuneManager;
	m_pSimulateMgr			= new GameSimulateManager;
	m_pPworldDifficulty		= new PworldDifficulty;
	m_pPworldScore			= new PworldScore;
	m_pBulletMgr			= new BulletManager;
	m_pMapSendingManager	= new GameMapSendingManager;
	m_pDreamLandMgr			= new DreamLandManager;
	m_pGameAutoAssist		= new AutoAssist;
	m_pGamePetLand			= new GamePetLand;
	m_pGameClan_BaseWar		= new GameClan_BaseWar;
	m_pGameIntelliRaceMgr	= new GameIntelliRaceMgr;
	m_pAutoPworldManager	= new AutoPworldManager;
	m_pMagicSmelterManager	= new MagicSmelterManager;
	m_pGameGetAccountAllRole = new GameGetAccountAllRole;
	m_pGeniusManager		 = new GameGeniusManager;
	m_pBusyManager			= new GameBusyManager;
	m_pGamePoly				= new GamePoly;
	m_pVipManager			= new GameVipManager;
	m_pGameSparta			= new GameSparta;
	m_pClanBoon				= new GameClanBoon;
	m_pGameSchool			= new GameSchool;
	m_pGameCardBox			= new GameCardBox;
	m_pGameStoreManager		= new GameStoreManager;
	m_pGameExerciseMgr		= new GameExerciseManager;
	m_pGameExActorMgr		= new ExtraActorMgr;
	m_pGameTopCareer		= new GameTopCareer;
	m_pGameSpanBaseWar			= new GameClan_SpanBaseWar;
	m_pGameHomelandManager	= new GameHomelandManager;
	m_pGameShiKong			= new GameShiKong;

	m_pScriptVM->setUserTypePointer("GameCollect",			"GameCollect",			m_pGameCollect);
	m_pScriptVM->setUserTypePointer("ActorMgr",				"ActorManager",			m_pActorMgr);
	m_pScriptVM->setUserTypePointer("GameShortCut",			"GameShortCut",			m_pGameShortCut);
	m_pScriptVM->setUserTypePointer("MapShower",			"GameMapShower",		m_pGameMapShower);
	m_pScriptVM->setUserTypePointer("SkillMgr",				"SkillManager",			m_pGameSkillMgr);
	m_pScriptVM->setUserTypePointer("RuneMgr",				"GameRuneManager",		m_pRuneMgr);
	m_pScriptVM->setUserTypePointer("NpcDialog",			"GameNpcDialog",		m_pNpcDialog);
	m_pScriptVM->setUserTypePointer("NpcShop",				"GameNpcShop",			m_pNpcShop);
	m_pScriptVM->setUserTypePointer("TeamManager",			"GameTeamManager",		m_pTeamManager);
	m_pScriptVM->setUserTypePointer("AutoTeamManager",		"AutoTeamManager",		m_pAutoTeamManager);
	m_pScriptVM->setUserTypePointer("Quest",				"GameQuest",			m_pQuest);
	m_pScriptVM->setUserTypePointer("GameWizard",			"GameWizard",			m_pGameWizard);
	m_pScriptVM->setUserTypePointer("GameSkillLearn",		"GameSkillLearn",		m_pGameSkillLearn);
	m_pScriptVM->setUserTypePointer("GameFriendManager",	"GameFriendManager",	m_pGameFriendManager);
	m_pScriptVM->setUserTypePointer("GameExchange",			"GameExchange",			m_pGameExchange);
	m_pScriptVM->setUserTypePointer("GameDynPWorld",		"GameDynPWorld",		m_pGameDynPWorld);
	m_pScriptVM->setUserTypePointer("GameClanManager",		"GameClanManager",		m_pClanManager);
	m_pScriptVM->setUserTypePointer("GameTrust",			"GameTrust",			m_pTrust);
	m_pScriptVM->setUserTypePointer("GameMailManager",		"GameMailManager",		m_pGameMailManager);
	m_pScriptVM->setUserTypePointer("GameStallManager",		"GameStallManager",		m_pGameStallManager);
	m_pScriptVM->setUserTypePointer("GameFollowManager",	"GameFollowManager",	m_pGameFollowManager);
	m_pScriptVM->setUserTypePointer("GameArmProce",			"GameArmProce",			m_pGameArmProce);
	m_pScriptVM->setUserTypePointer("GameWatchPlayer",		"GameWatchPlayer",		m_pGameWatchPlayer);
	m_pScriptVM->setUserTypePointer("GameDesignation",		"GameDesignation",		m_pGameDesignation);
	m_pScriptVM->setUserTypePointer("GameRespact",		    "GameRespact",		    m_pGameRespact);
	m_pScriptVM->setUserTypePointer("MarketManager",		"CMarketManager",		m_pGameMarket);
	m_pScriptVM->setUserTypePointer("GameActorPVP",			"GameActor_PVP",		&GameActor_PVP::getSingleton());
	m_pScriptVM->setUserTypePointer("GameDamageHealCounter",	"GameDamageHealCounter",	&GameDamageHealCounter::getSingleton());
	m_pScriptVM->setUserTypePointer("RankMgr",		"GameRankManager",			m_pGameRankMgr);
	m_pScriptVM->setUserTypePointer("PworldLimitInfo",		"PworldLimitInfo",		&PworldLimitInfo::getSingleton());
	m_pScriptVM->setUserTypePointer("BattleLimitInfo",		"BattleLimitInfo",		&BattleLimitInfo::getSingleton());
	m_pScriptVM->setUserTypePointer("GameBCT",				"GameBCT",				&GameBCT::getSingleton());
	m_pScriptVM->setUserTypePointer("GameWheelLottery",		"GameWheelLottery",		&GameWheelLottery::getSingleton());
	m_pScriptVM->setUserTypePointer("SecondarySkillMgr",	"SecondarySkillMgr",	m_pSecondarySkillMgr);
	m_pScriptVM->setUserTypePointer("GameUserConfig",		"GameUserConfig",		m_pGameUserConfig);
	m_pScriptVM->setUserTypePointer("MapLineSelecter",		"MapLineSelectManager",	&MapLineSelectManager::getSingleton());
	m_pScriptVM->setUserTypePointer("GameBattle",			"GameBattle",			m_pGameBattle);
	m_pScriptVM->setUserTypePointer("EffectManage",			"EffectManage",			&EffectManage::getSingleton());
	m_pScriptVM->setUserTypePointer("GameWarfogMgr",		"GameWarfogManager",	m_pGameWarfogMgr);
	m_pScriptVM->setUserTypePointer("GameRollMgr",			"GameRollManager",		m_pRollMgr);
	m_pScriptVM->setUserTypePointer("ItemPicker",			"ItemPicker",			&ItemPicker::getSingleton());
	m_pScriptVM->setUserTypePointer("KeyMoveOpt",			"GameKeyMoveOperate",	&GameKeyMoveOperate::getSingleton());
	m_pScriptVM->setUserTypePointer("SimulateMgr",			"GameSimulateManager",	m_pSimulateMgr);
	m_pScriptVM->setUserTypePointer("PworldDifficulty",		"PworldDifficulty",		m_pPworldDifficulty);
	m_pScriptVM->setUserTypePointer("PworldScore",			"PworldScore",			m_pPworldScore);
	m_pScriptVM->setUserTypePointer("GameMapSendingManager","GameMapSendingManager",m_pMapSendingManager);
	m_pScriptVM->setUserTypePointer("WorldCityManager",		"WorldCityManager",		&WorldCityManager::getSingleton());
	m_pScriptVM->setUserTypePointer("DreamLandMgr",			"DreamLandManager",		m_pDreamLandMgr);
	m_pScriptVM->setUserTypePointer("MapOre",				"MapOreInfo",			&MapOreInfo::getSingleton());
	m_pScriptVM->setUserTypePointer("AutoAssist",			"AutoAssist",			m_pGameAutoAssist);
	m_pScriptVM->setUserTypePointer("GamePetLand",			"GamePetLand",			m_pGamePetLand);
	m_pScriptVM->setUserTypePointer("GameClan_BaseWar",     "GameClan_BaseWar",		m_pGameClan_BaseWar);
	m_pScriptVM->setUserTypePointer("IntelliRaceMgr",       "GameIntelliRaceMgr",   m_pGameIntelliRaceMgr);
	m_pScriptVM->setUserTypePointer("AutoPworldManager",	"AutoPworldManager",	m_pAutoPworldManager);
	m_pScriptVM->setUserTypePointer("MagicSmelterManager",	"MagicSmelterManager",	m_pMagicSmelterManager);
	m_pScriptVM->setUserTypePointer("GameGetAccountAllRole", "GameGetAccountAllRole", m_pGameGetAccountAllRole);
	m_pScriptVM->setUserTypePointer( "GeniusMgr",			"GameGeniusManager",	m_pGeniusManager );
	m_pScriptVM->setUserTypePointer( "BusyMgr",				"GameBusyManager",		m_pBusyManager );
	m_pScriptVM->setUserTypePointer( "GamePoly",			"GamePoly",				m_pGamePoly );
	m_pScriptVM->setUserTypePointer( "VipManager",			"GameVipManager",		m_pVipManager );
	m_pScriptVM->setUserTypePointer( "GameSparta",			"GameSparta",			m_pGameSparta );
	m_pScriptVM->setUserTypePointer( "ClanBoon",			"GameClanBoon",			m_pClanBoon );
	m_pScriptVM->setUserTypePointer( "GameSchool",			"GameSchool",			m_pGameSchool );
	m_pScriptVM->setUserTypePointer( "CardBox",				"GameCardBox",			m_pGameCardBox );
	m_pScriptVM->setUserTypePointer( "StoreMgr",			"GameStoreManager",		m_pGameStoreManager );
	m_pScriptVM->setUserTypePointer( "ExerciseManager",		"GameExerciseManager",	m_pGameExerciseMgr );
	m_pScriptVM->setUserTypePointer( "ExtraActorMgr",		"ExtraActorMgr",		m_pGameExActorMgr );
	m_pScriptVM->setUserTypePointer( "GameTopCareer",		"GameTopCareer",		m_pGameTopCareer );
	m_pScriptVM->setUserTypePointer( "GameClan_SpanBaseWar",		"GameClan_SpanBaseWar",		m_pGameSpanBaseWar );
	m_pScriptVM->setUserTypePointer( "GameHomelandManager",		"GameHomelandManager",		m_pGameHomelandManager );
	m_pScriptVM->setUserTypePointer( "GameShiKong",		"GameShiKong",		m_pGameShiKong );
	m_pGameMapShower->initMapInfo();

#ifdef _WEB
#else
	UpdateScreenSplash("初始化界面...",90);
#endif

	m_pGameUI = new GameUI;
	
	RECT rect;
	::GetClientRect((HWND)m_hWnd, &rect);
	if(!m_pGameUI->Create("uires\\default.toc", rect.right, rect.bottom, m_pUIRenderer, m_pScriptVM))
	{
		LOG_SEVERE("create GameUI failed");
		return false;
	}
	m_pScriptVM->setUserTypePointer("FrameMgr",				"FrameManager",			g_pFrameMgr);
	m_pGameWarfogMgr->initTexture();
	g_pFrameMgr->UpdateAllSelfScale( m_SystemSettingConfig.m_fUIScale );
	LOG_INFO("create GameUI succeeded");
	g_pFrameMgr->setWndHandle( m_hWnd );

	m_pInputMgr->RegisterInputHandler(m_pGameUI);
	m_pInputMgr->RegisterInputHandler(m_pGamePlayMgr);
	m_pActorMgr->setScriptVM(m_pScriptVM);

	m_pGameMsgPrint->registGameMsgHandler( this );

	if( (float)m_SystemSettingConfig.m_nWinHeight <= DEFAULT_UI_HEIGHT )
	{
		g_pFrameMgr->UpdateGameFont( ( float )m_SystemSettingConfig.m_nWinHeight	/ DEFAULT_UI_HEIGHT * g_pFrameMgr->GetAllSelfScale() );
		g_pFrameMgr->SetScreenScaleX( ( float )m_SystemSettingConfig.m_nWinWidth	/ DEFAULT_UI_WIDTH	);
		g_pFrameMgr->SetScreenScaleY( ( float )m_SystemSettingConfig.m_nWinHeight	/ DEFAULT_UI_HEIGHT );
	}

#ifdef _WEB
#else
	UpdateScreenSplash("初始化登录界面资源...",100);
#endif

	g_pFrameMgr->m_bUpdate = true;
	
	// 载入装备套装数据
	DBDataManager::getSingleton().loadSuitSet( Equip::m_mapSuitSet );
	GamePet::loadPetConfig();

	// 设置是否写入CompliedShader
	RenderSystem::getSingleton().SetSaveCompiledShaders(m_SystemSettingConfig.m_bWriteCompiledShader);

	//m_pPreLoadMgr	= new ResoucePreLoadManager;
	//m_pPreLoadMgr->loadPublicMotions();

	//添加 加载额外的GM 指令包
	loadAdditionalScript();
	
	m_pFrameTick->setFrameLimit( m_SystemSettingConfig.m_nFPS );
	LOG_INFO("GameManager create succeeded");

 	if( isFirstRun() )
	{
		SendUIEvent("GE_APP_START");
		m_pScriptVM->callString("GameStart()");
	}else{
		if ( m_bStartFromMud )
		{
			if (!m_pLoginMgr->requestLoginGameServerDirect(pszdirectmud))
			{
				SendUIEvent("GE_APP_START");
			}
		}
		else
		{
			SendUIEvent("GE_APP_START");
		}
		m_pScriptVM->callString("GameStart()");
	}
	m_pBackground->show();
	//修改推荐配置
	m_sDefaultSysSetting.m_bBloom = false;
	m_sDefaultSysSetting.m_bDistort = true;
	m_sDefaultSysSetting.m_bWaterReflect = true;
	m_sDefaultSysSetting.m_nShadowmapSize = 0;
	m_sDefaultSysSetting.m_bWriteCompiledShader = true;
	return true;
}

void GameManager::SetCreated (bool bCreated)
{
	m_bIsCreated = bCreated;
}

bool GameManager::IsCreated ()
{
	return m_bIsCreated;
}

void GameManager::Destroy()
{
	LOG_INFO("GameMananger begin to destroy");
	if (isVideoPlaying())
	{
		clearVideoTag();
	}

	SAFE_DELETE(m_pMapSendingManager);
	SAFE_DELETE( m_pBulletMgr );
	SAFE_DELETE( m_pGamePlayMgr );
	SAFE_DELETE( m_pActorMgr );
	SAFE_DELETE( m_pSCTMgr );
	SAFE_DELETE( m_pGameSkillMgr );
	SAFE_DELETE( m_pLoginMgr );
	SAFE_DELETE( m_pChatMgr );
	SAFE_DELETE( m_pNetMgr );
	SAFE_DELETE( m_pInputMgr );
	SAFE_DELETE( m_pMCIWrap );
	SAFE_DELETE(m_pGameWarfogMgr);
	SAFE_DELETE( m_pGameBattle );
	SAFE_DELETE( m_pGameMsgPrint );
	SAFE_DELETE( m_pGameUIMsg );
	SAFE_DELETE( m_pFrameTick );
	SAFE_DELETE( m_pGameShortCut );
	SAFE_DELETE( m_pGameMapShower );
	SAFE_DELETE( m_pNpcDialog );
	SAFE_DELETE( m_pActionManager );
	SAFE_DELETE( m_pTeamManager );
	SAFE_DELETE( m_pAutoTeamManager );
	SAFE_DELETE( m_pNpcShop );
	SAFE_DELETE( m_pQuest );
	SAFE_DELETE( m_pGameCollect );
	SAFE_DELETE( m_pGameSkillLearn );
	SAFE_DELETE( m_pGameFriendManager );
	SAFE_DELETE( m_pGameExchange ); 
	SAFE_DELETE( m_pGameDynPWorld );
	SAFE_DELETE( m_pClanManager );
	SAFE_DELETE( m_pTrust );
	SAFE_DELETE( m_pGameMailManager );
	SAFE_DELETE( m_pGameStallManager );
	SAFE_DELETE( m_pGameArmProce );
	SAFE_DELETE( m_pBackground );
	SAFE_DELETE( m_pGamePromotion );
	SAFE_DELETE( m_pGameWatchPlayer );
	SAFE_DELETE( m_pGameFollowManager );
	SAFE_DELETE( m_pGameWizard );
	SAFE_DELETE( m_pGameDesignation );
	SAFE_DELETE( m_pGameRespact);
	//SAFE_DELETE( m_pPreLoadMgr );
	SAFE_DELETE( m_pGameUserConfig );
	SAFE_DELETE( m_pGameMarket );
	SAFE_DELETE( m_pGameRankMgr );
	SAFE_DELETE( m_pSecondarySkillMgr );
	SAFE_DELETE( m_pRollMgr );
	SAFE_DELETE(m_pSimulateMgr);
	SAFE_DELETE( m_pRuneMgr );
	SAFE_DELETE( m_pPworldDifficulty );
	SAFE_DELETE( m_pPworldScore );
	SAFE_DELETE( m_pDreamLandMgr );
	SAFE_DELETE( m_pGameAutoAssist );
	SAFE_DELETE(m_pGameClan_BaseWar);
	SAFE_DELETE( m_pGamePetLand );
	SAFE_DELETE( m_pGameIntelliRaceMgr );
	SAFE_DELETE( m_pAutoPworldManager );
	SAFE_DELETE( m_pMagicSmelterManager );
	SAFE_DELETE( m_pGameGetAccountAllRole );
	SAFE_DELETE( m_pGeniusManager );
	SAFE_DELETE( m_pBusyManager );
	SAFE_DELETE( m_pGamePoly );
	SAFE_DELETE( m_pVipManager );
	SAFE_DELETE( m_pGameSparta );
	SAFE_DELETE( m_pClanBoon );
	SAFE_DELETE( m_pGameSchool );
	SAFE_DELETE( m_pGameCardBox );
	SAFE_DELETE( m_pGameStoreManager );
	SAFE_DELETE( m_pGameExerciseMgr );
	SAFE_DELETE( m_pGameExActorMgr );
	SAFE_DELETE( m_pGameTopCareer );
	SAFE_DELETE( m_pGameSpanBaseWar );
	SAFE_DELETE( m_pGameHomelandManager );
	SAFE_DELETE( m_pGameShiKong );

	if( m_hMutex )
	{
		CloseHandle( m_hMutex );
		m_hMutex = NULL;
	}
	if( m_hInstMutex )
	{
		CloseHandle( m_hInstMutex );
		m_hInstMutex = NULL;
	}

	delete m_pGameUI;

	LOG_INFO("delete GameUI");

	delete m_pScriptVM;

	delete m_pMainRenderer;

	LOG_INFO("delete BorderRenderer");

	delete m_pUIRenderer;
	LOG_INFO("delete UIRenderer");

	TraceTime::WirteToCSV();
	TraceTime::SetCSVData();
	TraceTime::EndTraceTime();

	delete Root::getSingletonPtr();

	delete m_pDBMgr;
}

void GameManager::DoFrame()
{
	unsigned int dtick = m_pFrameTick->DoFrame();
	float dtime = TickToTime(dtick);

	if (DeathGameScene::getSingletonPtr())
		DeathGameScene::getSingleton().update(dtick);

	if (BloomEffect::getSingletonPtr())
		BloomEffect::getSingletonPtr()->update(dtick);

	ResourceManager::getSingleton().update(dtick);

	int ret = m_pNetMgr->Dispatch();
	if( ret == AUTH_ERR_NET && GameStateManager::getGameState() != GameStateManager::STATE_DIRLIST )
	{
		INetworkManager::getSingleton().clearMsg();
		INetworkManager::getSingleton().SetBusy( false );
		SendUIEvent("GE_BREAK_NETWORK_TO_DIR");
		LoginManager::getSingleton().m_bOffLine = true;
	}
// 	if( GameStateManager::getGameState() == GameStateManager::STATE_INWORLD )
// 	{
// 		static unsigned int uLastRecieveTick = timeGetTime();
// 		if( ret != -1 )
// 		{
// 			uLastRecieveTick = timeGetTime();
// 		}else{
// 			if( timeGetTime() - uLastRecieveTick >=5000 )
// 			{
// 				SendUIEvent("GE_BREAK_NETWORK");
// 				LoginManager::getSingleton().m_bOffLine = true;
// 			}
// 		}
// 	}
	m_pInputMgr->Dispatch();
	m_pMCIWrap->update(dtick);
	
	if( !m_pMCIWrap->isPlaying() )
	{
		if( m_lastVideoPlayed ) 
		{
			m_lastVideoPlayed = false;

			m_pFrameTick->setFrameLimit( m_SystemSettingConfig.m_nFPS );
			clearVideoTag();
			if (m_bFocus)
			{
				SoundSystem::getSingleton().setPause(false);
			}

		}
		m_pGameUI->Update(dtime);
		m_pGameMapShower->update( dtime );
		SceneManager::getSingleton().doFrame();
		if( m_bFocus )
		{
			SoundSystem::getSingleton().update();
		}
	}else{
		g_pFrameMgr->m_pCurCursor->updateCursor();
	}

	m_pLoginMgr->update(dtick);

	if( GameStateManager::getGameState() == GameStateManager::STATE_MAPLOADING )
	{
		if( ActorManager::getSingleton().getMainPlayer() != NULL )
		{
			m_pGamePlayMgr->trySwapMap();
		}
	}
	if( GameStateManager::getGameState() == GameStateManager::STATE_INWORLD )
	{
		// InWorld
		GameTickActionManager::getSingleton().update();
		m_pGameSkillMgr->update(dtick);
		m_pGamePlayMgr->doFrame(dtick);	
		m_pNpcDialog->update(dtick);
		m_pGameStallManager->update( dtick );
		m_pGameExchange->update( dtick );
		m_pQuest->update(dtick);
		m_pGameCollect->update(dtick);
		m_pGameFollowManager->update(dtick);
		ScriptFunctions::getSingleton().update(dtick);
		EffectManage::getSingleton().update(dtick);
		m_pClanManager->update();
		m_pGameMarket->update();
		m_pGameWarfogMgr->update();
		BulletManager::getSingleton().Update(dtick);
		GameClientOpt::getSingleton().update(dtick);
		ChatManager::getSingleton().Update();
		m_pGamePromotion->Update();
		m_pGameHomelandManager->Update( dtick );
	}

	if (!m_pMCIWrap->isPlaying())
	{
		m_pBackground->update(dtick, 0, 0, m_ViewportWidth, m_ViewportHeight);
	}
	
	static bool bPrintfMsg = false;
	if( !bPrintfMsg && Ogre::RenderSystem::getSingletonPtr() != NULL && (GetKeyState(VK_SNAPSHOT) & 0xff00) ) 
	{
		SYSTEMTIME tt;
		GetLocalTime(&tt);

		char szPath[MAX_PATH];
		sprintf( szPath, "%04d-%02d-%02d_%d-%d-%d.jpg", tt.wYear, tt.wMonth, tt.wDay, tt.wHour, tt.wMinute, tt.wSecond );

		//char dir[MAX_PATH];
		//GetCurrentDirectory(MAX_PATH, dir);

		std::string Temp = "";
		Temp += "ScreenShot";

		_mkdir(Temp.c_str());

		std::string strFilename = Temp;
		strFilename += "\\";
		strFilename += szPath;

		Ogre::RenderSystem::getSingletonPtr()->snapShotAll( strFilename.c_str() );
		bPrintfMsg = true;
	}
	
	if( Ogre::RenderSystem::getSingletonPtr() != NULL && bPrintfMsg && !Ogre::RenderSystem::getSingletonPtr()->m_bSnapShot )
	{
		ActorManager::getSingleton().getScriptVM()->callString( "AddGameMiddleTips(\"截图已经保存在ScreenShot目录\")" );
		bPrintfMsg = false;
	}

	if( m_pFrameTick->CheckFPSUpdated() )
	{
		if ( /*!m_SystemSettingConfig.m_bFullScreen || is_debug*/ true )
		{
			const RenderSystem::RenderStats &stats = RenderSystem::getSingleton().m_RenderStats;
			char buffer[256];

			if ( GameManager::getSingleton( ).GetConfig( ).win_title )
			{
				const char* pszWindowName = NULL;
				if ( GetACP() == 936 )
				{
					pszWindowName = "零纪元";
				}
				else
				{
					pszWindowName = "Era Zero";
				}
				sprintf(buffer,"%s [%s]%s",pszWindowName,uint_to_version( getLocalVersion()),
					LoginManager::getSingleton().getServerName() );
			}else{
				RuntimeSysInfo info;
				GetRuntimeSysInfo(info);
				sprintf(buffer, 
					"version [%s] FPS:%.1f, numthread:%d, CPU[%d]:%.1f%%, MEM:%dK, NP:%d, NT:%d, ND:%d, TM:%dK, NC:%d, NS:%d, 玩家:%d, 怪物:%d, NPC:%d",
					uint_to_version( getLocalVersion() ),
					m_pFrameTick->GetFPS(), 
					info.num_thread, info.num_cpu, 
					info.kernel_cpu_usage+info.user_cpu_usage, 
					info.memory_usage/1000, 
					stats.nprimitive,
					stats.ntexture,
					stats.ndraw,
					stats.texmem/1000,
					stats.ncontext,
					stats.nshaderswitch,
					ActorManager::getSingleton().getPlayerCount(),
					ActorManager::getSingleton().getMonsterCount(),
					ActorManager::getSingleton().getNpcCount()
					);
			}
			::SetWindowText((HWND)m_hWnd, buffer);
		}
	}
	TraceTime::WirteToCSV();
	TraceTime::SetCSVData();
	TraceTime::EndTraceTime();
}

void GameManager::addRightBtnCallBack( BTN_FUN p )
{
	m_lRightBtnCallBack.push_back( p );
}

void GameManager::addLeftBtnCallBack( BTN_FUN p )
{
	m_lLeftBtnCallBack.push_back( p );
}

void GameManager::unRightBtnCallBack( BTN_FUN p )
{
	for ( std::list<BTN_FUN>::iterator iter = m_lRightBtnCallBack.begin(); iter != m_lRightBtnCallBack.end(); )
	{
		if ( *iter == p )
		{
			iter = m_lRightBtnCallBack.erase( iter );
			continue;
		}
		++iter;
	}
}

void GameManager::unLeftBtnCallBack( BTN_FUN p )
{
	for ( std::list<BTN_FUN>::iterator iter = m_lLeftBtnCallBack.begin(); iter != m_lLeftBtnCallBack.end(); )
	{
		if ( *iter == p )
		{
			iter = m_lLeftBtnCallBack.erase( iter );
			continue;
		}
		++iter;
	}
}

bool GameManager::LoadConfigFile(Ogre::XMLData *pxmldata)
{
	return LoadGameConfigFromXML(m_Config, pxmldata);
}

void GameManager::LoadServerListConfig(const char* fileName)
{
	m_Config.dir_num = 0;
	// 先检测是否是加密文件，若是则先解密，再读取
	// 先检测是否serverlist.cfg文件，若没有则从加密的文件中生成一份serverlist.cfg，否则就不用生成serverlist.cfg
	string strFileName = fileName;
	strFileName = strFileName.substr( 0, strFileName.find_last_of( "." ) );
	
// 	char szPath[128] = { '\0' };
// 	::GetCurrentDirectory( 128, szPath );
	string strEncryptFilePath		= fileName;
	const char* pszDecryptBuffer	= getDecryptFileBuffer( strEncryptFilePath.c_str(), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		MessageBox(NULL, "读取serverlist.cfg文件错误", "错误", MB_OK);
		return;
	}

	// 这边全部改为用buffer来读取
	Ogre::XMLData xmldata;
	m_Config.dir_url[0] = '\0';
	if ( xmldata.loadBuffer( pszDecryptBuffer, strlen( pszDecryptBuffer ) ) )
	{
		XMLNode root = xmldata.getRootNode();
		XMLNode pnode = root.iterateChild();
		m_Config.dir_num = 0;
		while( !pnode.isNull() && m_Config.dir_num < MAX_DIR_SERV_NUM  )
		{
			if( strcmp( pnode.getName(),"DirServer" ) == 0 )
			{
				NetConfigData& data = m_Config.dir_iplist[m_Config.dir_num];
				strncpy( data.dir_server,pnode.attribToString("address"),128 );
				data.dir_port = pnode.attribToInt("port");
				data.num = 0;
				XMLNode dirNode;
				dirNode = pnode.iterateChild(dirNode);
				while( !dirNode.isNull() && data.num < MAX_DIR_SERV_NUM )
				{
					if( strcmp(dirNode.getName(),"iplist")==0 )
					{
						strncpy(data.dir_data[data.num].dir_ip,dirNode.attribToString("ip"),128);
						data.dir_data[data.num].dir_port = dirNode.attribToInt("port");
						data.num++;
					}
					dirNode = pnode.iterateChild(dirNode);
				}
				m_Config.dir_num ++;
			}
			if( strcmp( pnode.getName(),"DirUrl" ) == 0 )
			{
				strncpy( m_Config.dir_url,pnode.attribToString("address"),255);

			}
			pnode = root.iterateChild(pnode);
		}
		//ftp config
		pnode = root.getChild("DumpFtp");
		if( !pnode.isNull() )
		{
			MailConfig::getSingleton().bHasConfig = true;
			MailConfig::getSingleton().ftp_port = pnode.attribToInt("port");
			strncpy( MailConfig::getSingleton().ftp_address,pnode.attribToString("server"),255 );
			strncpy( MailConfig::getSingleton().ftp_user,pnode.attribToString("user"),255 );
			strncpy( MailConfig::getSingleton().ftp_pwd,pnode.attribToString("pwd"),255 );
		}else{
			MailConfig::getSingleton().bHasConfig = false;
		}

		//
		if (Root::getSingleton().IsSmartClient())
		{
			pnode = root.getChild("SmartClient");
			if (!pnode.isNull())
			{
				std::string ser = pnode.attribToString("server");
				Root::getSingleton().SetSmartClientServer(ser);
			}
		}
	}
	delete[] pszDecryptBuffer;
}

bool GameManager::InitEngine()
{
	if ( m_SystemSettingConfig.m_bFullScreen/* && m_Config.win_width==0*/ )
	{
		m_SystemSettingConfig.m_nWinWidth	= GetSystemMetrics( SM_CXSCREEN );
		m_SystemSettingConfig.m_nWinHeight	= GetSystemMetrics( SM_CYSCREEN );
	}

	unsigned long style;
	int nDestScreenWidth	= m_SystemSettingConfig.m_nWinWidth;
	int nDestScreenHeight	= m_SystemSettingConfig.m_nWinHeight;
	if ( m_SystemSettingConfig.m_bFullScreen ) 
	{
		style = WS_POPUP;
	}
	else 
	{
		#ifdef _WEB
			style = WS_CHILD | WS_VISIBLE | WS_THICKFRAME;
		#else
			style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
		#endif
	}

	RECT rect = { 0, 0, nDestScreenWidth, nDestScreenHeight };
	if ( !m_SystemSettingConfig.m_bFullScreen )
	{
		AdjustWindowRect( &rect, style, FALSE );
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		float screenWidth = (float)::GetSystemMetrics(SM_CXSCREEN);
		float screenHeight = (float)::GetSystemMetrics(SM_CYSCREEN);

		if(w > screenWidth) 
		{
			w = screenWidth;
		}

		if ( h > screenHeight ) 
		{
			h = screenHeight;
		}

		rect.left	= (screenWidth-w)/2;
		rect.top	= (screenHeight-h)/2;
		rect.right	= rect.left+w;
		rect.bottom = rect.top+h;
		checkWindowSize( rect );
		checkWindowPos( rect );
	}

#ifdef _WEB
	HWND hWnd = CreateWindow( TEXT( "GameManagerWin" ), TEXT( "GameManagerWin" ), style, 
		0, 0, 1024, 768, (HWND)m_hWebWnd, NULL, 
		::GetModuleHandle(NULL), NULL );
#else
	HWND hWnd = CreateWindow( TEXT( "GameManagerWin" ), TEXT( "GameManagerWin" ), style, 
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, 
		( HINSTANCE )m_hInstance, NULL );
#endif
	
	m_hWnd = hWnd;
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow( hWnd );
	GetClientRect((HWND)hWnd, &rect);
	return Root::getSingleton().initRenderSystem( hWnd );
}

void GameManager::onWindowSize( int width, int height )
{
	if(width == 0) 
	{
		width = m_ViewportWidth;
	}
	if ( height == 0 ) 
	{
		height = m_ViewportHeight;
	}

	m_ViewportWidth = width;
	m_ViewportHeight = height;
	if ( m_pGamePlayMgr != NULL )
	{
		Ogre::Camera *pcamera = m_pGamePlayMgr->getMainCamera()->GetCameraNode();
		Ogre::Camera *pcutcamera = m_pGamePlayMgr->getMainCamera()->GetCutActorCameraNode();
		if (pcamera == pcutcamera)
		{
			pcutcamera = NULL;
		}
		pcamera->setViewport(0, 0, (float)m_ViewportWidth, (float)m_ViewportHeight);
		if (pcutcamera != NULL)
		{
			pcutcamera->setViewport(0, 0, (float)m_ViewportWidth, (float)m_ViewportHeight);
		}
		if ( m_ViewportHeight == 0 ) 
		{
			pcamera->setRatio( 1.0f );
		}
		else 
		{
			pcamera->setRatio( float( m_ViewportWidth ) / m_ViewportHeight );
		}
		if (pcutcamera != NULL)
		{
			pcutcamera->setRatio(pcamera->getRatio());
		}

		RenderWindow *prwin = RenderSystem::getSingleton().findRenderWindow( m_hWnd );
		if( prwin != NULL )
		{
			prwin->onSizeOrMove();
		}
	}
}

void GameManager::setScreenMode(bool fullscreen, int w, int h)
{
	unsigned long style = 0;
	RECT rect = {0, 0, 0, 0};
	if(fullscreen)
	{
		style = WS_POPUP;
		rect.right = w;
		rect.bottom = h;
	}
	else
	{
		style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_VISIBLE;
		// TODO: 修改点

		rect.right = w;
		rect.bottom = h;
		AdjustWindowRect(&rect, style, FALSE);
		LONG winw = rect.right-rect.left;
		LONG winh = rect.bottom-rect.top;

		if(winw > s_DeskWorkW) winw = s_DeskWorkW;
		if(winh > s_DeskWorkH) winh = s_DeskWorkH;

		RECT rectBefore;
		::GetWindowRect((HWND)m_hWnd, &rectBefore);
		/*rect.left = rectBefore.left;
		rect.top = rectBefore.top;*/

		rect.left	= (s_DeskWorkW-winw)/2;
		rect.top	= (s_DeskWorkH-winh)/2;
		rect.right	= rect.left+winw;
		rect.bottom = rect.top+winh;
		checkWindowSize( rect );
		checkWindowPos( rect );
	}

	UINT flags = SWP_NOREDRAW|SWP_SHOWWINDOW;
	if ( m_SystemSettingConfig.m_bFullScreen != fullscreen || w != m_SystemSettingConfig.m_nWinWidth ||
		h != m_SystemSettingConfig.m_nWinHeight )
	{
		SetWindowLong((HWND)m_hWnd, GWL_STYLE, style);
		flags |= SWP_FRAMECHANGED;
	}
	SetWindowPos((HWND)m_hWnd, HWND_TOP, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, flags);

	if ( m_SystemSettingConfig.m_bFullScreen != fullscreen || w != m_SystemSettingConfig.m_nWinWidth ||
		h != m_SystemSettingConfig.m_nWinHeight )
	{
		if(fullscreen) SceneManager::getSingleton().setDisplayMode(w, h);
		else SceneManager::getSingleton().setDisplayMode(0, 0);

		m_SystemSettingConfig.m_bFullScreen = fullscreen;
	}
}

void GameManager::playCameraAnim( const char* szOmodPath )
{
	if( m_pGamePlayMgr != NULL )
	{
		m_pGamePlayMgr->getMainCamera()->playCameraAnim( szOmodPath );
	}
}

// 是否是在播放镜头动画
bool GameManager::isInPlayCamerAnim()
{
	if( m_pGamePlayMgr != NULL )
	{
		return m_pGamePlayMgr->getMainCamera()->isPlayCameraAnim();
	}
	return false;
}

// 中断相机动画
void GameManager::stopPlayCameraAnim()
{
	if( m_pGamePlayMgr != NULL )
	{
		return m_pGamePlayMgr->getMainCamera()->stopCameraAnim();
	}
}

SystemSettingConfig& GameManager::getUserSystemConfig()
{
	return m_SystemSettingConfig;
}

SystemSettingConfig& GameManager::getLatestUserSystemConfig()
{
	return m_LatestSystemSettingConfig;
}

void GameManager::ApplyDefaultUserConfig()
{
	ApplyNewUserConfig( m_sDefaultSysSetting );
	m_SystemSettingConfig = m_sDefaultSysSetting;
}

void GameManager::ApplyNewUserConfig( SystemSettingConfig& latestSystemSetting )
{
	if ( latestSystemSetting.m_bSyncRefresh != m_SystemSettingConfig.m_bSyncRefresh )
	{
		latestSystemSetting.m_curChgEffect.m_bChgSyncRefresh = true;
	}

	if ( latestSystemSetting.m_bBloom != m_SystemSettingConfig.m_bBloom )
	{
		// BloomEffect
		latestSystemSetting.m_curChgEffect.m_bChgBloom = true;
	}

	if ( latestSystemSetting.m_bDistort != m_SystemSettingConfig.m_bDistort )
	{
		// BloomEffect	DistortEffect
		latestSystemSetting.m_curChgEffect.m_bChgBloom = true;
		latestSystemSetting.m_curChgEffect.m_bChgDistortEffect = true;
	}

	if ( latestSystemSetting.m_bWaterReflect != m_SystemSettingConfig.m_bWaterReflect )
	{
		// TerrainBlock 需重启才能生效
		latestSystemSetting.m_curChgEffect.m_bChgWaterReflect = true;
	}

	if ( latestSystemSetting.m_nShadowmapSize != m_SystemSettingConfig.m_nShadowmapSize )
	{
		// Shadowmap
		latestSystemSetting.m_curChgEffect.m_bChgShadow = true;
	}

	if ( latestSystemSetting.m_nMultiSampleLevel != m_SystemSettingConfig.m_nMultiSampleLevel )
	{
		// BloomEffect	DistortEffect
		latestSystemSetting.m_curChgEffect.m_bChgMultiSampleLevel = true;
	}
	
	if ( latestSystemSetting.m_bMute )
	{
		Root::getSingleton().setGlobalMusicVolume( 0.0f );
		Root::getSingleton().setGlobalSoundVolume( 0.0f );
	}
	else
	{
		Root::getSingleton().setGlobalMusicVolume( latestSystemSetting.m_fGlobalMusicVolume );
		Root::getSingleton().setGlobalSoundVolume( latestSystemSetting.m_fGlobalSoundVolume );
	}

	if ( latestSystemSetting.m_bFullScreen != m_SystemSettingConfig.m_bFullScreen || 
		latestSystemSetting.m_nWinWidth != m_SystemSettingConfig.m_nWinWidth ||
		latestSystemSetting.m_nWinHeight != m_SystemSettingConfig.m_nWinHeight )
	{
		setScreenMode( latestSystemSetting.m_bFullScreen, latestSystemSetting.m_nWinWidth, latestSystemSetting.m_nWinHeight );
	}
	else // 在没变化分辨率的时候，检测是否要改变无需设备丢失也能生效的效果
	{
		latestSystemSetting.applyChgEffect();
	}

	m_SystemSettingConfig = latestSystemSetting;
	m_SystemSettingConfig.SaveSystemSettingFile();

	m_pFrameTick->setFrameLimit( m_SystemSettingConfig.m_nFPS );

	float fLastAllSelfScale = g_pFrameMgr->GetAllSelfScale();
	if ( fLastAllSelfScale != m_SystemSettingConfig.m_fUIScale )
	{
		g_pFrameMgr->UpdateAllSelfScale( m_SystemSettingConfig.m_fUIScale );
	}

}

void GameManager::setGlobalSoundVolume(float fVolume)
{
	Root::getSingleton().setGlobalSoundVolume( fVolume );
}

void GameManager::setGlobalMusicVolume(float volume)
{
	Root::getSingleton().setGlobalMusicVolume( volume );
}

int GameManager::OnWindowMsg( void *hwnd, unsigned int msgid, unsigned int wparam, unsigned int lparam )
{
	if ( m_pInputMgr ) 
	{
		if (!isVideoPlaying() || (
			msgid != WM_KEYDOWN &&
			msgid != WM_LBUTTONDOWN &&
			msgid != WM_LBUTTONUP &&
			msgid != WM_LBUTTONDBLCLK &&
			msgid != WM_RBUTTONDOWN &&
			msgid != WM_RBUTTONUP &&
			msgid != WM_RBUTTONDBLCLK))
		{
			m_pInputMgr->OnWindowMessage( hwnd, msgid, wparam, lparam );
		}
	}

	switch( msgid )		
	{
	case WM_KEYDOWN:
		if (isVideoPlaying())
		{
			skipVideo();
		}
	case WM_SETCURSOR:
		//::SetCursor( NULL );
		if ( RenderSystem::getSingletonPtr() )
		{
			RenderSystem::getSingletonPtr()->showCursor( true );
		}

		return TRUE;

	case WM_MOUSEMOVE:
		if ( RenderSystem::getSingletonPtr() )
		{
			//::POINT pt;
			//GetCursorPos(&pt);
			//RenderSystem::getSingletonPtr()->setCursorPosition(pt.x, pt.y);
		}
		break;

	case WM_SETFOCUS:
		m_bFocus = true;
		if (!m_lastVideoPlayed)
		{
			SoundSystem::getSingleton().setPause(false);
		}
		if ( m_pFrameTick ) 
		{
			m_pFrameTick->setFrameLimit( m_SystemSettingConfig.m_nFPS );
		}
		StopFlashWindow();
		break;

	case WM_KILLFOCUS:
		//判断当前焦点是否在子控件上面
		if( GetParent(GetFocus()) != m_hWnd )
		{
			m_bFocus = false;
			if (SoundSystem::getSingletonPtr())
				SoundSystem::getSingleton().setPause(true);

			if (m_bIsCreated)
			{
				if ( m_pFrameTick != NULL ) 
				{
					m_pFrameTick->setFrameLimit( m_SystemSettingConfig.m_nFPS / 3 );
				}
			}
		}
		
		break;

	case WM_DISPLAYCHANGE:
		//onWindowSize( 0, 0 );
		return 0;

	case WM_SIZE:
		{
			onWindowSize( LOWORD( lparam ), HIWORD( lparam ) );
			MCIWrap* pWrap = MCIWrap::getSingletonPtr();
			if (pWrap)
			{
				pWrap->updateWindow();
			}
			return 0;
		}

	case WM_LBUTTONDOWN:
		{
			::SetFocus((HWND)hwnd);
			if( msgid == WM_LBUTTONDOWN )
			{
				for( std::list<BTN_FUN>::iterator iter = m_lLeftBtnCallBack.begin(); 
					iter != m_lLeftBtnCallBack.end(); ++iter)
				{
					(*iter)(true);
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			if( msgid == WM_LBUTTONDOWN )
			{
				for( std::list<BTN_FUN>::iterator iter = m_lLeftBtnCallBack.begin(); 
					iter != m_lLeftBtnCallBack.end(); ++iter)
				{
					(*iter)(false);
				}
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			::SetFocus((HWND)hwnd);
			for( std::list<BTN_FUN>::iterator iter = m_lRightBtnCallBack.begin(); 
				iter != m_lRightBtnCallBack.end(); ++iter)
			{
				(*iter)(true);
			}
		}
		break;
	case WM_RBUTTONUP:
		{
			for( std::list<BTN_FUN>::iterator iter = m_lRightBtnCallBack.begin(); 
				iter != m_lRightBtnCallBack.end(); ++iter)
			{
				(*iter)(false);
			}
		}
		break;
	case WM_SYSCOMMAND:
		switch(wparam)
		{
		case SC_KEYMENU:
			return 0;
		}
		break;

	case WM_CLOSE://!!!!
		{
			GameManager::getSingleton().getScriptVM()->callString( "showQuitGameFrame()" );
			//PostQuitMessage(0);
			return 0;
		}
		break;

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return (int)DefWindowProc((HWND)hwnd, msgid, wparam, lparam);
}

void GameManager::InitDB()
{
	m_pDBMgr = new DBDataManager;
	if( !m_pDBMgr->LoadData( m_Config ) )
	{
		//return false;
	}
}

void GameManager::ScriptLog(const char *pinfo)
{
	LOG_INFO(pinfo);
}

void GameManager::SendUIEvent(const char *pevent)
{
	m_pGameUI->SendEvent(pevent);
}

void GameManager::QuitGame()
{
	HWND hWnd = NULL;
	hWnd = (HWND)RenderSystem::getSingleton().getMainWindow()->getHWnd();
	ShowWindow(hWnd, SW_HIDE);
	//在退出的时候保存用户配置
	GameUserConfig::getSingleton().writeData();
	GameUserConfig::getSingleton().release();
	GamePlayManager::getSingleton().leaveMap();
	GameShortCut::getSingleton().clearSpringSpell();
	GameStateManager::changeGameState( GameStateManager::STATE_INIT );
	ActorManager::getSingleton().DestroyAllActor(true);
	PostQuitMessage(0);	
	//exit(0);
}

void GameManager::returnLogin()
{	
	GamePlayManager::getSingleton().getMainCamera()->Reset(WorldPos(0,0,0));
	GamePlayManager::getSingleton().leaveMap();
	GameShortCut::getSingleton().clearSpringSpell();
	//在退出的时候保存用户配置
	GameUserConfig::getSingleton().writeData();
	m_pActorMgr->DestroyAllActor( true );
	while( ::ShowCursor(TRUE)<0 );
	GameStateManager::changeGameState( GameStateManager::STATE_ROLELIST );
	GameUserConfig::getSingleton().release();
}

// void GameManager::BeginPureClient(int mapid, int player_resid)
// {
// 	SendUIEvent("GE_ENTERWORLD");
// 
// 	GamePlayManager::getSingleton().enterWorld(mapid, 256000, 256000, 100, player_resid);
// 	GameManager::getSingleton().SendUIEvent("GE_LOADMAP_END");
// }

unsigned int GameManager::getTickTime()
{
	return timeGetTime();
}

void GameManager::cancelNextSelect()
{
	return m_pGamePlayMgr->getSelectManager()->cancelNextSelect();
}

void GameManager::HighlightNewItem( int nMemID )
{
	GameSelectManager* pSelectMgr = m_pGamePlayMgr->getSelectManager();
	pSelectMgr->HighlightNewItem( nMemID );
}

int GameManager::getLastHighlightItemID()
{
	GameSelectManager* pSelectMgr = m_pGamePlayMgr->getSelectManager();
	return pSelectMgr->getLastHighlightItemID();
}

int GameManager::getLastHighStallActorID()
{
	GameSelectManager* pSelectMgr = m_pGamePlayMgr->getSelectManager();
	return pSelectMgr->getLastHighStallActorID();
}

void GameManager::HighlightStallActor( int nActorID )
{
	GameSelectManager* pSelectMgr = m_pGamePlayMgr->getSelectManager();
	pSelectMgr->HighlightStallActor( nActorID );
}

void GameManager::loadAdditionalScript()
{
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp,"GMPackage.lua","rb");
	if( !err && fp != NULL )
	{
		fclose(fp);
		m_pScriptVM->callFile("GMPackage.lua");
	}
	FILE* fp1 = NULL;
	errno_t err1 = fopen_s(&fp1,"GMAutoQuest.lua","rb");
	if( !err1 && fp1 != NULL )
	{
		fclose(fp1);
		m_pScriptVM->callFile("GMAutoQuest.lua");
	}
	FILE* fp2 = NULL;
	errno_t err2 = fopen_s(&fp2,"GMAutoFunList.lua","rb");
	if( !err2 && fp2 != NULL )
	{
		fclose(fp2);
		m_pScriptVM->callFile("GMAutoFunList.lua");
	}
}

void GameManager::changeShowUI()
{
	g_pFrameMgr->m_bRenderUI = !g_pFrameMgr->m_bRenderUI ;
}

void GameManager::openWebSite( const char* szUrl )
{
	if( szUrl !=NULL )
	{
		ShellExecute(NULL,"open",szUrl,NULL,NULL,SW_SHOW);
	}
}

void GameManager::sendEMail( const char* szTitle,const char* szContent,int mailType )
{
	if( szContent )
	{
		char szTime[512];
		FILETIME CurrentTime;
		GetSystemTimeAsFileTime(&CurrentTime);
		szTime[0] = '\0';
		WORD wDate, wTime;
		if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
			FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
		{
			// 年月日以及时分秒
			wsprintf(szTime, "%s[%d-%d-%d %02d%02d%02d].txt",szTitle,
				(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
				(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);

		}
		sendDataToFtp( szContent,"BugList",szTime,strlen(szContent) );
	}
}

void GameManager::playVideo(const char *filename)
{
	m_lastVideoPlayed = true;
	//SoundSystem::getSingleton().setPause( isVideoPlaying() );
	setVideoTag();
	MCIWrap::getSingleton().play(filename);
	SoundSystem::getSingleton().playSound2D( filename, 1 );
}

void GameManager::skipVideo()
{
	MCIWrap::getSingleton().stop();
}

void GameManager::stopVideo()
{
	MCIWrap::getSingleton().reset();
}

bool GameManager::isVideoPlaying()
{
	return MCIWrap::getSingleton().isPlaying();
}

bool GameManager::isFirstRun()
{
	return m_isFirstRun;
}

void GameManager::setFirstRun(bool b)
{
	m_isFirstRun = b;
}

bool GameManager::isMapLoaded()
{
	return GamePlayManager::getSingleton().isMapLoadOK();
}

void GameManager::mapLoadFaded()
{
	GamePlayManager::getSingleton().finishFade();
}

void GameManager::reloadSystemConfig()
{
	RenderWindow *prwin = RenderSystem::getSingleton().findRenderWindow( m_hWnd );
	if( prwin != NULL )
	{
		prwin->forceReset();
	}
	Root::getSingleton().m_Config.saveFile( config_filename );
	m_SystemSettingConfig.LoadSystemSettingFile( &Root::getSingleton().m_Config );
}

void GameManager::setVideoTag()
{
	HANDLE handle = CreateFile("skipLogo", FILE_APPEND_DATA, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, NULL);
	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
	}
}

void GameManager::clearVideoTag()
{
	DeleteFile("skipLogo");
}

bool GameManager::IsLastVideoError()
{
	HANDLE handle = CreateFile("skipLogo", FILE_READ_DATA, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
		return true;
	}
	return false;
}

void GameManager::ShowLoading(bool show)
{
	GamePlayManager::getSingleton().ShowLoading(show);
}

void GameManager::setFashionChangeAngle(float pullDist)
{
	m_pGamePlayMgr->setFashionChangeAngle(pullDist);
}

void GameManager::resetPlayAngle()
{
	m_pGamePlayMgr->resetPlayAngle();
}

static bool bSendFile = false;
void GameManager::sendAllLogToFtp()
{
	if(bSendFile )
	{
		return;
	}
	if( !MailConfig::getSingleton().bHasConfig )
	{
		return;
	}

	char szTime[512];
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = '\0';
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// 年月日以及时分秒
		wsprintf(szTime, "All[%d-%d-%d %02d%02d%02d].log",
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);

	}
	sendFileToFtp( "log/all.log","AllLog",szTime );
	bSendFile = !bSendFile;
}

void GameManager::PrintRunningInfo()
{
	RuntimeSysInfo info;
	GetRuntimeSysInfo(info);
	const RenderSystem::RenderStats &stats = RenderSystem::getSingleton().m_RenderStats;
	char buffer[256];
	sprintf(buffer, 
		"FPS:%.1f, numthread:%d, CPU[%d]:%.1f%%, MEM:%dK, NP:%d, NT:%d, ND:%d, TM:%dK, NC:%d, NS:%d",
		m_pFrameTick->GetFPS(), 
		info.num_thread, info.num_cpu, 
		info.kernel_cpu_usage+info.user_cpu_usage, 
		info.memory_usage/1000, 
		stats.nprimitive,
		stats.ntexture,
		stats.ndraw,
		stats.texmem/1000,
		stats.ncontext,
		stats.nshaderswitch );

	LOG_INFO(buffer);

	char fileName[MAX_PATH];
	sprintf(fileName,"%ddump.info",timeGetTime());
	sendFileToFtp( "log/dump.info", "DUMPTXT",fileName );
	DeleteFile("log/dump.info");
}

DWORD WINAPI ScreenSnapMainFunc(void *pdata)
{
	RECT rcScreen;
	std::vector<RECT> wnd_list;
	std::vector<std::string> fileName;
	CScreenCapture sc;
	sc.GetScreenRect(&rcScreen);    
	wnd_list.push_back(rcScreen);
	sc.CaptureScreenRect(wnd_list,fileName );
	if( !fileName.empty() )
	{
		for( size_t i = 0 ; i < fileName.size() ; i++ )
		{
			std::string localPath = "log/" + fileName[i] + ".jpg";
			std::string remotePath = fileName[i] + ".jpg";
			GameManager::getSingleton().sendFileToFtp( localPath.c_str(), "ScreenCap",remotePath.c_str() );
			DeleteFile(localPath.c_str());
		}
	}
	return 0;
}

void GameManager::ScreenSnap()
{
	DWORD threadid;
	HANDLE h = CreateThread(NULL, 0, ScreenSnapMainFunc, this, 0, &threadid);
	CloseHandle( h );
}

void GameManager::MakeAttach()
{
	ResourceManager::getSingleton().MakeAttach();
}

void GameManager::MakeMD5()
{
	ResourceManager::getSingleton().MakeMD5();
}

void GameManager::FlashWindow()
{
	//没焦点的时候闪动
	if( !m_bFocus )
	{
		FLASHWINFO info;
		info.cbSize		= sizeof( FLASHWINFO );
		info.hwnd		= (HWND)m_hWnd;
		info.dwFlags	= FLASHW_TRAY;
		info.uCount		= 20;
		info.dwTimeout	= 0;	
		FlashWindowEx( &info );
	}
}

void GameManager::StopFlashWindow()
{
	FLASHWINFO info;
	memset( &info, 0, sizeof(FLASHWINFO) );
	info.cbSize		= sizeof( FLASHWINFO );
	info.hwnd		= (HWND)m_hWnd;
	info.dwFlags	= FLASHW_STOP;
	FlashWindowEx( &info );
}


bool CreateFtpMultipleDirectory( HINTERNET it,std::string strPath )
{
	if( it == NULL )
	{
		return FALSE;
	}

	std::string strDir(strPath);//存放要创建的目录字符串
	//确保以'/'结尾以创建最后一个目录
	if ( strDir.at(strDir.length()-1)!='/' )
	{
		strDir += '/';
	}
	std::vector<std::string> vPath;//存放每一层目录字符串
	std::string strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.length();++i)
	{
		if (strDir.at(i) != '/') 
		{//如果当前字符不是'/'
			strTemp += strDir.at(i);
		}
		else 
		{//如果当前字符是'/'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp += '/';
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<std::string>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = FtpCreateDirectory(it,vIter->c_str()) ? true : false;
	}
	return bSuccess;
}

void GameManager::sendFileToFtp( const char* srcFile, const char* destFilePath, const char* destFileName )
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(srcFile, 
		GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}

	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it == NULL )
	{
		CloseHandle(hFile);
		return;
	}
	HINTERNET it2 = InternetConnect(it,
		MailConfig::getSingleton().ftp_address,
		MailConfig::getSingleton().ftp_port,
		MailConfig::getSingleton().ftp_user,
		MailConfig::getSingleton().ftp_pwd,
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_PASSIVE,
		NULL);
	if( it2 == NULL )
	{
		InternetCloseHandle(it);
		CloseHandle(hFile);
		return;
	}
	FtpSetCurrentDirectory(it2,"/");
	std::stringstream ss;
	ss<<"/"<< uint_to_version( getLocalVersion() ) <<"/"<<destFilePath<<"/"<< LoginManager::getSingleton().getUserName();
	CreateFtpMultipleDirectory( it2,ss.str() );
	if( !FtpSetCurrentDirectory(it2,ss.str().c_str()) )
	{
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		CloseHandle(hFile);
		return;
	}

	HINTERNET ftpFile = FtpOpenFile(it2,destFileName,GENERIC_WRITE,FTP_TRANSFER_TYPE_BINARY,NULL);
	DWORD dwBytesWrite = 0;
 	DWORD dwBytesRead=0;
 	char buff[1024];
	for(;;)
	{
		BOOL bRead = ReadFile(hFile, buff, 1024, &dwBytesRead, NULL);
		if(!bRead || dwBytesRead==0)
			break;
		BOOL bWrite = InternetWriteFile( ftpFile,buff,dwBytesRead,&dwBytesWrite );
		if( !bWrite || dwBytesWrite != dwBytesRead )
		{
			break;
		}
	}
	CloseHandle(hFile);
	InternetCloseHandle(ftpFile);
	InternetCloseHandle(it);
	InternetCloseHandle(it2);
}

void GameManager::sendDataToFtp( const char* data, const char* destFilePath, const char* destFileName , unsigned int length )
{
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it == NULL )
	{
		return;
	}
	HINTERNET it2 = InternetConnect(it,
		MailConfig::getSingleton().ftp_address,
		MailConfig::getSingleton().ftp_port,
		MailConfig::getSingleton().ftp_user,
		MailConfig::getSingleton().ftp_pwd,
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_PASSIVE,
		NULL);
	if( it2 == NULL )
	{
		InternetCloseHandle(it);
		return;
	}
	FtpSetCurrentDirectory(it2,"/");
	std::stringstream ss;
	ss<<"/"<< uint_to_version( getLocalVersion() ) <<"/"<<destFilePath<<"/"<< LoginManager::getSingleton().getUserName();
	CreateFtpMultipleDirectory( it2,ss.str() );
	if( !FtpSetCurrentDirectory(it2,ss.str().c_str()) )
	{
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return;
	}
	HINTERNET ftpFile = FtpOpenFile(it2,destFileName,GENERIC_WRITE,FTP_TRANSFER_TYPE_BINARY,NULL);
	DWORD dwBytesWrite = 0;		
	BOOL bWrite = InternetWriteFile( ftpFile,data,length,&dwBytesWrite );
	InternetCloseHandle(ftpFile);
	InternetCloseHandle(it);
	InternetCloseHandle(it2);
}

void GameManager::SendHttpRequest( const char* szUrl )
{
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it != NULL )
	{
		HINTERNET it2 = InternetOpenUrl( it,szUrl,NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
		InternetCloseHandle(it2);
	}
	InternetCloseHandle(it);
}

void GameManager::registerSelfHandler( bool bReg )
{
	if( bReg )
	{
		m_pInputMgr->setSelfHandler(m_pGamePlayMgr);
	}else{
		m_pInputMgr->setSelfHandler(NULL);
	}
}

bool GameManager::checkInstanceNum()
{
	static const char *testName = "{8714D67D-7EAC-4179-B749-0C9FCD6721B6}";
	HANDLE testMutex[2];
	for (size_t i = 0; i < sizeof(testMutex) / sizeof(testMutex[0]); ++i)
	{
		testMutex[i] = CreateMutex(NULL, FALSE, testName);
	}
	if (GetLastError() == ERROR_SUCCESS)
	{
		return false;
	}
	for (size_t i = 0; i < sizeof(testMutex) / sizeof(testMutex[0]); ++i)
	{
		if (testMutex[i] != NULL)
		{
			CloseHandle(testMutex[i]);
		}
	}
	static const char *mutexNames[] = {
		"{3320DC73-6359-4187-8929-158033CBEDC6}",
		"{21A57878-8C39-4c26-921C-EE1F0BCB79A1}",
		"{D542D1AA-F661-4d3a-A64E-2F42ABE2901F}",
	};
	for (size_t i = 0; i < sizeof(mutexNames) / sizeof(mutexNames[0]); ++i)
	{
		m_hInstMutex = CreateMutex(NULL, FALSE, mutexNames[i]);
		if (GetLastError() == ERROR_SUCCESS)
		{
			return true;
		}
		if (m_hInstMutex != NULL)
		{
			CloseHandle(m_hInstMutex);
			m_hInstMutex = NULL;
		}
	}
	return false;
}

void GameManager::getMacString( char* szMacAdress )
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
		PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
		int aaa = 0;
		while (pAdapter)
		{
			__int64 val = (__int64)pAdapter->Address;
			if( pAdapter->AddressLength > 0 && (__int64)pAdapter->Address != 0 )
			{
				sprintf(szMacAdress,"%02x-%02x-%02x-%02x-%02x-%02x",pAdapter->Address[0],pAdapter->Address[1],pAdapter->Address[2],
					pAdapter->Address[3],pAdapter->Address[4],pAdapter->Address[5]);
				break;
			}
			pAdapter = pAdapter->Next;
		}
	}
	free(pAdapterInfo);
}

void GameManager::getCpuId( char* szCPUId )
{
	BOOL bException = FALSE;
	BYTE szCpu[16]	= { 0 };
	UINT uCpuID		= 0;
	__try
	{
		_asm
		{
			mov eax, 0
				cpuid
				mov dword ptr szCpu[0], ebx
				mov dword ptr szCpu[4], edx
				mov dword ptr szCpu[8], ecx
				mov eax, 1
				cpuid
				mov uCpuID, edx
		}
	}

	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		bException = TRUE;
	}

	if( !bException )
	{
		sprintf( szCPUId, "%u", uCpuID );
	}else{
		sprintf( szCPUId, "%s", "cpuid is none" );
	}
}

void GameManager::getHdSerialNum( char* serialNum )
{
	DISPATCH_OBJ(wmiSvc);
	DISPATCH_OBJ(colMedia);

	dhInitialize(TRUE);
	dhToggleExceptions(TRUE);

	dhGetObject(L"winmgmts:{impersonationLevel=impersonate}!\\\\.\\root\\cimv2", NULL, &wmiSvc);
	dhGetValue(L"%o", &colMedia, wmiSvc, L".ExecQuery(%S)",  L"SELECT * FROM Win32_PhysicalMedia");

	FOR_EACH(wmiMediaItem, colMedia, NULL)
	{
		char *pszSerial = NULL;
		dhGetValue(L"%s", &pszSerial, wmiMediaItem, L".SerialNumber");
		sprintf( serialNum, "%s",pszSerial );
		dhFreeString(pszSerial);
		break;

	} NEXT(wmiMediaItem);

	SAFE_RELEASE(colMedia);
	SAFE_RELEASE(wmiSvc);

	dhUninitialize(TRUE);
}

void GameManager::InitMachineID()
{
	getMacString(m_szMachineID);
	if(  std::string(m_szMachineID) == "00-00-00-00-00-00" || std::string(m_szMachineID) == "" )
	{
		m_szMachineID[0] = '\0';
		getHdSerialNum(m_szMachineID);
		if( strlen(m_szMachineID) == 0 )
		{
			sprintf( m_szMachineID, "%s", "cpuid is none" );
		}
	}
}

const char* GameManager::getMachineID()
{
	return m_szMachineID;
}

void GameManager::setTestMode( bool b )
{
	m_bTestMode = b;
}
bool GameManager::IsTestMode()
{
	return m_bTestMode;
}

void GameManager::SetMaxLevel( int maxLevel )
{
	m_nMaxLevel = maxLevel;
}
int GameManager::GetMaxLevel()
{
	return m_nMaxLevel;
}

void GameManager::SetLevelFeng( int nLv) 
{ 
	m_nLevelFeng = nLv ;
}
int  GameManager::GetLevelFeng() 
{
	return m_nLevelFeng;
}
const char* GameManager::GetUtf8Uri( const char* src )
{
	wchar_t wtmp[512];
	char utmp[512];

	static char ret[512];
	int end = StringUtil::AnsiToUnicode(wtmp, 512, src);
	end = StringUtil::UnicodeToUTF8(utmp, 512, wtmp);
	utmp[end] = 0;

	unsigned char *pWork = (unsigned char *)utmp;
	ret[0] = '\0';
	while( *pWork != 0x0 )
	{
		if ( *pWork != '!' && *pWork != '@' && *pWork != '#' &&
			*pWork != '$' && *pWork != '&' && *pWork != '*' &&
			*pWork != '(' && *pWork != ')' && *pWork != '=' &&
			*pWork != ':' && *pWork != '/' && *pWork != ';' &&
			*pWork != '?' && *pWork != '+' && *pWork != '\'' &&
			*pWork != '.' )
		{
			sprintf(ret+strlen(ret),"%%%2X",*pWork);
		}
		else
		{
			sprintf(ret+strlen(ret),"%c",*pWork);
		}
		pWork++;
	}
	return ret;
}
const char* GameManager::GetVersionName()
{
	return uint_to_version( getLocalVersion() );
}

bool tryDownloadAdFile( const char* fileName )
{
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it == NULL )
	{
		return false;
	}
	string path = "http://www.ljy0.com/data/game/";
	path += fileName;
	HINTERNET it2 = InternetOpenUrl( it,path.c_str(),NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
	if( it2 == NULL )
	{
		InternetCloseHandle(it);
		return false;
	}

	char retBuf[10] = { 0 };
	DWORD bufLen = sizeof(retBuf);
	HttpQueryInfo(it2, HTTP_QUERY_STATUS_CODE, retBuf, &bufLen , 0);
	DWORD dwRtn = (DWORD)atol(retBuf);
	if ( dwRtn != 200 )
	{
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}

	DWORD nTotalSize = 0;
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
	BOOL   bHwrite;
	DWORD  dwWritten;
	HANDLE hCreatefile;
	path = "resource\\ad\\";
	path += fileName;
	hCreatefile = CreateFile(path.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hCreatefile)
	{
		delete buf;
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}

	BOOL it3;
	while(true)
	{
		it3 = InternetReadFile(it2,buf,nTotalSize,&nLength);
		if (!nLength)
		{
			break;
		}
		bHwrite = WriteFile(hCreatefile, buf, nTotalSize, &dwWritten, NULL);
		if(!bHwrite)
		{
			InternetCloseHandle(it);
			InternetCloseHandle(it2);
			CloseHandle(hCreatefile);
			return false;
		}
	}
	InternetCloseHandle(it);
	InternetCloseHandle(it2);
	CloseHandle(hCreatefile);
	return true;
}

DWORD WINAPI beginDownAdfile( LPVOID argv )
{
	_mkdir("resource\\ad");
	int index = 1;
	int nMaxNum = 5;
	while (index <= nMaxNum) 
	{
		char temp[16];
		sprintf(temp, "%d", index);
		string nNum(temp);
		string path = nNum;
		path += ".jpg";
		if ( !tryDownloadAdFile(path.c_str()) )
		{
			path = "resource/ad/" + path;
			DeleteFile(path.c_str());
		}

		path = nNum;
		path += ".ini";
		if ( !tryDownloadAdFile(path.c_str()) )
		{
			path = "resource/ad/" + path;
			DeleteFile(path.c_str());
		}

		path = nNum;
		path = "Activity" + path;
		path += ".jpg";
		if ( !tryDownloadAdFile(path.c_str()) )
		{
			path = "resource/ad/" + path;
			DeleteFile(path.c_str());
		}
	
		path = nNum;
		path = "Activity" + path;
		path += ".ini";
		if ( !tryDownloadAdFile(path.c_str()) )
		{
			path = "resource/ad/" + path;
			DeleteFile(path.c_str());
		}

		++index;
	}
	return 0;
}

void GameManager::createDownloadAdThread()
{
	DWORD threadid;
	HANDLE threadHandle = CreateThread( NULL, 0, beginDownAdfile, 0, 0, &threadid );
	CloseHandle( threadHandle );
}

bool GameManager::CanFindFile( const char* path )
{
	ifstream fin(path);
	if ( !fin.fail() )
	{
		fin.close();
		return true;
	}
	return false;
}