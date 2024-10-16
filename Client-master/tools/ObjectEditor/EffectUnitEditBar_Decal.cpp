
#include "stdafx.h"
#include "EffectUnit_Decal.h"
#include "EffectUnitEditBar_Decal.h"
#include "EffectManager.h"
#include "SplitterGridView.h"
#include "NotifyProperty.h"
#include "ObjectTypeHelper.h"

IMPLEMENT_DYNAMIC(CDecalBar, CEffectEditBar)
CDecalBar::CDecalBar() : m_pTarget(NULL), m_bLockApp(true)
{
}

CDecalBar::~CDecalBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CDecalBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGEDA, ID_DECALTREE, OnTreeSelChanged)
	ON_NOTIFY(NM_DBLCLK, ID_DECALTREE, OnTreeDblClick)

	ON_COMMAND(ID_BTN_DCL_APPLY, OnApply)
	ON_COMMAND(ID_BTN_DCL_REMOVE, OnRemove)
	ON_COMMAND(ID_BTN_DCL_SAVE, OnSave)
	ON_UPDATE_COMMAND_UI(ID_BTN_DCL_VISIBLE, OnUpdateVisible)

	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)

	ON_COMMAND(ID_BTN_DCL_LOCKAPP, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_DCL_LOCKAPP, OnUpdateBtnLockApply)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


int CDecalBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this) 
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_DECAL))
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
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height () / 3 ),&context1);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height () / 3 * 2),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	CSplitterGridView* m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	CSplitterGridView* m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	m_wndTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS , 
		CRect(0, 0, 100, 100), m_pGrid1, ID_DECALTREE);
	m_hDesc = m_wndTree.InsertItem(_T("Ìù»¨²ÎÊý"));
	m_hKeyFrame = m_wndTree.InsertItem(_T("¹Ø¼üÖ¡"));


	m_pGrid1->SetInnerCtrl(&m_wndTree);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_DECALPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);

	//ChangePropList();
	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	return 0;
}

void CDecalBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}


BOOL CDecalBar::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call	the base class
	//SAFE_DELETE( m_pGrid1);
	//SAFE_DELETE (m_pGrid2);
	return CEffectEditBar::DestroyWindow();
}

void CDecalBar::OnTreeDblClick(NMHDR* pHdr, LRESULT* pResult)
{
	HTREEITEM item = m_wndTree.GetSelectedItem();
	if(item != NULL && 
		(item == m_hKeyFrame || m_wndTree.GetParentItem(item) == m_hKeyFrame))
	{
		UpdateKeyFrameWnd();
	}

}

LRESULT CDecalBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	if(m_hLastSel != NULL)
		SaveCurrentProp(m_hLastSel);
	else
		SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->UpdateSource();

	if(m_bLockApp && (pnm->pProp == (CNotifiedProp*)m_pPropTexture
		|| pnm->pProp == (CNotifiedProp*)m_pPropBindBone)
		|| pnm->pProp == (CNotifiedProp*)m_pPropSize
		|| pnm->pProp == (CNotifiedProp*)m_pPropAspect
		|| pnm->pProp == (CNotifiedProp*)m_pPropAlpha
		|| pnm->pProp == (CNotifiedProp*)m_pPropColor
		|| pnm->pProp == (CNotifiedProp*)m_pPropMaskTex
		|| pnm->pProp == (CNotifiedProp*)m_pPropBlendMode
		|| pnm->pProp == (CNotifiedProp*)m_pPropDistort
		|| pnm->pProp == (CNotifiedProp*)m_pPropTimeSlot
		|| pnm->pProp == (CNotifiedProp*)m_pPropTexRow
		|| pnm->pProp == (CNotifiedProp*)m_pPropTexCol
		)
	{
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}

	return 0;
}


void CDecalBar::OnDestroy()
{
	CEffectEditBar::OnDestroy();
}

void CDecalBar::OnNewFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	CDecal::DCL_FRAME item;
	if(m_pTarget->m_KeyFrames.size() > 0)
		item.lTick = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1].lTick + 1000;

	item.size = 20.0f;
	item.aspect = 1.0f;
	item.color = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	item.alpha = 1.0f;

	m_pTarget->m_KeyFrames.push_back(item);

	UpdateTree();
	m_pTarget->UpdateSource();
	UpdateKeyFrameWnd();
}

