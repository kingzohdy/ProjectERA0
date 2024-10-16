// BCGPGlobalUtils.cpp: implementation of the CBCGPGlobalUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPGlobalUtils.h"
#include "BCGPDockManager.h"
#include "BCGPBarContainerManager.h"
#include "BCGPDockingControlBar.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPMultiMiniFrameWnd.h"
#include "BCGPBaseTabbedBar.h"

#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPOleCntrFrameWnd.h"

#pragma warning (disable : 4706)

#include "multimon.h"

#pragma warning (default : 4706)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBCGPGlobalUtils globalUtils;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPGlobalUtils::CBCGPGlobalUtils()
{
	m_bDialogApp = FALSE;
	m_bIsDragging = FALSE;
}

CBCGPGlobalUtils::~CBCGPGlobalUtils()
{

}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CheckAlignment (CPoint point, CBCGPBaseControlBar* pBar, int nSencitivity, 
									   BOOL bOuterEdge, DWORD& dwAlignment, 
									   DWORD dwEnabledDockBars, LPCRECT lpRectBounds) const
{
	CRect rectBounds;
	if (pBar != NULL)
	{
		pBar->GetWindowRect (rectBounds);
	}
	else if (lpRectBounds != NULL)
	{
		rectBounds = *lpRectBounds;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nCaptionHeight = 0;
	int nTabAreaTopHeight = 0; 
	int nTabAreaBottomHeight = 0;

	CBCGPDockingControlBar* pDockingBar = 
				DYNAMIC_DOWNCAST (CBCGPDockingControlBar, pBar);

	if (pDockingBar != NULL)
	{
		nCaptionHeight = pDockingBar->GetCaptionHeight ();

		CRect rectTabAreaTop;
		CRect rectTabAreaBottom;
		pDockingBar->GetTabArea (rectTabAreaTop, rectTabAreaBottom);
		nTabAreaTopHeight = rectTabAreaTop.Height ();
		nTabAreaBottomHeight = rectTabAreaBottom.Height ();
	}

	// build rect for top area
	if (bOuterEdge)
	{
		CRect rectToCheck (rectBounds.left - nSencitivity, rectBounds.top - nSencitivity, 
						   rectBounds.right + nSencitivity, rectBounds.top);
		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_TOP)
		{
			dwAlignment = CBRS_ALIGN_TOP;
			return TRUE;
		}
		
		// build rect for left area
		rectToCheck.right = rectBounds.left;
		rectToCheck.bottom = rectBounds.bottom + nSencitivity;

		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_LEFT)
		{
			dwAlignment = CBRS_ALIGN_LEFT;
			return TRUE;
		}
		
		// build rect for bottom area
		rectToCheck.left = rectBounds.left - nSencitivity;
		rectToCheck.top = rectBounds.bottom;
		rectToCheck.right = rectBounds.right + nSencitivity;
		rectToCheck.bottom = rectBounds.bottom + nSencitivity;

		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM)
		{
			dwAlignment = CBRS_ALIGN_BOTTOM;
			return TRUE;
		}

		// build rect for right area
		rectToCheck.left = rectBounds.right;
		rectToCheck.top = rectBounds.top - nSencitivity;
		
		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT)
		{
			dwAlignment = CBRS_ALIGN_RIGHT;
			return TRUE;
		}
	}
	else
	{
		// build rect for top area
		CRect rectToCheck (rectBounds.left - nSencitivity, rectBounds.top - nSencitivity, 
						   rectBounds.right + nSencitivity, 
						   rectBounds.top + nSencitivity + nCaptionHeight);
		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_TOP)
		{
			dwAlignment = CBRS_ALIGN_TOP;
			return TRUE;
		}

		
		// build rect for left area
		rectToCheck.right = rectBounds.left + nSencitivity;
		rectToCheck.bottom = rectBounds.bottom + nSencitivity;

		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_LEFT)
		{
			dwAlignment = CBRS_ALIGN_LEFT;
			return TRUE;
		}
		
		// build rect for bottom area
		rectToCheck.left = rectBounds.left - nSencitivity;
		rectToCheck.top = rectBounds.bottom - nSencitivity - nTabAreaBottomHeight;
		rectToCheck.right = rectBounds.right + nSencitivity;
		rectToCheck.bottom = rectBounds.bottom + nSencitivity;

		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_BOTTOM)
		{
			dwAlignment = CBRS_ALIGN_BOTTOM;
			return TRUE;
		}

		// build rect for right area
		rectToCheck.left = rectBounds.right - nSencitivity;
		rectToCheck.top = rectBounds.top - nSencitivity;
		
		if (rectToCheck.PtInRect (point) && dwEnabledDockBars & CBRS_ALIGN_RIGHT)
		{
			dwAlignment = CBRS_ALIGN_RIGHT;
			return TRUE;
		}
	}

	return FALSE;
}
//------------------------------------------------------------------------//
CBCGPDockManager* CBCGPGlobalUtils::GetDockManager (CWnd* pWnd)
{
	if (pWnd == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pWnd);

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		return ((CBCGPFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		return ((CBCGPMDIFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleIPFrameWnd)))
	{
		return ((CBCGPOleIPFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleDocIPFrameWnd)))
	{
		return ((CBCGPOleDocIPFrameWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIChildWnd)))
	{
		return ((CBCGPMDIChildWnd*) pWnd)->GetDockManager ();
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CDialog)) ||
		pWnd->IsKindOf (RUNTIME_CLASS (CPropertySheet)))
	{
		m_bDialogApp = TRUE;
	}
	else if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPOleCntrFrameWnd)))
	{
		return ((CBCGPOleCntrFrameWnd*) pWnd)->GetDockManager ();
	}

	return NULL;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::FlipRect (CRect& rect, int nDegrees)
{
	CRect rectTmp = rect;
	switch (nDegrees)
	{
	case 90:
		rect.top = rectTmp.left;
		rect.right = rectTmp.top;
		rect.bottom = rectTmp.right;
		rect.left = rectTmp.bottom;
		break;
	case 180:
		rect.top = rectTmp.bottom;
		rect.bottom = rectTmp.top;
		break;
	case 275:
	case -90:
		rect.left = rectTmp.top;
		rect.top = rectTmp.right;
		rect.right = rectTmp.bottom;
		rect.bottom = rectTmp.left;
		break;
	}
}
//------------------------------------------------------------------------//
DWORD CBCGPGlobalUtils::GetOppositeAlignment (DWORD dwAlign)
{
	switch (dwAlign & CBRS_ALIGN_ANY)
	{
	case CBRS_ALIGN_LEFT:
		return CBRS_ALIGN_RIGHT;
	case CBRS_ALIGN_RIGHT:
		return CBRS_ALIGN_LEFT;
	case CBRS_ALIGN_TOP:
		return CBRS_ALIGN_BOTTOM;
	case CBRS_ALIGN_BOTTOM:
		return CBRS_ALIGN_TOP;
	}
	return 0;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::SetNewParent (CObList& lstControlBars, CWnd* pNewParent,
									 BOOL bCheckVisibility)
{
	ASSERT_VALID (pNewParent);
	for (POSITION pos = lstControlBars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPBaseControlBar* pBar = (CBCGPBaseControlBar*) lstControlBars.GetNext (pos);

		if (bCheckVisibility && !pBar->IsBarVisible ())
		{
			continue;
		}
		if (!pBar->IsKindOf (RUNTIME_CLASS (CBCGPSlider)))
		{
			pBar->ShowWindow (SW_HIDE);
			pBar->SetParent (pNewParent);
			CRect rectWnd;
			pBar->GetWindowRect (rectWnd);
			pNewParent->ScreenToClient (rectWnd);

			pBar->SetWindowPos (NULL, -rectWnd.Width (), -rectWnd.Height (), 
									  100, 100, SWP_NOZORDER | SWP_NOSIZE  | SWP_NOACTIVATE);
			pBar->ShowWindow (SW_SHOW);
		}
		else
		{
			pBar->SetParent (pNewParent);
		}
	}
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::CalcExpectedDockedRect (CBCGPBarContainerManager& barContainerManager, 
												CWnd* pWndToDock, CPoint ptMouse, 
												CRect& rectResult, BOOL& bDrawTab, 
												CBCGPDockingControlBar** ppTargetBar)
{
	ASSERT (ppTargetBar != NULL);

	DWORD dwAlignment = CBRS_ALIGN_LEFT;
	BOOL bTabArea = FALSE;
	BOOL bCaption = FALSE;
	bDrawTab = FALSE;
	*ppTargetBar = NULL;

	rectResult.SetRectEmpty ();

	if (GetKeyState (VK_CONTROL) < 0)
	{
		return;
	}

	if (!GetCBAndAlignFromPoint (barContainerManager, ptMouse, 
								 ppTargetBar, dwAlignment, bTabArea, bCaption) || 
		*ppTargetBar == NULL)
	{
		return;
	}

	if (*ppTargetBar != NULL)
	{
		DWORD dwTargetAlign = (*ppTargetBar)->GetCurrentAlignment ();

		if ((*ppTargetBar)->GetParentMiniFrame () != 0)
		{
			dwTargetAlign = CBRS_ALIGN_ANY;
		}
		
		CBCGPControlBar* pBar = NULL;

		if (pWndToDock->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
		{
			CBCGPMiniFrameWnd* pMiniFrame = 
				DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pWndToDock);
			ASSERT_VALID (pMiniFrame);
			pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pMiniFrame->GetControlBar ());
		}
		else
		{
			pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pWndToDock);
		}

		if (pBar != NULL && (pBar->GetEnabledAlignment () & dwTargetAlign) == 0)
		{
			return;
		}
	}

	if (bTabArea || bCaption)
	{
		// can't make tab on miniframe
		bDrawTab = ((*ppTargetBar) != NULL);

		if (bDrawTab)
		{
			bDrawTab = (*ppTargetBar)->CanBeAttached () && CanBeAttached (pWndToDock);
		}

		if (!bDrawTab)
		{
			return;
		}

	}

	if ((*ppTargetBar) != NULL && (*ppTargetBar)->GetParentMiniFrame () != NULL && 
		!IsWndCanFloatMulti (pWndToDock))
	{
		bDrawTab = FALSE;
		return;
	}

	if ((*ppTargetBar) != NULL && 
		pWndToDock->IsKindOf (RUNTIME_CLASS (CBCGPBaseControlBar)) && 
		!(*ppTargetBar)->CanAcceptBar ((CBCGPBaseControlBar*) pWndToDock))
	{
		bDrawTab = FALSE;
		return;
	}

	CRect rectOriginal; 
	(*ppTargetBar)->GetWindowRect (rectOriginal);

	if ((*ppTargetBar) == pWndToDock ||
		pWndToDock->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)) && 
		(*ppTargetBar)->GetParentMiniFrame () == pWndToDock)
	{
		bDrawTab = FALSE;
		return;
	}
	
	CRect rectInserted;
	CRect rectSlider;
	DWORD dwSliderStyle;
	CSize sizeMinOriginal (0, 0);
	CSize sizeMinInserted (0, 0);

	
	pWndToDock->GetWindowRect (rectInserted);

	barContainerManager.CalcRects (rectOriginal, rectInserted, rectSlider, dwSliderStyle, 
									 dwAlignment, sizeMinOriginal, sizeMinInserted);

	rectResult = rectInserted;
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::CanBeAttached (CWnd* pWnd) const
{
	ASSERT_VALID (pWnd);

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
	{
		return ((CBCGPMiniFrameWnd*) pWnd)->CanBeAttached ();
	}

	if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPControlBar)))
	{
		return ((CBCGPControlBar*) pWnd)->CanBeAttached ();
	}

	return FALSE;

}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::IsWndCanFloatMulti (CWnd* pWnd) const
{
	CBCGPControlBar* pBar = NULL;

	CBCGPMiniFrameWnd* pMiniFrame = 
		DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pWnd);
	
	if (pMiniFrame != NULL)
	{
		pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pMiniFrame->GetControlBar ());
	}
	else
	{
		pBar = DYNAMIC_DOWNCAST (CBCGPControlBar, pWnd);

	}

	if (pBar == NULL)
	{
		return FALSE;
	}

	if (pBar->IsTabbed ())
	{
		CWnd* pParentMiniFrame = pBar->GetParentMiniFrame ();
		// tabbed bar that is floating in multi miniframe
		if (pParentMiniFrame != NULL && pParentMiniFrame->IsKindOf (RUNTIME_CLASS (CBCGPMultiMiniFrameWnd)))
		{
			return TRUE;
		}
	}

	
	return ((pBar->GetBarStyle () & CBRS_FLOAT_MULTI) != 0);
}
//------------------------------------------------------------------------//
BOOL CBCGPGlobalUtils::GetCBAndAlignFromPoint (CBCGPBarContainerManager& barContainerManager, 
													 CPoint pt, 
												     CBCGPDockingControlBar** ppTargetControlBar, 
												     DWORD& dwAlignment, 
													 BOOL& bTabArea, 
													 BOOL& bCaption)
{
	ASSERT (ppTargetControlBar != NULL);
	*ppTargetControlBar = NULL;

	BOOL bOuterEdge = FALSE;

	// if the mouse is over a miniframe's caption and this miniframe has only one
	// visible docking control bar, we need to draw a tab
	bCaption = barContainerManager.CheckForMiniFrameAndCaption (pt, ppTargetControlBar);
	if (bCaption)
	{
		return TRUE;
	}


	*ppTargetControlBar = 
		barContainerManager.ControlBarFromPoint (pt, CBCGPDockManager::m_nDockSencitivity, 
													TRUE, bTabArea, bCaption);

	if ((bCaption || bTabArea) && *ppTargetControlBar != NULL) 
	{
		return TRUE;
	}

	if (*ppTargetControlBar == NULL)
	{
		barContainerManager.ControlBarFromPoint (pt, CBCGPDockManager::m_nDockSencitivity, 
														FALSE, bTabArea, bCaption);
		// the exact bar was not found - it means the docked frame at the outer edge
		bOuterEdge = TRUE;
		return TRUE;
	}

	if (*ppTargetControlBar != NULL)
	{
		if (!globalUtils.CheckAlignment (pt, *ppTargetControlBar,
										CBCGPDockManager::m_nDockSencitivity, 
										bOuterEdge, dwAlignment))
		{
			// unable for some reason to determine alignment
			*ppTargetControlBar = NULL;
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------//
void CBCGPGlobalUtils::AdjustRectToWorkArea (CRect& rect, CRect* pRectDelta)
{
	CPoint ptStart;

	if (m_bIsDragging)
	{
		::GetCursorPos (&ptStart);
	}
	else
	{
		ptStart = rect.TopLeft ();
	}

	CRect rectScreen;
	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);
	if (GetMonitorInfo (
		MonitorFromPoint (ptStart, MONITOR_DEFAULTTONEAREST),
		&mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	int nDelta = pRectDelta != NULL ? pRectDelta->left : 0;

	if (rect.right <= rectScreen.left + nDelta)
	{
		rect.OffsetRect (rectScreen.left - rect.left - nDelta, 0);
	}

	nDelta = pRectDelta != NULL ? pRectDelta->right : 0;
	if (rect.left >= rectScreen.right - nDelta)
	{
		rect.OffsetRect (rectScreen.right - rect.right - nDelta, 0);
	}

	nDelta = pRectDelta != NULL ? pRectDelta->bottom : 0;
	if (rect.top >= rectScreen.bottom - nDelta)
	{
		rect.OffsetRect (0, rectScreen.bottom - rect.top - nDelta);
	}

	nDelta = pRectDelta != NULL ? pRectDelta->top : 0;
	if (rect.bottom < rectScreen.top + nDelta)
	{
		rect.OffsetRect (0, rectScreen.top - rect.bottom + nDelta);
	}
}