
#include "stdafx.h"
#include "EffectUnit_Particle.h"
#include "EffectUnitEditBar_Particle.h"
#include "SplitterGridView.h"
#include "NotifyProperty.h"
#include "EffectManager.h"
#include "ObjectTypeHelper.h"
#include "NewColorKeyDlg.h"
#include "NewFloatKeyDlg.h"
#include "DirectionEditDlg.h"

IMPLEMENT_DYNAMIC(CParticleBar, CEffectEditBar)
CParticleBar::CParticleBar() : m_pGrid1(NULL), m_pGrid2(NULL)
, m_hKeyFrame(NULL), m_hDesc(NULL), m_hBindBone(NULL), m_bLockApp(true)
{
	m_wndPropList.RemoveAll();
	m_pPropAddressMode = NULL;
	m_pPropRandomTile = NULL;
	m_pPropCollideOnTerrain = NULL;
	m_pPropEmitterType = NULL;
	m_pPropParticleType = NULL;
	m_pPropPreUpdateTime = NULL;
	m_pPropFlag = NULL;
	m_pPropBlendType = NULL;
	m_pPropPos = NULL;
	m_pPropMaxParticle = NULL;
	m_pPropSlowDown = NULL;
	m_pPropRotation = NULL;
	m_pPropTexRows = NULL;
	m_pPropTexCols = NULL;
	m_pPropTimeSlot = NULL;

	m_pPropRandomRot = NULL;
	m_pPropColorKey1 = NULL;
	m_pPropUVRotate = NULL;

	m_pPropAlphaKey1 = NULL;
	m_pPropColorKey2 = NULL;
	m_pPropAlphaKey2 = NULL;
	m_pPropColorKey3 = NULL;
	m_pPropAlphaKey3 = NULL;

	m_pPropSizeKey1 = NULL;
	m_pPropSizeKey2 = NULL;
	m_pPropSizeKey3 = NULL;
	m_pPropAspectKey1 = NULL;
	m_pPropAspectKey2 = NULL;
	m_pPropAspectKey3 = NULL;

	m_pPropTexture = NULL;
	m_pPropIsLocal = NULL;
	m_pPropSpeed = NULL;
	m_pPropSpeedDir = NULL;
	m_pPropSpeedVar = NULL;
	m_pPropSpread = NULL;
	m_pPropLatitude = NULL;
	m_pPropAccelerate = NULL;
	m_pPropAccelerateDir = NULL;
	m_pPropLifespan = NULL;
	m_pPropRate = NULL;
	m_pPropAreaLenth = NULL;
	m_pPropAreaWidth = NULL;
	m_pPropAreaDepth = NULL;
	m_pPropDeacceleration = NULL;
	m_pPropTick = NULL;
	m_pPropBindBone = NULL;
	m_pPropColorBtn = NULL;
	m_pPropAlphaBtn = NULL;
	m_pPropSizeBtn = NULL;
	m_pPropAspectBtn = NULL;
	m_pPropCustomEmit = NULL;
	m_pPropRotStyle = NULL;
	m_pPropSpreadOffset = NULL;
	m_pPropUVScale = NULL;
	//m_pPropYaw = NULL;
	//m_pPropPitch = NULL;
	//m_pPropRoll = NULL;
	m_pPropEmitOnce = NULL;

	m_pPropSizeVar = NULL;
	m_pPropLifeVar = NULL;
	m_pPropRandomSpread = NULL;
	m_pPropDistort = NULL;

	m_pPropCenterOffsetMode = NULL;
	m_pPropCenterOffsetX = NULL;
	m_pPropCenterOffsetY = NULL;

	m_pPropMaskUVRotate = NULL;
	m_pPropMaskUVScale = NULL;
	m_pPropMaskUVAddressMode = NULL;
	m_pPropMaskRow = NULL;
	m_pPropMaskCol = NULL;
	m_pPropMaskTexture = NULL;
}

CParticleBar::~CParticleBar()
{
	//m_wndPropList.m_pSel
	ClearProp();
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CParticleBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGEDA, ID_PARTICLETREE, OnTreeSelChanged)
	ON_NOTIFY(NM_DBLCLK, ID_PARTICLETREE, OnTreeDblClick)

	ON_COMMAND(ID_BTN_PTL_NEWFRAME, OnNewFrame)
	ON_COMMAND(ID_BTN_PTL_DELETEFRAME, OnDeleteFrame)
	ON_COMMAND(ID_BTN_PTL_NEW_CLD, OnNewCld)
	ON_COMMAND(ID_BTN_PTL_DEL_CLD, OnDelCld)
	ON_COMMAND(ID_BTN_PTL_APPLY, OnApply)
	ON_COMMAND(ID_BTN_PTL_REMOVE, OnRemove)
	ON_COMMAND(ID_BTN_PTL_SAVE, OnSave)
	//ON_COMMAND(ID_BTN_PTL_VISIBLE, OnVisible)
	ON_COMMAND(ID_BTN_PTL_CLICK, OnBtnClick)
	ON_COMMAND(ID_BTN_PTL_LOCKAPPLY, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_PTL_LOCKAPPLY, OnUpdateBtnLockApply)
	ON_UPDATE_COMMAND_UI(ID_BTN_PTL_VISIBLE, OnUpdateVisible)
	ON_COMMAND(ID_BTN_PTL_DEL_REPLACEOBJ, OnBtnDelReplaceObj)	

	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickPropBtn)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CParticleBar message handlers


int CParticleBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this) 
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_PARTICLE))
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

	m_wndTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS , CRect(0, 0, 100, 100), m_pGrid1, ID_PARTICLETREE);
	m_hDesc = m_wndTree.InsertItem(_T("粒子参数"));
	m_hKeyFrame = m_wndTree.InsertItem(_T("关键帧"));
	m_hCollision = m_wndTree.InsertItem(_T("碰撞面"));
	m_hReplaceObj = m_wndTree.InsertItem(_T("替代体"));

	m_pGrid1->SetInnerCtrl(&m_wndTree);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_PARTICLEPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);
	m_hLastSel = NULL;

	ChangePropList();
	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	m_dropTargetReplaceObj.Register(&m_wndTree);
	m_dropTargetReplaceObj.SetNotifyTarget(GetSafeHwnd());

	return 0;
}

void CParticleBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, 0, 0,frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

