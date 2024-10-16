
#include "stdafx.h"
#include "maxplugin.h"
#include "export.h"
#include "stdmat.h"
#include "OgreRoot.h"
#include "exp_anim.h"

using namespace Ogre;

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\lib\\OgreMain_d.lib")
#else
#pragma comment(lib, "..\\..\\lib\\OgreMain.lib")
#endif

static int controlsInit = FALSE;
static Root *s_pRoot;


BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	if ( !controlsInit ) {
		controlsInit = TRUE;

		// jaguar controls
		InitCustomControls(hinstDLL);

		// initialize Chicago controls
		InitCommonControls();

		// register SXP readers
		RegisterSXPReader(_T("MARBLE_I.SXP"), Class_ID(MARBLE_CLASS_ID,0));
		RegisterSXPReader(_T("NOISE_I.SXP"),  Class_ID(NOISE_CLASS_ID,0));
		RegisterSXPReader(_T("NOISE2_I.SXP"), Class_ID(NOISE_CLASS_ID,0));
	}

	switch (fdwReason) 
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = hinstDLL;
		InitCustomControls(hinstDLL);
		InitCommonControls();

		s_pRoot = new Root("ogre_maxplugin.cfg");
		s_pRoot->Initlize();
		s_pRoot->initRenderSystem(NULL);
		DKEXP::LoadExpOptionFromFile("plugins\\DarkPlugin.cfg");
		break;
	case DLL_PROCESS_DETACH:
		delete s_pRoot;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:

		break;
	}

	return (TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESC);
}

/// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS
__declspec( dllexport ) int LibNumberClasses()
{
	return 2;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) {
		case 0: return DKEXP::GetMaxPluginDesc();
		case 1: return DKEXP::GetDarkExportDesc();
		default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

