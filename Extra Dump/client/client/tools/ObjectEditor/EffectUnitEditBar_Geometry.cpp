
#include "stdafx.h"
#include "EffectUnit_Geometry.h"
#include "EffectUnitEditBar_Geometry.h"
#include "SplitterGridView.h"
#include "NotifyProperty.h"
#include "EffectManager.h"
#include "BindInfo.h"
#include "ObjectTypeHelper.h"
#include <vector>

IMPLEMENT_DYNAMIC(CGeometryBar, CEffectEditBar)
CGeometryBar::CGeometryBar()
{
	m_bLockApp = true;
}

CGeometryBar::~CGeometryBar()
{
}


BEGIN_MESSAGE_MAP(CGeometryBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGEDA, ID_GEOMTREE, OnTreeSelChanged)
	ON_NOTIFY(NM_DBLCLK, ID_GEOMTREE, OnTreeDblClick)

	ON_COMMAND(ID_BTN_GEO_NEWFRAME, OnNewFrame)
	ON_COMMAND(ID_BTN_GEO_DELFRAME, OnDeleteFrame)
	ON_COMMAND(ID_BTN_GEO_UP, OnMoveUp)
	ON_COMMAND(ID_BTN_GEO_DOWN, OnMoveDown)
	ON_COMMAND(ID_BTN_GEO_APPLY, OnApply)
	ON_COMMAND(ID_BTN_GEO_DELETE, OnRemove)
	ON_COMMAND(ID_BTN_GEO_SAVE, OnSave)
	//ON_COMMAND(ID_BTN_GEO_VISIBLE, OnVisible)
	ON_UPDATE_COMMAND_UI(ID_BTN_GEO_VISIBLE, OnUpdateVisible)

	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)

	ON_COMMAND(ID_BTN_GEO_LOCKAPPLY, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_GEO_LOCKAPPLY, OnUpdateBtnLockApply)
END_MESSAGE_MAP()


// CGeometryBar message handlers

int CGeometryBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this) 
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_GEOMETRY))
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
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height () / 3),&context1);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height () / 3 * 2),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	CSplitterGridView* m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	CSplitterGridView* m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	m_wndTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS , 
		CRect(0, 0, 100, 100), m_pGrid1, ID_GEOMTREE);
	m_hDesc = m_wndTree.InsertItem(_T("几何体基本参数"));
	m_hKeyFrame = m_wndTree.InsertItem(_T("关键帧"));


	m_pGrid1->SetInnerCtrl(&m_wndTree);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_GEOMPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);

	//ChangePropList();
	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	return 0;
}

void CGeometryBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}


BOOL CGeometryBar::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call	the base class
	//SAFE_DELETE( m_pGrid1);
	//SAFE_DELETE (m_pGrid2);
	return CEffectEditBar::DestroyWindow();
}

void CGeometryBar::OnTreeDblClick(NMHDR* pHdr, LRESULT* pResult)
{
	HTREEITEM item = m_wndTree.GetSelectedItem();
	if(item != NULL && 
		(item == m_hKeyFrame || m_wndTree.GetParentItem(item) == m_hKeyFrame))
	{
		UpdateKeyFrameWnd();
	}

}

LRESULT CGeometryBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	if(m_hLastSel != NULL)
		SaveCurrentProp(m_hLastSel);
	else
		SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->UpdateSource();

	if(m_bLockApp && (pnm->pProp == (CNotifiedProp*)m_pPropBlendMode
		|| pnm->pProp == (CNotifiedProp*)m_pPropGeomType
		|| pnm->pProp == (CNotifiedProp*)m_pPropTexture
		|| pnm->pProp == (CNotifiedProp*)m_pPropBindBone
		|| pnm->pProp == (CNotifiedProp*)m_pPropAddressMode
		|| pnm->pProp == (CNotifiedProp*)m_pPropDistort
		|| pnm->pProp == (CNotifiedProp*)m_pPropUSeg
		|| pnm->pProp == (CNotifiedProp*)m_pPropVSeg
		|| pnm->pProp == (CNotifiedProp*)m_pPropShowGrid
		|| pnm->pProp == (CNotifiedProp*)m_pPropMaskTexture
		|| pnm->pProp == (CNotifiedProp*)m_pPropMaskUVAddressMode

		))
	{
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
	return 0;
}


void CGeometryBar::OnDestroy()
{
	CEffectEditBar::OnDestroy();
}

