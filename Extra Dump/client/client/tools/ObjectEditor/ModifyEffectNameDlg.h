#pragma once


// CModifyEffectNameDlg dialog

class CModifyEffectNameDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CModifyEffectNameDlg)

public:
	CModifyEffectNameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyEffectNameDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EFFECTNAME };

	void SetTitle(CString str);
	void SetDesc(CString str);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	CEdit m_editDesc;
	virtual BOOL OnInitDialog();

protected:
	CString m_strTitle;
	CString m_strDesc;
};
