
#include "stdafx.h"
#include "Resource.h"
#include "EffectUnit_Ribbon.h"
#include "EffectUnitEditBar_Ribbon.h"
#include "SplitterGridView.h"
#include "NotifyProperty.h"
#include "EffectManager.h"
#include "ObjectTypeHelper.h"
#include "RibbonSectionDlg.h"

IMPLEMENT_DYNAMIC(CRibbonBar, CEffectEditBar)
CRibbonBar::CRibbonBar() : m_pTarget(NULL), m_bLockApp(true)
{
}

CRibbonBar::~CRibbonBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CRibbonBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGEDA, ID_RIBBONTREE, OnTreeSelChanged)
	ON_NOTIFY(NM_DBLCLK, ID_RIBBONTREE, OnTreeDblClick)

	ON_COMMAND(ID_BTN_RIB_NEWFRAME, OnNewFrame)
	ON_COMMAND(ID_BTN_RIB_DELFRAME, OnDeleteFrame)
	ON_COMMAND(ID_BTN_RIB_UP, OnMoveUp)
	ON_COMMAND(ID_BTN_RIB_DOWN, OnMoveDown)
	ON_COMMAND(ID_BTN_RIB_APPLY, OnApply)
	ON_COMMAND(ID_BTN_RIB_DELETE, OnRemove)
	ON_COMMAND(ID_BTN_RIB_SAVE, OnSave)
	ON_UPDATE_COMMAND_UI(ID_BTN_RIB_VISIBLE, OnUpdateVisible)
	ON_COMMAND(ID_BTN_RIB_CLICK, OnClick)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickPropBtn)

	ON_COMMAND(ID_BTN_RIB_LOCKAPPLY, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_RIB_LOCKAPPLY, OnUpdateBtnLockApply)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CRibbonBar message handlers

//void CRibbonBar::SetEditTarget(CEffectUnit* pEffect)
//{
//	m_pTarget = (CRibbon*)pEffect;
//}
//
//CEffectUnit* CRibbonBar::GetEditTarget()
//{
//	return m_pTarget;
//}

int CRibbonBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this) 
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_RIBBON))
	{
		return -1;
	}
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	CCreateContext context1;
	context1.m_pNewViewClass = RUNTIME_CLASS(CSplitterGridView);
	context1.m_pCurrentDoc = NULL;
	context1.m_pNewDocTemplate = NULL;
	context1.m_pLastView = NULL;
	context1.m_pCurrentFrame = NULL;

	CCreateContext context2;
	context2.m_pNewViewClass = RUNTIME_CLASS(CSplitterGridView);
	context2.m_pCurrentDoc = NULL;
	context2.m_pNewDocTemplate = NULL;
	context2.m_pLastView = NULL;
	context2.m_pCurrentFrame = NULL;

	CRect rectClient;
	GetClientRect (rectClient);

	m_wndSplitter.CreateStatic(this, 2, 1);
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height () / 2),&context1);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height () / 2),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	CSplitterGridView* m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	CSplitterGridView* m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	m_wndTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS , 
		CRect(0, 0, 100, 100), m_pGrid1, ID_RIBBONTREE);
	m_hDesc = m_wndTree.InsertItem(_T("条带参数"));
	m_hKeyFrame = m_wndTree.InsertItem(_T("关键帧"));


	m_pGrid1->SetInnerCtrl(&m_wndTree);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_RIBBONPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);

	//ChangePropList();
	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	return 0;
}

void CRibbonBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}


BOOL CRibbonBar::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call	the base class
	//SAFE_DELETE( m_pGrid1);
	//SAFE_DELETE (m_pGrid2);
	return CEffectEditBar::DestroyWindow();
}

void CRibbonBar::OnTreeDblClick(NMHDR* pHdr, LRESULT* pResult)
{
	HTREEITEM item = m_wndTree.GetSelectedItem();
	if(item != NULL && 
		(item == m_hKeyFrame || m_wndTree.GetParentItem(item) == m_hKeyFrame))
	{
		UpdateKeyFrameWnd();
	}

}

