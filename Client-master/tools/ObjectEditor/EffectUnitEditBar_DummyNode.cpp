
#include "stdafx.h"
#include "EffectUnit_VirtualNode.h"
#include "EffectUnitEditBar_DummyNode.h"
#include "SplitterGridView.h"
#include "NotifyProperty.h"
#include "EffectManager.h"
#include "ObjectTypeHelper.h"

IMPLEMENT_DYNAMIC(CVirtualNodeBar, CEffectEditBar)
CVirtualNodeBar::CVirtualNodeBar()
{
}

CVirtualNodeBar::~CVirtualNodeBar()
{
}


BEGIN_MESSAGE_MAP(CVirtualNodeBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_NOTIFY(TVN_SELCHANGEDA, ID_VIRTUALTREE, OnTreeSelChanged)
	ON_NOTIFY(NM_DBLCLK, ID_VIRTUALTREE, OnTreeDblClick)

	ON_COMMAND(ID_BTN_VIR_APPLY, OnApply)
	ON_COMMAND(ID_BTN_VIR_REMOVE, OnRemove)
	ON_COMMAND(ID_BTN_VIR_CLICK, OnClick)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)


	ON_COMMAND(ID_BTN_VIR_LOCKAPP, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_VIR_LOCKAPP, OnUpdateBtnLockApply)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()

END_MESSAGE_MAP()



// CVirtualNodeBar message handlers


int CVirtualNodeBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_VIRTUALNODE))
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
		CRect(0, 0, 100, 100), m_pGrid1, ID_VIRTUALTREE);
	m_hDesc = m_wndTree.InsertItem(_T("虚拟体参数"));
	m_hKeyFrame = m_wndTree.InsertItem(_T("关键帧"));


	m_pGrid1->SetInnerCtrl(&m_wndTree);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_VIRTUALPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);

	//ChangePropList();
	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());


	return 0;
}

void CVirtualNodeBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}


void CVirtualNodeBar::OnTreeDblClick(NMHDR* pHdr, LRESULT* pResult)
{
	HTREEITEM item = m_wndTree.GetSelectedItem();
	if(item != NULL && 
		(item == m_hKeyFrame || m_wndTree.GetParentItem(item) == m_hKeyFrame))
	{
		UpdateKeyFrameWnd();
	}

}

LRESULT CVirtualNodeBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	if(m_hLastSel != NULL)
		SaveCurrentProp(m_hLastSel);
	else
		SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->UpdateSource();

	if(m_bLockApp && (pnm->pProp == (CNotifiedProp*)m_pPropBindBone))
	{
		//m_pTarget->RefreshNode();
		//m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
	return 0;
}


void CVirtualNodeBar::OnApply()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	//PostMainFrameMsg( EEM_APPLY);
	//PostMainFrameMsg(EEM_UPDATA_LIST);
	//m_pTarget->RefreshNode();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}

void CVirtualNodeBar::OnClick()
{
	m_pTarget->GetInteract()->OnLBtnDown(0.0f, 0.0f, true);
	CEffectMgr::GetInstance()->SetActiveEffect(m_pTarget);
}

void CVirtualNodeBar::OnRemove()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}



void CVirtualNodeBar::OnVisible()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg( EEM_APPLY);
}

void CVirtualNodeBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->IsVisible() ? 1 : 0);
}

void CVirtualNodeBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CVirtualNodeBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CVirtualNodeBar::OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_hLastSel = pNm->itemOld.hItem;
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(pNm->itemOld.hItem);
	ChangePropList();
	m_hLastSel = NULL;


}

void CVirtualNodeBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CVirtualNode*) pEffect;
	if(GetSafeHwnd())
	{
		SetWindowText(m_pTarget->m_strName.GetBuffer());
		UpdateTree();
		ChangePropList();
	}
}
CEffectUnit* CVirtualNodeBar::GetEditTarget()
{
	return m_pTarget;
}

