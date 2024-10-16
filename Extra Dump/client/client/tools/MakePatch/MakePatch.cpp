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
			cout<<"usage:"<<argv[0]<<" �ɰ汾Ŀ¼ �°汾Ŀ¼ \n";
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
		
		// ���ɰ汾��Ϣ�ļ�		
		if( version_to_uint( str1.c_str() ) > version_to_uint( str2.c_str() ) )
		{
			printf( "Ҫ�򲹶��İ汾�Ÿ���Ŀ��汾\n" );
			system("Pause");
			return nRetCode;
		}
// 		patch.createVer( str2.c_str(), strDes.c_str() );
// 		patch.createVer( str1.c_str(), strSrc.c_str() );
		
		string strVerDate = string( strDes.c_str() ) + string("\\version.data");
		// ����ѹ����
		patch.makePatch( strDes.c_str(), strSrc.c_str(), "update.pkg" );
		// �����Խ�ѹ���°�
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
printf( "�ļ�:%s�޷�����\n", strPath.c_str() );
}
}
}*/