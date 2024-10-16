#pragma once
#include "afxwin.h"


// CFindDialog dialog

class CFindDialog : public CDialog
{
	DECLARE_DYNAMIC(CFindDialog)

public:
	CFindDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindDialog();

// Dialog Data
	enum { IDD = IDD_FIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFind();
	CString m_FindName;
	CListBox m_ModeList;
	afx_msg void OnLbnSelchangeModellist();
	afx_msg void OnBnClickedPaste();
};