LRESULT CParticleBar::OnUpdatePropValue(WPARAM w, LPARAM l)
{
	PROPNM* pnm = (PROPNM*)l;
	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
	if(m_hLastSel != NULL)
		SaveCurrentProp(m_hLastSel);
	else
		SaveCurrentProp(m_wndTree.GetSelectedItem());
	m_pTarget->EndRecordCommand(pCmd);
	m_pTarget->UpdateSource();

	if(m_bLockApp && 
		(pnm->pProp == m_pPropMaxParticle 
		|| pnm->pProp == m_pPropTexture
		|| pnm->pProp == m_pPropBindBone
		|| pnm->pProp == m_pPropBlendType
		|| pnm->pProp == m_pPropParticleType
		|| pnm->pProp == m_pPropPreUpdateTime
		|| pnm->pProp == m_pPropEmitOnce
		|| pnm->pProp == m_pPropDistort	
		|| pnm->pProp == m_pPropCenterOffsetMode
		|| pnm->pProp == m_pPropCenterOffsetX
		|| pnm->pProp == m_pPropCenterOffsetY
		|| pnm->pProp == m_pPropAddressMode 
		|| pnm->pProp == m_pPropMaskTexture 
		|| pnm->pProp == m_pPropMaskUVAddressMode 
		))
	{
		//m_pTarget->RefreshNode();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}

	TRACE("Particle Updated\n");

	if(pnm->pProp == m_pPropBindBone)
		PostMainFrameMsg(EEM_UPDATA_LIST, UE_EFFECT_LIST);
	return 0;
}

LRESULT CParticleBar::OnClickPropBtn(WPARAM w, LPARAM l)
{
	PROPNM* pnm = (PROPNM*)l;
	CCvtHelper cvt;
	CString str;

	if(pnm->pProp == m_pPropColorBtn)
	{
		CNewColorKeyDlg ckdlg;
		ckdlg.m_KeyValues = m_pTarget->m_ParticleColorKey;
		
		if(IDOK == ckdlg.DoModal())
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
			m_pTarget->m_ParticleColorKey = ckdlg.m_KeyValues;
			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();

			UpdateList();
		}
	}
	else if(pnm->pProp == m_pPropAlphaBtn)
	{
		CNewFloatKeyDlg fkdlg;
		fkdlg.m_strValueName = _T("透明度");
		fkdlg.m_fMin = 0.0f;
		fkdlg.m_fMax = 1.0f;
		fkdlg.m_KeyValues = m_pTarget->m_ParticleAlphaKey;
		if(IDOK == fkdlg.DoModal())
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
			m_pTarget->m_ParticleAlphaKey = fkdlg.m_KeyValues ;
			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();

			UpdateList();
		}
	}
	else if(pnm->pProp == m_pPropSizeBtn)
	{
		CNewFloatKeyDlg fkdlg;
		fkdlg.m_strValueName = _T("粒子大小");
		fkdlg.m_fMin = 0.0f;
		fkdlg.m_fMax = 1000.0f;
		fkdlg.m_KeyValues = m_pTarget->m_ParticleSizeKey;

		if(IDOK == fkdlg.DoModal())
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
			m_pTarget->m_ParticleSizeKey = fkdlg.m_KeyValues ;
			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();
			UpdateList();
		}
	}
	else if(pnm->pProp == m_pPropAspectBtn) 
	{
		CNewFloatKeyDlg fkdlg;
		fkdlg.m_strValueName = _T("纵横比");
		fkdlg.m_fMin = 0.1f;
		fkdlg.m_fMax = 10.0f;
		fkdlg.m_Mode = FKD_LOG;
		fkdlg.m_KeyValues = m_pTarget->m_ParticleAspectKey;

		if(IDOK == fkdlg.DoModal())
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

			m_pTarget->m_ParticleAspectKey = fkdlg.m_KeyValues ;
			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();

			UpdateList();
		}
	}
	else if(pnm->pProp == m_pPropAccelerateDir)
	{
		CParticle::KFItem* pFrameItem = (CParticle::KFItem*)GetItemParam(m_wndTree.GetSelectedItem());
		CDirectionEditDlg dlg;
		dlg.m_fx = pFrameItem->data.acceleratedir.x ;
		dlg.m_fy = pFrameItem->data.acceleratedir.y;
		dlg.m_fz = pFrameItem->data.acceleratedir.z;
		dlg.UpdateRotByDir();
		if(IDOK == dlg.DoModal())
		{
			pFrameItem->data.acceleratedir.x = dlg.m_fx;
			pFrameItem->data.acceleratedir.y = dlg.m_fy;
			pFrameItem->data.acceleratedir.z = dlg.m_fz;

			CString str;
			str.Format("%g, %g, %g", dlg.m_fx, dlg.m_fy, dlg.m_fz);
			pnm->pProp->SetValue(str.GetBuffer());
			m_pTarget->UpdateSource();
		}
	}
	return 0;
}

BOOL CParticleBar::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call	the base class
	//delete( m_pGrid1);
	//delete (m_pGrid2);
	return CEffectEditBar::DestroyWindow();
}

void CParticleBar::OnDestroy()
{
	CEffectEditBar::OnDestroy();
}

void CParticleBar::OnBtnDelReplaceObj()
{
	HTREEITEM hSel = m_wndTree.GetSelectedItem();
	if(hSel == NULL || m_wndTree.GetParentItem(hSel) != m_hReplaceObj)
		return;

	CString seltext = m_wndTree.GetItemText(hSel);
	TVITEM item;
	item.mask = TVIF_PARAM;
	item.hItem = m_wndTree.GetSelectedItem();
	m_wndTree.GetItem(&item);

	CEffectUnit* pToDelete = (CEffectUnit*)item.lParam;
	m_pTarget->RemoveReplaceObject(pToDelete);
	UpdateReplaceObject();


}

void CParticleBar::OnNewFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;
	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	CParticle::KFItem item;
	if(m_pTarget->m_KeyFrames.size() > 0)
		item.data.tick = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1].data.tick + 1000;
	item.handle = NULL;

	m_pTarget->m_KeyFrames.push_back(item);
	m_pTarget->EndRecordCommand(pCmd);
	UpdateTree();
	m_pTarget->UpdateSource();
	//UpdateKeyFrameWnd();
}

void CParticleBar::OnDeleteFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
	{
		m_pTarget->EndRecordCommand(pCmd);
		return ;
	}

	CParticle::KFItem* pFrameItem = (CParticle::KFItem*)GetItemParam(hSelected);

	std::vector<CParticle::KFItem>::iterator it = m_pTarget->m_KeyFrames.begin();;
	for(; it != m_pTarget->m_KeyFrames.end(); it++)
	{
		if(&(*it) == pFrameItem)
		{
			m_pTarget->m_KeyFrames.erase(it);
			break;
		}
	}

	m_pTarget->EndRecordCommand(pCmd);
	m_wndTree.SelectItem(m_hKeyFrame);
	UpdateTree();

	//UpdateKeyFrameWnd();
}

void CParticleBar::OnNewCld()
{
	Ogre::PECollisionFace face;
	face.m_bReverseUpLevel = false;
	face.m_fLength = 115.0f;
	face.m_fWidth = 115.0f;
	face.m_iAnchor = 0;
	face.m_iUpLevel = 0;
	face.m_fAttenuation = 0.4f;
	face.m_Offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	face.m_Rotation.identity();

	m_pTarget->m_CollisionFaces.push_back(face);
	UpdateTree();
}

void CParticleBar::OnDelCld()
{
	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
	if(m_wndTree.GetParentItem(hSelected) != m_hCollision)
	{
		return ;
	}

	m_pTarget->SelectCollisionFace(NULL);

	Ogre::PECollisionFace* pItem = (Ogre::PECollisionFace*)GetItemParam(hSelected);

	std::vector<Ogre::PECollisionFace>::iterator it = m_pTarget->m_CollisionFaces.begin();;
	for(; it != m_pTarget->m_CollisionFaces.end(); it++)
	{
		if(&(*it) == pItem)
		{
			m_pTarget->m_CollisionFaces.erase(it);
			break;
		}
	}

	m_wndTree.SelectItem(m_hCollision);
	UpdateTree();
}

//void CParticleBar::OnMoveUp()
//{
//	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
//	
//	m_wndPropList.EndEditItem(TRUE);
//	SaveCurrentProp(m_wndTree.GetSelectedItem());
//	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
//	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
//	{
//		m_pTarget->EndRecordCommand(pCmd);
//		return;
//	}
//	CParticle::KFItem* pItem = (CParticle::KFItem* )GetItemParam(hSelected);
//	std::vector<CParticle::KFItem>::iterator back = NULL;
//	std::vector<CParticle::KFItem>::iterator it = m_pTarget->m_KeyFrames.begin();
//	for(; it != m_pTarget->m_KeyFrames.end(); it++)
//	{
//		if(pItem == &(*it) && back != NULL)
//		{
//			CParticle::KFItem tmp = *it;
//			m_pTarget->m_KeyFrames.erase(it);
//			m_pTarget->m_KeyFrames.insert(back, tmp);
//			break;
//		}
//		back = it;
//	}
//
//	m_pTarget->EndRecordCommand(pCmd);
//	UpdateTree();
//}
//
//void CParticleBar::OnMoveDown()
//{
//	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
//	
//	m_wndPropList.EndEditItem(TRUE);
//	SaveCurrentProp(m_wndTree.GetSelectedItem());
//	HTREEITEM hSelected = m_wndTree.GetSelectedItem();
//	if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
//	{
//		m_pTarget->EndRecordCommand(pCmd);
//		return;
//	}
//	CParticle::KFItem* pItem = (CParticle::KFItem* )GetItemParam(hSelected);
//	std::vector<CParticle::KFItem>::iterator back = NULL;
//	std::vector<CParticle::KFItem>::iterator it = m_pTarget->m_KeyFrames.begin();
//	for(; it != m_pTarget->m_KeyFrames.end(); it++)
//	{
//		if(back != NULL && pItem == &(*back))
//		{
//			CParticle::KFItem tmp = *it;
//			m_pTarget->m_KeyFrames.erase(it);
//			m_pTarget->m_KeyFrames.insert(back, tmp);
//			break;
//		}
//		back = it;
//	}
//	m_pTarget->EndRecordCommand(pCmd);
//	UpdateTree();
//}

