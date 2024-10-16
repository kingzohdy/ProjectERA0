
#include "stdafx.h"
#include "BoneEditBar.h"
#include "SplitterGridView.h"
#include "SkeletonInfo.h"
#include "EffectManager.h"
#include "AnchorSet.h"
#include "EffectUnit.h"
#include "BindInfo.h"
#include "NotifyProperty.h"
#include "EditorManager.h"

#include "OgreSkeletonData.h"

IMPLEMENT_DYNAMIC(CBoneEditPane, CWnd)
CBoneEditPane::CBoneEditPane()
{
}

CBoneEditPane::~CBoneEditPane()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CBoneEditPane, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTON_APPLY_BONE, OnBtnApplyBone)
	ON_COMMAND(ID_BUTTON_SHOW_BONE, OnBtnShowBone)
	ON_COMMAND(ID_BUTTON_UPDATE_ANCHOR, OnBtnUpdateAnchor)

	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_BONE, OnUpdateBtnShowbone)

	ON_NOTIFY(TVN_SELCHANGED, ID_BONETREE, OnBonetreeSelchange)

	ON_MESSAGE(EEN_PROP_END_EDIT, OnPropEndEdit)
END_MESSAGE_MAP()



// CBoneEditPane message handlers


void CBoneEditPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	CBrush b(RGB(255, 255, 255));
	CRect rect;
	GetClientRect(rect);
	dc.FillRect(rect, &b);
	dc.TextOut(0, 0, CString(_T("还没有实现。。。")));
}

int CBoneEditPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.Create(this);
	//UINT btnid[] = {IDB_BTN_PLAYANIM, IDB_BTN_STOPANIM, IDB_EDIT_CURRANIM};
	//m_wndToolBar.SetButtons(btnid, 3);
	//CBCGPToolbarEditBoxButton editBtn(IDB_EDIT_CURRANIM, 0); ;
	//m_wndToolBar.ReplaceButton(IDB_EDIT_CURRANIM, editBtn);

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

	//分割窗口
	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context1);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 1);

	//创建PropList
	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_BONEPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);

	//创建树形控件
	m_wndBoneTree.Create(WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, CRect(0, 0, 100, 100), m_pGrid1, ID_BONETREE);
	m_hBone = m_wndBoneTree.InsertItem(_T("骨骼"), 0, 0);
	m_hAnchor = m_wndBoneTree.InsertItem(_T("绑定点"), 0, 0);

	m_pGrid1->SetInnerCtrl(&m_wndBoneTree);
	m_wndSplitter.SetColumnInfo(0, 200, 10);
	m_bPropEdited = FALSE;
	return 0;
}

void CBoneEditPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CBoneEditPane::SetEditTarget(CSkeleton* pTarget)
{
	m_pSkeleton = pTarget;


	UpdateTree();
}

void CBoneEditPane::UpdateTree()
{
	if(m_pSkeleton->m_pSkeleton == NULL)
		return;
	m_bPropEdited = FALSE;
	m_wndBoneTree.DeleteAllItems();
	m_hBone = m_wndBoneTree.InsertItem(_T("骨骼"), 0, 0);
	m_hAnchor = m_wndBoneTree.InsertItem(_T("绑定点"), 0, 0);

	std::vector<std::string> boneNames;
	m_pSkeleton->GetAllDisplayName(boneNames);
	for(size_t i=0; i<m_pSkeleton->m_pSkeleton->getNumBone(); i++)
	{
		Ogre::BoneData* pBone = m_pSkeleton->m_pSkeleton->getIthBone(i);
		HTREEITEM hFather = FindItem(m_hBone, (LPARAM)pBone->m_FatherID);
		if(hFather == NULL)
			hFather = m_hBone;

		CString strName = m_pSkeleton->GetDisplayName(pBone->m_ID).c_str();
		HTREEITEM hNewItem = m_wndBoneTree.InsertItem(TVIF_TEXT | TVIF_PARAM,strName, 0, 0, 0, 0, (LPARAM)pBone,  hFather, TVI_LAST);
	}

	CAnchorSet* pAnchor = CEffectMgr::GetInstance()->m_anchorInfo;
	for(int i = 0; i < (int) pAnchor->m_AnchorInfo.size(); i++)
	{
		CAnchorSet::ANCHOR_INFO& ai = pAnchor->m_AnchorInfo[i];
		CString strDsp;
		strDsp.Format("绑定点 %d", ai.id);
		HTREEITEM hItem = m_wndBoneTree.InsertItem(strDsp.GetBuffer(), 0, 0, m_hAnchor);
		m_wndBoneTree.SetItem(hItem, TVIF_PARAM, 0, 0, 0, 0, 0, (LPARAM)ai.id);
		UpdateEffects(ai.id, hItem);
	}
}

