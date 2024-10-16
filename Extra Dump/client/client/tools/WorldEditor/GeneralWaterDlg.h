#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CGeneralWaterDlg dialog

class CGeneralWaterDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeneralWaterDlg)

public:
	CGeneralWaterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGeneralWaterDlg();

// Dialog Data
	enum { IDD = IDD_GENERAL_WATER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void UpdateToTerrainEdit();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_BaseTexSelect;
	CSliderCtrl m_TexRepeatSlider;
	CBCGPColorButton m_WaterColor1;
	CBCGPColorButton m_WaterColor2;
	int m_WaterAlpha1;
	int m_WaterAlpha2;
	float m_WaterDepth1;
	float m_WaterDepth2;

	afx_msg void OnEnChangeEditWateralpha2();
	afx_msg void OnBnClickedBtnWatercolor1();
	afx_msg void OnBnClickedBtnWatercolor2();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboWatertexture();
	afx_msg void OnEnChangeEditWateralpha1();
	afx_msg void OnEnChangeEditWaterdepth1();
	afx_msg void OnEnChangeEditWaterdepth2();
	afx_msg void OnNMReleasedcaptureSliderWatertexrepeat(NMHDR *pNMHDR, LRESULT *pResult);
};