void CParticleBar::OnApply()
{	

	//CParticleCommand* pCmd = m_pTarget->BeginRecordCommand();
	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	//PostMainFrameMsg( EEM_APPLY);
	//m_pTarget->RefreshNode();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	//m_pTarget->EndRecordCommand(pCmd);
	PostMainFrameMsg(EEM_UPDATA_LIST, UE_EFFECT_LIST);
}

void CParticleBar::OnBtnClick()
{
	m_pTarget->GetInteract()->OnLBtnDown(0.0f, 0.0f, true);
	CEffectMgr::GetInstance()->SetActiveEffect(m_pTarget);
}

void CParticleBar::OnRemove()
{
	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	m_pTarget->EndRecordCommand(pCmd);
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CParticleBar::OnSave()
{
	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->EndRecordCommand(pCmd);
	SendMainFrameMsg( EEM_SAVE, 0, (LPARAM)m_pTarget);
}

void CParticleBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CParticleBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CParticleBar::OnVisible()
{
	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->EndRecordCommand(pCmd);
	m_pTarget->SetVisible(!m_pTarget->m_bVisible);
	PostMainFrameMsg( EEM_APPLY);
}

void CParticleBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->m_bVisible ? 1 : 0);
}

void CParticleBar::OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_hLastSel = pNm->itemOld.hItem;
	m_wndPropList.EndEditItem(TRUE);

	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
	SaveCurrentProp(pNm->itemOld.hItem);
	m_pTarget->EndRecordCommand(pCmd);
	ChangePropList();
	m_hLastSel = NULL;
	m_pTarget->SelectCollisionFace(NULL);
}

void CParticleBar::OnTreeDblClick(NMHDR* pHdr, LRESULT* pResult)
{
	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if(hItem != NULL && 
		(hItem == m_hKeyFrame || m_wndTree.GetParentItem(hItem) == m_hKeyFrame))
	{
		//UpdateKeyFrameWnd();
	}
	else if(hItem != NULL && 
		(m_wndTree.GetParentItem(hItem) == m_hCollision))
	{
		TVITEM item;
		item.mask = TVIF_PARAM;
		item.hItem = hItem;
		m_wndTree.GetItem(&item);
		Ogre::PECollisionFace* pFace = (Ogre::PECollisionFace*)item.lParam;
		m_pTarget->SelectCollisionFace(pFace);

	}
	else if(hItem != NULL && 
		(m_wndTree.GetParentItem(hItem) == m_hReplaceObj))
	{
		TVITEM item;
		item.mask = TVIF_PARAM;
		item.hItem = hItem;
		m_wndTree.GetItem(&item);
		CEffectUnit* pUnit = (CEffectUnit*)item.lParam;
		//m_pTarget->SelectCollisionFace(pFace);
		SendMainFrameMsg(EEM_GOTO_PAGE, 0, (LPARAM)pUnit);

	}
}

LRESULT CParticleBar::OnDropRes(WPARAM w, LPARAM l)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)l;
	if(pdn->hWnd == m_wndPropList.GetSafeHwnd())
	{
		OnDropResOnPropList(pdn);
	}
	else if(pdn->hWnd == m_wndTree.GetSafeHwnd())
	{
		OnDropResOnTree(pdn);
	}
	return 0;
}

extern bool IsTextureResource(const char *path);

void CParticleBar::OnDropResOnPropList(CResDropTarget::DROPNOTIFY* pdn)
{
	if(!IsTextureResource(pdn->strChunk)) return;

	CNotifiedProp* pNotifyProp = NULL;
	if(m_wndTree.GetSelectedItem() == m_hDesc )
	{ 

		if(m_pPropTexture->GetRect().PtInRect(pdn->dropPos))
		{
			m_pPropTexture->SetValue(pdn->strChunk);
			pNotifyProp = m_pPropTexture;
		}

		if(m_pPropMaskTexture->GetRect().PtInRect(pdn->dropPos))
		{
			m_pPropMaskTexture->SetValue(pdn->strChunk);
			pNotifyProp = m_pPropMaskTexture;
		}
	}
	if(m_bLockApp)
	{
		PROPNM nm;
		nm.pProp =pNotifyProp;
		OnUpdatePropValue(0, (LPARAM)&nm);
	}
}

extern bool IsRenderableResource(const char *path);

void CParticleBar::OnDropResOnTree(CResDropTarget::DROPNOTIFY* pdn)
{
	if(IsRenderableResource(pdn->strChunk))
	{
		//m_pTarget->m_replaceObjects.push_back(pdn->strChunk);
		m_pTarget->AddReplaceObject(pdn->strChunk);
		UpdateReplaceObject();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}

}

void CParticleBar::UpdateReplaceObject()
{
	if(m_hReplaceObj == NULL)
		return;

	HTREEITEM hItem = m_wndTree.GetChildItem(m_hReplaceObj);
	HTREEITEM hOld = NULL;
	while(hItem != NULL)
	{
		hOld = hItem;
		hItem = m_wndTree.GetNextSiblingItem(hItem);
		m_wndTree.DeleteItem(hOld);
	}

	for(int i = 0; i < (int) m_pTarget->m_replaceEffect.size(); i++)
	{
		HTREEITEM hTmp = m_wndTree.InsertItem(m_pTarget->m_replaceEffect[i]->GetName(), 0, 0, m_hReplaceObj);
		m_wndTree.SetItem(hTmp, TVIF_PARAM, NULL, 0, 0, 0, 0, (LPARAM)(m_pTarget->m_replaceEffect[i]));

	}

	m_wndTree.Expand(m_hReplaceObj, TVE_EXPAND );
}

void CParticleBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CParticle*) pEffect;
	if(GetSafeHwnd())
	{
		SetWindowText(m_pTarget->m_strName.GetBuffer());
		UpdateTree();
		ChangePropList();
	}
}
CEffectUnit* CParticleBar::GetEditTarget()
{
	return m_pTarget;
}

LPARAM CParticleBar::GetItemParam(HTREEITEM hItem)
{
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;
	m_wndTree.GetItem(&item);
	return item.lParam;
}

void CParticleBar::UpdateTree()
{
	//m_wndPropList.EndEditItem(TRUE);
	//关键帧
	HTREEITEM hItem = m_wndTree.GetChildItem(m_hKeyFrame);
	HTREEITEM hTmp = NULL;
	while(hItem != NULL)
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

	//碰撞面
	hItem = m_wndTree.GetChildItem(m_hCollision);
	hTmp = NULL;
	while(hItem != NULL)
	{
		hTmp = hItem;
		hItem = m_wndTree.GetNextSiblingItem(hItem);
		m_wndTree.DeleteItem(hTmp);
	}

	for(int i = 0; i < (int)m_pTarget->m_CollisionFaces.size(); i++)
	{
		CString strName;
		strName.Format(_T("碰撞面 %d"), i + 1);
		m_wndTree.InsertItem(TVIF_TEXT | TVIF_PARAM,strName.GetBuffer(), 0, 0, 0,0,
			(LPARAM)&m_pTarget->m_CollisionFaces[i], m_hCollision, TVI_LAST);
	}
	m_wndTree.Expand(m_hCollision, TVE_EXPAND);

	UpdateReplaceObject();


	m_wndTree.SelectItem(m_hDesc);
}