LRESULT CRibbonBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	if(m_hLastSel != NULL)
		SaveCurrentProp(m_hLastSel);
	else
		SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->UpdateSource();

	if(m_bLockApp && (pnm->pProp == m_pPropMaxSeg
		|| pnm->pProp == m_pPropLifeSpan
		|| pnm->pProp == m_pPropEmitteRate
		|| pnm->pProp == m_pPropBlendMode
		|| pnm->pProp == m_pPropTexture
		|| pnm->pProp == m_pPropBindBone
		|| pnm->pProp == m_pPropDistort
		|| pnm->pProp == m_pPropAddressMode
		|| pnm->pProp == m_pPropLerpSeq
		|| pnm->pProp == m_pPropMaskTexture
		|| pnm->pProp == m_pPropMaskUVAddressMode
		))
	{
		//m_pTarget->RefreshNode();
		//m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
	return 0;
}

LRESULT CRibbonBar::OnClickPropBtn(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	CCvtHelper cvt;
	CString str;

	if(pnm->pProp == m_pPropEditTemplate)
	{
		CRibbonSectionDlg dlg;
		dlg.m_indexes = m_pTarget->m_Template.m_lines;
		dlg.m_vectors = m_pTarget->m_Template.m_dots;
		dlg.m_texv = m_pTarget->m_Template.m_vdata;
		if(dlg.DoModal() == IDOK)
		{
			m_pTarget->m_Template.m_lines = dlg.m_indexes;
			m_pTarget->m_Template.m_dots = dlg.m_vectors;
			m_pTarget->m_Template.m_vdata = dlg.m_texv;

			CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
		}
	}
	return 0;
}

void CRibbonBar::OnDestroy()
{
	CEffectEditBar::OnDestroy();
}

void CRibbonBar::OnNewFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	CRibbon::RIB_FRAME item;
	if(m_pTarget->m_KeyFrames.size() > 0)
		item.lTick = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1].lTick + 1000;

	m_pTarget->m_KeyFrames.push_back(item);

	UpdateTree();
	m_pTarget->UpdateSource();
	UpdateKeyFrameWnd();
}

void CRibbonBar::OnDeleteFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return ;

	CRibbon::RIB_FRAME* pFrameItem = (CRibbon::RIB_FRAME*)GetItemParam(hSelected);

	std::vector<CRibbon::RIB_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(&(*it) == pFrameItem)
		{
			m_pTarget->m_KeyFrames.erase(it);
			break;
		}
	}
	m_wndTree.SelectItem(m_hKeyFrame);
	UpdateTree();

	UpdateKeyFrameWnd();
}

void CRibbonBar::OnMoveUp()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return;
	CRibbon::RIB_FRAME* pItem = (CRibbon::RIB_FRAME* )GetItemParam(hSelected);
	std::vector<CRibbon::RIB_FRAME>::iterator back = m_pTarget->m_KeyFrames.end();
	std::vector<CRibbon::RIB_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(pItem == &(*it) && back != m_pTarget->m_KeyFrames.end())
		{
			CRibbon::RIB_FRAME tmp = *it;
			m_pTarget->m_KeyFrames.erase(it);
			m_pTarget->m_KeyFrames.insert(back, tmp);
			break;
		}
		back = it;
	}
	UpdateTree();
}

void CRibbonBar::OnMoveDown()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return;
	CRibbon::RIB_FRAME* pItem = (CRibbon::RIB_FRAME* )GetItemParam(hSelected);
	std::vector<CRibbon::RIB_FRAME>::iterator back = m_pTarget->m_KeyFrames.end();
	std::vector<CRibbon::RIB_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(back != m_pTarget->m_KeyFrames.end() && pItem == &(*back))
		{
			CRibbon::RIB_FRAME tmp = *it;
			m_pTarget->m_KeyFrames.erase(it);
			m_pTarget->m_KeyFrames.insert(back, tmp);
			break;
		}
		back = it;
	}
	UpdateTree();
}

