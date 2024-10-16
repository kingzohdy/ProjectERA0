
#include "stdafx.h"
#include "EffectUnitListBar.h"
#include "ObjectEditor.h"
#include "ControlsHelper.h"
#include "EffectManager.h"
#include "ModifyEffectNameDlg.h"
#include "EffectUnit.h"
#include "BindInfo.h"

IMPLEMENT_DYNAMIC(CEditBar, CBCGPDockingControlBar)
CEditBar::CEditBar()
{
}

CEditBar::~CEditBar()
{
}


BEGIN_MESSAGE_MAP(CEditBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_BTN_EDT_NEWEFFECT, OnBtnNewEffect)
	ON_COMMAND(ID_BTN_EDT_LOADEFFECT, OnBtnLoadEffect)

	ON_COMMAND(ID_MNU_INSERT_NORPAR, OnInsertNorpar)
	ON_COMMAND(ID_MNU_INSERT_LIGHT, OnInsertLight)
	ON_COMMAND(ID_MNU_INSERT_BILLBOARD, OnInsertBillboard)
	ON_COMMAND(ID_MNU_INSERT_DECAL, OnInsertDecal)
	ON_NOTIFY(NM_RCLICK, ID_EFFECTLIST, OnNMRclickEffectlist)

	ON_NOTIFY(LVN_ITEMACTIVATE , ID_EFFECTLIST, OnNmItemActive)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_COMMAND(ID_BTN_EDT_APPLYALL, OnBtnApplyAll)

	ON_COMMAND(ID_MNU_EDITEFFECT, OnMnuEditeffect)
	ON_COMMAND(ID_MNU_INSERT_RIBBON, OnMnuInsertRibbon)
	ON_COMMAND(ID_MNU_INSERT_BEAM, OnMnuInsertBeam)

	ON_COMMAND(ID_MNU_INSERT_SOUND, OnMnuInsertSound)
	ON_COMMAND(ID_MNU_SET_INDEPENDENT, OnMnuSetIndependent)
	ON_UPDATE_COMMAND_UI(ID_MNU_SET_INDEPENDENT, OnUpdateMnuSetIndependent)
	ON_COMMAND(ID_MNU_SET_INTEGRATE, OnMnuSetIntegrate)
	ON_UPDATE_COMMAND_UI(ID_MNU_SET_INTEGRATE, OnUpdateMnuSetIntegrate)
	ON_COMMAND(ID_MNU_INSERT_GEOM, OnMnuInsertGeom)

	ON_COMMAND(ID_BTN_EDT_REFRESHLIST, OnRefreshList)
	ON_COMMAND(ID_MNU_INSERT_VIRTUAL, OnMnuInsertVirtual)
	ON_COMMAND(ID_BTN_MO_NEW_POST, &CEditBar::OnBtnMoNewPost)
END_MESSAGE_MAP()



// CEditBar message handlers


int CEditBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Tool Bar
	if(!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_EFFECTLIST))
	{
		return -1;
	}
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CBCGPToolbarMenuButton mnuNewEffect(ID_BTN_EDT_NEWEFFECT, 
		GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_NEWEFFECT)), 0), 0);
	mnuNewEffect.SetMenuOnly(TRUE);
	m_wndToolBar.ReplaceButton(ID_BTN_EDT_NEWEFFECT, mnuNewEffect);

	//Effect List
	m_wndEffectList.CreateEx(LVS_EX_FULLROWSELECT,  WS_VISIBLE | LVS_REPORT, 
		CRect(0, 0, 100, 100), this, ID_EFFECTLIST);
	m_wndEffectList.InsertColumn(0, _T("特效名"), LVCFMT_LEFT, 102, -1);
	m_wndEffectList.InsertColumn(1, _T("类型"), LVCFMT_LEFT, 50, -1);
	m_wndEffectList.InsertColumn(2, _T("可视"), LVCFMT_LEFT, 50, -1);
	m_wndEffectList.InsertColumn(3, _T("绑定"), LVCFMT_LEFT, 50, -1);
	m_wndEffectList.InsertColumn(4, _T("存储"), LVCFMT_LEFT, 50, -1);
	m_wndEffectList.ShowWindow(SW_SHOW);

	this->SetWindowText(_T("特效编辑"));

	m_dropTarget.Register(&m_wndEffectList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());
	return 0;
}

void CEditBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);
	m_wndEffectList.SetWindowPos(NULL, 0, 24,
		rect.right - rect.left, rect.bottom - rect.top - 24,
		SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		rect.right - rect.left, 24, SWP_NOZORDER);
}


BOOL CEditBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	return CBCGPDockingControlBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CEditBar::OnBtnNewEffect()
{

}

void CEditBar::OnBtnLoadEffect()
{
	CString path = GetOpenFilePath(TRUE, "effect", "", "emo", "Motion Files(*.emo)\0*.emo\0\0", TRUE);
	if(path.GetLength() > 0)
	{
		NEWEFFECT_INFO ni; 
		ni.anchor = 0;
		ni.offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		ni.scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);
		ni.path = path;
		ni.realpath = path;
		ni.bDynamic = false;
		ni.bShowPane = true;
		ni.bIsPath = true;

		SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0, (LPARAM)&ni);
	}

	/*
	//PostMainFrameMsg(WM_COMMAND, ID_LOAD_EFFECT, 0);
	CResLoadSaveDlg dlg;
	//dlg.SetTitle("载入");
	//dlg.SetDesc("输入载入的名称");
	//dlg.m_strText = pEU->GetName();

	if(IDOK == dlg.DoModal())
	{
	NEWEFFECT_INFO ni; 
	ni.anchor = 0;
	ni.offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	ni.scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);
	ni.path = dlg.m_strSelectedChunk.GetBuffer();
	ni.realpath = dlg.m_strSelectedChunk.GetBuffer();
	ni.bDynamic = false;
	ni.bShowPane = true;
	ni.bIsPath = true;

	SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0, (LPARAM)&ni);

	}*/
}

void CEditBar::OnInsertNorpar()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_PEMITTER, 0);
}

void CEditBar::OnInsertLight()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_LIGHT, 0);
}

void CEditBar::OnInsertBillboard()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_BILLBOARD, 0);
}
void CEditBar::OnMnuInsertRibbon()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_RIBBON, 0);
}
void CEditBar::OnMnuInsertBeam()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_BEAM, 0);
}
void CEditBar::OnMnuInsertSound()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_SOUND, 0);
}
void CEditBar::OnInsertDecal()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_DECAL, 0);
}

void CEditBar::OnMnuInsertGeom()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_GEOM, 0);
}

void CEditBar::OnMnuInsertVirtual()
{
	PostMainFrameMsg(EEM_NEW_EFFECT, EFFECT_VIRTUALNODE, 0);
}


void CEditBar::OnNMRclickEffectlist(NMHDR *pNMHDR, LRESULT *pResult)
{

	//CMenu menu;
	//menu.LoadMenu(IDR_MENU_LIST);
	POINT point;
	::GetCursorPos(&point);
	//if(m_wndEffectList.GetSelectionMark() != -1)
	//	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);

	CBCGPPopupMenu* popup = new CBCGPPopupMenu();
	popup->Create(this, point.x, point.y, ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_EFFECTLIST)), 0));

	*pResult = 0;
}

void CEditBar::OnNmItemActive(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iIndex = m_wndEffectList.GetSelectionMark();

	if(iIndex < 0)
		return;

	CEffectUnit* pUnit = CEffectMgr::GetInstance()->m_Effects[iIndex];

	CModifyEffectNameDlg dlg;
	dlg.SetTitle( "修改特效名字");
	dlg.SetDesc( "在此输入新的名字");
	dlg.m_strText = pUnit->GetName();
	if(dlg.DoModal() == IDOK)
	{
		pUnit->SetName(dlg.m_strText);
	}
	PostMainFrameMsg(EEM_UPDATA_LIST, UE_EFFECT_LIST, 0);
}

LRESULT CEditBar::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	if(pdn->hWnd == m_wndEffectList.GetSafeHwnd())
	{
		NEWEFFECT_INFO ni;
		ni.anchor = 0;
		ni.bDynamic = false;
		ni.offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		ni.scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);
		ni.path = (const char*)pdn->strChunk;
		ni.realpath = (const char*)pdn->strChunk;
		ni.bShowPane = true;
		ni.bIsPath = true;

		SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0, (LPARAM)&ni);
	}
	return 0;
}

