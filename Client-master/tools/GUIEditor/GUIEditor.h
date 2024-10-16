// GUIEditor.h : main header file for the GUIEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#pragma comment(lib, "winmm.lib")
#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "UILib_d.lib")
#pragma comment(lib, "tolua++_d.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#pragma comment(lib, "UILib.lib")
#pragma comment(lib, "tolua++.lib")
#endif

#pragma comment(lib, "Imm32.lib")
// CGUIEditorApp:
// See GUIEditor.cpp for the implementation of this class
//

class CGUIEditorApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CGUIEditorApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
public:
};

extern CGUIEditorApp theApp;