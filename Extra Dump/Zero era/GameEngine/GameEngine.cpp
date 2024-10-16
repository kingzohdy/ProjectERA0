#include "AvatarEntity.h"
#include "OgreXMLData.h"
#include "OgreLog.h"
#include <Windows.h>
#include "cs_net.h"
#include "des.h"
#include "CIniFile.h"

using namespace Ogre;
using namespace std;

bool LoadGameConfigFromXML(GameConfig &config, XMLData *pxmldata)
{
	XMLNode root = pxmldata->getRootNode();

	XMLNode winnode			= root.getChild("RenderSystem").getChild("MainWindow");
	//config.win_title		= winnode.attribToBool( "title" ); // 标题显示 2012.04.09
	config.win_width		= winnode.attribToInt("width");
	config.win_height		= winnode.attribToInt("height");
	config.win_fullscreen	= winnode.attribToBool("fullscreen");

	XMLNode pnode = root.getChild("ResDB");
	if ( !pnode.isNull() )
	{
		if ( strcmp(pnode.attribToString("mode"), "db" ) == 0 )
		{ 
			config.resdb_mode = RESDB_DB;
		}
		else config.resdb_mode = RESDB_FILE;

		XMLNode pfilenode = pnode.getChild("file");
		if(!pfilenode.isNull())
		{
			strncpy(config.resdb_dir, pfilenode.attribToString("dir"),128);
		}
		XMLNode pdbnode = pnode.getChild("db");
		if(!pdbnode.isNull())
		{
			strncpy(config.resdb_server, pdbnode.attribToString("server"),128);
			strncpy(config.resdb_user, pdbnode.attribToString("user"),32);
			strncpy(config.resdb_passwd, pdbnode.attribToString("passwd"),32);
			strncpy(config.resdb_database, pdbnode.attribToString("database"),32);
		}
	}
	else
	{
		LOG_SEVERE("no resdb info in config");
		return false; 
	}

	// begin  标题显示 2012.04.10
	pnode = root.getChild( "ShowTitle" );
	if ( !pnode.isNull() )
	{
		config.win_title = pnode.attribToBool( "flag" );
	}
	else
	{
		LOG_SEVERE("no showtitle info in config");
		config.win_title = true; // show all information
	} // end  标题显示 2012.04.10
	
	//pnode = root.getChild("AccountServer");
	//if(!pnode.isNull())
	//{
	//	strncpy(config.account_server, pnode.attribToString("ip"));
	//	config.account_port = pnode.attribToInt("port");
	//}

	pnode = root.getChild( "ClientType" );
	if( !pnode.isNull() )
	{
		config.clt_type = pnode.attribToInt( "value" );
	}else
	{
		config.clt_type = 0;
	}

	return true;
}

// 获取客户端版本号
unsigned int getLocalVersion()
{
// 	char curdir[256] = {0};
// 	::GetCurrentDirectory(256, curdir);
	static unsigned int uVer = 0;
	if( uVer != 0 )
	{
		return uVer;
	}
	
	// 先打开已经加密的文件
	string strEncryptFilePath	= "version.data";

	// 再解密已经加密的
	char* pszDecryptBuffer	= getDecryptFileBuffer( strEncryptFilePath.c_str(), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		//MessageBox(NULL, "读取version.data文件错误", "错误", MB_OK);
		return 0;
	}
	
	char szVersion[512];
	CIniFile iniNewVer;
	iniNewVer.Prepare( pszDecryptBuffer );
	iniNewVer.GetString( "version", "ver", "0", szVersion, 512 );
	iniNewVer.Close();

	delete[] pszDecryptBuffer;
	uVer = version_to_uint( ( char* )szVersion );	
	return uVer; 
}