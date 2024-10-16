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

// BCGVisualManagerXP.h: interface for the CBCGPVisualManagerXP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVISUALMANAGERXP_H__062013FA_7440_4CEC_AA78_67893D195FFA__INCLUDED_)
#define AFX_BCGPVISUALMANAGERXP_H__062013FA_7440_4CEC_AA78_67893D195FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"
#include "BCGPVisualManager.h"
#include "MenuImages.h"

class CBCGPStatusBar;
class CBCGPBaseControlBar;


class BCGCBPRODLLEXPORT CBCGPVisualManagerXP : public CBCGPVisualManager  
{
	DECLARE_DYNCREATE(CBCGPVisualManagerXP)

protected:
	CBCGPVisualManagerXP(BOOL bIsTemporary = FALSE);

public:
	virtual ~CBCGPVisualManagerXP();

protected:
	virtual void OnUpdateSystemColors ();

	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);
	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect);
	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);
	virtual void OnDrawMenuShadow (CDC* pDC, const CRect& rectClient, const CRect& rectExclude,
									int nDepth,  int iMinBrightness,  int iMaxBrightness,  
									CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight);
	virtual void OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz);
	
	virtual void OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnHighlightMenuItem (CDC*pDC, CBCGPToolbarMenuButton* pButton,
		CRect rect, COLORREF& clrText);
	virtual void OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed);

	virtual void OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnDrawTab (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill, int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);

	virtual void OnEraseTabsButton (CDC* pDC, CRect rect, CBCGPButton* pButton,
									CBCGPBaseTabWnd* pWndTab);
	virtual void OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
										CBCGPButton* pButton, UINT uiState, CBCGPBaseTabWnd* pWndTab);

	virtual COLORREF OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons);

	virtual void OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton,
									BOOL bActive, BOOL bHorz, BOOL bMaximized, BOOL bDisabled, 
									int nImageID = -1);
	virtual void OnDrawCaptionButtonIcon (CDC* pDC, 
											CBCGPCaptionButton* pButton,
											CMenuImages::IMAGES_IDS id,
											BOOL bActive, BOOL bDisabled,
											CPoint ptImage);

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
	virtual void OnDrawControlBorder (CWnd* pWndCtrl);

	virtual COLORREF GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive);
	virtual COLORREF OnFillCalendarBarNavigationRow (CDC* pDC, CRect rect);
	virtual void OnFillCalendarBarWeekColumn (CDC* pDC, CRect rect,
		COLORREF& clrText, COLORREF& clrTextDisabled);
	virtual void OnDrawSpinButtons (CDC* pDC, CRect rectSpin, int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl);

	virtual COLORREF OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected = FALSE);
	virtual void OnDrawMenuArrowOnCustomizeList (CDC* pDC, CRect rectCommand, BOOL bSelected);

	virtual void OnDrawMenuSystemButton (CDC* pDC, CRect rect, UINT uiSystemCommand, 
										UINT nStyle, BOOL bHighlight);
	virtual void OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* pBar,
					CRect rectPane, UINT uiID, UINT nStyle);

	virtual int GetMenuImageMargin () const
	{
		return 3;
	}

	virtual COLORREF OnFillMiniFrameCaption (CDC* pDC, CRect rectCaption, 
											CBCGPMiniFrameWnd* pFrameWnd, 
											BOOL bActive);
	virtual void OnDrawMiniFrameBorder (CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize);
	virtual void OnDrawFloatingToolbarBorder (	CDC* pDC, CBCGPBaseToolBar* pToolBar, 
												CRect rectBorder, CRect rectBorderSize);
	// Tasks pane:
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
	
	BOOL IsLook2000Allowed () const
	{
		return FALSE;
	}

	virtual void OnDrawSplitterBorder (CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect rect);
	virtual void OnDrawSplitterBox (CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect& rect);

	// Calculator:
	virtual BOOL OnDrawCalculatorButton (CDC* pDC, CRect rect, CBCGPToolbarButton* pButton, CBCGPVisualManager::BCGBUTTON_STATE state, int cmd /* CBCGPCalculator::CalculatorCommands */, CBCGPCalculator* pCalculator);
	virtual BOOL OnDrawCalculatorDisplay (CDC* pDC, CRect rect, const CString& strText, BOOL bMem, CBCGPCalculator* pCalculator);

	// Edit box:
	virtual BOOL OnDrawBrowseButton (CDC* pDC, CRect rect, CBCGPEdit* pEdit, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& clrText);

	CBrush		m_brGripperHorz;
	CBrush		m_brGripperVert;

	COLORREF	m_clrBarBkgnd;			// Control bar background color (expect menu bar)
	CBrush		m_brBarBkgnd;

	COLORREF	m_clrMenuLight;			// Color of the light menu area
	CBrush		m_brMenuLight;

	COLORREF	m_clrInactiveTabText;
	CBrush		m_brTabBack;

	COLORREF	m_clrHighlight;			// Highlighted toolbar/menu item color
	CBrush		m_brHighlight;

	COLORREF	m_clrHighlightDn;		// Highlighted and pressed toolbar item color
	CBrush		m_brHighlightDn;

	COLORREF	m_clrHighlightChecked;
	CBrush		m_brHighlightChecked;

	COLORREF	m_clrGripper;

	CPen		m_penSeparator;

	COLORREF	m_clrPaneBorder;		// Status bar pane border

	COLORREF	m_clrMenuBorder;		// Menu border
	COLORREF	m_clrMenuItemBorder;	// Highlighted menu item border

	CBrush		m_brFloatToolBarBorder;	// Floating toolbar border

	BOOL		m_bConnectMenuToParent;
	COLORREF	m_clrMenuShadowBase;

	virtual void CreateGripperBrush ();
	virtual void ExtendMenuButton (CBCGPToolbarMenuButton* pMenuButton, CRect& rect);

	virtual void OnFillHighlightedArea (CDC* pDC, CRect rect, CBrush* pBrush,
		CBCGPToolbarButton* pButton);
};

#endif // !defined(AFX_BCGPVISUALMANAGERXP_H__062013FA_7440_4CEC_AA78_67893D195FFA__INCLUDED_)
