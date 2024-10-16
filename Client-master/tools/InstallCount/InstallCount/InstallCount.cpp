// InstallCount.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "InstallCount.h"
#include <Wininet.h>

#pragma comment( lib, "Wininet.lib" )
#pragma comment(lib, "pluginapi.lib")

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

enum
{
	START,
	COMPLETE,
	UNINSTALL,
};

static void count(int type, const char *url);

void StartInstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop)
{
	EXDLL_INIT();
	count(START, 0);
}

void FinishInstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop)
{
	EXDLL_INIT();
	char parameter[200];
	popstring(parameter);
	count(COMPLETE, parameter);
}

int FindClientProc(const char *szPath)
{
	int iLen,iLenP,indx;
	iLenP=strlen(szPath);
	char tmpPath[MAX_PATH];
	strcpy(tmpPath,szPath);
	strupr(tmpPath);
	HINSTANCE hInstLib = LoadLibraryA("PSAPI.DLL");
	if(hInstLib == NULL)
		return 0;
	BOOL (WINAPI *lpfEnumProcesses)( DWORD *, DWORD cb, DWORD * );
	BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *,
		DWORD, LPDWORD );
	DWORD (WINAPI *lpfGetModuleBaseName)( HANDLE, HMODULE,
		LPTSTR, DWORD );
	DWORD (WINAPI *lpGetModuleFileNameEx)(HANDLE, HMODULE,
		LPTSTR, DWORD);

	// Get procedure addresses.
	lpfEnumProcesses = (BOOL(WINAPI *)(DWORD *,DWORD,DWORD*))
		GetProcAddress( hInstLib, "EnumProcesses" ) ;
	lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *,
		DWORD, LPDWORD)) GetProcAddress( hInstLib,
		"EnumProcessModules" ) ;
	lpfGetModuleBaseName =(DWORD (WINAPI *)(HANDLE, HMODULE,
		LPTSTR, DWORD )) GetProcAddress( hInstLib,
		"GetModuleBaseNameA" ) ;
	lpGetModuleFileNameEx = (DWORD (WINAPI *)(HANDLE, HMODULE,
		LPTSTR, DWORD )) GetProcAddress( hInstLib,
		"GetModuleFileNameExA" ) ;

	if(lpfEnumProcesses == NULL ||
		lpfEnumProcessModules == NULL ||
		lpfGetModuleBaseName == NULL ||
		lpGetModuleFileNameEx == NULL )
	{
		FreeLibrary(hInstLib);
		return 0;
	}
	DWORD aiPID[1000],iCb=1000,iCbneeded;
	BOOL bResult=lpfEnumProcesses(aiPID,iCb,&iCbneeded);
	if(!bResult)
	{
		// Unable to get process list, EnumProcesses failed
		FreeLibrary(hInstLib);
		return 0;
	}

	// How many processes are there?
	DWORD iNumProc=iCbneeded/sizeof(DWORD);

	// Get and match the name of each process
	for(int i=0;i<iNumProc;i++)
	{
		// First, get a handle to the process
		HANDLE hProc=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,
			aiPID[i]);
		// Now, get the process name		
		if(hProc)
		{
			HMODULE hMod;
			char szName[MAX_PATH];
			if(lpfEnumProcessModules(hProc,&hMod,sizeof(hMod),&iCbneeded) )
			{
				iLen = lpGetModuleFileNameEx( hProc, hMod, szName, MAX_PATH );
			}			
			sprintf( tmpPath, "%s\\client.exe",szPath );
			if( strcmp( strupr(szName),strupr(tmpPath) ) == 0 )
			{
				CloseHandle(hProc);
				return 1;
			}
			sprintf( tmpPath, "%s\\launcher.exe",szPath );
			if( strcmp( strupr(szName),strupr(tmpPath) ) == 0 )
			{	
				CloseHandle(hProc);
				return 1;
			}
			sprintf( tmpPath,"%s\\offline2.exe",szPath );
			if( strcmp( strupr(szName),strupr(tmpPath) ) == 0 )
			{	
				CloseHandle(hProc);
				return 1;
			}
			sprintf( tmpPath, "%s\\smartdownloader.exe",szPath );
			if( strcmp( strupr(szName),strupr(tmpPath) ) == 0 )
			{
				//kill
				CloseHandle(hProc);
				hProc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,aiPID[i]);
				if(NULL!=hProc)       
				{ 
					TerminateProcess(hProc,0); 
				} 
				CloseHandle(hProc);
			}
		}
		CloseHandle(hProc);
	}
	return 0;
}

void StartUninstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop)
{
	char parameter[200];
	EXDLL_INIT();
	popstring(parameter);
	int ret = FindClientProc( parameter );
	wsprintf(parameter,"%d",ret);
	setuservariable(INST_R0, parameter);
}

void Uninstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop)
{
	EXDLL_INIT();
	char parameter[200];
	popstring(parameter);
	count(UNINSTALL, parameter);
}

void count(int type, const char *url)
{
	switch (type)
	{
	case START:
		//::MessageBox(NULL, version, "开始安装", MB_OK);
		break;

	case COMPLETE:
		{
			HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
			if( it != NULL )
			{
				char szBuf[256];
				sprintf( szBuf,url );
				HINTERNET it2 = InternetOpenUrl( it,szBuf,NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
				InternetCloseHandle(it2);
			}
			InternetCloseHandle(it);
		}
		//::MessageBox(NULL, version, "安装完成", MB_OK);
		break;

	case UNINSTALL:
		//::MessageBox(NULL, version, "卸载", MB_OK);
		HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
		if( it != NULL )
		{
			char szBuf[256];
			sprintf( szBuf,url );
			HINTERNET it2 = InternetOpenUrl( it,szBuf,NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
			InternetCloseHandle(it2);
		}
		InternetCloseHandle(it);
		break;
	}
}