void CRibbonBar::OnApply()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	//PostMainFrameMsg( EEM_APPLY);
	//PostMainFrameMsg(EEM_UPDATA_LIST);
	//m_pTarget->RefreshNode();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}

void CRibbonBar::OnClick()
{
	m_pTarget->GetInteract()->OnLBtnDown(0.0f, 0.0f, true);
	CEffectMgr::GetInstance()->SetActiveEffect(m_pTarget);
}

void CRibbonBar::OnRemove()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CRibbonBar::OnSave()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	SendMainFrameMsg( EEM_SAVE, 0, (LPARAM)m_pTarget);
}

void CRibbonBar::OnVisible()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg( EEM_APPLY);
}

void CRibbonBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->IsVisible() ? 1 : 0);
}

void CRibbonBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CRibbonBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CRibbonBar::OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_hLastSel = pNm->itemOld.hItem;
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(pNm->itemOld.hItem);
	ChangePropList();
	m_hLastSel = NULL;


}

extern bool IsTextureResource(const char *path);

LRESULT CRibbonBar::OnDropRes(WPARAM w, LPARAM l)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)l;
	if(pdn->hWnd == m_wndPropList.GetSafeHwnd())
	{
		if(!IsTextureResource(pdn->strChunk)) return 0;
		if(m_wndTree.GetSelectedItem() == m_hDesc)
		{
			if( m_pPropTexture->GetRect().PtInRect(pdn->dropPos))
			{
				m_pPropTexture->SetValue(pdn->strChunk);
				PROPNM nm;
				nm.pProp = (CNotifiedProp*)m_pPropTexture;
				OnUpdatePropValue(0, (LPARAM)&nm);
			}
			else if(m_pPropMaskTexture->GetRect().PtInRect(pdn->dropPos))
			{
				m_pPropMaskTexture->SetValue(pdn->strChunk);
				PROPNM nm;
				nm.pProp = (CNotifiedProp*)m_pPropMaskTexture;
				OnUpdatePropValue(0, (LPARAM)&nm);
			}
		}
	}
	return 0;
}

void CRibbonBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CRibbon*) pEffect;
	if(GetSafeHwnd())
	{
		SetWindowText(m_pTarget->m_strName.GetBuffer());
		UpdateTree();
		ChangePropList();
	}
}
CEffectUnit* CRibbonBar::GetEditTarget()
{
	return m_pTarget;
}

LPARAM CRibbonBar::GetItemParam(HTREEITEM hItem)
{
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;
	m_wndTree.GetItem(&item);
	return item.lParam;
}

void CRibbonBar::UpdateTree()
{
	HTREEITEM hItem = m_wndTree.GetChildItem(m_hKeyFrame);
	HTREEITEM hTmp = NULL;
	while(hItem)
	{
		hTmp = hItem;
		hItem = m_wndTree.GetNextSiblingItem(hItem);
		m_wndTree.DeleteItem(hTmp);
	}

	for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
	{
		CString strName;
		strName.Format(_T("关键帧 %d"), i + 1);
		m_wndTree.InsertItem(TVIF_TEXT | TVIF_PARAM,strName.GetBuffer(), 0, 0, 0,0,
			(LPARAM)&m_pTarget->m_KeyFrames[i], m_hKeyFrame, TVI_LAST);
	}
	m_wndTree.Expand(m_hKeyFrame, TVE_EXPAND);
	m_wndTree.SelectItem(m_hDesc);
}