void CBoneEditPane::UpdateEffects(int anchor, HTREEITEM hItem)
{
	CEffectMgr* pEffectMgr = CEffectMgr::GetInstance();
	for(int i = 0; i <(int) pEffectMgr->m_Effects.size(); i++)
	{
		CEffectUnit* pEfffect = pEffectMgr->m_Effects[i];
		if(pEfffect->GetBindInfo()->boneid == anchor && pEfffect->GetBindInfo()->bDynamic == false)
		{
			CString strDsp = pEfffect->GetName();
			HTREEITEM hEffectItem = m_wndBoneTree.InsertItem(strDsp.GetBuffer(), 0, 0, hItem);
			m_wndBoneTree.SetItem(hEffectItem, TVIF_PARAM, 0, 0, 0, 0, 0, (LPARAM) i );
		}
	}
}

void CBoneEditPane::UpdateCurrentItem()
{
	m_bPropEdited = FALSE;
	m_wndPropList.RemoveAll();

	TVITEM item;
	item.mask = TVIF_HANDLE | TVIF_PARAM;
	item.hItem = m_wndBoneTree.GetSelectedItem();
	m_wndBoneTree.GetItem(&item);

	if(item.hItem == m_hAnchor || item.hItem == m_hBone)
		return;

	if(IsBone(item.hItem))
	{
		Ogre::BoneData* pBone = (Ogre::BoneData*) item.lParam;

		CString strName = (const char*)pBone->m_Name;
		if(strName == "")
			strName = _T("<未命名骨骼>");
		CString strTitle = strName;
		strTitle.AppendFormat(_T("(%d)"), pBone->m_ID);

		CNotifiedProp* pRoot = new CNotifiedProp(strTitle);
		m_pPropID = new CNotifiedProp(_T("骨骼 ID"), _variant_t((int)pBone->m_ID));
		m_pPropFatherID = new CNotifiedProp(_T("父骨骼 ID"), _variant_t((int)pBone->m_FatherID));
		m_pPropName = new CNotifiedProp(_T("骨骼名称"), _variant_t(strName.GetBuffer()));

		m_pPropID->SetNotifyTarget(GetSafeHwnd());
		m_pPropFatherID->SetNotifyTarget(GetSafeHwnd());
		m_pPropName->SetNotifyTarget(GetSafeHwnd());

		m_pPropFatherID->Enable(FALSE);
		m_pPropID->Enable(FALSE);

		pRoot->AddSubItem(m_pPropID);
		pRoot->AddSubItem(m_pPropFatherID);
		pRoot->AddSubItem(m_pPropName);
		m_wndPropList.AddProperty(pRoot);
		CEnginAgent::GetInstance()->SetHightLightPos(pBone->m_ID, true);
	}

}

bool CBoneEditPane::IsBone(HTREEITEM hItem)
{
	HTREEITEM hTmp = hItem;
	while(hTmp != TVI_ROOT && hTmp != NULL)
	{
		if(hTmp == m_hAnchor)
			return false;
		if(hTmp == m_hBone)
			return true;

		hTmp = m_wndBoneTree.GetParentItem(hTmp);
	}
	return false;
}

HTREEITEM CBoneEditPane::FindItem(HTREEITEM hParentItem, LPARAM id)
{
	if(hParentItem == NULL)
		return NULL;
	if (hParentItem != m_hBone)
	{
		TVITEM item;
		item.mask = TVIF_HANDLE|TVIF_PARAM;
		item.hItem = hParentItem;

		m_wndBoneTree.GetItem(&item);
		Ogre::BoneData* pBone = (Ogre::BoneData*) item.lParam;

		if(hParentItem != m_hBone && pBone->m_ID == id)
		{
			return hParentItem;
		}
	}

	HTREEITEM hRet = NULL;
	HTREEITEM hChild = m_wndBoneTree.GetChildItem(hParentItem);
	while(hChild != NULL)
	{
		hRet = FindItem(hChild, id);
		if(hRet != NULL)
			return hRet;
		hChild = m_wndBoneTree.GetNextSiblingItem(hChild);
	}
	return NULL;
}

