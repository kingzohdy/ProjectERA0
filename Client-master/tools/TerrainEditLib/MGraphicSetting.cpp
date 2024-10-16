

#include ".\mgraphicsetting.h"

#include "mterrainedit.h"

using namespace TE;
using namespace Ogre;

extern Ogre::Light *g_plightnode ;

MGraphicSetting  g_GraphicSetting;

MGraphicSetting::MGraphicSetting(void)
{
//	memset( &m_Option , 0 , sizeof( MGrahicOption_t ) );
//
//	memset( &m_GraphicOption , 0 , sizeof( GraphicOptions )) ;
//
//	m_GraphicOption.m_nViewDistance = 10000;
}

MGraphicSetting::~MGraphicSetting(void)
{
}

void   MGraphicSetting::SetFog( bool bFog )
{
/*     m_Option.bFog = bFog;*/
};
	
void   MGraphicSetting::SetWaterLevel( int  nlevel )
{
//	m_Option.nWaterLevel = nlevel;
//		
//	m_GraphicOption.m_nWaterLevel  = nlevel ;
//
//	g_pEngine->SetGraphicOptions(m_GraphicOption);

};
	
void   MGraphicSetting::SetParticleLevel( int nlevel )
{
//	m_Option.nParticleLevel = nlevel;
//	
//	m_GraphicOption.m_nParticalDetailLevel  = nlevel ;
//
//	g_pEngine->SetGraphicOptions(m_GraphicOption);

};
	
void   MGraphicSetting::SetTerrainLevel( int nLevel )
{
//	m_Option.nTerrainLevel  = nLevel;
//	
//	//m_GraphicOption.
//
//	g_pEngine->SetGraphicOptions(m_GraphicOption);
};
	
	
void   MGraphicSetting::SetBlur( float  fBlur)
{
/*	g_pEngine->GetSceneMgr ()->SetBlur ( fBlur );*/
};
	
void   MGraphicSetting::SetGray( float  fGray)
{
//	g_pEngine->GetSceneMgr ()->SetGrayValue ( fGray );
};
	
	
void  MGraphicSetting::SetMotionBlur( bool sign)
{
//	g_pEngine->GetSceneMgr()->SetMotionBlur( sign );
};
	
void  MGraphicSetting::SetGrayEx( bool sign )
{
//	g_pEngine->GetSceneMgr()->SetGray( sign );
};

void   MGraphicSetting::SetHeightScene( float fHeightScene)
{
//	g_pEngine->GetSceneMgr ()->SetHighScene ( fHeightScene );
};

void   MGraphicSetting::SetScene(int nlevel )
{
//	m_GraphicOption.m_nBloom = nlevel ;
//
//	g_pEngine->SetGraphicOptions(m_GraphicOption);
//
//	if( nlevel == 1)
//		g_pEngine->GetSceneMgr()->SetBloom( true );
//	else
//		g_pEngine->GetSceneMgr()->SetBloom( false );
}
	
void   MGraphicSetting::SetBloomPara( float threshold, float bloomwidth,float bloomscale )
{
//	g_pEngine->GetSceneMgr()->SetBloomPara( threshold,  bloomwidth, bloomscale);
};

void   MGraphicSetting::SetBloom( bool  bBloom )
{
//	m_Option.bBloom = bBloom;
//
//	m_GraphicOption.m_nBloom = bBloom ;
//
//	g_pEngine->GetSceneMgr()->SetBloom( bBloom );
//
//	//g_pEngine->SetGraphicOptions(m_GraphicOption);
};
	
void  MGraphicSetting::SetShaderLevel( int nLevel )
{
//	g_pEngine->GetSceneMgr ()->SetShaderLevel( nLevel );
}
		
void   MGraphicSetting::SetGlow( bool bGlow )
{
//	g_pEngine->GetSceneMgr()->SetGlow( bGlow );
};

void   MGraphicSetting::SetHDR ( bool   bHDR )
{
//	m_Option.bHDR = bHDR ;
//
//	g_pEngine->SetGraphicOptions(m_GraphicOption);

};
	
void   MGraphicSetting::SetShadowLevel( int nlevel )
{
//	m_Option.nShadowLevel = nlevel;
//	m_GraphicOption.m_nShadowLevel = nlevel ;
//
//	g_pEngine->SetGraphicOptions(m_GraphicOption);
//
//	
//	if( nlevel != 0 )
//	{
//		if( g_plightnode )
//		{
//			g_plightnode->EnableShadow( g_pRoom );
//		}
//
//	}
//	else
//	{
//		if( g_plightnode )
//		{
//			g_plightnode->DisableShadow();
//		}
//	}

};