
#include "stdafx.h"
#include "EditorManager.h"

#include <Windows.h>
#include <MMSystem.h>
#include <stdio.h>
#include <map>

#include "EditEventHandlerMgr.h"
#include "EnvRegion.h"
#include "Terrain.h"
#include "TerrainEdit.h"
#include "ControlsHelper.h"
#include "EditorCamera.h"
#include "MaskMap.h"
#include "MaskMapEdit.h"

#include "OgreGameScene.h"
#include "OgreLight.h"
#include "OgreCamera.h"
#include "OgreResourceManager.h"
#include "OgreTerrainData.h"
#include "OgreLog.h"

using namespace Ogre;
using namespace TE;

HINSTANCE g_Hinst;
EditorManager *g_pApp = NULL;

Ogre::Light *g_plightnode = NULL;

namespace TE
{
	extern std::string g_path;
}

HWND   g_hWnd ;

Ogre::GameScene *g_pRoom;
Ogre::GameScene *g_pModelRoom;
Ogre::GameScene *g_pTextureRoom;
Ogre::GameScene *g_pTerrainRoom;

static void RecoverCurDir()
{
	SetCurrentDirectory(g_path.c_str());
}

const char *GetCurDir()
{
	return g_path.c_str();
}

EditorManager::EditorManager()
{
	m_nWidth = 800 ;
	m_nHeight = 600 ;
	m_nColorBit = 32;

	m_bShowLiquid = true;
	m_bShowTerrain = true;;
	m_bShowObject = true;
	m_bShowQuad = true;
	m_bShowShadow = true;
}

EditorManager::~EditorManager()
{		
	g_EventProcessFactory.Destory();
	g_Terrain.Release();
	g_TerrainEdit.Release();
}

void    EditorManager::SavePathFile()
{
	TCHAR szFilename[MAX_PATH] = TEXT("");
	BOOL  bResult = FALSE;
	DWORD dwError = NOERROR;
	OPENFILENAME  ofn = {0};
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.lpstrFilter = TEXT("path file\0*.path\0\0");
	ofn.lpstrFile  = szFilename;
	ofn.nMaxFile  = MAX_PATH;
	ofn.Flags     = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY |
		OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;

	bResult = GetSaveFileName( &ofn );
	if( !bResult )
	{
		return ;
	}

	g_TerrainEdit.SavePathFile ( szFilename );

};

void    EditorManager::OpenPathFile()
{

	TCHAR strMapName[MAX_PATH] = _T("");
	TCHAR strMapNamePath[MAX_PATH] = _T("");

	OPENFILENAME ofn = { sizeof(OPENFILENAME), NULL, NULL,
		_T("Path Files (.path)\0*.path\0\0"), 
		NULL, 0, 1, strMapNamePath, MAX_PATH, strMapName, MAX_PATH, 
		strMapNamePath, _T("Open Path File"), 
		OFN_FILEMUSTEXIST, 0, 1, NULL, 0, NULL, NULL };

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		RecoverCurDir();
		g_TerrainEdit.LoadPathFile  ( strMapNamePath );
	}
};

void    EditorManager::Undo()
{
	g_TerrainEdit.Undo ();
};

void    EditorManager::SetLayer(int layer)
{
	g_TerrainEdit.SetCurShowLayer ( layer );
};

