
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include "../../sdk/zlib/zlib.h"
#include "zip.h"
#include <afxinet.h>
#include <vector>
#include "des.h"
#include "XMLData.h"
#include "CIniFile.h"

#pragma comment(lib, "..\\..\\sdk\\zlib\\zdll.lib")

struct FtpConfig
{
	FtpConfig()
	{
		strFtpServer = "";
	}
	CString		strFtpServer;
	int			nFtpPort;
	CString		strFtpUser;
	CString		strFtpPwd;
}g_FtpConfig;

CStringA UCS2ToANSI(const CStringW &strUCS2)
{
	int size = WideCharToMultiByte(CP_ACP, 0, strUCS2, -1, NULL, 0, NULL, NULL);

	if (size == 0)
	{
		return "";
	}

	CHAR *szANSI = new CHAR[size];

	if (WideCharToMultiByte(CP_ACP, 0, strUCS2, -1, szANSI, size, NULL, NULL) == 0)
	{
		delete[] szANSI;
		return "";
	}

	CStringA ret = szANSI;
	delete[] szANSI;

	return ret;
}

CStringW ANSIToUCS2(const CStringA &strANSI )
{
	int size = MultiByteToWideChar(CP_ACP, 0, strANSI, -1, NULL, 0);

	if (size == 0)
	{
		return L"";
	}

	WCHAR *szUCS2 = new WCHAR[size];

	if (MultiByteToWideChar(CP_ACP, 0, strANSI, -1, szUCS2, size) == 0)
	{
		delete[] szUCS2;
		return L"";
	}

	CStringW ret = szUCS2;
	delete[] szUCS2;

	return ret;
}


//加载本地配置
BOOL LoadLocalConfig()
{
	const char* pszDecryptBuffer = getDecryptFileBuffer( _T("serverlist.data"), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return FALSE;
	}
	XMLData xmldata;
	if( !xmldata.loadBuffer( pszDecryptBuffer, strlen( pszDecryptBuffer )) )
	{
		delete[] pszDecryptBuffer;
		return FALSE;
	}

	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.getChild("DumpFtp");
	if( pChildNode.isNull() )
	{
		delete[] pszDecryptBuffer;
		return FALSE;
	}
	g_FtpConfig.strFtpServer = ANSIToUCS2( pChildNode.attribToString("server") );
	g_FtpConfig.nFtpPort	 = pChildNode.attribToInt("port");
	g_FtpConfig.strFtpUser	 = ANSIToUCS2( pChildNode.attribToString("user") );
	g_FtpConfig.strFtpPwd	 = ANSIToUCS2( pChildNode.attribToString("pwd") );

	delete[] pszDecryptBuffer;
	return TRUE;
}

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

BOOL CreateFtpMultipleDirectory( CFtpConnection* pFtp,const CString& szPath )
{
	if( pFtp == NULL )
	{
		return FALSE;
	}

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
		bSuccess = pFtp->CreateDirectory( *vIter ) ? true : false;    
	}
	return bSuccess;
}

