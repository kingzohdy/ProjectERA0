// EditPanel.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "EditPanel.h"


// CEditPanel

IMPLEMENT_DYNAMIC(CEditPanel, CBCGPDockingControlBar)

CEditPanel::CEditPanel()
{

}

CEditPanel::~CEditPanel()
{
}


BEGIN_MESSAGE_MAP(CEditPanel, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CEditPanel message handlers



int CEditPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if ( !m_wndTabs.Create( CBCGPTabWnd::STYLE_3D, rectDummy, this, 1 ) )
	{
		TRACE0("Failed to create EditPanel tab window\n");
		return -1;      // fail to create
	}
	m_wndTabs.SetLocation(CBCGPBaseTabWnd::LOCATION_TOP);

	m_HeightEditDlg.Create(m_HeightEditDlg.IDD, &m_wndTabs);
	m_wndTabs.AddTab(&m_HeightEditDlg, "地形");

	m_TerrainLayerDlg.Create(m_TerrainLayerDlg.IDD, &m_wndTabs);
	m_wndTabs.AddTab(&m_TerrainLayerDlg, "地表");

	m_WaterEditDlg.Create(m_WaterEditDlg.IDD, &m_wndTabs);	
	m_wndTabs.AddTab(&m_WaterEditDlg, "水面");

	m_ModelEditDlg.Create(m_ModelEditDlg.IDD, &m_wndTabs);
	m_wndTabs.AddTab(&m_ModelEditDlg, "模型");

	m_LightEditDlg.Create(m_LightEditDlg.IDD, &m_wndTabs);
	m_wndTabs.AddTab(&m_LightEditDlg, "灯光");

	m_AreaEnvRollup.Create(WS_VISIBLE|WS_CHILD, rectDummy, &m_wndTabs, 65535);
	m_AreaEditDlg.Create(m_AreaEditDlg.IDD, &m_AreaEnvRollup);
	m_AreaEnvRollup.InsertPage("区域编辑", &m_AreaEditDlg, FALSE);
	m_EnvEditDlg.Create(m_EnvEditDlg.IDD, &m_AreaEnvRollup);
	m_AreaEnvRollup.InsertPage("区域环境编辑", &m_EnvEditDlg, FALSE);
	m_AreaEditDlg.m_pEnvEditDlg = &m_EnvEditDlg;
	m_ProcEditDlg.Create(m_ProcEditDlg.IDD, &m_AreaEnvRollup);
	m_AreaEnvRollup.InsertPage("区域逻辑编辑", &m_ProcEditDlg, FALSE);
	m_AreaEditDlg.m_pProcEditDlg = &m_ProcEditDlg;

	m_wndTabs.AddTab(&m_AreaEnvRollup, "区域");

	m_NpcEditDlg.Create(m_NpcEditDlg.IDD, &m_wndTabs);
	m_wndTabs.AddTab(&m_NpcEditDlg, "NPC");

	m_wndTabs.RecalcLayout();
	return 0;
}

void CEditPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_wndTabs.SetWindowPos (this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
