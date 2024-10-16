#include <memory>
#include <string>
#include "SystemSettingConfig.h"
#include "tinyxml.h"
#include "XMLData.h"

using namespace std;
using namespace Launcher;

const string CLIENT_CFG_PATH = "client.cfg";

static int Win_ScreenMode[3][2] = 
{
	{800, 600}, {1024, 768}, {1280, 800}
};

bool SystemSettingConfig::SaveSystemSettingFile( )
{
	XMLData data;

	if( !data.loadFile( CLIENT_CFG_PATH ) )
	{
		return false;
	}
	XMLNode		root = data.getRootNode();

	XMLNode winnode	= root.getChild( "RenderSystem" ).getChild( "MainWindow" );
	winnode.setAttribBool( "fullscreen",	m_bFullScreen );
	winnode.setAttribInt(	"width",		m_nWinWidth 	);	
	winnode.setAttribInt(	"height",		m_nWinHeight 	);	
	winnode.setAttribInt(	"multisample",	m_nMultiSampleLevel 	);	
	winnode.setAttribBool(	"sync_refresh",	m_bSyncRefresh 	);	

	XMLNode fxnode	= root.getChild( "RenderSystem" ).getChild( "FXSetting" );
	fxnode.setAttribBool(	"bloom",		m_bBloom	);
	fxnode.setAttribBool(	"distort",		m_bDistort	);
	fxnode.setAttribBool(	"waterreflect",	m_bWaterReflect	);
	fxnode.setAttribInt(	"shadowmap",	m_nShadowmapSize	);

	XMLNode soundnode	= root.getChild( "SoundSystem" ).getChild( "SoundParam" );

	auto_ptr<TiXmlElement> pSoundParamElement;
	if ( soundnode.isNull() )
	{
		XMLNode& soundSystemNode = root.getChild( "SoundSystem" );
		pSoundParamElement.reset( new TiXmlElement( "SoundParam") );
		soundSystemNode.m_pElement->LinkEndChild( pSoundParamElement.get() );
		soundnode = root.getChild( "SoundSystem" ).getChild( "SoundParam" );
	}
	soundnode.setAttribFloat(	"soundvol",	m_fGlobalSoundVolume	);
	soundnode.setAttribFloat(	"musicvol",	m_fGlobalMusicVolume	);

	data.saveFile( CLIENT_CFG_PATH );

	return true;
}


bool SystemSettingConfig::LoadSystemSettingFile( )
{
	XMLData data;

	if( !data.loadFile( CLIENT_CFG_PATH ) )
	{
		return false;
	}

	XMLNode root = data.getRootNode();

	XMLNode winnode		= root.getChild( "RenderSystem" ).getChild( "MainWindow" );
	m_nWinWidth			= winnode.attribToInt( "width" );
	m_nWinHeight		= winnode.attribToInt( "height" );
	m_bFullScreen		= winnode.attribToBool( "fullscreen" );
	m_nMultiSampleLevel	= winnode.attribToBool( "multisample" );
	m_bSyncRefresh		= winnode.attribToBool( "sync_refresh" );

	XMLNode fxnode		= root.getChild( "RenderSystem" ).getChild( "FXSetting" );	
	m_bBloom			= fxnode.attribToBool( "bloom" );
	m_bDistort			= fxnode.attribToBool( "distort" );
	m_nShadowmapSize	= fxnode.attribToInt( "shadowmap" );
	m_bWaterReflect		= fxnode.attribToBool( "waterreflect" );

	float musicvol = 1.0f;
	float soundvol = 1.0f;

	XMLNode soundnode	= root.getChild( "SoundSystem" ).getChild( "SoundParam" );
	if ( !soundnode.isNull() )
	{
		musicvol = soundnode.attribToFloat("musicvol");
		soundvol = soundnode.attribToFloat("soundvol");
	}

	m_fGlobalSoundVolume = soundvol;
	m_fGlobalMusicVolume = musicvol;

	return true;
}
