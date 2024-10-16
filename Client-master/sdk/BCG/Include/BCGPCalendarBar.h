// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.

#if !defined(AFX_BCGPCALENDARBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
#define AFX_BCGPCALENDARBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPCalendarBar.h : header file
//

#ifndef BCG_NO_CALENDAR

#include "BCGCBPro.h"
#include "BCGPPopupMenuBar.h"

class CBCGPDateTimeCtrl;
class CCalendarWeekButton;
class CCalendarWeekNumButton;
class CCalendarTodayButton;
class CBCGCalendarExCtrl;

/////////////////////////////////////////////////////////////////////////////
// CBCGPCalendarBar window

#define HMARGINS 2 // in pixels
#define VMARGINS 2 // in pixels

class BCGCBPRODLLEXPORT CBCGPCalendarBar : public CBCGPPopupMenuBar
{
	friend class CBCGCalendarExCtrl;
	friend class CCalendarPopup;
	friend class CBCGPCalendarMenuButton;
	friend class CBCGPDateTimeCtrl;
	friend class CBCGPWeekButton;
	friend class CBCGPCalendarButton;
	friend class CCalendarWeekButton;
	friend class CCalendarWeekNumButton;
	friend class CBCGPCalendarProp;
	friend class CCalendarTodayButton;
	friend class CCalendarMonthButton;
	friend class CCalendarNavigateButton;

	DECLARE_SERIAL(CBCGPCalendarBar)

// Construction
public:

	CBCGPCalendarBar();

	virtual BOOL Create(
			CWnd*		pParentWnd,
			DWORD		dwStyle,
			UINT		nID,
			BOOL		enableSelection
			);

	virtual BOOL CreateControl (
			CWnd*			pParentWnd,
			const CRect&	rect,
			UINT			nID,
			BOOL			enableSelection = FALSE,
			DWORD			dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER
		);

protected:

	enum SelectionMODE
	{
		Select_BYNONE,
		Select_BYDAYS,
		Select_BYWEEKS,
		Select_BYWEEKDAYS
	};
	
	enum CommandTYPE
	{
		Click_NOWHERE,
		Click_DAY,
		Click_WEEKDAY,
		Click_WEEKNO,
		Click_PREVYEAR,
		Click_PREVMONTH,
		Click_NEXTMONTH,
		Click_NEXTYEAR
	};

	void TranslateSelectionMode(SelectionMODE newMode);

	CBCGPCalendarBar(
					const COleDateTime& month, 
					UINT nCommandID,
					CBCGPDateTimeCtrl* pParentBtn);

	CBCGPCalendarBar (CBCGPCalendarBar& src, UINT uiCommandID, BOOL enableSelection);

// Operations:
public:

	void ContextToSize ();

// Attributes
public:
	void SetDate (const COleDateTime& date);
	COleDateTime GetDate () const
	{
		return m_dateSelected;
	}

	void SetFirstDayOfWeek (int nDay);	// 0-6
	int GetFirstDayOfWeek () const
	{
		return m_nWeekStart - 1;
	}

	const CList<int,int>& GetSelectedDays () const
	{
		return m_lstSelectedDays;
	}
	void SelectDays (const CList<int,int>& lstDays, BOOL bRedraw = TRUE);

	const CList<int,int>& GetMarkedDays () const
	{
		return m_lstMarkedDays;
	}
	void MarkDays (const CList<int,int>& lstDays, BOOL bRedraw = TRUE);

	UINT GetCommandID () const
	{
		return m_nCommandID;
	}

	void SetCommandID (UINT nCommandID)
	{
		m_nCommandID = nCommandID;
	}

	BOOL IsTearOff () const
	{
		return m_bIsTearOff;
	}

	UINT GetState() const
	{
		return m_styleFlags;
	}

	void SetState(UINT flags, UINT mask);

	void SetVertMargin (int nVertMargin);
	void SetHorzMargin (int nHorzMargin);

	int GetVertMargin () const
	{
		return m_nVertMargin;
	}

	int GetHorzMargin () const
	{
		return m_nHorzMargin;
	}

	static const UINT CBR_WEEKDAYSEL;
	static const UINT CBR_WEEKNUMBER;
	static const UINT CBR_MULTISELECTION;
	static const UINT CBR_ENABLED;
	static const UINT CBR_NAVIGATION_BUTTONS;

protected:

	SelectionMODE		m_selMode;

//layout
	int					m_iTitleHeight;
	int					m_nWeekStart;

//Attributes	
	UINT				m_styleFlags;

	int					m_iStartMultiplySelection;
	int					m_iLastMultiplySelection;

