
#include "stdafx.h"
#include "EntityEditBar.h"

IMPLEMENT_DYNCREATE(CModelEditDlg, CBCGPDockingControlBar)
CModelEditDlg::CModelEditDlg()
{
}

CModelEditDlg::~CModelEditDlg()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CModelEditDlg, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CModelEditDlg message handlers


int CModelEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndTab.Create(CBCGPTabWnd::STYLE_3D_ONENOTE , CRect(0, 0, 500, 500), this, 1, 
		CBCGPTabWnd::LOCATION_BOTTOM))
	{
		return -1;
	}
	m_wndTab.SetActiveTabBoldFont(TRUE);//把活动的Tab标签设置为粗体

	if(!m_wndAnim.CreateEx(0, NULL, _T("Animation"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), &m_wndTab, ID_WND_ANIM))
	{
		return -1;
	}

	if(!m_wndBone.CreateEx(0, NULL, _T("Bones"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), &m_wndTab, ID_WND_BONE))
	{
		return -1;
	}

	//if(!m_wndAnchor.CreateEx(0, NULL, _T("Anchor"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), &m_wndTab, ID_WND_ANCHOR))
	//{
	//	return -1;
	//}

	if(!m_wndMotion.CreateEx(0, NULL, _T("Motion"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), &m_wndTab, ID_WND_MOTION))
	{
		return -1;
	}	

	if(!m_wndItem.CreateEx(0, NULL, _T("Motion"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), &m_wndTab, ID_WND_ITEMPREVIEW))
	{
		return -1;
	}	
	/*
	if(!m_wndSkill.CreateEx(0, NULL, _T("Motion"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), &m_wndTab, ID_WND_SKILLPREVEW))
	{
		return -1;
	}
	if(!m_wndSkillTemplate.CreateEx(0, NULL, _T("Motion"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 100, 100), &m_wndTab, ID_WND_SKILLTEMPLATE))
	{
		return -1;
	}*/


	m_wndTab.AddTab(&m_wndMotion, _T("特效动画"), -1, FALSE);	
	m_wndTab.AddTab(&m_wndAnim, _T("动作"), -1, FALSE);
	m_wndTab.AddTab(&m_wndBone, _T("骨骼与绑定点"), -1, FALSE);
	//m_wndTab.AddTab(&m_wndAnchor, _T("绑定点"), -1, FALSE);

	m_wndTab.AddTab(&m_wndItem, _T("装备设定"), -1, FALSE);	
	//m_wndTab.AddTab(&m_wndSkill, _T("战斗模拟"), -1, FALSE);
	//m_wndTab.AddTab(&m_wndSkillTemplate, _T("技能模版"), -1, FALSE);
	m_wndTab.SetActiveTab(0);
	//m_wndTab.EnableTabDetach(1, TRUE);


	return 0;
}

void CModelEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	static int tag = 0;

	CRect frameRect;
	CRect tabRect;
	CRect clientRect;
	GetClientRect(frameRect);
	m_wndTab.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top,
		SWP_NOZORDER | SWP_NOMOVE);


	m_wndTab.GetTabsRect(tabRect);
	m_wndTab.GetClientRect(clientRect);

	clientRect.bottom -= tabRect.Height() + 4;
	m_wndAnim.SetWindowPos(NULL, 0, 0,
		clientRect.Width(), clientRect.Height(),
		SWP_NOZORDER | SWP_NOMOVE);

	m_wndBone.SetWindowPos(NULL, 0, 0,
		clientRect.Width(), clientRect.Height(),
		SWP_NOZORDER | SWP_NOMOVE);

	//m_wndAnchor.SetWindowPos(NULL, 0, 0,
	//	clientRect.Width(), clientRect.Height(),
	//	SWP_NOZORDER | SWP_NOMOVE);

	m_wndMotion.SetWindowPos(NULL, 0, 0,
		clientRect.Width(), clientRect.Height(),
		SWP_NOZORDER | SWP_NOMOVE);

	/*
	m_wndSkill.SetWindowPos(NULL, 0, 0,
		clientRect.Width(), clientRect.Height(),
		SWP_NOZORDER | SWP_NOMOVE);

	m_wndSkillTemplate.SetWindowPos(NULL, 0, 0,
		clientRect.Width(), clientRect.Height(),
		SWP_NOZORDER | SWP_NOMOVE);
		*/
}

BOOL CModelEditDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndAnim.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ||
		m_wndBone.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ||
		//m_wndAnchor.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ||
		m_wndMotion.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ||
		m_wndItem.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)
		//m_wndSkill.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ||
		//m_wndSkillTemplate.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)
		)
		return TRUE;

	return CBCGPDockingControlBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CModelEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CBCGPDockingControlBar::OnPaint() for painting messages
	CBrush b(RGB(255, 0, 0));
	dc.FillRect(CRect(0, 0, 500, 500), &b);
}