void EditorManager::SetupChooseColor(CHOOSECOLOR *cc,COLORREF *customclrs,HWND hwnd,COLORREF &init)
{
	int ca;
	for(ca = 0;ca<16;ca++){*(customclrs+ca) = RGB(255,255,255);}
	cc->lStructSize = sizeof(CHOOSECOLOR);
	cc->hwndOwner = hwnd;
	cc->rgbResult = init;
	cc->lpCustColors = customclrs;
	cc->Flags = CC_ANYCOLOR|CC_FULLOPEN|CC_RGBINIT;
}
void    EditorManager::SetAmbientLight()
{
	CHOOSECOLOR		 choosecolor;
	static COLORREF		customcolors[16];

	COLORREF	                m_currentcolor ;

	unsigned char ar,ag,ab;
	ar = ( g_TerrainEdit.m_MapEditInfo .nAmibentLightColor & 0x00ff0000 ) >> 16 ;
	ag = ( g_TerrainEdit.m_MapEditInfo .nAmibentLightColor & 0x0000ff00 ) >> 8 ;
	ab = ( g_TerrainEdit.m_MapEditInfo .nAmibentLightColor & 0x000000ff ) ;

	m_currentcolor = RGB(ar,ag,ab);

	SetupChooseColor(&choosecolor,customcolors,m_hWnd,m_currentcolor);

	float mr,mg,mb;
	if(ChooseColor(&choosecolor))
	{     	
		int color =  choosecolor.rgbResult;
		mb = ((color&0x00ff0000)>>16 )/ 255.0f;
		mg = ((color&0x0000ff00)>>8) / 255.0f;
		mr = ((color&0x000000ff)) / 255.0f;

		//g_TerrainEdit.SetAmbientLightColor ( mr,mg,mb,1.0f );	

		/*
		if( g_pRoom )
		{
		g_pRoom->SetAmbientLight ( mr , mg , mb );
		}
		if( g_pTerrainRoom )
		{
		g_pTerrainRoom->SetAmbientLight ( mr, mg , mb );
		}*/

		g_TerrainEdit.m_MapEditInfo.nAmibentLightColor =0xff000000 | ( ( color&0x000000ff)<<16 ) |  ( color&0x0000ff00) | ((color&0x00ff0000)>>16);

		g_TerrainEdit.SaveMapEditIni ();

	}
};

void    EditorManager::SetDirectLight()
{

	CHOOSECOLOR		 choosecolor;
	static COLORREF		customcolors[16];

	COLORREF	                m_currentcolor ;

	unsigned char ar,ag,ab;
	ar = ( g_TerrainEdit.m_MapEditInfo .nDirectLightColor & 0x00ff0000 ) >> 16 ;
	ag = ( g_TerrainEdit.m_MapEditInfo .nDirectLightColor & 0x0000ff00 ) >> 8 ;
	ab = ( g_TerrainEdit.m_MapEditInfo .nDirectLightColor & 0x000000ff ) ;

	m_currentcolor = RGB(ar,ag,ab);


	SetupChooseColor(&choosecolor,customcolors,m_hWnd,m_currentcolor);
	float mr,mg,mb;
	if(ChooseColor(&choosecolor))
	{     	
		int color =  choosecolor.rgbResult;
		mb = ((color&0x00ff0000)>>16)/ 255.0f;
		mg = ((color&0x0000ff00)>>8)/ 255.0f;
		mr = ((color&0x000000ff))/ 255.0f;

		//g_TerrainEdit.SetDirLightColor  ( mr,mg,mb,1.0f );		

		if( g_plightnode )
		{
			g_plightnode->setColor(Ogre::ColourValue( mr, mg, mb ,1.0f ));
		}

		g_TerrainEdit.m_MapEditInfo.nDirectLightColor  = 0xff000000 | ( ( color&0x000000ff)<<16 ) |  ( color&0x0000ff00) | ((color&0x00ff0000)>>16);

		g_TerrainEdit.SaveMapEditIni ();

	}
};

void EditorManager::SetShowMode(int mode)
{
	switch( mode )
	{
	case 0:
		m_bShowShadow = !m_bShowShadow;
		g_TerrainEdit.SetShowShadow (m_bShowShadow); 
		break;
	case 1:
		m_bShowTerrain = !m_bShowTerrain;
		g_TerrainEdit.SetShowChunk (m_bShowTerrain);
		break;
	case 2:
		m_bShowObject  = !m_bShowObject;
		g_TerrainEdit.SetShowObject (m_bShowObject);
		break;
	case 3:
		m_bShowQuad = !m_bShowQuad;
		g_TerrainEdit.SetShowQuadPath (m_bShowQuad);
		break;
	case 4:
		m_bShowLiquid = !m_bShowLiquid;
		g_TerrainEdit.SetShowLiquid (m_bShowLiquid);
		break;
	}
};

void EditorManager::SetCameraType(int mode)
{
	if( mode == 2 )
	{
		g_Camera.m_ViewYaw = -45.3f;
		g_Camera.m_ViewPitch = 314.7f;
		g_Camera.Update();
	}
	else
	{
		g_TerrainEdit.SetCameraType (mode);
	}
}

