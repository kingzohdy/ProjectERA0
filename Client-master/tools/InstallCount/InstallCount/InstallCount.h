#ifndef _INSTALLCOUNT_H
#define _INSTALLCOUNT_H

#ifdef INSTALLCOUNT_EXPORTS
#define INSTALLCOUNT_API __declspec(dllexport)
#else
#define INSTALLCOUNT_API __declspec(dllimport)
#endif

#include "stdafx.h"

extern "C"
{
	INSTALLCOUNT_API void	StartInstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop);
	INSTALLCOUNT_API void	FinishInstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop);
	INSTALLCOUNT_API void	StartUninstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop);
	INSTALLCOUNT_API void	Uninstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop);
}

#endif
