//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of BCGControlBar Library Professional Edition
// Copyright (C) 1998-2008 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonMainPanel.cpp: implementation of the CBCGPRibbonMainPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPRibbonBar.h"
#include "BCGPRibbonMainPanel.h"
#include "BCGPRibbonButtonsGroup.h"
#include "BCGPRibbonCategory.h"
#include "BCGPVisualManager.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonPanelMenu.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonRecentFilesList

#define SEPARATOR_HEIGHT	4
#define FILE_MARGIN			4
#define LABEL_MARGIN		4

class CBCGPRibbonRecentFilesList : public CBCGPRibbonButtonsGroup
{
	DECLARE_DYNCREATE(CBCGPRibbonRecentFilesList)

public:
	CBCGPRibbonRecentFilesList(LPCTSTR lpszLabel = NULL)
	{
		SetText (lpszLabel == NULL ? _T("") : lpszLabel);
	}

	void FillList ();

protected:
	virtual void OnAfterChangeRect (CDC* pDC);
	virtual CSize GetRegularSize (CDC* pDC);
	virtual void OnDraw (CDC* pDC);
	virtual BOOL OnMenuKey (UINT nUpperChar);
};

IMPLEMENT_DYNCREATE(CBCGPRibbonRecentFilesList, CBCGPRibbonButtonsGroup)

void CBCGPRibbonRecentFilesList::OnAfterChangeRect (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	int y = m_rect.top + 2;

	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		pButton->SetParentMenu (m_pParentMenu);

		pButton->OnCalcTextSize (pDC);
		CSize sizeButton = pButton->GetSize (pDC);

		CRect rectButton = m_rect;
		rectButton.DeflateRect (1, 0);

		rectButton.top = y;
		rectButton.bottom = y + sizeButton.cy + 2 * FILE_MARGIN;

		pButton->SetRect (rectButton);
		pButton->OnAfterChangeRect (pDC);

		y = rectButton.bottom;
	}
}
//**********************************************************************************
CSize CBCGPRibbonRecentFilesList::GetRegularSize (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	int cy = 4;

	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		pButton->OnCalcTextSize (pDC);
		CSize sizeButton = pButton->GetSize (pDC);

		cy += sizeButton.cy + 2 * FILE_MARGIN;
	}

	const int nDefaultSize = 300;

	return CSize (
		globalData.GetRibbonImageScale () == 1. ? nDefaultSize : (int) (globalData.GetRibbonImageScale () *  nDefaultSize), 
		cy);
}

//-----------------------------------------------------
// My "classic " trick - how I can access to protected
// member m_pRecentFileList?
//-----------------------------------------------------
class CBCGPApp : public CWinApp
{
	friend class CBCGPRibbonRecentFilesList;
};