void CRibbonBar::ChangePropList()
{
	m_wndPropList.EndEditItem(TRUE);

	m_wndPropList.RemoveAll();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{

		DescUpdate(CREATE_PROP);

		m_pPropMaxSeg->SetNotifyTarget(GetSafeHwnd());
		m_pPropLifeSpan->SetNotifyTarget(GetSafeHwnd());
		m_pPropEmitteRate->SetNotifyTarget(GetSafeHwnd());
		m_pPropBlendMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropTexture->SetNotifyTarget(GetSafeHwnd());
		m_pPropBindBone->SetNotifyTarget(GetSafeHwnd());
		m_pPropDistort->SetNotifyTarget(GetSafeHwnd());
		m_pPropAddressMode->SetNotifyTarget(GetSafeHwnd());
		//m_pPropLifeSpan->SetFloatDigitRatio(0.01f);
		m_pPropLerpSeq->SetNotifyTarget(GetSafeHwnd());


		m_pPropSeeMask->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUVAddressMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskTexture->SetNotifyTarget(GetSafeHwnd());
		m_pPropEditTemplate->SetNotifyTarget(GetSafeHwnd());


		CBCGPProp* pTex = new CBCGPProp("条带贴图");
		pTex->AddSubItem(m_pPropAddressMode);
		pTex->AddSubItem(m_pPropTexture);


		CBCGPProp* pMaskTex = new CBCGPProp("蒙板贴图");
		pMaskTex->AddSubItem(m_pPropSeeMask);
		pMaskTex->AddSubItem(m_pPropMaskUVAddressMode);
		pMaskTex->AddSubItem(m_pPropMaskTexture);

		m_wndPropList.AddProperty(m_pPropMaxSeg);
		m_wndPropList.AddProperty(m_pPropLifeSpan);
		m_wndPropList.AddProperty(m_pPropEmitteRate);
		m_wndPropList.AddProperty(m_pPropBlendMode);
		m_wndPropList.AddProperty(m_pPropLerpSeq);
		m_wndPropList.AddProperty(m_pPropDistort);
		m_wndPropList.AddProperty(m_pPropEditTemplate);

		m_wndPropList.AddProperty(pTex);
		m_wndPropList.AddProperty(pMaskTex);

		m_wndPropList.AddProperty(m_pPropBindBone);

	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CRibbon::RIB_FRAME* pFrameItem = (CRibbon::RIB_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());

		KeyFrameUpdate(CREATE_PROP, pFrameItem);

		//((CNotifiedProp*)m_pPropColor)->SetNotifyTarget(GetSafeHwnd());
		m_pPropAlpha->SetNotifyTarget(GetSafeHwnd());
		m_pPropAbove->SetNotifyTarget(GetSafeHwnd());
		m_pPropBelow->SetNotifyTarget(GetSafeHwnd());
		m_pPropTick->SetNotifyTarget(GetSafeHwnd());

		m_pPropUVRotate->SetNotifyTarget(GetSafeHwnd());
		m_pPropUScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropVScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropUOffset->SetNotifyTarget(GetSafeHwnd());
		m_pPropVOffset->SetNotifyTarget(GetSafeHwnd());

		m_pPropMaskUVRotate->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskVScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUOffset->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskVOffset->SetNotifyTarget(GetSafeHwnd());


		CBCGPProp* pTex = new CBCGPProp("条带贴图");
		//CBCGPProp* pUVScale = new CBCGPProp("UV缩放");
		pTex->AddSubItem(m_pPropUScale);
		pTex->AddSubItem(m_pPropVScale);
		//CBCGPProp* pUVOffset = new CBCGPProp("UV偏移");
		pTex->AddSubItem(m_pPropUOffset);
		pTex->AddSubItem(m_pPropVOffset);
		pTex->AddSubItem(m_pPropUVRotate);


		CBCGPProp* pMaskTex = new CBCGPProp("蒙板贴图");
		//CBCGPProp* pMaskUVScale = new CBCGPProp("UV缩放");
		pMaskTex->AddSubItem(m_pPropMaskUScale);
		pMaskTex->AddSubItem(m_pPropMaskVScale);
		//CBCGPProp* pMaskUVOffset = new CBCGPProp("UV偏移");
		pMaskTex->AddSubItem(m_pPropMaskUOffset);
		pMaskTex->AddSubItem(m_pPropMaskVOffset);
		pMaskTex->AddSubItem(m_pPropMaskUVRotate);



		m_wndPropList.AddProperty(pTex);
		m_wndPropList.AddProperty(pMaskTex);


		m_wndPropList.AddProperty(m_pPropColor);
		m_wndPropList.AddProperty(m_pPropAlpha);
		m_wndPropList.AddProperty(m_pPropAbove);
		m_wndPropList.AddProperty(m_pPropBelow);
		m_wndPropList.AddProperty(m_pPropTick); 

	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
		{
			CString strName;
			strName.Format(_T("关键帧 %d"), i + 1);
			CBCGPProp* pProp = new CBCGPProp(strName, _T(""));
			//((CNotifiedProp*)pProp)->SetNotifyTarget(GetSafeHwnd());
			m_wndPropList.AddProperty(pProp);
		}
	}

	UpdateList();
}

