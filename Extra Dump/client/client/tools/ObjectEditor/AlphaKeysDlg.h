#pragma once
#include "OgreKeyFrameArray.h"

// CAlphaKeysDlg dialog

class CAlphaKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CAlphaKeysDlg)

public:
	CAlphaKeysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAlphaKeysDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ALPHAKEYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_wndList;
	CSliderCtrl m_sldAlpha;
	float m_fAlpha;

	void UpdateList();
	void UpdatePane();
	void SaveCurKf();

	void SetCurKf(int index);
	int GetCurKf();

	int m_curkf;
	bool m_bNeedRefresh;
	Ogre::KeyFrameArray<float> m_alphakeys;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnEnChangeEditAlpha();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelete();
};
