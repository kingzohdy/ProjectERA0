
#include "stdafx.h"
#include "EffectUnitEditBar_Billboard.h"
#include "SplitterGridView.h"
#include "EffectManager.h"
#include "NotifyProperty.h"
#include "ObjectTypeHelper.h"

extern bool IsTextureResource(const char *path);

IMPLEMENT_DYNAMIC(CBillboardBar, CEffectEditBar)
CBillboardBar::CBillboardBar() : m_pTarget(NULL), m_bLockApp(true)
{
	m_pPropOffsetX = NULL;
	m_pPropOffsetY = NULL;
	m_pPropOffsetZ = NULL;
	m_pPropType = NULL;
	m_pPropCenterX = NULL;
	m_pPropCenterY = NULL;
	m_pPropCenterZ = NULL;
	m_pPropBlendType = NULL;
	m_pPropPos = NULL;
	m_pPropMidPoint = NULL;
	m_pPropTexRows = NULL;
	m_pPropTexCols = NULL;
	m_pPropTimeSlot = NULL;
	m_pPropAspectKey1 = NULL;
	m_pPropAspectKey2 = NULL;
	m_pPropAspectKey3 = NULL;
	m_pPropSizeKey1 = NULL;
	m_pPropSizeKey2 = NULL;
	m_pPropSizeKey3 = NULL;


	m_pPropColorKey1 = NULL;
	m_pPropColorKey2 = NULL;
	m_pPropColorKey3 = NULL;

	m_pPropAlphaKey1 = NULL;
	m_pPropAlphaKey2 = NULL;
	m_pPropAlphaKey3 = NULL;

	m_pPropTexture = NULL;
	m_pPropBindBone = NULL;
	m_pPropColorBtn = NULL;
	m_pPropAlphaBtn = NULL;
	m_pPropSizeBtn = NULL;
	m_pPropAspectBtn = NULL;

	m_pPropYaw = NULL;
	m_pPropPitch = NULL;
	m_pPropRoll = NULL;
	m_pPropDistort = NULL;
	m_pPropAddressMode = NULL;
	m_pPropCenterOffsetMode = NULL;
	m_pPropCenterOffsetX = NULL;
	m_pPropCenterOffsetY = NULL;


	m_pPropMaskUVAddressMode = NULL;
	m_pPropMaskTexCols = NULL;
	m_pPropMaskTexRows = NULL;
	m_pPropMaskTexture = NULL;

	m_pPropMaskUVRot = NULL;
	m_pPropMaskUScale = NULL;
	m_pPropMaskVScale = NULL;
	m_pPropMaskUOffset = NULL;
	m_pPropMaskVOffset = NULL;
	m_pPropMaskSeqFrame = NULL;

	//m_pPropScale = NULL;

}

CBillboardBar::~CBillboardBar()
{
	ClearProps();
	//DestroyWindow();
}



BEGIN_MESSAGE_MAP(CBillboardBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	//ON_COMMAND(ID_BTN_BLB_VISIBLE, OnVisible)
	ON_UPDATE_COMMAND_UI(ID_BTN_BLB_VISIBLE, OnUpdateVisible)
	ON_COMMAND(ID_BTN_BLB_APPLY, OnApply)
	ON_COMMAND(ID_BTN_BLB_SAVE, OnSave)
	ON_COMMAND(ID_BTN_BLB_REMOVE, OnRemove)
	ON_COMMAND(ID_BTN_BLB_CLICK, OnBtnClick)

	ON_COMMAND(ID_BTN_BLB_NEWFRAME, OnNewFrame)
	ON_COMMAND(ID_BTN_BLB_DELFRAME, OnDeleteFrame)

	ON_NOTIFY(TVN_SELCHANGEDA, ID_BILLBOARDTREE, OnTreeSelChanged)
	ON_COMMAND(ID_BTN_BLB_LOCKAPPLY, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_BLB_LOCKAPPLY, OnUpdateBtnLockApply)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	//ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickPropBtn)

	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()



// CBillboardBar message handlers


int CBillboardBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this)
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_BILLBOARD))
	{
		return -1;
	}

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
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (0, 150),&context1);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height () / 2),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	CSplitterGridView* m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	CSplitterGridView* m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	m_wndTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS , 
		CRect(0, 0, 100, 100), m_pGrid1, ID_BILLBOARDTREE);
	m_hDesc = m_wndTree.InsertItem(_T("公告板参数"));
	m_hKeyFrame = m_wndTree.InsertItem(_T("关键帧"));


	m_pGrid1->SetInnerCtrl(&m_wndTree);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_BILLBOARDEPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_pGrid2->SetInnerCtrl(&m_wndPropList);
	m_wndSplitter.ShowWindow(SW_SHOW);


	//CreateProp();
	//UpdateTree();

	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	return 0;
}

void CBillboardBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CBillboardBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->m_bVisible ? 1 : 0);
}

void CBillboardBar::OnVisible()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg(EEM_APPLY);
}

void CBillboardBar::OnApply()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	//m_pTarget->RefreshNode();
}

void CBillboardBar::OnBtnClick()
{
	m_pTarget->GetInteract()->OnLBtnDown(0.0f, 0.0f, true);
	CEffectMgr::GetInstance()->SetActiveEffect(m_pTarget);
}

void CBillboardBar::OnSave()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	SendMainFrameMsg( EEM_SAVE, 0, (LPARAM)m_pTarget);
}

void CBillboardBar::OnRemove()
{
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CBillboardBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CBillboardBar::OnNewFrame()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	m_wndPropList.EndEditItem(TRUE);
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	CBillboard::BIL_FRAME item;
	if(m_pTarget->m_KeyFrames.size() > 0)
		item.lTick = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1].lTick + 1000;

	m_pTarget->m_KeyFrames.push_back(item);

	UpdateTree();
	m_pTarget->UpdateSource();
	//UpdateKeyFrameWnd();
}