void CRibbonBar::SaveCurrentProp(HTREEITEM hOldItem)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	m_wndPropList.EndEditItem(FALSE);

	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	if(hOldItem == m_hDesc)
	{
		DescUpdate(PROP_TO_DATA);

	}
	else if(m_wndTree.GetParentItem(hOldItem) == m_hKeyFrame)
	{
		CRibbon::RIB_FRAME* pFrameItem = (CRibbon::RIB_FRAME*)GetItemParam(hOldItem);
		KeyFrameUpdate(PROP_TO_DATA, pFrameItem);
	}
	else if(hOldItem == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
		{
			cvt.ParseInt(OLE2T((_bstr_t)m_wndPropList.GetProperty(i)->GetValue()), (int*)&m_pTarget->m_KeyFrames[i].lTick);
		}
	}

	m_pTarget->EndRecordCommand(pCmd);
}

void CRibbonBar::UpdateList()
{
	CCvtHelper cvt;
	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		DescUpdate(DATA_TO_PROP);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CRibbon::RIB_FRAME* pFrameItem = (CRibbon::RIB_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		KeyFrameUpdate(DATA_TO_PROP, pFrameItem);
	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
			m_wndPropList.GetProperty(i)->SetValue(cvt.IntToString(m_pTarget->m_KeyFrames[i].lTick));
	}
	m_wndPropList.Invalidate();
}

