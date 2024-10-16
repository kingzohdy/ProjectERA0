#pragma once


// CSelectSkillDlg dialog

class CSelectSkillDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectSkillDlg)

public:
	CSelectSkillDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectSkillDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECTSKILL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	unsigned int m_iSelId;
	CListCtrl m_wndList;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
