
#include "stdafx.h"
#include "AnchorEditBar.h"
#include "SplitterGridView.h"
#include "AnchorSet.h"
#include "EffectManager.h"
#include "NotifyProperty.h"
#include "SkeletonInfo.h"
#include "EffectUnit.h"
#include "ObjectTypeHelper.h"
#include "BindInfo.h"
#include <vector>
#include <string>

IMPLEMENT_DYNAMIC(CAnchorPane, CWnd)
CAnchorPane::CAnchorPane() : m_pAnchor(NULL)
{
}

CAnchorPane::~CAnchorPane()
{
}


BEGIN_MESSAGE_MAP(CAnchorPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, ID_ANCHORTREE, OnAchortreeSelchange)
END_MESSAGE_MAP()



// CAnchorPane message handlers


int CAnchorPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_wndToolBar.Create(this);

	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_BONE);
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

	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context1);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 1);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_ANCHORPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);

	m_wndTree.Create(WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, CRect(0, 0, 100, 100), m_pGrid1, ID_ANCHORTREE);
	m_pGrid1->SetInnerCtrl(&m_wndTree);
	m_wndSplitter.SetColumnInfo(0, 200, 10);

	return 0;
}

void CAnchorPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CAnchorPane::SetEditTarget(CAnchorSet* pAnchor)
{
	m_pAnchor = pAnchor;
	UpdateTree();
}

void CAnchorPane::UpdateTree()
{
	//m_wndTree.DeleteAllItems();

	//for(int i = 0; i < (int) m_pAnchor->m_AnchorInfo.size(); i++)
	//{
	//	CAnchorSet::ANCHOR_INFO& ai = m_pAnchor->m_AnchorInfo[i];
	//	CString strDsp;
	//	strDsp.Format("绑定点 %d", ai.id);
	//	m_wndTree.InsertItem(strDsp.GetBuffer(), 0, 0);
	//}
	UpdateAnchor();
}

void CAnchorPane::UpdateCurrentItem()
{
	HTREEITEM hCurr = m_wndTree.GetSelectedItem();

	if(hCurr != NULL && 
		(m_wndTree.GetParentItem(hCurr) == TVI_ROOT || m_wndTree.GetParentItem( hCurr) == NULL))
	{
		TVITEM item;
		item.hItem = hCurr;
		item.mask = TVIF_PARAM;
		m_wndTree.GetItem(&item);
		int anchorid = int(item.lParam);
		CreateAnchorProp(anchorid);
		UpdataAnchorProp(anchorid);
	}
}

void CAnchorPane::UpdateAnchor()
{
	if(GetSafeHwnd() == NULL)
		return ;
	m_wndTree.DeleteAllItems();
	if(m_pAnchor == NULL)
		return;
	for(int i = 0; i < (int) m_pAnchor->m_AnchorInfo.size(); i++)
	{
		CAnchorSet::ANCHOR_INFO& ai = m_pAnchor->m_AnchorInfo[i];
		CString strDsp;
		strDsp.Format("绑定点 %d", ai.id);
		HTREEITEM hItem = m_wndTree.InsertItem(strDsp.GetBuffer(), 0, 0);
		m_wndTree.SetItem(hItem, TVIF_PARAM, 0, 0, 0, 0, 0, (LPARAM)ai.id);
		UpdateEffects(ai.id, hItem);
	}
}

void CAnchorPane::UpdateEffects(int anchor, HTREEITEM hItem)
{
	CEffectMgr* pEffectMgr = CEffectMgr::GetInstance();
	for(int i = 0; i <(int) pEffectMgr->m_Effects.size(); i++)
	{
		if(pEffectMgr->m_Effects[i]->GetBindInfo()->boneid == anchor)
		{
			CString strDsp = pEffectMgr->m_Effects[i]->GetName();
			HTREEITEM hEffectItem = m_wndTree.InsertItem(strDsp.GetBuffer(), 0, 0, hItem);
			m_wndTree.SetItem(hEffectItem, TVIF_PARAM, 0, 0, 0, 0, 0, (LPARAM) i );
		}
	}
}

void CAnchorPane::OnAchortreeSelchange(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_wndPropList.EndEditItem(TRUE);
	//if(m_bPropEdited)
	//{
	//	SaveCurrentItem(pNm->itemOld.hItem);
	//	PostMainFrameMsg(EEM_APPLY);
	//}
	//UpdateCurrentItem();

	if(m_wndTree.GetParentItem( pNm->itemNew.hItem) == TVI_ROOT ||
		m_wndTree.GetParentItem( pNm->itemNew.hItem) == NULL)
	{
		TVITEM item;
		item.hItem = pNm->itemNew.hItem;
		item.mask = TVIF_PARAM;
		m_wndTree.GetItem(&item);
		int anchorid = int(item.lParam);
		CreateAnchorProp(anchorid);
		UpdataAnchorProp(anchorid);
	}
}

void CAnchorPane::CreateAnchorProp(int anchorid)
{
	m_pPropId = new CNotifiedProp(_T("绑定点ID"), _variant_t(int(0) ));
	m_pPropBone = new CNotifiedProp(_T("所在骨骼"), _T(""));
	m_pPropOffset = new CNotifiedProp(_T("偏移量"), _T(""));

	std::vector<std::string> names;
	CEffectMgr::GetInstance()->GetSkeleton()->GetAllDisplayName(names);
	for(int i = 0; i < (int) names.size(); i++)
	{
		m_pPropBone->AddOption(names[i].data());
	}

	m_wndPropList.RemoveAll();
	m_wndPropList.AddProperty(m_pPropId);
	m_wndPropList.AddProperty(m_pPropBone);
	m_wndPropList.AddProperty(m_pPropOffset);

}
void CAnchorPane::UpdataAnchorProp(int anchorid)
{
	CCvtHelper cvt;
	CAnchorSet::ANCHOR_INFO* pAi = m_pAnchor->FindAnchor(anchorid);
	if(pAi == NULL)
		return ;

	m_pPropId->SetValue(int(pAi->id));
	std::string strDspBone = CEffectMgr::GetInstance()->GetSkeleton()->GetDisplayName(pAi->boneid);
	m_pPropBone->SetValue(strDspBone.data());
	//m_pPropOffset->SetValue(cvt.V3ToString(pAi->offset));
}