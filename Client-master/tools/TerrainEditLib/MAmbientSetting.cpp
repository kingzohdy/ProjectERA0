
#include ".\mambientsetting.h"

#include "mstream.h"
#include "mcamera.h"
#include "mterrainedit.h"
#include "soundmanager.h"
#include "MArchive.h"

#include "OgreLight.h"

using namespace TE;
using namespace Ogre;

#ifdef USE_OLD_AMBIENT
//定义全局环境信息
AmbientInfo_st  g_AmbientInfo;

extern Light *g_plightnode;

extern unsigned int g_ulFogColor;

extern std::string  g_skyFile;

bool g_bUpdateChangeData = false;

MAmbientSetting  g_AmbientSetting;

MAmbientSetting::MAmbientSetting(void)
{
	//LoadFile( "test" );
	memset( &g_AmbientInfo , 0 , sizeof( AmbientInfo_st ) );
	
	g_AmbientInfo.ambientColor.x = g_AmbientInfo.ambientColor.y 
		= g_AmbientInfo.ambientColor.z = 0.5f;
	g_AmbientInfo.directColor.x = g_AmbientInfo.directColor.y 
		= g_AmbientInfo.directColor.z = 1.0f;

	g_AmbientInfo.LightDirect.x = 1.0f ;
	g_AmbientInfo.LightDirect.y = 1.0f ;
	g_AmbientInfo.LightDirect.z = 1.0f ;
}

MAmbientSetting::~MAmbientSetting(void)
{

}
//修改对应时间段信息
void   MAmbientSetting::ModifyInfo( int time , MVec3 ambientColor , MVec3 directColor ,
		MVec3  LightDirect , const char* skyName )
{
	if( time < 0 || time >= M_DAY_TIME )
	{
		return;
	}

	//m_AmbientInfo[time].ambientColor = ambientColor;
	//m_AmbientInfo[time].directColor = directColor;
	m_AmbientInfo[time].LightDirect = LightDirect;
	strcpy( m_AmbientInfo[time].skyFileName , skyName );

};   
//保存文件 
bool   MAmbientSetting::SaveFile(const char* filename)
{
	std::string str = AMBIENT_INFO_PATH;

	MStream  stream;
	if ( !stream.WriteFile ( str.c_str () ) )
	{
		return false;
	}

	stream.Write ( &m_AmbientInfo , M_DAY_TIME*sizeof( AmbientInfo_st )) ;

	stream.WriteEnd ();

	return true;
};		
//保存缺省环境信息处理; 参数( 文件名 ) 返回( 为真表示成功 )
bool   MAmbientSetting::SaveCurAmbientInfo(const char* filename )
{	

	MArchive *ar = g_FileManager.CreateFileWriter( filename , 0 );
	if( ar )
	{
        g_AmbientInfo = *(g_AreaSet.GetArea(0)->GetAmbientInfo());
        g_AmbientInfo.cameraPos = g_Camera.m_Pos ;
		g_AmbientInfo.cameraDir.x = g_Camera.m_ViewPitch ;
		g_AmbientInfo.cameraDir .y = g_Camera.m_ViewYaw ;

		*ar << g_AmbientInfo ;
		MSAFE_DELETE( ar );	
		return true;
	}

	return false;
};

