#pragma once


// CAnimFilesDlg dialog

class CAnimFilesDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnimFilesDlg)

public:
	CAnimFilesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimFilesDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ANIMFILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl	m_wndList;
	CString		m_strSel;
protected:
	virtual void OnOK();
};
