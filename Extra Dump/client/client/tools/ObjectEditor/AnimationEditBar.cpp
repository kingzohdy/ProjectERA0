
#include "stdafx.h"
#include "AnimationEditBar.h"
#include "EffectManager.h"
#include "SplitterGridView.h"
#include "NotifyProperty.h"
#include "SkeletonInfo.h"
#include "ObjectTypeHelper.h"
#include "AnimFilesDlg.h"

#include "OgreModel.h"
#include "OgreEntity.h"

IMPLEMENT_DYNAMIC(CAnimationEditPane, CWnd)

CAnimationEditPane::CAnimationEditPane() : m_bPropEdited(FALSE)
{
}

CAnimationEditPane::~CAnimationEditPane()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAnimationEditPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_ANIMPLAY_SINGLE, OnBtnPlayanim)
	ON_COMMAND(ID_BUTTON_ANIM_STOP, OnBtnStopanim)
	ON_COMMAND(ID_BUTTON_ANIM_PAUSE, OnBtnPauseanim)
	ON_COMMAND(ID_BUTTON_ANIM_UP, OnMoveUp)
	ON_COMMAND(ID_BUTTON_ANIM_DOWN, OnMoveDown)

	ON_UPDATE_COMMAND_UI(ID_BUTTON_ANIM_PAUSE, OnUpdateBtnPauseanim)

	ON_NOTIFY(LVN_ITEMCHANGED, ID_ANMINLIST, OnItemChanged)
	ON_NOTIFY(NM_DBLCLK, ID_ANMINLIST, OnItemDblClick)
	ON_MESSAGE(EEN_PROP_END_EDIT, OnPropEndEdit)
	//ON_COMMAND(ID_BTN_ANIM_OPEN, OnOpenAnim)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_COMMAND(ID_BUTTON_ANIMPLAY_SELECTED, OnPlayMulity)
	ON_COMMAND(ID_BUTTON_ANIMPLAY_ADD, OnPlayAdd)
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTON_ANIM_REMOVE, OnRemoveAnim)
	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CAnimationEditPane message handlers


int CAnimationEditPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//工具条
	m_wndToolBar.Create(this);
	//UINT btnid[] = {IDB_BTN_PLAYANIM, IDB_BTN_STOPANIM, IDB_EDIT_CURRANIM};
	//m_wndToolBar.SetButtons(btnid, 3);
	//CBCGPToolbarEditBoxButton editBtn(IDB_EDIT_CURRANIM, 0); ;
	//m_wndToolBar.ReplaceButton(IDB_EDIT_CURRANIM, editBtn);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_ANIM);
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
	//CSplitterGridView* pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	//CSplitterGridView* pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	//创建PropList
	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_ANIMPROP);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);

	m_pPropAnimName = new CNotifiedProp(_T("动画名称"), _T(""));
	m_pPropLayer = new CNotifiedProp(_T("优先级"),_variant_t((int)0));
	//m_pPropSeqs = new CBCGPProp(_T("所有序列"));
	//m_pPropTriggers = new CBCGPProp(_T("所有触发器"));

	CBCGPProp* pPropRoot = new CBCGPProp(_T("动画"));
	pPropRoot->AddSubItem(m_pPropAnimName);
	pPropRoot->AddSubItem(m_pPropLayer);
	//pPropRoot->AddSubItem(m_pPropSeqs);
	//pPropRoot->AddSubItem(m_pPropTriggers);

	m_wndPropList.AddProperty(pPropRoot);
	m_wndPropList.EnableHeaderCtrl(FALSE);


	//列表框
	m_wndAnimList.Create(WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS, CRect(0, 0, 100, 100), m_pGrid1, ID_ANMINLIST);
	m_wndAnimList.SetExtendedStyle(m_wndAnimList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_wndAnimList.InsertColumn(0, "动画ID", LVCFMT_LEFT, 200);
	m_wndAnimList.InsertColumn(1, "所在组", LVCFMT_LEFT, 200);
	m_pGrid1->SetInnerCtrl(&m_wndAnimList);


	m_wndSplitter.SetColumnInfo(0, 200, 10);
	m_wndSplitter.ShowWindow(SW_SHOW);
	m_dropTarget.Register(&m_wndAnimList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());
	//UpdateCurrentItem(0);
	return 0;
}

void CAnimationEditPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	//GetRect(frameRect);
	this->GetWindowRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.Width(), frameRect.Height() - 24 , SWP_NOZORDER);


	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

LRESULT CAnimationEditPane::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	SaveCurrentItem();
	return 0;
}


//播放按钮
void CAnimationEditPane::OnBtnPlayanim()
{
	int iItem = m_wndAnimList.GetSelectionMark();
	if(iItem < 0)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentItem();

	LVITEM item;
	item.mask = LVIF_PARAM | TVIF_HANDLE;
	item.iItem = iItem;
	m_wndAnimList.GetItem(&item);

	CEffectMgr::GetInstance()->PlayAnim((int)item.lParam);
}

void CAnimationEditPane::OnPlayMulity()
{/*
	Ogre::AnimPlayInfo animPlay;
	animPlay.nNumSequences = 0;

	for(int i = 0; i < m_wndAnimList.GetItemCount(); i++)
	{
		if(m_wndAnimList.GetCheck(i) != TRUE)
			continue;

		if(animPlay.nNumSequences < 4)
		{
			LVITEM item;
			item.mask = LVIF_PARAM | TVIF_HANDLE;
			item.iItem = i;
			m_wndAnimList.GetItem(&item);
			int id = (int)item.lParam;
			int priority = i;
			animPlay.nPriority[animPlay.nNumSequences] = priority;
			animPlay.nSequence[animPlay.nNumSequences] = id;
			animPlay.nNumSequences++;
		}
		else
		{
			m_wndAnimList.SetCheck(i, FALSE);
		}
	}
	CEffectMgr::GetInstance()->GetModelActor()->playAnim(animPlay);
	*/
}

//播放叠加动画
void CAnimationEditPane::OnPlayAdd()
{
	int iItem = m_wndAnimList.GetSelectionMark();
	if(iItem < 0)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentItem();

	LVITEM item;
	item.mask = LVIF_PARAM;
	item.iItem = iItem;
	m_wndAnimList.GetItem(&item);
	int seqid = (int)item.lParam;
	int priority = iItem;
	CEffectMgr::GetInstance()->GetModelActor()->playAnim(seqid);

	if(m_wndAnimList.GetCheck(iItem) == FALSE)
		m_wndAnimList.SetCheck(iItem, TRUE);
}

void CAnimationEditPane::OnUpdateBtnPauseanim(CCmdUI* pUI)
{
	if(CEffectMgr::GetInstance())
	{
		bool bPause = CEffectMgr::GetInstance()->m_bPause;
		pUI->SetCheck(bPause ? 1 : 0);
	}
}

//暂停按钮
void CAnimationEditPane::OnBtnPauseanim()
{
	bool bPause = CEffectMgr::GetInstance()->m_bPause;
	CEffectMgr::GetInstance()->PauseAnim(!bPause);
}

//停止按钮
void CAnimationEditPane::OnBtnStopanim()
{
	CEffectMgr::GetInstance()->StopAnim(0);
}

void CAnimationEditPane::OnOpenAnim()
{

}

//上移优先级
void CAnimationEditPane::OnMoveUp()
{
	int iSel = m_wndAnimList.GetSelectionMark();

	if(iSel <= 0 || m_wndAnimList.GetItemCount() <= 1)
		return;

	ANIMINFO tmp;
	tmp = m_targetSkeleton->m_animOrder[iSel - 1];
	m_targetSkeleton->m_animOrder[iSel - 1] = m_targetSkeleton->m_animOrder[iSel];
	m_targetSkeleton->m_animOrder[iSel] = tmp;
	UpdateList();
	m_wndAnimList.SetSelectionMark(iSel - 1);

	UpdateCurrentItem(iSel - 1);
}


