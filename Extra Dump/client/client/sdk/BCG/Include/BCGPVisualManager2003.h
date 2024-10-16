// BCGPVisualManager2003.h: interface for the CBCGPVisualManager2003 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVISUALMANAGER2003_H__44F195EA_6269_4D0C_80D4_2E53D5D1D0DB__INCLUDED_)
#define AFX_BCGPVISUALMANAGER2003_H__44F195EA_6269_4D0C_80D4_2E53D5D1D0DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPVisualManagerXP.h"

class BCGCBPRODLLEXPORT CBCGPVisualManager2003 :	public CBCGPVisualManagerXP,
													public CBCGPWinXPThemeManager
{
	DECLARE_DYNCREATE(CBCGPVisualManager2003)

public:
	CBCGPVisualManager2003();
	virtual ~CBCGPVisualManager2003();

	static void SetUseGlobalTheme (BOOL bUseGlobalTheme = TRUE);
	static BOOL IsUseGlobalTheme ()
	{
		return m_bUseGlobalTheme;
	}

	virtual BOOL IsWinXPThemeSupported () const	{	return m_hThemeWindow != NULL;	}

	static void SetStatusBarOfficeXPLook (BOOL bStatusBarOfficeXPLook = TRUE);
	static BOOL IsStatusBarOfficeXPLook ()
	{
		return m_bStatusBarOfficeXPLook;
	}

	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect);
	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);
	virtual void OnDrawComboBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnFillOutlookPageButton (	CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText);
	virtual void OnDrawOutlookPageButtonBorder (CDC* pDC, CRect& rectBtn, 
										BOOL bIsHighlighted, BOOL bIsPressed);
	virtual void OnDrawOutlookBarSplitter (CDC* pDC, CRect rectSplitter);
	virtual void OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, COLORREF& clrText);
	virtual void OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz);

	virtual int GetToolBarCustomizeButtonMargin () const
	{
		return 1;
	}

	virtual COLORREF OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected = FALSE);

	virtual void DrawCustomizeButton (CDC* pDC, CRect rect, BOOL bIsHorz,
						  CBCGPVisualManager::BCGBUTTON_STATE state,
						  BOOL bIsCustomize, BOOL bIsMoreButtons);

	virtual void OnUpdateSystemColors ();
	virtual void OnFillHighlightedArea (CDC* pDC, CRect rect, CBrush* pBrush,
		CBCGPToolbarButton* pButton);

	virtual BOOL IsOffsetPressedButton () const
	{
		return FALSE;
	}

	virtual int GetShowAllMenuItemsHeight (CDC* pDC, const CSize& sizeDefault);
	virtual void OnDrawShowAllMenuItems (CDC* pDC, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnDrawCaptionBarBorder (CDC* pDC, CBCGPCaptionBar* pBar, CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder);
	virtual void OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive);

	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);

	virtual void OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual BOOL OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnEraseTabsButton (CDC* pDC, CRect rect, CBCGPButton* pButton,
									CBCGPBaseTabWnd* pWndTab);
	virtual void OnDrawTab (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill, int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual void GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack);
	virtual BOOL IsHighlightOneNoteTabs () const	{	return TRUE;	}

	// Tasks pane:
	virtual void OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea);

	virtual void OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
										BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, 
										BOOL bCanCollapse = FALSE);

	virtual void OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL bSpecial = FALSE);
	virtual void OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial = FALSE, 
											BOOL bNoTitle = FALSE);
	virtual void OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, 
							BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE);

	virtual void OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited);

	virtual void OnDrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* pStatusBar,
				CRect rectProgress, int nProgressTotal, int nProgressCurr,
				COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
				BOOL bProgressText);
	virtual void OnFillHeaderCtrlBackground (CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect rect);

	virtual COLORREF OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons);

	// Auto-hide buttons:
	virtual void OnFillAutoHideButtonBackground (CDC* pDC, CRect rect, CBCGPAutoHideButton* pButton);
	virtual void OnDrawAutoHideButtonBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* pButton);

	virtual BOOL DrawPushButtonWinXP (CDC* pDC, CRect rect, CBCGPButton* pButton, UINT uiState)	
	{	
		return DrawPushButton (pDC, rect, pButton, uiState);
	}

	virtual BOOL DrawComboDropButtonWinXP (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted)
	{
		return DrawComboDropButton (pDC, rect,
										bDisabled,
										bIsDropped,
										bIsHighlighted);
	}

	virtual BOOL DrawComboBorderWinXP (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted)
	{
		return DrawComboBorder (pDC, rect,
								bDisabled,
								bIsDropped,
								bIsHighlighted);
	}

	// Calculator:
	virtual BOOL OnDrawCalculatorButton (CDC* pDC, CRect rect, CBCGPToolbarButton* pButton, CBCGPVisualManager::BCGBUTTON_STATE state, int cmd /* CBCGPCalculator::CalculatorCommands */, CBCGPCalculator* pCalculator);
	virtual BOOL OnDrawCalculatorDisplay (CDC* pDC, CRect rect, const CString& strText, BOOL bMem, CBCGPCalculator* pCalculator);

	// Edit box:
	virtual BOOL OnDrawBrowseButton (CDC* pDC, CRect rect, CBCGPEdit* pEdit, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& clrText);