	CList<int,int>		m_lstSelectedDays;
	CList<int,int>		m_lstMarkedDays;

//other values

	CSize					m_sizeBox;
	CSize					m_sizeBoxMin;
	COleDateTime			m_dateSelected;
	COleDateTime			m_LastAddedDate;
	UINT					m_nCommandID;
	BOOL					m_bIsEnabled;
	BOOL					m_bIsTearOff;
	CBCGPDateTimeCtrl*		m_pParentBtn;
	BOOL					m_bInternal;	// Used in customization
	int						m_nVertMargin;
	int						m_nHorzMargin;
	int						m_nDaysHorzOffset;

	COLORREF				m_clrNavButton;
	COLORREF				m_clrWeekText;
	COLORREF				m_clrWeekTextDisabled;

	BOOL					m_bIsCtrl;

// Overrides
public:
	virtual void AdjustLocations ();
	virtual CSize CalcSize (BOOL bVertDock);

protected:
	virtual BOOL OnSendCommand (const CBCGPToolbarButton* pButton);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual void OnSelectionChanged ();
	
	virtual BOOL OnKey (UINT nChar);

	virtual void Serialize (CArchive& ar);
	virtual void ShowCommandMessageString (UINT uiCmdId);

	virtual BOOL AllowChangeTextLabels () const
	{
		return FALSE;
	}

	virtual BOOL AllowShowOnList () const
	{	
		return FALSE;
	}

	virtual void Rebuild ();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPCalendarBar)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPCalendarBar();
	virtual void  OnFillBackground (CDC* pDC);

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPCalendarBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	void CommonInit ();

	CSize GetCalendarGridSize (BOOL bVertDock) const;

	void SelectWeekday(const COleDateTime& date, BOOL isCtrlPressed, BOOL isShiftPressed);
	void SelectWeek(const COleDateTime& date, BOOL isCtrlPressed, BOOL isShiftPressed);
	void SelectDay(const COleDateTime& date, BOOL isCtrlPressed, BOOL isShiftPressed);
	void SelectDaySequence(const COleDateTime& date);
	
	void ClearSelectedDays ();
	void AddSelectedDay (int iDay);
	void RemoveSelectedDay (int iDay);
	BOOL IsDaySelected (const COleDateTime& date) const;

	BOOL GetButtonRect (int iID, CRect& rect, BOOL isHitTest = FALSE) const;
	CBCGPCalendarButton* GetDayButton(int iID) const;

	static int GetMaxMonthDay(COleDateTime date);
	static int GetWeekNo(const COleDateTime& date, int weekStart = 1);

private:
	UINT_PTR m_nHoldTimerID;
	int		m_nElapseInterval;

	static CString m_strMonthNames[12];
	static LPCTSTR GetMonthName(int index);

	void OnMonthMenu(UINT id);
	void OnSelectMonth(LPCRECT lpRect);
};

#endif // BCG_NO_CALENDAR

BCGCBPRODLLEXPORT extern UINT BCGM_CALENDAR_ON_SELCHANGED;

// CBCGPCalendarButton

class BCGCBPRODLLEXPORT CBCGPCalendarButton : public CBCGPToolbarButton  
{
	friend class CBCGPCalendarBar;
	friend class CBCGPCalendarMenuButton;

	DECLARE_SERIAL(CBCGPCalendarButton)

public:
	virtual BOOL IsCurrMonth () const;
	virtual BOOL IsHighlight () const
	{
		return m_bHighlight;
	}

	virtual COLORREF GetTextColor ()
	{
		return (COLORREF)-1;
	}

protected:
    CBCGPCalendarButton(const COleDateTime& date, BOOL bHighlight = FALSE);
	CBCGPCalendarButton();

	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
						BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
						BOOL bHighlight = FALSE,
						BOOL bDrawBorder = TRUE,
						BOOL bGrayDisabledButtons = TRUE);

	virtual BOOL OnToolHitTest(const CWnd* /*pWnd*/, TOOLINFO* pTI);
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	virtual void SetDate (const COleDateTime& date);

	virtual BOOL IsSelected() const
	{
		ASSERT_VALID (m_pParentBar);
		return IsActualDay () && m_pParentBar->IsDaySelected(m_Calendar);
	}

	virtual BOOL IsActualDay () const
	{
		return TRUE;
	}

	BOOL				m_isSelected;
	COleDateTime		m_Calendar;
	BOOL				m_bHighlight;
	CBCGPCalendarBar*	m_pParentBar;
};

class CBCGPDefaultLocale
{
public:
	CBCGPDefaultLocale ();
	~CBCGPDefaultLocale ();

protected:
	CString	m_strLanguage;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPCALENDARBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