void CEditBar::OnMnuEditeffect()
{
	int iIndex = m_wndEffectList.GetSelectionMark();
	LVITEM item;
	item.mask = LVIF_PARAM;
	item.iItem = iIndex;
	m_wndEffectList.GetItem(&item);

	PostMainFrameMsg(EEM_GOTO_PAGE, 0, item.lParam);
}

void CEditBar::OnMnuShow()
{
	int iIndex = m_wndEffectList.GetSelectionMark();
	SendMainFrameMsg(EEM_EFFECT_LIST_SELECTED, (WPARAM)iIndex, UNIT_VIEW_SHOW);
}

void CEditBar::OnUpdateMnuShow(CCmdUI* pUI)
{
	int iIndex = m_wndEffectList.GetSelectionMark();
	if(iIndex >= 0)
		pUI->Enable(TRUE);
	else
		pUI->Enable(FALSE);
}

void CEditBar::OnMnuHide()
{
	int iIndex = m_wndEffectList.GetSelectionMark();
	SendMainFrameMsg(EEM_EFFECT_LIST_SELECTED, (WPARAM)iIndex, UNIT_VIEW_HIDE);

}

void CEditBar::OnUpdateMnuHide(CCmdUI* pUI)
{
	int iIndex = m_wndEffectList.GetSelectionMark();
	if(iIndex >= 0)
		pUI->Enable(TRUE);
	else
		pUI->Enable(FALSE);
}

void CEditBar::UpdateList(std::vector<CEffectUnit*>& effects)
{
	if(m_hWnd == NULL)
		return ;
	m_wndEffectList.DeleteAllItems();
	for(int i = 0; i < (int)effects.size(); i++)
	{
		CString strType;
		CString strVisible = effects[i]->IsVisible() ? "显示" : "隐藏";
		CString strName = effects[i]->GetName();
		CString strBind = effects[i]->GetBindInfo()->bDynamic == true ? "动态" : "静态";
		CString strSave = effects[i]->GetBindInfo()->bIntegrated == true ? "集成" : "独立";
		switch(effects[i]->GetType())
		{
		case EFFECT_PEMITTER:
			strType = "粒子";
			break;
		case EFFECT_BILLBOARD:
			strType = "公告板";
			break;
		case EFFECT_LIGHT:
			strType = "光源";
			break;
		case EFFECT_RIBBON:
			strType = "条带";
			break;
		case EFFECT_BEAM:
			strType = "波带";
			break;
		case EFFECT_SOUND:
			strType = "声音";
			break;
		case EFFECT_DECAL:
			strType = "贴花";
			break;
		case EFFECT_VIRTUALNODE:
			strType = "虚拟体";
			break;		
		default:
			strType = "其他";
			break;
		}
		m_wndEffectList.InsertItem(LVIF_TEXT | LVIF_PARAM, i, strName.GetBuffer(), 0, 0, 0, (LPARAM)effects[i]);
		m_wndEffectList.SetItem(i, 1, LVIF_TEXT, strType.GetBuffer(), 0, 0, 0, 0);
		m_wndEffectList.SetItem(i, 2, LVIF_TEXT, strVisible.GetBuffer(), 0, 0, 0, 0);
		m_wndEffectList.SetItem(i, 3, LVIF_TEXT, strBind.GetBuffer(), 0, 0, 0, 0);
		m_wndEffectList.SetItem(i, 4, LVIF_TEXT, strSave.GetBuffer(), 0, 0, 0, 0);

	}

}

void CEditBar::OnBtnApplyAll()
{
	CEffectMgr* pEffectMgr = CEffectMgr::GetInstance();
	for(int i = 0; i < (int) pEffectMgr->m_Effects.size(); i++)
	{
		pEffectMgr->RefreshEffect(pEffectMgr->m_Effects[i]);
	}
}


void CEditBar::OnMnuSetIndependent()
{
	// TODO: Add your command handler code here
}

void CEditBar::OnUpdateMnuSetIndependent(CCmdUI *pCmdUI)
{
	//m_wndEffectList.GetFirstSelectedItemPosition();
	//m_wndEffectList.GetNextSelectedItem();
}

void CEditBar::OnMnuSetIntegrate()
{
	// TODO: Add your command handler code here
}

void CEditBar::OnUpdateMnuSetIntegrate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CEditBar::OnRefreshList()
{
	UpdateList(CEffectMgr::GetInstance()->m_Effects);
}


void CEditBar::OnBtnMoNewPost()
{
	// TODO: Add your command handler code here
}