void CDecalBar::OnDeleteFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return ;

	CDecal::DCL_FRAME* pFrameItem = (CDecal::DCL_FRAME*)GetItemParam(hSelected);

	std::vector<CDecal::DCL_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
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

void CDecalBar::OnMoveUp()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return;
	CDecal::DCL_FRAME* pItem = (CDecal::DCL_FRAME* )GetItemParam(hSelected);
	std::vector<CDecal::DCL_FRAME>::iterator back = m_pTarget->m_KeyFrames.end();
	std::vector<CDecal::DCL_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(pItem == &(*it) && back != m_pTarget->m_KeyFrames.end())
		{
			CDecal::DCL_FRAME tmp = *it;
			m_pTarget->m_KeyFrames.erase(it);
			m_pTarget->m_KeyFrames.insert(back, tmp);
			break;
		}
		back = it;
	}
	UpdateTree();
}

void CDecalBar::OnMoveDown()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return;
	CDecal::DCL_FRAME* pItem = (CDecal::DCL_FRAME* )GetItemParam(hSelected);
	std::vector<CDecal::DCL_FRAME>::iterator back = m_pTarget->m_KeyFrames.end();
	std::vector<CDecal::DCL_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(back != m_pTarget->m_KeyFrames.end() && pItem == &(*back))
		{
			CDecal::DCL_FRAME tmp = *it;
			m_pTarget->m_KeyFrames.erase(it);
			m_pTarget->m_KeyFrames.insert(back, tmp);
			break;
		}
		back = it;
	}
	UpdateTree();
}

void CDecalBar::OnApply()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	//PostMainFrameMsg( EEM_APPLY);
	//PostMainFrameMsg(EEM_UPDATA_LIST);
	//m_pTarget->RefreshNode();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}

void CDecalBar::OnRemove()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CDecalBar::OnSave()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	SendMainFrameMsg( EEM_SAVE, 0, (LPARAM)m_pTarget);
}

void CDecalBar::OnVisible()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg( EEM_APPLY);
}

void CDecalBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->IsVisible() ? 1 : 0);
}

void CDecalBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CDecalBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CDecalBar::OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_hLastSel = pNm->itemOld.hItem;
	m_wndPropList.EndEditItem(TRUE);
	//SaveCurrentProp(pNm->itemOld.hItem);
	ChangePropList();
	m_hLastSel = NULL;


}

LRESULT CDecalBar::OnDropRes(WPARAM w, LPARAM l)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)l;
	if(pdn->hWnd == m_wndPropList.GetSafeHwnd())
	{
		if(!IsTextureResource(pdn->strChunk))
			return  0;
		if(m_wndTree.GetSelectedItem() == m_hDesc)
		{
			if(m_pPropTexture->GetRect().PtInRect(pdn->dropPos))
			{
				m_pPropTexture->SetValue(pdn->strChunk);
				PROPNM nm;
				nm.pProp = (CNotifiedProp*)m_pPropTexture;
				OnUpdatePropValue(0, (LPARAM)&nm);
			}
			else if(m_pPropMaskTex->GetRect().PtInRect(pdn->dropPos))
			{
				m_pPropMaskTex->SetValue(pdn->strChunk);
				PROPNM nm;
				nm.pProp = (CNotifiedProp*)m_pPropMaskTex;
				OnUpdatePropValue(0, (LPARAM)&nm);
			}
		}
	}
	return 0;
}

void CDecalBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CDecal*) pEffect;
	if(GetSafeHwnd())
	{
		SetWindowText(m_pTarget->m_strName.GetBuffer());
		UpdateTree();
		ChangePropList();
	}
}
CEffectUnit* CDecalBar::GetEditTarget()
{
	return m_pTarget;
}

LPARAM CDecalBar::GetItemParam(HTREEITEM hItem)
{
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;
	m_wndTree.GetItem(&item);
	return item.lParam;
}

