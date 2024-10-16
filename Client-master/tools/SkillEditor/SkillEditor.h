// SkillEditor.h : main header file for the SkillEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CSkillEditorApp:
// See SkillEditor.cpp for the implementation of this class
//

class CSkillEditorApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CSkillEditorApp();

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
};

extern CSkillEditorApp theApp;