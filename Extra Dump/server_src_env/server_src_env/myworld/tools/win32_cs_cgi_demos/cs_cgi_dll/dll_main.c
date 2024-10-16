#include <windows.h>

#define UNUSED(arg) ((void)(arg))

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	UNUSED(hinstDLL);
	UNUSED(dwReason);
	UNUSED(lpvReserved);
	return TRUE;
}