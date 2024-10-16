// workspace2.cpp : implementation of the CWorkSpaceBar2 class
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "WorkSpaceBar2.h"

#include "ui_frame.h"
#include "xml_uimgr.h"
#include "OgreResourceManager.h"
#include "NotifyProperty.h"
#include "ui_gameui.h"
#include "ui_framemgr.h"
#include "TypeBind.h"
#include "ui_editmgr.h"
#include "CmdMgr.h"
#include "OgreInputHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UIEditManager * g_pUIEditMgr;
extern CCommandMgr * g_pCmdMgr;

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2

BEGIN_MESSAGE_MAP(CWorkSpaceBar2, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkSpaceBar2)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, &CWorkSpaceBar2::GetMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 construction/destruction

CWorkSpaceBar2::CWorkSpaceBar2()
{
	// TODO: add one-time construction code here
	m_bWorkspaceON = false;
}

CWorkSpaceBar2::~CWorkSpaceBar2()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 message handlers

int CWorkSpaceBar2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();
	// Create tree windows.
	// TODO: create your own tab windows here:
	//const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
	//							TVS_LINESATROOT | TVS_HASBUTTONS;
	//
	//if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	//{
	//	TRACE0("Failed to create workspace view\n");
	//	return -1;      // fail to create
	//}
	const DWORD dwViewStyle = WS_VISIBLE;
	if( !m_wndPropList.Create( dwViewStyle, rectDummy, this, ID_PROPLIST_LAYOUT))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;
	}

	m_wndPropList.EnableHeaderCtrl(FALSE);
	return 0;
}

void CWorkSpaceBar2::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndPropList.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndPropList.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

void CWorkSpaceBar2::DisplayProp(UIObject *pUIObj)
{
	m_wndPropList.RemoveAll();

	SetCurrentObj( pUIObj );

	CBCGPPropList * pproplist = &m_wndPropList;
	
	if ( !g_pUIEditMgr->SetPropList( pUIObj, pproplist ) )
	{
		MessageBox("\tcan not set property list\t");
	}
	
}

void CWorkSpaceBar2::SetCurrentObj(UIObject *pUIObj)
{
	m_CurObj = pUIObj;
}

bool CWorkSpaceBar2::HasCurObj()
{
	if ( NULL == m_CurObj)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

UIObject * CWorkSpaceBar2::GetCurrentObj()
{
	return m_CurObj;
}

LRESULT CWorkSpaceBar2::GetMsg(WPARAM wParam, LPARAM lParam)
{
	CBCGPPropList * pproplist = &m_wndPropList;
	g_pUIEditMgr->UpdateUIObj( m_CurObj ,pproplist );
	
	return 1;
}

bool CWorkSpaceBar2::SavePropToXML(const char * path)
{
	return g_pUIEditMgr->SaveUIToXml( path );
}

void CWorkSpaceBar2::WorkSpaceON(bool bOn)
{
	m_bWorkspaceON = bOn;
}

bool CWorkSpaceBar2::WorkSpaceIsON()
{
	return m_bWorkspaceON;
}
//------------------------------------------
//CMyPropList
//------------------------------------------
CMyProplist::CMyProplist()
{

}

CMyProplist::~CMyProplist()
{

}
