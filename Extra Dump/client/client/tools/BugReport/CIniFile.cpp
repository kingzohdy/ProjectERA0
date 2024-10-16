//---------------------------------------------------------------------------
// FileName     : CommonIni.h
// Author       : LaiLai
// Comment      : 与语言无关的读写Ini文件的实现
//              : 顺序读写可提高速度.
//              : 对于同一个实例，读写不可穿插进行
//---------------------------------------------------------------------------
// NOTE         : Use std libc function str*** instead of ms ext function 
//                lstr*** for platform compatibility reason.
//---------------------------------------------------------------------------
// #include "stdafx.h"
#include "CIniFile.h"
#include <string>
#include <map>
using namespace std;

//---------------------------------------------------------------------------

#ifdef _WIN32
#pragma warning(disable:4127)
#endif


bool			IsBlank(char    cChar);
void			NextLine(char*& lpBuffer);
string			BuildSection(char*& lpBuffer);
bool			BuildKey(char*& lpBuffer, map<string, string>& Section);


struct INIFILE_HANDLE
{
	string	m_sFileName;
	map< string, map<string, string> > m_Struct;
};

INIFILE_HANDLE&  GetIniFileHandle( void* hIniFile )
{
	return *( (INIFILE_HANDLE*)hIniFile );
}

CIniFile::CIniFile() : m_hFileHandle( new INIFILE_HANDLE )
{	
}

CIniFile::~CIniFile()
{
	Close();
	delete ( (INIFILE_HANDLE*)m_hFileHandle );
}

// 创建一个新的 .Ini文件，为写(保存)做准备
bool CIniFile::Create(const char* szFileName)
{
	// 如果链表不为空，清空链表.
	Close();

	GetIniFileHandle( m_hFileHandle ).m_sFileName = szFileName;
	return true;
}

// 打开文件，扫描一遍，为以后的读操作做好准备(Get...)
bool CIniFile::Open(const char* szFileName)
{
	FILE*		File;
	char*	    pFileBuff;
	int			nSize;
	bool        IsSuccess;

	if( !szFileName || !szFileName[0] )
		return false;

	// comp file name  strcmp
	if( GetIniFileHandle( m_hFileHandle ).m_sFileName == szFileName )
		return true;

	// 先释放
	Close();
	// open Ini file  , 在内部形成全路径
	File = fopen( szFileName,"rb" );
	if ( File == NULL )
		return false;

	fseek( File, 0, SEEK_END );
	nSize = ftell( File );
	fseek( File, 0, SEEK_SET );

	// get file size
	if ( nSize == 0 )
		return false;

	// alloc memory
	pFileBuff = new char[nSize+1];

	// read data from file
	if( fread( pFileBuff, nSize, 1, File ) != 1 )
	{
		delete pFileBuff;
		return false;
	}

	pFileBuff[nSize] = 0;

	// close the file
	fclose(File);

	// save file name
	GetIniFileHandle( m_hFileHandle ).m_sFileName = szFileName;

	// 建立内部链表
	IsSuccess = Prepare(pFileBuff);
	if( !IsSuccess )
		Close();

	delete pFileBuff;

	return IsSuccess;
}

/*
*	建立内部链表
*/
bool CIniFile::Prepare(char* lpBuffer)
{
	char*   lpCursor = lpBuffer;
	char    cCursor = *lpCursor;

	string sSection;
	while( 0 != cCursor)
	{
		if( IsBlank(cCursor))
		{
			lpBuffer++;
			lpCursor = lpBuffer;
			cCursor = *lpCursor;
			continue;
		}
		if( '/' == cCursor)
		{
			NextLine(lpBuffer);
			lpCursor    = lpBuffer;
			cCursor     = *lpCursor;
			continue;
		}
		if( '[' == cCursor )
		{
			lpBuffer++;
			sSection = BuildSection(lpBuffer);
			if( sSection.empty() )
				return false;
			GetIniFileHandle( m_hFileHandle ).m_Struct[sSection] = map<string,string>();
			lpCursor    = ++lpBuffer;
			cCursor     = *lpCursor;
			continue;
		}

		//--------read key and value-----------------
		if( !BuildKey(lpBuffer, GetIniFileHandle( m_hFileHandle ).m_Struct[sSection]))
			return false;
		lpCursor    = lpBuffer;
		cCursor     = *lpCursor;
		//-------------------------------------------

	}

	return true;  
}

