#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTerrainLayerDlg dialog

class CTerrainLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CTerrainLayerDlg)

public:
	CTerrainLayerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTerrainLayerDlg();

	void SelectLayer(int sel);
	void SelectPaintShape(int mode);

// Dialog Data
	enum { IDD = IDD_TERRAIN_TEXTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTexshape();
	afx_msg void OnCbnSelchangeComboTexPaintshape();
	afx_msg void OnEnChangeEditTexbrushSize();
	afx_msg void OnEnChangeEditTexbrushStrength();
	afx_msg void OnEnChangeEditPlantScale();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedRadioLayer1();
	afx_msg void OnBnClickedRadioLayer2();
	afx_msg void OnBnClickedRadioLayer3();
	afx_msg void OnBnClickedRadioLayer4();
	afx_msg void OnBnClickedRadioLayer5();
	afx_msg void OnBnClickedRadioLayer6();
	afx_msg void OnBnClickedRadioLayer7();
	afx_msg void OnBnClickedRadioLayer8();
	afx_msg void OnBnClickedRadioLayer9();
	afx_msg void OnNMReleasedcaptureSliderTexbrushSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderTexbrushStrength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderPlantScale(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioPaintshapeCircle();
	afx_msg void OnBnClickedRadioPaintshapeRect();
	afx_msg void OnBnClickedRadioPaintshapeRandomcircle();
	afx_msg void OnBnClickedRadioPaintshapeRandomrect();
	CSliderCtrl m_BrushSizeSlider;
	CSliderCtrl m_BrushStrengthSlider;
	CSliderCtrl m_PlantScaleSlider;
	afx_msg void OnNMCustomdrawSliderTexbrushSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderTexbrushStrength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderPlantScale(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioLayer10();
	afx_msg void OnBnClickedImportWalkbmp();
};
