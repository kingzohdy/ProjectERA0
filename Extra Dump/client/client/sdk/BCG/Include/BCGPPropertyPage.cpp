// BCGPPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPPopupMenu.h"
#include "BCGPPropertyPage.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropertyPage

IMPLEMENT_DYNCREATE(CBCGPPropertyPage, CPropertyPage)

#pragma warning (disable : 4355)

CBCGPPropertyPage::CBCGPPropertyPage() :
	m_Impl (*this)
{
	CommonInit ();
}

CBCGPPropertyPage::CBCGPPropertyPage(UINT nIDTemplate, UINT nIDCaption) :
	CPropertyPage (nIDTemplate, nIDCaption),
	m_Impl (*this)
{
	CommonInit ();
}

CBCGPPropertyPage::CBCGPPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption) :
	CPropertyPage (lpszTemplateName, nIDCaption),
	m_Impl (*this)
{
	CommonInit ();
}

#pragma warning (default : 4355)

void CBCGPPropertyPage::CommonInit ()
{
	m_pCategory = NULL;
	m_nIcon = -1;
	m_nSelIconNum = -1;
	m_hTreeNode = NULL;
}

CBCGPPropertyPage::~CBCGPPropertyPage()
{
}

BEGIN_MESSAGE_MAP(CBCGPPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBCGPPropertyPage)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropertyPage message handlers

void CBCGPPropertyPage::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	m_Impl.OnActivate (nState, pWndOther);
	CPropertyPage::OnActivate (nState, pWndOther, bMinimized);
}
//****************************************************************************************
BOOL CBCGPPropertyPage::OnNcActivate(BOOL bActive) 
{
	m_Impl.OnNcActivate (bActive);

	//-----------------------------------------------------------
	// Do not call the base class because it will call Default()
	// and we may have changed bActive.
	//-----------------------------------------------------------
	return (BOOL) DefWindowProc (WM_NCACTIVATE, bActive, 0L);
}
//**************************************************************************************
void CBCGPPropertyPage::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
	m_Impl.SetActiveMenu (pMenu);
}
//***************************************************************************************
BOOL CBCGPPropertyPage::PreTranslateMessage(MSG* pMsg) 
{
	if (m_Impl.PreTranslateMessage (pMsg))
	{
		return TRUE;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}
//****************************************************************************************
BOOL CBCGPPropertyPage::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (m_Impl.OnCommand (wParam, lParam))
	{
		return TRUE;
	}

	return CPropertyPage::OnCommand(wParam, lParam);
}
//****************************************************************************************
BOOL CBCGPPropertyPage::OnSetActive() 
{
	CBCGPPropertySheet* pParent = DYNAMIC_DOWNCAST(CBCGPPropertySheet, GetParent ());
	if (pParent != NULL)
	{
		pParent->OnActivatePage (this);
	}
	
	return CPropertyPage::OnSetActive();
}
