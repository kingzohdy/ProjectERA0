#pragma once


// CBeachNameDialog dialog

class CBeachNameDialog : public CDialog
{
	DECLARE_DYNAMIC(CBeachNameDialog)

public:
	CBeachNameDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBeachNameDialog();

// Dialog Data
	enum { IDD = IDD_BEACHNAMEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CString m_BeachName;
public:
	afx_msg void OnEnChangeEditBeachname();
	afx_msg void OnBnClickedBeachnameok();
};
