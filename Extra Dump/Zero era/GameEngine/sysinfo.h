//-----------------------------------------------------------------------------
// File: sysinfo.h
// Auth: wizard
// Date: 2004/02/03
// Last: 2004/06/22
// Desc: �õ�ϵͳ��Ϣ
//
// Copyright (c) 1985-2003 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#ifdef _WIN32		//ֻ��winƽ̨��Ч
#include <Windows.h>
class CSysInfo
{
public:

	// �õ�Windows�汾�����ַ���
	static BOOL GetWinVer(LPTSTR pszVersion, LPTSTR pszBuild);
	// ����Ƿ�NT����ϵͳ
	static BOOL IsNT();

	// �õ�DX�汾
	static DWORD GetDXVersion();

	// �����쳣��õ��쳣�����ַ���
	static CONST TCHAR *GetExceptionDescription(DWORD ExceptionCode);

	// FormatTime �ʺ���Ҫ����ʹ��CRT�����
	static VOID FormatTime(LPTSTR pDest, FILETIME TimeToPrint);

	// GetFilePart
	static TCHAR* GetFilePart(LPCTSTR pSource);

	// lstrrchr (avoid the C Runtime )
	static TCHAR* lstrrchr(LPCTSTR string, int ch);

	// Usage: SetThreadName (-1, "MainThread");
	static VOID SetThreadName( DWORD dwThreadID, LPCSTR szThreadName);

	// �ر�ϵͳ
	static BOOL SystemShutdown();

	// �õ�CPUָ�����
	static DWORD GetCpuCaps();
	// �õ�CPU����
	static INT GetProcessorNum();

};

#endif