#pragma once
#include "afxwin.h"


// CProcEditDlg dialog

class CProcEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcEditDlg)

public:
	CProcEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcEditDlg();

	void GetDataFromUI();
	void SetDataToUI();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROCEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditLinkMapid();
	afx_msg void OnEnChangeEditLinkPosx();
	afx_msg void OnEnChangeEditLinkPosy();
	afx_msg void OnEnChangeEditLinkDir();
	afx_msg void OnEnChangeEditBackPosx();
	afx_msg void OnEnChangeEditBackPosy();
	afx_msg void OnEnChangeEditBackDir();
	int m_LinkMapID;
	int m_LinkPosX;
	int m_LinkPosY;
	int m_LinkDir;
	int m_BackPosX;
	int m_BackPosY;
	int m_BackDir;
	afx_msg void OnBnClickedButtonGetPreviewdir();
	int m_PreviewActorDir;
	int m_PreviewCameraDir;
	int m_LinkCameraDir;
	CEdit m_LinkScript;
	afx_msg void OnEnChangeEditLinkScript();
	int m_PreviewActorX;
	int m_PreviewActorY;

	bool m_bInit;

	virtual BOOL OnInitDialog();
};