void MAmbientSetting::LoadCurAmbientInfo(const AmbientInfo_st& ambientInfo , bool bMoveCamera)
{
    g_AmbientInfo = ambientInfo;
    
    unsigned char r,g,b;
    r = (unsigned char)(g_AmbientInfo.fogColor.x * 255.0f)	;
    g = (unsigned char)(g_AmbientInfo.fogColor.y * 255.0f)  ;
    b =	(unsigned char)(g_AmbientInfo.fogColor.z * 255.0f)  ;

    Ogre::ColorQuad color1( r,g,b );
    g_ulFogColor = color1.c ;
    if( g_plightnode )
    {
        g_plightnode->setColor(ColourValue( g_AmbientInfo.directColor.x, g_AmbientInfo.directColor.y, g_AmbientInfo.directColor.z ,g_AmbientInfo.directColor.w ));
    }

    g_TerrainEdit.ChangeDirectLight( (int)g_AmbientInfo.lightAngle .x , (int)g_AmbientInfo.lightAngle.y );

    MVec3 turb;
    turb.x = g_AmbientInfo.nTurb / 1000.0f;
    turb.y = g_AmbientInfo.nWaterColor / 1000.0f;
    turb.z = g_AmbientInfo.nWaterColorWeight / 100.0f;

    g_TerrainEdit.SetWaterInfo( ambientInfo.nWeight / 100.0f , (float)g_AmbientInfo.nUVWrap , g_AmbientInfo.nSpeed / 1000.0f , g_AmbientInfo.nWaveDir );

    g_TerrainEdit.SetShadowStrength( g_AmbientInfo.fLightStrength );

	/*
    if( g_pRoom )
    {
        g_pRoom->SetAmbientLight ( g_AmbientInfo.ambientColor.x , g_AmbientInfo.ambientColor.y , g_AmbientInfo.ambientColor.z );

        g_pRoom->SetFogColor ( color1 );
        g_pRoom->SetFogRange( g_AmbientInfo.fogNear , g_AmbientInfo.fogFar );

        g_pRoom->SetHeightFog( true );
        g_pRoom->SetHeightFogColor( g_AmbientInfo.vHeightFogColor );
        g_pRoom->SetHeightFogHigh( g_AmbientInfo.fHeightFogMax );
        g_pRoom->SetHeightFogLow( g_AmbientInfo.fHeightFogMin );

        g_pRoom->SetFresnelTerm( g_AmbientInfo.nFresnelTerm / 100.0f );	
        g_pRoom->SetTurb( turb );
    }
    if( g_pTerrainRoom )
    {
        g_pTerrainRoom->SetAmbientLight ( g_AmbientInfo.ambientColor .x , g_AmbientInfo.ambientColor.y , g_AmbientInfo.ambientColor.z );

        g_pTerrainRoom->SetFogColor ( color1 );
        g_pTerrainRoom->SetFogRange( g_AmbientInfo.fogNear , g_AmbientInfo.fogFar );

        g_pTerrainRoom->SetHeightFog( true );
        g_pTerrainRoom->SetHeightFogColor( g_AmbientInfo.vHeightFogColor );
        g_pTerrainRoom->SetHeightFogHigh( g_AmbientInfo.fHeightFogMax );
        g_pTerrainRoom->SetHeightFogLow( g_AmbientInfo.fHeightFogMin );

        g_pTerrainRoom->SetFresnelTerm( g_AmbientInfo.nFresnelTerm / 100.0f );	
        g_pTerrainRoom->SetTurb( turb );
    }
	*/

    //如果需要，则更新镜头位置
    if( bMoveCamera )
    {
        g_Camera.m_ViewPitch 	=	g_AmbientInfo.cameraDir.x ;
        g_Camera.m_ViewYaw      =   g_AmbientInfo.cameraDir.y ;

        g_Camera.SetPosition ( g_AmbientInfo.cameraPos .x , g_AmbientInfo.cameraPos.y , g_AmbientInfo.cameraPos.z );

        g_MapInfoManager.m_MapData .id = g_AmbientInfo.mapID ;
        g_MapInfoManager.LoadMapData();

    }

    if( g_AmbientInfo.skyFileName[0] != 0 )
    {
        g_TerrainEdit.SetSkyModel( g_AmbientInfo.skyFileName );
    }
    if( g_AmbientInfo.strSoundFile[0] != 0 )
    {
        g_SoundManager.SetSceneMusic( g_AmbientInfo.strSoundFile );
    }
}