void CParticleBar::ChangePropList()
{
	m_wndPropList.EndEditItem(TRUE);

	char* a = (char*)malloc(100);
	long an = 0;
	_CrtIsMemoryBlock(a, 100, &an, NULL, NULL);
	sprintf(a, "{%d}ParticleBar ", an);

	//_CrtSetBreakAlloc(an + 8);
	ClearProp();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		DescUpdate(CREATE_PROP);

		m_pPropMaxParticle->EnableSpinControl(TRUE, 0, 10000);
		m_pPropTexRows->EnableSpinControl(TRUE, 1, 10000);
		m_pPropTexCols->EnableSpinControl(TRUE, 1, 10000);

		m_pPropColorKey1->EnableOtherButton(_T("更多颜色"), TRUE, TRUE);
		m_pPropColorKey2->EnableOtherButton(_T("更多颜色"), TRUE, TRUE);
		m_pPropColorKey3->EnableOtherButton(_T("更多颜色"), TRUE, TRUE);

		m_pPropColorBtn		= new CNotifiedProp(_T("颜色调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
		m_pPropAlphaBtn		= new CNotifiedProp(_T("Alpha调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
		m_pPropSizeBtn		= new CNotifiedProp(_T("大小调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
		m_pPropAspectBtn	= new CNotifiedProp(_T("纵横比调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());

		CBCGPProp* pPropColorKeys		= new CBCGPProp(_T("颜色变化"));
		pPropColorKeys->AddSubItem(m_pPropColorBtn);
		pPropColorKeys->AddSubItem(m_pPropAlphaBtn);

		CBCGPProp*  pPropSizeKeys			= new CBCGPProp(_T("大小变化"));
		pPropSizeKeys->AddSubItem(m_pPropSizeBtn);

		//CBCGPProp* pAspect = new CBCGPProp("纵横比变化");
		pPropSizeKeys->AddSubItem(m_pPropAspectBtn);

		m_pPropEmitterType->AddOption(_T("平面发射"));
		m_pPropEmitterType->AddOption(_T("球体发射"));
		m_pPropEmitterType->AddOption(_T("圆面发射"));
		m_pPropEmitterType->AddOption(_T("球面发射"));
		m_pPropEmitterType->AddOption(_T("柱面发射"));
		m_pPropEmitterType->AddOption(_T("柱体发射"));

		m_pPropParticleType->AddOption(_T("Normal"));
		m_pPropParticleType->AddOption(_T("Quad"));
		m_pPropParticleType->AddOption(_T("Unknown"));
		m_pPropParticleType->AddOption(_T("Object"));

		//CBCGPProp* pPos = new CBCGPProp(_T("相对位置"));
		//pPos->AddSubItem(m_pPropOffsetX);
		//pPos->AddSubItem(m_pPropOffsetY);
		//pPos->AddSubItem(m_pPropOffsetZ);

		//CBCGPProp* pRot = new CBCGPProp(_T("旋转"));
		//pRot->AddSubItem(m_pPropYaw);
		//pRot->AddSubItem(m_pPropPitch);
		//pRot->AddSubItem(m_pPropRoll);

		m_pPropRotStyle->AddOption("面对观察点");
		m_pPropRotStyle->AddOption("绕竖直旋转");
		m_pPropRotStyle->AddOption("面对上方");
		m_pPropRotStyle->AddOption("绕运动方向旋转");
		m_pPropRotStyle->AddOption("面向运动方向");
		m_pPropRotStyle->AddOption("面对上方不旋转");


		CBCGPProp* pTex = new CBCGPProp(_T("粒子贴图"));
		pTex->AddSubItem(m_pPropAddressMode);
		pTex->AddSubItem(m_pPropUVRotate);
		pTex->AddSubItem(m_pPropUVScale);
		pTex->AddSubItem(m_pPropTexRows);
		pTex->AddSubItem(m_pPropTexCols);
		pTex->AddSubItem(m_pPropTexture);

		CBCGPProp* pMaskTex = new CBCGPProp(_T("蒙板贴图"));
		pMaskTex->AddSubItem(m_pPropMaskUVAddressMode);
		pMaskTex->AddSubItem(m_pPropMaskUVRotate);
		pMaskTex->AddSubItem(m_pPropMaskUVScale);
		pMaskTex->AddSubItem(m_pPropMaskRow);
		pMaskTex->AddSubItem(m_pPropMaskCol);
		pMaskTex->AddSubItem(m_pPropMaskTexture);

		m_pPropEmitterType->SetNotifyTarget(GetSafeHwnd());
		m_pPropParticleType->SetNotifyTarget(GetSafeHwnd());
		m_pPropPreUpdateTime->SetNotifyTarget(GetSafeHwnd());

		m_pPropBlendType->SetNotifyTarget(GetSafeHwnd());

		m_pPropMaxParticle->SetNotifyTarget(GetSafeHwnd());
		//m_pPropMidPoint->SetNotifyTarget(GetSafeHwnd());
		m_pPropSlowDown->SetNotifyTarget(GetSafeHwnd());
		m_pPropRotation->SetNotifyTarget(GetSafeHwnd());
		m_pPropTexRows->SetNotifyTarget(GetSafeHwnd());
		m_pPropTexCols->SetNotifyTarget(GetSafeHwnd());
		m_pPropTimeSlot->SetNotifyTarget(GetSafeHwnd());
		m_pPropBindBone->SetNotifyTarget(GetSafeHwnd());
		m_pPropTexture->SetNotifyTarget(GetSafeHwnd());
		m_pPropIsLocal->SetNotifyTarget(GetSafeHwnd());
		m_pPropRotStyle->SetNotifyTarget(GetSafeHwnd());
		//m_pPropYaw->SetNotifyTarget(GetSafeHwnd());
		//m_pPropPitch->SetNotifyTarget(GetSafeHwnd());
		//m_pPropRoll->SetNotifyTarget(GetSafeHwnd());
		m_pPropEmitOnce->SetNotifyTarget(GetSafeHwnd());
		//m_pPropScale->SetNotifyTarget(GetSafeHwnd());

		m_pPropUVRotate->SetNotifyTarget(GetSafeHwnd());
		m_pPropUVScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropRandomTile->SetNotifyTarget(GetSafeHwnd());
		m_pPropCollideOnTerrain->SetNotifyTarget(GetSafeHwnd());
		m_pPropDistort->SetNotifyTarget(GetSafeHwnd());
		m_pPropCenterOffsetMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropCenterOffsetX->SetNotifyTarget(GetSafeHwnd());
		m_pPropCenterOffsetY->SetNotifyTarget(GetSafeHwnd());

		m_pPropAddressMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropRandomRot->SetNotifyTarget(GetSafeHwnd());

		m_pPropMaskUVRotate->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUVScale->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskUVAddressMode->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskRow->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskCol->SetNotifyTarget(GetSafeHwnd());
		m_pPropMaskTexture->SetNotifyTarget(GetSafeHwnd());

		m_wndPropList.AddProperty(m_pPropEmitterType);
		m_wndPropList.AddProperty(m_pPropParticleType);
		m_wndPropList.AddProperty(m_pPropPreUpdateTime);
		m_wndPropList.AddProperty(m_pPropBlendType);
		m_wndPropList.AddProperty(m_pPropDistort);
		m_wndPropList.AddProperty(m_pPropCenterOffsetMode);
		m_wndPropList.AddProperty(m_pPropCenterOffsetX);
		m_wndPropList.AddProperty(m_pPropCenterOffsetY);

		m_wndPropList.AddProperty(m_pPropRotStyle);
		m_wndPropList.AddProperty(m_pPropIsLocal);
		m_wndPropList.AddProperty(m_pPropCustomEmit);
		m_wndPropList.AddProperty(m_pPropEmitOnce);

		m_wndPropList.AddProperty(m_pPropMaxParticle);
		m_wndPropList.AddProperty(m_pPropSlowDown);
		m_wndPropList.AddProperty(m_pPropRotation);

		m_wndPropList.AddProperty(m_pPropRandomRot);

		m_wndPropList.AddProperty(m_pPropTimeSlot);
		m_wndPropList.AddProperty(m_pPropRandomTile);
		m_wndPropList.AddProperty(m_pPropCollideOnTerrain);
		m_wndPropList.AddProperty(pTex);
		m_wndPropList.AddProperty(pMaskTex);

		m_wndPropList.AddProperty(pPropColorKeys);
		m_wndPropList.AddProperty(pPropSizeKeys);

		m_wndPropList.AddProperty(m_pPropBindBone);

	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CParticle::KFItem* pFrameItem = (CParticle::KFItem*)GetItemParam(m_wndTree.GetSelectedItem());


		KeyFrameUpdate(CREATE_PROP, pFrameItem);

		m_pPropSpeed		->SetNotifyTarget(GetSafeHwnd());
		m_pPropSpeedVar		->SetNotifyTarget(GetSafeHwnd());
		m_pPropSpread		->SetNotifyTarget(GetSafeHwnd());
		m_pPropSpreadOffset	->SetNotifyTarget(GetSafeHwnd());
		m_pPropLatitude		->SetNotifyTarget(GetSafeHwnd());
		m_pPropAccelerate	->SetNotifyTarget(GetSafeHwnd());
		m_pPropAccelerateDir->SetNotifyTarget(GetSafeHwnd());
		m_pPropLifespan		->SetNotifyTarget(GetSafeHwnd());
		m_pPropRate			->SetNotifyTarget(GetSafeHwnd());
		m_pPropAreaLenth	->SetNotifyTarget(GetSafeHwnd());
		m_pPropAreaWidth	->SetNotifyTarget(GetSafeHwnd());
		m_pPropAreaDepth	->SetNotifyTarget(GetSafeHwnd());
		m_pPropDeacceleration->SetNotifyTarget(GetSafeHwnd());
		m_pPropTick			->SetNotifyTarget(GetSafeHwnd());

		m_pPropSizeVar		->SetNotifyTarget(GetSafeHwnd());
		m_pPropLifeVar		->SetNotifyTarget(GetSafeHwnd());
		m_pPropRandomSpread	->SetNotifyTarget(GetSafeHwnd());
		if(pFrameItem == &(m_pTarget->m_KeyFrames[0]))
			m_pPropTick->Enable(FALSE);


		m_wndPropList.AddProperty(m_pPropSpeed);
		m_wndPropList.AddProperty(m_pPropSpeedVar);
		m_wndPropList.AddProperty(m_pPropSpread);
		m_wndPropList.AddProperty(m_pPropSpreadOffset);
		m_wndPropList.AddProperty(m_pPropRandomSpread);
		m_wndPropList.AddProperty(m_pPropLatitude);
		m_wndPropList.AddProperty(m_pPropAccelerate); 
		m_wndPropList.AddProperty(m_pPropAccelerateDir);
		m_wndPropList.AddProperty(m_pPropLifespan);
		m_wndPropList.AddProperty(m_pPropLifeVar);
		m_wndPropList.AddProperty(m_pPropRate);
		m_wndPropList.AddProperty(m_pPropSizeVar);
		m_wndPropList.AddProperty(m_pPropAreaLenth);
		m_wndPropList.AddProperty(m_pPropAreaWidth);
		m_wndPropList.AddProperty(m_pPropAreaDepth);
		m_wndPropList.AddProperty(m_pPropDeacceleration);
		m_wndPropList.AddProperty(m_pPropTick);


	}

	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hCollision)
	{
		Ogre::PECollisionFace* pItem = (Ogre::PECollisionFace*)GetItemParam(m_wndTree.GetSelectedItem());

		CollisionFaceUpdate(CREATE_PROP, pItem);

		CBCGPProp* pPos = new CBCGPProp(_T("相对位置"));
		pPos->AddSubItem(m_pPropCldOffsetX);
		pPos->AddSubItem(m_pPropCldOffsetY);
		pPos->AddSubItem(m_pPropCldOffsetZ);

		CBCGPProp* pRot = new CBCGPProp(_T("旋转"));
		pRot->AddSubItem(m_pPropCldYaw);
		pRot->AddSubItem(m_pPropCldPitch);
		pRot->AddSubItem(m_pPropCldRoll);

		m_pPropCldOffsetX		->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldOffsetY		->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldOffsetZ		->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldYaw			->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldPitch			->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldRoll			->SetNotifyTarget(GetSafeHwnd());
		m_pPropUpLevel			->SetNotifyTarget(GetSafeHwnd());
		m_pPropReverseUpLevel	->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldWidth			->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldLength		->SetNotifyTarget(GetSafeHwnd());
		m_pPropCldAtten			->SetNotifyTarget(GetSafeHwnd());

		m_pPropUpLevel->SetFloatDigitRatio(0.05f);
		m_pPropCldAtten->SetFloatDigitRatio(0.01f);



		m_wndPropList.AddProperty(pPos);
		m_wndPropList.AddProperty(pRot);
		m_wndPropList.AddProperty(m_pPropUpLevel);
		m_wndPropList.AddProperty(m_pPropReverseUpLevel);
		m_wndPropList.AddProperty(m_pPropCldAtten);
		m_wndPropList.AddProperty(m_pPropCldWidth);
		m_wndPropList.AddProperty(m_pPropCldLength); 


	}

	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
		{
			CString strName;
			strName.Format(_T("关键帧 %d"), i + 1);
			CBCGPProp* pProp = new CBCGPProp(strName, _T(""));
			if(i == 0)
				pProp->Enable(FALSE);
			m_wndPropList.AddProperty(pProp);
		}
	}

	UpdateList();
}

void CParticleBar::ClearProp()
{
	if(GetSafeHwnd() && m_wndPropList.GetSafeHwnd())
		m_wndPropList.RemoveAll();
	//m_pPropOffsetX = NULL;
	//m_pPropOffsetY = NULL;
	//m_pPropOffsetZ = NULL;
	m_pPropEmitterType = NULL;
	m_pPropParticleType = NULL;
	m_pPropPreUpdateTime = NULL;
	m_pPropFlag = NULL;
	m_pPropBlendType = NULL;
	m_pPropPos = NULL;
	m_pPropMaxParticle = NULL;
	//m_pPropMidPoint = NULL;
	m_pPropSlowDown = NULL;
	m_pPropRotation = NULL;
	m_pPropTexRows = NULL;
	m_pPropTexCols = NULL;
	m_pPropTimeSlot = NULL;
	//m_pPropScale = NULL;
	//m_pPropColorKeys = NULL;

	OGRE_DELETE(m_pPropColorKey1);
	OGRE_DELETE(m_pPropAlphaKey1);
	OGRE_DELETE(m_pPropColorKey2);
	OGRE_DELETE(m_pPropAlphaKey2);
	OGRE_DELETE(m_pPropColorKey3);
	OGRE_DELETE(m_pPropAlphaKey3);

	OGRE_DELETE(m_pPropSizeKey1);
	OGRE_DELETE(m_pPropSizeKey2);
	OGRE_DELETE(m_pPropSizeKey3);
	OGRE_DELETE(m_pPropAspectKey1);
	OGRE_DELETE(m_pPropAspectKey2);
	OGRE_DELETE(m_pPropAspectKey3);

	m_pPropTexture = NULL;
	m_pPropIsLocal = NULL;
	m_pPropSpeed = NULL;
	m_pPropSpeedDir = NULL;
	m_pPropSpeedVar = NULL;
	m_pPropSpread = NULL;
	m_pPropLatitude = NULL;
	m_pPropAccelerate = NULL;
	m_pPropAccelerateDir = NULL;
	m_pPropLifespan = NULL;
	m_pPropRate = NULL;
	m_pPropAreaLenth = NULL;
	m_pPropAreaWidth = NULL;
	m_pPropAreaDepth = NULL;
	m_pPropDeacceleration = NULL;
	m_pPropTick = NULL;
	m_pPropBindBone = NULL;
	m_pPropColorBtn = NULL;
	m_pPropAlphaBtn = NULL;
	m_pPropSizeBtn = NULL;
	m_pPropAspectBtn = NULL;
	m_pPropCustomEmit = NULL;
	m_pPropRotStyle = NULL;
	m_pPropSpreadOffset = NULL;
	m_pPropRandomRot = NULL;
	m_pPropAddressMode = NULL;
	m_pPropSizeVar = NULL;
	m_pPropLifeVar = NULL;

	//m_pPropYaw = NULL;
	//m_pPropPitch = NULL;
	//m_pPropRoll = NULL;

	m_pPropUVScale = NULL;
	m_pPropRandomTile = NULL;
	m_pPropCollideOnTerrain = NULL;

	m_pPropCldOffsetX = NULL;
	m_pPropCldOffsetY = NULL;
	m_pPropCldOffsetZ = NULL;

	m_pPropCldYaw = NULL;
	m_pPropCldPitch = NULL;
	m_pPropCldRoll = NULL;

	m_pPropUpLevel = NULL;
	m_pPropReverseUpLevel = NULL;
	m_pPropCldWidth = NULL;
	m_pPropCldLength = NULL;
	m_pPropCldAtten = NULL;
	m_pPropEmitOnce = NULL;

	m_pPropDistort = NULL;
	m_pPropCenterOffsetMode = NULL;
	m_pPropCenterOffsetX = NULL;
	m_pPropCenterOffsetY = NULL;

	m_pPropMaskUVRotate = NULL;
	m_pPropMaskUVScale = NULL;
	m_pPropMaskUVAddressMode = NULL;
	m_pPropMaskRow = NULL;
	m_pPropMaskCol = NULL;
	m_pPropMaskTexture = NULL;
}

void CParticleBar::UpdateList()
{
	CCvtHelper cvt;
	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	m_wndPropList.EndEditItem(FALSE);
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{

		DescUpdate(DATA_TO_PROP);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CParticle::KFItem* pFrameItem = (CParticle::KFItem*)GetItemParam(m_wndTree.GetSelectedItem());

		KeyFrameUpdate(DATA_TO_PROP, pFrameItem);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hCollision)
	{
		Ogre::PECollisionFace* pItem = (Ogre::PECollisionFace*)GetItemParam(m_wndTree.GetSelectedItem());
		CollisionFaceUpdate(DATA_TO_PROP, pItem);
	}
	//else if(m_wndTree.GetSelectedItem() == m_hBindBone)
	//{
	//	m_pPropBindBone		->SetValue(_variant_t(pSk->GetDisplayName(m_pTarget->m_bindInfo.boneid).c_str()));
	//}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
			m_wndPropList.GetProperty(i)->SetValue(cvt.IntToString(m_pTarget->m_KeyFrames[i].data.tick));
	}
	m_wndPropList.Invalidate();
}

void CParticleBar::SaveCurrentProp(HTREEITEM hOldItem)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	if(hOldItem == m_hDesc)
	{
		DescUpdate(PROP_TO_DATA);
	}
	else if(m_wndTree.GetParentItem(hOldItem) == m_hKeyFrame)
	{
		CParticle::KFItem* pFrameItem = (CParticle::KFItem*)GetItemParam(hOldItem);
		KeyFrameUpdate(PROP_TO_DATA, pFrameItem);
	}
	else if(m_wndTree.GetParentItem(hOldItem) == m_hCollision)
	{
		Ogre::PECollisionFace* ptem = (Ogre::PECollisionFace*)GetItemParam(hOldItem);
		CollisionFaceUpdate(PROP_TO_DATA, ptem);
	}
	else if(hOldItem == m_hKeyFrame)
	{
		for(int i = 1; i < (int)m_wndPropList.GetPropertyCount(); i++)
		{
			cvt.ParseInt(OLE2T((_bstr_t)m_wndPropList.GetProperty(i)->GetValue()), &m_pTarget->m_KeyFrames[i].data.tick);
		}
	}
}

void CParticleBar::DescUpdate(DATA_UPDATA_TYPE type)
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

	PropUpdate_emittertype((CBCGPProp**)&m_pPropEmitterType, &m_pTarget->m_Desc.m_EmitterType, type, _T("发射器类型"));
	PropUpdate_particletype((CBCGPProp**)&m_pPropParticleType, &m_pTarget->m_Desc.m_ParticleType, type, _T("粒子类型"));
	PropUpdate_int((CBCGPProp**)&m_pPropPreUpdateTime, &m_pTarget->m_PreUpdateTime, type, _T("粒子预发射时间"));

	//PropUpdate_int((CBCGPProp**)&m_pPropFlag, (int*)&m_pTarget->m_Desc.m_Flags, type, _T("标志"));
	PropUpdate_blendtype((CBCGPProp**)&m_pPropBlendType, (Ogre::UIBlendMode*)&m_pTarget->m_Desc.m_BlendType, type, _T("混合模式"));

	PropUpdate_addressmode((CBCGPProp**)&m_pPropAddressMode, &m_pTarget->m_DescEx.m_iUVAddressMode, type, _T("贴图寻址模式"));

	//PropUpdate_v3((CBCGPProp**)&m_pPropPos, &m_pTarget->m_Desc.m_Pos, type, _T("位移"));
	PropUpdate_int((CBCGPProp**)&m_pPropMaxParticle,(int*) &m_pTarget->m_Desc.m_MaxParticles, type, _T("粒子数上限"));

	PropUpdate_float((CBCGPProp**)&m_pPropUVRotate, &m_pTarget->m_DescEx.m_UVRotate, type, _T("UV偏转角度"), 1.0f, 180.0f, -180.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropUVScale, &m_pTarget->m_DescEx.m_UVScale, type, _T("UV缩放比例"), 0.01f, 32767.0f, 0.01f);

	PropUpdate_float((CBCGPProp**)&m_pPropSlowDown, &m_pTarget->m_Desc.m_Slowdown, type, _T("阻力"));
	PropUpdate_float((CBCGPProp**)&m_pPropRotation, &m_pTarget->m_Desc.m_Rotation, type, _T("旋转"));

	PropUpdate_bool((CBCGPProp**)&m_pPropRandomTile, &m_pTarget->m_DescEx.m_bRandomTile, type, _T("随机起始帧"));

	PropUpdate_bool((CBCGPProp**)&m_pPropCollideOnTerrain, &m_pTarget->m_DescExx.m_CollideWithTerrain, type, _T("地表检测"));


	PropUpdate_bool((CBCGPProp**)&m_pPropDistort, &m_pTarget->m_DescEx.m_bDistort, type, _T("空间扭曲模式"));

	PropUpdate_bool((CBCGPProp**)&m_pPropCenterOffsetMode, &m_pTarget->m_DescEx.m_CenterAbsOffset, type, "中心偏移绝对坐标");
	PropUpdate_float((CBCGPProp**)&m_pPropCenterOffsetX, &m_pTarget->m_DescEx.m_CenterOffset.x, type, _T("中心偏移X"), 0.1f, 1000.0f, -1000.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropCenterOffsetY, &m_pTarget->m_DescEx.m_CenterOffset.y, type, _T("中心偏移Y"), 0.1f, 1000.0f, -1000.0f);

	PropUpdate_int((CBCGPProp**)&m_pPropTexRows, &m_pTarget->m_Desc.m_TexRows, type, _T("贴图行数"), 0.05f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropTexCols, &m_pTarget->m_Desc.m_TexCols, type, _T("贴图列数"), 0.05f, 32767, 1);
	PropUpdate_float((CBCGPProp**)&m_pPropTimeSlot, &m_pTarget->m_Desc.m_TimeSlot, type, _T("序列帧间隔"), 0.03f, 32767.0f, 0.0f);

	PropUpdate_color(&m_pPropColorKey1, &m_pTarget->m_Desc.m_ColorKeys[0], type, _T("初始颜色"));
	PropUpdate_color(&m_pPropColorKey2, &m_pTarget->m_Desc.m_ColorKeys[1], type, _T("中点颜色"));
	PropUpdate_color(&m_pPropColorKey3, &m_pTarget->m_Desc.m_ColorKeys[2], type, _T("末端颜色"));

	PropUpdate_float((CBCGPProp**)&m_pPropAlphaKey1, &m_pTarget->m_Desc.m_ColorKeys[0].a, type, _T("初始透明度"));
	PropUpdate_float((CBCGPProp**)&m_pPropAlphaKey2, &m_pTarget->m_Desc.m_ColorKeys[1].a, type, _T("中点透明度"));
	PropUpdate_float((CBCGPProp**)&m_pPropAlphaKey3, &m_pTarget->m_Desc.m_ColorKeys[2].a, type, _T("末端透明度"));

	PropUpdate_float((CBCGPProp**)&m_pPropSizeKey1, &m_pTarget->m_Desc.m_SizeKeys[0], type, _T("初始大小"));
	PropUpdate_float((CBCGPProp**)&m_pPropSizeKey2, &m_pTarget->m_Desc.m_SizeKeys[1], type, _T("中点大小"));
	PropUpdate_float((CBCGPProp**)&m_pPropSizeKey3, &m_pTarget->m_Desc.m_SizeKeys[2], type, _T("末端大小"));

	PropUpdate_float((CBCGPProp**)&m_pPropAspectKey1, &m_pTarget->m_Desc.m_AspectRatioKeys[0], type, _T("初始纵横比"));
	PropUpdate_float((CBCGPProp**)&m_pPropAspectKey2, &m_pTarget->m_Desc.m_AspectRatioKeys[1], type, _T("中点纵横比"));
	PropUpdate_float((CBCGPProp**)&m_pPropAspectKey3, &m_pTarget->m_Desc.m_AspectRatioKeys[2], type, _T("末尾纵横比"));

	PropUpdate_cstring((CBCGPProp**)&m_pPropTexture, &m_pTarget->m_strTextureName, type, _T("纹理地址"));
	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("绑定点"));

	PropUpdate_flag((CBCGPProp**)&m_pPropIsLocal, &m_pTarget->m_Desc.m_Flags, type, _T("相对局部坐标"), Ogre::PEFLAG_LOCAL);
	PropUpdate_flag((CBCGPProp**)&m_pPropCustomEmit, &m_pTarget->m_Desc.m_Flags, type, _T("程序控制发射"), Ogre::PEFLAG_CUSTOM_EMIT );
	PropUpdate_flag((CBCGPProp**)&m_pPropEmitOnce, &m_pTarget->m_Desc.m_Flags, type, _T("单次发射"), Ogre::PEFLAG_EMIT_ONCE);

	PropUpdate_rotstyle((CBCGPProp**)&m_pPropRotStyle, &m_pTarget->m_RotStyle, type, _T("朝向") );

	//PropUpdate_geomv3((CBCGPProp**)&m_pPropScale, &m_pTarget->m_bindInfo.scale, type, _T("等比缩放") , 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropRandomRot, &m_pTarget->m_DescEx.m_fRandomRot, type, _T("随机起始旋转角度") , 1.00f, 360.0f, 0.0f);

	PropUpdate_addressmode((CBCGPProp**)&m_pPropMaskUVAddressMode, &m_pTarget->m_DescEx.m_iMaskUVAddrMode, type, _T("蒙板寻址模式") );
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVRotate, &m_pTarget->m_DescEx.m_MaskUVRotate, type, _T("蒙板UV旋转") , 1.0f, 180.0f, -180.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVScale,	&m_pTarget->m_DescEx.m_MaskUVScale, type, _T("蒙板UV缩放") , 0.01f, 32767.0f, 0.01f);
	PropUpdate_int((CBCGPProp**)&m_pPropMaskRow,		&m_pTarget->m_DescEx.m_MaskTexRows, type, _T("蒙板贴图行数") , 0.05f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropMaskCol,		&m_pTarget->m_DescEx.m_MaskTexCols, type, _T("蒙板贴图列数") , 0.05f, 32767, 1);
	PropUpdate_cstring((CBCGPProp**)&m_pPropMaskTexture,	&m_pTarget->m_strMaskTextureName, type, _T("蒙板贴图地址") );
}

