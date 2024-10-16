#include <memory>
#include <string>
#include <Windows.h>
#include "SystemSettingConfig.h"
#include "OgreXMLData.h"
#include "tinyxml.h"
#include "OgreRoot.h"
#include "OgreOSUtility.h"
#include "GameClientOpt.h"
#include "GameActorManager.h"
#include "ogrebloom.h"
#include "ogredistort.h"
#include "OgreShadowmap.h"
//#include "OgreSceneManager.h"
#include "GameManager.h"
#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"

using namespace std;
using namespace Ogre;

const string CLIENT_CFG_PATH = "client.cfg";

SystemSettingConfig::SystemSettingConfig() 
{
	m_bFullScreen		= false;
	m_nWinWidth			= 1024;
	m_nWinHeight		= 768;
	m_bBloom			= true;
	m_bDistort			= true;
	m_bWaterReflect		= true;
	m_nShadowmapSize	= 2048;
	m_nMultiSampleLevel	= 0;
	m_bSyncRefresh		= true;

	m_fGlobalSoundVolume	= 0.5f;
	m_fGlobalMusicVolume	= 0.5f;
	m_bMute					= false;

	m_bWriteCompiledShader = false;
	m_fUIScale	= 1.0f;
	m_fViewSizeScale = 1.0f;
	m_nFPS		= 30;
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_GAMESETTING, MAX_GAME_SET_OPT * sizeof( char ) );
	memset( &m_curChgEffect, 0, sizeof( m_curChgEffect ) );
}

void SystemSettingConfig::applyChgEffect()
{
	if ( m_curChgEffect.m_bChgBloom )
	{
		Root::getSingleton().setBloom( m_bBloom );
	}

	if ( m_curChgEffect.m_bChgDistortEffect )
	{
		Root::getSingleton().setDistort( m_bDistort );
	}

	if ( m_curChgEffect.m_bChgShadow )
	{
		Root::getSingleton().setShadowmapSize( m_nShadowmapSize );
	}

	if ( m_curChgEffect.m_bChgSyncRefresh )
	{
		Root::getSingleton().setSyncRefresh( m_bSyncRefresh );
	}

	if ( m_curChgEffect.m_bChgWaterReflect )
	{
		// TerrainBlock 需重启才能生效
		Root::getSingleton().setWaterReflect( m_bWaterReflect );
	}

	if ( m_curChgEffect.m_bChgMultiSampleLevel )
	{
		Root::getSingleton().setMultiSample( m_nMultiSampleLevel );
	}

	if ( m_curChgEffect.m_bChgBloom || m_curChgEffect.m_bChgDistortEffect || m_curChgEffect.m_bChgShadow ||
			m_curChgEffect.m_bChgWaterReflect || m_curChgEffect.m_bChgSyncRefresh || m_curChgEffect.m_bChgMultiSampleLevel )
	{
		//SceneManager::getSingleton().resetDraw();
		RenderWindow *prwin = RenderSystem::getSingleton().findRenderWindow( GameManager::getSingleton().m_hWnd );
		if( prwin != NULL )
		{
			prwin->setSyncRefresh( m_bSyncRefresh );
			prwin->setMultiSampleLevel( m_nMultiSampleLevel );
			prwin->forceReset();
		}
	}
	Root::getSingleton().setViewSize( m_fViewSizeScale );
	
	memset( &m_curChgEffect, 0, sizeof( m_curChgEffect ) );
}

void SystemSettingConfig::setGameOpt( int nIndex, bool bOpen )
{
	assert( nIndex < MAX_GAME_SET_OPT );
	ActorManager& actorMgr = ActorManager::getSingleton();
	actorMgr.setGameOpt( nIndex, bOpen );
}

bool SystemSettingConfig::getGameOpt( int nIndex )
{
	assert( nIndex < MAX_GAME_SET_OPT );
	return ActorManager::getSingleton().getGameOpt( nIndex );
}

char SystemSettingConfig::getGameOptValue( int nIndex )
{
	assert( nIndex < MAX_GAME_SET_OPT );
	return ActorManager::getSingleton().getGameConfig().getGameOptValue( nIndex );
}

void SystemSettingConfig::setGameOptValue( int nIndex, char chValue )
{
	assert( nIndex < MAX_GAME_SET_OPT );
	ActorManager& actorMgr = ActorManager::getSingleton();
	actorMgr.setGameOptValue( nIndex, chValue );
}

void SystemSettingConfig::setGameSaveValue( int nIndex, char chValue )
{
	ActorManager::getSingleton().getGameConfig().setGameOpt( nIndex, chValue );
}

