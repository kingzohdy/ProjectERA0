//-----------------------------------------------------------------------------
// File: sysinfo.cpp
// Auth: wizard
// Date: 2004/02/03
// Desc: 得到系统信息
//
// Copyright (c) 1985-2003 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "sysinfo.h"
#include <tchar.h>

#ifdef _WIN32		//只对win平台有效

//-----------------------------------------------------------------------------
// CPU特殊功能
//-----------------------------------------------------------------------------
enum
{
	CPU_MMX       = 0x00000001,
	CPU_MMX_EXT   = 0x00000002,
	CPU_3DNOW     = 0x00000004,
	CPU_3DNOW_EXT = 0x00000008,
	CPU_SSE       = 0x00000010,
	CPU_SSE2      = 0x00000020,
};


// 操作系统类型定义字符串表
LPCTSTR WUNKNOWNSTR		= _T("unknown Windows version");
LPCTSTR W95STR			= _T("Windows 95");
LPCTSTR W95SP1STR		= _T("Windows 95 SP1");
LPCTSTR W95OSR2STR		= _T("Windows 95 OSR2");
LPCTSTR W98STR			= _T("Windows 98");
LPCTSTR W98SP1STR		= _T("Windows 98 SP1");
LPCTSTR W98SESTR		= _T("Windows 98 SE");
LPCTSTR WMESTR			= _T("Windows ME");
LPCTSTR WNT351STR		= _T("Windows NT 3.51");
LPCTSTR WNT4STR			= _T("Windows NT 4");
LPCTSTR W2KSTR			= _T("Windows 2000");
LPCTSTR WXPSTR			= _T("Windows XP");
LPCTSTR W2003SERVERSTR	= _T("Windows 2003 Server");
LPCTSTR WCESTR			= _T("Windows CE");

// from winbase.h
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s             0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS      1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT           2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE           3
#endif

// 记录各种异常信息字符串
struct ExceptionNames
{
	DWORD	ExceptionCode;
	TCHAR *	ExceptionName;
};

ExceptionNames ExceptionMap[] =
{
	{0x40010005, _T("a Control-C")},
	{0x40010008, _T("a Control-Break")},
	{0x80000002, _T("a Datatype Misalignment")},
	{0x80000003, _T("a Breakpoint")},
	{0xc0000005, _T("an Access Violation")},
	{0xc0000006, _T("an In Page Error")},
	{0xc0000017, _T("a No Memory")},
	{0xc000001d, _T("an Illegal Instruction")},
	{0xc0000025, _T("a Noncontinuable Exception")},
	{0xc0000026, _T("an Invalid Disposition")},
	{0xc000008c, _T("a Array Bounds Exceeded")},
	{0xc000008d, _T("a Float Denormal Operand")},
	{0xc000008e, _T("a Float Divide by Zero")},
	{0xc000008f, _T("a Float Inexact Result")},
	{0xc0000090, _T("a Float Invalid Operation")},
	{0xc0000091, _T("a Float Overflow")},
	{0xc0000092, _T("a Float Stack Check")},
	{0xc0000093, _T("a Float Underflow")},
	{0xc0000094, _T("an Integer Divide by Zero")},
	{0xc0000095, _T("an Integer Overflow")},
	{0xc0000096, _T("a Privileged Instruction")},
	{0xc00000fD, _T("a Stack Overflow")},
	{0xc0000142, _T("a DLL Initialization Failed")},
	{0xe06d7363, _T("a Microsoft C++ Exception")},
};



