//-----------------------------------------------------------------------------
// File: exception
// Desc: winƽ̨��¼��������
// Auth: wizard
// Date: 2005/12/18
//
// Copyright (c) 1985-2005 tidy Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "winexception.h"
#include "sysinfo.h"
#include <tchar.h>
#include "OgreLog.h"
#include "crashrpt/include/CrashRpt.h"

#ifdef _WIN32		//ֻ��winƽ̨��Ч
//-----------------------------------------------------------------------------

//#pragma comment(lib, "imagehlp.lib")		// �˿���ϵͳ�Դ�

#pragma comment(linker, "/defaultlib:dbghelp.lib")
//#pragma comment(lib, "debug\\dbghelp.lib")	// �˿���΢����
#pragma warning(push)
#pragma warning(disable:4312)

#include <psapi.h>		// for EnumProcessModules


// �Ƿ���MiniDump�ļ�,
// �������ļ���΢���WinDbg���߽�Ͽ������ֵ�������,
#define DUMP_MINI_DUMP 


#define WRITE_SEPARATOR()	{ LOG_DUMP( _T("\r\n") ); }

#define REFERENCE(x)

// Microsoft C++ exception code
CONST UINT C_PLUS_PLUS_EXCEPTION	= 0xe06d7363;
CONST INT MAX_EXCEPTION_MSG_SIZE	= 256;
CONST INT MAX_CODE_BYTE_DUMP		= 16;	// ��¼�����ֽ�ָ��

CONST INT ONE_KILO			= 1024;
CONST INT SIXTYFOUR_KILO	= 64*ONE_KILO;
CONST INT ONE_MEGA			= ONE_KILO*ONE_KILO;
CONST INT ONE_GIGA			= ONE_KILO*ONE_KILO*ONE_KILO;

UINT		CWinException::m_unExceptionCode = 0;
HANDLE		CWinException::m_hProcess = 0;

using namespace Ogre;

//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
CWinException::CWinException(LPTSTR szMsg, LPSTR szFileName, INT nLineNum)
{
	//����log��������
	char szTime[512];
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = _T('\0');
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// �������Լ�ʱ����
		wsprintf(szTime, _T("log\\Dump[%d-%d-%d %02d%02d%02d].log"),
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);

	}
	LogAddFileHandler(szTime, LOGL_DUMP);
	crAddFile2(szTime, NULL, "dump log", CR_AF_TAKE_ORIGINAL_FILE);
	// ����Ա�ֶ��׳����쳣����¼�׳��ļ����кŵ���Ϣ
	WRITE_SEPARATOR();

	LOG_DUMP( _T("%s\r\n"), szMsg );
	LOG_DUMP( _T("c++ exception thrown in %s line%d\r\n"), szFileName, nLineNum );
	LOG_DUMP( _T("process would crash itself here for recording more information\r\n") );

	// �����׳�һ���ṹ���쳣���Ա��¼��ǰ��ջ����Ϣ
	RaiseException(C_PLUS_PLUS_EXCEPTION, 0, 0, NULL);
}

//-----------------------------------------------------------------------------
//consructor
//-----------------------------------------------------------------------------
CWinException::CWinException(const std::string& strMsg, LPSTR szFileName, INT nLineNum)
{
	//����log��������
	char szTime[512];
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = _T('\0');
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// �������Լ�ʱ����
		wsprintf(szTime, _T("log\\Dump[%d-%d-%d %02d%02d%02d].log"),
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);

	}
	LogAddFileHandler(szTime, LOGL_DUMP);
	crAddFile2(szTime, NULL, "dump log", CR_AF_TAKE_ORIGINAL_FILE);
	// ����Ա�ֶ��׳����쳣����¼�׳��ļ����кŵ���Ϣ
	WRITE_SEPARATOR();

	LOG_DUMP( _T("%s\r\n"), strMsg.c_str());
	LOG_DUMP( _T("c++ exception thrown in %s line%d\r\n"), szFileName, nLineNum );
	LOG_DUMP( _T("process would crash itself here for recording more information\r\n") );

	// �����׳�һ���ṹ���쳣���Ա��¼��ǰ��ջ����Ϣ
	RaiseException(C_PLUS_PLUS_EXCEPTION, 0, 0, NULL);
}

CWinException::CWinException(EXCEPTION_POINTERS* pExp ,UINT unCode)
{
 	InitDebugHelp();
 	DumpExceptionAddress( pExp );
	//����log��������
 	char szTime[512];
 	FILETIME CurrentTime;
 	GetSystemTimeAsFileTime(&CurrentTime);
 	szTime[0] = _T('\0');
 	WORD wDate, wTime;
 	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
 		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
 	{
 		// �������Լ�ʱ����
 		wsprintf(szTime, _T("log\\Dump[%d-%d-%d %02d%02d%02d].log"),
 			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
 			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);
 
 	}
 	LogAddFileHandler(szTime, LOGL_DUMP);
	crAddFile2(szTime, NULL, "dump log", CR_AF_TAKE_ORIGINAL_FILE);

 	m_unExceptionCode = unCode;
 	DumpAll(pExp);
}

CWinException::~CWinException()
{
	SymCleanup(GetCurrentProcess());
}