void CParticleBar::KeyFrameUpdate(DATA_UPDATA_TYPE type, CParticle::KFItem* pItem)
{
	PropUpdate_float((CBCGPProp**)&m_pPropSpeed, &pItem->data.speed, type, _T("初速度"));
	PropUpdate_float((CBCGPProp**)&m_pPropSpeedVar, &pItem->data.speedvar, type, _T("初速度误差(%)"));
	PropUpdate_float((CBCGPProp**)&m_pPropSpread, &pItem->data.spread, type, _T("Spread"), 1.0f, 180.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropSpreadOffset, &pItem->data.spreadoffset, type, _T("Spread偏移角度"), 1.0f, 180.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropLatitude, &pItem->data.latitude, type, _T("Latitude"), 1.0f, 180.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropAccelerate, &pItem->data.accelerate, type, _T("加速度"));
	PropUpdate_acc((CBCGPProp**)&m_pPropAccelerateDir, &pItem->data.acceleratedir, type, _T("加速度方向"));
	PropUpdate_float((CBCGPProp**)&m_pPropLifespan, &pItem->data.lifespan, type, _T("生命长度"), 0.05f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropRate, &pItem->data.rate, type, _T("发射率"), 1.0f, 32767.0f, 0.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropDeacceleration, &pItem->data.deacceleration, type, _T("衰减"));
	PropUpdate_int((CBCGPProp**)&m_pPropTick, &pItem->data.tick, type, _T("关键帧时间"));


	PropUpdate_float((CBCGPProp**)&m_pPropSizeVar, &pItem->data.sizevar, type, _T("粒子大小误差(%)"));
	PropUpdate_float((CBCGPProp**)&m_pPropLifeVar, &pItem->data.lifevar, type, _T("生命长度误差(%)"));
	PropUpdate_float((CBCGPProp**)&m_pPropRandomSpread, &pItem->data.randomspread, type, _T("随机扩散角度"));

	if(m_pTarget != NULL && m_pTarget->m_Desc.m_EmitterType == Ogre::EMITTER_COLUMN_UP)
	{
		PropUpdate_float((CBCGPProp**)&m_pPropAreaLenth, &pItem->data.areal, type, _T("区域高度"), 1.0f, 32767.0f, -32767.0f);
		PropUpdate_float((CBCGPProp**)&m_pPropAreaWidth, &pItem->data.areaw, type, _T("区域半径"), 1.0f, 32767.0f, -32767.0f);
		PropUpdate_float((CBCGPProp**)&m_pPropAreaDepth, &pItem->data.aread, type, _T("区域深度(球面发射器有效)"), 0.0f, 1.0f, 0.0f);
	}
	else
	{
		PropUpdate_float((CBCGPProp**)&m_pPropAreaLenth, &pItem->data.areal, type, _T("区域长度"), 1.0f, 32767.0f, 0.0f);
		PropUpdate_float((CBCGPProp**)&m_pPropAreaWidth, &pItem->data.areaw, type, _T("区域宽度"), 1.0f, 32767.0f, 0.0f);
		PropUpdate_float((CBCGPProp**)&m_pPropAreaDepth, &pItem->data.aread, type, _T("区域深度(球面发射器有效)"), 0.0f, 1.0f, 0.0f);
	}
}


