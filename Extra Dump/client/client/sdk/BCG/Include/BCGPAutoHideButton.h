// BCGPAutoHideButton.h: interface for the CBCGPAutoHideButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPAutoHideBUTTON_H__582BE570_F465_4330_AA95_7DFF1250F0FC__INCLUDED_)
#define AFX_BCGPAutoHideBUTTON_H__582BE570_F465_4330_AA95_7DFF1250F0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBCGPAutoHideToolBar;
class CBCGPDockingControlBar;

#include "BCGCBPro.h"
#include "BCGPAutoHideToolBar.h"

class BCGCBPRODLLEXPORT CBCGPAutoHideButton : public CObject  
{
	DECLARE_DYNCREATE(CBCGPAutoHideButton)
public:
	CBCGPAutoHideButton();
	virtual ~CBCGPAutoHideButton();

	virtual BOOL Create (CBCGPAutoHideToolBar* pParentBar, 
						 CBCGPDockingControlBar* pAutoHideWnd, DWORD dwAlignment);

	void ShowAttachedWindow (BOOL bShow);

	void BringToTop () {m_bTop = TRUE;}
	void Move (int nOffset);
	void SetOffset (int rectOffset); 

	virtual void ShowButton (BOOL bShow); 
	virtual BOOL IsVisible () const {return m_bVisible;}

	BOOL IsTop () const {return m_bTop;}
	BOOL IsActive () const
	{
		return m_pParentBar != NULL && m_pParentBar->m_bActiveInGroup;
	}

	virtual void OnDraw (CDC* pDC);
	virtual void OnFillBackground (CDC* pDC, CRect rect);
	virtual void OnDrawBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize);

	virtual void UnSetAutoHideMode (CBCGPDockingControlBar* pFirstBarInGroup);

	CSize GetSize () const;
	CRect GetRect () const {return m_rect;}
	virtual CSize GetTextSize () const;

	BOOL IsHorizontal () const;

	CBCGPDockingControlBar* GetAutoHideWindow () const {return m_pAutoHideWindow;}
	CBCGPAutoHideToolBar*	GetParentBar () {return m_pParentBar;}

	static int m_nBorderSize;
	static int m_nMarginSize;
	static int m_nTextMargin;
	static int m_nTextSizeNoIcon;

	static BOOL m_bOverlappingTabs;

protected:
	CRect	m_rect;
	BOOL	m_bTop;
	DWORD	m_dwAlignment;
	int		m_nOffset;  // offset from the left bound of the parent bar 
						// for the horz buttons and from left bound for vert buttons

	BOOL	m_bVisible;

	CBCGPAutoHideToolBar*	m_pParentBar;
	CBCGPDockingControlBar*	m_pAutoHideWindow;
};

#endif // !defined(AFX_BCGPAutoHideBUTTON_H__582BE570_F465_4330_AA95_7DFF1250F0FC__INCLUDED_)
