#pragma once
#include "LauncherDlg.h"
#include "LauncherFlashDialog.h"

// CLauncherLayerDlg dialog

class CLauncherLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CLauncherLayerDlg)

public:
	CLauncherLayerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLauncherLayerDlg();

// Dialog Data
	enum { IDD = IDD_LAUNCHERLAYER_DIALOG };
	CLauncherDlg	m_dlg;
	CLauncherFlashDialog m_flashDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	HICON			m_hIcon;
	CImage			m_bkgImg;

private:
	void SetAlphaWindow();

	DECLARE_MESSAGE_MAP()

public:
	void OnMove(int x, int y);
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
