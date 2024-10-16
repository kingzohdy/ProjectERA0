// ObjectEditor.h : ObjectEditor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CObjectEditorApp:
// 有关此类的实现，请参阅 ObjectEditor.cpp
//

class CObjectEditorApp : public CWinApp, public CBCGPWorkspace
{
public:
	CObjectEditorApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
};

extern CObjectEditorApp theApp;