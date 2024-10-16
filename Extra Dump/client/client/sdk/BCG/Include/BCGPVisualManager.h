// BCGPVisualManager.h: interface for the CBCGPPVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVISUALMANAGER_H__8070C021_60CE_11D5_A304_8B7130518C10__INCLUDED_)
#define AFX_BCGPVISUALMANAGER_H__8070C021_60CE_11D5_A304_8B7130518C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"

class CBCGPBaseControlBar;
class CBCGPPopupMenu;
class CBCGPBaseToolBar;
class CBCGPToolBar;
class CBCGPToolbarButton;
class CBCGPToolbarMenuButton;
class CBCGPCaptionButton;
class CBCGPCaptionMenuButton;
class CBCGPBaseTabWnd;
class CBCGPToolbarComboBoxButton;
class CBCGPStatusBar;
class CBCGPButton;
class CBCGPMiniFrameWnd;
class CBCGPCaptionBar;
class CBCGPHeaderCtrl;
class CBCGPToolbarEditBoxButton;
class CBCGPDockingControlBar;
class CBCGPTask;
class CBCGPTasksGroup;
class CBCGPSlider;
class CBCGPCalendarBar;
class CBCGPCalendarButton;
class CBCGPSplitterWnd;
class CBCGPAutoHideButton;
class CBCGPCalculator;
class CBCGPEdit;
class CBCGPSpinButtonCtrl;

#define	SPIN_PRESSEDUP			0x0001
#define	SPIN_PRESSEDDOWN		0x0002
#define	SPIN_HIGHLIGHTEDUP		0x0004
#define	SPIN_HIGHLIGHTEDDOWN	0x0008
#define	SPIN_DISABLED			0x0010

#ifndef _UXTHEME_H_

// From uxtheme.h:
typedef HANDLE HTHEME;          // handle to a section of theme data for class

#endif // THEMEAPI

typedef HTHEME (__stdcall * OPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
typedef HRESULT (__stdcall * CLOSETHEMEDATA)(HTHEME hTheme);
typedef HRESULT (__stdcall * DRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, 
						int iPartId, int iStateId, const RECT *pRect, 
						OPTIONAL const RECT *pClipRect);
typedef HRESULT (__stdcall * GETTHEMECOLOR)(HTHEME hTheme, int iPartId, 
						int iStateId, int iPropId, OUT COLORREF *pColor);
typedef COLORREF (__stdcall * GETTHEMESYSCOLOR)(HTHEME hTheme, int iColorId);

typedef HRESULT (__stdcall * GETCURRENTTHEMENAME)(
    OUT LPWSTR pszThemeFileName, int cchMaxNameChars, 
    OUT OPTIONAL LPWSTR pszColorBuff, int cchMaxColorChars,
    OUT OPTIONAL LPWSTR pszSizeBuff, int cchMaxSizeChars);

typedef HTHEME (__stdcall * GETWINDOWTHEME)(HWND hWnd);

class BCGCBPRODLLEXPORT CBCGPWinXPThemeManager
{
public:
	CBCGPWinXPThemeManager ();
	virtual ~CBCGPWinXPThemeManager ();

	virtual BOOL DrawPushButton (CDC* pDC, CRect rect, CBCGPButton* pButton, UINT uiState);

	virtual BOOL DrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* pStatusBar,
			CRect rectProgress, int nProgressTotal, int nProgressCurr,
			COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
			BOOL bProgressText);
	virtual BOOL DrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted);
	virtual BOOL DrawComboBorder	(CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted);

protected:
	void UpdateSystemColors ();
	void CleanUpThemes ();

	HTHEME				m_hThemeToolBar;
	HTHEME				m_hThemeRebar;
	HTHEME				m_hThemeButton;
	HTHEME				m_hThemeStatusBar;
	HTHEME				m_hThemeWindow;
	HTHEME				m_hThemeComboBox;
	HTHEME				m_hThemeProgress;
	HTHEME				m_hThemeHeader;
	HTHEME				m_hThemeScrollBar;
	HTHEME				m_hThemeExplorerBar;
	HTHEME				m_hThemeTree;
	HTHEME				m_hThemeStartPanel;
	HTHEME				m_hThemeTaskBand;
	HTHEME				m_hThemeTaskBar;
	HTHEME				m_hThemeSpin;

	HINSTANCE			m_hinstUXDLL;

	OPENTHEMEDATA		m_pfOpenThemeData;
	CLOSETHEMEDATA		m_pfCloseThemeData;
	DRAWTHEMEBACKGROUND	m_pfDrawThemeBackground;
	GETTHEMECOLOR		m_pfGetThemeColor;
	GETTHEMESYSCOLOR	m_pfGetThemeSysColor;
	GETCURRENTTHEMENAME	m_pfGetCurrentThemeName;
	GETWINDOWTHEME		m_pfGetWindowTheme;
};