//加载当前环境信息处理; 参数( 文件名 ) 返回( 为真表示成功 )
bool   MAmbientSetting::LoadCurAmbientInfo(const char* filename , bool bMoveCamera )
{
	{


		 MArchive *ar = g_FileManager.CreateFileReader( filename , 0 );

		 if( ar == NULL )
		 {
			 return false;
		 }

		 *ar << g_AmbientInfo ;
		 MSAFE_DELETE( ar );

		unsigned char r,g,b;
		r = (unsigned char)(g_AmbientInfo.fogColor.x * 255.0f)	;
		g = (unsigned char)(g_AmbientInfo.fogColor.y * 255.0f)  ;
		b =	(unsigned char)(g_AmbientInfo.fogColor.z * 255.0f)  ;

		Ogre::ColorQuad color1( r,g,b );
		g_ulFogColor = color1.c ;
		if( g_plightnode )
		{
	        g_plightnode->setColor(ColourValue( g_AmbientInfo.directColor.x, g_AmbientInfo.directColor.y, g_AmbientInfo.directColor.z ,g_AmbientInfo.directColor.w ));
		}

		g_TerrainEdit.ChangeDirectLight( (int)g_AmbientInfo.lightAngle .x , (int)g_AmbientInfo.lightAngle.y );

     	MVec3 turb;
		turb.x = g_AmbientInfo.nTurb / 1000.0f;
		turb.y = g_AmbientInfo.nWaterColor / 1000.0f;
		turb.z = g_AmbientInfo.nWaterColorWeight / 100.0f;

        g_TerrainEdit.SetWaterInfo( g_AmbientInfo.nWeight / 100.0f , (float)g_AmbientInfo.nUVWrap , g_AmbientInfo.nSpeed / 1000.0f , g_AmbientInfo.nWaveDir );
   
		g_TerrainEdit.SetShadowStrength( g_AmbientInfo.fLightStrength );

		/*
		if( g_pRoom )
		{
			g_pRoom->SetAmbientLight ( g_AmbientInfo.ambientColor.x , g_AmbientInfo.ambientColor.y , g_AmbientInfo.ambientColor.z );

			g_pRoom->SetFogColor ( color1 );
			g_pRoom->SetFogRange( g_AmbientInfo.fogNear , g_AmbientInfo.fogFar );

			g_pRoom->SetHeightFog( true );
			g_pRoom->SetHeightFogColor( g_AmbientInfo.vHeightFogColor );
			g_pRoom->SetHeightFogHigh( g_AmbientInfo.fHeightFogMax );
			g_pRoom->SetHeightFogLow( g_AmbientInfo.fHeightFogMin );

	    	g_pRoom->SetFresnelTerm( g_AmbientInfo.nFresnelTerm / 100.0f );	
			g_pRoom->SetTurb( turb );
		}
		if( g_pTerrainRoom )
		{
			g_pTerrainRoom->SetAmbientLight ( g_AmbientInfo.ambientColor .x , g_AmbientInfo.ambientColor.y , g_AmbientInfo.ambientColor.z );

			g_pTerrainRoom->SetFogColor ( color1 );
			g_pTerrainRoom->SetFogRange( g_AmbientInfo.fogNear , g_AmbientInfo.fogFar );

			g_pTerrainRoom->SetHeightFog( true );
			g_pTerrainRoom->SetHeightFogColor( g_AmbientInfo.vHeightFogColor );
			g_pTerrainRoom->SetHeightFogHigh( g_AmbientInfo.fHeightFogMax );
			g_pTerrainRoom->SetHeightFogLow( g_AmbientInfo.fHeightFogMin );

	    	g_pTerrainRoom->SetFresnelTerm( g_AmbientInfo.nFresnelTerm / 100.0f );	
			g_pTerrainRoom->SetTurb( turb );
		}
		*/

        //如果需要，则更新镜头位置
		if( bMoveCamera )
		{
			g_Camera.m_ViewPitch 	=	g_AmbientInfo.cameraDir.x ;
			g_Camera.m_ViewYaw      =   g_AmbientInfo.cameraDir.y ;
				
			g_Camera.SetPosition ( g_AmbientInfo.cameraPos .x , g_AmbientInfo.cameraPos.y , g_AmbientInfo.cameraPos.z );

			g_MapInfoManager.m_MapData .id = g_AmbientInfo.mapID ;
			g_MapInfoManager.LoadMapData();

		}

		if( g_AmbientInfo.skyFileName[0] != 0 )
		{
			g_TerrainEdit.SetSkyModel( g_AmbientInfo.skyFileName );
		}
		if( g_AmbientInfo.strSoundFile[0] != 0 )
		{
			g_SoundManager.SetSceneMusic( g_AmbientInfo.strSoundFile );
		}
	
		g_bUpdateChangeData = true;

		return true;
	}

	return false;

};
//加载文件	
bool   MAmbientSetting::LoadFile(const char* filename)
{
	MStream  stream;

	if (!stream.ReadFile ( AMBIENT_INFO_PATH ))
	{
		return false;
	}

	stream.Read ( &m_AmbientInfo , M_DAY_TIME*sizeof( AmbientInfo_st ) );


	return true;
};		
//获得每个时间断信息
AmbientInfo_st*    MAmbientSetting::GetInfo( int time )
{
	if( time > M_DAY_TIME )
	{
		return NULL;
	}

	return  &m_AmbientInfo[time];
};

#endif