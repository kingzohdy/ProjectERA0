#pragma once


// CMergeMotionDlg dialog

class CMergeMotionDlg : public CDialog
{
	DECLARE_DYNAMIC(CMergeMotionDlg)

public:
	CMergeMotionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMergeMotionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MERGEMOTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bNoDelete;
	CString		m_strFromMotion;
	int			m_iToMotion;
	CComboBox m_cmbMotionList;
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