void CBillboardBar::OnDeleteFrame()
{
	//if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		//return;

	if (m_pTarget->m_KeyFrames.size() == 1)
	{
		PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
	}
	else if (m_pTarget->m_KeyFrames.size() > 1)
	{
		m_wndPropList.EndEditItem(TRUE);
		SaveCurrentProp(m_wndTree.GetSelectedItem());
		HTREEITEM hSelected = m_wndTree.GetSelectedItem();

		if(m_wndTree.GetParentItem(hSelected) != m_hKeyFrame)
			return ;

		CBillboard::BIL_FRAME* pFrameItem = (CBillboard::BIL_FRAME*)GetItemParam(hSelected);

		std::vector<CBillboard::BIL_FRAME>::iterator it = m_pTarget->m_KeyFrames.begin();
		for(; it != m_pTarget->m_KeyFrames.end(); it++)
		{
			if(&(*it) == pFrameItem)
			{
				m_pTarget->m_KeyFrames.erase(it);
				UpdateTree();
				m_pTarget->UpdateSource();
				CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
				m_wndTree.SelectItem(m_hKeyFrame);
				break;
			}
		}
	}
	//UpdateKeyFrameWnd();
}



void CBillboardBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CBillboardBar::OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_hLastSel = pNm->itemOld.hItem;
	m_wndPropList.EndEditItem(TRUE);
	//SaveCurrentProp(pNm->itemOld.hItem);
	CreateProp();
	m_hLastSel = NULL;
}


LRESULT CBillboardBar::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	if(pdn->hWnd == m_wndPropList.GetSafeHwnd())
	{
		if(!IsTextureResource(pdn->strChunk))
		{
			return 0;
		}

		if(m_wndTree.GetSelectedItem() == m_hDesc )
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

LRESULT CBillboardBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	//if(pnm->pProp == m_pPropColor)
	//	return 0;
	if(m_hLastSel != NULL)
		SaveCurrentProp(m_hLastSel);
	else
		SaveCurrentProp(m_wndTree.GetSelectedItem());
	//SaveCurrentProp(m_wndTree.GetSelectedItem());


	if(m_bLockApp && 
		(//pnm->pProp == m_pPropMaxParticle 
		pnm->pProp == m_pPropTexture
		|| pnm->pProp == m_pPropBindBone
		|| pnm->pProp == m_pPropBlendType
		|| pnm->pProp == m_pPropDistort
		|| pnm->pProp == m_pPropCenterOffsetMode
		|| pnm->pProp == m_pPropCenterOffsetX
		|| pnm->pProp == m_pPropCenterOffsetY
		|| pnm->pProp == m_pPropAddressMode	
		|| pnm->pProp == m_pPropMaskUVAddressMode	
		|| pnm->pProp == m_pPropMaskTexture
		))
	{
		//m_pTarget->RefreshNode();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
	m_pTarget->UpdateSource();
	return 0;
}

LRESULT CBillboardBar::OnClickPropBtn(WPARAM w, LPARAM l)
{
	PROPNM* pnm = (PROPNM*)l;
	CCvtHelper cvt;

	//if(pnm->pProp == m_pPropColorBtn)
	//{
	//	CColorKeyDlg ckdlg;
	//	ckdlg.m_fMidPoint = m_pTarget->m_descEx.m_ColorMid;
	//	ckdlg.m_color1 = cvt.ColorFloatToInt(m_pTarget->m_desc.m_ColorKeys[0]);
	//	ckdlg.m_color2 = cvt.ColorFloatToInt(m_pTarget->m_desc.m_ColorKeys[1]);
	//	ckdlg.m_color3 = cvt.ColorFloatToInt(m_pTarget->m_desc.m_ColorKeys[2]);

	//	if(IDOK == ckdlg.DoModal())
	//	{
	//		CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	//		m_pTarget->m_descEx.m_ColorMid = ckdlg.m_fMidPoint;
	//		m_pTarget->m_desc.m_ColorKeys[0].r = cvt.ColorIntToFloat(ckdlg.m_color1).r;
	//		m_pTarget->m_desc.m_ColorKeys[0].g = cvt.ColorIntToFloat(ckdlg.m_color1).g;
	//		m_pTarget->m_desc.m_ColorKeys[0].b = cvt.ColorIntToFloat(ckdlg.m_color1).b;

	//		m_pTarget->m_desc.m_ColorKeys[1].r = cvt.ColorIntToFloat(ckdlg.m_color2).r;
	//		m_pTarget->m_desc.m_ColorKeys[1].g = cvt.ColorIntToFloat(ckdlg.m_color2).g;
	//		m_pTarget->m_desc.m_ColorKeys[1].b = cvt.ColorIntToFloat(ckdlg.m_color2).b;

	//		m_pTarget->m_desc.m_ColorKeys[2].r = cvt.ColorIntToFloat(ckdlg.m_color3).r;
	//		m_pTarget->m_desc.m_ColorKeys[2].g = cvt.ColorIntToFloat(ckdlg.m_color3).g;
	//		m_pTarget->m_desc.m_ColorKeys[2].b = cvt.ColorIntToFloat(ckdlg.m_color3).b;

	//		m_pTarget->EndRecordCommand(pCmd);
	//		m_pTarget->UpdateSource();

	//		UpdateList();
	//	}
	//}
	//else if(pnm->pProp == m_pPropAlphaBtn)
	//{
	//	CFloatKeyDlg fkdlg;
	//	fkdlg.m_fMidPoint = m_pTarget->m_descEx.m_ColorMid;
	//	fkdlg.m_strValueName = _T("透明度");
	//	fkdlg.m_fMin = 0.0f;
	//	fkdlg.m_fMax = 1.0f;
	//	fkdlg.m_fValue1 = m_pTarget->m_desc.m_ColorKeys[0].a;
	//	fkdlg.m_fValue2 = m_pTarget->m_desc.m_ColorKeys[1].a;
	//	fkdlg.m_fValue3 = m_pTarget->m_desc.m_ColorKeys[2].a;

	//	if(IDOK == fkdlg.DoModal())
	//	{
	//		CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	//		m_pTarget->m_descEx.m_ColorMid = fkdlg.m_fMidPoint;
	//		m_pTarget->m_desc.m_ColorKeys[0].a = fkdlg.m_fValue1;
	//		m_pTarget->m_desc.m_ColorKeys[1].a = fkdlg.m_fValue2;
	//		m_pTarget->m_desc.m_ColorKeys[2].a = fkdlg.m_fValue3;

	//		m_pTarget->EndRecordCommand(pCmd);
	//		m_pTarget->UpdateSource();

	//		UpdateList();
	//	}
	//}
	//else if(pnm->pProp == m_pPropSizeBtn)
	//{
	//	CFloatKeyDlg fkdlg;
	//	fkdlg.m_fMidPoint = m_pTarget->m_descEx.m_SizeMid;
	//	fkdlg.m_strValueName = _T("公告板大小");
	//	fkdlg.m_fMin = 0.0f;
	//	fkdlg.m_fMax = 100.0f;
	//	fkdlg.m_fValue1 = m_pTarget->m_desc.m_SizeKeys[0];
	//	fkdlg.m_fValue2 = m_pTarget->m_desc.m_SizeKeys[1];
	//	fkdlg.m_fValue3 = m_pTarget->m_desc.m_SizeKeys[2];

	//	if(IDOK == fkdlg.DoModal())
	//	{
	//		CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	//		m_pTarget->m_descEx.m_SizeMid = fkdlg.m_fMidPoint;
	//		m_pTarget->m_desc.m_SizeKeys[0] = fkdlg.m_fValue1;
	//		m_pTarget->m_desc.m_SizeKeys[1] = fkdlg.m_fValue2;
	//		m_pTarget->m_desc.m_SizeKeys[2] = fkdlg.m_fValue3;

	//		m_pTarget->EndRecordCommand(pCmd);
	//		m_pTarget->UpdateSource();

	//		UpdateList();
	//	}
	//}
	//else if(pnm->pProp == m_pPropAspectBtn)
	//{
	//	CFloatKeyDlg fkdlg;
	//	fkdlg.m_fMidPoint = m_pTarget->m_descEx.m_AspectMid;
	//	fkdlg.m_strValueName = _T("纵横比");
	//	fkdlg.m_fMin = 0.1f;
	//	fkdlg.m_fMax = 10.0f;
	//	fkdlg.m_fValue1 = m_pTarget->m_desc.m_AspectRatioKeys[0];
	//	fkdlg.m_fValue2 = m_pTarget->m_desc.m_AspectRatioKeys[1];
	//	fkdlg.m_fValue3 = m_pTarget->m_desc.m_AspectRatioKeys[2];
	//	fkdlg.m_Mode = FKD_LOG;

	//	if(IDOK == fkdlg.DoModal())
	//	{
	//		CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	//		m_pTarget->m_descEx.m_AspectMid = fkdlg.m_fMidPoint;
	//		m_pTarget->m_desc.m_AspectRatioKeys[0] = fkdlg.m_fValue1;
	//		m_pTarget->m_desc.m_AspectRatioKeys[1] = fkdlg.m_fValue2;
	//		m_pTarget->m_desc.m_AspectRatioKeys[2] = fkdlg.m_fValue3;

	//		m_pTarget->EndRecordCommand(pCmd);
	//		m_pTarget->UpdateSource();

	//		UpdateList();
	//	}
	//}
	return 0;
}

void CBillboardBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CBillboard*)pEffect;
	if(GetSafeHwnd())
	{
		UpdateTree();
		UpdateList();

		SetWindowText(m_pTarget->m_strName.GetBuffer());
	}

}

