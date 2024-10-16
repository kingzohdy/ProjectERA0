#pragma once
#include "OgreKeyFrameArray.h"


// CZoomKeysDlg dialog

class CZoomKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CZoomKeysDlg)

public:
	CZoomKeysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZoomKeysDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ZOOMKEYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_wndList;
	CSliderCtrl m_sldZoom;
	float m_fZoom;

	void UpdateList();
	void UpdatePane();
	void SaveCurKf();

	void SetCurKf(int index);
	int GetCurKf();

	int m_curkf;
	bool m_bNeedRefresh;
	Ogre::KeyFrameArray<float> m_zoomkeys;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnEnChangeEditZoom();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelete();
};
