#include "stdafx.h"
#include "CreatePatch.h"
#include "OgrePackageFile.h"
#include "CIniFile.h"

CCreatePatch::CCreatePatch(void)
{
}

CCreatePatch::~CCreatePatch(void)
{
}

// 创建补丁包
bool CCreatePatch::makePatch( string strVersionPath, string strOldVersionPath, string strPackageName )
{
	DeleteFile( strPackageName.c_str() );

	if( searchChangeFiles( strVersionPath, strOldVersionPath, m_mapChangeFiles ) )
	{
		printf( "开始打包文件\n" );
		FilePackage filePackage;
		filePackage.open( strPackageName.c_str(), false );
		for( map<string,string>::iterator iter = m_mapChangeFiles.begin(); iter != m_mapChangeFiles.end(); ++iter )
		{
			char szInfo[512];
			sprintf( szInfo, "打包文件: %s\n", iter->first.c_str() );
			printf( szInfo );
			filePackage.appendOrReplaceFile( iter->first.c_str(), true, strVersionPath.c_str() );
		}
		filePackage.close();
		
		printf( "打包结束\n" );
		return true;
	}

	return false;
}

// 创建自解压包
bool CCreatePatch::makeSelfExtractor( const char* szExtractorPath, const char* szPatchName )
{
	m_SelfExtractor.Create( CString( szExtractorPath ), CString( szPatchName ),CCreatePatch::AddCallBack, NULL );

	return true;
}

// 构建版本信息
const char* CCreatePatch::createVer( const char* szVer, const char* szPath )
{
	string strVer = string( szPath ) + string("\\version.data");
	CIniFile iniFile;
	iniFile.Create( strVer.c_str() );
	iniFile.WriteString( "version", "ver", szVer );
	iniFile.Save();
	iniFile.Close();

	return strVer.c_str();
}

void CCreatePatch::AddCallBack(void* CallbackData, void *userData)
{
	CSEFileInfo* pData = static_cast<CSEFileInfo*>(CallbackData);
	//CSelfExtractorDlg* pDlg = static_cast<CSelfExtractorDlg*>(userData);

	//pDlg->m_strCurrFile = "Adding " + pData->GetPathname();
	//pDlg->m_Progress.StepIt();
	//pDlg->UpdateData(FALSE);
}

bool CCreatePatch::searchChangeFiles( string sNewDirName, string sOldDirName, map<string,string>& mapChangeFiles )
{
	HANDLE hFind;
	WIN32_FIND_DATA ffd;

	string szSeach = sNewDirName + "/*.*";

	ZeroMemory( &ffd, sizeof(WIN32_FIND_DATA) );
	hFind = FindFirstFile( szSeach.c_str(), &ffd );

	if( hFind == INVALID_HANDLE_VALUE )
		return false;

	while( FindNextFile( hFind, &ffd ) )
	{
		if(_stricmp( ffd.cFileName, "." ) == 0 ||
			_stricmp( ffd.cFileName, ".." ) == 0 )
			continue;

		string sNewFileName = sNewDirName + "\\" + ffd.cFileName;
		string sOldFileName = sOldDirName + "\\" + ffd.cFileName;
		if( ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
			searchChangeFiles( sNewFileName, sOldFileName, mapChangeFiles );
		else
		{
			struct _stat stat_new;
			if( _stat( sNewFileName.c_str(), &stat_new ) )
			{
				//cerr << "无法获取" << sNewFileName << "文件信息\n";
				exit(1);
			}

			struct _stat stat_old;
			if( _stat( sOldFileName.c_str(), &stat_old ) )
			{
				mapChangeFiles[sNewFileName] = "";
			}else if(  stat_new.st_size != stat_old.st_size ) 
			{
				m_mapChangeFiles[sNewFileName] = sOldFileName;
			}else
			{
				// MD5比较或者下面的内存比较，相对较慢
				char szOldMd5[33] = {0},szNewMd5[33] = {0};
				CString oldmd5 = file_checksum(sOldFileName.c_str(),0);
				CString newmd5 = file_checksum(sNewFileName.c_str(),0);				
				if( oldmd5.CompareNoCase(newmd5) != 0 )
				{
					mapChangeFiles[sNewFileName] = sOldFileName;
				}
			}
		}
	}
	FindClose( hFind );
	return true;
}

size_t CCreatePatch::diff( FILE* pOldFile, FILE* pNewFile, string& strDiff )
{
	for( int32 nSame = 0, nNew = 0; nNew != EOF; ) 
	{
		int32 nSkip = nSame;
		int32 nOld = 0;
		while( nNew != EOF && nOld != EOF ) 
		{
			nOld = getc( pOldFile );
			nNew = getc( pNewFile );

			if( nOld != nNew )
				break;
			nSkip++; 
		}

		// 如果目标文件先结束或同时结束可以不管剩下的数据
		// 如果源文件先结束，剩下的数据也算diff
		if ( nNew == EOF )
			break;

		int32 nOffset = (int32)strDiff.size() + sizeof(int32);
		int32 nSize   = 0;
		strDiff.append( (char*)&nSkip, sizeof(int32) );
		strDiff.append( (char*)&nSize, sizeof(int32) );
		strDiff.append( 1, (char)nNew );

		char szBuf[8];
		for( nSize = 1, nSame = 0; nSame < 8; nSize++ ) 
		{
			nOld = getc( pOldFile );
			nNew = getc( pNewFile );
			if( nOld == nNew )
			{
				szBuf[nSame] = nNew;
				++nSame;
			}
			else 
			{
				if( nSame > 0 ) 
					strDiff.append( szBuf, nSame );
				nSame = 0;
				strDiff.append( 1, (char)nNew );
			}
		}

		nSize -= nSame;
		*( (int32*)&strDiff[nOffset] ) = nSize;
	}

	return strDiff.size();
}

const char *CCreatePatch::file_checksum(const char *szFileName, int offset)
{
	static char checksum[128] = { '\0' };

	FILE *fp = fopen( szFileName ,"rb" );
	if ( fp == NULL )
	{
		return "";
	}

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp) - offset;
	fseek( fp, offset, SEEK_SET );

	CMd5 md5;
	md5.Begin();

	const int BUF_SIZE	= 4096;
	unsigned char buf[BUF_SIZE] = { '\0' };
	while ( file_size > BUF_SIZE )
	{
		fread( buf, BUF_SIZE, 1, fp );
		file_size -= BUF_SIZE;
		md5.Append(buf, BUF_SIZE);
	}

	fread(buf, file_size, 1, fp);
	md5.Append(buf, file_size);
	md5.GetMD5Base16Str( checksum );
	fclose(fp);
	return checksum;
}