// 按顺序将链表中的内容保存到磁盘文件
void CIniFile::Save()
{	
	FILE* pFile = fopen( GetIniFileHandle( m_hFileHandle ).m_sFileName.c_str(), "wb" );
	if( !pFile )
		return;

	for( map< string, map<string, string> >::iterator itSection = GetIniFileHandle( m_hFileHandle ).m_Struct.begin();
		itSection != GetIniFileHandle( m_hFileHandle ).m_Struct.end(); itSection++ )
	{
		string sSection = "[" + itSection->first + "]\r\n";
		fwrite((void*)sSection.c_str(), sSection.size(), 1, pFile );
		for( map<string,string>::iterator itKey = itSection->second.begin();
			itKey != itSection->second.end(); itKey++ )
		{
			string sKey = itKey->first + " = " + itKey->second + "\r\n";
			fwrite((void*)sKey.c_str(), sKey.size(), 1, pFile );
		}
	}
	fclose( pFile );
}

// 关闭文件,释放内存
void CIniFile::Close()
{
	GetIniFileHandle( m_hFileHandle ).m_sFileName.clear();
	GetIniFileHandle( m_hFileHandle ).m_Struct.clear();
}

// 遍历链表，查找匹配的值, 注意结果Buf 不能大于Size
bool CIniFile::GetKeyValue(const char*	lpSection,const char*	lpKeyName,	char*	lpRString, unsigned int	nSize)
{
	// 能执行到这里，要求ini文件已成功的打开了, 链表也已成功的建立了.
	//ArkAst( NULL != m_lpSectionCur);
	if( !lpSection || !lpKeyName || !lpRString )
		return false;

	map< string, map<string, string> >::iterator itSection = GetIniFileHandle( m_hFileHandle ).m_Struct.find( lpSection );
	if( itSection == GetIniFileHandle( m_hFileHandle ).m_Struct.end() )
		return false;

	map<string,string>::iterator itKey = itSection->second.find( lpKeyName );
	if( itKey == itSection->second.end() )
		return false;

	if( itKey->second.size() > nSize )
		return false;
	strcpy(lpRString, itKey->second.c_str() );

	return true;
}

//	读取一个字符串
void CIniFile::GetString(const char* lpSection,	const char* lpKeyName,	const char* lpDefault,char* lpRString, unsigned int nSize	)
{
	if (!GetKeyValue(lpSection,lpKeyName,lpRString,nSize))
	{
		strcpy(lpRString,lpDefault);
	}
}

//	读取一个整数
void CIniFile::GetInteger(const char* lpSection, const char* lpKeyName,	int   nDefault,	int   &Value)
{
	char szValue[16];
	if (GetKeyValue(lpSection,lpKeyName,szValue,sizeof(szValue)))
	{
		Value = atoi(szValue);
	}
	else
	{
		Value = nDefault;
	}
}


//	读取一个整数
int	CIniFile::GetInteger(const char*	lpSection,const char*	lpKeyName, int nDefault)
{
	char szValue[16];
	int  retval;
	if (GetKeyValue(lpSection,lpKeyName,szValue,sizeof(szValue)))
	{
		retval = atoi(szValue);
	}
	else
	{
		retval = nDefault;
	}
	return retval;
};

//	读取一个整数
char CIniFile::GetChar(const char*	lpSection,const char*	lpKeyName, char chDefault)
{
	char szValue[16];
	char  retval;
	if (GetKeyValue(lpSection,lpKeyName,szValue,sizeof(szValue)))
	{
		retval = szValue[0];
	}
	else
	{
		retval = chDefault;
	}

	return retval;
};



// Comment      :   忽略 空格(0x20)，tab(0x09), 回车(0x0d)，换行符(0x0a)
//              :   可以做一些优化，如传入指针，一次做多个处理，以减少函数调用
bool IsBlank(char    cChar)
{
	if( 0x20 == cChar )
		return true;
	if( 0x09 == cChar )
		return true;
	if( 0x0d == cChar )
		return true;
	if( 0x0a == cChar )
		return true;
	return false;
}

