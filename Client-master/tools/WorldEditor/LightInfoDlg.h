#pragma once
#include "afxwin.h"


// CLightInfoDlg dialog

class CLightInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightInfoDlg)

public:
	CLightInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightInfoDlg();

	void InitData();
	void UpdatePos();

// Dialog Data
	enum { IDD = IDD_DIALOG_LIGHT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBCGPColorButton m_LightColorBtn;
	afx_msg void OnEnChangeEditLightX();
	afx_msg void OnEnChangeEditLightY();
	afx_msg void OnEnChangeEditLightZ();
	afx_msg void OnBnClickedButtonLightColor();
	afx_msg void OnEnChangeEditLightRange();
	afx_msg void OnCbnSelchangeComboLightType();
	CComboBox m_LightTypeCombo;
	virtual BOOL OnInitDialog();
};
