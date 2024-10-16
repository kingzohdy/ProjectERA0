// BCGPOutlookControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPOutlookControlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBCGPOutlookControlBar,CBCGPBaseTabbedBar)
/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookControlBar

CBCGPOutlookControlBar::CBCGPOutlookControlBar (BOOL bAutoDestroy) : CBCGPBaseTabbedBar (bAutoDestroy)
{
}

CBCGPOutlookControlBar::~CBCGPOutlookControlBar()
{
}


BEGIN_MESSAGE_MAP(CBCGPOutlookControlBar, CBCGPBaseTabbedBar)
	//{{AFX_MSG_MAP(CBCGPOutlookControlBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookControlBar message handlers

int CBCGPOutlookControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPBaseTabbedBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddControlBar (this);

	return 0;
}
