
#include ".\projsetting.h"

#include <windows.h>
#include   <algorithm>   

#include   <direct.h>   
#include   <shlobj.h>   

using namespace TE ;

extern std::string      g_path;
extern HWND             g_hWnd;

CProjSetting g_ProjSetting;


CProjSetting::CProjSetting(void)
{	
	//Ĭ��·��
	m_strTileTexturePath = "scene\\tileset";
	//��ͼ���·��
	m_strMapPath = "maps";
	//ģ�ʹ��·��
	m_strModelPath = "scene\\model";
	//�����Ӧ·��
	m_strTexturePath = "scene\\texture";

	ChangeMayPath();
}

CProjSetting::~CProjSetting(void)
{
}
 //�򿪶Ի���Ŀ¼;����( ��Ӧģʽ )
void CProjSetting::OpenDirDlg(int mode)
{
	BROWSEINFO   bi   ={0};   
	char   pszDisplayName[MAX_PATH]   ="\0";   

    bi.pszDisplayName   =   pszDisplayName;   
    bi.lpszTitle= "ѡ��Ŀ¼:";   
    bi.hwndOwner=  g_hWnd ;   
    bi.ulFlags= BIF_STATUSTEXT ;//   
    LPITEMIDLIST   lpIL   =   SHBrowseForFolder(&bi);   
   if(lpIL)   
   {     
	   SHGetPathFromIDList(lpIL,bi.pszDisplayName);    

	   std::string info = bi.pszDisplayName ;
	   int index = info.find("\\");

	   info = info.substr( index );

	   switch( mode )
	   {
	   case 0://�ر�������Ŀ¼
		   m_strTileTexturePath = info;
		   break;
	   case 1://��ͼ���Ŀ¼
		   m_strMapPath = info;
		   break;
	   case 2://ģ�ʹ��Ŀ¼
		   m_strModelPath = info;
		   break;
	   case 3://������Ŀ¼
		   m_strTexturePath = info;
		   break;
	   }
	 //  MessageBox(bi.pszDisplayName); 
  }
}
//�滻ΪMAY��·��
void CProjSetting::ChangeMayPath()
{
	/*
	int index = m_strTileTexturePath.find("chunkdata");
	if( index == -1 )
	{
		MessageBox( NULL , "��Ŀ���ô���,�ļ�Ŀ¼������chunkdata��" , "��ʾ"  , MB_OK );
		return;
	}

	m_strTileTextureMayPath = m_strTileTexturePath.substr( index + strlen("chunkdata") +1  );

	std::replace( m_strTileTextureMayPath.begin() , m_strTileTextureMayPath.end() , '\\' , '.');
	std::replace( m_strTileTextureMayPath.begin() , m_strTileTextureMayPath.end() , '/' , '.');
	*/
}
//����Ĭ�ε������ļ� ; ����Ϊ�ٱ�ʾû�гɹ�
bool CProjSetting::Init ()
{
	m_strMapFullPath = g_path + "\\" + m_strMapPath;
	m_strModelFullPath = g_path + "\\" + m_strModelPath;
	m_strTileTextureFullPath = g_path + "\\" + m_strTileTexturePath;
	m_strTextureFullPath = g_path + "\\" + m_strTexturePath;

	/*
	char strProjPath[260],strInfo[260];

	strcpy( strProjPath , g_path.c_str () );
	strcat( strProjPath , "\\data\\proj\\projectSetting.ini" );

	FILE *fp = fopen( strProjPath , "rb" );

	if( fp == NULL )
	{
		return false;
	}

	fclose( fp );

    GetPrivateProfileString( "ProjectSetting" , "MapPath" , "",strInfo , 260 ,strProjPath );
	m_strMapPath = strInfo ;

    GetPrivateProfileString( "ProjectSetting" , "ModelPath" , "",strInfo , 260 ,strProjPath );
    m_strModelPath = strInfo ;

    GetPrivateProfileString( "ProjectSetting" , "TileTexturePath" ,"", strInfo , 260 ,strProjPath );
	m_strTileTexturePath = strInfo ;
   
    GetPrivateProfileString( "ProjectSetting" , "TexturePath" ,"", strInfo , 260 ,strProjPath );
	m_strTexturePath = strInfo ;


	ChangeMayPath();
*/
	return true ;
}
	
bool  CProjSetting::Read()
{
	return false;
};
//�����Ӧ����Ŀ����	
bool  CProjSetting::Save()
{/*
	char strProjPath[260] ;

	strcpy( strProjPath , g_path.c_str () );
	strcat( strProjPath , "\\data\\proj\\projectSetting.ini" );

	FILE *fp = fopen( strProjPath , "rb" );

	if( fp == NULL )
	{
		return false;
	}
	
	fclose( fp );

	WritePrivateProfileString( "ProjectSetting" , "MapPath" ,m_strMapPath.c_str() ,strProjPath );
	WritePrivateProfileString( "ProjectSetting" , "ModelPath" , m_strModelPath.c_str() ,strProjPath );
	WritePrivateProfileString( "ProjectSetting" , "TileTexturePath" , m_strTileTexturePath.c_str()  ,strProjPath );
	WritePrivateProfileString( "ProjectSetting" , "TexturePath" , m_strTexturePath.c_str() ,strProjPath );
*/
	return true;
};