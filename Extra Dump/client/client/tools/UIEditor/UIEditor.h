// UIEditor.h : main header file for the UIEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CUIEditorApp:
// See UIEditor.cpp for the implementation of this class
//

class CUIEditorApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CUIEditorApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUIEditorApp theApp;