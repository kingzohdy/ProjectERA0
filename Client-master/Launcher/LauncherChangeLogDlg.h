#pragma once
#include <atlimage.h>
#include "BtnSt.h"
#include "webbrowser.h"
// #include "skinscrollbar.h"
#include "afxwin.h"

// LauncherChangeLogDlg dialog

class LauncherChangeLogDlg : public CDialog
{
	DECLARE_DYNAMIC(LauncherChangeLogDlg)

public:
	LauncherChangeLogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LauncherChangeLogDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHANGE_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CImage m_imgOK[3];
	CButtonST m_OKBtn;

	CImage m_imgClose[3];
	CButtonST m_CloseBtn;

	CWebBrowser2 m_web;
	CScrollBar m_scroll;

	CImage		m_bkgImg;
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonClose();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:

	string					m_strVer;
	string					m_strNewVer;

	afx_msg void OnPaint();
};