void CParticleBar::CollisionFaceUpdate(DATA_UPDATA_TYPE type, Ogre::PECollisionFace* pItem)
{
	PropUpdate_float((CBCGPProp**)&m_pPropCldOffsetX, &pItem->m_Offset.x, type, _T("X"));
	PropUpdate_float((CBCGPProp**)&m_pPropCldOffsetY, &pItem->m_Offset.y, type, _T("Y"));
	PropUpdate_float((CBCGPProp**)&m_pPropCldOffsetZ, &pItem->m_Offset.z, type, _T("Z"));

	PropUpdate_eular_yaw((CBCGPProp**)&m_pPropCldYaw, &pItem->m_Rotation, type, _T("转角"));
	PropUpdate_eular_pitch((CBCGPProp**)&m_pPropCldPitch, &pItem->m_Rotation, type, _T("俯仰角"));
	PropUpdate_eular_roll((CBCGPProp**)&m_pPropCldRoll, &pItem->m_Rotation, type, _T("侧滚角"));

	PropUpdate_int((CBCGPProp**)&m_pPropUpLevel, &pItem->m_iUpLevel, type, _T("相对父节点层数"), 1.0f, 32767, 0);
	PropUpdate_bool((CBCGPProp**)&m_pPropReverseUpLevel, &pItem->m_bReverseUpLevel, type, _T("倒数层数"));
	PropUpdate_float((CBCGPProp**)&m_pPropCldWidth, &pItem->m_fWidth, type, _T("宽度"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropCldLength, &pItem->m_fLength, type, _T("长度"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropCldAtten, &pItem->m_fAttenuation, type, _T("速度衰减"));
}

void CParticleBar::PropUpdate_emittertype(CBCGPProp** ppProp, Ogre::EmitterType* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::EmitterType)cvt.EmitterTypeToValue(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else  if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.ValueToEmitterTypeAsPtr((int)(*data)));
	}
}