class BCGCBPRODLLEXPORT CBCGPVisualManager : public CObject
{
	friend class CBCGPSkinManager;

	DECLARE_DYNCREATE (CBCGPVisualManager)

public:
	CBCGPVisualManager(BOOL bTemporary = FALSE);
	virtual ~CBCGPVisualManager();

	static void SetDefaultManager (CRuntimeClass* pRTI);

	virtual BOOL IsWinXPThemeSupported () const	{	return FALSE;	}

// Operations:
public:
	static void RedrawAll ();

// Overrides:
public:
	virtual void OnUpdateSystemColors () {};

	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect);
	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);
	virtual void OnDrawMenuShadow (CDC* pDC, const CRect& rectClient, const CRect& rectExclude,
									int nDepth,  int iMinBrightness,  int iMaxBrightness,  
									CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight);
	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);
	virtual void OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz);
	virtual COLORREF OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons);
	virtual void OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive, BOOL bHorz,
										BOOL bMaximized, BOOL bDisabled, int nImageID = -1);
	virtual void OnDrawMenuSystemButton (CDC* pDC, CRect rect, UINT uiSystemCommand, 
										UINT nStyle, BOOL bHighlight);
	virtual void OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* pBar,
					CRect rectPane, UINT uiID, UINT nStyle);
	virtual void OnDrawStatusBarProgress (CDC* pDC, CBCGPStatusBar* /*pStatusBar*/,
										CRect rectProgress, int nProgressTotal, 
										int nProgressCurr,
										COLORREF clrBar, 
										COLORREF clrProgressBarDest, 
										COLORREF clrProgressText,
										BOOL bProgressText);
	virtual void OnDrawStatusBarSizeBox (CDC* pDC, CBCGPStatusBar* pStatBar,
				CRect rectSizeBox);

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

	enum BCGBUTTON_STATE
	{
		ButtonsIsRegular,
		ButtonsIsPressed,
		ButtonsIsHighlighted,
	};

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

	virtual COLORREF GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												CBCGPVisualManager::BCGBUTTON_STATE state);

	// Caption bar:
	virtual COLORREF GetCaptionBarTextColor (CBCGPCaptionBar* pBar);
	virtual void OnDrawCaptionBarBorder (CDC* pDC, CBCGPCaptionBar* pBar, CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder);

	// Outlook bar:
	virtual void OnFillOutlookPageButton (CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText);
	virtual void OnDrawOutlookPageButtonBorder (CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed);
	virtual void OnDrawOutlookBarSplitter (CDC* pDC, CRect rectSplitter);
	virtual void OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, COLORREF& clrText);

	// Tab overrides:
	virtual void OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnDrawTab (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill, int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnDrawTabContent (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd,
							COLORREF clrText);
	virtual void OnEraseTabsButton (CDC* pDC, CRect rect, CBCGPButton* pButton,
									CBCGPBaseTabWnd* pWndTab);
	virtual void OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
									CBCGPButton* pButton, UINT uiState, CBCGPBaseTabWnd* pWndTab);
	virtual void GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack);
	virtual BOOL OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual BOOL IsHighlightOneNoteTabs () const	{	return FALSE;	}

	// Customization dialog:
	virtual COLORREF OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected = FALSE);
	virtual void OnDrawMenuArrowOnCustomizeList (CDC* pDC, CRect rectCommand, BOOL bSelected);

	virtual CSize GetButtonExtraBorder () const
	{
		return CSize (0, 0);
	}

	virtual CSize GetCaptionButtonExtraBorder () const
	{
		return CSize (0, 0);
	}

	virtual COLORREF OnFillMiniFrameCaption (CDC* pDC, CRect rectCaption, 
											CBCGPMiniFrameWnd* pFrameWnd,
											BOOL bActive);
	virtual void OnDrawMiniFrameBorder (CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize);
	virtual void OnDrawFloatingToolbarBorder (	CDC* pDC, CBCGPBaseToolBar* pToolBar, 
												CRect rectBorder, CRect rectBorderSize);
	// Header control:
	virtual void OnFillHeaderCtrlBackground (CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect rect);
	virtual void OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC,
								CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted);
	virtual void OnDrawHeaderCtrlSortArrow (CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsUp);

	// Tasks pane:
	virtual void OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea);

	virtual void OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, BOOL bCanCollapse = FALSE);

	virtual void OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL bSpecial = FALSE);
	virtual void OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial = FALSE, BOOL bNoTitle = FALSE);
	virtual void OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE);

	virtual void OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited);

	// Slider
	virtual void OnDrawSlider (CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode);

	// Property list:
	virtual void OnDrawExpandingBox (CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox);

	// Splitter:
	virtual void OnDrawSplitterBorder (CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect rect);
	virtual void OnDrawSplitterBox (CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect& rect);
	virtual void OnFillSplitterBackground (CDC* pDC, CBCGPSplitterWnd* pSplitterWnd, CRect rect);

	// Date/time controls:
	virtual void OnFillCalendarBarWeekColumn (CDC* pDC, CRect rect,
		COLORREF& clrText, COLORREF& clrTextDisabled);

	virtual COLORREF OnFillCalendarBarNavigationRow (CDC* pDC, CRect rect);
	virtual void OnDrawCheckBox (CDC *pDC, CRect rect, 
										 BOOL bHighlighted, 
										 BOOL bChecked,
										 BOOL bEnabled);

	virtual void OnDrawControlBorder (CWnd* pWndCtrl);

	// Auto-hide buttons:
	virtual void OnFillAutoHideButtonBackground (CDC* pDC, CRect rect, CBCGPAutoHideButton* pButton);
	virtual void OnDrawAutoHideButtonBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* pButton);

	// Calculator:
	virtual BOOL OnDrawCalculatorButton (CDC* pDC, CRect rect, CBCGPToolbarButton* pButton, CBCGPVisualManager::BCGBUTTON_STATE state, int cmd /* CBCGPCalculator::CalculatorCommands */, CBCGPCalculator* pCalculator);
	virtual BOOL OnDrawCalculatorDisplay (CDC* pDC, CRect rect, const CString& strText, BOOL bMem, CBCGPCalculator* pCalculator);

	// Edit box:
	virtual BOOL OnDrawBrowseButton (CDC* pDC, CRect rect, CBCGPEdit* pEdit, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& clrText);

	// Spin control:
	virtual void OnDrawSpinButtons(CDC* pDC, CRect rectSpin, int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* pSpinCtrl);

	// Windows XP drawing methods:
	virtual BOOL DrawPushButtonWinXP (CDC* /*pDC*/, CRect /*rect*/, CBCGPButton* /*pButton*/, UINT /*uiState*/)	{	return FALSE;	}
	virtual BOOL DrawComboDropButtonWinXP (CDC* /*pDC*/, CRect /*rect*/,
										BOOL /*bDisabled*/,
										BOOL /*bIsDropped*/,
										BOOL /*bIsHighlighted*/)	{	return FALSE;	}
	virtual BOOL DrawComboBorderWinXP (CDC* /*pDC*/, CRect /*rect*/,
										BOOL /*bDisabled*/,
										BOOL /*bIsDropped*/,
										BOOL /*bIsHighlighted*/)	{	return FALSE;	}

