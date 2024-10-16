// InfoPanel.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "InfoPanel.h"


// CInfoPanel

IMPLEMENT_DYNAMIC(CInfoPanel, CBCGPDockingControlBar)

CInfoPanel::CInfoPanel()
{

}

CInfoPanel::~CInfoPanel()
{
}


BEGIN_MESSAGE_MAP(CInfoPanel, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CInfoPanel message handlers



int CInfoPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if(!m_wndTabs.Create(CBCGPTabWnd::STYLE_3D, rectDummy, this, 1))
	{
		TRACE0("Failed to create EditPanel tab window\n");
		return -1;      // fail to create
	}
	m_wndTabs.SetLocation( CBCGPBaseTabWnd::LOCATION_TOP );

	//m_MapViewInfoDlg.Create(m_MapViewInfoDlg.IDD, &m_wndTabs);
	//m_wndTabs.AddTab(&m_MapViewInfoDlg, "地图信息");

	m_ResBrowerDlg.Create(m_ResBrowerDlg.IDD, &m_wndTabs);
	m_wndTabs.AddTab(&m_ResBrowerDlg, "资源浏览选择");

	m_wndTabs.RecalcLayout();
	return 0;
}

void CInfoPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_wndTabs.SetWindowPos (this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