// ��ʼ�����ű�
//-----------------------------------------------------------------------------
VOID CWinException::InitDebugHelp()
{
	SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME 
		| SYMOPT_LOAD_LINES | SYMOPT_OMAP_FIND_NEAREST);

	if( FALSE == CSysInfo::IsNT() )
		//TODO HERE
		m_hProcess = (HANDLE)(GetCurrentProcessId());
	else
		m_hProcess = GetCurrentProcess();

	if( FALSE == SymInitialize(m_hProcess, "symbol", TRUE) )
	{
		typedef  BOOL (WINAPI *MYPROC1)(HANDLE, HMODULE*, DWORD, LPDWORD); 
		typedef DWORD (WINAPI *MYPROC2)(HANDLE, HMODULE, LPTSTR, DWORD);

		MYPROC1 ProcEnumProcessModules;
		MYPROC2 ProcGetModuleFileNameEx;

		HMODULE hHandle = LoadLibrary("psapi.dll");	// for EnumProcessModules
		if( NULL != hHandle )
		{
			ProcEnumProcessModules = (MYPROC1)GetProcAddress(hHandle, "EnumProcessModules");
			ProcGetModuleFileNameEx = (MYPROC2)GetProcAddress(hHandle, "GetModuleFileNameExA");
			if( NULL != ProcEnumProcessModules && NULL != ProcGetModuleFileNameEx )
			{
				// ���÷���Ѱ��·��
				SymSetSearchPath(m_hProcess, "symbol");

				HMODULE hMods[1024];
				DWORD cbNeeded;
				// ��������ģ��ķ��ű�
				// Get a list of all the modules in this process.
				HANDLE hProcess = ::GetCurrentProcess();
				if( (ProcEnumProcessModules)(hProcess, hMods, sizeof(hMods), &cbNeeded) )
				{
					for( unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
					{
						char szModName[MAX_PATH+1];

						// Get the full path to the module's file.
						if( (ProcGetModuleFileNameEx)( hProcess, hMods[i], szModName,
							sizeof(szModName)))
						{
							// load all modules' symbol
							SymLoadModule(m_hProcess, NULL, szModName, NULL, 0, 0);
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ͨ��������ʼ��ַ��ú�����
//-----------------------------------------------------------------------------
BOOL CWinException::GetFunctionName(LPTSTR szDest, PVOID pFunction)
{    
	TCHAR SymbolBuffer[sizeof(IMAGEHLP_SYMBOL)+1024];   
	DWORD dwTemp = 0;    
	
	PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)SymbolBuffer;    
	memset(SymbolBuffer, 0, sizeof(SymbolBuffer));    
	pSymbol->SizeOfStruct = sizeof(SymbolBuffer);    
	pSymbol->MaxNameLength = 512;
	if( SymGetSymFromAddr(m_hProcess, (DWORD)PtrToUlong(pFunction), &dwTemp, pSymbol) )
	{        
		IMAGEHLP_LINE l;
		ZeroMemory(&l, sizeof(l));
		l.SizeOfStruct = sizeof(l);

#ifdef UNICODE		
		// ��ȡԴ�ļ�����Դ������
		if( SymGetLineFromAddr(m_hProcess, (DWORD)pFunction, &dwTemp, &l) )
		{
			wsprintf(szDest, _T("%s() %s Line%lu"), 
				CUtil::CharStringToUnicode(pSymbol->Name), 
				CSysInfo::GetFilePart(
				CUtil::CharStringToUnicode(l.FileName) ), l.LineNumber);
		}
		else
		{
			lstrcpy( szDest, CUtil::CharStringToUnicode(pSymbol->Name) );
			lstrcat( szDest, _T("()") );
		}
#else
		// ��ȡԴ�ļ�����Դ������
		if( SymGetLineFromAddr(m_hProcess, (DWORD)PtrToUlong(pFunction), &dwTemp, &l) )
		{
			wsprintf(szDest, _T("%s() %s Line%lu"), pSymbol->Name, 
				CSysInfo::GetFilePart(l.FileName), l.LineNumber);
		}
		else
		{
			lstrcpy( szDest, pSymbol->Name );
			lstrcat( szDest, _T("()") );


			TCHAR szModulePathName[MAX_PATH+1024];
			ZeroMemory(szModulePathName, sizeof(szModulePathName));
			MEMORY_BASIC_INFORMATION MemInfo;

			// �跨ȡ�ú�������ģ������
			if( VirtualQuery(pFunction, &MemInfo, sizeof(MemInfo)) &&
				(GetModuleFileName((HINSTANCE)MemInfo.AllocationBase,
										  szModulePathName,
										  sizeof(szModulePathName)-2) > 0) )
			{
				lstrcat( szDest, " in " );
				lstrcat( szDest, CSysInfo::GetFilePart(szModulePathName) );
			}
		}
#endif // #ifdef UNICODE

		return TRUE;    
	}
	else
	{
		TCHAR szModulePathName[MAX_PATH+1024];
		ZeroMemory(szModulePathName, sizeof(szModulePathName));
		MEMORY_BASIC_INFORMATION MemInfo;

		// ����ò�����������,���跨ȡ�ú�������ģ������
		if( VirtualQuery(pFunction, &MemInfo, sizeof(MemInfo)) &&
			(GetModuleFileName((HINSTANCE)MemInfo.AllocationBase,
										  szModulePathName,
										  sizeof(szModulePathName)-2) > 0) )
		{
			lstrcpy( szDest, CSysInfo::GetFilePart(szModulePathName) );
		}
		else
		{
			lstrcpy( szDest, _T("Unknown Function") );
		}
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// ��¼��ǰ����
//-----------------------------------------------------------------------------
INT CWinException::DumpAll(EXCEPTION_POINTERS *pEx)
{
	static BOOL bFirstTime = TRUE;
	
	// ����ݹ����,�ݹ���ñ�ʾ�˺����������쳣
	if( !bFirstTime )
		return EXCEPTION_CONTINUE_SEARCH;
	bFirstTime = FALSE;

	// ע��������Щ�����в��ܶ�pEx��ָ�����������ݽ����޸�
	// �����̵���û����������
	DumpExceptionInfo(pEx);
	DumpCallStack(pEx);
	DumpRegisters(pEx);
	DumpInstructions(pEx);
	DumpSystemInfo(pEx);
	DumpVariable(pEx);

	// ������Ҫ����MiniDump�ļ�,�����Ƚ�DUMP_MINI_DUMP���
	DumpMiniDump(pEx);	
	
	// ʹϵͳֹͣ���������쳣ƥ��
	return EXCEPTION_CONTINUE_EXECUTION;

}

//-----------------------------------------------------------------------------
// ��¼�쳣��������Ϣ
//-----------------------------------------------------------------------------
VOID CWinException::DumpExceptionInfo(EXCEPTION_POINTERS *pEx)
{
	// ��¼���������û�����ʱ��
	TCHAR szModuleName[MAX_PATH*2];
	ZeroMemory(szModuleName, sizeof(szModuleName));
	if (GetModuleFileName(0, szModuleName, MAX_PATH*2-2) <= 0)
		lstrcpy(szModuleName, _T("Unknown"));

	TCHAR *pszFilePart = CSysInfo::GetFilePart(szModuleName);

	// Extract the file name portion and remove it's file extension
	TCHAR szFileName[MAX_PATH*2];
	lstrcpy(szFileName, pszFilePart);

	TCHAR szUserName[200];
	ZeroMemory(szUserName, sizeof(szUserName));
	DWORD UserNameSize = 200-2;
	if( !GetUserName(szUserName, &UserNameSize) )
		lstrcpy(szUserName, _T("Unknown"));

	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	TCHAR szTime[100];
	CSysInfo::FormatTime(szTime, CurrentTime);

	WRITE_SEPARATOR();
	LOG_DUMP( _T("%s run by %s in %s crashed.\r\n"), szFileName, szUserName, szTime );

	// ��¼�쳣����
	PEXCEPTION_RECORD Exception = pEx->ExceptionRecord;
	PCONTEXT          Context   = pEx->ContextRecord;
	LOG_DUMP( _T("The program caused %s (0x%08x)\r\n"),
				CSysInfo::GetExceptionDescription(Exception->ExceptionCode),
				Exception->ExceptionCode);

	// ��¼�����쳣��ģ��
	TCHAR szCrashModulePathName[MAX_PATH*2];
	ZeroMemory(szCrashModulePathName, sizeof(szCrashModulePathName));

	TCHAR *pszCrashModuleFileName = _T("Unknown");
	MEMORY_BASIC_INFORMATION MemInfo;

	// VirtualQuery can be used to get the allocation base associated with a
	// code address, which is the same as the ModuleHandle. This can be used
	// to get the filename of the module that the crash happened in.
	if( VirtualQuery((void*)Context->Eip, &MemInfo, sizeof(MemInfo)) &&
						(GetModuleFileName((HINSTANCE)MemInfo.AllocationBase,
										  szCrashModulePathName,
										  sizeof(szCrashModulePathName)-2) > 0) )
	{
		pszCrashModuleFileName = CSysInfo::GetFilePart(szCrashModulePathName);
	}

	LOG_DUMP( _T("in module %s at %04x:%08x.\r\n"),
				pszCrashModuleFileName, Context->SegCs, Context->Eip );

	// ����������ǷǷ�����,��¼�Ƕ�����д�����Լ���ͼ������λ��
	if( Exception->ExceptionCode == STATUS_ACCESS_VIOLATION &&
				Exception->NumberParameters >= 2)
	{
		TCHAR szDebugMessage[1000];
		const TCHAR* readwrite = _T("Access violation: read from");
		if (Exception->ExceptionInformation[0])
			readwrite = _T("Access violation: write to");
		wsprintf(szDebugMessage, _T("%s location %08x\r\n"),
					readwrite, Exception->ExceptionInformation[1]);

#ifdef	_DEBUG
		// Visual C++ ������ȱ�ٴ˹���,����Ҳ��������
		OutputDebugString(_T("Exception handler: "));
		OutputDebugString(szDebugMessage);
#endif

		LOG_DUMP(szDebugMessage);
	}


}

//-----------------------------------------------------------------------------
// ��¼ϵͳ��Ϣ
//-----------------------------------------------------------------------------
VOID CWinException::DumpSystemInfo(EXCEPTION_POINTERS *pEx)
{
	REFERENCE(pEx);

	WRITE_SEPARATOR();
	// print out operating system
	TCHAR szWinVer[50], szBuild[50];
	CSysInfo::GetWinVer(szWinVer, szBuild);
	LOG_DUMP( _T("Operating system:  %s (%s).\r\n"), szWinVer, szBuild );

	// ����¼��һ��CPU���ͺ�
	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);
	LOG_DUMP( _T("%d processor(s), type %d.\r\n"),
				SystemInfo.dwNumberOfProcessors, SystemInfo.dwProcessorType);

	MEMORYSTATUS MemInfo;
	MemInfo.dwLength = sizeof(MemInfo);
	GlobalMemoryStatus(&MemInfo);

	// Print out info on memory, rounded up.
	LOG_DUMP( _T("Physical memory: Total-%dMB Free-%dMB.\r\n"),
		(MemInfo.dwTotalPhys + ONE_MEGA - 1) / ONE_MEGA,
		(MemInfo.dwAvailPhys + ONE_MEGA - 1) / ONE_MEGA );

	LOG_DUMP( _T("Paging File: Total-%dMB Free-%dMB.\r\n"),
		(MemInfo.dwTotalPageFile + ONE_MEGA - 1) / ONE_MEGA,
		(MemInfo.dwAvailPageFile + ONE_MEGA - 1) / ONE_MEGA );
}



//-----------------------------------------------------------------------------
// ��¼���ö�ջ
//-----------------------------------------------------------------------------
VOID CWinException::DumpCallStack(EXCEPTION_POINTERS *pEx)
{
	
	DWORD dw1=1, dw2=2, dw3;
	dw3 = dw2*dw1+dw2;

	WRITE_SEPARATOR();
	LOG_DUMP(_T("CallStack:\r\n"));

	STACKFRAME StackFrame;
	memset(&StackFrame, 0, sizeof(STACKFRAME));
	StackFrame.AddrPC.Mode = AddrModeFlat;    
	StackFrame.AddrPC.Offset = pEx->ContextRecord->Eip;    
	StackFrame.AddrStack.Mode = AddrModeFlat;    
	StackFrame.AddrStack.Offset = pEx->ContextRecord->Esp;    
	StackFrame.AddrFrame.Mode = AddrModeFlat;    
	StackFrame.AddrFrame.Offset = pEx->ContextRecord->Ebp;

	BOOL bRet = TRUE;
	TCHAR szName[1024];
	TCHAR szTemp[1024];
	
	// StackWalk�����context,�����������Ǳ��븴��һ�ݸ�����
	// ��ΪCONTEXT�ṹ��û��ָ���Ա,���Կ��԰�ȫ����ȫ����
	CONTEXT context;
	memcpy( &context, pEx->ContextRecord, sizeof(context) );

	for(INT nCount=0; nCount < 256 && bRet; nCount++)    
	{       
		// ���ݺ���
		bRet = StackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), 
			GetCurrentThread(), &StackFrame, &context, NULL, 
			SymFunctionTableAccess, SymGetModuleBase, NULL);

		if( bRet )
		{
			GetFunctionName(szName, (PVOID)StackFrame.AddrPC.Offset);
			wsprintf(szTemp, _T("%s at %#08lx\r\n"), szName, 
				StackFrame.AddrPC.Offset);

			LOG_DUMP(szTemp);
		}
	}
}

//-----------------------------------------------------------------------------
// ��¼�Ĵ���״̬
//-----------------------------------------------------------------------------
VOID CWinException::DumpRegisters(EXCEPTION_POINTERS *pEx)
{
	if( !pEx )
		return;

	PCONTEXT pContext = pEx->ContextRecord;
	
	// Print out the register values in an XP error window compatible format.
	WRITE_SEPARATOR();
	LOG_DUMP( _T("Context:\r\n") );
	LOG_DUMP( _T("EDI:    0x%08x  ESI: 0x%08x  EAX:   0x%08x\r\n"),
				pContext->Edi, pContext->Esi, pContext->Eax );
	LOG_DUMP( _T("EBX:    0x%08x  ECX: 0x%08x  EDX:   0x%08x\r\n"),
				pContext->Ebx, pContext->Ecx, pContext->Edx);
	LOG_DUMP( _T("EIP:    0x%08x  EBP: 0x%08x  SegCs: 0x%08x\r\n"),
				pContext->Eip, pContext->Ebp, pContext->SegCs);
	LOG_DUMP( _T("EFlags: 0x%08x  ESP: 0x%08x  SegSs: 0x%08x\r\n"),
				pContext->EFlags, pContext->Esp, pContext->SegSs);
				
}

//-----------------------------------------------------------------------------
// ��¼��ǰָ��
//-----------------------------------------------------------------------------
VOID CWinException::DumpInstructions(EXCEPTION_POINTERS *pEx)
{
	// �п���EIP�Ĵ���ָ����Ч��ַ,����������Ҫ���쳣����
	WRITE_SEPARATOR();
	LOG_DUMP( _T("Bytes at CS:EIP:  (XX means unaccessible)\r\n") );
	BYTE * pCode = (BYTE *)pEx->ContextRecord->Eip;
	for( int nCodeByte = 0; nCodeByte < MAX_CODE_BYTE_DUMP; nCodeByte++ )
	{
		__try
		{
			LOG_DUMP( _T("%02x "), pCode[nCodeByte] );
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			LOG_DUMP( _T("XX ") );
		}
	}
	
	LOG_DUMP( _T("\r\n") );
}

//-----------------------------------------------------------------------------
// ����MiniDump�ļ�,�����ļ���WinDbg��Ͽ������ֵ�������
//-----------------------------------------------------------------------------
VOID CWinException::DumpMiniDump(EXCEPTION_POINTERS* pEx)
{
	REFERENCE(pEx);

// ����Ҫ����MiniDump�ļ�,���Ƚ�DUMP_MINI_DUMP���
#ifdef DUMP_MINI_DUMP

	// ���ɺ��ʵ�dmp�ļ���
	TCHAR szTime[512], szTemp[512];
	GetModuleFileNameA(NULL, szTemp, MAX_PATH);

	// ��ȥ·��
	TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
	pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
	_stprintf(m_szPath, "log\\%s", pResult);
	// ��ȥ�ļ���չ��
	pResult = _tcsrchr(m_szPath, _T('.'));
	if( pResult )
		*pResult = '\0';	
	// ����ʱ��
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = _T('\0');
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// �������Լ�ʱ����
		wsprintf(szTime, _T("[%d-%d-%d %02d%02d%02d].dmp"),
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);

		_tcscat(m_szPath, szTime);
	}

	// Create the file
	HANDLE hFile = CreateFile( m_szPath, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,	NULL);

	if( hFile == INVALID_HANDLE_VALUE )
		return;

	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId = GetCurrentThreadId();
	eInfo.ExceptionPointers = pEx;
	eInfo.ClientPointers = TRUE;

	// Write the minidump to the file
	MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile,
			(_MINIDUMP_TYPE)
			MiniDumpNormal ,
			&eInfo, NULL, NULL );

	CloseHandle(hFile);

#endif
}




//-----------------------------------------------------------------------------
// ��¼����
//-----------------------------------------------------------------------------
VOID CWinException::DumpVariable(EXCEPTION_POINTERS *pEx)
{
	WRITE_SEPARATOR();
	WRITE_SEPARATOR();
	WRITE_SEPARATOR();
	LOG_DUMP(_T("Variable:\r\n"));

	STACKFRAME StackFrame;
	memset(&StackFrame, 0, sizeof(STACKFRAME));
	StackFrame.AddrPC.Mode = AddrModeFlat;    
	StackFrame.AddrPC.Offset = pEx->ContextRecord->Eip;    
	StackFrame.AddrStack.Mode = AddrModeFlat;    
	StackFrame.AddrStack.Offset = pEx->ContextRecord->Esp;    
	StackFrame.AddrFrame.Mode = AddrModeFlat;    
	StackFrame.AddrFrame.Offset = pEx->ContextRecord->Ebp;
	
	BOOL bRet = TRUE;
	TCHAR szName[1024];
	TCHAR szTemp[1024];
	
	// StackWalk�����context,�����������Ǳ��븴��һ�ݸ�����
	// ��ΪCONTEXT�ṹ��û��ָ���Ա,���Կ��԰�ȫ����ȫ����
	CONTEXT context;
	memcpy( &context, pEx->ContextRecord, sizeof(context) );

	for(INT nCount=0; nCount < 256 && bRet; nCount++)    
	{       
		// ���ݺ���
		bRet = StackWalk(IMAGE_FILE_MACHINE_I386, m_hProcess, 
			GetCurrentThread(), &StackFrame, &context, NULL, 
			SymFunctionTableAccess, SymGetModuleBase, NULL);

		if( bRet )
		{
			BOOL bResult = GetFunctionName(szName, (PVOID)StackFrame.AddrPC.Offset);
			if( FALSE == bResult )
				continue;

			WRITE_SEPARATOR();
			wsprintf(szTemp, _T("%s at %#08lx\r\n"), szName, 
				StackFrame.AddrPC.Offset);
			LOG_DUMP(szTemp);

			IMAGEHLP_STACK_FRAME iStackFrame;
			iStackFrame.InstructionOffset = StackFrame.AddrPC.Offset;
			SymSetContext( m_hProcess, &iStackFrame, 0 );

			// ���ǽ�context�����ص�����,�Ա���ʹ��
			SymEnumSymbols( m_hProcess, 0, NULL, SymEnumSymbolsProc, &StackFrame );
		}
	}
}

void CWinException::DumpExceptionAddress( EXCEPTION_POINTERS *pExp )
{
	PEXCEPTION_RECORD Exception = pExp->ExceptionRecord;
	PCONTEXT          Context   = pExp->ContextRecord;

	LOG_WARNING( _T("\r\nThe program caused %s (0x%08x)\r\n"),
		CSysInfo::GetExceptionDescription(Exception->ExceptionCode),
		Exception->ExceptionCode);
	TCHAR szCrashModulePathName[MAX_PATH*2];
	TCHAR *pszCrashModuleFileName = _T("Unknown");
	MEMORY_BASIC_INFORMATION MemInfo;

	if( VirtualQuery((void*)Context->Eip, &MemInfo, sizeof(MemInfo)) &&
		(GetModuleFileName((HINSTANCE)MemInfo.AllocationBase,
		szCrashModulePathName,
		sizeof(szCrashModulePathName)-2) > 0) )
	{
		pszCrashModuleFileName = CSysInfo::GetFilePart(szCrashModulePathName);
	}
	LOG_WARNING( _T("in module %s at %04x:%08x.\r\n"),
		pszCrashModuleFileName, Context->SegCs, Context->Eip );

	DWORD dw1=1, dw2=2, dw3;
	dw3 = dw2*dw1+dw2;

	LOG_WARNING(_T("CallStack:\r\n"));

	STACKFRAME StackFrame;
	memset(&StackFrame, 0, sizeof(STACKFRAME));
	StackFrame.AddrPC.Mode = AddrModeFlat;    
	StackFrame.AddrPC.Offset = pExp->ContextRecord->Eip;    
	StackFrame.AddrStack.Mode = AddrModeFlat;    
	StackFrame.AddrStack.Offset = pExp->ContextRecord->Esp;    
	StackFrame.AddrFrame.Mode = AddrModeFlat;    
	StackFrame.AddrFrame.Offset = pExp->ContextRecord->Ebp;

	BOOL bRet = TRUE;
	TCHAR szName[1024];
	TCHAR szTemp[1024];

	// StackWalk�����context,�����������Ǳ��븴��һ�ݸ�����
	// ��ΪCONTEXT�ṹ��û��ָ���Ա,���Կ��԰�ȫ����ȫ����
	CONTEXT context;
	memcpy( &context, pExp->ContextRecord, sizeof(context) );

	for(INT nCount=0; nCount < 256 && bRet; nCount++)    
	{       
		// ���ݺ���
		bRet = StackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), 
			GetCurrentThread(), &StackFrame, &context, NULL, 
			SymFunctionTableAccess, SymGetModuleBase, NULL);

		if( bRet )
		{
			GetFunctionName(szName, (PVOID)StackFrame.AddrPC.Offset);
			wsprintf(szTemp, _T("%s at %#08lx\r\n"), szName, 
				StackFrame.AddrPC.Offset);
			LOG_WARNING(szTemp);
		}
	}
}


//-----------------------------------------------------------------------------
// �ֲ�����ö�ٻص�����
//-----------------------------------------------------------------------------
BOOL CALLBACK CWinException::SymEnumSymbolsProc(PSYMBOL_INFO pSym, 
												 ULONG dwSize, PVOID pContext)
{
	REFERENCE(dwSize);

    __try
    {
        if( FALSE == FormatSymbolValue(pSym, (STACKFRAME*)pContext) )
			LOG_DUMP( _T("failed on formatting symbol %s\r\n"), pSym->Name );
    }
    __except( 1 )
    {
        LOG_DUMP( _T("kicked out at symbol %s\r\n"), pSym->Name );
    }

    return TRUE;
/*
	__try
    {
		if( pSym && pSym->Name && pSym->NameLen > 1 )
		{
			DWORD3264 pVariable = 0;
			
			if( pSym->Flags & SYMFLAG_REGREL )
			{
				pVariable = pCon->AddrFrame.Offset;
				pVariable += (DWORD3264)pSym->Address;
			}
			else 
			{
				if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER )
					pVariable = 0;
				else
					pVariable = (DWORD3264)pSym->Address;   
			}

			GetFrameData( pSym, pVariable, pContext );
		}
    }
    __except( 1 )
    {
    }

	return TRUE;*/
}


//-----------------------------------------------------------------------------
// Ѱ��Frame�������
//-----------------------------------------------------------------------------
VOID CWinException::GetFrameData(PSYMBOL_INFO pSym, PVOID pPos, PVOID pContext)
{
	REFERENCE(pPos);
	REFERENCE(pContext);
//	STACKFRAME *pCon = (STACKFRAME*)pContext;
	
	LOG_DUMP(_T("	"));

	if( pSym->Flags & SYMFLAG_PARAMETER )
		LOG_DUMP(_T("function parameter : "));
	else 
	{
		if( pSym->Flags & SYMFLAG_LOCAL )
			LOG_DUMP(_T("local variable : "));
		else 
			LOG_DUMP(_T("unknow type : "));
	}

#ifdef UNICODE
	LOG_DUMP( CUtil::CharStringToUnicode(pSym->Name) );
#else
	LOG_DUMP(pSym->Name);
#endif	// #ifdef UNICODE
	
		DWORD typeId = 0;
		SymGetTypeInfo( m_hProcess, pSym->ModBase, pSym->TypeIndex, 
			TI_GET_TYPEID, &typeId);

	// �õ��ӳ�Ա������
	DWORD dwChildCounter = 0;
	SymGetTypeInfo( m_hProcess, pSym->ModBase, typeId, 
		TI_GET_CHILDRENCOUNT, &dwChildCounter );

	if( 0 == lstrcmpA("this", pSym->Name) )
	{
		// Display the class name
		LPTSTR pClassName;
		SymGetTypeInfo( m_hProcess, pSym->ModBase, typeId, 
			TI_GET_SYMNAME, &pClassName);
		
		LOG_DUMP(_T("(%ls "), pClassName );
		LocalFree( pClassName );
		
		// record sizeof the class
		DWORD dwSize = 0;
		SymGetTypeInfo( m_hProcess, pSym->ModBase, typeId, TI_GET_LENGTH, &dwSize);
		LOG_DUMP(_T("size%lu)"), dwSize);
		
	
		// ׼�����ݽṹ������ӳ�Ա��Ϣ
		INT nChildLen = sizeof(_TI_FINDCHILDREN_PARAMS) + 4*dwChildCounter;
		BYTE* pChild = new BYTE[nChildLen];
		ZeroMemory(pChild, nChildLen);
		((_TI_FINDCHILDREN_PARAMS*)pChild)->Count = dwChildCounter;
		
		SymGetTypeInfo( m_hProcess, pSym->ModBase, typeId, 
			TI_FINDCHILDREN, pChild );
		
		WRITE_SEPARATOR();
		for( DWORD n=0; n<dwChildCounter; n++ )
		{
			// Display the class name
			LPTSTR pClassName;
			SymGetTypeInfo( m_hProcess, pSym->ModBase, 
				((_TI_FINDCHILDREN_PARAMS*)pChild)->ChildId[n], 
				TI_GET_SYMNAME, &pClassName);
			
			DWORD dwChildOffset = ~0;
			if( !SymGetTypeInfo( m_hProcess, pSym->ModBase, 
				((_TI_FINDCHILDREN_PARAMS*)pChild)->ChildId[n], 
				TI_GET_OFFSET, &dwChildOffset) )
				dwChildOffset = ~0;

			LOG_DUMP(_T("		%ls off:%x\r\n "), pClassName, dwChildOffset );
			LocalFree( pClassName );
		}
		
		delete[] pChild;
	}
	
	/*			DWORD dwChildCounter = 0;
	DWORD* p = &dwChildCounter;
	BOOL bResult = SymGetTypeInfo(m_hProcess, pSym->ModBase, 
				pSym->TypeIndex, TI_FINDCHILDREN, p);
				
				  if( bResult )
				  {
				  LOG_DUMP(_T(" Child(%x)"), dwChildCounter);
				  }
	*/			
	
	/*
	if( 0 ) //pSym->TypeIndex == 12 )
		LOG_DUMP(_T(" = %s"), (LPTSTR)(*(DWORD*)pVariable));
	else
		LOG_DUMP(_T(" = %X"), pVariable);
	LOG_DUMP("\r\n");	*/
}


//-----------------------------------------------------------------------------
// Given a SYMBOL_INFO representing a particular variable, displays its
// contents.  If it's a user defined type, display the members and their
// values.
//-----------------------------------------------------------------------------
BOOL CWinException::FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME * sf)
{
    // ��������
    if ( pSym->Tag == 5 )   // SymTagFunction from CVCONST.H from the DIA SDK
        return FALSE;

    DWORD3264 pVariable = 0;    // ָ�������ʵ������
    if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE )
    {
		pVariable = sf->AddrFrame.Offset;
		pVariable += (DWORD3264)pSym->Address;
    }
    else 
	{
		if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER )
	        return FALSE;   // Don't try to report register variable
		else
            pVariable = (DWORD3264)pSym->Address;
    }

    // �����ǲ������Ǳ���
    if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER )
        LOG_DUMP( _T("-Parameter ") );
    else if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_LOCAL )
        LOG_DUMP( _T("-Local ") );
	else
		LOG_DUMP( _T("-Unknow data kind ") );


	// ���������������
