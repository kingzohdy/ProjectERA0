#pragma once
#include "webbrowser.h"


// CLauncherFlashDialog dialog

class CLauncherFlashDialog : public CDialog
{
	DECLARE_DYNAMIC(CLauncherFlashDialog)

public:
	CLauncherFlashDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLauncherFlashDialog();

// Dialog Data
	enum { IDD = IDD_LAUNCHER_FLASH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	CWebBrowser2 m_web;
};
