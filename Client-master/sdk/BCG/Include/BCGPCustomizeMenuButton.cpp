// BCGPCustomizeMenuButton.cpp: implementation of the CBCGPCustomizeMenuButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgprores.h"
#include "bcgcbpro.h"
#include "BCGPCustomizeMenuButton.h"
#include "CustomizeButton.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPKeyboardManager.h"
#include "resource.h"
#include "BCGPLocalResource.h"
#include "afxtempl.h"
#include "BCGPVisualManagerXP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CBCGPCustomizeMenuButton, CBCGPToolbarMenuButton)

CMap<UINT, UINT, int, int>	 CBCGPCustomizeMenuButton::m_mapPresentIDs;
CBCGPToolBar* CBCGPCustomizeMenuButton::m_pWndToolBar = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CBCGPCustomizeMenuButton::m_bRecentlyUsedOld = FALSE;


CBCGPCustomizeMenuButton::CBCGPCustomizeMenuButton()
{
	
}
//****************************************************************************************
CBCGPCustomizeMenuButton::~CBCGPCustomizeMenuButton()
{
	
}
//****************************************************************************************
CBCGPCustomizeMenuButton::CBCGPCustomizeMenuButton(UINT uiID,HMENU hMenu,int iImage,LPCTSTR lpszText,BOOL bUserButton):
	CBCGPToolbarMenuButton (uiID, hMenu/* HMENU */, iImage /*iImage*/, lpszText, bUserButton)
{
	m_uiIndex = (UINT)-1;
	bSeparator = FALSE;
	m_bAddSpr = FALSE;
	m_bIsEnabled = TRUE;
	m_bBrothersBtn = FALSE;
}
//****************************************************************************************
void CBCGPCustomizeMenuButton::SetItemIndex(UINT uiIndex, BOOL bExist, BOOL bAddSpr)
{
	m_uiIndex = uiIndex;
	m_bExist = bExist;
	m_bAddSpr = bAddSpr;
	
	if((uiIndex != ID_BCGBARRES_TOOLBAR_RESET_PROMT)
		&& !bSeparator && bExist)
	{
		CBCGPToolbarButton* pBtn = m_pWndToolBar->GetButton(uiIndex);
		m_bShow = pBtn->IsVisible();
		
	}else
	{
		m_bShow = FALSE;
	}
}
//****************************************************************************************
void CBCGPCustomizeMenuButton::CopyFrom (const CBCGPToolbarButton& s)
{
	CBCGPToolbarButton::CopyFrom (s);
	const CBCGPCustomizeMenuButton& src = (const CBCGPCustomizeMenuButton&) s;
	
	m_uiIndex      =   src.m_uiIndex;
	m_bShow        =   src.m_bShow;
	m_pWndToolBar  =   src.m_pWndToolBar;
	bSeparator     =   src.bSeparator;
	m_bExist       =   src.m_bExist;
	m_bAddSpr      =   src.m_bAddSpr;
	m_bIsEnabled   =   src.m_bIsEnabled;
	m_bBrothersBtn =   src.m_bBrothersBtn;	
}
//****************************************************************************************
SIZE CBCGPCustomizeMenuButton::OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	if (bSeparator)
	{
		return CSize(0,  4);
	}
	
	if (m_bBrothersBtn)
	{
		return CBCGPToolbarMenuButton::OnCalculateSize(pDC, sizeDefault, bHorz);
	}
	
	//-----------------------------
	//  Try to Find Buttons Text
	//-----------------------------
	if (m_strText.IsEmpty ())
	{
		//-------------------------------------------
		// Try to find the command name in resources:
		//-------------------------------------------
		CString strMessage;
		int iOffset;
		if (strMessage.LoadString (m_nID) &&
			(iOffset = strMessage.Find (_T('\n'))) != -1)
		{
			m_strText = strMessage.Mid (iOffset + 1);
		}
	}
	else
	{
		// m_strText.Remove (_T('&'));
		
		//----------------------------------------
		// Remove trailing label (ex.:"\tCtrl+S"):
		//----------------------------------------
		int iOffset = m_strText.Find (_T('\t'));
		if (iOffset != -1)
		{
			m_strText = m_strText.Left (iOffset);
		}
	}
	
	//--------------------
	// Change accelerator:
	//--------------------
	if (g_pKeyboardManager != NULL &&
		m_bMenuMode &&
		(m_nID < 0xF000 || m_nID >= 0xF1F0))	// Not system.
	{
		//-----------------------------------
		// Remove standard aceleration label:
		//-----------------------------------
		int iTabOffset = m_strText.Find (_T('\t'));
		if (iTabOffset >= 0)
		{
			m_strText = m_strText.Left (iTabOffset);
		}
		
		//---------------------------------
		// Add an actual accelartion label:
		//---------------------------------
		CString strAccel;
		CFrameWnd* pParent = m_pWndParent == NULL ?
			DYNAMIC_DOWNCAST (CFrameWnd, AfxGetMainWnd ()) :
		BCGCBProGetTopLevelFrame (m_pWndParent);
		
		if (pParent != NULL &&
			(CBCGPKeyboardManager::FindDefaultAccelerator (
			m_nID, strAccel, pParent, TRUE) ||
			CBCGPKeyboardManager::FindDefaultAccelerator (
			m_nID, strAccel, pParent->GetActiveFrame (), FALSE)))
		{
			m_strText += _T('\t');
			m_strText += strAccel;
		}
	}
	
	
	
	int nTolalWidth = m_strText.GetLength();
	TEXTMETRIC tm;
	pDC->GetTextMetrics (&tm);
	nTolalWidth *= tm.tmAveCharWidth;
	CSize sizeImage = CBCGPToolBar::GetMenuButtonSize();
	nTolalWidth += 2*sizeImage.cx;	
	nTolalWidth += 3*CBCGPVisualManager::GetInstance ()->GetMenuImageMargin () + 50;
	
	
	int nTotalHeight = sizeImage.cy;
	
	if (!m_bMenuMode)
	{
		nTotalHeight += CBCGPVisualManager::GetInstance ()->GetMenuImageMargin ();
	}
	
	return CSize(nTolalWidth,  nTotalHeight);
}
//****************************************************************************************
BOOL CBCGPCustomizeMenuButton::OnClickMenuItem()
{
	if (bSeparator || !m_bIsEnabled)
	{
		return TRUE;
	}
	
	CBCGPPopupMenuBar* pMenuBar = (CBCGPPopupMenuBar*)m_pWndParent;
	int nIndex = pMenuBar->ButtonToIndex(this);
	if (nIndex !=-1)
	{
		if (pMenuBar->m_iHighlighted != nIndex)
		{
			pMenuBar->m_iHighlighted = nIndex;
			pMenuBar->InvalidateRect (this->Rect ());
		}
	}

	if (m_bBrothersBtn) 
	{
		if (m_pWndToolBar->IsOneRowWithBrother ())
		{
			m_pWndToolBar->SetTwoRowsWithBrother ();
		}
		else
		{
			m_pWndToolBar->SetOneRowWithBrother ();
		}

		return FALSE;
	}
	
	if (m_uiIndex == ID_BCGBARRES_TOOLBAR_RESET_PROMT) // reset pressed
	{
		//load default toolbar
		m_pWndToolBar->PostMessage (BCGM_RESETRPROMPT);
		return FALSE;	
	}
	
	if (!m_bExist)
	{	
		const CObList& lstOrignButtons = m_pWndToolBar->GetOrigResetButtons ();
		
		POSITION pos = lstOrignButtons.FindIndex (m_uiIndex);
		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*)lstOrignButtons.GetAt (pos);
		if (pButton == NULL)
		{
			return TRUE;
		}
		
		UINT nIndex = m_pWndToolBar->InsertButton(*pButton, m_uiIndex);
		
		if (nIndex == -1)
		{
			nIndex = m_pWndToolBar->InsertButton(*pButton);	
		}
		else
		{
			CBCGPPopupMenuBar* pMenuBar = (CBCGPPopupMenuBar*)m_pWndParent;
			int nCount = pMenuBar->GetCount ();
			for (int i=0; i< nCount; i++)
			{
				CBCGPCustomizeMenuButton* pBtn = (CBCGPCustomizeMenuButton*)pMenuBar->GetButton(i);
				if ((pBtn->m_uiIndex >= nIndex) && 
					(pBtn->m_uiIndex != ID_BCGBARRES_TOOLBAR_RESET_PROMT))
				{
					if (pBtn->m_bExist)
					{
						pBtn->m_uiIndex += 1; 
					}
				}
			}
		}

		m_uiIndex = nIndex;
		
		if (m_bAddSpr) 
		{
			if (nIndex < (UINT)m_pWndToolBar->GetCount ())
			{
				CBCGPToolbarButton* pBtn = m_pWndToolBar->GetButton (nIndex+1);
				if (!(pBtn->m_nStyle & TBBS_SEPARATOR))
				{
					m_pWndToolBar->InsertSeparator ();
				}	
			}
			else
			{
				m_pWndToolBar->InsertSeparator ();
			}
		}

		m_pWndToolBar->AdjustLayout ();
		m_pWndToolBar->AdjustSizeImmediate ();
		UpdateCustomizeButton ();
		
		m_bExist = TRUE;
		m_bShow = TRUE;
		CBCGPPopupMenuBar* pMenuBar = (CBCGPPopupMenuBar*)m_pWndParent;
		pMenuBar->Invalidate ();

		return TRUE;	
	}
	
	CBCGPToolbarButton* pBtn = m_pWndToolBar->GetButton (m_uiIndex);
	BOOL bVisible = pBtn->IsVisible ();
	pBtn->SetVisible (!bVisible);
	m_bShow = !bVisible;
	
	//-------------------------------------
	//  Make next Separator the same state
	//-------------------------------------
	int nNext = m_uiIndex + 1;
	if (nNext < m_pWndToolBar->GetCount ())
	{
		CBCGPToolbarButton* pBtnNext = m_pWndToolBar->GetButton (nNext);
		if (pBtnNext->m_nStyle & TBBS_SEPARATOR)
		{
			pBtnNext->SetVisible (!bVisible);	
		}	
	}
	
	m_pWndToolBar->AdjustLayout();
	m_pWndToolBar->AdjustSizeImmediate ();
	UpdateCustomizeButton();
	pMenuBar->Invalidate();
	
	CCustomizeButton* pCustBtn = m_pWndToolBar->GetCustomizeButton();
	CBCGPPopupMenu* pMenu = pCustBtn->GetPopupMenu();
	if (pMenu != NULL)
	{
		pMenu->RedrawWindow ();
	}
	
	return TRUE;
}
//****************************************************************************************
void CBCGPCustomizeMenuButton::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
									   BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,
									   BOOL bDrawBorder, BOOL bGrayDisabledButtons)
{
	ASSERT_VALID (pDC);
	
	//-----------------------------
	// Draw Separator
	//-----------------------------
	if(bSeparator)
	{
		CRect rcSeparator(rect);
		rcSeparator.left = 2*CBCGPToolBar::GetMenuImageSize ().cx + 
			CBCGPVisualManager::GetInstance ()->GetMenuImageMargin ();
		
		CBCGPPopupMenuBar* pMenuBar = (CBCGPPopupMenuBar*)m_pWndParent;
		CBCGPVisualManager::GetInstance ()->OnDrawSeparator (pDC, pMenuBar, rcSeparator, FALSE);
		
		return;
	}

	if (m_bBrothersBtn)
	{
		CBCGPToolbarMenuButton::OnDraw (pDC, rect, NULL,
									   bHorz, bCustomizeMode, bHighlight,
									   bDrawBorder, bGrayDisabledButtons);
		return;
	}
	
	CRect rectDefault(rect);
	CRect rectCheck = rectDefault;
		  rectCheck.bottom -= 2;
		  rectCheck.left += 2;
		  rectCheck.right = rectCheck.left + rectCheck.Height();
		  
	CRect rectFrameBtn = rectCheck;
		  
	if (m_bIsEnabled)
	{	  
		if (m_bShow && bHighlight)
		{		  
			SetStyle (TBBS_BUTTON|TBBS_CHECKED);
		}
		else
		{	  
			SetStyle (TBBS_BUTTON);
		 }		  
	}
	else
	{
		SetStyle (TBBS_DISABLED);
		bGrayDisabledButtons = TRUE;
		bHighlight = FALSE;
	}
		  
	 //-------------------------------
	 //	Highlight All Button if needed
	 //-------------------------------
	 if (bHighlight && m_bIsEnabled)
	 {
		CRect rcHighlight = rectDefault;
		rcHighlight.bottom -= 2;
		rcHighlight.left += 2;
			 
		 COLORREF clrText;

		if (GetRValue (globalData.clrBtnText) > 128 &&
				GetGValue (globalData.clrBtnText) > 128 &&
				GetBValue (globalData.clrBtnText) > 128)
		 {
			clrText = RGB (0, 0, 0);
		 }
		 else
		 {
			clrText = RGB (255, 255, 255);
		 }
			  
		 CRuntimeClass* pRTC = CBCGPVisualManager::GetInstance ()->GetRuntimeClass();

		 if(!pRTC->IsDerivedFrom(RUNTIME_CLASS(CBCGPVisualManagerXP)) && (m_nStyle & TBBS_CHECKED))
		 {
			  rcHighlight.left += CBCGPToolBar::GetMenuImageSize ().cx + 
					3*CBCGPVisualManager::GetInstance ()->GetMenuImageMargin ();
		 }
			 
		 CBCGPVisualManager::GetInstance ()->OnHighlightMenuItem (pDC, this, rcHighlight, clrText);	 
			 
	}
		  	  
	//---------------------------------
	// Draw CheckBox for Visible Items
	//--------------------------------
	DrawCheckBox (pDC, rectCheck, bHighlight);
		  	  
	if (bHighlight && !(m_nStyle & TBBS_DISABLED))
	{
		SetStyle (TBBS_BUTTON);	  
	}
		   
	CRect rectStdMenu(rect);
		rectStdMenu.left += rectCheck.right;
		rectStdMenu.bottom -= 2;
  
	DrawMenuItem (pDC, rectStdMenu, pImages, bCustomizeMode, bHighlight, bGrayDisabledButtons);		  
}
//****************************************************************************************
void CBCGPCustomizeMenuButton::DrawMenuItem(CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
											BOOL bCustomizeMode, BOOL bHighlight, BOOL bGrayDisabledButtons)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (this);
	
	BOOL bDisableImage = CMD_MGR.IsMenuItemWithoutImage (m_nID);
	
	CBCGPUserTool* pUserTool = NULL;
	if (g_pUserToolsManager != NULL && !m_bUserButton)
	{
		pUserTool = g_pUserToolsManager->FindTool (m_nID);
	}
	
	CSize sizeImage = CMenuImages::Size ();
	
	BOOL bDisabled = (bCustomizeMode && !IsEditable ()) ||
		(!bCustomizeMode && (m_nStyle & TBBS_DISABLED));
	
	if (m_pPopupMenu != NULL && !m_bToBeClosed)
	{
		bHighlight = TRUE;
	}
	
	CFont* pOldFont = NULL;
	
	CBCGPPopupMenuBar* pParentMenu =
		DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, m_pWndParent);
	if (m_nID != 0 && m_nID != (UINT) -1 && !m_bMenuOnly && 
		pParentMenu != NULL && pParentMenu->GetDefaultMenuId () == m_nID)
	{
		pOldFont = (CFont*) pDC->SelectObject (&globalData.fontBold);
	}
	
	CRect rectImage;
	rectImage = rect;
	
	rectImage.left += CBCGPVisualManager::GetInstance ()->GetMenuImageMargin ();
	rectImage.right = rectImage.left + CBCGPToolBar::GetMenuImageSize ().cx + 
		CBCGPVisualManager::GetInstance ()->GetMenuImageMargin ();
	
	
	CRect rectFrameBtn = rectImage;
	rectFrameBtn.InflateRect (1, -1);
	
	BOOL bIsRarelyUsed = (CBCGPMenuBar::IsRecentlyUsedMenus () && 
		CBCGPToolBar::IsCommandRarelyUsed (m_nID));
	
	
	if (bIsRarelyUsed)
	{
		bIsRarelyUsed = FALSE;
		
		CBCGPPopupMenuBar* pParentMenuBar =
			DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, m_pWndParent);
		
		if (pParentMenuBar != NULL)
		{
			CBCGPPopupMenu* pParentMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, 
				pParentMenuBar->GetParent ());
			if (pParentMenu != NULL && pParentMenu->HideRarelyUsedCommands ())
			{
				bIsRarelyUsed = TRUE;
			}
		}
	}
	
	BOOL bLightImage = FALSE;
	BOOL bFadeImage = !bHighlight && CBCGPVisualManager::GetInstance ()->IsFadeInactiveImage ();
	
	
	if (bIsRarelyUsed)
	{
		bLightImage = TRUE;
		if (bHighlight && (m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
		{
			bLightImage = FALSE;
		}
		
		if (GetImage () < 0 && !(m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
		{
			bLightImage = FALSE;
		}
	}
	else if (m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE))
	{
		bLightImage = !bHighlight;
	}
	
	//----------------
	// Draw the image:
	//----------------
	if (!IsDrawImage ())	// Try to find a matched image
	{
		BOOL bImageSave = m_bImage;
		BOOL bUserButton = m_bUserButton;
		BOOL bSuccess = TRUE;
		
		m_bImage = TRUE;	// Always try to draw image!
		m_bUserButton = TRUE;
		
		if (GetImage () < 0)
		{
			m_bUserButton = FALSE;
			
			if (GetImage () < 0)
			{
				bSuccess = FALSE;
			}
		}
		
		if (!bSuccess)
		{
			m_bImage = bImageSave;
			m_bUserButton = bUserButton;
		}
	}
		
	if(m_uiIndex == ID_BCGBARRES_TOOLBAR_RESET_PROMT)
	{
		m_bImage = FALSE;
	}
	
	BOOL bImageIsReady = FALSE;
	
	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect (&rectImage);
	
	if(m_iImage != -1)
	{
		FillInterior (pDC, rectFrameBtn, bHighlight);
	}
	
	if (!bDisableImage && IsDrawImage () && pImages != NULL)
	{
		BOOL bDrawImageShadow = 
			bHighlight && !bCustomizeMode &&
			CBCGPVisualManager::GetInstance ()->IsShadowHighlightedImage () &&
			((m_nStyle & TBBS_CHECKED) == 0) &&
			((m_nStyle & TBBS_DISABLED) == 0);
		
		pDC->SelectObject (&rgnClip);
		
		CPoint ptImageOffset (
			(rectImage.Width () - CBCGPToolBar::GetMenuImageSize ().cx) / 2, 
			(rectImage.Height () - CBCGPToolBar::GetMenuImageSize ().cy) / 2);
		
		if ((m_nStyle & TBBS_CHECKED) && !bCustomizeMode &&
			!CBCGPVisualManager::GetInstance ()->IsShadowHighlightedImage ())
		{
			ptImageOffset.x ++;
			ptImageOffset.y ++;
			
		}
		
		if ((m_nStyle & TBBS_PRESSED) || 
			!(m_nStyle & TBBS_DISABLED) ||
			!bGrayDisabledButtons ||
			bCustomizeMode)
		{
			if (pUserTool != NULL)
			{
				pUserTool->DrawToolIcon (pDC, 
					CRect (CPoint (rectImage.left + ptImageOffset.x, 
					rectImage.top + ptImageOffset.y),
					CBCGPToolBar::GetMenuImageSize ()));
			}
			else
			{
				CPoint pt = rectImage.TopLeft ();
				pt += ptImageOffset;
				
				if (bDrawImageShadow)
				{
					pImages->Draw (pDC, 
						pt.x,
						pt.y, 
						GetImage (), FALSE, FALSE, FALSE, TRUE);
					
					pt.Offset (-1, -1);
				}
				
				pImages->Draw (pDC, 
					pt.x, 
					pt.y, 
					GetImage ());
			}
			
			if (bCustomizeMode || m_nStyle & TBBS_PRESSED)
			{
				bImageIsReady = TRUE;
			}
		}
		
		if (!bImageIsReady)
		{
			if (pUserTool != NULL)
			{
				pUserTool->DrawToolIcon (pDC, 
					CRect (CPoint (rectImage.left + ptImageOffset.x, 
					rectImage.top + ptImageOffset.y),
					CBCGPToolBar::GetMenuImageSize ()));
			}
			else
			{
				if (bDrawImageShadow)
				{
					pImages->Draw (pDC, 
						rectImage.left + ptImageOffset.x,
						rectImage.top + ptImageOffset.y,
						GetImage (), FALSE, FALSE, FALSE, TRUE);
					
					rectImage.OffsetRect (-1, -1);
				}
				
				pImages->Draw (pDC, 
					rectImage.left + ptImageOffset.x, rectImage.top + ptImageOffset.y, 
					GetImage (), FALSE, bDisabled && bGrayDisabledButtons,
					FALSE, FALSE, bFadeImage);
			}
			
			bImageIsReady = TRUE;
		}
	}
	
	/*	
	if (m_bAlwaysCallOwnerDraw || !bImageIsReady)
	{
	CFrameWnd* pParentFrame = m_pWndParent == NULL ?
	DYNAMIC_DOWNCAST (CFrameWnd, AfxGetMainWnd ()) :
	BCGCBProGetTopLevelFrame (m_pWndParent);
	
	  //------------------------------------
	  // Get chance to user draw menu image:
	  //------------------------------------
	  CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pParentFrame);
	  if (pMainFrame != NULL)
	  {
	  bImageIsReady = pMainFrame->OnDrawMenuImage (pDC, this, rectImage);
	  }
	  else	// Maybe, SDI frame...
	  {
	  CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pParentFrame);
	  if (pFrame != NULL)
	  {
	  bImageIsReady = pFrame->OnDrawMenuImage (pDC, this, rectImage);
	  }
	  else	// Maybe, OLE frame...
	  {
	  CBCGPOleIPFrameWnd* pOleFrame = 
	  DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, pParentFrame);
	  if (pOleFrame != NULL)
	  {
	  bImageIsReady = pOleFrame->OnDrawMenuImage (pDC, this, rectImage);
	  }
	  else
	  {
	  CBCGPOleDocIPFrameWnd* pOleDocFrame = 
						DYNAMIC_DOWNCAST (CBCGPOleDocIPFrameWnd, pParentFrame);
						if (pOleDocFrame != NULL)
						{
						bImageIsReady = pOleDocFrame->OnDrawMenuImage (pDC, this, rectImage);
						}
						}
						
						  }
						  }
						  }
						  */
						  
						  pDC->SelectClipRgn (NULL);
						  
						  rectImage.right ++;
						  
						  /*
						  if (m_nStyle & TBBS_CHECKED && !bImageIsReady)
						  {
						  CRect rectCheck = rectImage;
						  rectCheck.DeflateRect (0, 1);
						  
							if (!bLightImage && bIsRarelyUsed)
							{
							pDC->FillRect (rectCheck, &globalData.brBtnFace);
							}
							
							  CPoint pointCheck (
							  rectCheck.left + (rectCheck.Width () - sizeImage.cx) / 2,
							  rectCheck.top + (rectCheck.Height () - sizeImage.cy) / 2);
							  
								int iImage = m_bIsRadio ? CMenuImages::IdRadio : CMenuImages::IdCheck;
								
								  FillInterior (pDC, rectCheck, bHighlight);
								  CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, 
								  pointCheck,
								  (m_nStyle & TBBS_DISABLED) ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
								  
									CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
									this, rectFrameBtn, CBCGPVisualManager::ButtonsIsPressed);
									}
									else if (bImageIsReady && bHighlight && !(m_nStyle & TBBS_CHECKED))
									{
									CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
									this, rectFrameBtn, CBCGPVisualManager::ButtonsIsHighlighted);
									}
									else if (bImageIsReady && (m_nStyle & TBBS_CHECKED))
									{
									CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
									this, rectFrameBtn, CBCGPVisualManager::ButtonsIsPressed);
									}
						  */
						  
						  int iSystemImageId = -1;
						  /*
						  //-------------------------------
						  // Try to draw system menu icons:
						  //-------------------------------
						  if (!bImageIsReady)
						  {
						  switch (m_nID)
						  {
						  case SC_MINIMIZE:
						  iSystemImageId = CMenuImages::IdMinimize;
						  break;
						  
							case SC_RESTORE:
							iSystemImageId = CMenuImages::IdRestore;
							break;
							
							  case SC_CLOSE:
							  iSystemImageId = CMenuImages::IdClose;
							  break;
							  
								case SC_MAXIMIZE:
								iSystemImageId = CMenuImages::IdMaximize;
								break;
								}
								
								  if (iSystemImageId != -1)
								  {
								  CRect rectSysImage = rectImage;
								  rectSysImage.DeflateRect (CBCGPVisualManager::GetInstance ()->GetMenuImageMargin (), CBCGPVisualManager::GetInstance ()->GetMenuImageMargin ());
								  
									
									  CPoint pointSysImage (
									  rectSysImage.left + (rectSysImage.Width () - sizeImage.cx) / 2,
									  rectSysImage.top + (rectSysImage.Height () - sizeImage.cy) / 2);
									  
										FillInterior (pDC, rectFrameBtn, bHighlight);
										CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iSystemImageId, pointSysImage,
										bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
										
										  if (bHighlight)
										  {
										  CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
										  this, rectFrameBtn, CBCGPVisualManager::ButtonsIsHighlighted);
										  }
										  }
										  }
						  */
						  
						  //-------------------------------
						  // Fill text area if highlighted:
						  //-------------------------------
						  CRect rectText = rect;
						  rectText.left = rectFrameBtn.right + CBCGPVisualManager::GetInstance ()->GetMenuImageMargin () + 2;
						  
						  
						  COLORREF clrText;
						  
						  if (bHighlight)
						  {
							  CRect rectFill = rectFrameBtn;
							  
							  if ((m_nStyle & (TBBS_CHECKED) || bImageIsReady) ||
								  iSystemImageId != -1)
							  {
								  rectFill.left = rectText.left - 1;
							  }
							  
							  rectFill.right = rect.right - 1;
							  
							 
							  CRuntimeClass* pRTC = CBCGPVisualManager::GetInstance ()->GetRuntimeClass();

							  if(!pRTC->IsDerivedFrom(RUNTIME_CLASS(CBCGPVisualManagerXP)))
							  {
								  clrText = globalData.clrTextHilite;					
	
							  }else
							  { 
								  clrText = globalData.clrBtnText;	 
							  }
							 
						  }
						  else
						  {
							  clrText	= bDisabled ?
								  globalData.clrGrayedText : 
							  globalData.clrBtnText;
						  }
						  
						  //-------------------------
						  // Find acceleration label:
						  //-------------------------
						  CString strText = m_strText;
						  CString strAccel;
						  
						  int iTabOffset = m_strText.Find (_T('\t'));
						  if (iTabOffset >= 0)
						  {
							  strText = strText.Left (iTabOffset);
							  strAccel = m_strText.Mid (iTabOffset + 1);
						  }
						  
						  //-----------
						  // Draw text:
						  //-----------
						  COLORREF clrTextOld = pDC->GetTextColor ();
						  
						  rectText.left += TEXT_MARGIN;
						  
						  if (!m_bWholeText)
						  {
							  CString strEllipses (_T("..."));
							  while (strText.GetLength () > 0 &&
								  pDC->GetTextExtent (strText + strEllipses).cx > rectText.Width ())
							  {
								  strText = strText.Left (strText.GetLength () - 1);
							  }
							  
							  strText += strEllipses;
						  }
						  
						  if (bDisabled && !bHighlight)
						  {
							  pDC->SetTextColor (globalData.clrBtnHilite);
							  
							  CRect rectShft = rectText;
							  rectShft.OffsetRect (1, 1);
							  pDC->DrawText (strText, &rectShft, DT_SINGLELINE | DT_VCENTER);
						  }
						  
						  pDC->SetTextColor (clrText);
						  pDC->DrawText (strText, &rectText, DT_SINGLELINE | DT_VCENTER);
						  
						  //------------------------
						  // Draw accelerator label:
						  //------------------------
						  if (!strAccel.IsEmpty ())
						  {
							  CRect rectAccel = rectText;
							  rectAccel.right -= TEXT_MARGIN + sizeImage.cx;
							  
							  if (bDisabled && !bHighlight)
							  {
								  pDC->SetTextColor (globalData.clrBtnHilite);
								  
								  CRect rectAccelShft = rectAccel;
								  rectAccelShft.OffsetRect (1, 1);
								  pDC->DrawText (strAccel, &rectAccelShft, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
							  }
							  
							  pDC->SetTextColor (clrText);
							  pDC->DrawText (strAccel, &rectAccel, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
						  }
						  
						  //--------------------------------------------
						  // Draw triangle image for the cascade menues:
						  //--------------------------------------------
						  if (m_nID == (UINT) -1 || m_bDrawDownArrow || m_bMenuOnly)
						  {
							  CFont* pRegFont = pDC->SelectObject (&globalData.fontMarlett);
							  ASSERT (pRegFont != NULL);
							  
							  CRect rectTriangle = rect;
							  
							  //const CString strTriangle (_T("6"));	// Marlett's right arrow
							  
							  CString strTriangle (_T("4")); 	// Marlett's right arrow
							  if(TRUE == m_bQuickCustomMode)
							  {
								  
								  strTriangle = _T("6");  	// Marlett's down arrow
							  }
							  
							  pDC->DrawText (strTriangle, &rectTriangle, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
							  
							  pDC->SelectObject (pRegFont);
						  }
						  
						  if (pOldFont != NULL)
						  {
							  pDC->SelectObject (pOldFont);
						  }
						  
						  pDC->SetTextColor (clrTextOld);
}
//****************************************************************************************
CString CBCGPCustomizeMenuButton::SearchCommandText(CMenu* pMenu, UINT in_uiCmd)
{
	ASSERT (pMenu != NULL);
	
	int iCount = (int) pMenu->GetMenuItemCount ();
	
	for (int i = 0; i < iCount; i ++)
	{
		UINT uiCmd = pMenu->GetMenuItemID (i);
		if (uiCmd == in_uiCmd)
		{
			CString strText;
			pMenu->GetMenuString (i, strText, MF_BYPOSITION);
			return strText;
		}
		
		switch (uiCmd)
		{
		case 0:		// Separator, ignore it.
			break;
			
		case -1:	// Submenu
			{
				CMenu* pSubMenu = pMenu->GetSubMenu (i);
				
				CString strText = SearchCommandText (pSubMenu, in_uiCmd);
				if(strText != _T("")) return strText;
			}
			break;
			
		}//end switch
	}//end for
	
	return _T("");
}
//****************************************************************************************
void CBCGPCustomizeMenuButton::DrawCheckBox(CDC* pDC, const CRect& rect, BOOL bHighlight)
{	
	CRect rectFrameBtn = rect;
	if (m_bShow && (!(m_nStyle & TBBS_DISABLED)))  
	{
		CRect rectCheck = rect;
		rectCheck.DeflateRect (1, 1);
				
		CPoint pointCheck (
			rect.left + ((rect.Width () / 2)-5),
			rect.top + ((rect.Height () / 2)-5));
		
		int iImage = CMenuImages::IdCheck;
		
		UINT nStyle = m_nStyle;
		m_nStyle |= TBBS_CHECKED;
		
		FillInterior (pDC, rectCheck, bHighlight);
		
		m_nStyle = nStyle;
		
		CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, 
			pointCheck,
			(m_nStyle & TBBS_DISABLED) ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
		
		
		CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
			this, rectFrameBtn, CBCGPVisualManager::ButtonsIsPressed);
	}
	else if (m_bShow && (m_nStyle & TBBS_DISABLED)) 
	{
		CRect rectCheck = rect;
		rectCheck.DeflateRect (0, 1);
		
		FillInterior (pDC, rectCheck, TRUE);
		
		CPoint pointCheck (
			rectCheck.left + (rectCheck.Width () )/ 4,
			rectCheck.top + (rectCheck.Height () )/ 4);
		
		int iImage = CMenuImages::IdCheck;
		CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, 
			pointCheck, CMenuImages::ImageGray );
		
		CRect rcFrame = rectFrameBtn;
		rcFrame.DeflateRect(0,0,1,1);
		
		CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
			this, rectFrameBtn, CBCGPVisualManager::ButtonsIsPressed);	
	}	
	else if (bHighlight && !(m_nStyle & TBBS_CHECKED))
	{
		//CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
		//	this, rectFrameBtn, CBCGPVisualManager::ButtonsIsHighlighted);
	}
	else if ((m_nStyle & TBBS_CHECKED))
	{
		CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC,
			this, rectFrameBtn, CBCGPVisualManager::ButtonsIsPressed);
	}
}
//****************************************************************************************
void CBCGPCustomizeMenuButton::UpdateCustomizeButton()
{
	CCustomizeButton* pCustomizeBtn = m_pWndToolBar->GetCustomizeButton ();
	if ((pCustomizeBtn != NULL) &&  (pCustomizeBtn->GetExtraSize () != CSize (0, 0)))
	{
		CRect rect = pCustomizeBtn->GetInvalidateRect ();
		rect.InflateRect (10, 10);
		m_pWndToolBar->InvalidateRect (rect);
		rect.InflateRect (pCustomizeBtn->GetExtraSize ());
		m_pWndToolBar->RedrawWindow (rect, NULL, RDW_FRAME | RDW_INVALIDATE);
	}	
}
//****************************************************************************************
BOOL CBCGPCustomizeMenuButton::IsCommandExist(UINT uiCmdId)
{
	int nTmp = 0;
	return m_mapPresentIDs.Lookup(uiCmdId, nTmp);
}
