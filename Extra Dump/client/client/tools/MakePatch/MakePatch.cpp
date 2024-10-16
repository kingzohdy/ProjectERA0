// MakePatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MakePatch.h"
#include "CreatePatch.h"
#include "OgrePackageFile.h"
#include "OgreStringUtil.h"

#include "Md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

unsigned int version_to_uint(const char *pszVer)
{
	// 1.2.100   1--8bit  2--8bit  100--16bit
	unsigned int iVersion = 0;
	int iSegment = 0;
	const char *pszTempVer = pszVer;
	int iCount = 0;

	while(pszTempVer)
	{
		iCount ++;
		iSegment = atoi(pszTempVer);
		if (iSegment < 0)
		{
			return -1;
		}
		if (iCount == 1)
		{
			if (iSegment > 255)
			{
				return -1;
			}

			iVersion += iSegment << 24;
		}
		else if(iCount == 2)
		{
			if (iSegment > 255)
			{
				return -1;
			}
			iVersion += iSegment << 16;
		}
		else if(iCount == 3)
		{
			if (iSegment > 65535)
			{
				return -1;
			}
			iVersion += iSegment;
		}
		else
		{return -1;}

		pszTempVer = strstr(pszTempVer, ".");
		if (!pszTempVer)
		{
			return iVersion;
		}
		pszTempVer ++;
	}
	return -1;
}

// The one and only application object

CWinApp theApp;

using namespace std;

bool CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//存放要创建的目录字符串
	//确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	
	StringUtil::init();

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		
		if ( argc < 2 )
		{
			cout<<"usage:"<<argv[0]<<" 旧版本目录 新版本目录 \n";
			system("Pause");
			exit(EXIT_FAILURE);
		};
		
		string strexePath	= argv[0];
		string strSrc		= argv[1];
		string strDes		= argv[2];
		string str1 = strSrc;
		string str2 = strDes;
		
		char curPath[MAX_PATH];
		memset( curPath, 0, MAX_PATH );
		GetCurrentDirectory( MAX_PATH, curPath );

		strexePath = strexePath.substr( 0, strexePath.find_last_of("\\") );
		CCreatePatch patch;
		if( strSrc.find_last_of("\\") == -1 )
		{
			strSrc = string(curPath) + "\\" + strSrc;
		}else{
			str1 = strSrc.substr( strSrc.find_last_of("\\") + 1, strSrc.length() );
		}
		if( strDes.find_last_of("\\") == -1 )
		{
			strDes = string(curPath) + "\\" + strDes;
		}else{
			str2 = strDes.substr( strDes.find_last_of("\\") + 1, strDes.length() );
		}
	
		char szPatchName[256];
		sprintf( szPatchName, "%s_%s.exe", str1.c_str(), str2.c_str() );
		
		// 生成版本信息文件		
		if( version_to_uint( str1.c_str() ) > version_to_uint( str2.c_str() ) )
		{
			printf( "要打补丁的版本号高于目标版本\n" );
			system("Pause");
			return nRetCode;
		}
// 		patch.createVer( str2.c_str(), strDes.c_str() );
// 		patch.createVer( str1.c_str(), strSrc.c_str() );
		
		string strVerDate = string( strDes.c_str() ) + string("\\version.data");
		// 创建压缩包
		patch.makePatch( strDes.c_str(), strSrc.c_str(), "update.pkg" );
		// 创建自解压更新包
		patch.m_SelfExtractor.AddFile( "update.pkg" );
		patch.m_SelfExtractor.AddFile( strVerDate.c_str() );

		DeleteFile(szPatchName);
		patch.makeSelfExtractor( "Extractor.exe", szPatchName );
		DeleteFile( "update.pkg" );
	}
	system("Pause");
	return nRetCode;
}

/*FilePackage package;
if( package.open( "update.pkg", true ) )
{
size_t nFileNum = package.getNumFiles();
for( size_t i = 0; i < nFileNum; i++ )
{
DataStream* pStream = package.openFile( i );
const PkgFileInfo& fileInfo = package.getFileInfo( i );

string strPath( fileInfo.szPath );
if( string( fileInfo.szPath ).find_last_of("\\") != -1 )
{
char curPath[MAX_PATH];
memset( curPath, 0, MAX_PATH );
GetCurrentDirectory( MAX_PATH, curPath );
strPath = string( curPath ) + "\\" + strPath;

string createPath = strPath.substr( 0, strPath.find_last_of("\\") );
CreateMultipleDirectory( CString( createPath.c_str() ) );
}
FILE*	fp		= fopen( strPath.c_str() , "wb" );
if( fp != NULL )
{
long	len		= fileInfo.originsize;
char*	pbuf	= new char[len];
pStream->read( pbuf, len );
fwrite( pbuf, len, 1, fp );
fclose( fp );

delete pbuf;
}else{
printf( "文件:%s无法更新\n", strPath.c_str() );
}
}
}*/