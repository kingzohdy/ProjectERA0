#pragma once


// CNewItemDlg dialog

class CNewItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewItemDlg)

public:
	CNewItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewItemDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEWITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_iItemID;
	CString m_ItemName;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
