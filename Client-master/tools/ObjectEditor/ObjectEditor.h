// ObjectEditor.h : ObjectEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CObjectEditorApp:
// �йش����ʵ�֣������ ObjectEditor.cpp
//

class CObjectEditorApp : public CWinApp, public CBCGPWorkspace
{
public:
	CObjectEditorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
};

extern CObjectEditorApp theApp;