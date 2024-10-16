#pragma once
#include "afxcmn.h"


// CBoardDialog dialog

class CBoardDialog : public CDialog
{
	DECLARE_DYNAMIC(CBoardDialog)

public:
	CBoardDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBoardDialog();

// Dialog Data
	enum { IDD = IDD_BOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT onNetNotify(WPARAM wParam, LPARAM lParam);
	CString m_cstrBroadcastMsg;
};
