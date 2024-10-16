//-----------------------------------------------------------------------------
// File: exception
// Desc: winƽ̨��¼��������
// Auth: wizard
// Date: 2005/12/18
//
// Copyright (c) 1985-2005 tidy Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#ifdef _WIN32		//ֻ��winƽ̨��Ч
#include <Windows.h>
#include <dbghelp.h>
// #include <eh.h>	// for "_set_se_translator"
#include <string>

#pragma warning(push)
#pragma warning(disable:4297)

// �ٶ������������쳣����ȷʵ������
#define EXCEPTION_BEGIN		\
	SetUnhandledExceptionFilter(TopLevelFilter);	\
	_set_invalid_parameter_handler(myInvalidParameterHandler); try{
#define EXCEPTION_END	} catch(CWinException e){ MessageBox(NULL, "������Ϣ�Ѿ����͸�������Ա�����ڵ�һʱ�䴦����", "��Ϸ���Բ���������Ʒ��", MB_OK);}

// ���ⲿʹ�õ��ó��������׳��쳣�ĺ�
#define WINEXCEPTION(excp_info) CWinException(excp_info, __FILE__, __LINE__);
//-----------------------------------------------------------------------------
// CWinException ���˼��:
// 1.ʹ�����ܹ�������׳��쳣
// 2.�ܹ�����Windows�ṹ���쳣,��ת��ΪC++�쳣
// 3.���κ��쳣�׳�ʱ(�����Ƿ�������),�ܼ�¼���ö�ջ,�Ĵ���״̬����Ϣ

/* ��׼�÷�: 
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, INT nCmdShow)
{
	INT nResult = -1;
	_set_se_translator( SEH_Translation );

	try
	{
		CApp app;
		app.Initialize(hInst, lpCmdLine, nCmdShow);
		nResult = app.MainLoop();
	}
	catch(CWinException)
	{
	}
	return nResult;
}*/

// �������ֶ��׳��쳣ʱ
// throw( "your exception's description" );



//-----------------------------------------------------------------------------

class CWinLog;

typedef DWORD64   DWORD3264;

class CWinException
{
public:

	CWinException(LPTSTR szMsg, LPSTR szFileName, INT nLineNum);
	CWinException(const std::string&, LPSTR szFileName, INT nLineNum);
	
	CWinException(EXCEPTION_POINTERS* pExp,UINT unCode = 0 );
	~CWinException();

public:  //�ṩһЩͨ�õĺ���
	//�׳�assert������Ϣ
	 static void ThrowAssert(LPTSTR);
	 //�׳���Ϣ������̨
	 static void ThrowMsg(LPSTR);

	 TCHAR	m_szPath[1024];

private:

	enum EBasicType  // ���� CVCONST.H (Microsoft DIA 2.0 SDK)
	{
		EBT_NoType = 0,
		EBT_Void = 1,
		EBT_Char = 2,
		EBT_WChar = 3,
		EBT_Int = 6,
		EBT_UInt = 7,
		EBT_Float = 8,
		EBT_BCD = 9,
		EBT_Bool = 10,
		EBT_Long = 13,
		EBT_ULong = 14,
		EBT_Currency = 25,
		EBT_Date = 26,
		EBT_Variant = 27,
		EBT_Complex = 28,
		EBT_Bit = 29,
		EBT_BSTR = 30,
		EBT_Hresult = 31
	};


	VOID InitDebugHelp();
	BOOL GetFunctionName(LPTSTR szDest, PVOID pFunction);
	
	INT  DumpAll(EXCEPTION_POINTERS *pEx);
	VOID DumpExceptionInfo(EXCEPTION_POINTERS *pEx);	// �쳣��Ϣ
	VOID DumpSystemInfo(EXCEPTION_POINTERS *pEx);		// ��ͨϵͳ��Ϣ
	VOID DumpCallStack(EXCEPTION_POINTERS *pEx);		// ���ö�ջ
	VOID DumpRegisters(EXCEPTION_POINTERS *pEx);		// �Ĵ���״̬
	VOID DumpInstructions(EXCEPTION_POINTERS *pEx);		// �����쳣����ָ��
	VOID DumpMiniDump(EXCEPTION_POINTERS *pEx);			// ����mini-dump
	VOID DumpVariable(EXCEPTION_POINTERS *pEx);			// ����

	VOID DumpExceptionAddress( EXCEPTION_POINTERS *pEx );
	
	static BOOL CALLBACK SymEnumSymbolsProc(PSYMBOL_INFO, ULONG, PVOID);
	static VOID GetFrameData(PSYMBOL_INFO pSym, PVOID pPos, PVOID pContext);


	static BOOL FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME * sf);
	static BOOL DumpTypeIndex(DWORD64, DWORD, DWORD3264, INT nCycle);
	static VOID FormatOutputValue(EBasicType, DWORD64, PVOID);
	static EBasicType GetBasicType( DWORD, DWORD64);

	static UINT			m_unExceptionCode;
	static HANDLE		m_hProcess;
};

static void SEH_Translation(unsigned int unCode, EXCEPTION_POINTERS* pExp)
{
#pragma warning(disable : 4297)

	throw CWinException(pExp,unCode);

#pragma warning(default : 4297)
}

static LONG WINAPI TopLevelFilter(EXCEPTION_POINTERS* pException)
{
	throw(CWinException(pException));
	return EXCEPTION_EXECUTE_HANDLER;
}



static void myInvalidParameterHandler(const wchar_t* expression,
									  const wchar_t* function, 
									  const wchar_t* file, 
									  unsigned int line, 
									  uintptr_t pReserved)
{
	RaiseException(EXCEPTION_BREAKPOINT,0,0,NULL);
}


#pragma warning(pop)

#endif