void CGeometryBar::OnNewFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	CGeometry::GEO_FRAME item;
	if(m_pTarget->m_KeyFrames.size() > 0)
		item.lTick = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1].lTick + 1000;

	m_pTarget->m_KeyFrames.push_back(item);

	UpdateTree();
	m_pTarget->UpdateSource();
	UpdateKeyFrameWnd();
}

void CGeometryBar::OnDeleteFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return ;

	CGeometry::GEO_FRAME* pFrameItem = (CGeometry::GEO_FRAME*)GetItemParam(hSelected);

	std::vector<CGeometry::GEO_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
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

void CGeometryBar::OnMoveUp()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return;
	CGeometry::GEO_FRAME* pItem = (CGeometry::GEO_FRAME* )GetItemParam(hSelected);
	std::vector<CGeometry::GEO_FRAME>::iterator back = m_pTarget->m_KeyFrames.end();
	std::vector<CGeometry::GEO_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(pItem == &(*it) && back != m_pTarget->m_KeyFrames.end())
		{
			CGeometry::GEO_FRAME tmp = *it;
			m_pTarget->m_KeyFrames.erase(it);
			m_pTarget->m_KeyFrames.insert(back, tmp);
			break;
		}
		back = it;
	} 
	UpdateTree();
}

void CGeometryBar::OnMoveDown()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
		return;
	CGeometry::GEO_FRAME* pItem = (CGeometry::GEO_FRAME* )GetItemParam(hSelected);
	std::vector<CGeometry::GEO_FRAME>::iterator back = m_pTarget->m_KeyFrames.end();
	std::vector<CGeometry::GEO_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(back!=m_pTarget->m_KeyFrames.end() && pItem == &(*back))
		{
			CGeometry::GEO_FRAME tmp = *it;
			m_pTarget->m_KeyFrames.erase(it);
			m_pTarget->m_KeyFrames.insert(back, tmp);
			break;
		}
		back = it;
	}
	UpdateTree();
}

void CGeometryBar::OnApply()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	//PostMainFrameMsg( EEM_APPLY);
	//PostMainFrameMsg(EEM_UPDATA_LIST);
	//m_pTarget->RefreshNode();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}

void CGeometryBar::OnRemove()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CGeometryBar::OnSave()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	SendMainFrameMsg( EEM_SAVE, 0, (LPARAM)m_pTarget);
}

void CGeometryBar::OnVisible()
{
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg( EEM_APPLY);
}

void CGeometryBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->IsVisible() ? 1 : 0);
}

void CGeometryBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CGeometryBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CGeometryBar::OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_hLastSel = pNm->itemOld.hItem;
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(pNm->itemOld.hItem);
	ChangePropList();
	m_hLastSel = NULL;


}

extern bool IsTextureResource(const char *path);

LRESULT CGeometryBar::OnDropRes(WPARAM w, LPARAM l)
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

void CGeometryBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = static_cast<CGeometry*>( pEffect);
	if(GetSafeHwnd())
	{
		SetWindowText(m_pTarget->m_strName.GetBuffer());
		UpdateTree();
		ChangePropList();
	}
}
CEffectUnit* CGeometryBar::GetEditTarget()
{
	return m_pTarget;
}

LPARAM CGeometryBar::GetItemParam(HTREEITEM hItem)
{
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;
	m_wndTree.GetItem(&item);
	return item.lParam;
}

void CGeometryBar::UpdateTree()
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