//---------------------------------------------------------------------------
// Function     : 把Buffer指针移到下一行, 比如处理注释时需要.
// Comment      : 传入的Buffer指针是一个引用，内部修改后可影响外部.
//              : 当碰到Buffer结尾时（0），返回的lpBuffer 指向 0
//              : 当碰到行结束时
//---------------------------------------------------------------------------
void NextLine(char*& lpBuffer)
{
	char    cChar = *lpBuffer;

	while( 0 != cChar )
	{
		if( 0x0d == cChar )
		{
			lpBuffer += 2;
			return;
		}
		cChar = *++lpBuffer;
	}
}

//---------------------------------------------------------------------------
// Function     :  读取Section的名字，传给AddtoSectionList
//---------------------------------------------------------------------------
string BuildSection(char*& lpBuffer)
{
	char*   lpCursor = lpBuffer;
	char    cChar = *lpBuffer;

	while( 1 )
	{
		if( 0 == cChar )
			return "";
		if( ']' == cChar)
		{
			*lpBuffer = 0;
			string sSection = lpCursor;
			*lpBuffer = ']';
			return sSection;
		}
		lpBuffer++;
		cChar = *lpBuffer;
	}
}

// 形成KeyName, 及KeyValue
bool BuildKey(char*& lpBuffer, map<string, string>& Section )
{
	char    cChar = *lpBuffer;
	char*   lpCursor = lpBuffer;
	string  sKey;
	string  sValue;
	size_t     nLen;

	// Read Key
	while( 1 )
	{
		if( '=' == cChar)
		{
			*lpBuffer = 0;
			nLen = strlen( lpCursor );
			string sTemp = lpCursor;
			//modified by weck to eat space .
			for( size_t i = sTemp.size() - 1; i > 0 && IsBlank( sTemp[i] ); i-- )
				sTemp[i] = 0;

			sKey = sTemp.c_str();

			*lpBuffer = '=';
			//modified by weck to eat space .
			while ( *lpBuffer == '\t' || *lpBuffer == ' ' || *lpBuffer == '=')
			{
				lpBuffer++;
			}
			lpCursor = lpBuffer;
			cChar = *lpBuffer;
			break;
		}
		if( 0 == cChar )
			return false;
		// modified by weck
		// just linebreak or return indicate thtat the format is incorrect.
		// some people like to add space between key and '=' and value.
		if ( cChar == 0x0d || cChar ==0x0a )
			return false;
		/*
		if( IsBlank(cChar))
		return false;
		*/
		lpBuffer++;
		cChar = *lpBuffer;
	}

	// Read Value
	while( 1 )
	{
		if( ( 0 == cChar ) || (0x0d == cChar) || (0x0a == cChar))
		{
			// 回退到非空格
			while(1)
			{
				lpBuffer--;
				if( !IsBlank(*lpBuffer))
					break;
			}
			cChar = *++lpBuffer;

			*lpBuffer = 0;
			sValue = lpCursor;
			*lpBuffer = cChar;
			break;
		}
		lpBuffer++;
		cChar = *lpBuffer;
	}

	Section[sKey] = sValue;

	return true;
}

//---------------------------------------------------------------------------
//  Function    :   Write string to SectionList
//	Comment     :	把一个字符串写到由 Section, Key 标记的List节点中去.
//              :   先查找Section节点是否已存在,若存在，在次Section节点
//              ：  的KeyList中插入由lpKey标记的节点(这一步不需查找)
//---------------------------------------------------------------------------
void CIniFile::WriteString(
						   const char* lpSection,		// pointer to section name
						   const char* lpKey,		    // pointer to key name
						   const char* lpValue		    // pointer to string to add
						   )
{
	if( !lpSection || !lpSection[0] )
		return;
	if( !lpKey || !lpKey[0] )
		GetIniFileHandle( m_hFileHandle ).m_Struct[lpSection] = map<string,string>();
	else
		GetIniFileHandle( m_hFileHandle ).m_Struct[lpSection][lpKey] = !lpValue || !lpValue[0] ? "" : lpValue;
}

//---------------------------------------------------------------------------
//  Function:   Write integer to ini file
//	Comment:	写一个整数到INI文件
//---------------------------------------------------------------------------
void CIniFile::WriteInteger(
							const char* lpSection,		// pointer to section name
							const char* lpKeyName,		// pointer to key name
							int 	nValue			// Integer to write
							)
{
	char    lpValue[32];

	sprintf_s(lpValue, "%d", nValue);
	//itoa( nValue, lpValue, 10);
	WriteString( lpSection, lpKeyName, lpValue);
}
