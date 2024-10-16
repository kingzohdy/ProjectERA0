#pragma once
#include "afxcmn.h"
#include "OgreAmbientRegion.h"
#include "afxwin.h"


// CEnvironmentEditDlg dialog

class CEnvironmentEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CEnvironmentEditDlg)

public:
	CEnvironmentEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnvironmentEditDlg();

	void GetDataFromUI();
	void SetDataToUI();

// Dialog Data
	enum { IDD = IDD_ENVIRONMENT_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool m_bInit;

	CBCGPColorButton m_DiffuseColor;
	CBCGPColorButton m_SpecularColor;
	CBCGPColorButton m_AmbientColorBtn;
	CBCGPColorButton m_TopLightColor;
	CButton m_DistFogEnable;
	CSliderCtrl m_DistFogNear;
	CSliderCtrl m_DistFogFar;
	CBCGPColorButton m_DistFogColor;
	CButton m_HeightFogEnable;
	CButton m_ShadowEnable;
	CSliderCtrl m_Shadowdensity;
	CSliderCtrl m_HeightFogFar;
	CSliderCtrl m_HeightFogNear;
	CSliderCtrl m_BloomExposure;
	CSliderCtrl m_Bloomamp;
	CSliderCtrl m_Bloomwidth;
	CBCGPColorButton m_HeightFogColor;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAmbientColor();
	afx_msg void OnBnClickedBtnDiffuseColor();
	afx_msg void OnBnClickedBtnSpecularColor();
	afx_msg void OnBnClickedCheckDistfogEnable();
	afx_msg void OnBnClickedCheckHeightfogEnable();
	afx_msg void OnBnClickedBtnDistfogColor();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheckBigLock();
	afx_msg void OnNMReleasedcaptureSliderDirHorizon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDirVertical(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_DirHorizonSlider;
	CSliderCtrl m_DirVerticalSlider;
	afx_msg void OnNMReleasedcaptureSliderDistfogNear(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDistfogFar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderHeightfogFar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderHeightfogNear(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnHeightfogColor();
	afx_msg void OnBnClickedBtnSkymodel();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedBtnBgmusic();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCameraeffect();
	afx_msg void OnBnClickedBtnToplightColor();
	CComboBox m_ToplightShadowType;
	afx_msg void OnCbnSelchangeComboToplightShadow();
	afx_msg void OnEnChangeEditMusicvolume();
	CEdit m_MusicVolEdit;
	afx_msg void OnBnClickedBtnBgsound();
	afx_msg void OnEnChangeEditBgsoundvol();
	CEdit m_BGSoundVolEdit;
	afx_msg void OnBnClickedBtnSkymodelClear();
	afx_msg void OnBnClickedBtnBgmusicClear();
	afx_msg void OnBnClickedBtnBgsoundClear();
	afx_msg void OnBnClickedBtnCameraeffectClear();
	afx_msg void OnBnClickedBtnSaveAmbient();
	afx_msg void OnBnClickedBtnLoadambient();
	afx_msg void OnNMCustomdrawSliderHeightfogFar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderGrass(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	CSliderCtrl m_GrassLod;
};