protected:

	COLORREF GetThemeColor (HTHEME hTheme, int nIndex) const;

	COLORREF	m_clrBarGradientDark;
	COLORREF	m_clrBarGradientLight;

	COLORREF	m_clrToolBarGradientDark;
	COLORREF	m_clrToolBarGradientLight;

	COLORREF	m_clrToolBarGradientVertLight;
	COLORREF	m_clrToolBarGradientVertDark;

	COLORREF	m_clrCustomizeButtonGradientDark;
	COLORREF	m_clrCustomizeButtonGradientLight;

	COLORREF	m_clrToolBarBottomLine;

	COLORREF	m_colorToolBarCornerTop;
	COLORREF	m_colorToolBarCornerBottom;

	COLORREF	m_clrHighlightMenuItem;

	COLORREF	m_clrHighlightGradientLight;
	COLORREF	m_clrHighlightGradientDark;

	COLORREF	m_clrHighlightDnGradientLight;
	COLORREF	m_clrHighlightDnGradientDark;

	COLORREF	m_clrHighlightCheckedGradientLight;
	COLORREF	m_clrHighlightCheckedGradientDark;

	CPen		m_penSeparatorLight;

	COLORREF	m_clrGripper;

	COLORREF	m_clrCaptionBarGradientLight;
	COLORREF	m_clrCaptionBarGradientDark;

	CBrush		m_brTearOffCaption;
	CBrush		m_brFace;

	COLORREF	m_clrTaskPaneGradientDark;
	COLORREF	m_clrTaskPaneGradientLight;
	COLORREF	m_clrTaskPaneGroupCaptionDark;
	COLORREF	m_clrTaskPaneGroupCaptionLight;
	COLORREF	m_clrTaskPaneGroupCaptionSpecDark;
	COLORREF	m_clrTaskPaneGroupCaptionSpecLight;
	COLORREF	m_clrTaskPaneGroupAreaLight;
	COLORREF	m_clrTaskPaneGroupAreaDark;
	COLORREF	m_clrTaskPaneGroupAreaSpecLight;
	COLORREF	m_clrTaskPaneGroupAreaSpecDark;
	COLORREF	m_clrTaskPaneGroupBorder;
	
	CPen		m_penTaskPaneGroupBorder;

	BOOL		m_bIsStandardWinXPTheme;
	CString		m_strWinXPThemeColor;

	static BOOL	m_bUseGlobalTheme;
	static BOOL m_bStatusBarOfficeXPLook;

	virtual void ModifyGlobalColors ();
};

#endif // !defined(AFX_BCGPVISUALMANAGER2003_H__44F195EA_6269_4D0C_80D4_2E53D5D1D0DB__INCLUDED_)
