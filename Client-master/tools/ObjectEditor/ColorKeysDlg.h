#pragma once
#include "OgreColourValue.h"
#include "OgreKeyFrameArray.h"
#include "OgreVector4.h"


// CColorKeysDlg dialog

class CColorKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorKeysDlg)

public:
	CColorKeysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColorKeysDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_COLORKEYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_wndColorFrame;
	CBCGPColorBar	m_wndColor;

	Ogre::ColourValue m_color;
	Ogre::KeyFrameArray<Ogre::Vector4> m_colorkeys;

	virtual BOOL OnInitDialog();
	void UpdateList();
	void UpdatePane();
	void SaveCurKf();

	void SetCurKf(int index);
	int GetCurKf();

	int m_curkf;
	bool m_bNeedRefresh;
protected:
	virtual void OnOK();
public:
	CSliderCtrl m_sldAlpha;
	afx_msg void OnTbMove(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fAlpha;
	afx_msg void OnEnChangeEditAlpha();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CListCtrl m_wndList;
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelete();
};
