//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
// BCGWinXPVisualManager.h: interface for the CBCGPWinXPVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPWINXPVISUALMANAGER_H__0795BCE7_8E67_4145_A840_D9655AC0293D__INCLUDED_)
#define AFX_BCGPWINXPVISUALMANAGER_H__0795BCE7_8E67_4145_A840_D9655AC0293D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"
#include "BCGPVisualManagerXP.h"

class CBCGPButton;

class BCGCBPRODLLEXPORT CBCGPWinXPVisualManager :	public CBCGPVisualManagerXP,
													public CBCGPWinXPThemeManager
{
	DECLARE_DYNCREATE(CBCGPWinXPVisualManager)

public:
	CBCGPWinXPVisualManager(BOOL bIsTemporary = FALSE);
	virtual ~CBCGPWinXPVisualManager();

	static BOOL IsWinXPThemeAvailible ();

	void SetOfficeStyleMenus (BOOL bOn = TRUE);
	BOOL IsOfficeStyleMenus () const
	{
		return m_bOfficeStyleMenus;
	}

	virtual BOOL IsWinXPThemeSupported () const	{	return m_hThemeWindow != NULL;	}

	virtual void OnUpdateSystemColors ();

	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);
	virtual void OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz);
	virtual void OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive, BOOL bHorz, 
										BOOL bMaximized, BOOL bDisabled, int nImageID = -1);
	virtual void OnDrawCaptionButtonIcon (CDC* pDC, 
											CBCGPCaptionButton* pButton,
											CMenuImages::IMAGES_IDS id,
											BOOL bActive, BOOL bDisabled,
											CPoint ptImage);
	virtual void OnDrawMenuSystemButton (CDC* pDC, CRect rect, UINT uiSystemCommand, 
										UINT nStyle, BOOL bHighlight);
	virtual void OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* pBar,
					CRect rectPane, UINT uiID, UINT nStyle);

	virtual void OnDrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* pStatusBar,
				CRect rectProgress, int nProgressTotal, int nProgressCurr,
				COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
				BOOL bProgressText);
	virtual void OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* pStatBar,
				CRect rectSizeBox);

	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);
	virtual void OnDrawMenuShadow (CDC* pDC, const CRect& rectClient, const CRect& rectExclude,
									int nDepth,  int iMinBrightness,  int iMaxBrightness,  
									CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight);
	virtual void OnDrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawComboBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawEditBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsHighlighted,
										CBCGPToolbarEditBoxButton* pButton);
	virtual void OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive);

	virtual COLORREF GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state,
		BOOL bHorz);

	virtual void OnHighlightMenuItem (CDC *pDC, CBCGPToolbarMenuButton* pButton,
		CRect rect, COLORREF& clrText);
	virtual void OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed);

	virtual void CBCGPWinXPVisualManager::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pWndTab);
	virtual void CBCGPWinXPVisualManager::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT uiState,
												 CBCGPBaseTabWnd* pWndTab);

	virtual COLORREF OnFillMiniFrameCaption (CDC* pDC, CRect rectCaption, 
											CBCGPMiniFrameWnd* pFrameWnd, 
											BOOL bActive);
	virtual void OnDrawMiniFrameBorder (CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize);

	// Outlook bar page buttons:
	virtual void OnFillOutlookPageButton (	CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText);
	virtual void OnDrawOutlookPageButtonBorder (CDC* pDC, 
							CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed);

	// Customization dialog:
	virtual COLORREF OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected = FALSE);

	virtual CSize GetButtonExtraBorder () const;
	virtual CSize GetCaptionButtonExtraBorder () const;

	virtual void OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC,
		CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted);
	virtual void OnDrawHeaderCtrlSortArrow (CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsUp);

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
	
	virtual void OnDrawExpandingBox (CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox);

	virtual void OnDrawControlBorder (CWnd* pWndCtrl);

	// Date/time controls:
	virtual COLORREF OnFillCalendarBarNavigationRow (CDC* pDC, CRect rect);
	virtual void OnDrawCheckBox (CDC *pDC, CRect rect, 
										 BOOL bHighlighted, 
										 BOOL bChecked,
										 BOOL bEnabled);

	virtual BOOL IsOfficeXPStyleMenus () const
	{
		return m_bOfficeStyleMenus;
	}

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

	// Edit box:
	virtual BOOL OnDrawBrowseButton (CDC* pDC, CRect rect, CBCGPEdit* pEdit, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& clrText);
	virtual void OnDrawSpinButtons (CDC* pDC, CRect rectSpin, int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl);

protected:
	BOOL	m_bOfficeStyleMenus;
};

#endif // !defined(AFX_BCGPWINXPVISUALMANAGER_H__0795BCE7_8E67_4145_A840_D9655AC0293D__INCLUDED_)
