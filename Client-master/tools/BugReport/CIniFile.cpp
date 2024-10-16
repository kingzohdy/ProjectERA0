//---------------------------------------------------------------------------
// FileName     : CommonIni.h
// Author       : LaiLai
// Comment      : �������޹صĶ�дIni�ļ���ʵ��
//              : ˳���д������ٶ�.
//              : ����ͬһ��ʵ������д���ɴ������
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

// ����һ���µ� .Ini�ļ���Ϊд(����)��׼��
bool CIniFile::Create(const char* szFileName)
{
	// �������Ϊ�գ��������.
	Close();

	GetIniFileHandle( m_hFileHandle ).m_sFileName = szFileName;
	return true;
}

// ���ļ���ɨ��һ�飬Ϊ�Ժ�Ķ���������׼��(Get...)
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

	// ���ͷ�
	Close();
	// open Ini file  , ���ڲ��γ�ȫ·��
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

	// �����ڲ�����
	IsSuccess = Prepare(pFileBuff);
	if( !IsSuccess )
		Close();

	delete pFileBuff;

	return IsSuccess;
}

/*
*	�����ڲ�����
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

// ��˳�������е����ݱ��浽�����ļ�
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

// �ر��ļ�,�ͷ��ڴ�
void CIniFile::Close()
{
	GetIniFileHandle( m_hFileHandle ).m_sFileName.clear();
	GetIniFileHandle( m_hFileHandle ).m_Struct.clear();
}

// ������������ƥ���ֵ, ע����Buf ���ܴ���Size
bool CIniFile::GetKeyValue(const char*	lpSection,const char*	lpKeyName,	char*	lpRString, unsigned int	nSize)
{
	// ��ִ�е����Ҫ��ini�ļ��ѳɹ��Ĵ���, ����Ҳ�ѳɹ��Ľ�����.
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

//	��ȡһ���ַ���
void CIniFile::GetString(const char* lpSection,	const char* lpKeyName,	const char* lpDefault,char* lpRString, unsigned int nSize	)
{
	if (!GetKeyValue(lpSection,lpKeyName,lpRString,nSize))
	{
		strcpy(lpRString,lpDefault);
	}
}

//	��ȡһ������
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


//	��ȡһ������
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

//	��ȡһ������
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



// Comment      :   ���� �ո�(0x20)��tab(0x09), �س�(0x0d)�����з�(0x0a)
//              :   ������һЩ�Ż����紫��ָ�룬һ������������Լ��ٺ�������
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
// Function     : ��Bufferָ���Ƶ���һ��, ���紦��ע��ʱ��Ҫ.
// Comment      : �����Bufferָ����һ�����ã��ڲ��޸ĺ��Ӱ���ⲿ.
//              : ������Buffer��βʱ��0�������ص�lpBuffer ָ�� 0
//              : �������н���ʱ
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
// Function     :  ��ȡSection�����֣�����AddtoSectionList
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

// �γ�KeyName, ��KeyValue
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
			// ���˵��ǿո�
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
//	Comment     :	��һ���ַ���д���� Section, Key ��ǵ�List�ڵ���ȥ.
//              :   �Ȳ���Section�ڵ��Ƿ��Ѵ���,�����ڣ��ڴ�Section�ڵ�
//              ��  ��KeyList�в�����lpKey��ǵĽڵ�(��һ���������)
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
//	Comment:	дһ��������INI�ļ�
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