// Attributes:
public:

	void SetMenuShadowDepth (int nDepth)	// Pixels
	{
		m_nMenuShadowDepth = nDepth;
	}

	int GetMenuShadowDepth () const
	{
		return m_nMenuShadowDepth;
	}
	
	static CBCGPVisualManager* GetInstance ()
	{
		if (m_pVisManager != NULL)
		{
			ASSERT_VALID (m_pVisManager);
			return m_pVisManager;
		}

		if (m_pRTIDefault == NULL)
		{
			m_pVisManager = new CBCGPVisualManager;
		}
		else
		{
			m_pVisManager = (CBCGPVisualManager*) m_pRTIDefault->CreateObject ();
			ASSERT_VALID (m_pVisManager);
		}

		m_pVisManager->m_bAutoDestroy = TRUE;
		m_pVisManager->OnUpdateSystemColors ();

		return m_pVisManager;
	}

	BOOL IsLook2000 () const			{	return m_bLook2000; }
	void SetLook2000 (BOOL bLook2000 = TRUE);

	BOOL IsMenuFlatLook () const			{	return m_bMenuFlatLook; }
	void SetMenuFlatLook (BOOL bMenuFlatLook = TRUE)
	{
		m_bMenuFlatLook = bMenuFlatLook;
		RedrawAll ();
	}

	BOOL IsAutoDestroy () const
	{
		return m_bAutoDestroy;
	}

	void SetShadowHighlightedImage (BOOL bShadow = TRUE)
	{
		m_bShadowHighlightedImage = bShadow;
	}

	BOOL IsShadowHighlightedImage () const
	{
		return m_bShadowHighlightedImage;
	}

	void EnableToolbarButtonFill (BOOL bEnable = TRUE)
	{
		m_bEnableToolbarButtonFill = bEnable;
	}

	BOOL IsToolbarButtonFillEnabled () const
	{
		return m_bEnableToolbarButtonFill;
	}

	BOOL IsEmbossDisabledImage () const
	{
		return m_bEmbossDisabledImage;
	}

	void SetEmbossDisabledImage (BOOL bEmboss = TRUE)
	{
		m_bEmbossDisabledImage = bEmboss;
	}

	BOOL IsFadeInactiveImage () const
	{
		return m_bFadeInactiveImage;
	}

	void SetFadeInactiveImage (BOOL bFade = TRUE)
	{
		m_bFadeInactiveImage = bFade;
	}

	virtual int GetMenuImageMargin () const
	{
		return 2;
	}

	virtual BOOL IsLook2000Allowed () const
	// Allows choose "Look 2000" in the customization dialog
	{
		return TRUE;
	}

	// TasksPane:
	int GetTasksPaneVertMargin() const
	{
		return m_nVertMargin;
	}

	int GetTasksPaneHorzMargin() const
	{
		return m_nHorzMargin;
	}

	int GetTasksPaneGroupVertOffset() const
	{
		return m_nGroupVertOffset;
	}

	int GetTasksPaneGroupCaptionHeight() const
	{
		return m_nGroupCaptionHeight;
	}

	int GetTasksPaneGroupCaptionHorzOffset() const
	{
		return m_nGroupCaptionHorzOffset;
	}

	int GetTasksPaneGroupCaptionVertOffset() const
	{
		return m_nGroupCaptionVertOffset;
	}

	int GetTasksPaneTaskHorzOffset() const
	{
		return m_nTasksHorzOffset;
	}

	int GetTasksPaneIconHorzOffset() const
	{
		return m_nTasksIconHorzOffset;
	}

	int GetTasksPaneIconVertOffset() const
	{
		return m_nTasksIconVertOffset;
	}

	virtual int GetToolBarCustomizeButtonMargin () const
	{
		return 2;
	}

	virtual BOOL IsOffsetPressedButton () const
	{
		return TRUE;
	}

	virtual BOOL IsOfficeXPStyleMenus () const
	{
		return m_bOfficeXPStyleMenus;
	}

	virtual BOOL GetPopupMenuBorderSize () const
	{
		return m_nMenuBorderSize;
	}

	// "Show all menu items" methods:
	virtual int GetShowAllMenuItemsHeight (CDC* pDC, const CSize& sizeDefault);
	virtual void OnDrawShowAllMenuItems (CDC* pDC, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

protected:
	static CBCGPVisualManager* CreateVisualManager (CRuntimeClass* pVisualManager);
	static void DestroyInstance ();

// Attributes:
protected:
	static CRuntimeClass*		m_pRTIDefault;
	static CBCGPVisualManager*	m_pVisManager;

	BOOL	m_bLook2000;				// Single grippers
	int		m_nMenuShadowDepth;
	BOOL	m_bMenuFlatLook;			// Menu item is always stil unpressed
	BOOL	m_bShadowHighlightedImage;
	BOOL	m_bEmbossDisabledImage;
	BOOL	m_bFadeInactiveImage;
	BOOL	m_bEnableToolbarButtonFill;

	BOOL	m_bIsTemporary;

	int		m_nVertMargin;
	int		m_nHorzMargin;
	int		m_nGroupVertOffset;
	int		m_nGroupCaptionHeight;
	int		m_nGroupCaptionHorzOffset;
	int		m_nGroupCaptionVertOffset;
	int		m_nTasksHorzOffset;
	int		m_nTasksIconHorzOffset;
	int		m_nTasksIconVertOffset;
	BOOL	m_bActiveCaptions;

	BOOL	m_bOfficeXPStyleMenus;
	int		m_nMenuBorderSize;

private:
	BOOL	m_bAutoDestroy;
};


#endif // !defined(AFX_BCGPVISUALMANAGER_H__8070C021_60CE_11D5_A304_8B7130518C10__INCLUDED_)