//向zip包中添加文件
BOOL ZipAddFile(zipFile zf, LPCTSTR lpszFileNameInZip, LPCTSTR lpszFilePath)
{
	DWORD dwFileAttr = GetFileAttributes(lpszFilePath);

	if (dwFileAttr == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}

	DWORD dwOpenAttr = (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY) != 0 ? FILE_FLAG_BACKUP_SEMANTICS : 0;
	HANDLE hFile = CreateFile(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, dwOpenAttr, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	FILETIME ftUTC, ftLocal;
	GetFileTime(hFile, NULL, NULL, &ftUTC);
	FileTimeToLocalFileTime(&ftUTC, &ftLocal);
	WORD wDate, wTime;
	FileTimeToDosDateTime(&ftLocal, &wDate, &wTime);
	zip_fileinfo FileInfo;
	ZeroMemory(&FileInfo, sizeof(FileInfo));
	FileInfo.dosDate = ((((DWORD)wDate) << 16) | (DWORD)wTime);
	FileInfo.external_fa |= dwFileAttr;

	CString strFileNameInZip = lpszFileNameInZip;

	CStringA strFileNameInZipA = UCS2ToANSI(lpszFileNameInZip);
	if (zipOpenNewFileInZip(zf, strFileNameInZipA.GetBuffer(), &FileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, 9) != ZIP_OK)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	const DWORD BUFFER_SIZE = 4096;
	BYTE byBuffer[BUFFER_SIZE];

	LARGE_INTEGER li = {};

	if (!GetFileSizeEx(hFile, &li))
	{
		CloseHandle(hFile);
		zipCloseFileInZip(zf);
		return FALSE;
	}

	while (li.QuadPart > 0)
	{
		DWORD dwSizeToRead = li.QuadPart > (LONGLONG)BUFFER_SIZE ? BUFFER_SIZE : (DWORD)li.LowPart;
		DWORD dwRead = 0;

		if (!ReadFile(hFile, byBuffer, dwSizeToRead, &dwRead, NULL))
		{
			CloseHandle(hFile);
			zipCloseFileInZip(zf);
			return FALSE;
		}

		if (zipWriteInFileInZip(zf, byBuffer, dwRead) < 0)
		{
			CloseHandle(hFile);
			zipCloseFileInZip(zf);
			return FALSE;
		}

		li.QuadPart -= (LONGLONG)dwRead;
	}
	CloseHandle(hFile);
	zipCloseFileInZip(zf);
	return TRUE;
}

CString GetUserName()
{
	TCHAR user[64] = {'\0'};
	TCHAR szPath[MAX_PATH] = { '\0' };
	GetCurrentDirectory( MAX_PATH, szPath );
	swprintf_s( szPath, MAX_PATH, _T("%s\\UserInfoLoginRecord.ini"), szPath );
	GetPrivateProfileString( _T("UserName"), _T("UserName"), _T(""), user, 64, szPath );
	return user;
}

CString GetFileVersion()
{
	char* pszDecryptBuffer	= getDecryptFileBuffer( _T("version.data"), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return _T("");
	}

	char szVersion[512];
	CIniFile iniNewVer;
	iniNewVer.Prepare( pszDecryptBuffer );
	iniNewVer.GetString( "version", "ver", "0", szVersion, 512 );
	iniNewVer.Close();
	delete[] pszDecryptBuffer;

	CString ret = ANSIToUCS2(szVersion);
	return ret;
}

BOOL sendDumpFile()
{
	//没有配置 ftp
	if( g_FtpConfig.strFtpServer == _T("") )
	{
		return FALSE;
	}
	//打包dump
	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("log\\*.dmp"));

	CString zipPath = _T("");
	zipFile zf = NULL;
	while( bWorking )
	{
		bWorking = finder.FindNextFile();
		if( zipPath == _T("") )
		{	
			zipPath = _T("log/") + finder.GetFileTitle() + _T(".zip");
		}
		if( zf == NULL )
		{
			CStringA strDestFile = UCS2ToANSI(zipPath);
			zf = zipOpen( strDestFile.GetBuffer(), 0 );
		}
		if(zf != NULL )
		{
			ZipAddFile( zf,finder.GetFileName(),finder.GetFilePath() );
		}
		//把文件移到新的文件夹中
		CreateMultipleDirectory( _T("log/dumps") );
		//把该目录的原先这个文件删除掉
		::DeleteFile( _T("log/dumps/") + finder.GetFileName());
		MoveFile(finder.GetFilePath(), _T("log/dumps/") + finder.GetFileName());
	}
	finder.Close();

	//log 中没有.dmp文件
	if( zf == NULL )
	{
		return FALSE;
	}
	ZipAddFile( zf, _T("all.log"), _T("log/all.log") );
	ZipAddFile( zf, _T("path.log"), _T("log/path.log") );

	zipClose(zf, (const char *)NULL);

	//上传到ftp
	CFile f;
	if( !f.Open( zipPath, CFile::modeRead ) )
	{
		return FALSE;
	}
	//ftp 的上传 路径是  /dumps/ver/username CreateFtpMultipleDirectory
	//zip包超过10M 不上传
	if( f.GetLength() > 10*1024*1024 )
	{
		f.Close();
		::DeleteFile(zipPath);
		return FALSE;
	}

	CInternetSession session(_T("BugReport"));
	CFtpConnection* pFtp = session.GetFtpConnection(g_FtpConfig.strFtpServer,
		g_FtpConfig.strFtpUser,
		g_FtpConfig.strFtpPwd,
		g_FtpConfig.nFtpPort);
	if( pFtp == NULL )
	{
		f.Close();
		::DeleteFile(zipPath);
		return FALSE;
	}
	pFtp->SetCurrentDirectory( _T("\\") );

	CString ftpPath = _T("\\dumps\\");

	CString ver = GetFileVersion();
	if( ver.GetLength() == 0 )
	{
		ver = _T("nonVer");
	}
	CString userName = GetUserName();
	if( userName.GetLength() == 0 )
	{
		userName = _T("nonUser");
	}
	ftpPath = ftpPath + ver + _T("\\") + userName + "\\";
	CreateFtpMultipleDirectory(pFtp,ftpPath);
	if(!pFtp->SetCurrentDirectory(ftpPath) )
	{
		f.Close();
		pFtp->Close();
		delete pFtp;
		session.Close();
		::DeleteFile(zipPath);
		return FALSE;
	}

	CInternetFile* pFile = pFtp->OpenFile( f.GetFileName(),GENERIC_WRITE );
	if( pFile == NULL )
	{
		f.Close();
		pFtp->Close();
		delete pFtp;
		session.Close();
		::DeleteFile(zipPath);
		return FALSE;
	}

	DWORD fLength = (DWORD)f.GetLength();
	char buf[4096] = {'\0'};
	while (fLength > 0)
	{
		DWORD dwSizeToRead = fLength > 4096 ? 4096 : fLength;
		DWORD dwRead = 0;
		dwRead = f.Read(buf,dwSizeToRead);
		if( dwRead == 0 )
		{
			break;
		}
		pFile->Write(buf,dwRead);
		fLength -= dwRead;
	}

	f.Close();
	pFile->Close();
	pFtp->Close();
	delete pFtp;
	session.Close();

	::DeleteFile(zipPath);
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if( strcmp(lpCmdLine,"launcher") != 0 )
	{
		return 0;
	}
	if( LoadLocalConfig() )
	{
		try
		{
			sendDumpFile();
		}catch (CException* e){

		}
	}

	return 0;
}
