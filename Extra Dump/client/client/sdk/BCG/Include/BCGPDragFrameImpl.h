// BCGPDragFrameImpl.h: interface for the CBCGPDragFrameImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPDRAGFRAMEIMPL_H__033E88D4_FCB7_4D7F_836F_3B8CDE004344__INCLUDED_)
#define AFX_BCGPDRAGFRAMEIMPL_H__033E88D4_FCB7_4D7F_836F_3B8CDE004344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBCGPDockManager;
class CBCGPControlBar;
class CBCGPDockingControlBar;

class CBCGPDragFrameImpl  
{
public:
	CBCGPDragFrameImpl();
	virtual ~CBCGPDragFrameImpl();

	void Init (CWnd* pDraggedWnd);
	void MoveDragFrame (BOOL bForceMove = FALSE); 
	void EndDrawDragFrame (BOOL bClearInternalRects = TRUE);

	CPoint		m_ptHot;
	CRect		m_rectDrag;
	CRect		m_rectExpectedDocked;

	CBCGPDockingControlBar*	m_pFinalTargetBar;
	BOOL					m_bDockToTab;
	BOOL					m_bDragStarted;

	int						m_nInsertedTabID;

protected:
	void DrawDragFrame (LPCRECT lpRectOld, LPCRECT lpRectNew, BOOL bFirstTime = FALSE,
						int nNewThickness = 4, int nOldThickness = 4);
	void DrawFrameTab (CBCGPDockingControlBar* pTargetBar, BOOL bErase);

	int			m_nOldThikness;

	CBCGPDockManager*		m_pDockManager;
	CWnd*					m_pDraggedWnd;
	CBCGPDockingControlBar*	m_pTargetBar;
	BOOL					m_bFrameTabDrawn;
};

#endif // !defined(AFX_BCGPDRAGFRAMEIMPL_H__033E88D4_FCB7_4D7F_836F_3B8CDE004344__INCLUDED_)