void SystemSettingConfig::setDefaultGameOpt()
{
	static int s_defaultConfig[][2] = { { GAME_OPT_SHOW_MONSTER_HP, 0 }, { GAME_OPT_SHOW_MONSTER_NAME, 0 }, 
	{ GAME_OPT_SHOW_OTHERPLAYER_HP, 0 }, { GAME_OPT_SHOW_MAINPLAYER_AND_TEAM_HP, 0 }, 
	{ GAME_OPT_SHOW_MAIN_CLAN_NAME, 1 }, { GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME, 1 }, };
	if( Root::getSingleton().IsSmartClient() )
	{
		// 微端默认显示血量
		s_defaultConfig[0][1] = 1;
	}
	ActorManager& actorMgr			= ActorManager::getSingleton();
	
	for ( int i = 0; i < GAME_OPT_REFUSE_RIDE_INVITE; ++i )
	{
		actorMgr.setGameOpt( i, 1 );
	}

	for ( int i = 0, size = sizeof( s_defaultConfig ) / ( 2 * sizeof( int ) ); i < size; ++i )
	{
		actorMgr.setGameOpt( s_defaultConfig[i][0], s_defaultConfig[i][1] );
	}

	for ( int i = GAME_OPT_REFUSE_RIDE_INVITE; i < MAX_GAME_SET_OPT; ++i )
	{
		actorMgr.setGameOpt( i, 0 );
	}

	saveGameOpt();
}

void SystemSettingConfig::saveGameOpt( bool bDelaySave /*= true*/ )
{
	GameClientOpt::getSingleton().setClientOpt(	SAVE_OPT_GAMESETTING, getGameOptBuffer(), bDelaySave );
}

char* SystemSettingConfig::getGameOptBuffer()
{
	return ActorManager::getSingleton().getGameConfig().getRawBuffer();
}

bool SystemSettingConfig::isSolutionSupport( int width, int height )
{
	return Ogre::IsSolutionSupport( width, height );
}

int SystemSettingConfig::getNumScreenMode( )
{
	return (int)Ogre::GetNumDisplayMode();
}

int SystemSettingConfig::getScreenModeWidth( int nMode )
{
	int width	= 0;
	int height	= 0;
	Ogre::GetDisplayMode( nMode, width, height );
	return width;
}

int SystemSettingConfig::getScreenModeHeight( int nMode )
{
	int width	= 0;
	int height	= 0;
	Ogre::GetDisplayMode( nMode, width, height );
	return height;
}

int SystemSettingConfig::getScreenModeWidth( bool bFullScreen, int width, int height )
{
	Ogre::GetScreenMode( bFullScreen, width, height );
	return width;
}

int SystemSettingConfig::getScreenModeHeight( bool bFullScreen, int width, int height )
{
	Ogre::GetScreenMode( bFullScreen, width, height );
	return height;
}

int SystemSettingConfig::getCurScreenWidth()
{
	return GetSystemMetrics( SM_CXSCREEN );
}

int SystemSettingConfig::getCurScreenHeight()
{
	return GetSystemMetrics( SM_CYSCREEN );
}

void SystemSettingConfig::SaveSystemSettingFile( )
{
	XMLData&	data = Ogre::Root::getSingleton().m_Config;
	if ( !data.loadRawFile( CLIENT_CFG_PATH ) )
	{
		return;
	}

	XMLNode	root = data.getRootNode();

	XMLNode winnode	= root.getChild( "RenderSystem" ).getChild( "MainWindow" );
	winnode.setAttribBool( "fullscreen",	m_bFullScreen );
	winnode.setAttribInt(	"width",		m_nWinWidth 	);	
	winnode.setAttribInt(	"height",		m_nWinHeight 	);	
	winnode.setAttribInt(	"multisample",	m_nMultiSampleLevel 	);	
	winnode.setAttribBool(	"sync_refresh",	m_bSyncRefresh 	);
	winnode.setAttribInt(	"fps",			m_nFPS 	);
	winnode.setAttribFloat(	"uiScale",		m_fUIScale 	);
	winnode.setAttribFloat( "viewSize",		m_fViewSizeScale );

	XMLNode fxnode	= root.getChild( "RenderSystem" ).getChild( "FXSetting" );
	fxnode.setAttribBool(	"bloom",		m_bBloom	);
	fxnode.setAttribBool(	"distort",		m_bDistort	);
	fxnode.setAttribBool(	"waterreflect",	m_bWaterReflect	);
	fxnode.setAttribInt(	"shadowmap",	m_nShadowmapSize	);
	
	XMLNode		soundSystemNode	= root.getChild( "SoundSystem" );
	XMLNode		soundnode		= soundSystemNode.getChild( "SoundParam" );

	if ( soundnode.isNull() )
	{
		soundSystemNode.m_pElement->LinkEndChild( new TiXmlElement( "SoundParam") );
		soundnode = soundSystemNode.getChild( "SoundParam" );
	}
	soundnode.setAttribFloat(	"soundvol",	m_fGlobalSoundVolume	);
	soundnode.setAttribFloat(	"musicvol",	m_fGlobalMusicVolume	);
	soundnode.setAttribBool(	"isMute",	m_bMute );

	data.saveFile( CLIENT_CFG_PATH );
}


