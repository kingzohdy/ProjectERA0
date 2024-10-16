//-----------------------------------------------------------------------------
// File: sysinfo.h
// Auth: wizard
// Date: 2004/02/03
// Last: 2004/06/22
// Desc: 得到系统信息
//
// Copyright (c) 1985-2003 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#ifdef _WIN32		//只对win平台有效
#include <Windows.h>
class CSysInfo
{
public:

	// 得到Windows版本描述字符串
	static BOOL GetWinVer(LPTSTR pszVersion, LPTSTR pszBuild);
	// 检查是否NT操作系统
	static BOOL IsNT();

	// 得到DX版本
	static DWORD GetDXVersion();

	// 根据异常码得到异常描述字符串
	static CONST TCHAR *GetExceptionDescription(DWORD ExceptionCode);

	// FormatTime 适合需要避免使用CRT的情况
	static VOID FormatTime(LPTSTR pDest, FILETIME TimeToPrint);

	// GetFilePart
	static TCHAR* GetFilePart(LPCTSTR pSource);

	// lstrrchr (avoid the C Runtime )
	static TCHAR* lstrrchr(LPCTSTR string, int ch);

	// Usage: SetThreadName (-1, "MainThread");
	static VOID SetThreadName( DWORD dwThreadID, LPCSTR szThreadName);

	// 关闭系统
	static BOOL SystemShutdown();

	// 得到CPU指令集能力
	static DWORD GetCpuCaps();
	// 得到CPU个数
	static INT GetProcessorNum();

};

#endif