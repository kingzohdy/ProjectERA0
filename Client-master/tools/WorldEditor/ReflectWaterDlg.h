#pragma once


// CReflectWaterDlg dialog

class CReflectWaterDlg : public CDialog
{
	DECLARE_DYNAMIC(CReflectWaterDlg)

public:
	CReflectWaterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReflectWaterDlg();

// Dialog Data
	enum { IDD = IDD_REFLECT_WATER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateToTerrainEdit();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	void SetDataToUI();
	CSliderCtrl m_TexRepeatSlider;
	CSliderCtrl m_SpeedSlider;
	CSliderCtrl m_AmpSlider;
	CSliderCtrl m_AlphaSlider;

	CBCGPColorButton m_WaterColor1;
	CBCGPColorButton m_WaterColor2;
	int m_WaterAlpha1;
	int m_WaterAlpha2;
	float m_WaterDepth1;
	float m_WaterDepth2;

	
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnReflectwatercolor1();
	afx_msg void OnEnChangeEditReflectwateralpha1();
	afx_msg void OnEnChangeEditReflectwaterdepth1();
	afx_msg void OnBnClickedBtnReflectwatercolor2();
	afx_msg void OnEnChangeEditReflectwateralpha2();
	afx_msg void OnEnChangeEditReflectwaterdepth2();
};