void CParticleBar::PropUpdate_rotstyle(CBCGPProp** ppProp, Ogre::ParticleDirType* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::ParticleDirType)cvt.StringToParticleRot(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else  if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.ParticleRotToStringAsPtr(*data));
	}
}

void CParticleBar::PropUpdate_particletype(CBCGPProp** ppProp, Ogre::ParticleDrawType* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::ParticleDrawType)cvt.ParticleTypeToValue(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else  if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.ValueToParticleTypeAsPtr((int)(*data)));
	}
}

void CParticleBar::PropUpdate_acc(CBCGPProp** ppProp, Ogre::Vector3* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""), TRUE, GetSafeHwnd());
	}
	else if(type == PROP_TO_DATA)
	{
		cvt.ParseVector3(OLE2T((_bstr_t)(*ppProp)->GetValue()), data);
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.V3ToString((*data)));
	}
}



void CParticleBar::OnSetFocus(CWnd* pOldWnd)
{
	CEffectEditBar::OnSetFocus(pOldWnd);

	//TRACE("Focus\n");
}

void CParticleBar::OnKillFocus(CWnd* pNewWnd)
{
	CEffectEditBar::OnKillFocus(pNewWnd);

	m_wndPropList.EndEditItem(TRUE);
	TRACE("Particle Kill Focus\n");
}


void CParticleBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CEffectEditBar::OnShowWindow(bShow, nStatus);
	if(bShow == FALSE)
	{
		ReleaseKeyFrameWnd();
	}
	// TODO: Add your message handler code here
}


void CParticleBar::UpdateKeyFrameWnd()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_REQUEST_KFWND, 0, (LPARAM) pObj);
}

void CParticleBar::ReleaseKeyFrameWnd()
{
	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_RELEASE_KFWND, 0, (LPARAM) pObj);
}

void CParticleBar::CopyKeyFrame(int from, int to)
{
	//TRACE("Copy From %d to %d\n", from, to);
	if(from < 0 || from > (int)m_pTarget->m_KeyFrames.size() 
		|| to < 0 || to > (int)m_pTarget->m_KeyFrames.size() )
	{
		return;
	}
	long tick = m_pTarget->m_KeyFrames[to].data.tick;
	m_pTarget->m_KeyFrames[to] = m_pTarget->m_KeyFrames[from];
	m_pTarget->m_KeyFrames[to].data.tick = tick;

	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}

#define PTL_LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))
int CParticleBar::AddKeyFrame(UINT tick)
{
	m_wndPropList.EndEditItem(FALSE);
	CParticle::KFItem kf;
	kf.data.tick = tick;

	int i = 0;
	for(i = 0; i < (int) m_pTarget->m_KeyFrames.size(); i++)
	{
		if(m_pTarget->m_KeyFrames[i].data.tick > kf.data.tick)
			break;
	}

	//m_pTarget->m_KeyFrames.push_back(kf);

	ASSERT( i > 0 && "Add KeyFrame, Never add at first ");
	if(i == (int) m_pTarget->m_KeyFrames.size())
	{
		CParticle::KFItem& last = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1];
		kf.data.speed			= last.data.speed;
		kf.data.speedvar		= last.data.speedvar;
		kf.data.spread			= last.data.spread;
		kf.data.spreadoffset	= last.data.spreadoffset;
		kf.data.latitude		= last.data.latitude;
		kf.data.accelerate		= last.data.accelerate;
		kf.data.acceleratedir	= last.data.acceleratedir;
		kf.data.lifespan		= last.data.lifespan;
		kf.data.rate			= last.data.rate;
		kf.data.areal			= last.data.areal;
		kf.data.areaw			= last.data.areaw;
		kf.data.deacceleration	= last.data.deacceleration;
		kf.data.sizevar			= last.data.sizevar;
		kf.data.lifevar			= last.data.lifevar;

		kf.data.tick = tick;

		m_pTarget->m_KeyFrames.push_back(kf);
	}
	else
	{
		CParticle::KFItem& next = m_pTarget->m_KeyFrames[i];
		CParticle::KFItem& prev = m_pTarget->m_KeyFrames[i - 1];
		float pos = (float)(kf.data.tick - prev.data.tick ) / (next.data.tick - prev.data.tick);

		kf.data.speed			= PTL_LINEAR_LERP(prev.data.speed,		next.data.speed,		pos);
		kf.data.speedvar		= PTL_LINEAR_LERP(prev.data.speedvar,	next.data.speedvar,		pos);
		kf.data.spread			= PTL_LINEAR_LERP(prev.data.spread,		next.data.spread,		pos);
		kf.data.spreadoffset	= PTL_LINEAR_LERP(prev.data.spreadoffset,next.data.spreadoffset,pos);
		kf.data.latitude		= PTL_LINEAR_LERP(prev.data.latitude,	next.data.latitude,		pos);
		kf.data.accelerate		= PTL_LINEAR_LERP(prev.data.accelerate, next.data.accelerate,	pos);
		kf.data.lifespan		= PTL_LINEAR_LERP(prev.data.lifespan,	next.data.lifespan,		pos);
		kf.data.rate			= PTL_LINEAR_LERP(prev.data.rate,		next.data.rate,			pos);
		kf.data.areal			= PTL_LINEAR_LERP(prev.data.areal,		next.data.areal,		pos);
		kf.data.areaw			= PTL_LINEAR_LERP(prev.data.areaw,		next.data.areaw,		pos);
		kf.data.deacceleration	= PTL_LINEAR_LERP(prev.data.deacceleration, next.data.deacceleration, pos);

		kf.data.acceleratedir.x	= PTL_LINEAR_LERP(prev.data.acceleratedir.x, next.data.acceleratedir.x, pos);
		kf.data.acceleratedir.y	= PTL_LINEAR_LERP(prev.data.acceleratedir.y, next.data.acceleratedir.y, pos);
		kf.data.acceleratedir.z	= PTL_LINEAR_LERP(prev.data.acceleratedir.z, next.data.acceleratedir.z, pos);

		kf.data.sizevar			= PTL_LINEAR_LERP(prev.data.sizevar,		next.data.sizevar,		pos);
		kf.data.lifevar			= PTL_LINEAR_LERP(prev.data.lifevar,		next.data.lifevar,		pos);

		Ogre::Normalize(kf.data.acceleratedir);

		kf.data.tick = tick;
		m_pTarget->m_KeyFrames.insert(m_pTarget->m_KeyFrames.begin() + i, kf);
	}
	UpdateTree();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	return i;

}

void CParticleBar::DeleteKeyFrame(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames.erase(m_pTarget->m_KeyFrames.begin() + index);
		UpdateTree();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

void CParticleBar::ActiveKeyFrame(int index)
{
	int i = 0;
	HTREEITEM hCurr = m_wndTree.GetChildItem(m_hKeyFrame);
	while(hCurr != NULL)
	{
		CParticle::KFItem* pFrameItem = (CParticle::KFItem*)GetItemParam(hCurr);
		if(index<m_pTarget->m_KeyFrames.size() && pFrameItem==&m_pTarget->m_KeyFrames[index])
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

void CParticleBar::SetKeyFramePos(int index, UINT tick)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames[index].data.tick = tick;
		UpdateList();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

int CParticleBar::GetKeyFrameCount()
{
	return (int)m_pTarget->m_KeyFrames.size();
}

UINT CParticleBar::GetKeyFrameTick(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		return (UINT)m_pTarget->m_KeyFrames[index].data.tick;
	}
	return -1;
}

CString CParticleBar::GetName()
{
	return m_pTarget->GetName();
}