void CDecalBar::UpdateTree()
{
	HTREEITEM hItem = m_wndTree.GetChildItem(m_hKeyFrame);
	HTREEITEM hTmp = NULL;
	m_wndTree.SelectItem(m_hDesc);
	while(hItem)
	{
		hTmp = hItem;
		hItem = m_wndTree.GetNextSiblingItem(hItem);
		m_wndTree.DeleteItem(hTmp);
	}

	for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
	{
		CString strName;
		strName.Format(_T("¹Ø¼üÖ¡ %d"), i + 1);
		m_wndTree.InsertItem(TVIF_TEXT | TVIF_PARAM,strName.GetBuffer(), 0, 0, 0,0,
			(LPARAM)&m_pTarget->m_KeyFrames[i], m_hKeyFrame, TVI_LAST);
	}
	m_wndTree.Expand(m_hKeyFrame, TVE_EXPAND);

}

void CDecalBar::ChangePropList()
{
	m_wndPropList.EndEditItem(TRUE);

	m_wndPropList.RemoveAll();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{

		DescUpdate(CREATE_PROP);

		m_wndPropList.AddProperty( m_pPropBlendMode );
		m_pPropBlendMode->SetNotifyTarget( GetSafeHwnd() );
		m_wndPropList.AddProperty(m_pPropDistort);
		m_pPropDistort->SetNotifyTarget(GetSafeHwnd());
		m_wndPropList.AddProperty(m_pPropTimeSlot);
		m_pPropTimeSlot->SetNotifyTarget(GetSafeHwnd());
		m_wndPropList.AddProperty(m_pPropTexRow);
		m_pPropTexRow->SetNotifyTarget(GetSafeHwnd());
		m_wndPropList.AddProperty(m_pPropTexCol);
		m_pPropTexCol->SetNotifyTarget(GetSafeHwnd());

		m_pPropBindBone->SetNotifyTarget( GetSafeHwnd() );
		m_pPropMaskTex->SetNotifyTarget( GetSafeHwnd() );
		m_pPropTexture->SetNotifyTarget( GetSafeHwnd() );

		CBCGPProp* pTexData = new CBCGPProp( _T("Ìù»¨ÌùÍ¼") );
		pTexData->AddSubItem( m_pPropTexture );		

		CBCGPProp* pMaskTex = new CBCGPProp( _T("ÃÉ°åÌùÍ¼") );
		pMaskTex->AddSubItem( m_pPropMaskTex );

		m_wndPropList.AddProperty( pTexData );
		m_wndPropList.AddProperty( pMaskTex );
		m_wndPropList.AddProperty( m_pPropBindBone );
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CDecal::DCL_FRAME* pFrameItem = (CDecal::DCL_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());

		KeyFrameUpdate(CREATE_PROP, pFrameItem);

		//((CNotifiedProp*)m_pPropColor)->SetNotifyTarget(GetSafeHwnd());

		m_pPropSize->SetNotifyTarget(GetSafeHwnd());
		m_pPropAspect->SetNotifyTarget(GetSafeHwnd());
		m_pPropColor->SetNotifyTarget(GetSafeHwnd());
		m_pPropAlpha->SetNotifyTarget(GetSafeHwnd());
		m_pPropTick->SetNotifyTarget(GetSafeHwnd());

		m_wndPropList.AddProperty(m_pPropSize);
		m_wndPropList.AddProperty(m_pPropAspect);
		m_wndPropList.AddProperty(m_pPropColor);
		m_wndPropList.AddProperty(m_pPropAlpha);
		m_wndPropList.AddProperty(m_pPropTick); 

		CBCGPProp* pTexData = new CBCGPProp( _T("Ìù»¨ÌùÍ¼") );

		m_pPropUScale->SetNotifyTarget( GetSafeHwnd() );
		m_pPropVScale->SetNotifyTarget( GetSafeHwnd() );
		m_pPropUOff->SetNotifyTarget( GetSafeHwnd() );
		m_pPropVOff->SetNotifyTarget( GetSafeHwnd() );
		m_pPropUVRotate->SetNotifyTarget( GetSafeHwnd() );

		pTexData->AddSubItem( m_pPropUScale );
		pTexData->AddSubItem( m_pPropVScale );
		pTexData->AddSubItem( m_pPropUOff );
		pTexData->AddSubItem( m_pPropVOff );
		pTexData->AddSubItem( m_pPropUVRotate );

		m_wndPropList.AddProperty( pTexData );

		CBCGPProp* pMaskTexData = new CBCGPProp( _T("ÃÉ°åÌùÍ¼") );

		m_pPropMaskUScale->SetNotifyTarget( GetSafeHwnd() );
		m_pPropMaskVScale->SetNotifyTarget( GetSafeHwnd() );
		m_pPropMaskUOff->SetNotifyTarget( GetSafeHwnd() );
		m_pPropMaskVOff->SetNotifyTarget( GetSafeHwnd() );
		m_pPropMaskUVRotate->SetNotifyTarget( GetSafeHwnd() );

		pMaskTexData->AddSubItem( m_pPropMaskUScale );
		pMaskTexData->AddSubItem( m_pPropMaskVScale );
		pMaskTexData->AddSubItem( m_pPropMaskUOff );
		pMaskTexData->AddSubItem( m_pPropMaskVOff );
		pMaskTexData->AddSubItem( m_pPropMaskUVRotate );

		m_wndPropList.AddProperty( pMaskTexData );

	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
		{
			CString strName;
			strName.Format(_T("¹Ø¼üÖ¡ %d"), i + 1);
			CBCGPProp* pProp = new CBCGPProp(strName, _T(""));
			//((CNotifiedProp*)pProp)->SetNotifyTarget(GetSafeHwnd());
			m_wndPropList.AddProperty(pProp);
		}
	}

	UpdateList();
}