bool SystemSettingConfig::LoadSystemSettingFile( Ogre::XMLData *pxmldata )
{
	XMLNode root = pxmldata->getRootNode();

	XMLNode winnode		= root.getChild( "RenderSystem" ).getChild( "MainWindow" );
	m_nWinWidth			= winnode.attribToInt( "width" );
	m_nWinHeight		= winnode.attribToInt( "height" );
	m_bFullScreen		= winnode.attribToBool( "fullscreen" );
	m_nMultiSampleLevel	= winnode.attribToInt( "multisample" );
	m_bSyncRefresh		= winnode.attribToBool( "sync_refresh" );
	m_nFPS				= winnode.attribToInt(	"fps" );
	if( winnode.hasAttrib("uiScale") )
	{
		m_fUIScale = winnode.attribToFloat( "uiScale" );
	}
	
	if( m_fUIScale < 0.5f )
	{
		m_fUIScale = 0.5f;
	}
	if( winnode.hasAttrib("viewSize") )
	{
		m_fViewSizeScale = winnode.attribToFloat( "viewSize" );
	}
	if( m_fViewSizeScale > 1.5f )
	{
		m_fViewSizeScale = 1.5f;
	}
	if( m_fViewSizeScale < 0.5f )
	{
		m_fViewSizeScale = 0.5f;
	}

	XMLNode fxnode		= root.getChild( "RenderSystem" ).getChild( "FXSetting" );	
	m_bBloom			= fxnode.attribToBool( "bloom" );
	m_bDistort			= fxnode.attribToBool( "distort" );
	m_nShadowmapSize	= fxnode.attribToInt( "shadowmap" );
	m_bWaterReflect		= fxnode.attribToBool( "waterreflect" );
	
	float musicvol	= 1.0f;
	float soundvol	= 1.0f;
	bool  bMute		= false;
	
	XMLNode soundnode	= root.getChild( "SoundSystem" ).getChild( "SoundParam" );
	if ( !soundnode.isNull() )
	{
		musicvol	= soundnode.attribToFloat("musicvol");
		soundvol	= soundnode.attribToFloat("soundvol");
		bMute		= soundnode.attribToBool( "isMute" );
	}
	
	m_fGlobalSoundVolume	= soundvol;
	m_fGlobalMusicVolume	= musicvol;
	m_bMute					= bMute;

	// 判断是否第一次启动游戏，若是则设置推荐配置
	XMLNode firstRunNode = root.getChild( "FirstRun" );
	if ( firstRunNode.isNull() )
	{
		Ogre::Root::getSingleton().setFirstRun( true );
		GameManager::getSingleton().setFirstRun( true );
		root.m_pElement->LinkEndChild( new TiXmlElement( "FirstRun") );
		firstRunNode = root.getChild( "FirstRun" );
		firstRunNode.setAttribBool( "flag", false );
		pxmldata->saveFile( CLIENT_CFG_PATH );
	}
	else
	{
		if ( firstRunNode.attribToBool( "flag" ) )
		{
			Ogre::Root::getSingleton().setFirstRun( true );
			GameManager::getSingleton().setFirstRun( true );
			firstRunNode.setAttribBool( "flag", false );
			pxmldata->saveFile( CLIENT_CFG_PATH );
		}
		else
		{
			Ogre::Root::getSingleton().setFirstRun( false );
			GameManager::getSingleton().setFirstRun( false );
		}
	}

	XMLNode writeCompiledShaderNode = root.getChild( "WriteCompiledShader" );
	if ( writeCompiledShaderNode.isNull() )
	{
		root.m_pElement->LinkEndChild( new TiXmlElement( "WriteCompiledShader") );
		writeCompiledShaderNode = root.getChild( "WriteCompiledShader" );
		writeCompiledShaderNode.setAttribBool( "flag", false );
		pxmldata->saveFile( CLIENT_CFG_PATH );

		m_bWriteCompiledShader = false;
	}
	else
	{
		if ( writeCompiledShaderNode.attribToBool( "flag" ) )
		{
			m_bWriteCompiledShader = true;
		}
		else
		{
			m_bWriteCompiledShader = false;
		}
	}

	XMLNode actorTransparentProcess = root.getChild("ActorTransparentProcess");
	if (actorTransparentProcess.isNull())
	{
		root.m_pElement->LinkEndChild(new TiXmlElement("ActorTransparentProcess"));
		actorTransparentProcess = root.getChild("ActorTransparentProcess");
		actorTransparentProcess.setAttribBool("flag", false);
		pxmldata->saveFile(CLIENT_CFG_PATH);

		Ogre::Root::getSingleton().setActorTransProcess(false);
	}
	else
	{
		if (actorTransparentProcess.attribToBool("flag"))
		{
			Ogre::Root::getSingleton().setActorTransProcess(true);
		}
		else
		{
			Ogre::Root::getSingleton().setActorTransProcess(false);
		}
	}

	XMLNode bakcSceneProcess = root.getChild("BackSceneProcess");
	if (bakcSceneProcess.isNull())
	{
		root.m_pElement->LinkEndChild(new TiXmlElement("BackSceneProcess"));
		bakcSceneProcess = root.getChild("BackSceneProcess");
		bakcSceneProcess.setAttribBool("flag", false);
		pxmldata->saveFile(CLIENT_CFG_PATH);

		Ogre::Root::getSingleton().setBackSceneProcess(false);
	}
	else
	{
		if (bakcSceneProcess.attribToBool("flag"))
		{
			Ogre::Root::getSingleton().setBackSceneProcess(true);
		}
		else
		{
			Ogre::Root::getSingleton().setBackSceneProcess(false);
		}
	}

	XMLNode saveShaderEnvKeys = root.getChild("SaveShaderEnvKeys");
	if (saveShaderEnvKeys.isNull())
	{
		root.m_pElement->LinkEndChild(new TiXmlElement("SaveShaderEnvKeys"));
		saveShaderEnvKeys = root.getChild("SaveShaderEnvKeys");
		saveShaderEnvKeys.setAttribBool("flag", false);
		pxmldata->saveFile(CLIENT_CFG_PATH);

		Ogre::Root::getSingleton().setSaveShaderEnvKey(false);
	}
	else
	{
		if (saveShaderEnvKeys.attribToBool("flag"))
		{
			Ogre::Root::getSingleton().setSaveShaderEnvKey(true);
		}
		else
		{
			Ogre::Root::getSingleton().setSaveShaderEnvKey(false);
		}
	}

	XMLNode compileShaderEnvKeys = root.getChild("CompileShaderEnvKeys");
	if (compileShaderEnvKeys.isNull())
	{
		root.m_pElement->LinkEndChild(new TiXmlElement("CompileShaderEnvKeys"));
		compileShaderEnvKeys = root.getChild("CompileShaderEnvKeys");
		compileShaderEnvKeys.setAttribBool("flag", false);
		pxmldata->saveFile(CLIENT_CFG_PATH);

		Ogre::Root::getSingleton().setCompileShaderEnvKeys(false);
	}
	else
	{
		if (compileShaderEnvKeys.attribToBool("flag"))
		{
			Ogre::Root::getSingleton().setCompileShaderEnvKeys(true);
		}
		else
		{
			Ogre::Root::getSingleton().setCompileShaderEnvKeys(false);
		}
	}

	XMLNode testMode = root.getChild("TestMode");
	if (testMode.isNull())
	{
		GameManager::getSingleton().setTestMode( false );
	}else{
		GameManager::getSingleton().setTestMode( testMode.attribToBool("flag") );
	}
	ValidateConfig();

	return true;
}


void SystemSettingConfig::ValidateConfig()
{
	if ( !m_bFullScreen )
	{
		return;
	}
	
	XMLData&	data = Ogre::Root::getSingleton().m_Config;
	if ( !data.loadRawFile( CLIENT_CFG_PATH ) )
	{
		return;
	}

	XMLNode	root	= data.getRootNode();
	XMLNode winnode	= root.getChild( "RenderSystem" ).getChild( "MainWindow" );

	int nScreenWidth = getCurScreenWidth();
	if ( m_nWinWidth != nScreenWidth )
	{
		m_nWinWidth = nScreenWidth;
		winnode.setAttribInt(	"width",		m_nWinWidth 	);	
	}

	int nScreenHeight = getCurScreenHeight();
	if ( m_nWinHeight != nScreenHeight )
	{
		m_nWinHeight = nScreenHeight;
		winnode.setAttribInt(	"height",		m_nWinHeight 	);	
	}

	data.saveFile( CLIENT_CFG_PATH );
}