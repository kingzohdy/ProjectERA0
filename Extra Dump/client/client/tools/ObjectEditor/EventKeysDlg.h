#pragma once
#include <vector>

// CEventKeysDlg dialog

class CEventKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventKeysDlg)

public:
	CEventKeysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEventKeysDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EVENTKEYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void RefreshList();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAddevent();
	afx_msg void OnBnClickedBtnDelevent();

	std::vector<CString> m_EventNames;
	CListCtrl m_wndList;
	CEdit m_editNew;
};
