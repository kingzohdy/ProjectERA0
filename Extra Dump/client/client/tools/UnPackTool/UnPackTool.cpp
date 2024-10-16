// UnPackTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UnPackTool.h"

#include "SelfExtractor.h"
#include "GamePackageFile.h"
#include "OgrePackageFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;


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
	}

	
	CSelfExtractor SelfExtractor;

	if( argc != 3 )
	{
		cout<<"请输入补丁文件路径和解压路径\n"<<endl;
		return 1;
	}
	CString szEXEPath = argv[1];
	CString strPatchPath = argv[2];
	string path = argv[2];
	SelfExtractor.ExtractAll( strPatchPath, szEXEPath, NULL, NULL );
	strPatchPath = strPatchPath + "\\update.pkg";
	FilePackage package;
 	if ( package.open( strPatchPath.GetBuffer(), true ))
	{
		int nFileNum = (int)package.getNumFiles();
		for ( int i = 0; i < nFileNum; i++ )
		{
			path = argv[2];
			DataStream* pStream = package.openFile( i );
			const PkgFileInfo& fileInfo = package.getFileInfo( i );
			string strPath( fileInfo.szPath );
			if( string( fileInfo.szPath ).find_last_of("\\") != string::npos )
			{
				path.append("\\");
				path.append(strPath);
				strPath = path;
				string createPath = strPath.substr( 0, strPath.find_last_of("\\") );
				CreateMultipleDirectory( CString( createPath.c_str() ) );
			}
			path = argv[2];
			path.append("\\");			
			path.append( fileInfo.szPath );
			string unComPath = path ;
			char tmpBuffer[256];
			_snprintf_s( tmpBuffer, sizeof( tmpBuffer ), "%s", fileInfo.szPath );
			_snprintf_s( tmpBuffer, sizeof( tmpBuffer ), "%s", unComPath.c_str() );
			FILE* fp = fopen( unComPath.c_str() , "wb" );
			if ( fp != NULL )
			{
				long	len		= fileInfo.originsize;
				char*	pbuf	= new char[len];
				pStream->read( pbuf, len );
				fwrite( pbuf, len, 1, fp );
				fclose( fp );
				delete pbuf;
			}
		}
	}
	return nRetCode;
}
