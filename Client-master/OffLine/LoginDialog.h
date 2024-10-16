#pragma once
#include "afxwin.h"

#include <vector>
#include <string>

#include "MudNetMgr.h"

// CLoginDialog dialog

class CLoginDialog : public CDialog
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDialog();

// Dialog Data
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT onNetNotify(WPARAM,LPARAM);

public:

protected:

public:
	afx_msg void OnBnClickedUpdate();
	CComboBox m_ServerList;
	CComboBox m_WorldList;
	CString m_cstrPassWorld;
	virtual BOOL OnInitDialog();

	// 默认显示的玩家姓名
	std::string m_DefUserName;
	std::string m_DefServerName;
	std::string m_DefWorldName;

	std::string m_LastUserName;
	std::string m_LastServerName;
	std::string m_LastWorldName;

	afx_msg void OnBnClickedExpand();
private:
	CStatic m_wndSplitter;
	bool m_bExpand;
	CRect m_OriginRect;
public:
	afx_msg void OnCbnSelchangeServerlist();
protected:
	virtual void OnOK();
public:
	afx_msg void OnCbnSelchangeUsername();
	CString m_UserName;
	CComboBox m_UserList;
protected:
	virtual void OnCancel();
};