void EditorManager::Redo()
{
	g_TerrainEdit.Redo ();
};

bool EditorManager::init(HINSTANCE hinst, HWND hmainview, HWND hrespreview, HWND hradarview)
{
	m_hWnd = hmainview;
	g_Hinst = hinst;
	g_hWnd  = m_hWnd;

	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	std::string _file = exeFullPath;
	size_t index = _file.rfind ('\\');
	g_path = _file.substr(0 , index);

	RecoverCurDir();

	if(!g_TerrainEdit.Init(hinst, hmainview, hrespreview, hradarview))
	{
		return false;
	}

	LOG_INFO("MapEditor start in directory: %s", g_path);
	return true;
};

void EditorManager::calNaviMap()
{
	g_NavMapProcess.calNaviMapData(&g_TerrainEdit, 64, 50.0f);
}

void EditorManager::copyWalkMaskToFly()
{
	g_NavMapProcess.copyWalkMaskToFly(&g_TerrainEdit, 64);
}

void EditorManager::exportMaskFile()
{
	CString path = GetOpenFilePath(FALSE, "maps", "", "msk", "Mask Files(*.msk)\0*.msk\0\0", TRUE);
	EditorMaskMap maskmap;
	g_NavMapProcess.saveNaviDataToMaskMap(maskmap, &g_TerrainEdit, 64);

	maskmap.SaveNavFile(path);

	for(int i=0; i<4; i++)
	{
		char buffer[256];
		sprintf(buffer, "%s_%d.bmp", path.GetBuffer(), i);
		maskmap.SaveBmpFile(buffer, i);
	}
}

void EditorManager::ImportMaskFile()
{
	CString path = GetOpenFilePath(TRUE, "maps", "", "bmp", "BMP Files(*.bmp)\0*.bmp\0\0", TRUE);

	EditorMaskMap maskmap;
	maskmap.LoadBmpFile(path);
	g_NavMapProcess.loadMaskBMPMapToNaviData(maskmap, &g_TerrainEdit, 64);

	updateMaskFile(path);
}

void EditorManager::updateMaskFile(const char* path)
{
	EditorMaskMap maskmap;
	std::string filepath = path;

	size_t pos = filepath.find(".msk");
	std::string filename = filepath.substr( 0, pos );
	std::string mqtname = filename + ".mqt";
	std::string mskname = filename + ".msk";

	remove(mqtname.c_str());
	remove(mskname.c_str());
	g_NavMapProcess.saveNaviDataToMaskMap(maskmap, &g_TerrainEdit, 64);
	maskmap.SaveNavFile(mskname.c_str());
}

void EditorManager::AddViewMayMap()
{

	OPENFILENAME ofn;
	TCHAR szOpenFileNames[80*MAX_PATH];
	TCHAR szPath[MAX_PATH];
	TCHAR szFileName[80*MAX_PATH];
	TCHAR* p;
	int nLen = 0;
	ZeroMemory( &ofn, sizeof(ofn) );
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = TEXT("All Files(*.chu)\0*.chu\0");

	if( TRUE == GetOpenFileName( &ofn ) )
	{	
		if( g_pTerrainRoom == NULL)
			return;

		RecoverCurDir();

		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
		//当只选了一个文件时,下面这个NULL字符是必需的.
		//这里不区别对待选了一个和多个文件的情况
		szPath[ ofn.nFileOffset ] = '\0';
		nLen = lstrlen(szPath);

		if( szPath[nLen-1] != '\\' )   //如果选了多个文件,则必须加上'\\'
		{
			lstrcat(szPath, TEXT("\\"));
		}

		p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件


		while( *p )
		{   		
			ZeroMemory(szFileName, sizeof(szFileName));
			lstrcat(szFileName, szPath);  //给文件名加上路径  
			lstrcat(szFileName, p);    //加上文件名  
			lstrcat(szFileName, TEXT("\n")); //换行   
			p += lstrlen(p) +1;     //移至下一个文件

			std::string  mapFile = szFileName;
			int index = mapFile.rfind ('.');
			std::string  mapFileEx ;

			if( index != -1)    
			{
				mapFileEx = mapFile.substr ( 0 , index );
			}

			index = mapFileEx.find ("chunkdata");

			if( index == - 1)
			{
				MessageBox(NULL,"请打开chunkdata下的地形文件","提示" , MB_OK );
				return ;
			}
			mapFile = mapFileEx.substr ( index + strlen("chunkdata") + 1 );

			Ogre::TerrainTileSource *ptileres = static_cast<Ogre::TerrainTileSource *>(Ogre::ResourceManager::getSingleton().blockLoad(mapFile.c_str()));

			//g_pTerrainRoom->AddTerrainTile ( pTileSourece );

			//MSAFE_RELEASE( pTileSourece );
		}
	}
};

