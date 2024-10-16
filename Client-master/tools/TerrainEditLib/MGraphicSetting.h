
#pragma once

namespace  TE
{


struct MGrahicOption_t
{
	bool     bFog;
	int      nWaterLevel;
	int      nParticleLevel;
	bool     bBloom;
	bool     bHDR;
	int      nShadowLevel;
	int      nTerrainLevel;
};

class MGraphicSetting
{
public:
	MGraphicSetting(void);
	~MGraphicSetting(void);

	void   SetFog( bool bFog );
	void   SetWaterLevel( int  nlevel );
	void   SetParticleLevel( int nlevel );
	void   SetTerrainLevel( int nLevel );
	void   SetBloom( bool  bBloom );
	void   SetHDR ( bool   bHDR );
	void   SetShadowLevel( int nlevel );
	void   SetScene(int nlevel );


	void  SetBlur( float  fBlur);
	void  SetGray( float  fGray);
	void  SetHeightScene( float fHeightScene);

	void  SetMotionBlur( bool sign);
	void  SetGrayEx( bool sign );

	void  SetShaderLevel( int nLevel );

	void   SetGlow( bool bGlow );
	void   SetBloomPara( float threshold, float bloomwidth,float bloomscale );
private:
	MGrahicOption_t               m_Option;
		 
	//MAY::GraphicOptions            m_GraphicOption;
};


};

extern TE::MGraphicSetting  g_GraphicSetting;