//下移优先级
void CAnimationEditPane::OnMoveDown()
{
	int iSel = m_wndAnimList.GetSelectionMark();

	if(iSel == m_wndAnimList.GetItemCount() - 1 || iSel == -1 || m_wndAnimList.GetItemCount() <= 1)
		return;

	ANIMINFO tmp;
	tmp = m_targetSkeleton->m_animOrder[iSel + 1];
	m_targetSkeleton->m_animOrder[iSel + 1] = m_targetSkeleton->m_animOrder[iSel];
	m_targetSkeleton->m_animOrder[iSel] = tmp;
	UpdateList();
	m_wndAnimList.SetSelectionMark(iSel + 1);
	UpdateCurrentItem(iSel + 1);
}


LRESULT CAnimationEditPane::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	if(pdn->hWnd == m_wndAnimList.GetSafeHwnd())
	{
		char* tmp = new char[256];
		strcpy(tmp, (const char*)pdn->strChunk);
		CEffectMgr::GetInstance()->AttachAnim(tmp);
		UpdateList();
		m_wndPropList.RemoveAll();
	}
	return 0;
}

void CAnimationEditPane::UpdateList()
{
	//m_wndAnimList.DeleteAllItems();
	//int i = 0;
	//for(std::list<Ogre::CoreSkeleton::stAnimRes>::iterator it = m_targetSkeleton->m_pSkeleton->m_BoneAnims.begin();
	//	it != m_targetSkeleton->m_pSkeleton->m_BoneAnims.end(); 
	//	it++, i++)
	//{
	//	if(it->panim->m_Name != "" && it->panim->m_Name != NULL)
	//		m_wndAnimList.InsertItem(i, _T((const char*)(it->panim->m_Name)));
	//	else
	//		m_wndAnimList.InsertItem(i, _T("<未命名动画>"));
	//}

	CCvtHelper cvt;

	//更新之前清楚列表框的内容
	m_wndAnimList.DeleteAllItems();
	int i = 0;
	if(m_targetSkeleton->m_pSkeleton == NULL)
		return ;
	//for(std::list<Ogre::CoreSkeleton::stAnimRes>::iterator it = m_targetSkeleton->m_pSkeleton->m_BoneAnims.begin();
	//	it != m_targetSkeleton->m_pSkeleton->m_BoneAnims.end(); 
	//	it++)
	//{
	//	CString strName;
	//	if(it->panim->m_Name != "" && it->panim->m_Name != NULL)
	//		strName = _T((const char*)(it->panim->m_Name));
	//	else
	//		strName = _T("<未命名动画>");

	//	for(int j = 0; j < (int)it->panim->m_Sequences.size(); j++)
	//	{
	//		CString strSeqName;
	//		strSeqName = cvt.AnimIDToDesc(it->panim->m_Sequences[j].id);
	//		m_wndAnimList.InsertItem(LVIF_PARAM | LVIF_TEXT, i,
	//			strSeqName.GetBuffer(), 0, 0, 0,(LPARAM) &it->panim->m_Sequences[j]);
	//		m_wndAnimList.SetItem(i, 1, LVIF_TEXT, strName.GetBuffer(), 0, 0, 0, 0);
	//	}
	//}

	for(int i = 0; i < (int)m_targetSkeleton->m_animOrder.size(); i++)
	{
		int group = m_targetSkeleton->m_animOrder[i].iAnimGroup;
		int seqid = m_targetSkeleton->m_animOrder[i].iSeqId;
		int seqindex = m_targetSkeleton->m_animOrder[i].iSeqIndex;

		CString strSeqName;
		strSeqName = cvt.AnimIDToDesc(seqid);

		CString strName;

		if(group != -1)
		{
			Ogre::AnimationData *panim = m_targetSkeleton->m_pModelData->getAnimation(group);
			strName = panim->getResPath();

			m_wndAnimList.InsertItem(LVIF_PARAM | LVIF_TEXT, i, strSeqName.GetBuffer(), 0, 0, 0,seqid);
		}
		else
		{
			strName = "组合动画";
			m_wndAnimList.InsertItem(LVIF_PARAM | LVIF_TEXT, i,	strSeqName.GetBuffer(), 0, 0, 0, seqid);
		}
		m_wndAnimList.SetItem(i, 1, LVIF_TEXT, strName.GetBuffer(), 0, 0, 0, 0);
	}
}

