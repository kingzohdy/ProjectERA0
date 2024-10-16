#pragma once


// CSelectAnimDlg dialog

class CSelectAnimDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectAnimDlg)

public:
	CSelectAnimDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectAnimDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECTANIM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cmbAnim;
	int m_iSel;
protected:
	virtual void OnOK();
public:
	float m_fStart;
	float m_fEnd;
	BOOL m_bOverwrite;
	BOOL m_bNoStart;
	BOOL m_bNoEnd;
	afx_msg void OnBnClickedChkAnselNostart();
	afx_msg void OnBnClickedChkAnselNoend();
	int m_iPriority;
};
