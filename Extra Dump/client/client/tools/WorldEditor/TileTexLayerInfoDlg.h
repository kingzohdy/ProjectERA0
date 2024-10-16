#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTileTexLayerInfoDlg dialog

class CTileTexLayerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CTileTexLayerInfoDlg)

public:
	CTileTexLayerInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTileTexLayerInfoDlg();

	void InitData();

// Dialog Data
	enum { IDD = IDD_DIALOG_TILETEX_LAYERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditTexrepeatLayer1();
	afx_msg void OnEnChangeEditTexrepeatLayer2();
	afx_msg void OnEnChangeEditTexrepeatLayer3();
	afx_msg void OnEnChangeEditTexrepeatLayer4();
	CSpinButtonCtrl m_TexRepeatSpin[4];
	int m_BakTexRepeat[4];
protected:
	virtual void OnCancel();
public:
	CEdit m_TexNameLayer1;
	CEdit m_TexNameLayer2;
	CEdit m_TexNameLayer3;
	CEdit m_TexNameLayer4;
	afx_msg void OnDeltaposSpinTexrepeatLayer2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditTexnameShadow();
	CEdit m_ShadowName;
};