void CAnimationEditPane::SetEditTarget(CSkeleton* pSkeleton) 
{ 
	m_targetSkeleton = pSkeleton; 
	UpdateList();
}

void CAnimationEditPane::OnItemChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMLISTVIEW* pNm = (NMLISTVIEW*) pHdr;

	if((pNm->uOldState & LVIS_SELECTED) == 0 && (pNm->uNewState & LVIS_SELECTED) != 0)
	{
		m_wndPropList.EndEditItem(TRUE);
		//if(pNm->itemOld.hItem != NULL)
		//	SaveCurrentItem(pNm->itemOld.hItem);
		if(m_bPropEdited)
			PostMainFrameMsg(EEM_APPLY);

		UpdateCurrentItem(pNm->iItem);
	}
}

 
// 双击一个Item 播放动画
void CAnimationEditPane::OnItemDblClick(NMHDR* pHdr, LRESULT* pResult)
{
	OnBtnPlayanim();
}

// Item被激活 更新Item
void CAnimationEditPane::OnItemActived(NMHDR* pHdr, LRESULT* pResult)
{
	NMLISTVIEW* pNm = (NMLISTVIEW*) pHdr;
	UpdateCurrentItem();
}


// 更新当前选中的Item
void CAnimationEditPane::UpdateCurrentItem(int theItem)
{
	m_bPropEdited = FALSE;
	if(m_targetSkeleton == NULL)
		return;

	int iSel = theItem;
	if(iSel < 0)
	{
		iSel = m_wndAnimList.GetSelectionMark();
		if(iSel < 0)
			return;
	}

	CCvtHelper cvt;
	//if(hFather == TVI_ROOT || hFather == NULL)
	//{	//选择的是动画
	//	TVITEM item;
	//	item.mask = TVIF_PARAM | TVIF_HANDLE;
	//	item.hItem = hItem;
	//	m_wndAnimList.GetItem(&item);
	//	Ogre::SkeletonAnim* panim = (Ogre::SkeletonAnim*)item.lParam;

	//	CString strName;
	//	if(panim->m_Name != "" && panim->m_Name != NULL)
	//		strName = _T((const char*)(panim->m_Name));
	//	else
	//		strName = _T("<未命名动画>");

	//	m_wndPropList.RemoveAll();
	//	m_pPropAnimName = new CNotifiedProp(_T("动画名称"), strName.GetBuffer());
	//	m_pPropLayer = new CNotifiedProp(_T("优先级"),_variant_t(panim->m_Layer));
	//	
	//	m_pPropAnimName->SetNotifyTarget(GetSafeHwnd());
	//	m_pPropLayer->SetNotifyTarget(GetSafeHwnd());

	//	CBCGPProp* pPropRoot = new CBCGPProp(_T("动画"));
	//	m_wndPropList.AddProperty(pPropRoot);
	//	pPropRoot->AddSubItem(m_pPropAnimName);
	//	pPropRoot->AddSubItem(m_pPropLayer);

	//	pPropRoot->Expand();
	//	m_wndPropList.EnableHeaderCtrl(FALSE);
	//}
	//else
	//{	//选择的是序列
	m_wndPropList.RemoveAll();
	LVITEM item;
	item.mask = LVIF_PARAM;
	item.iItem = iSel;
	m_wndAnimList.GetItem(&item);

	Ogre::SequenceDesc* seq = CEffectMgr::GetInstance()->GetSkeleton()->GetSequence((int)item.lParam);

	if(seq == NULL)
		return;

	CString strTitle;
	strTitle.Format(_T("序列 %d"), seq->id);

	CBCGPProp* pPropTitle = new CBCGPProp(strTitle);
	m_pPropID = new CNotifiedProp(_T("序列ID"), _variant_t((int)seq->id));
	m_pPropTimeStart = new CNotifiedProp(_T("开始时间"), _variant_t((int)seq->time_start));
	m_pPropTimeEnd = new CNotifiedProp(_T("结束时间"), _variant_t((int)seq->time_end));
	CString strLoopmod = cvt.LoopmodToString(seq->loopmode);
	m_pPropType = new CNotifiedProp(_T("循环模式"), _variant_t(strLoopmod.GetBuffer() ));
	m_pPropType->AddOption(_T("循环播放"));
	m_pPropType->AddOption(_T("单次播放"));
	m_pPropType->AddOption(_T("单次播放后停止"));

	m_pPropID->SetNotifyTarget(GetSafeHwnd());
	m_pPropTimeStart->SetNotifyTarget(GetSafeHwnd());
	m_pPropTimeEnd->SetNotifyTarget(GetSafeHwnd());

	pPropTitle->AddSubItem(m_pPropID);
	pPropTitle->AddSubItem(m_pPropTimeStart);
	pPropTitle->AddSubItem(m_pPropTimeEnd);
	pPropTitle->AddSubItem(m_pPropType);
	m_wndPropList.AddProperty(pPropTitle);
	pPropTitle->Expand();
	//}
}



