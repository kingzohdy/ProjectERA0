// BCGPCaptionButton.cpp: implementation of the CBCGPCaptionButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGGlobals.h"
#include "BCGPCaptionButton.h"
#include "BCGPVisualManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int  CBCGPCaptionButton::m_nButtonMargin = 2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPCaptionButton::CBCGPCaptionButton()
{
	m_bPushed = FALSE;
	m_bFocused = FALSE;
	m_bHidden = FALSE;
	m_bEnabled = TRUE;
	m_nHit = HTNOWHERE;
	m_bLeftAlign = FALSE;
	m_clrForeground = (COLORREF)-1;
}

CBCGPCaptionButton::CBCGPCaptionButton(UINT nHit, BOOL bLeftAlign)
{
	m_bPushed = FALSE;
	m_bFocused = FALSE;
	m_bHidden = FALSE;
	m_bEnabled = TRUE;
	m_nHit = nHit;
	m_bLeftAlign = bLeftAlign;
	m_clrForeground = (COLORREF)-1;
}

CBCGPCaptionButton::~CBCGPCaptionButton()
{
}

UINT CBCGPCaptionButton::GetHit () const
{
	return m_nHit;
}

void CBCGPCaptionButton::OnDraw (CDC* pDC, BOOL bActive,
								 BOOL bHorz, BOOL bMaximized, BOOL bDisabled)
{
	if (m_bHidden)
	{
		return;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawCaptionButton (
			pDC, this, bActive, bHorz, bMaximized, bDisabled || !m_bEnabled);
}