void CRibbonBar::DescUpdate(DATA_UPDATA_TYPE type)
{
	PropUpdate_int((CBCGPProp**)&m_pPropMaxSeg, &m_pTarget->m_descEx.m_MaxSegsNew, type, _T("最大区块数"), 1.0f, 32767, 1);
	PropUpdate_float((CBCGPProp**)&m_pPropLifeSpan, &m_pTarget->m_desc.m_LifeSpan, type, _T("生命长度"), 0.05f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropEmitteRate, &m_pTarget->m_desc.m_EmitteRate, type, _T("发射速率"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_blendtype((CBCGPProp**)&m_pPropBlendMode, (Ogre::UIBlendMode*)&m_pTarget->m_desc.m_BlendMode, type, _T("混合模式"));

	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("绑定点"));

	PropUpdate_int((CBCGPProp**)&m_pPropLerpSeq, &m_pTarget->m_descEx.m_LerpSeg,type, _T("平滑段数"), 0.1f, 32767.0f, 1.0f);

	PropUpdate_bool((CBCGPProp**)&m_pPropDistort, &m_pTarget->m_descEx.m_bDistort,type, _T("空间扭曲模式"));

	PropUpdate_addressmode((CBCGPProp**)&m_pPropAddressMode, &m_pTarget->m_descEx.m_iUVAddressMode,type, _T("贴图寻址模式"));
	PropUpdate_cstring((CBCGPProp**)&m_pPropTexture, &m_pTarget->m_strTexture, type, _T("贴图"));

	PropUpdate_addressmode((CBCGPProp**)&m_pPropMaskUVAddressMode, &m_pTarget->m_descEx.m_iMaskUVAddrMode,type, _T("蒙板寻址模式"));
	PropUpdate_cstring((CBCGPProp**)&m_pPropMaskTexture, &m_pTarget->m_strMaskTexture, type, _T("蒙板贴图"));

	PropUpdate_bool((CBCGPProp**)&m_pPropSeeMask, &m_pTarget->m_bSeeMask, type, _T("显示蒙板效果"));

	if(type == CREATE_PROP)
	{
		m_pPropEditTemplate = new CNotifiedProp("编辑截面", _T(""), TRUE, GetSafeHwnd());
	}
}
void CRibbonBar::KeyFrameUpdate(DATA_UPDATA_TYPE type, CRibbon::RIB_FRAME* pItem)
{
	PropUpdate_color(&m_pPropColor, &(pItem->color), type, _T("颜色"));
	PropUpdate_float((CBCGPProp**)&m_pPropAlpha, &(pItem->color.a), type, _T("透明度"), 0.05f, 1.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropAbove, &(pItem->above), type, _T("条带宽度"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropBelow, &(pItem->below), type, _T("条带高度"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_int((CBCGPProp**)&m_pPropTick, (int*)(&pItem->lTick), type, _T("帧时间"));

	PropUpdate_float((CBCGPProp**)&m_pPropUVRotate,		&pItem->uvrot, type, _T("UV旋转"), 1.0f, 180.0f, -180.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropUScale,		&pItem->uscale, type, _T("U缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropVScale,		&pItem->vscale, type, _T("V缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropUOffset,		&pItem->uoffset, type, _T("U偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropVOffset,		&pItem->voffset, type, _T("V偏移"), 0.01f, 32767.0f, -32767.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVRotate,		&pItem->maskuvrot, type, _T("UV旋转"), 1.0f, 180.0f, -180.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUScale,		&pItem->maskuscale, type, _T("U缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVScale,		&pItem->maskvscale, type, _T("V缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUOffset,		&pItem->maskuoffset, type, _T("U偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVOffset,		&pItem->maskvoffset, type, _T("V偏移"), 0.01f, 32767.0f, -32767.0f);
}
void CRibbonBar::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CEffectEditBar::OnClose();
}

void CRibbonBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CEffectEditBar::OnShowWindow(bShow, nStatus);
	if(bShow == FALSE)
	{
		ReleaseKeyFrameWnd();
	}
	// TODO: Add your message handler code here
}


void CRibbonBar::UpdateKeyFrameWnd()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_REQUEST_KFWND, 0, (LPARAM) pObj);
}

void CRibbonBar::ReleaseKeyFrameWnd()
{
	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_RELEASE_KFWND, 0, (LPARAM) pObj);
}

void CRibbonBar::CopyKeyFrame(int from, int to)
{
	if(from < 0 || from > (int)m_pTarget->m_KeyFrames.size() 
		|| to < 0 || to > (int)m_pTarget->m_KeyFrames.size() )
	{
		return;
	}

	long tick = m_pTarget->m_KeyFrames[to].lTick;
	m_pTarget->m_KeyFrames[to] = m_pTarget->m_KeyFrames[from];
	m_pTarget->m_KeyFrames[to].lTick = tick;
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);

}

#define RIB_LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))
int CRibbonBar::AddKeyFrame(UINT tick)
{
	m_wndPropList.EndEditItem(FALSE);
	CRibbon::RIB_FRAME rib;
	rib.lTick = tick;

	int i = 0;
	for(i = 0; i < (int) m_pTarget->m_KeyFrames.size(); i++)
	{
		if(m_pTarget->m_KeyFrames[i].lTick > rib.lTick)
			break;
	}

	ASSERT( i > 0 && "Add KeyFrame, Never add at first ");
	if(i == (int) m_pTarget->m_KeyFrames.size())
	{
		CRibbon::RIB_FRAME& last = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1];
		rib.above = last.above;
		rib.below = last.below;
		rib.color = last.color;
		rib.uvrot = last.uvrot;
		rib.voffset = last.voffset;
		rib.uoffset = last.uoffset;
		rib.uscale = last.uscale;
		rib.vscale = last.vscale;

		rib.maskuvrot = last.maskuvrot;
		rib.maskvoffset = last.maskvoffset;
		rib.maskuoffset = last.maskuoffset;
		rib.maskuscale = last.maskuscale;
		rib.maskvscale = last.maskvscale;

		m_pTarget->m_KeyFrames.push_back(rib);
	}
	else
	{
		CRibbon::RIB_FRAME& next = m_pTarget->m_KeyFrames[i];
		CRibbon::RIB_FRAME& prev = m_pTarget->m_KeyFrames[i - 1];
		float pos = (float)(rib.lTick - prev.lTick ) / ( next.lTick - prev.lTick );

		rib.above = RIB_LINEAR_LERP(prev.above, next.above, pos);
		rib.below = RIB_LINEAR_LERP(prev.below, next.below, pos);
		rib.color.r = RIB_LINEAR_LERP(prev.color.r, next.color.r, pos);
		rib.color.g = RIB_LINEAR_LERP(prev.color.g, next.color.g, pos);
		rib.color.b = RIB_LINEAR_LERP(prev.color.b, next.color.b, pos);
		rib.color.a = RIB_LINEAR_LERP(prev.color.a, next.color.a, pos);

		rib.uvrot = RIB_LINEAR_LERP(prev.uvrot, next.uvrot, pos);
		rib.voffset = RIB_LINEAR_LERP(prev.voffset, next.voffset, pos);
		rib.uoffset = RIB_LINEAR_LERP(prev.uoffset, next.uoffset, pos);
		rib.uscale = RIB_LINEAR_LERP(prev.uscale, next.uscale, pos);
		rib.vscale = RIB_LINEAR_LERP(prev.vscale, next.vscale, pos);


		rib.maskuvrot = RIB_LINEAR_LERP(prev.maskuvrot, next.maskuvrot, pos);
		rib.maskvoffset = RIB_LINEAR_LERP(prev.maskvoffset, next.maskvoffset, pos);
		rib.maskuoffset = RIB_LINEAR_LERP(prev.maskuoffset, next.maskuoffset, pos);
		rib.maskuscale = RIB_LINEAR_LERP(prev.maskuscale, next.maskuscale, pos);
		rib.maskvscale = RIB_LINEAR_LERP(prev.maskvscale, next.maskvscale, pos);

		m_pTarget->m_KeyFrames.insert(m_pTarget->m_KeyFrames.begin() + i, rib);
	}
	UpdateTree();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	return i;

}

