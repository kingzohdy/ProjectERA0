// BCGPDurationCtrl.h: interface for the CBCGPDurationCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPDURATIONCTRL_H__4AD6FDCB_64F2_4906_B871_7A882DEDB32E__INCLUDED_)
#define AFX_BCGPDURATIONCTRL_H__4AD6FDCB_64F2_4906_B871_7A882DEDB32E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPButton.h"
#include "BCGPSpinButtonCtrl.h"

#define PARTS_NUM	6

class BCGCBPRODLLEXPORT CBCGPDurationCtrl : public CButton  
{
public:
	UINT GetState () const;
	void SetState (UINT flags, UINT mask);

	COleDateTimeSpan	GetDuration() const;
	void SetDuration(const COleDateTimeSpan& timeSpan);

	CBCGPDurationCtrl();
	virtual ~CBCGPDurationCtrl();

	static const UINT DRTN_DAYS;
	static const UINT DRTN_HOURS_MINS;
	static const UINT DRTN_SPIN;

protected:
	enum PART_TYPE
	{
		UNDEFINED_PART = -1,
		DAYS,
		DAYS_LABEL,
		HOURS,
		HOURS_LABEL,
		MINUTES,
		MINUTES_LABEL
	};

// Operations:
protected:
	void AdjustLayout ();
	void GetPartSize (const CString& strText, CSize& size);
	void SelectPart (int iNewSel);
	void SelectNext ();
	void SelectPrev ();
	void PushDigit (int iDigit);
	void ScrollCurrPart (int iDelta);

protected:
	BOOL				m_spinButton;
	BOOL				m_bShowDays;
	BOOL				m_bShowHoursMinutes;

	CString				m_strDaysLabel;
	CString				m_strHoursLabel;
	CString				m_strMinutesLabel;
	CBCGPSpinButtonCtrl	m_wndSpin;

	PART_TYPE			m_CurrPartType;
	CString				m_strTimeSeparator;
	int					m_iPrevDigit;
	int					m_iSelectedPart;

	CRect				m_rectParts [PARTS_NUM];

	int					m_lDays;
	int					m_lHours;
	int					m_lMinutes;

	int					m_iControlWidth;
	int					m_iControlHeight;

	PART_TYPE	GetPartType(const CPoint& pt);

// Overrides
protected:
	virtual void OnDurationChanged ();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPDurationCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPDurationCtrl)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_BCGPDURATIONCTRL_H__4AD6FDCB_64F2_4906_B871_7A882DEDB32E__INCLUDED_)
