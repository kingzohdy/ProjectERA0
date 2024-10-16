#ifndef __SYSTEM_SETTING_CONFIG_H__
#define __SYSTEM_SETTING_CONFIG_H__

class SystemSettingConfig//tolua_export
{
public:
	SystemSettingConfig() 
	{
		m_bFullScreen		= false;
		m_nWinWidth			= 1024;
		m_nWinHeight		= 768;
		m_bBloom			= true;
		m_bDistort			= true;
		m_bWaterReflect		= true;
		m_nShadowmapSize	= 2048;
		m_nMultiSampleLevel	= 2;
		m_bSyncRefresh		= true;

		m_fGlobalSoundVolume	= 0.5f;
		m_fGlobalMusicVolume	= 0.5f;
	}

	bool SaveSystemSettingFile( );
	bool LoadSystemSettingFile( );

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

};


#endif 