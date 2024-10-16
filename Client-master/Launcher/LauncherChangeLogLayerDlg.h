#pragma once
#include "LauncherChangeLogDlg.h"


// LauncherChangeLogLayerDlg dialog

class LauncherChangeLogLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(LauncherChangeLogLayerDlg)

public:
	LauncherChangeLogLayerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LauncherChangeLogLayerDlg();

	void setVersionInfo( const string& ver, const string& newVer );
// Dialog Data
	enum { IDD = IDD_DIALOG_CHANGE_LOG_LAYER };

private:
	LauncherChangeLogDlg	m_dlg;
	CImage					m_imgBack;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void			SetAlphaWindow();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	virtual void OnOK();
};