void CAnimationEditPane::SaveCurrentItem()
{
	USES_CONVERSION;
	m_bPropEdited = FALSE;
	if(m_targetSkeleton == NULL)
		return;
	int iItem = m_wndAnimList.GetSelectionMark();
	if(iItem < 0)
		return;
	CCvtHelper cvt;
	//HTREEITEM hFather = m_wndAnimList.GetParentItem(hItem);
	//if(hItem == TVI_ROOT || hItem == NULL)
	//	return;
	//if(hFather == TVI_ROOT || hFather == NULL)
	//{	//选择的是动画
	//	TVITEM item;
	//	item.mask = TVIF_PARAM | TVIF_HANDLE;
	//	item.hItem = hItem;
	//	m_wndAnimList.GetItem(&item);
	//	Ogre::SkeletonAnim* panim = (Ogre::SkeletonAnim*)item.lParam;

	//	CString strTitle = OLE2T((_bstr_t)m_pPropAnimName->GetValue());
	//	CString strName = strTitle;
	//	if(strName == _T("<未命名动画>"))
	//		strName = _T("");
	//	panim->m_Name = strName.GetBuffer();
	//	
	//	if(strTitle == _T(""))
	//		strTitle = _T("<未命名动画>");
	//	m_wndAnimList.SetItem(hItem, TVIF_TEXT, strTitle.GetBuffer(), 0, 0, 0, 0, 0);

	//	panim->m_Layer = (int)m_pPropLayer->GetValue();

	//}
	//else
	//{	//选择的是序列
	LVITEM item;
	item.mask = LVIF_PARAM ;
	item.iItem = iItem;
	m_wndAnimList.GetItem(&item);
	Ogre::SequenceDesc *seq = CEffectMgr::GetInstance()->GetSkeleton()->GetSequence((int)item.lParam);
	if(seq == NULL)
		return;

	seq->time_start = (int)m_pPropTimeStart->GetValue();
	seq->time_end = (int)m_pPropTimeEnd->GetValue();

	CString strSelText = OLE2T((_bstr_t)m_pPropType->GetValue());
	seq->loopmode = (Ogre::AnimPlayMode)cvt.StringToLoopmod(strSelText);
	//}
}

LRESULT CAnimationEditPane::OnPropEndEdit(WPARAM w, LPARAM l)
{
	PROPNM* pNm = (PROPNM*)l;
	CNotifiedProp* pProp = static_cast<CNotifiedProp*>(pNm->pProp);
	m_bPropEdited = TRUE;
	return  0;
}

void CAnimationEditPane::OnDestroy()
{
	CWnd::OnDestroy();

	delete m_pGrid1;
	delete m_pGrid2;
}

void CAnimationEditPane::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CAnimationEditPane::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_SPACE)
		{
			OnBtnPlayanim();	
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}


// 移除按钮
void CAnimationEditPane::OnRemoveAnim()
{
	CAnimFilesDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CEffectMgr::GetInstance()->m_skeleton->RemoveAnims(dlg.m_strSel.GetBuffer());
		UpdateList();
	}
}