void CDecalBar::SaveCurrentProp(HTREEITEM hOldItem)
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
		CDecal::DCL_FRAME* pFrameItem = (CDecal::DCL_FRAME*)GetItemParam(hOldItem);
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

void CDecalBar::UpdateList()
{
	CCvtHelper cvt;
	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		DescUpdate(DATA_TO_PROP);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CDecal::DCL_FRAME* pFrameItem = (CDecal::DCL_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		KeyFrameUpdate(DATA_TO_PROP, pFrameItem);
	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
			m_wndPropList.GetProperty(i)->SetValue(cvt.IntToString(m_pTarget->m_KeyFrames[i].lTick));
	}
	m_wndPropList.Invalidate();
}

void CDecalBar::DescUpdate(DATA_UPDATA_TYPE type)
{
	PropUpdate_blendtype((CBCGPProp**)&m_pPropBlendMode, (Ogre::UIBlendMode*)&m_pTarget->m_desc.m_BlendMode, type, _T("»ìºÏÄ£Ê½"));
	PropUpdate_bool((CBCGPProp**)&m_pPropDistort, &m_pTarget->m_desc.m_bDistort, type, "¿Õ¼äÅ¤ÇúÄ£Ê½");
	PropUpdate_int((CBCGPProp**)&m_pPropTexRow, &m_pTarget->m_desc.m_TexRows, type, _T("ÌùÍ¼ÐÐÊý"));
	PropUpdate_int((CBCGPProp**)&m_pPropTexCol, &m_pTarget->m_desc.m_TexCols, type, _T("ÌùÍ¼ÁÐÊý"));
	PropUpdate_float((CBCGPProp**)&m_pPropTimeSlot, &m_pTarget->m_desc.m_TimeSlot, type, _T("Ö¡¼ä¸ô"), 0.03f, 32767.0f, 0.0f);

	PropUpdate_cstring((CBCGPProp**)&m_pPropTexture, &m_pTarget->m_strTexture, type, _T("ÎÆÀíÌùÍ¼"));
	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("°ó¶¨µã"));

	PropUpdate_cstring((CBCGPProp**)&m_pPropMaskTex, &m_pTarget->m_strMaskTexture, type, _T("ÃÉ°åÌùÍ¼"));
}
void CDecalBar::KeyFrameUpdate(DATA_UPDATA_TYPE type, CDecal::DCL_FRAME* pItem)
{
	PropUpdate_float((CBCGPProp**)&m_pPropSize, &(pItem->size), type, _T("¸²¸Ç°ë¾¶"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropAspect, &(pItem->aspect), type, _T("×Ýºá±È"), 0.1f, 100.0f, 0.1f);
	PropUpdate_float((CBCGPProp**)&m_pPropAlpha, &(pItem->alpha), type, _T("Í¸Ã÷¶È"), 0.01f, 1.0f, 0);
	PropUpdate_color((CBCGPColorProp**)&m_pPropColor, &pItem->color, type, _T("ÑÕÉ«"));

	PropUpdate_int((CBCGPProp**)&m_pPropTick, (int*)(&pItem->lTick), type, _T("Ö¡Ê±¼ä"));

	PropUpdate_float((CBCGPProp**)&m_pPropUScale, &(pItem->fUScale), type, _T("UËõ·Å"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropVScale, &(pItem->fVScale), type, _T("VËõ·Å"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropUOff, &(pItem->fUOff), type, _T("UÆ«ÒÆ"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropVOff, &(pItem->fVOff), type, _T("VÆ«ÒÆ"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropUVRotate, &(pItem->fUVRotate), type, _T("UVÐý×ª"), 1.0f, 180.0f, -180.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropMaskUScale, &(pItem->fMaskUScale), type, _T("Mask UËõ·Å"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVScale, &(pItem->fMaskVScale), type, _T("Mask VËõ·Å"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUOff, &(pItem->fMaskUOff), type, _T("Mask UÆ«ÒÆ"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVOff, &(pItem->fMaskVOff), type, _T("Mask VÆ«ÒÆ"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVRotate, &(pItem->fMaskUVRotate), type, _T("Mask UVÐý×ª"), 1.0f, 180.0f, -180.0f);
}
void CDecalBar::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CEffectEditBar::OnClose();
}

void CDecalBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CEffectEditBar::OnShowWindow(bShow, nStatus);
	if(bShow == FALSE)
	{
		ReleaseKeyFrameWnd();
	}
	// TODO: Add your message handler code here
}


void CDecalBar::UpdateKeyFrameWnd()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_REQUEST_KFWND, 0, (LPARAM) pObj);
}

void CDecalBar::ReleaseKeyFrameWnd()
{
	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_RELEASE_KFWND, 0, (LPARAM) pObj);
}

void CDecalBar::CopyKeyFrame(int from, int to)
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

#define DCL_LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))
int CDecalBar::AddKeyFrame(UINT tick)
{
	m_wndPropList.EndEditItem(FALSE);
	CDecal::DCL_FRAME dcl;
	dcl.lTick = tick;

	int i = 0;
	for(i = 0; i < (int) m_pTarget->m_KeyFrames.size(); i++)
	{
		if(m_pTarget->m_KeyFrames[i].lTick > dcl.lTick)
			break;
	}

	ASSERT( i > 0 && "Add KeyFrame, Never add at first ");
	if(i == (int) m_pTarget->m_KeyFrames.size())
	{
		CDecal::DCL_FRAME& last = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1];
		dcl.size = last.size;

		m_pTarget->m_KeyFrames.push_back(dcl);
	}
	else
	{
		CDecal::DCL_FRAME& next = m_pTarget->m_KeyFrames[i];
		CDecal::DCL_FRAME& prev = m_pTarget->m_KeyFrames[i - 1];
		float pos = (float)(dcl.lTick - prev.lTick ) / ( next.lTick - prev.lTick );

		dcl.size = DCL_LINEAR_LERP(prev.size, next.size, pos);

		m_pTarget->m_KeyFrames.insert(m_pTarget->m_KeyFrames.begin() + i, dcl);
	}
	UpdateTree();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	return i;

}

void CDecalBar::DeleteKeyFrame(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames.erase(m_pTarget->m_KeyFrames.begin() + index);
		UpdateTree();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

void CDecalBar::ActiveKeyFrame(int index)
{
	int i = 0;
	HTREEITEM hCurr = m_wndTree.GetChildItem(m_hKeyFrame);
	while(hCurr != NULL)
	{
		CDecal::DCL_FRAME* pFrameItem = (CDecal::DCL_FRAME*)GetItemParam(hCurr);
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

void CDecalBar::SetKeyFramePos(int index, UINT tick)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames[index].lTick = tick;
		UpdateList();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

int CDecalBar::GetKeyFrameCount()
{
	return (int)m_pTarget->m_KeyFrames.size();
}

UINT CDecalBar::GetKeyFrameTick(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		return (UINT)m_pTarget->m_KeyFrames[index].lTick;
	}
	return -1;
}

CString CDecalBar::GetName()
{
	return m_pTarget->GetName();
}
// CDecalBar message handlers