void CRibbonBar::DeleteKeyFrame(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames.erase(m_pTarget->m_KeyFrames.begin() + index);
		UpdateTree();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

void CRibbonBar::ActiveKeyFrame(int index)
{
	int i = 0;
	HTREEITEM hCurr = m_wndTree.GetChildItem(m_hKeyFrame);
	while(hCurr != NULL)
	{
		CRibbon::RIB_FRAME* pFrameItem = (CRibbon::RIB_FRAME*)GetItemParam(hCurr);
		if(index<m_pTarget->m_KeyFrames.size() && pFrameItem == &m_pTarget->m_KeyFrames[index])
			break;
		hCurr = m_wndTree.GetNextSiblingItem(hCurr);
	}

	if(hCurr == NULL)
	{
		UpdateTree();
	}
	else
	{
		m_wndTree.SelectItem(hCurr);
	}

}

void CRibbonBar::SetKeyFramePos(int index, UINT tick)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames[index].lTick = tick;
		UpdateList();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

int CRibbonBar::GetKeyFrameCount()
{
	return (int)m_pTarget->m_KeyFrames.size();
}

UINT CRibbonBar::GetKeyFrameTick(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		return (UINT)m_pTarget->m_KeyFrames[index].lTick;
	}
	return -1;
}

CString CRibbonBar::GetName()
{
	return m_pTarget->GetName();
}