//-----------------------------------------------------------------------------
// 版本数据非官方给出
//-----------------------------------------------------------------------------
BOOL CSysInfo::GetWinVer(LPTSTR pszVersion, LPTSTR pszBuild)
{
	if( !pszVersion || !pszBuild )
	{
		return FALSE;
	}

	lstrcpy(pszVersion, WUNKNOWNSTR);

	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if( !GetVersionEx(&osinfo) )
		return FALSE;

	DWORD dwPlatformId   = osinfo.dwPlatformId;
	DWORD dwMinorVersion = osinfo.dwMinorVersion;
	DWORD dwMajorVersion = osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF;	// Win 95 needs this

	wsprintf(pszBuild, _T("%u.%u.%u"), 
		dwMajorVersion, dwMinorVersion, dwBuildNumber);

	if( (dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4) )
	{
		if( (dwMinorVersion < 10) && (dwBuildNumber == 950) )
		{
			lstrcpy(pszVersion, W95STR);
		}
		else if( (dwMinorVersion < 10) && 
				((dwBuildNumber > 950) && (dwBuildNumber <= 1080)) )
		{
			lstrcpy(pszVersion, W95SP1STR);
		}
		else if( (dwMinorVersion < 10) && (dwBuildNumber > 1080) )
		{
			lstrcpy(pszVersion, W95OSR2STR);
		}
		else if( (dwMinorVersion == 10) && (dwBuildNumber == 1998) )
		{
			lstrcpy(pszVersion, W98STR);
		}
		else if( (dwMinorVersion == 10) && 
				((dwBuildNumber > 1998) && (dwBuildNumber < 2183)) )
		{
			lstrcpy(pszVersion, W98SP1STR);
		}
		else if( (dwMinorVersion == 10) && (dwBuildNumber >= 2183) )
		{
			lstrcpy(pszVersion, W98SESTR);
		}
		else if( dwMinorVersion == 90 )
		{
			lstrcpy(pszVersion, WMESTR);
		}
	}
	else if( dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		if( (dwMajorVersion == 3) && (dwMinorVersion == 51) )
		{
			lstrcpy(pszVersion, WNT351STR);
		}
		else if( (dwMajorVersion == 4) && (dwMinorVersion == 0) )
		{
			lstrcpy(pszVersion, WNT4STR);
		}
		else if( (dwMajorVersion == 5) && (dwMinorVersion == 0) )
		{
			lstrcpy(pszVersion, W2KSTR);
		}
		else if( (dwMajorVersion == 5) && (dwMinorVersion == 1) )
		{
			lstrcpy(pszVersion, WXPSTR);
		}
		else if( (dwMajorVersion == 5) && (dwMinorVersion == 2) )
		{
			lstrcpy(pszVersion, W2003SERVERSTR);
		}
	}
	else if( dwPlatformId == VER_PLATFORM_WIN32_CE )
	{
		lstrcpy(pszVersion, WCESTR);
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// 检查是否NT操作系统
//-----------------------------------------------------------------------------
BOOL CSysInfo::IsNT()
{
	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if( !GetVersionEx(&osinfo) )
		return FALSE;

	if( VER_PLATFORM_WIN32_NT == osinfo.dwPlatformId )
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
// 得到CPU个数
//-----------------------------------------------------------------------------
INT CSysInfo::GetProcessorNum()
{
	SYSTEM_INFO  sysinfo;
    GetSystemInfo(&sysinfo);
    return (INT)sysinfo.dwNumberOfProcessors;
}

//-----------------------------------------------------------------------------
// 根据异常码得到异常描述字符串
//-----------------------------------------------------------------------------
const TCHAR *CSysInfo::GetExceptionDescription(DWORD ExceptionCode)
{
	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
			return ExceptionMap[i].ExceptionName;

	return _T("an Unknown exception type");
}



//-----------------------------------------------------------------------------
// FormatTime 适合需要避免使用CRT的情况
//-----------------------------------------------------------------------------
VOID CSysInfo::FormatTime(LPTSTR pDest, FILETIME TimeToPrint)
{
	pDest[0] = _T('\0');
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&TimeToPrint, &TimeToPrint) &&
				FileTimeToDosDateTime(&TimeToPrint, &wDate, &wTime))
	{
		wsprintf(pDest, _T("%d/%d/%d %02d:%02d:%02d"),
					(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
					(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);
	}
}

//-----------------------------------------------------------------------------
// GetFilePart
//-----------------------------------------------------------------------------
TCHAR* CSysInfo::GetFilePart(LPCTSTR pSource)
{
	if( pSource )
	{
		TCHAR *pResult = lstrrchr(pSource, _T('\\'));
		if( pResult )
			pResult++;
		else
			pResult = (TCHAR *)pSource;
		return pResult;
	}

	return NULL;
}

// lstrrchr (avoid the C Runtime )
TCHAR* CSysInfo::lstrrchr(LPCTSTR string, int ch)
{
	TCHAR *start = (TCHAR *)string;

	while (*string++)                       /* find end of string */
		;
											/* search towards front */
	while (--string != start && *string != (TCHAR) ch)
		;

	if (*string == (TCHAR) ch)                /* char found ? */
		return (TCHAR *)string;

	return NULL;
}

typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // must be 0x1000
   LPCSTR szName; // pointer to name (in user addr space)
   DWORD dwThreadID; // thread ID (-1=caller thread)
   DWORD dwFlags; // reserved for future use, must be zero
} THREADNAME_INFO;

//-----------------------------------------------------------------------------
// 设置线程的名字
//-----------------------------------------------------------------------------
void CSysInfo::SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}

//-----------------------------------------------------------------------------
// 关闭系统
//-----------------------------------------------------------------------------
BOOL CSysInfo::SystemShutdown()
{
   HANDLE hToken;
   TOKEN_PRIVILEGES tkp;

   // Get a token for this process.
   if( !OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) )
      return( FALSE );

   // Get the LUID for the shutdown privilege.
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

   tkp.PrivilegeCount = 1;  // one privilege to set
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   // Get the shutdown privilege for this process.
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

   if( GetLastError() != ERROR_SUCCESS )
      return FALSE;

   // Shut down the system and force all applications to close.
   if( !ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0) )
      return FALSE;

   return TRUE;
}

#endif