#ifdef _UNICODE
	LOG_DUMP(_T("\'%s\'"), CUtil::CharStringToUnicode(pSym->Name) );
#else	
    LOG_DUMP(_T("\'%s\'"), pSym->Name );
#endif

    // Determine if the variable is a user defined type (UDT).  IF so, bHandled
    // will return true.
	DWORD dwTypeIndex = pSym->TypeIndex;
	EBasicType basicType = EBT_NoType;
	BOOL bHandled = FALSE;

    bHandled = DumpTypeIndex(pSym->ModBase, dwTypeIndex, pVariable, 0);

    if ( !bHandled )
    {
        // The symbol wasn't a UDT, so do basic, stupid formatting of the
        // variable.  Based on the size, we're assuming it's a char, WORD, or
        // DWORD.
        basicType = GetBasicType( pSym->TypeIndex, pSym->ModBase );
        FormatOutputValue(basicType, pSym->Size, (PVOID)pVariable ); 
    }


    return true;
}

//-----------------------------------------------------------------------------
// If it's a user defined type (UDT), recurse through its members until we're
// at fundamental types.  When he hit fundamental types, return
// bHandled = false, so that FormatSymbolValue() will format them.
//-----------------------------------------------------------------------------
BOOL CWinException::DumpTypeIndex(DWORD64 Base64, DWORD dwTypeIndex, 
							   DWORD3264 pOffset, INT nCycle)
{
	if( nCycle > 10 )	// �ݹ����̫��
		return TRUE;

	DWORD dwSymTag = 0;
	SymGetTypeInfo( m_hProcess, Base64, dwTypeIndex, TI_GET_SYMTAG, &dwSymTag );
    if( dwSymTag == 5 )	// ������������
		return TRUE;

    // ���������
    WCHAR* pwszTypeName;
    if ( SymGetTypeInfo( m_hProcess, Base64, dwTypeIndex, TI_GET_SYMNAME,
                        &pwszTypeName ) )
    {
		for( INT n=0; n<nCycle; n++ )
			LOG_DUMP( _T("    ") );
        LOG_DUMP( _T("%ls"), pwszTypeName );
        LocalFree( pwszTypeName );
    }


	if( dwSymTag == 0xe )	// �����ָ������
	{
		// �õ���Ա��ʵ��"TypeId"
		SymGetTypeInfo( m_hProcess, Base64, dwTypeIndex,
			TI_GET_TYPEID, &dwTypeIndex );

		pOffset = *(DWORD*)pOffset;
	}

    // ����ж��ٸ���Ա
    DWORD dwChildrenCount = 0;
    SymGetTypeInfo( m_hProcess, Base64, dwTypeIndex, TI_GET_CHILDRENCOUNT,
                    &dwChildrenCount );

	// û�г�Ա
    if( !dwChildrenCount )     
	{
		return FALSE;
		if( EBT_NoType == GetBasicType(dwTypeIndex, Base64) )
			return TRUE;
		else
			return FALSE;
	}

	WRITE_SEPARATOR();

    // Prepare to get an array of "TypeIds", representing each of the children.
    // SymGetTypeInfo(TI_FINDCHILDREN) expects more memory than just a
    // TI_FINDCHILDREN_PARAMS struct has.  Use derivation to accomplish this.
    struct FINDCHILDREN : TI_FINDCHILDREN_PARAMS
    {
        ULONG   MoreChildIds[1024];
        FINDCHILDREN(){Count = sizeof(MoreChildIds) / sizeof(MoreChildIds[0]);}
    } children;

    children.Count = dwChildrenCount;
    children.Start= 0;

    // Get the array of TypeIds, one for each child type
    if( !SymGetTypeInfo( m_hProcess, Base64, dwTypeIndex, TI_FINDCHILDREN,
                            &children ) )
    {
        return FALSE;
    }

    // ���ÿ����Ա
    for( DWORD i = 0; i < dwChildrenCount; i++ )
    {
        // �ݹ�������Ա
        BOOL bHandled2 =
			DumpTypeIndex( Base64, children.ChildId[i], pOffset, nCycle+1 );

        if( !bHandled2 )	// ��Ա����UDT,���������������ʵ��ֵ
        {
            // �õ���Ա����ڸ��ṹ��ƫ����
            DWORD dwMemberOffset;
            SymGetTypeInfo( m_hProcess, Base64, children.ChildId[i],
                            TI_GET_OFFSET, &dwMemberOffset );

            // �õ���Ա��ʵ��"TypeId"
            DWORD typeId;
            SymGetTypeInfo( m_hProcess, Base64, children.ChildId[i],
                            TI_GET_TYPEID, &typeId );

            // �õ���Ա�Ĵ�С
            ULONG64 length;
            SymGetTypeInfo(m_hProcess, Base64, typeId, TI_GET_LENGTH,&length);

            // �����Ա�ĵ�ַ
            DWORD3264 dwFinalOffset = pOffset + dwMemberOffset;
            EBasicType basicType = GetBasicType(children.ChildId[i], Base64 );
            FormatOutputValue( basicType, length, (PVOID)dwFinalOffset ); 
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID CWinException::FormatOutputValue(EBasicType eType, DWORD64 Len64, PVOID pAddress)
{
	__try
	{
		if ( Len64 == 1 )
			LOG_DUMP( _T(" = 0x%X"), *(PBYTE)pAddress );
		else if ( Len64 == 2 )
			LOG_DUMP( _T(" = 0x%X"), *(PWORD)pAddress );
		else if ( Len64 == 4 )
		{
			if ( eType == EBT_Float )
			{
				LOG_DUMP( _T(" = %f"), *(PFLOAT)pAddress);
			}
			else if ( eType == EBT_Char )
			{
				if ( !IsBadStringPtr( *(LPTSTR*)pAddress, 1024) )
					LOG_DUMP( _T(" = \"%s\""), *(PDWORD)pAddress );
				else
					LOG_DUMP( _T(" = 0x%X"), *(PDWORD)pAddress );
			}
			else
				LOG_DUMP( _T(" = 0x%X"), *(PDWORD)pAddress );
		}
		else if ( Len64 == 8 )
		{
			if ( eType == EBT_Float )
				LOG_DUMP( _T(" = %lf"), *(double *)pAddress );
			else
				LOG_DUMP( _T(" = 0x%I64X"), *(DWORD64*)pAddress );
		}

		WRITE_SEPARATOR();
	}
	__except(1)
	{
		WRITE_SEPARATOR();
	}
}


//-----------------------------------------------------------------------------
// ���ݸ���dwTypeIndex,���ŵõ���������
//-----------------------------------------------------------------------------
CWinException::EBasicType CWinException::GetBasicType( DWORD dwTypeIndex, DWORD64 Base64 )
{
    EBasicType eType = EBT_NoType;
    if( SymGetTypeInfo(m_hProcess, Base64, dwTypeIndex, TI_GET_BASETYPE, &eType) )
        return eType;

    // ������ָ��������͵�ָ��,�����Ż�ȡ����������
    DWORD dwTypeId;
    if( SymGetTypeInfo(m_hProcess, Base64, dwTypeIndex, TI_GET_TYPEID, &dwTypeId) )
    {
        if( SymGetTypeInfo(m_hProcess, Base64, dwTypeId, TI_GET_BASETYPE, &eType) )
            return eType;
    }

    return EBT_NoType;
}
//-----------------------------------------------------------------------------
//�׳�assert������Ϣ
//-----------------------------------------------------------------------------
void CWinException::ThrowAssert(LPTSTR szMsg)
{
	//��ӡ������̨
	ThrowMsg(szMsg);

	//��ʽ��Ӫʱ��Ҫ��Щ�޸�, ֻ��ӡ���ļ���, ���׳��쳣
	switch(MessageBox(NULL,szMsg,"�쳣",MB_OKCANCEL))
	{
	case IDOK:
		{
			throw CWinException(szMsg, __FILE__, __LINE__);
		}
		break;
	case IDCANCEL:
		break;
	}
}
//-----------------------------------------------------------------------------
//�׳���Ϣ������̨
//-----------------------------------------------------------------------------
void CWinException::ThrowMsg(LPSTR szMsg)
{
	//��ӡ������̨
	LOG_SEVERE(szMsg);
}
#pragma warning(pop)
#endif