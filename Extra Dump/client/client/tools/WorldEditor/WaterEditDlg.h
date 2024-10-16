#pragma once
#include "afxwin.h"
#include "RollupCtrl.h"
#include "GeneralWaterDlg.h"
#include "ReflectWaterDlg.h"
#include "BeachNameDialog.h"
#include "afxcmn.h"


// CWaterEditDlg dialog

class CWaterEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaterEditDlg)

public:
	CWaterEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaterEditDlg();

// Dialog Data
	enum { IDD = IDD_WATER_EDIT };

public:
	void UpdateToTerrainEdit();
	void UpdateWaterData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboWatertype();
	afx_msg void OnEnChangeEditWaterheight();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonWaterEnvmap();
	afx_msg void OnCbnSelchangeComboWatertexture();
	afx_msg void OnNMReleasedcaptureSliderWaterSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderWaterUvrepeat(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderWaterAmp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnWatercolor1();
	afx_msg void OnEnChangeEditWateralpha1();
	afx_msg void OnEnChangeEditWaterdepth1();
	afx_msg void OnBnClickedBtnWatercolor2();
	afx_msg void OnEnChangeEditWateralpha2();
	afx_msg void OnEnChangeEditWaterdepth2();

	bool m_bInit;
	CComboBox m_WaterTypeSelect;
	CComboBox m_AnimTexture;
	CEdit m_EnvMapTexture;
	CSliderCtrl m_SpeedSlider;
	CSliderCtrl m_UVRepeatSlider;
	CSliderCtrl m_AmpSlider;
	CBCGPColorButton m_WaterColor1;
	CEdit m_WaterAlpha1;
	CEdit m_WaterDepth1;
	CBCGPColorButton m_WaterColor2;
	CEdit m_WaterAlpha2;
	CEdit m_WaterDepth2;
	CEdit m_WaterHeight;
	afx_msg void OnNMCustomdrawSliderWaterSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckEditbeach();
	CButton m_CheckBeach;
	afx_msg void OnBnClickedBeachAdd();
	afx_msg void OnBnClickedBeachdelete();
	afx_msg void OnBnClickedBeachCopy();
	afx_msg void OnLbnSelchangeBeachlist();
	CListBox m_BeachList;
	float m_UVSpeed;
	float m_BeachWidth;
	float m_BeachHeight;
	CString m_BeachTexture;
	afx_msg void OnEnChangeBuvspeed();
	afx_msg void OnEnChangeBeachwidth();
	afx_msg void OnEnChangeBeachheight();
	afx_msg void OnBnClickedBeachtextureUse();
	afx_msg void OnBnClickedBtDelete();
	float m_uRepeat;
	CBeachNameDialog m_BeachNameDlg;
private:
	float m_vRepeat;
public:
	afx_msg void OnEnChangeUrepeat();
	afx_msg void OnEnChangeVrepeat();
	float m_fUVRange;
	afx_msg void OnEnChangeBuvrange();
	afx_msg void OnBnClickedBeachmaskUse();
	afx_msg void OnBnClickedBmDelete();
	CString m_maskTextureUse;
	afx_msg void OnEnChangeBeachmask();
	afx_msg void OnEnChangeBtname();
	afx_msg void OnBnClickedRename();
};