void EditorManager::SaveAlphaTex(int layer )
{
	TCHAR szFilename[MAX_PATH] = TEXT("");
	BOOL  bResult = FALSE;
	DWORD dwError = NOERROR;
	OPENFILENAME  ofn = {0};
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.lpstrFilter = TEXT("bmp file\0*.bmp\0\0");
	ofn.lpstrFile  = szFilename;
	ofn.nMaxFile  = MAX_PATH;
	ofn.Flags     = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY |
		OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;

	bResult = GetSaveFileName( &ofn );
	if( !bResult )
	{
		return ;
	}

	RecoverCurDir();

	unsigned char  *newmap = new unsigned char [1024*1024*3];

	g_TerrainEdit.SaveAlphaTex (szFilename , newmap, layer);

	delete [] newmap;
}

void EditorManager::LoadAlphaTex(int layer)
{
	char strMapName[MAX_PATH] ;
	char strMapNamePath[MAX_PATH] ; 
	strMapName[0] = 0;
	strMapNamePath[0]=0;

	memset( strMapNamePath , 0 , MAX_PATH);

	OPENFILENAME ofn = { sizeof(OPENFILENAME), NULL, NULL,
		_T("BMP Files (.bmp)\0*.bmp\0\0"), 
		NULL, 0, 1, strMapNamePath, MAX_PATH, strMapName, MAX_PATH, 
		strMapNamePath, _T("Open BMP Map File"), 
		OFN_FILEMUSTEXIST, 0, 1, NULL, 0, NULL, NULL };

	if( TRUE == GetOpenFileName( &ofn ) )
	{	
		RecoverCurDir();

		g_TerrainEdit.UpdateAlphaData ( strMapNamePath , layer );

	}
}


void EditorManager::GetGameDrawInfo(size_t&  drawNum,size_t &drawPra)
{
	drawNum = drawPra = 0;
};

void EditorManager::doFrame()
{
	static unsigned int lasttick = timeGetTime();
	static unsigned int accumtick = 0;

	unsigned int curtick = timeGetTime();
	accumtick += curtick - lasttick;

	if(accumtick >= 1000)
	{
		accumtick = 0;
	}

	ResourceManager::getSingleton().update(curtick-lasttick);
	g_TerrainEdit.doFrame(curtick-lasttick);

	lasttick = curtick;
}

float EditorManager::GetCameraPosX()
{
	return g_Camera.GetPosition ().z ;
};

float EditorManager::GetCameraPosY()
{
	return g_Camera.GetPosition ().y ;
};

float EditorManager::GetCameraPosZ()
{
	return g_Camera.GetPosition ().x ;
};

bool  EditorManager::AddMapSourc()
{
	char strMapName[MAX_PATH] ;
	char strMapNamePath[MAX_PATH] ; 
	strMapName[0] = 0;
	strMapNamePath[0]=0;

	memset( strMapNamePath , 0 , MAX_PATH);

	OPENFILENAME ofn = { sizeof(OPENFILENAME), NULL, NULL,
		_T("Map Files (.map)\0*.map\0\0"), 
		NULL, 0, 1, strMapNamePath, MAX_PATH, strMapName, MAX_PATH, 
		NULL, _T("Open Edit Map File"), 
		OFN_FILEMUSTEXIST, 0, 1, NULL, 0, NULL, NULL };

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		RecoverCurDir();

		EditorTerrainTile *pTile = new EditorTerrainTile;

		if( !pTile->LoadFile ( strMapNamePath ) )
			return false;

		g_Terrain.AddTitle ( pTile );

		return true;
	}     

	return false;

}

void  EditorManager::AddBulidingRoom(const char* filename)
{ 
	g_TerrainEdit.SetCurSelectBuildingModelName ("");
};