void CGeometryBar::ChangePropList()
{
	m_wndPropList.EndEditItem(TRUE);

	m_wndPropList.RemoveAll();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{

		DescUpdate(CREATE_PROP);

		//CBCGPProp* pPos = new CBCGPProp(_T("相对位置"));
		//pPos->AddSubItem(m_pPropOffsetX);
		//pPos->AddSubItem(m_pPropOffsetY);
		//pPos->AddSubItem(m_pPropOffsetZ);

		//CBCGPProp* pRot = new CBCGPProp(_T("旋转"));
		//pRot->AddSubItem(m_pPropYaw);
		//pRot->AddSubItem(m_pPropPitch);
		//pRot->AddSubItem(m_pPropRoll);

		//CBCGPProp* pScale = new CBCGPProp(_T("缩放"));
		//pScale->AddSubItem(m_pPropScaleX);
		//pScale->AddSubItem(m_pPropScaleY);
		//pScale->AddSubItem(m_pPropScaleZ);

		//m_pPropOffsetX->SetNotifyTarget(GetSafeHwnd());
		//m_pPropOffsetY->SetNotifyTarget(GetSafeHwnd());
		//m_pPropOffsetZ->SetNotifyTarget(GetSafeHwnd());

		//m_pPropYaw->SetNotifyTarget(GetSafeHwnd());
		//m_pPropPitch->SetNotifyTarget(GetSafeHwnd());
		//m_pPropRoll->SetNotifyTarget(GetSafeHwnd());

		//m_pPropScaleX->SetNotifyTarget(GetSafeHwnd());
		//m_pPropScaleY->SetNotifyTarget(GetSafeHwnd());
		//m_pPropScaleZ->SetNotifyTarget(GetSafeHwnd());

		m_pPropBlendMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropGeomType->SetNotifyTarget(GetSafeHwnd());
		m_pPropTexture->SetNotifyTarget(GetSafeHwnd());

		m_pPropTexCol->SetNotifyTarget(GetSafeHwnd());
		m_pPropTexRow->SetNotifyTarget(GetSafeHwnd());
		m_pPropFrameTime->SetNotifyTarget(GetSafeHwnd());
		m_pPropAddressMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropDistort->SetNotifyTarget(GetSafeHwnd());

		m_pPropUSeg->SetNotifyTarget(GetSafeHwnd());
		m_pPropVSeg->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUVAddressMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskTexCols->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskTexRows->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskTexture->SetNotifyTarget(GetSafeHwnd());
		m_pPropShowGrid->SetNotifyTarget(GetSafeHwnd());

		CBCGPProp* pTex = new CBCGPProp("几何体贴图");
		pTex->AddSubItem(m_pPropAddressMode);
		pTex->AddSubItem(m_pPropTexRow);
		pTex->AddSubItem(m_pPropTexCol);
		pTex->AddSubItem(m_pPropTexture);


		CBCGPProp* pMaskTex = new CBCGPProp("蒙板贴图");
		pMaskTex->AddSubItem(m_pPropMaskUVAddressMode);
		pMaskTex->AddSubItem(m_pPropMaskTexRows);
		pMaskTex->AddSubItem(m_pPropMaskTexCols);
		pMaskTex->AddSubItem(m_pPropMaskTexture);


		//m_wndPropList.AddProperty(pPos);
		//m_wndPropList.AddProperty(pRot);
		//m_wndPropList.AddProperty(pScale);

		m_wndPropList.AddProperty(m_pPropBlendMode);
		//m_wndPropList.AddProperty(m_pPropAddressMode);
		m_wndPropList.AddProperty(m_pPropGeomType);
		m_wndPropList.AddProperty(m_pPropDistort);
		m_wndPropList.AddProperty(m_pPropShowGrid);
		m_wndPropList.AddProperty(m_pPropUSeg);
		m_wndPropList.AddProperty(m_pPropVSeg);
		m_wndPropList.AddProperty(m_pPropFrameTime);
		m_wndPropList.AddProperty(pTex);
		m_wndPropList.AddProperty(pMaskTex);




		//m_wndPropList.AddProperty(m_pPropTexture);
		m_wndPropList.AddProperty(m_pPropBindBone);


	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CGeometry::GEO_FRAME* pFrameItem = (CGeometry::GEO_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());

		KeyFrameUpdate(CREATE_PROP, pFrameItem);

		//((CNotifiedProp*)m_pPropColor)->SetNotifyTarget(GetSafeHwnd());
		m_pPropShereR->SetNotifyTarget(GetSafeHwnd());
		m_pPropShereAngle->SetNotifyTarget(GetSafeHwnd());

		m_pPropTursoLR->SetNotifyTarget(GetSafeHwnd());
		m_pPropTursoNR->SetNotifyTarget(GetSafeHwnd());
		m_pPropTubeH->SetNotifyTarget(GetSafeHwnd());
		m_pPropTubeLR->SetNotifyTarget(GetSafeHwnd());
		m_pPropTubeNR->SetNotifyTarget(GetSafeHwnd());

		m_pPropColumnH->SetNotifyTarget(GetSafeHwnd());
		m_pPropColumnDR->SetNotifyTarget(GetSafeHwnd());
		m_pPropColumnUR->SetNotifyTarget(GetSafeHwnd());

		m_pPropUOffset->SetNotifyTarget(GetSafeHwnd());
		m_pPropVOffset->SetNotifyTarget(GetSafeHwnd());
		m_pPropRound->SetNotifyTarget(GetSafeHwnd());

		m_pPropUScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropVScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropUVRotate->SetNotifyTarget(GetSafeHwnd());
		m_pPropColor->SetNotifyTarget(GetSafeHwnd());
		m_pPropAlpha->SetNotifyTarget(GetSafeHwnd());
		m_pPropTick->SetNotifyTarget(GetSafeHwnd());

		m_pPropMaskUVRotate->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskVScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUOffset->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskVOffset->SetNotifyTarget(GetSafeHwnd());


		CBCGPProp* pSphere = new CBCGPProp("球体参数");
		pSphere->AddSubItem(m_pPropShereR);
		pSphere->AddSubItem(m_pPropShereAngle);

		CBCGPProp* pTurso= new CBCGPProp("圆环参数");
		pTurso->AddSubItem(m_pPropTursoLR);
		pTurso->AddSubItem(m_pPropTursoNR);

		CBCGPProp* pTube = new CBCGPProp("圆桶参数");
		pTube->AddSubItem(m_pPropTubeH);
		pTube->AddSubItem(m_pPropTubeLR);
		pTube->AddSubItem(m_pPropTubeNR);

		CBCGPProp* pColumn = new CBCGPProp("圆柱参数");
		pColumn->AddSubItem(m_pPropColumnH);
		pColumn->AddSubItem(m_pPropColumnDR);
		pColumn->AddSubItem(m_pPropColumnUR);

		CBCGPProp* pTex = new CBCGPProp("几何体贴图");
		pTex->AddSubItem(m_pPropUScale);
		pTex->AddSubItem(m_pPropVScale);
		pTex->AddSubItem(m_pPropUOffset);
		pTex->AddSubItem(m_pPropVOffset);

		pTex->AddSubItem(m_pPropUVRotate);


		CBCGPProp* pMaskTex = new CBCGPProp("蒙板贴图");
		pMaskTex->AddSubItem(m_pPropMaskUScale);
		pMaskTex->AddSubItem(m_pPropMaskVScale);
		pMaskTex->AddSubItem(m_pPropMaskUOffset);
		pMaskTex->AddSubItem(m_pPropMaskVOffset);
		pMaskTex->AddSubItem(m_pPropMaskUVRotate);


		//CBCGPProp* pUVOffset = new CBCGPProp("UV偏移");
		//pUVOffset->AddSubItem(m_pPropUOffset);
		//pUVOffset->AddSubItem(m_pPropVOffset);

		//CBCGPProp* pUVScale = new CBCGPProp("UV缩放");
		//pUVScale->AddSubItem(m_pPropUScale);
		//pUVScale->AddSubItem(m_pPropVScale);

		m_wndPropList.AddProperty(pSphere);
		m_wndPropList.AddProperty(pTurso);
		m_wndPropList.AddProperty(pTube);
		m_wndPropList.AddProperty(pColumn);

		//m_wndPropList.AddProperty(m_pPropUVRotate);
		m_wndPropList.AddProperty(pTex);
		m_wndPropList.AddProperty(pMaskTex);
		m_wndPropList.AddProperty(m_pPropRound);
		m_wndPropList.AddProperty(m_pPropColor);
		m_wndPropList.AddProperty(m_pPropAlpha);	
		m_wndPropList.AddProperty(m_pPropTick); 


		pSphere->Expand(FALSE);
		pTurso->Expand(FALSE);
		pTube->Expand(FALSE);
		pColumn->Expand(FALSE);

		if(m_pTarget && m_pTarget->m_GeomType == Ogre::GEO_SPHERE)
			pSphere->Expand();
		if(m_pTarget && m_pTarget->m_GeomType == Ogre::GEO_TORUS)
			pTurso->Expand();
		if(m_pTarget && m_pTarget->m_GeomType == Ogre::GEO_TUBE)
			pTube->Expand();
		if(m_pTarget && m_pTarget->m_GeomType == Ogre::GEO_COLUMN)
			pColumn->Expand();
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

void CGeometryBar::SaveCurrentProp(HTREEITEM hOldItem)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	m_wndPropList.EndEditItem(FALSE);

	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	if(hOldItem == m_hDesc)
	{
		DescUpdate(PROP_TO_DATA);

	}
	else if(m_wndTree.GetParentItem(hOldItem) == m_hKeyFrame)
	{
		CGeometry::GEO_FRAME* pFrameItem = (CGeometry::GEO_FRAME*)GetItemParam(hOldItem);
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

void CGeometryBar::UpdateList()
{
	CCvtHelper cvt;
	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		DescUpdate(DATA_TO_PROP);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CGeometry::GEO_FRAME* pFrameItem = (CGeometry::GEO_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		KeyFrameUpdate(DATA_TO_PROP, pFrameItem);
	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
			m_wndPropList.GetProperty(i)->SetValue(cvt.IntToString(m_pTarget->m_KeyFrames[i].lTick));
	}
	m_wndPropList.Invalidate();
}

void CGeometryBar::DescUpdate(DATA_UPDATA_TYPE type)
{
	//PropUpdate_float((CBCGPProp**)&m_pPropOffsetX, &m_pTarget->m_bindInfo.offset.x, type, _T("X"));
	//PropUpdate_float((CBCGPProp**)&m_pPropOffsetY, &m_pTarget->m_bindInfo.offset.y, type, _T("Y"));
	//PropUpdate_float((CBCGPProp**)&m_pPropOffsetZ, &m_pTarget->m_bindInfo.offset.z, type, _T("Z"));

	//if(type == PROP_TO_DATA)
	//{
	//	float y,p,r;
	//	//C3DMath::QuaternionToEular(*data, &y, &p, &r);
	//	r = (float)(m_pPropRoll->GetValue());
	//	p = (float)(m_pPropPitch->GetValue());
	//	y = (float)(m_pPropYaw->GetValue());

	//	if(r == 0.0f && p == 0.0f && y == 0.0f)
	//	{
	//		m_pTarget->m_bindInfo.rotate.SetEulerAngle(y, p, r);
	//	}
	//}
	//else
	//{
	//	PropUpdate_eular_yaw  ((CBCGPProp**)&m_pPropYaw, &m_pTarget->m_bindInfo.rotate, type, _T("转角"));
	//	PropUpdate_eular_pitch((CBCGPProp**)&m_pPropPitch, &m_pTarget->m_bindInfo.rotate, type, _T("俯仰角"));
	//	PropUpdate_eular_roll ((CBCGPProp**)&m_pPropRoll, &m_pTarget->m_bindInfo.rotate, type, _T("侧滚角"));
	//}

	//PropUpdate_rot((CBCGPProp**)&m_pPropYaw, (CBCGPProp**)&m_pPropPitch,(CBCGPProp**)&m_pPropRoll, &m_pTarget->m_bindInfo.rotate, type, _T("旋转"));

	//PropUpdate_float((CBCGPProp**)&m_pPropScaleX, &m_pTarget->m_bindInfo.scale.x, type, _T("X"), 0.01f, 32767.0f, 0.01f);
	//PropUpdate_float((CBCGPProp**)&m_pPropScaleY, &m_pTarget->m_bindInfo.scale.y, type, _T("Y"),  0.01f, 32767.0f, 0.01f);
	//PropUpdate_float((CBCGPProp**)&m_pPropScaleZ, &m_pTarget->m_bindInfo.scale.z, type, _T("Z"),  0.01f, 32767.0f, 0.01f);

	PropUpdate_geomtype((CBCGPProp**)&m_pPropGeomType , &m_pTarget->m_GeomType, type, _T("几何体类型"));
	PropUpdate_blendtype((CBCGPProp**)&m_pPropBlendMode, (Ogre::UIBlendMode*)&m_pTarget->m_iBlendMode, type, _T("混合模式"));
	PropUpdate_int((CBCGPProp**)&m_pPropTexCol, &m_pTarget->m_iTexCol , type, _T("贴图列数"), 0.01f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropTexRow, &m_pTarget->m_iTexRow , type, _T("贴图行数"), 0.01f, 32767, 1);
	PropUpdate_float((CBCGPProp**)&m_pPropFrameTime, &m_pTarget->m_fFrameTime, type, _T("帧间隔时间"), 0.01f, 32767.0f, 0.0f);

	PropUpdate_bool((CBCGPProp**)&m_pPropDistort, &m_pTarget->m_bDistort, type, "空间扭曲模式");

	PropUpdate_cstring((CBCGPProp**)&m_pPropTexture, &m_pTarget->m_strTexture, type, _T("贴图地址"));
	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("绑定点"));
	PropUpdate_addressmode((CBCGPProp**)&m_pPropAddressMode, &m_pTarget->m_iUVAddressMode, type, _T("UV寻址模式"));

	PropUpdate_int((CBCGPProp**)&m_pPropVSeg, &m_pTarget->m_nVSeg, type, _T("V方向分段"),  0.1f, 32767.0f, 2.0f);
	PropUpdate_int((CBCGPProp**)&m_pPropUSeg, &m_pTarget->m_nUSeg, type, _T("U方向分段"),  0.1f, 32767.0f, 2.0f);
	PropUpdate_bool((CBCGPProp**)&m_pPropShowGrid, &m_pTarget->m_bShowGrid, type, _T("网格模式"));

	PropUpdate_int((CBCGPProp**)&m_pPropMaskTexCols, &m_pTarget->m_iMaskTexCols , type, _T("贴图列数"), 0.01f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropMaskTexRows, &m_pTarget->m_iMaskTexRows , type, _T("贴图行数"), 0.01f, 32767, 1);
	PropUpdate_addressmode((CBCGPProp**)&m_pPropMaskUVAddressMode, &m_pTarget->m_iMaskUVAddressMode, type, _T("UV寻址模式"));
	PropUpdate_cstring((CBCGPProp**)&m_pPropMaskTexture, &m_pTarget->m_strMaskTexture, type, _T("贴图地址"));


}

void CGeometryBar::KeyFrameUpdate(DATA_UPDATA_TYPE type, CGeometry::GEO_FRAME* pItem)
{
	PropUpdate_float((CBCGPProp**)&m_pPropShereR, &(pItem->fShpereR), type, _T("球半径"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropShereAngle, &(pItem->fShpereAngle), type, _T("半球角度"), 1.0f, 360.0f, 0.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropTursoLR, &(pItem->fTurosLR), type, _T("环主半径"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropTursoNR, &(pItem->fTurosNR), type, _T("环口半径"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropTubeH, &(pItem->fTubeH), type, _T("桶高"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropTubeLR, &(pItem->fTubeLR), type, _T("桶外半径"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropTubeNR, &(pItem->fTubeNR), type, _T("桶内半径"), 1.0f, 32767.0f, 0.0f);


	PropUpdate_float((CBCGPProp**)&m_pPropColumnH, &(pItem->fColumnH), type, _T("柱高"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropColumnDR, &(pItem->fColumnDR), type, _T("柱下半径"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropColumnUR, &(pItem->fColumnUR), type, _T("柱上半径"), 1.0f, 32767.0f, 0.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropRound, &(pItem->fRound), type, _T("圆周角度"), 1.0f, 360.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropUOffset, &(pItem->fUOffset), type, _T("U偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropVOffset, &(pItem->fVOffset), type, _T("V偏移"), 0.01f, 32767.0f, -32767.0f);


	PropUpdate_float((CBCGPProp**)&m_pPropUScale, &(pItem->fUScale), type, _T("U缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropVScale, &(pItem->fVScale), type, _T("V缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropUVRotate, &(pItem->fUVRotate), type, _T("UV旋转"), 1.0f, 32767.0f, -32767.0f);
	PropUpdate_color((CBCGPColorProp**)&m_pPropColor, &(pItem->color), type, _T("颜色"));
	PropUpdate_float((CBCGPProp**)&m_pPropAlpha, &(pItem->alpha), type, _T("透明度"), 0.01f, 1.0f, 0.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropMaskUOffset, &(pItem->fMaskUOffset), type, _T("U偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVOffset, &(pItem->fMaskVOffset), type, _T("V偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUScale, &(pItem->fMaskUScale), type, _T("U缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVScale, &(pItem->fMaskVScale), type, _T("V缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVRotate, &(pItem->fMaskUVRotate), type, _T("UV旋转"), 1.0f, 32767.0f, -32767.0f);


	PropUpdate_int((CBCGPProp**)&m_pPropTick, (int*)(&pItem->lTick), type, _T("帧时间"));

	if(type == CREATE_PROP)
	{
		if(m_pTarget->m_GeomType == Ogre::GEO_SPHERE)
		{
			m_pPropShereR->Enable(TRUE);
			m_pPropShereAngle->Enable(TRUE);

			m_pPropTursoLR->Enable(FALSE);
			m_pPropTursoNR->Enable(FALSE);

			m_pPropTubeH->Enable(FALSE);
			m_pPropTubeLR->Enable(FALSE);
			m_pPropTubeNR->Enable(FALSE);

			m_pPropColumnH->Enable(FALSE);
			m_pPropColumnDR->Enable(FALSE);
			m_pPropColumnUR->Enable(FALSE);
		}
		else if(m_pTarget->m_GeomType == Ogre::GEO_TORUS )
		{
			m_pPropShereR->Enable(FALSE);
			m_pPropShereAngle->Enable(FALSE);

			m_pPropTursoLR->Enable(TRUE);
			m_pPropTursoNR->Enable(TRUE);

			m_pPropTubeH->Enable(FALSE);
			m_pPropTubeLR->Enable(FALSE);
			m_pPropTubeNR->Enable(FALSE);

			m_pPropColumnH->Enable(FALSE);
			m_pPropColumnDR->Enable(FALSE);
			m_pPropColumnUR->Enable(FALSE);
		}
		else if(m_pTarget->m_GeomType == Ogre::GEO_TUBE)
		{
			m_pPropShereR->Enable(FALSE);
			m_pPropShereAngle->Enable(FALSE);

			m_pPropTursoLR->Enable(FALSE);
			m_pPropTursoNR->Enable(FALSE);

			m_pPropTubeH->Enable(TRUE);
			m_pPropTubeLR->Enable(TRUE);
			m_pPropTubeNR->Enable(TRUE);

			m_pPropColumnH->Enable(FALSE);
			m_pPropColumnDR->Enable(FALSE);
			m_pPropColumnUR->Enable(FALSE);
		}
		else if(m_pTarget->m_GeomType == Ogre::GEO_TUBE)
		{
			m_pPropShereR->Enable(FALSE);
			m_pPropShereAngle->Enable(FALSE);

			m_pPropTursoLR->Enable(FALSE);
			m_pPropTursoNR->Enable(FALSE);

			m_pPropTubeH->Enable(FALSE);
			m_pPropTubeLR->Enable(FALSE);
			m_pPropTubeNR->Enable(FALSE);

			m_pPropColumnH->Enable(TRUE);
			m_pPropColumnDR->Enable(TRUE);
			m_pPropColumnUR->Enable(TRUE);
		}
	}
}
void CGeometryBar::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CEffectEditBar::OnClose();
}

void CGeometryBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CEffectEditBar::OnShowWindow(bShow, nStatus);
	if(bShow == FALSE)
	{
		ReleaseKeyFrameWnd();
	}
	// TODO: Add your message handler code here
}

void CGeometryBar::PropUpdate_geomtype(CBCGPProp** ppProp, Ogre::PARAMSHAPE_TYPE* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		std::vector<CString> geomtypes;
		(*ppProp) = new CNotifiedProp(name, _T(""));
		cvt.EnumGeomType(geomtypes);
		for(int i = 0; i < (int) geomtypes.size(); i++)
		{
			(*ppProp)->AddOption(geomtypes[i].GetBuffer());
		}
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::PARAMSHAPE_TYPE)cvt.StringToGeomType(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.GeomTypeToStringAsPtr((*data)));
	}
}


void CGeometryBar::UpdateKeyFrameWnd()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_REQUEST_KFWND, 0, (LPARAM) pObj);
}

void CGeometryBar::ReleaseKeyFrameWnd()
{
	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_RELEASE_KFWND, 0, (LPARAM) pObj);
}

void CGeometryBar::CopyKeyFrame(int from, int to)
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

#define GEO_LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))
int CGeometryBar::AddKeyFrame(UINT tick)
{
	m_wndPropList.EndEditItem(FALSE);
	CGeometry::GEO_FRAME frm;
	frm.lTick = tick;

	int i = 0;
	for(i = 0; i < (int) m_pTarget->m_KeyFrames.size(); i++)
	{
		if(m_pTarget->m_KeyFrames[i].lTick > frm.lTick)
			break;
	}

	ASSERT( i > 0 && "Add KeyFrame, Never add at first ");
	if(i == (int) m_pTarget->m_KeyFrames.size())
	{
		CGeometry::GEO_FRAME& last = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1];
		frm.fShpereR = last.fShpereR;
		frm.fTurosNR = last.fTurosNR;
		frm.fTurosLR = last.fTurosLR;
		frm.fTubeH = last.fTubeH;
		frm.fTubeLR = last.fTubeLR;
		frm.fTubeNR = last.fTubeNR;

		frm.fColumnDR = last.fColumnDR;
		frm.fColumnH = last.fColumnH;
		frm.fColumnUR = last.fColumnUR;
		frm.fRound = last.fRound;
		frm.fShpereAngle = last.fShpereAngle;
		frm.fUOffset = last.fUOffset;
		frm.fVOffset = last.fVOffset;
		frm.color = last.color;
		frm.fUVRotate = last.fUVRotate;
		frm.fUScale = last.fUScale;
		frm.fVScale = last.fVScale;
		frm.fMaskUOffset = last.fMaskUOffset;
		frm.fMaskVOffset = last.fMaskVOffset;
		frm.fMaskUVRotate = last.fMaskUVRotate;
		frm.fMaskUScale = last.fMaskUScale;
		frm.fMaskVScale = last.fMaskVScale;

		m_pTarget->m_KeyFrames.push_back(frm);
	}
	else
	{
		CGeometry::GEO_FRAME& next = m_pTarget->m_KeyFrames[i];
		CGeometry::GEO_FRAME& prev = m_pTarget->m_KeyFrames[i - 1];
		float pos = (float)(frm.lTick - prev.lTick ) / ( next.lTick - prev.lTick);

		frm.fShpereR	= GEO_LINEAR_LERP(prev.fShpereR,	next.fShpereR,		pos);
		frm.fTurosNR	= GEO_LINEAR_LERP(prev.fTurosNR,	next.fTurosNR,		pos);
		frm.fTurosLR	= GEO_LINEAR_LERP(prev.fTurosLR,	next.fTurosLR,		pos);
		frm.fTubeH		= GEO_LINEAR_LERP(prev.fTubeH,		next.fTubeH,		pos);
		frm.fTubeLR		= GEO_LINEAR_LERP(prev.fTubeLR,		next.fTubeLR,		pos);
		frm.fTubeNR		= GEO_LINEAR_LERP(prev.fTubeNR,		next.fTubeNR,		pos);
		frm.fColumnDR	= GEO_LINEAR_LERP(prev.fColumnDR,	next.fColumnDR,		pos);
		frm.fColumnH	= GEO_LINEAR_LERP(prev.fColumnH,	next.fColumnH,		pos);
		frm.fColumnUR	= GEO_LINEAR_LERP(prev.fColumnUR,	next.fColumnUR,		pos);
		frm.fRound		= GEO_LINEAR_LERP(prev.fRound,		next.fRound,		pos);
		frm.fShpereAngle= GEO_LINEAR_LERP(prev.fShpereAngle,next.fShpereAngle,	pos);
		frm.fUOffset	= GEO_LINEAR_LERP(prev.fUOffset,	next.fUOffset,		pos);
		frm.fVOffset	= GEO_LINEAR_LERP(prev.fVOffset,	next.fVOffset,		pos);

		frm.color		= GEO_LINEAR_LERP(prev.color,		next.color,			pos);
		frm.fUVRotate	= GEO_LINEAR_LERP(prev.fUVRotate,	next.fUVRotate,		pos);
		frm.fUScale		= GEO_LINEAR_LERP(prev.fUScale,		next.fUScale,		pos);
		frm.fVScale		= GEO_LINEAR_LERP(prev.fVScale,		next.fVScale,		pos);

		frm.fMaskUOffset	= GEO_LINEAR_LERP(prev.fMaskUOffset,	next.fMaskUOffset,		pos);
		frm.fMaskVOffset	= GEO_LINEAR_LERP(prev.fMaskVOffset,	next.fMaskVOffset,		pos);
		frm.fMaskUVRotate	= GEO_LINEAR_LERP(prev.fMaskUVRotate,	next.fMaskUVRotate,		pos);
		frm.fMaskUScale		= GEO_LINEAR_LERP(prev.fMaskUScale,		next.fMaskUScale,		pos);
		frm.fMaskVScale		= GEO_LINEAR_LERP(prev.fMaskVScale,		next.fMaskVScale,		pos);		

		m_pTarget->m_KeyFrames.insert(m_pTarget->m_KeyFrames.begin() + i, frm);
	}
	UpdateTree();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	return i;

}

void CGeometryBar::DeleteKeyFrame(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames.erase(m_pTarget->m_KeyFrames.begin() + index);
		UpdateTree();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

void CGeometryBar::ActiveKeyFrame(int index)
{
	int i = 0;
	HTREEITEM hCurr = m_wndTree.GetChildItem(m_hKeyFrame);
	while(hCurr != NULL)
	{
		CGeometry::GEO_FRAME* pFrameItem = (CGeometry::GEO_FRAME*)GetItemParam(hCurr);
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

void CGeometryBar::SetKeyFramePos(int index, UINT tick)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames[index].lTick = tick;
		UpdateList();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

int CGeometryBar::GetKeyFrameCount()
{
	return (int)m_pTarget->m_KeyFrames.size();
}

UINT CGeometryBar::GetKeyFrameTick(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		return (UINT)m_pTarget->m_KeyFrames[index].lTick;
	}
	return -1;
}

CString CGeometryBar::GetName()
{
	return m_pTarget->GetName();
}