void CBCGPRibbonRecentFilesList::FillList ()
{
	ASSERT_VALID (this);

	RemoveAll ();

	//-----------
	// Add label:
	//-----------
	AddButton (new CBCGPRibbonLabel (m_strText));

	CRecentFileList* pMRUFiles = 
		((CBCGPApp*) AfxGetApp ())->m_pRecentFileList;

	TCHAR szCurDir [_MAX_PATH];
	::GetCurrentDirectory (_MAX_PATH, szCurDir);

	int nCurDir = lstrlen (szCurDir);
	ASSERT (nCurDir >= 0);

	szCurDir [nCurDir] = _T('\\');
	szCurDir [++ nCurDir] = _T('\0');

	int iNumOfFiles = 0;	// Actual added to menu

	if (pMRUFiles != NULL)
	{
		for (int i = 0; i < pMRUFiles->GetSize (); i++)
		{
			CString strName;

			if (pMRUFiles->GetDisplayName (strName, i, szCurDir, nCurDir))
			{
				//---------------------
				// Add shortcut number:
				//---------------------
				CString strItem;

				if (iNumOfFiles == 9)
				{
					strItem.Format (_T("1&0 %s"), strName);
				}
				else if (iNumOfFiles < 9)
				{
					strItem.Format (_T("&%d %s"), iNumOfFiles + 1, strName);
				}
				else
				{
					strItem = strName;
				}

				CBCGPRibbonButton* pFile = new CBCGPRibbonButton;
				pFile->SetText (strItem);
				pFile->SetID (ID_FILE_MRU_FILE1 + i);
				pFile->SetToolTipText ((*pMRUFiles)[i]);

				AddButton (pFile);

				iNumOfFiles++;
			}
		}
	}
}
//********************************************************************************
void CBCGPRibbonRecentFilesList::OnDraw (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pButton = m_arButtons [i];
		ASSERT_VALID (pButton);

		pButton->OnDraw (pDC);
	}
}
//********************************************************************************
BOOL CBCGPRibbonRecentFilesList::OnMenuKey (UINT nUpperChar)
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arButtons.GetSize (); i++)
	{
		CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (
			CBCGPRibbonButton, m_arButtons [i]);

		if (pButton == NULL)
		{
			continue;
		}

		ASSERT_VALID (pButton);

		CString strLabel = pButton->GetText ();

		int iAmpOffset = strLabel.Find (_T('&'));
		if (iAmpOffset >= 0 && iAmpOffset < strLabel.GetLength () - 1)
		{
			TCHAR szChar [2] = { strLabel.GetAt (iAmpOffset + 1), '\0' };
			CharUpper (szChar);

			if ((UINT) (szChar [0]) == nUpperChar && !pButton->IsDisabled ())
			{
				pButton->OnClick (pButton->GetRect ().TopLeft ());
				return TRUE;
			}
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonMainPanel

IMPLEMENT_DYNCREATE (CBCGPRibbonMainPanel, CBCGPRibbonPanel)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonMainPanel::CBCGPRibbonMainPanel()
{
	m_nBottomElementsNum = 0;
	m_nTopMargin = 0;
	m_pElemOnRight = NULL;
	m_nRightPaneWidth = 0;
	m_bMenuMode = TRUE;
	m_pMainButton = NULL;

	m_rectMenuElements.SetRectEmpty ();
}
//********************************************************************************
CBCGPRibbonMainPanel::~CBCGPRibbonMainPanel()
{

}
//********************************************************************************
void CBCGPRibbonMainPanel::RecalcWidths (CDC* pDC, int /*nHeight*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	m_arWidths.RemoveAll ();
	m_nCurrWidthIndex = 0;
	m_bIsCalcWidth = TRUE;

	Repos (pDC, CRect (0, 0, 32767, 32767));
	m_arWidths.Add (m_nFullWidth);

	m_bIsCalcWidth = FALSE;
}
//********************************************************************************
void CBCGPRibbonMainPanel::Repos (CDC* pDC, const CRect& rect)
{
	ASSERT_VALID (pDC);

	CSize size = rect.Size ();
	size.cx -= m_nXMargin;
	size.cy -= m_nYMargin;

	int y = m_nTopMargin;
	int i = 0;

	int nMenuElements = GetMenuElements ();

	m_rectMenuElements = rect;
	m_rectMenuElements.DeflateRect (m_nXMargin, m_nYMargin);
	m_rectMenuElements.top += m_nTopMargin;

	int nImageWidth = 0;

	if (m_pParent != NULL)
	{
		ASSERT_VALID (m_pParent);
		nImageWidth = m_pParent->GetImageSize (TRUE).cx;
	}

	//----------------------------------------
	// Repos menu elements (on the left side):
	//----------------------------------------
	int nColumnWidth = 0;

	for (i = 0; i < nMenuElements; i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (pDC);
		pElem->SetTextAlwaysOnRight ();

		CSize sizeElem = pElem->GetSize (pDC);

		if (sizeElem == CSize (0, 0))
		{
			pElem->SetRect (CRect (0, 0, 0, 0));
			continue;
		}

		CRect rectElem = CRect
			(CPoint (rect.left + m_nXMargin, rect.top + y + m_nYMargin), 
			sizeElem);

		pElem->SetRect (rectElem);

		nColumnWidth = max (nColumnWidth, sizeElem.cx);
		y += sizeElem.cy;
	}

	nColumnWidth += 2 * m_nXMargin;

	m_rectMenuElements.right = m_rectMenuElements.left + nColumnWidth;
	m_rectMenuElements.bottom = y + m_nYMargin;

	m_rectMenuElements.InflateRect (1, 1);

	m_nFullWidth = nColumnWidth + 2 * m_nXMargin;

	//----------------------------------------------
	// All menu elements should have the same width:
	//----------------------------------------------
	for (i = 0; i < nMenuElements; i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		CRect rectElem = pElem->GetRect ();

		if (!rectElem.IsRectEmpty ())
		{
			rectElem.right = rectElem.left + nColumnWidth;
			
			if (nImageWidth > 0 && 
				pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
			{
				rectElem.left += nImageWidth + LABEL_MARGIN;
			}

			pElem->SetRect (rectElem);
		}
	}

	//----------------------
	// Put element on right:
	//----------------------
	if (m_pElemOnRight != NULL)
	{
		CBCGPRibbonRecentFilesList* pRecentList = 
			DYNAMIC_DOWNCAST (CBCGPRibbonRecentFilesList, m_pElemOnRight);

		if (pRecentList != NULL)
		{
			ASSERT_VALID (pRecentList);

			if (pRecentList->GetCount () == 0)
			{
				pRecentList->FillList ();
			}
		}

		m_pElemOnRight->SetInitialMode ();
		m_pElemOnRight->OnCalcTextSize (pDC);
		
		CSize sizeRecentList = m_pElemOnRight->GetSize (pDC);

		int nDefaultWidth =
			globalData.GetRibbonImageScale () == 1. ? m_nRightPaneWidth : (int) (globalData.GetRibbonImageScale () *  m_nRightPaneWidth);

		sizeRecentList.cx = max (sizeRecentList.cx, nDefaultWidth);

		if (m_rectMenuElements.Height () < sizeRecentList.cy)
		{
			m_rectMenuElements.bottom = m_rectMenuElements.top + sizeRecentList.cy;
		}

		CRect rectRecentList = CRect
			(m_rectMenuElements.right, m_rectMenuElements.top, 
			m_rectMenuElements.right + sizeRecentList.cx, m_rectMenuElements.bottom);

		if (pRecentList == NULL)
		{
			rectRecentList.DeflateRect (0, 1);
		}

		m_pElemOnRight->SetRect (rectRecentList);

		m_nFullWidth += sizeRecentList.cx;
	}

	//---------------------------------
	// Put "bottom" elements on bottom:
	//---------------------------------
	if (m_nBottomElementsNum > 0)
	{
		int x = rect.left + m_nFullWidth - m_nXMargin;
		int nRowHeight = 0;

		y = m_rectMenuElements.bottom + m_nYMargin;

		for (int nCount = 0; nCount < m_nBottomElementsNum; nCount++)
		{
			int nIndex = (int) m_arElements.GetSize () - nCount - 1;

			CBCGPBaseRibbonElement* pElem = m_arElements [nIndex];
			ASSERT_VALID (pElem);

				pElem->OnCalcTextSize (pDC);

			CSize sizeElem = pElem->GetSize (pDC);

			if (sizeElem == CSize (0, 0))
			{
				pElem->SetRect (CRect (0, 0, 0, 0));
				continue;
			}

			sizeElem.cx += LABEL_MARGIN - 1;

			if (x - sizeElem.cx < rect.left + m_nXMargin)
			{
				x = rect.left + m_nFullWidth - m_nXMargin;
				y += nRowHeight; 
				nRowHeight = 0;
			}

			CRect rectElem = CRect (CPoint (x - sizeElem.cx, y), sizeElem);
			pElem->SetRect (rectElem);

			nRowHeight = max (nRowHeight, sizeElem.cy);
			x = rectElem.left - LABEL_MARGIN;
		}

		y += nRowHeight;
	}

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnAfterChangeRect (pDC);
	}

	m_rect = rect;
	m_rect.bottom = m_rect.top + y + m_nYMargin;
	m_rect.right = m_rect.left + m_nFullWidth + m_nXMargin;
}
//**************************************************************************************
void CBCGPRibbonMainPanel::AddRecentFilesList (LPCTSTR lpszLabel, int nWidth)
{
	ASSERT_VALID (this);
	ASSERT (lpszLabel != NULL);

	AddToRight (new CBCGPRibbonRecentFilesList (lpszLabel), nWidth);
}
//**************************************************************************************
void CBCGPRibbonMainPanel::AddToRight (CBCGPBaseRibbonElement* pElem, int nWidth)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	if (m_pElemOnRight != NULL)
	{
		// Already exist, delete previous
		m_arElements.RemoveAt (GetMenuElements ());

		ASSERT_VALID (m_pElemOnRight);
		delete m_pElemOnRight;

		m_pElemOnRight = NULL;
	}

	pElem->SetParentCategory (m_pParent);

	m_arElements.InsertAt (GetMenuElements (), pElem);
	
	m_pElemOnRight = pElem;
	m_nRightPaneWidth = nWidth;
}
//**************************************************************************************
void CBCGPRibbonMainPanel::AddToBottom (CBCGPRibbonMainPanelButton* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	m_nBottomElementsNum++;

	pElem->SetParentCategory (m_pParent);
	m_arElements.Add (pElem);
}
//********************************************************************************
void CBCGPRibbonMainPanel::Add (CBCGPBaseRibbonElement* pElem)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElem);

	pElem->SetParentCategory (m_pParent);
	m_arElements.InsertAt (GetMenuElements (), pElem);
}
//********************************************************************************
int CBCGPRibbonMainPanel::GetMenuElements () const
{
	ASSERT_VALID (this);

	int nMenuElements = (int) m_arElements.GetSize () - m_nBottomElementsNum;
	if (m_pElemOnRight != NULL)
	{
		nMenuElements--;
	}

	ASSERT (nMenuElements >= 0);
	return nMenuElements;
}
//********************************************************************************
void CBCGPRibbonMainPanel::CopyFrom (CBCGPRibbonPanel& s)
{
	ASSERT_VALID (this);

	CBCGPRibbonPanel::CopyFrom (s);

	CBCGPRibbonMainPanel& src = (CBCGPRibbonMainPanel&) s;

	m_nBottomElementsNum = src.m_nBottomElementsNum;
	m_nTopMargin = src.m_nTopMargin;
	m_pMainButton = src.m_pMainButton;

	m_pElemOnRight = NULL;
	m_nRightPaneWidth = src.m_nRightPaneWidth;

	if (src.m_pElemOnRight != NULL)
	{
		ASSERT_VALID (src.m_pElemOnRight);

		for (int i = 0; i < src.m_arElements.GetSize (); i++)
		{
			if (src.m_arElements [i] == src.m_pElemOnRight)
			{
				m_pElemOnRight = m_arElements [i];
				break;
			}
		}

		ASSERT_VALID (m_pElemOnRight);

		CBCGPRibbonRecentFilesList* pRecentList = 
			DYNAMIC_DOWNCAST (CBCGPRibbonRecentFilesList, m_pElemOnRight);

		if (pRecentList != NULL)
		{
			ASSERT_VALID (pRecentList);
			pRecentList->RemoveAll ();
		}
	}
}
//********************************************************************************
BOOL CBCGPRibbonMainPanel::GetPreferedMenuLocation (CRect& rect)
{
	ASSERT_VALID (this);

	if (m_pElemOnRight == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (m_pElemOnRight);

	rect = m_pElemOnRight->GetRect ();
	rect.DeflateRect (1, 1);

	const int nShadowSize = CBCGPMenuBar::IsMenuShadows () &&
					!CBCGPToolBar::IsCustomizeMode () &&
					globalData.m_nBitsPerPixel > 8 ? // Don't draw shadows in 256 colors or less
						CBCGPVisualManager::GetInstance ()->GetMenuShadowDepth () : 0;


	rect.right -= nShadowSize + 3;
	rect.bottom -= nShadowSize + 3;

	return TRUE;
}
//********************************************************************************
void CBCGPRibbonMainPanel::DoPaint (CDC* pDC)
{
	ASSERT_VALID (pDC);

	if (m_rect.IsRectEmpty ())
	{
		return;
	}

	CRect rectClip;
	pDC->GetClipBox (rectClip);

	CRect rectInter;

	if (!rectInter.IntersectRect (m_rect, rectClip))
	{
		return;
	}

	COLORREF clrTextOld = pDC->GetTextColor ();

	//-----------------------
	// Fill panel background:
	//-----------------------
	COLORREF clrText = CBCGPVisualManager::GetInstance ()->OnDrawRibbonPanel (
		pDC, this, m_rect, CRect (0, 0, 0, 0));

	DrawMainButton (pDC, GetParentWnd ());

	CRect rectFrame = m_rectMenuElements;

	CRect rectRecentFiles;
	rectRecentFiles.SetRectEmpty ();

	if (m_pElemOnRight != NULL)
	{
		ASSERT_VALID (m_pElemOnRight);

		rectRecentFiles = m_pElemOnRight->GetRect ();

		CBCGPVisualManager::GetInstance ()->OnDrawRibbonRecentFilesFrame (
			pDC, this, rectRecentFiles);
	}

	if (!rectRecentFiles.IsRectEmpty ())
	{
		rectFrame.right = rectRecentFiles.right;
	}

	CBCGPVisualManager::GetInstance ()->OnFillRibbonMenuFrame (pDC, this, m_rectMenuElements);
	CBCGPVisualManager::GetInstance ()->OnDrawRibbonMainPanelFrame (pDC, this, rectFrame);

	pDC->SetTextColor (clrText);

	//---------------------
	// Draw panel elements:
	//---------------------
	for (int i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (rectInter.IntersectRect (pElem->GetRect (), rectClip))
		{
			pDC->SetTextColor (clrText);
			pElem->OnDraw (pDC);
		}
	}

	pDC->SetTextColor (clrTextOld);
}
//**********************************************************************************
CRect CBCGPRibbonMainPanel::GetCommandsFrame () const
{
	ASSERT_VALID (this);

	CRect rectFrame = m_rectMenuElements;

	if (m_pElemOnRight != NULL)
	{
		ASSERT_VALID (m_pElemOnRight);

		CRect rectRecentFiles = m_pElemOnRight->GetRect ();
		if (!rectRecentFiles.IsRectEmpty ())
		{
			rectFrame.right = rectRecentFiles.right;
		}
	}

	return rectFrame;
}
//********************************************************************************
void CBCGPRibbonMainPanel::OnDrawMenuBorder (CDC* pDC, CBCGPRibbonPanelMenuBar* pMenuBar)
{
	ASSERT_VALID (pMenuBar);
	DrawMainButton (pDC, pMenuBar->GetParent ());
}
//********************************************************************************
void CBCGPRibbonMainPanel::DrawMainButton (CDC* pDC, CWnd* pWnd)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (pWnd);

	if (m_pMainButton == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pMainButton);
	ASSERT_VALID (m_pMainButton->GetParentRibbonBar ());

	CRect rectMainButtonSaved = m_pMainButton->GetRect ();
	CRect rectMainButton = rectMainButtonSaved;

	m_pMainButton->GetParentRibbonBar ()->ClientToScreen (&rectMainButton);
	pWnd->ScreenToClient (&rectMainButton);

	if (rectMainButton.top > m_rectMenuElements.bottom)
	{
		return;
	}

	m_pMainButton->SetRect (rectMainButton);

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonMainButton
		(pDC, m_pMainButton);

	m_pMainButton->OnDraw (pDC);

	m_pMainButton->SetRect (rectMainButtonSaved);
}
//*****************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonMainPanel::MouseButtonDown (CPoint point)
{
	ASSERT_VALID (this);

	CBCGPBaseRibbonElement* pElement = CBCGPRibbonPanel::MouseButtonDown (point);

	if (m_pMainButton != NULL)
	{
		ASSERT_VALID (m_pMainButton);
		ASSERT_VALID (m_pMainButton->GetParentRibbonBar ());
		ASSERT_VALID (GetParentWnd ());
		
		CRect rectMainButton = m_pMainButton->GetRect ();

		m_pMainButton->GetParentRibbonBar ()->ClientToScreen (&rectMainButton);
		GetParentWnd ()->ScreenToClient (&rectMainButton);

		if (rectMainButton.PtInRect (point))
		{
			m_pMainButton->ClosePopupMenu ();
			return NULL;
		}
	}

	return pElement;
}

//////////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonMainPanelButton

IMPLEMENT_DYNCREATE(CBCGPRibbonMainPanelButton, CBCGPRibbonButton)

CBCGPRibbonMainPanelButton::CBCGPRibbonMainPanelButton()
{
}
//********************************************************************************
CBCGPRibbonMainPanelButton::CBCGPRibbonMainPanelButton (UINT nID, LPCTSTR lpszText, 
									  int nSmallImageIndex) :
	CBCGPRibbonButton (nID, lpszText, nSmallImageIndex)
{
}
//********************************************************************************
CBCGPRibbonMainPanelButton::CBCGPRibbonMainPanelButton (
		UINT	nID, 
		LPCTSTR lpszText, 
		HICON	hIcon) :
	CBCGPRibbonButton (nID, lpszText, hIcon)
{
}
//********************************************************************************
CBCGPRibbonMainPanelButton::~CBCGPRibbonMainPanelButton()
{
}
//********************************************************************************
COLORREF CBCGPRibbonMainPanelButton::OnFillBackground (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (DYNAMIC_DOWNCAST (CBCGPRibbonMainPanel, GetParentPanel ()) == NULL)
	{
		return CBCGPRibbonButton::OnFillBackground (pDC);
	}

	return CBCGPVisualManager::GetInstance ()->OnFillRibbonMainPanelButton (pDC, this);
}
//*****************************************************************************
void CBCGPRibbonMainPanelButton::OnDrawBorder (CDC* pDC)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	if (DYNAMIC_DOWNCAST (CBCGPRibbonMainPanel, GetParentPanel ()) == NULL)
	{
		CBCGPRibbonButton::OnDrawBorder (pDC);
		return;
	}

	CBCGPVisualManager::GetInstance ()->OnDrawRibbonMainPanelButtonBorder (pDC, this);
}

#endif // BCGP_EXCLUDE_RIBBON
