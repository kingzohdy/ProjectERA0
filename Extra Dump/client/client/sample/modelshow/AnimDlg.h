#pragma once


// CAnimDlg dialog

class CAnimDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnimDlg)

public:
	CAnimDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_AnimId;
	int GetAnimID(){return m_AnimId;}
	afx_msg void OnBnClickedOk();
};
