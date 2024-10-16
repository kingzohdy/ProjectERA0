#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "CurveWnd.h"


// CHeightEditDlg dialog

class CHeightEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CHeightEditDlg)

public:
	CHeightEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHeightEditDlg();

// Dialog Data
	enum { IDD = IDD_HEIGHTMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditHmapBrushsize();
	afx_msg void OnEnChangeEditHmapBrushstrength();
	afx_msg void OnBnClickedButtonHmapSmooth();
	afx_msg void OnNMReleasedcaptureSliderHmapBrushsize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderHmapBrushstrength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioHmapcurveSoft();
	afx_msg void OnBnClickedRadioHmapcurveHard();
	afx_msg void OnBnClickedRadioHmapcurveSmooth();
	afx_msg void OnBnClickedRadioHmapcurveFlat();
	afx_msg void OnBnClickedRadioHmapcurveSlope();
	afx_msg void OnBnClickedRadioHmapcurveDitch();
	afx_msg void OnBnClickedRadioHmaptrimNone();
	afx_msg void OnBnClickedRadioHmaptrimTrim();
	afx_msg void OnBnClickedRadioHmaptrimRecover();
	afx_msg void OnBnClickedRadioHmapLift();
	afx_msg void OnBnClickedRadioHmapLower();
	CSliderCtrl m_BrushSizeSlider;
	CSliderCtrl m_BrushStrengthSlider;
	afx_msg void OnNMCustomdrawSliderHmapBrushsize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderHmapBrushstrength(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CCurveWnd m_wndCurveWnd;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
private:
	CComboBox m_PointMode;
public:
	afx_msg void OnCbnSelchangePointMode();
	afx_msg void OnBnClickedRadioHmapcurveCurve();
};