CEffectUnit* CBillboardBar::GetEditTarget()
{
	return m_pTarget;
}

void CBillboardBar::Increase(int step)
{
	//DataUpdate(INC_DATA_RELATED);
	//m_pTarget->UpdateSource();
}

void CBillboardBar::Decrease(int step)
{
	//DataUpdate(DEC_DATA_RELATED);
	//m_pTarget->UpdateSource();
}

void CBillboardBar::CreateProp()
{
	m_wndPropList.EndEditItem(TRUE);

	ClearProps();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		CreateDescProp();
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CBillboard::BIL_FRAME* pFrameItem = (CBillboard::BIL_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		CreateKeyFrameProp(pFrameItem);
		if(pFrameItem == &m_pTarget->m_KeyFrames[0])
		{
			m_pPropTick->Enable(FALSE);
		}
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


void CBillboardBar::CreateKeyFrameProp(CBillboard::BIL_FRAME* pFrameItem)
{
	KeyFrameUpdate(CREATE_PROP, pFrameItem);
	m_pPropUVRot->SetNotifyTarget(GetSafeHwnd());
	m_pPropUScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropVScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropUOffset->SetNotifyTarget(GetSafeHwnd());
	m_pPropVOffset->SetNotifyTarget(GetSafeHwnd());
	m_pPropSeqFrame->SetNotifyTarget(GetSafeHwnd());
	m_pPropColor->SetNotifyTarget(GetSafeHwnd());
	m_pPropAlpha->SetNotifyTarget(GetSafeHwnd());
	m_pPropSize->SetNotifyTarget(GetSafeHwnd());
	m_pPropAspect->SetNotifyTarget(GetSafeHwnd());

	// 104
	m_pPropSizeX->SetNotifyTarget(GetSafeHwnd());
	m_pPropSizeY->SetNotifyTarget(GetSafeHwnd());

	m_pPropTick->SetNotifyTarget(GetSafeHwnd());

	m_pPropMaskUVRot->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskUScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskVScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskUOffset->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskVOffset->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskSeqFrame->SetNotifyTarget(GetSafeHwnd());

	CBCGPProp* pTex = new CBCGPProp("公告板贴图");
	//CBCGPProp* pUVScale = new CBCGPProp("UV缩放");
	pTex->AddSubItem(m_pPropUScale);
	pTex->AddSubItem(m_pPropVScale);
	//CBCGPProp* pUVOffset = new CBCGPProp("UV偏移");
	pTex->AddSubItem(m_pPropUOffset);
	pTex->AddSubItem(m_pPropVOffset);
	pTex->AddSubItem(m_pPropSeqFrame);
	pTex->AddSubItem(m_pPropUVRot);
	//pTex->AddSubItem(pUVScale);
	//pTex->AddSubItem(pUVOffset);

	CBCGPProp* pMaskTex = new CBCGPProp("蒙板贴图");
	//CBCGPProp* pMaskUVScale = new CBCGPProp("UV缩放");
	pMaskTex->AddSubItem(m_pPropMaskUScale);
	pMaskTex->AddSubItem(m_pPropMaskVScale);
	//CBCGPProp* pMaskUVOffset = new CBCGPProp("UV偏移");
	pMaskTex->AddSubItem(m_pPropMaskUOffset);
	pMaskTex->AddSubItem(m_pPropMaskVOffset);
	pMaskTex->AddSubItem(m_pPropMaskSeqFrame);
	pMaskTex->AddSubItem(m_pPropMaskUVRot);
	//pMaskTex->AddSubItem(pMaskUVScale);
	//pMaskTex->AddSubItem(pMaskUVOffset);


	m_wndPropList.AddProperty(pTex);
	m_wndPropList.AddProperty(pMaskTex);

	m_wndPropList.AddProperty(m_pPropColor);
	m_wndPropList.AddProperty(m_pPropAlpha);
	m_wndPropList.AddProperty(m_pPropSize);
	m_wndPropList.AddProperty(m_pPropAspect);

	// 104
	m_wndPropList.AddProperty(m_pPropSizeX);
	m_wndPropList.AddProperty(m_pPropSizeY);

	m_wndPropList.AddProperty(m_pPropTick);

}


void CBillboardBar::CreateDescProp()
{

	DescUpdate(CREATE_PROP);

	m_pPropType->AddOption(_T("面对观察点"));
	m_pPropType->AddOption(_T("绕竖直旋转"));
	m_pPropType->AddOption(_T("面对上方"));
	m_pPropType->AddOption(_T("简单面片"));

	//m_pPropColorBtn		= new CNotifiedProp(_T("颜色调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
	//m_pPropAlphaBtn		= new CNotifiedProp(_T("Alpha调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
	//m_pPropSizeBtn		= new CNotifiedProp(_T("大小调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
	//m_pPropAspectBtn	= new CNotifiedProp(_T("纵横比调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());


	//CBCGPProp* pPos = new CBCGPProp(_T("相对位置"));
	//pPos->AddSubItem(m_pPropOffsetX);
	//pPos->AddSubItem(m_pPropOffsetY);
	//pPos->AddSubItem(m_pPropOffsetZ);

	//CBCGPProp* pSize = new CBCGPProp(_T("大小变化"));
	//pSize->AddSubItem(m_pPropSizeBtn);

	//CBCGPProp* pAspect = new CBCGPProp(_T("纵横比变化"));
	//pSize->AddSubItem(m_pPropAspectBtn);


	//CBCGPProp* pColor = new CBCGPProp(_T("颜色变化"));
	//pColor->AddSubItem(m_pPropColorBtn);
	//pColor->AddSubItem(m_pPropAlphaBtn);

	//CBCGPProp* pRot = new CBCGPProp(_T("旋转"));
	//pRot->AddSubItem(m_pPropYaw);
	//pRot->AddSubItem(m_pPropPitch);
	//pRot->AddSubItem(m_pPropRoll);

	//CBCGPProp* pScale = new CBCGPProp(_T("缩放"));
	//pScale->AddSubItem(m_pPropScaleX);
	//pScale->AddSubItem(m_pPropScaleY);
	//pScale->AddSubItem(m_pPropScaleZ);


	m_pPropType->SetNotifyTarget(GetSafeHwnd());
	//m_pPropOffsetX->SetNotifyTarget(GetSafeHwnd());
	//m_pPropOffsetY->SetNotifyTarget(GetSafeHwnd());
	//m_pPropOffsetZ->SetNotifyTarget(GetSafeHwnd());
	//m_pPropYaw->SetNotifyTarget(GetSafeHwnd());
	//m_pPropPitch->SetNotifyTarget(GetSafeHwnd());
	//m_pPropRoll->SetNotifyTarget(GetSafeHwnd());
	//
	//m_pPropScaleX->SetNotifyTarget(GetSafeHwnd());
	//m_pPropScaleY->SetNotifyTarget(GetSafeHwnd());
	//m_pPropScaleZ->SetNotifyTarget(GetSafeHwnd());

	//m_pPropUVRotate->SetNotifyTarget(GetSafeHwnd());
	//m_pPropUVScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropDistort->SetNotifyTarget(GetSafeHwnd());
	m_pPropCenterOffsetMode->SetNotifyTarget(GetSafeHwnd());
	m_pPropCenterOffsetX->SetNotifyTarget(GetSafeHwnd());
	m_pPropCenterOffsetY->SetNotifyTarget(GetSafeHwnd());


	//m_pPropMidPoint->SetNotifyTarget(GetSafeHwnd());
	m_pPropTexRows->SetNotifyTarget(GetSafeHwnd());
	m_pPropTexCols->SetNotifyTarget(GetSafeHwnd());
	m_pPropTimeSlot->SetNotifyTarget(GetSafeHwnd());
	m_pPropAddressMode->SetNotifyTarget(GetSafeHwnd());
	//m_pPropAspectKey1->SetNotifyTarget(GetSafeHwnd());
	//m_pPropAspectKey2->SetNotifyTarget(GetSafeHwnd());
	//m_pPropAspectKey3->SetNotifyTarget(GetSafeHwnd());
	//m_pPropSizeKey1->SetNotifyTarget(GetSafeHwnd());
	//m_pPropSizeKey2->SetNotifyTarget(GetSafeHwnd());
	//m_pPropSizeKey3->SetNotifyTarget(GetSafeHwnd());
	//m_pPropAlphaKey1->SetNotifyTarget(GetSafeHwnd());
	//m_pPropAlphaKey2->SetNotifyTarget(GetSafeHwnd());
	//m_pPropAlphaKey3->SetNotifyTarget(GetSafeHwnd());
	m_pPropTexture->SetNotifyTarget(GetSafeHwnd());
	m_pPropBindBone->SetNotifyTarget(GetSafeHwnd());
	m_pPropBlendType->SetNotifyTarget(GetSafeHwnd());
	m_pPropFrameType->SetNotifyTarget(GetSafeHwnd());
	//m_pPropScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropXYDepend->SetNotifyTarget(GetSafeHwnd());

	//	m_pPropMidPoint->SetFloatDigitRatio(0.01f);
	m_pPropTexRows->SetFloatDigitRatio(0.05f);
	m_pPropTexCols->SetFloatDigitRatio(0.05f);
//	m_pPropFrameType->SetNotifyTarget(GetSafeHwnd());

	m_pPropMaskUVAddressMode->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskTexCols->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskTexRows->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskTexture->SetNotifyTarget(GetSafeHwnd());


	CBCGPProp* pTex = new CBCGPProp("公告板贴图");
	pTex->AddSubItem(m_pPropAddressMode);
	pTex->AddSubItem(m_pPropTexRows);
	pTex->AddSubItem(m_pPropTexCols);
	pTex->AddSubItem(m_pPropTexture);


	CBCGPProp* pMaskTex = new CBCGPProp("蒙板贴图");
	pMaskTex->AddSubItem(m_pPropMaskUVAddressMode);
	pMaskTex->AddSubItem(m_pPropMaskTexRows);
	pMaskTex->AddSubItem(m_pPropMaskTexCols);
	pMaskTex->AddSubItem(m_pPropMaskTexture);

	m_wndPropList.AddProperty(m_pPropType);
	m_wndPropList.AddProperty(m_pPropBlendType);
	m_wndPropList.AddProperty(m_pPropDistort);

	m_wndPropList.AddProperty(m_pPropCenterOffsetMode);
	m_wndPropList.AddProperty(m_pPropCenterOffsetX);
	m_wndPropList.AddProperty(m_pPropCenterOffsetY);

	m_wndPropList.AddProperty(m_pPropTimeSlot);
	m_wndPropList.AddProperty(m_pPropFrameType);
	m_wndPropList.AddProperty(m_pPropXYDepend); // 104
	m_wndPropList.AddProperty(pTex);
	m_wndPropList.AddProperty(pMaskTex);
	m_wndPropList.AddProperty(m_pPropBindBone);
}

void CBillboardBar::SaveCurrentProp(HTREEITEM hOldItem)
{
	//m_wndPropList.EndEditItem(FALSE);
	//CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
	//DataUpdate(PROP_TO_DATA);
	//m_pTarget->EndRecordCommand(pCmd);

	CCvtHelper cvt;
	USES_CONVERSION;
	m_wndPropList.EndEditItem(FALSE);

	CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();

	CSkeleton* pSk = CEffectMgr::GetInstance()->GetSkeleton();
	CBillboard::BIL_FRAME* pFrameItem;
	if(hOldItem == m_hDesc)
	{
		DescUpdate(PROP_TO_DATA);

	}
	else if(m_wndTree.GetParentItem(hOldItem) == m_hKeyFrame)		// 关键帧
	{
		pFrameItem = (CBillboard::BIL_FRAME*)GetItemParam(hOldItem);
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

void CBillboardBar::UpdateTree()
{
	if(m_pTarget == NULL)
		return;

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

void CBillboardBar::UpdateList()
{
	//DataUpdate(DATA_TO_PROP);
	CCvtHelper cvt;

	if(m_wndTree.GetSelectedItem() == m_hDesc)// 参数树
	{
		DescUpdate(DATA_TO_PROP);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)// 上级Item为关键帧
	{
		CBillboard::BIL_FRAME* pFrameItem = (CBillboard::BIL_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		KeyFrameUpdate(DATA_TO_PROP, pFrameItem);
	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)// 关键帧
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
			m_wndPropList.GetProperty(i)->SetValue(cvt.IntToString(m_pTarget->m_KeyFrames[i].lTick));
	}
	m_wndPropList.Invalidate();
}

void CBillboardBar::ClearProps()
{
	if(m_wndPropList.GetSafeHwnd())
		m_wndPropList.RemoveAll();

	//m_pPropOffsetX = NULL;
	//m_pPropOffsetY = NULL;
	//m_pPropOffsetZ = NULL;
	m_pPropType = NULL;
	//m_pPropCenterX = NULL;
	//m_pPropCenterY = NULL;
	//m_pPropCenterZ = NULL;
	m_pPropBlendType = NULL;
	m_pPropPos = NULL;
	m_pPropMidPoint = NULL;
	m_pPropTexRows = NULL;
	m_pPropTexCols = NULL;
	m_pPropTimeSlot = NULL;

	if(m_pPropSizeKey1)  delete(m_pPropSizeKey1);
	if(m_pPropSizeKey2)  delete(m_pPropSizeKey2);
	if(m_pPropSizeKey3)  delete(m_pPropSizeKey3);


	if(m_pPropColorKey1) delete(m_pPropColorKey1);
	if(m_pPropColorKey2) delete(m_pPropColorKey2);
	if(m_pPropColorKey3) delete(m_pPropColorKey3);

	if(m_pPropAlphaKey1) delete(m_pPropAlphaKey1);
	if(m_pPropAlphaKey2) delete(m_pPropAlphaKey2);
	if(m_pPropAlphaKey3) delete(m_pPropAlphaKey3);

	if(m_pPropAspectKey1) delete(m_pPropAspectKey1);
	if(m_pPropAspectKey2) delete(m_pPropAspectKey2);
	if(m_pPropAspectKey3) delete(m_pPropAspectKey3);

	m_pPropTexture = NULL;
	m_pPropBindBone = NULL;
	m_pPropColorBtn = NULL;
	m_pPropAlphaBtn = NULL;
	m_pPropSizeBtn = NULL;
	m_pPropAspectBtn = NULL;

	//m_pPropYaw = NULL;
	//m_pPropPitch = NULL;
	//m_pPropRoll = NULL;
	m_pPropDistort = NULL;
	m_pPropCenterOffsetMode = NULL;
	m_pPropCenterOffsetX = NULL;
	m_pPropCenterOffsetY = NULL;
	m_pPropAddressMode = NULL;

	m_pPropMaskUVRot = NULL;
	m_pPropMaskUScale = NULL;
	m_pPropMaskVScale = NULL;
	m_pPropMaskUOffset = NULL;
	m_pPropMaskVOffset = NULL;
	m_pPropMaskSeqFrame = NULL;
	m_pPropMaskUVAddressMode = NULL;
	m_pPropMaskTexCols = NULL;
	m_pPropMaskTexRows = NULL;
	m_pPropMaskTexture = NULL;


}

void CBillboardBar::DescUpdate(DATA_UPDATA_TYPE type)
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
	//PropUpdate_eular_yaw  ((CBCGPProp**)&m_pPropYaw, &m_pTarget->m_bindInfo.rotate, type, _T("转角"));
	//PropUpdate_eular_pitch((CBCGPProp**)&m_pPropPitch, &m_pTarget->m_bindInfo.rotate, type, _T("俯仰角"));
	//PropUpdate_eular_roll ((CBCGPProp**)&m_pPropRoll, &m_pTarget->m_bindInfo.rotate, type, _T("侧滚角"));

	//PropUpdate_rot_x ((CBCGPProp**)&m_pPropYaw, &m_pTarget->m_bindInfo.rotate, type, _T("转角"));
	//PropUpdate_rot_y ((CBCGPProp**)&m_pPropPitch, &m_pTarget->m_bindInfo.rotate, type, _T("俯仰角"));
	//PropUpdate_rot_z ((CBCGPProp**)&m_pPropRoll, &m_pTarget->m_bindInfo.rotate, type, _T("侧滚角"));
	//PropUpdate_rot((CBCGPProp**)&m_pPropYaw, (CBCGPProp**)&m_pPropPitch,(CBCGPProp**)&m_pPropRoll, &m_pTarget->m_bindInfo.rotate, type, _T("旋转"));
	//}
	PropUpdate_facemode((CBCGPProp**)&m_pPropType, &m_pTarget->m_desc.m_iType, type, "朝向");
	PropUpdate_blendtype((CBCGPProp**)&m_pPropBlendType, (Ogre::UIBlendMode*)&m_pTarget->m_desc.m_BlendType, type, _T("混合模式"));
	PropUpdate_addressmode((CBCGPProp**)&m_pPropAddressMode, &m_pTarget->m_descEx.m_iUVAddressMode, type, _T("贴图寻址模式"));
	PropUpdate_bool((CBCGPProp**)&m_pPropDistort, &m_pTarget->m_descEx.m_bDistort, type, "空间扭曲模式");

	PropUpdate_bool((CBCGPProp**)&m_pPropCenterOffsetMode, &m_pTarget->m_descEx.m_CenterAbsOffset, type, "中心偏移绝对坐标");
	PropUpdate_float((CBCGPProp**)&m_pPropCenterOffsetX, &m_pTarget->m_descEx.m_CenterOffset.x, type, _T("中心偏移X"), 0.1f, 1000.0f, -1000.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropCenterOffsetY, &m_pTarget->m_descEx.m_CenterOffset.y, type, _T("中心偏移Y"), 0.1f, 1000.0f, -1000.0f);

	//PropUpdate_float(&m_pPropCenterX, &m_pTarget->m_desc.m_Center.x, type, _T("X"));
	//PropUpdate_float(&m_pPropCenterY, &m_pTarget->m_desc.m_Center.y, type, _T("Y"));
	//PropUpdate_float(&m_pPropCenterZ, &m_pTarget->m_desc.m_Center.z, type, _T("Z"));

	//PropUpdate_v3((CBCGPProp**)&m_pPropPos, &m_pTarget->m_desc.m_Pos, type, _T("位移"));
	//PropUpdate_float((CBCGPProp**)&m_pPropMidPoint, &m_pTarget->m_desc.m_MidPoint, type, _T("中点"), 0.01f, 1.0f, 0.0f);
	PropUpdate_int((CBCGPProp**)&m_pPropTexRows, &m_pTarget->m_desc.m_TexRows, type, _T("贴图行数"), 0.05f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropTexCols, &m_pTarget->m_desc.m_TexCols, type, _T("贴图列数"), 0.05f, 32767, 1);
	PropUpdate_float((CBCGPProp**)&m_pPropTimeSlot, &m_pTarget->m_desc.m_TimeSlot, type, _T("帧间隔"), 0.03f, 32767.0f, 0.0f);

	//PropUpdate_float((CBCGPProp**)&m_pPropUVRotate, &m_pTarget->m_descEx.m_UVRotate, type, _T("UV偏转角度"), 1.0f, 180.0f, -180.0f);
	//PropUpdate_float((CBCGPProp**)&m_pPropUVScale, &m_pTarget->m_descEx.m_UVScale, type, _T("UV缩放比例"), 0.01f, 32767.0f, 0.01f);


	//PropUpdate_float((CBCGPProp**)&m_pPropAspectKey1, &m_pTarget->m_desc.m_AspectRatioKeys[0], type, _T("初始纵横比"));
	//PropUpdate_float((CBCGPProp**)&m_pPropAspectKey2, &m_pTarget->m_desc.m_AspectRatioKeys[1], type, _T("中点纵横比"));
	//PropUpdate_float((CBCGPProp**)&m_pPropAspectKey3, &m_pTarget->m_desc.m_AspectRatioKeys[2], type, _T("末尾纵横比"));
	//
	//PropUpdate_float((CBCGPProp**)&m_pPropSizeKey1, &m_pTarget->m_desc.m_SizeKeys[0], type, _T("初始大小"));
	//PropUpdate_float((CBCGPProp**)&m_pPropSizeKey2, &m_pTarget->m_desc.m_SizeKeys[1], type, _T("中点大小"));
	//PropUpdate_float((CBCGPProp**)&m_pPropSizeKey3, &m_pTarget->m_desc.m_SizeKeys[2], type, _T("末尾大小"));

	//PropUpdate_color(&m_pPropColorKey1, &m_pTarget->m_desc.m_ColorKeys[0], type, _T("初始颜色"));
	//PropUpdate_color(&m_pPropColorKey2, &m_pTarget->m_desc.m_ColorKeys[1], type, _T("中点颜色"));
	//PropUpdate_color(&m_pPropColorKey3, &m_pTarget->m_desc.m_ColorKeys[2], type, _T("末尾颜色"));

	//PropUpdate_float((CBCGPProp**)&m_pPropAlphaKey1, &m_pTarget->m_desc.m_ColorKeys[0].a, type, _T("初始透明度"));
	//PropUpdate_float((CBCGPProp**)&m_pPropAlphaKey2, &m_pTarget->m_desc.m_ColorKeys[1].a, type, _T("中点透明度"));
	//PropUpdate_float((CBCGPProp**)&m_pPropAlphaKey3, &m_pTarget->m_desc.m_ColorKeys[2].a, type, _T("末尾透明度"));

	PropUpdate_cstring((CBCGPProp**)&m_pPropTexture, &m_pTarget->m_strTexture, type, _T("贴图"));
	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("绑定点"));

	//PropUpdate_geomv3((CBCGPProp**)&m_pPropScale, &m_pTarget->m_bindInfo.scale, type, _T("等比缩放") , 0.01f, 32767.0f, 0.01f);
	//PropUpdate_float((CBCGPProp**)&m_pPropScaleX, &m_pTarget->m_bindInfo.scale.x, type, _T("X"), 0.01f, 32767.0f, 0.01f);
	//PropUpdate_float((CBCGPProp**)&m_pPropScaleY, &m_pTarget->m_bindInfo.scale.y, type, _T("Y"),  0.01f, 32767.0f, 0.01f);
	//PropUpdate_float((CBCGPProp**)&m_pPropScaleZ, &m_pTarget->m_bindInfo.scale.z, type, _T("Z"),  0.01f, 32767.0f, 0.01f);

	PropUpdate_frametype((CBCGPProp**)&m_pPropFrameType, &m_pTarget->m_descEx.m_FrameType, type, _T("序列帧类型"));

	// 104
	PropUpdate_bool((CBCGPProp**)&m_pPropXYDepend, &m_pTarget->m_descEx104.m_bSizeXYDepend, type, "XY帧独立");


	PropUpdate_int((CBCGPProp**)&m_pPropMaskTexRows, &m_pTarget->m_descEx.m_MaskTexRows, type, _T("贴图行数"), 0.05f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropMaskTexCols, &m_pTarget->m_descEx.m_MaskTexCols, type, _T("贴图列数"), 0.05f, 32767, 1);
	PropUpdate_addressmode((CBCGPProp**)&m_pPropMaskUVAddressMode, &m_pTarget->m_descEx.m_iMaskUVAddrMode, type, _T("贴图寻址模式"));
	PropUpdate_cstring((CBCGPProp**)&m_pPropMaskTexture, &m_pTarget->m_strMaskTexture, type, _T("蒙板贴图"));
}

void CBillboardBar::KeyFrameUpdate(DATA_UPDATA_TYPE type,  CBillboard::BIL_FRAME* pItem)
{

	PropUpdate_float((CBCGPProp**)&m_pPropUVRot,		&pItem->UVRotat, type, _T("UV旋转"), 1.0f, 180.0f, -180.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropUScale,		&pItem->UScale, type, _T("U缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropVScale,		&pItem->VScale, type, _T("V缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropUOffset,		&pItem->UOffset, type, _T("U偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropVOffset,		&pItem->VOffset, type, _T("V偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropSeqFrame,		&pItem->SeqFrame, type, _T("序列帧"), 1.0f, 32767.0f, -32767.0f);
	PropUpdate_color((CBCGPColorProp**)&m_pPropColor,		&pItem->color, type, _T("颜色"));
	PropUpdate_float((CBCGPProp**)&m_pPropAlpha,			&pItem->alpha, type, _T("透明度"), 0.01f, 1.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropSize,			&pItem->Size, type, _T("大小"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropAspect,		&pItem->Aspect, type, _T("纵横比"), 0.1f, 32767.0f, 0.1f);

	// 104
	PropUpdate_float((CBCGPProp**)&m_pPropSizeX,		&pItem->SizeX, type, _T("X大小"), 1.0f, 32767.0f, 0.1f);
	PropUpdate_float((CBCGPProp**)&m_pPropSizeY,		&pItem->SizeY, type, _T("Y大小"), 1.0f, 32767.0f, 0.1f);

	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVRot,		&pItem->MaskUVRotat, type, _T("UV旋转"), 1.0f, 180.0f, -180.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUScale,		&pItem->MaskUScale, type, _T("U缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVScale,		&pItem->MaskVScale, type, _T("V缩放"), 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUOffset,		&pItem->MaskUOffset, type, _T("U偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVOffset,		&pItem->MaskVOffset, type, _T("V偏移"), 0.01f, 32767.0f, -32767.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskSeqFrame,		&pItem->MaskSeqFrame, type, _T("序列帧"), 1.0f, 32767.0f, -32767.0f);

	PropUpdate_int((CBCGPProp**)&m_pPropTick,		&pItem->lTick, type, _T("帧时间"));

}



void CBillboardBar::PropUpdate_frametype(CBCGPProp** ppProp, Ogre::SEQ_FRAME_TYPE* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		std::vector<CString> framemodes;
		(*ppProp) = new CNotifiedProp(name, _T(""));
		cvt.EnumBillboardFrameMode(framemodes);
		for(int i = 0; i < (int) framemodes.size(); i++)
		{
			(*ppProp)->AddOption(framemodes[i].GetBuffer());
		}

	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::SEQ_FRAME_TYPE)cvt.StringToBillboardFrameMode(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else  if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.BillboardFrameModeToStringAsPtr(*data));
	}
}


void CBillboardBar::OnSetFocus(CWnd* pOldWnd)
{
	CEffectEditBar::OnSetFocus(pOldWnd);

	//CDigitHelpMgr::GetInstance()->RequestHelp(this);
	//TRACE("Request\n");
}

void CBillboardBar::OnKillFocus(CWnd* pNewWnd)
{
	CEffectEditBar::OnKillFocus(pNewWnd);
	//if(pNewWnd != ((CFrameWnd*)theApp.m_pMainWnd));
	//{	
	//	TRACE("Release\n");
	//	CDigitHelpMgr::GetInstance()->ReleaseHelp();
	//}

}

void CBillboardBar::OnDestroy()
{

	__super::OnDestroy();

	// TODO: Add your message handler code here
}

void CBillboardBar::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//m_wndPropList.RemoveAll();
	__super::OnClose();
}

LPARAM CBillboardBar::GetItemParam(HTREEITEM hItem)
{
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;
	m_wndTree.GetItem(&item);
	return item.lParam;
}


void CBillboardBar::UpdateKeyFrameWnd()
{
	if(m_pTarget->GetBindInfo()->bCommKeyFrame == true)
		return;

	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_REQUEST_KFWND, 0, (LPARAM) pObj);
}

void CBillboardBar::ReleaseKeyFrameWnd()
{
	CKeyFrameObject* pObj = static_cast<CKeyFrameObject*> (this);
	PostMainFrameMsg(EEM_RELEASE_KFWND, 0, (LPARAM) pObj);
}

void CBillboardBar::CopyKeyFrame(int from, int to)
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

#define BIL_LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))
int CBillboardBar::AddKeyFrame(UINT tick)
{
	m_wndPropList.EndEditItem(FALSE);
	CBillboard::BIL_FRAME frame;
	frame.lTick = tick;

	int i = 0;
	for(i = 0; i < (int) m_pTarget->m_KeyFrames.size(); i++)
	{
		if(m_pTarget->m_KeyFrames[i].lTick > frame.lTick)
			break;
	}


	ASSERT( i > 0 && "Add KeyFrame, Never add at first ");
	if(i == (int) m_pTarget->m_KeyFrames.size())
	{
 		CBillboard::BIL_FRAME& last = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1];
		frame.UVRotat = last.UVRotat;
		frame.UScale = last.UScale;
		frame.VScale = last.VScale;
		frame.UOffset = last.UOffset;
		frame.VOffset = last.VOffset;
		frame.SeqFrame = last.SeqFrame;
		frame.color = last.color;
		frame.Size = last.Size;
		frame.Aspect = last.Aspect;

		// 104
		frame.SizeX = last.SizeX;
		frame.SizeY = last.SizeY;

		frame.MaskUVRotat = last.MaskUVRotat;
		frame.MaskUScale = last.MaskUScale;
		frame.MaskVScale = last.MaskVScale;
		frame.MaskUOffset = last.MaskUOffset;
		frame.MaskVOffset = last.MaskVOffset;


		m_pTarget->m_KeyFrames.push_back(frame);
	}
	else
	{
		CBillboard::BIL_FRAME& next = m_pTarget->m_KeyFrames[i];
		CBillboard::BIL_FRAME& prev = m_pTarget->m_KeyFrames[i - 1];
		float pos = (float)(frame.lTick - prev.lTick ) / ( next.lTick - prev.lTick );

		frame.UVRotat = BIL_LINEAR_LERP(prev.UVRotat, next.UVRotat, pos);
		frame.UScale = BIL_LINEAR_LERP(prev.UScale, next.UScale, pos);
		frame.VScale = BIL_LINEAR_LERP(prev.VScale, next.VScale, pos);
		frame.UOffset = BIL_LINEAR_LERP(prev.UOffset, next.UOffset, pos);
		frame.VOffset = BIL_LINEAR_LERP(prev.VOffset, next.VOffset, pos);
		frame.SeqFrame = BIL_LINEAR_LERP(prev.SeqFrame, next.SeqFrame, pos);
		frame.color = BIL_LINEAR_LERP(prev.color, next.color, pos);
		frame.Size = BIL_LINEAR_LERP(prev.Size, next.Size, pos);
		frame.Aspect = BIL_LINEAR_LERP(prev.Aspect, next.Aspect, pos);

		// 104
		frame.SizeX = BIL_LINEAR_LERP(prev.SizeX, next.SizeX, pos);
		frame.SizeY = BIL_LINEAR_LERP(prev.SizeY, next.SizeY, pos);

		frame.MaskUVRotat = BIL_LINEAR_LERP(prev.MaskUVRotat, next.MaskUVRotat, pos);
		frame.MaskUScale = BIL_LINEAR_LERP(prev.MaskUScale, next.MaskUScale, pos);
		frame.MaskVScale = BIL_LINEAR_LERP(prev.MaskVScale, next.MaskVScale, pos);
		frame.MaskUOffset = BIL_LINEAR_LERP(prev.MaskUOffset, next.MaskUOffset, pos);
		frame.MaskVOffset = BIL_LINEAR_LERP(prev.MaskVOffset, next.MaskVOffset, pos);
		frame.MaskSeqFrame = BIL_LINEAR_LERP(prev.MaskSeqFrame, next.MaskSeqFrame, pos);

		m_pTarget->m_KeyFrames.insert(m_pTarget->m_KeyFrames.begin() + i, frame);
	}
	UpdateTree();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	return i;

}

void CBillboardBar::DeleteKeyFrame(int index)
{
	if(m_pTarget->m_KeyFrames.size() <= 1) return;

	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames.erase(m_pTarget->m_KeyFrames.begin() + index);
		UpdateTree();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

void CBillboardBar::ActiveKeyFrame(int index)
{
	int i = 0;
	HTREEITEM hCurr = m_wndTree.GetChildItem(m_hKeyFrame);
	while(hCurr != NULL)
	{
		CBillboard::BIL_FRAME* pFrameItem = (CBillboard::BIL_FRAME*)GetItemParam(hCurr);
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

void CBillboardBar::SetKeyFramePos(int index, UINT tick)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames[index].lTick = tick;
		UpdateList();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

int CBillboardBar::GetKeyFrameCount()
{
	return (int)m_pTarget->m_KeyFrames.size();
}

UINT CBillboardBar::GetKeyFrameTick(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		return (UINT)m_pTarget->m_KeyFrames[index].lTick;
	}
	return -1;
}

CString CBillboardBar::GetName()
{
	return m_pTarget->GetName();
}
