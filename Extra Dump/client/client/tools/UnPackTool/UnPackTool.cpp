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
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	bool bSuccess = false;//�ɹ���־
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
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
		cout<<"�����벹���ļ�·���ͽ�ѹ·��\n"<<endl;
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