void CBoneEditPane::OnDestroy()
{
	CWnd::OnDestroy();

	delete m_pGrid1;
	delete m_pGrid2;
}


void CBoneEditPane::OnBonetreeSelchange(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_wndPropList.EndEditItem(TRUE);
	//if(m_bPropEdited)
	//{
	//	SaveCurrentItem(pNm->itemOld.hItem);
	//	PostMainFrameMsg(EEM_APPLY);
	//}
	UpdateCurrentItem();
}

void CBoneEditPane::SaveCurrentItem(HTREEITEM itemOld)
{
	//if(itemOld == NULL || itemOld == TVI_ROOT)
	//	return;
	//USES_CONVERSION;
	//TVITEM item;
	//item.mask = TVIF_HANDLE | TVIF_PARAM;
	//item.hItem = itemOld;
	//m_wndBoneTree.GetItem(&item);
	//Ogre::BoneData* pBone = (Ogre::BoneData*)item.lParam;

	////Update ID and Name
	//pBone->m_ID = (int)m_pPropID->GetValue();

	//CString strTitle = OLE2T((_bstr_t)m_pPropName->GetValue());
	//CString strName = strTitle;
	//if(strTitle == _T("<未命名骨骼>"))
	//	strName = _T("");
	//pBone->m_Name = strName.GetBuffer();

	//if(strTitle == _T(""))
	//	strTitle = _T("<未命名骨骼>");
	//strTitle.AppendFormat("(%d)", pBone->m_ID);
	//m_wndBoneTree.SetItem(itemOld, TVIF_TEXT, strTitle.GetBuffer(), 0, 0, 0, 0, 0);
}

LRESULT CBoneEditPane::OnPropEndEdit(WPARAM w, LPARAM l)
{
	//PROPNM* pNm = (PROPNM*)l;
	//CNotifiedProp* pProp = pNm->pProp;
	//m_bPropEdited = TRUE;

	//TVITEM item;
	//item.mask = TVIF_HANDLE | TVIF_PARAM;
	//item.hItem = m_wndBoneTree.GetSelectedItem();
	//m_wndBoneTree.GetItem(&item);
	//Ogre::BoneData* pBone = (Ogre::BoneData*)item.lParam;
	//
	//if(pProp == m_pPropID)
	//{
	//	int iNewID = (int)pProp->GetValue();
	//	if(iNewID != pBone->m_ID)
	//	{
	//		for(int i = 0; i < (int)m_pSkeleton->m_pSkeleton->m_BoneTable.size(); i++)
	//		{
	//			if(iNewID == m_pSkeleton->m_pSkeleton->m_BoneTable[i]->m_ID)
	//			{
	//				MessageBox(_T("ID与已存在的骨骼重复"), _T("错误"));
	//				pNm->bCancel = TRUE;
	//				return 0;
	//			}
	//		}
	//	}
	//}

	//else if(pProp == m_pPropFatherID)
	//{
	//	int iNewID = (int)pProp->GetValue();

	//	if(NULL != FindItem(item.hItem, iNewID))
	//	{
	//		MessageBox(_T("父ID不能是自己或者自己的子骨骼ID"), _T("错误"));
	//		pNm->bCancel = TRUE;
	//		return 0;
	//	}
	//}
	return 0;
}

void CBoneEditPane::OnBtnApplyBone()
{
	//if(m_bPropEdited)
	//{
	//m_wndPropList.EndEditItem(TRUE);
	//SaveCurrentItem(m_wndBoneTree.GetSelectedItem());
	//PostMainFrameMsg(EEM_APPLY);
	//}
	//UpdateCurrentItem();
}

void CBoneEditPane::OnBtnShowBone()
{
	CEnginAgent::GetInstance()->ShowBone(
		!CEnginAgent::GetInstance()->IsShowBone());
}

void CBoneEditPane::OnUpdateBtnShowbone(CCmdUI* pUI)
{
	if(CEnginAgent::GetInstance()->IsShowBone())
		pUI->SetCheck(1);
	else
		pUI->SetCheck(0);
}

void CBoneEditPane::OnBtnUpdateAnchor()
{
	CEffectMgr::GetInstance()->updateAnchors();
}
