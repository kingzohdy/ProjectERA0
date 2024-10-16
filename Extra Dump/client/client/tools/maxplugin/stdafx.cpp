#include "stdafx.h"

HINSTANCE g_hInstance;
ExportOption g_ExpOption;
Interface *g_Interface;


TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (g_hInstance)
		return LoadString(g_hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

const char *GetMaxRootDir()
{
	/*
#if _MSC_VER < 1400
	return GetMaxRootDir();
#else
	return GetCOREInterface()->GetDir(APP_MAX_SYS_ROOT_DIR);
#endif
	*/
	return GetCOREInterface()->GetDir(APP_MAXROOT_DIR);
}