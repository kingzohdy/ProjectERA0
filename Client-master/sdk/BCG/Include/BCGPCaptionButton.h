// BCGPCaptionButton.h: interface for the CBCGPCaptionButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPCAPTIONBUTTON_H__30374C20_EBE0_479E_9F48_B116A6E2524C__INCLUDED_)
#define AFX_BCGPCAPTIONBUTTON_H__30374C20_EBE0_479E_9F48_B116A6E2524C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"
#include "MenuImages.h"
#include "BCGPVisualManager.h"

#define HTCLOSE_BCG	HTOBJECT	// To prevent standard Windows 98/2000 close tooltip
#define HTLEFTBUTTON_BCG	HTHELP + 2
#define HTRIGHTBUTTON_BCG	HTHELP + 3
#define HTMENU_BCG			HTHELP + 4

class BCGCBPRODLLEXPORT CBCGPCaptionButton : public CObject
{
public:
	CBCGPCaptionButton();
	CBCGPCaptionButton(UINT nHit, BOOL bLeftAlign = FALSE);
	virtual ~CBCGPCaptionButton();

	void Move (const CPoint& ptTo, BOOL bHide = FALSE)
	{
		m_ptOrg = ptTo; 
		m_bHidden = bHide;
	}


	CRect GetRect () const
	{ 
		return CRect (m_ptOrg, m_bHidden ? CSize (0, 0) : GetSize ());
	}

	static CSize GetSize ()
	{
		return CMenuImages::Size () + CSize (2 * m_nButtonMargin, 2 * m_nButtonMargin) +
			CBCGPVisualManager::GetInstance ()->GetCaptionButtonExtraBorder ();
	}

	virtual void OnDraw (CDC* pDC, BOOL bActive, 
		BOOL bHorz = TRUE, BOOL bMaximized = TRUE, BOOL bDisabled = FALSE);

	UINT GetHit () const;

	BOOL		m_bPushed;
    BOOL		m_bFocused;
	BOOL		m_bHidden;
	BOOL		m_bEnabled;

	COLORREF	m_clrForeground;

	UINT		m_nHit;

	BOOL		m_bLeftAlign;

	static int  m_nButtonMargin;

protected:
	CPoint		m_ptOrg;
};

#endif // !defined(AFX_BCGPCAPTIONBUTTON_H__30374C20_EBE0_479E_9F48_B116A6E2524C__INCLUDED_)
