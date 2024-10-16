#pragma once


// CEularEditDlg dialog

class CEularEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CEularEditDlg)

public:
	CEularEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEularEditDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG_EULAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_fRoll;
	float m_fYaw;
	float m_fPitch;
};