LPARAM CVirtualNodeBar::GetItemParam(HTREEITEM hItem)
{
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;
	m_wndTree.GetItem(&item);
	return item.lParam;
}

void CVirtualNodeBar::UpdateTree()
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

void CVirtualNodeBar::ChangePropList()
{
	m_wndPropList.EndEditItem(TRUE);

	m_wndPropList.RemoveAll();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{

		DescUpdate(CREATE_PROP);

		CBCGPProp* pPos = new CBCGPProp(_T("相对位置"));
		pPos->AddSubItem(m_pPropOffsetX);
		pPos->AddSubItem(m_pPropOffsetY);
		pPos->AddSubItem(m_pPropOffsetZ);

		CBCGPProp* pRot = new CBCGPProp(_T("旋转"));
		pRot->AddSubItem(m_pPropYaw);
		pRot->AddSubItem(m_pPropPitch);
		pRot->AddSubItem(m_pPropRoll);

		((CNotifiedProp*)m_pPropOffsetX)->SetNotifyTarget(GetSafeHwnd());
		((CNotifiedProp*)m_pPropOffsetY)->SetNotifyTarget(GetSafeHwnd());
		((CNotifiedProp*)m_pPropOffsetZ)->SetNotifyTarget(GetSafeHwnd());

		((CNotifiedProp*)m_pPropYaw)->SetNotifyTarget(GetSafeHwnd());
		((CNotifiedProp*)m_pPropPitch)->SetNotifyTarget(GetSafeHwnd());
		((CNotifiedProp*)m_pPropRoll)->SetNotifyTarget(GetSafeHwnd());

		((CNotifiedProp*)m_pPropBindBone)->SetNotifyTarget(GetSafeHwnd());

		//m_pPropLifeSpan->SetFloatDigitRatio(0.01f);
		((CNotifiedProp*)m_pShowBox)->SetNotifyTarget(GetSafeHwnd());


		m_wndPropList.AddProperty(pPos);
		m_wndPropList.AddProperty(pRot);

		m_wndPropList.AddProperty(m_pPropBindBone);

		CBCGPProp* pShowBox = new CBCGPProp(_T("显示包围盒子"));
		pShowBox->AddSubItem(m_pShowBox);
		m_wndPropList.AddProperty(pShowBox);

	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CVirtualNode::VIR_FRAME* pFrameItem = (CVirtualNode::VIR_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());

		KeyFrameUpdate(CREATE_PROP, pFrameItem);

		((CNotifiedProp*)m_pPropTick)->SetNotifyTarget(GetSafeHwnd());

		//m_pPropAlpha->SetFloatDigitRatio(0.01f);
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

void CVirtualNodeBar::SaveCurrentProp(HTREEITEM hOldItem)
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
		CVirtualNode::VIR_FRAME* pFrameItem = (CVirtualNode::VIR_FRAME*)GetItemParam(hOldItem);
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

void CVirtualNodeBar::UpdateList()
{
	CCvtHelper cvt;
	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		DescUpdate(DATA_TO_PROP);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CVirtualNode::VIR_FRAME* pFrameItem = (CVirtualNode::VIR_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		KeyFrameUpdate(DATA_TO_PROP, pFrameItem);
	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
			m_wndPropList.GetProperty(i)->SetValue(cvt.IntToString(m_pTarget->m_KeyFrames[i].lTick));
	}
	m_wndPropList.Invalidate();
}

void CVirtualNodeBar::DescUpdate(DATA_UPDATA_TYPE type)
{
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetX, &m_pTarget->m_bindInfo.offset.x, type, _T("X"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetY, &m_pTarget->m_bindInfo.offset.y, type, _T("Y"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetZ, &m_pTarget->m_bindInfo.offset.z, type, _T("Z"));

	PropUpdate_rot((CBCGPProp**)&m_pPropYaw, (CBCGPProp**)&m_pPropPitch,(CBCGPProp**)&m_pPropRoll, &m_pTarget->m_bindInfo.rotate, type, _T("旋转"));
	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("绑定点"));

	PropUpdate_bool((CBCGPProp**)&m_pShowBox, &m_pTarget->m_bShowBox, type, _T("显示Box"));

	bool is = m_pTarget->m_bShowBox;
}
void CVirtualNodeBar::KeyFrameUpdate(DATA_UPDATA_TYPE type, CVirtualNode::VIR_FRAME* pItem)
{
	PropUpdate_int((CBCGPProp**)&m_pPropTick, (int*)(&pItem->lTick), type, _T("帧时间"));
}
void CVirtualNodeBar::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CEffectEditBar::OnClose();
}

void CVirtualNodeBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CEffectEditBar::OnShowWindow(bShow, nStatus);
	if(bShow == FALSE)
	{
		ReleaseKeyFrameWnd();
	}
	// TODO: Add your message handler code here
}


