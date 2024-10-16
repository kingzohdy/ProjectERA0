#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CModelInfoDlg dialog

class CModelInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelInfoDlg)

public:
	CModelInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModelInfoDlg();

	void InitData();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODELINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_PitchSpin;
	CSpinButtonCtrl m_YawSpin;
	CSpinButtonCtrl m_RollSpin;
	CSpinButtonCtrl m_ScaleSpin;
	CComboBox m_TypeList;
	CComboBox m_AnimList;
	CEdit m_PathEdit;
	afx_msg void OnEnChangeEditModelinfoX();
	afx_msg void OnEnChangeEditModelinfoY();
	afx_msg void OnEnChangeEditModelinfoZ();
	afx_msg void OnEnChangeEditModelinfoPitch();
	afx_msg void OnEnChangeEditModelinfoYaw();
	afx_msg void OnEnChangeEditModelinfoRoll();
	afx_msg void OnEnChangeEditModelinfoScale();
	afx_msg void OnCbnSelchangeComboModelinfoType();
	afx_msg void OnCbnSelchangeComboModelinfoAnim();
	virtual BOOL OnInitDialog();

protected:
	void SavePosData();
	CString m_CurAnimName;
	bool m_bDataInit;
public:
	CButton m_ReflectedBtn;
	CButton m_RefractedBtn;
	afx_msg void OnBnClickedCheckReflected();
	afx_msg void OnBnClickedCheckRefracted();
	afx_msg void OnDeltaposSpinModelinfoPitch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinModelinfoYaw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	CButton m_TransableBtn;
	afx_msg void OnBnClickedCheckTransable();
};
