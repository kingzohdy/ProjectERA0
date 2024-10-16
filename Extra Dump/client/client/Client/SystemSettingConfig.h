#ifndef __SYSTEM_SETTING_CONFIG_H__
#define __SYSTEM_SETTING_CONFIG_H__

#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "GameOpt.h"

class Ogre::XMLData;
//tolua_begin

struct tagChgEffect
{
	bool	m_bChgBloom;
	bool	m_bChgDistortEffect;
	bool	m_bChgWaterReflect;
	bool	m_bChgShadow;
	bool	m_bChgSyncRefresh;
	bool	m_bChgMultiSampleLevel;
};

class SystemSettingConfig//tolua_export
{//tolua_export
public:
	SystemSettingConfig(); 
	void	applyChgEffect();
	void	setGameOpt( int nIndex, bool bOpen );
	bool	getGameOpt( int nIndex );
	char	getGameOptValue( int nIndex );
	void	setGameOptValue( int nIndex, char chValue );
	void	setGameSaveValue( int nIndex, char chValue );
	void	setDefaultGameOpt();
	void	saveGameOpt(bool bDelaySave = true);
	char*	getGameOptBuffer();

	bool	isSolutionSupport( int width, int height );
	int		getNumScreenMode();
	int		getScreenModeWidth( int nMode );
	int		getScreenModeHeight( int nMode );
	int		getScreenModeWidth( bool bFullScreen, int width, int height );
	int		getScreenModeHeight( bool bFullScreen, int width, int height );
	int		getCurScreenWidth();
	int		getCurScreenHeight();

	void	SaveSystemSettingFile( );
	bool	LoadSystemSettingFile( Ogre::XMLData* pxmldata );
	
public:
	void	ValidateConfig();
public:

	bool	m_bFullScreen;
	int		m_nWinWidth;
	int		m_nWinHeight;
	bool	m_bBloom;
	bool	m_bDistort;
	bool	m_bWaterReflect;
	int		m_nShadowmapSize;
	int		m_nMultiSampleLevel;
	bool	m_bSyncRefresh;

	float	m_fGlobalSoundVolume;	// “Ù–ß
	float	m_fGlobalMusicVolume;	// ±≥æ∞“Ù¿÷
	bool	m_bMute;
	float	m_fUIScale;
	int		m_nFPS;
	bool	m_bWriteCompiledShader;
	float	m_fViewSizeScale;

	tagChgEffect	m_curChgEffect;

};//tolua_export
//tolua_end

#endif 
