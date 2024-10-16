#pragma once
#include "afxwin.h"


// CLightEditDlg dialog

class CLightEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightEditDlg)

public:
	CLightEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightEditDlg();

	void UpdateLightList();
	void OnSelectLight(int sel);
	void OnDeleteLight(int sel);

// Dialog Data
	enum { IDD = IDD_LIGHT_EDIT };

private:
	void setLightSelectedInList(int sel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedButtonLoadlights();
	afx_msg void OnBnClickedCheckLightLockheight();
	afx_msg void OnBnClickedCheckRenderLightmodels();
	afx_msg void OnBnClickedCheckRenderLights();
	CListBox m_LightList;
	CButton m_RenderLights;
	CButton m_RenderLightModels;
	CButton m_LockHeight;
	CComboBox m_EffectEditMode;
	afx_msg void OnCbnSelchangeComboEffectEditmode();
	CBCGPColorButton m_LightColorBtn;
	CEdit m_EffectPosX;
	CEdit m_EffectPosY;
	CEdit m_EffectPosZ;
	CEdit m_EffectRange;
	CEdit m_EffectVolume;
	CEdit m_EffectName;
	CEdit m_EffectResPath;
	afx_msg void OnLbnSelchangeListLights();
	afx_msg void OnBnClickedButtonEffectPath();
	afx_msg void OnBnClickedButtonEffectColor();
	afx_msg void OnEnChangeEditEffectRange();
	afx_msg void OnEnChangeEditSoundVolume();
	afx_msg void OnEnChangeEditEffectName();
	afx_msg void OnEnChangeEditEffectRespath();
	afx_msg void OnEnChangeEditEffectX();
	CEdit m_SoundTime0;
	CEdit m_SoundTime1;
	afx_msg void OnEnChangeSoundtime0();
	afx_msg void OnEnChangeSoundtime1();
	afx_msg void OnEnChangeFullrange();
	CEdit mSoundFullRange;
	afx_msg void OnBnClickedSoundisloop();
	CButton m_CheckIsLoop;
};
