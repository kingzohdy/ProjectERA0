//-----------------------------------------------------------------------------
// File: Util.cpp
// Desc: Game System Util 2.0
// Auth: wizard
// Date: 2003/11/13
//
// Copyright (c) 1985-2003 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "util.h"

#ifdef _WIN32		//只对win平台有效

//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
CUtil::CUtil():m_dwIDHolder(1),m_nRandHolder(0),m_nRandCallCount(0)
{
	InitializeCriticalSection(&m_Lock);
}

CUtil::~CUtil()
{
	DeleteCriticalSection(&m_Lock);
}

//-----------------------------------------------------------------------------
// 得到下一个空闲的ID
//-----------------------------------------------------------------------------
DWORD CUtil::GetFreeID()
{
	if( m_dwIDHolder >= 0x7fffffff )	// 此种情况应该很难发生
		m_dwIDHolder = 1;
	return m_dwIDHolder++;
}

//-----------------------------------------------------------------------------
// 用给定的随机数种子初始化随机数发生器
//-----------------------------------------------------------------------------
VOID CUtil::Randomize(DWORD dwSeed)
{
	m_nRandHolder = (INT)dwSeed;
	m_nRandCallCount = 0;
}

//-----------------------------------------------------------------------------
// 得到随机数
//-----------------------------------------------------------------------------
INT CUtil::Rand()
{
	EnterCriticalSection(&m_Lock);
	m_nRandCallCount++;
	INT nR = (((m_nRandHolder = m_nRandHolder * 214013L + 2531011L) >> 16) & 0x7fff);
	LeaveCriticalSection(&m_Lock);
	return nR;
}

//-----------------------------------------------------------------------------
// IntersectionRect 在Win98操作系统下有BUG, 这里我们重写一个
//-----------------------------------------------------------------------------
BOOL CUtil::RectIntersection(RECT& rcOut, RECT rc1, RECT rc2)
{
	if (rc1.left > rc2.right || rc2.left > rc1.right 
		|| rc1.top > rc2.bottom || rc2.top > rc1.bottom)
		return FALSE;	// 两矩形不相交

	// 下面计算相交矩形
	rcOut.left = rc1.left > rc2.left ? rc1.left : rc2.left;
	rcOut.top = rc1.top > rc2.top ? rc1.top : rc2.top;
	rcOut.right = rc1.right < rc2.right ? rc1.right : rc2.right;
	rcOut.bottom = rc1.bottom < rc2.bottom ? rc1.bottom : rc2.bottom;
	return TRUE;
	
}

WORD CUtil::m_wUnicodeString[MAX_PATH];
//-----------------------------------------------------------------------------
// 将单字节码转成类似UNICODE的双字节码
//-----------------------------------------------------------------------------
WORD* CUtil::CharStringToUnicode(CHAR* pString)
{
	INT nLen = lstrlenA(pString);
	if( nLen >= MAX_PATH-2 )
		return m_wUnicodeString;
	int n=0;
	for( ; n<nLen; n++ )
		m_wUnicodeString[n] = (WORD)pString[n];

	m_wUnicodeString[n++] = 0;
	m_wUnicodeString[n++] = 0;
	return m_wUnicodeString;
}

//-----------------------------------------------------------------------------
// 去掉std::string首尾的空格
//-----------------------------------------------------------------------------
VOID CUtil::CutSpaceOfStringHeadAndTail(std::string& string)
{
	// 必须检查:如果字符串中只有空格,不能进行此操作
	if( string.find_first_not_of(' ') != -1 )
	{
		string.assign(string, string.find_first_not_of(' '),
			string.find_last_not_of(' ')-string.find_first_not_of(' ')+1);
	}
}

//-----------------------------------------------------------------------------
// 将长串字符转为token;
//-----------------------------------------------------------------------------
VOID CUtil::StringToToken(std::vector<std::string>& token, LPCSTR szString)
{
	std::string strTemp(szString);
	std::string str;
	token.clear();
	strTemp.append(" ");

	while(1)
	{
		if( strTemp.find_first_of(' ') != -1 )
		{
			str.assign(strTemp, 0, strTemp.find_first_of(' '));
			strTemp.erase(0, strTemp.find_first_of(' ')+1);

			// 去掉首尾的空格
			CutSpaceOfStringHeadAndTail(str);
			if( str.size() > 0 )
				token.push_back(str);
		}
		else
		{
			if( strTemp.size() > 0  )
			{
				// 去掉头尾的空格
				CutSpaceOfStringHeadAndTail(strTemp);
				token.push_back(strTemp);
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 检测注册表中指定键值是否存在
//-----------------------------------------------------------------------------
BOOL CUtil::CheckReg(HKEY key, LPCTSTR subKey)
{
	HKEY hKey;
	long ret0 = RegOpenKeyEx(key, subKey, 0, KEY_READ, &hKey);
	if ( ret0 == ERROR_SUCCESS )
	{
		RegCloseKey(hKey);
		return true;
	}

	RegCloseKey(hKey);
	return false;
}

//-----------------------------------------------------------------------------
// 将指定键值写入注册表中指定路径,如果指定路径没有,则创建并写入
//-----------------------------------------------------------------------------
BOOL CUtil::WriteReg(char* KEY_ROOT/*注册表中的路径*/, char* KEY_NAME, LPCSTR sz)
{
	if ( KEY_ROOT == NULL ) return false;
	if ( KEY_NAME == NULL ) return false;

	HKEY hKey;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;

	long ret0;

	char buf[256]={0};
	if ( !CheckReg(HKEY_LOCAL_MACHINE, KEY_ROOT) )
	{
		ret0 = RegCreateKeyEx(HKEY_LOCAL_MACHINE, KEY_ROOT, 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
		if ( ret0 != ERROR_SUCCESS )
			return false;

		RegCloseKey(hKey);
	}
	// -- 写入
	RegOpenKeyEx( HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_WRITE, &hKey );
	ret0 = RegSetValueEx(hKey, KEY_NAME, NULL, REG_SZ, (const BYTE*)sz, (DWORD)(strlen(sz)+sizeof(sz[0])));
	if ( ret0 != ERROR_SUCCESS )
		return false;

	RegCloseKey(hKey);
	return true;
}

//-----------------------------------------------------------------------------
// 读取注册表中指定路径的键值
//-----------------------------------------------------------------------------
BOOL CUtil::ReadReg(char *KEY_ROOT, char* KEY_NAME, DWORD& key_value)
{
	HKEY hKey;
	long ret0 = RegOpenKeyEx( HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_QUERY_VALUE, &hKey );
	if( ret0 != ERROR_SUCCESS )
		return FALSE;

	DWORD dwBufLen=32;
	RegQueryValueEx( hKey, KEY_NAME, NULL, NULL, (LPBYTE)&key_value, &dwBufLen);
	RegCloseKey(hKey);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 读注册表
//-----------------------------------------------------------------------------
BOOL CUtil::ReadReg(char* KEY_ROOT, char* KEY_NAME, char outstr[])
{
	HKEY hKey;
	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_ROOT, 0, KEY_READ, &hKey);
	if ( ret0 != ERROR_SUCCESS )
	{
		outstr[0] = 0;
		return FALSE;
	}
	DWORD dwLen = MAX_PATH, dwType = REG_SZ;
	// if char* (LPBYTE)char*
	// if char [] (LPBYTE)&char
	RegQueryValueEx(hKey, KEY_NAME, NULL, &dwType, (LPBYTE)outstr, &dwLen);

	RegCloseKey(hKey);
	return TRUE;
}

#endif