void CVirtualNodeBar::UpdateKeyFrameWnd()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_REQUEST_KFWND, 0, (LPARAM) pObj);
}

void CVirtualNodeBar::ReleaseKeyFrameWnd()
{
	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_RELEASE_KFWND, 0, (LPARAM) pObj);
}

void CVirtualNodeBar::CopyKeyFrame(int from, int to)
{
	return;

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

#define VIR_LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))
int CVirtualNodeBar::AddKeyFrame(UINT tick)
{

	m_wndPropList.EndEditItem(FALSE);
	return 0;
	CVirtualNode::VIR_FRAME rib;
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
		CVirtualNode::VIR_FRAME& last = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1];
		//rib.above = last.above;
		//rib.below = last.below;
		//rib.color = last.color;
		m_pTarget->m_KeyFrames.push_back(rib);
	}
	else
	{
		CVirtualNode::VIR_FRAME& next = m_pTarget->m_KeyFrames[i];
		CVirtualNode::VIR_FRAME& prev = m_pTarget->m_KeyFrames[i - 1];
		float pos = (float)(rib.lTick - prev.lTick ) / ( next.lTick - prev.lTick );

		//rib.above = RIB_LINEAR_LERP(prev.above, next.above, pos);
		//rib.below = RIB_LINEAR_LERP(prev.below, next.below, pos);
		//rib.color.r = RIB_LINEAR_LERP(prev.color.r, next.color.r, pos);
		//rib.color.g = RIB_LINEAR_LERP(prev.color.g, next.color.g, pos);
		//rib.color.b = RIB_LINEAR_LERP(prev.color.b, next.color.b, pos);
		//rib.color.a = RIB_LINEAR_LERP(prev.color.a, next.color.a, pos);

		m_pTarget->m_KeyFrames.insert(m_pTarget->m_KeyFrames.begin() + i, rib);
	}
	UpdateTree();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	return i;

}

void CVirtualNodeBar::DeleteKeyFrame(int index)
{
	return;
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames.erase(m_pTarget->m_KeyFrames.begin() + index);
		UpdateTree();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

void CVirtualNodeBar::ActiveKeyFrame(int index)
{
	return;
	int i = 0;
	HTREEITEM hCurr = m_wndTree.GetChildItem(m_hKeyFrame);
	while(hCurr != NULL)
	{
		CVirtualNode::VIR_FRAME* pFrameItem = (CVirtualNode::VIR_FRAME*)GetItemParam(hCurr);
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

void CVirtualNodeBar::SetKeyFramePos(int index, UINT tick)
{
	return;
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames[index].lTick = tick;
		UpdateList();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

int CVirtualNodeBar::GetKeyFrameCount()
{
	return (int)m_pTarget->m_KeyFrames.size();
}

UINT CVirtualNodeBar::GetKeyFrameTick(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		return (UINT)m_pTarget->m_KeyFrames[index].lTick;
	}
	return -1;
}

CString CVirtualNodeBar::GetName()
{
	return m_pTarget->GetName();
}
