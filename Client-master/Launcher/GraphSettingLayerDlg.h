#pragma once
#include "GraphSetting.h"


// CGraphSettingDlg dialog

class CGraphSettingLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphSettingLayerDlg)

public:
	CGraphSettingLayerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphSettingLayerDlg();

// Dialog Data
	enum { IDD = IDD_LAUNCHERLAYER_DIALOG };

	CGraphSetting	m_dlg;
	CImage			m_imgBack;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	void			SetAlphaWindow();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
public:
	virtual void OnCancel();
};
