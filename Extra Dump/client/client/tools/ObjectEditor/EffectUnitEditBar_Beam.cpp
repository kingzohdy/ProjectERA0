
#include "stdafx.h"
#include "EffectUnit_Beam.h"
#include "EffectUnitEditBar_Beam.h"
#include "EffectManager.h"
#include "NotifyProperty.h"
#include "ObjectTypeHelper.h"
#include "SplitterGridView.h"
#include "NewColorKeyDlg.h"
#include "NewFloatKeyDlg.h"

// CBeamBar

IMPLEMENT_DYNAMIC(CBeamBar, CEffectEditBar)
CBeamBar::CBeamBar()
{
	m_bLockApp = true;
	ResetProp();
}

CBeamBar::~CBeamBar()
{
}


BEGIN_MESSAGE_MAP(CBeamBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_NOTIFY(TVN_SELCHANGEDA, ID_BEAMTREE, OnTreeSelChanged)

	ON_UPDATE_COMMAND_UI(ID_BTN_BEA_VISIBLE, OnUpdateVisible)
	ON_COMMAND(ID_BTN_BEA_APPLY, OnApply)
	ON_COMMAND(ID_BTN_BEA_SAVE, OnSave)
	ON_COMMAND(ID_BTN_BEA_DELETE, OnRemove)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)

	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickPropBtn)


END_MESSAGE_MAP()


int CBeamBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this)
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_BEAM))
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
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize(0,rectClient.Height()/3), &context1);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, rectClient.Height()/3*2), &context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	m_wndTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, CRect(0, 0, 100, 100), m_pGrid1, ID_BEAMTREE);
	m_hDesc = m_wndTree.InsertItem(_T("波带参数"));
	m_hKeyFrame = m_wndTree.InsertItem(_T("关键帧"));

	m_pGrid1->SetInnerCtrl(&m_wndTree);

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_BEAMPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);

	m_pGrid2->SetInnerCtrl(&m_wndPropList);

	m_wndSplitter.ShowWindow(SW_SHOW);

	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());
	return 0;
}


void CBeamBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, 0, 0,frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CBeamBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->m_bVisible ? 1 : 0);
}

void CBeamBar::OnVisible()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->m_bVisible = !m_pTarget->m_bVisible;
	PostMainFrameMsg(EEM_APPLY);
}

void CBeamBar::OnApply()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp(m_wndTree.GetSelectedItem());
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}

void CBeamBar::OnSave()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp(m_wndTree.GetSelectedItem());

	SendMainFrameMsg(EEM_SAVE, 0, (LPARAM)m_pTarget);
}

void CBeamBar::OnRemove()
{
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

LRESULT CBeamBar::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
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

void CBeamBar::OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;
	m_hLastSel = pNm->itemOld.hItem;
	m_wndPropList.EndEditItem(TRUE);
	ChangePropList();
	m_hLastSel = NULL;
}

LRESULT CBeamBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	if(m_hLastSel != NULL)
		SaveCurrentProp(m_hLastSel);
	else
		SaveCurrentProp(m_wndTree.GetSelectedItem());

	m_pTarget->UpdateSource();

	if(m_bLockApp && 
		(pnm->pProp == m_pPropMaxParticles 
		|| pnm->pProp == m_pPropTexture
		|| pnm->pProp == m_pPropMaskTexture
		|| pnm->pProp == m_pPropBindBone
		|| pnm->pProp == m_pPropBlendType
		|| pnm->pProp == m_pPropAddressMode
		|| pnm->pProp == m_pPropMaskAddressMode
		|| pnm->pProp == m_pPropDistort
		|| pnm->pProp == m_pPropEmitOnce

		))
	{
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}

	return 0;
}

LRESULT CBeamBar::OnClickPropBtn(WPARAM w, LPARAM l)
{
	PROPNM* pnm = (PROPNM*)l;

	if(pnm->pProp == m_pPropColorKey)
	{
		CNewColorKeyDlg ckdlg;
		ckdlg.m_KeyValues = m_pTarget->m_ColorKey;
		if(IDOK == ckdlg.DoModal())
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
			m_pTarget->m_ColorKey = ckdlg.m_KeyValues;

			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();

			UpdateList();
		}
	}
	else if(pnm->pProp == m_pPropAlphaKey)
	{
		CNewFloatKeyDlg fkdlg;
		fkdlg.m_strValueName = _T("透明度");
		fkdlg.m_fMin = 0.0f;
		fkdlg.m_fMax = 1.0f;

		fkdlg.m_KeyValues = m_pTarget->m_AlphaKey;
		if(IDOK == fkdlg.DoModal())
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
			m_pTarget->m_AlphaKey = fkdlg.m_KeyValues;

			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();

			UpdateList();
		}
	}
	else if(pnm->pProp == m_pPropWidthKey)
	{
		CNewFloatKeyDlg fkdlg;
		fkdlg.m_strValueName = _T("波带宽度");
		fkdlg.m_fMin = 0.0f;
		fkdlg.m_fMax = 100.0f;
		fkdlg.m_KeyValues = m_pTarget->m_WidthKey;
		if(IDOK == fkdlg.DoModal())
		{
			CEffectCommand* pCmd = m_pTarget->BeginRecordCommand();
			m_pTarget->m_WidthKey = fkdlg.m_KeyValues;
			m_pTarget->EndRecordCommand(pCmd);
			m_pTarget->UpdateSource();

			UpdateList();
		}
	}

	return 0;
}

void CBeamBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CBeam*)pEffect;
	if(GetSafeHwnd())
	{
		UpdateTree();
		ChangePropList();
		SetWindowText(m_pTarget->m_strName.GetBuffer());
	}
}

CEffectUnit* CBeamBar::GetEditTarget()
{
	return m_pTarget;
}

LPARAM CBeamBar::GetItemParam(HTREEITEM hItem)
{
	TVITEM item;
	item.mask = TVIF_PARAM | TVIF_HANDLE;
	item.hItem = hItem;
	m_wndTree.GetItem(&item);
	return item.lParam;
}

void CBeamBar::UpdateTree()
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
		strName.Format(_T("关键帧 %d"), i + 1);
		m_wndTree.InsertItem(TVIF_TEXT | TVIF_PARAM,strName.GetBuffer(), 0, 0, 0,0,
			(LPARAM)&m_pTarget->m_KeyFrames[i], m_hKeyFrame, TVI_LAST);
	}
	m_wndTree.Expand(m_hKeyFrame, TVE_EXPAND);

}

void CBeamBar::ChangePropList()
{
	m_wndPropList.EndEditItem(TRUE);

	m_wndPropList.RemoveAll();
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		CreateDescProp();

	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CBeam::BEA_FRAME* pFrameItem = (CBeam::BEA_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		CreateKeyFrameProp(pFrameItem);
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

void CBeamBar::CreateDescProp()
{

	ClearProp();
	DescUpdate(CREATE_PROP);

	m_pPropColorKey		= new CNotifiedProp(_T("颜色调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
	m_pPropAlphaKey		= new CNotifiedProp(_T("Alpha调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());
	m_pPropWidthKey		= new CNotifiedProp(_T("宽度调节"), _T("单击按钮调节"), TRUE, GetSafeHwnd());



	CBCGPProp* pSize = new CBCGPProp(_T("大小变化"));
	pSize->AddSubItem(m_pPropWidthKey);


	CBCGPProp* pColor = new CBCGPProp(_T("颜色变化"));
	pColor->AddSubItem(m_pPropColorKey);
	pColor->AddSubItem(m_pPropAlphaKey);

	CBCGPProp* pLF = new CBCGPProp(_T("低频噪波"));
	pLF->AddSubItem(m_pPropLFPoints);
	pLF->AddSubItem(m_pPropWaveTypeY);
	pLF->AddSubItem(m_pPropLFYParam1);
	pLF->AddSubItem(m_pPropLFYParam2);
	pLF->AddSubItem(m_pPropLFYParam3);
	pLF->AddSubItem(m_pPropWaveTypeZ);
	pLF->AddSubItem(m_pPropLFZParam1);
	pLF->AddSubItem(m_pPropLFZParam2);
	pLF->AddSubItem(m_pPropLFZParam3);

	CBCGPProp* pHF = new CBCGPProp(_T("高频噪波"));
	pHF->AddSubItem(m_pPropHFPoints);
	pHF->AddSubItem(m_pPropHFRange1);
	pHF->AddSubItem(m_pPropHFRange2);

	CBCGPProp* pTextureProp = new CBCGPProp(_T("贴图选项"));
	pTextureProp->AddSubItem(m_pPropAddressMode);
	pTextureProp->AddSubItem(m_pPropUVRotate);
	pTextureProp->AddSubItem(m_pPropUVScale);
	pTextureProp->AddSubItem(m_pPropUOgnOff);
	pTextureProp->AddSubItem(m_pPropVOgnOff);
	pTextureProp->AddSubItem(m_pPropUSpeed);
	pTextureProp->AddSubItem(m_pPropVSpeed);
	pTextureProp->AddSubItem(m_pPropTexRows);
	pTextureProp->AddSubItem(m_pPropTexCols);
	pTextureProp->AddSubItem(m_pPropTexture);


	CBCGPProp* pMaskTextureProp = new CBCGPProp(_T("蒙板贴图选项"));
	pMaskTextureProp->AddSubItem(m_pPropMaskAddressMode);
	pMaskTextureProp->AddSubItem(m_pPropMaskUVRotate);
	pMaskTextureProp->AddSubItem(m_pPropMaskUVScale);
	pMaskTextureProp->AddSubItem(m_pPropMaskUOgnOff);
	pMaskTextureProp->AddSubItem(m_pPropMaskVOgnOff);
	pMaskTextureProp->AddSubItem(m_pPropMaskUSpeed);
	pMaskTextureProp->AddSubItem(m_pPropMaskVSpeed);
	pMaskTextureProp->AddSubItem(m_pPropMaskTexRows);
	pMaskTextureProp->AddSubItem(m_pPropMaskTexCols);
	pMaskTextureProp->AddSubItem(m_pPropMaskTexture);

	//____
	m_wndPropList.AddProperty(m_pPropBlendType);

	m_wndPropList.AddProperty(m_pPropEmitOnce);
	m_wndPropList.AddProperty(m_pPropDistort);	
	m_wndPropList.AddProperty(m_pPropMaxParticles);
	m_wndPropList.AddProperty(m_pPropRandomTile);
	m_wndPropList.AddProperty(m_pPropFrameTick);
	m_wndPropList.AddProperty(pLF);
	m_wndPropList.AddProperty(pHF);
	m_wndPropList.AddProperty(pColor);
	m_wndPropList.AddProperty(pSize);
	m_wndPropList.AddProperty(pTextureProp);
	m_wndPropList.AddProperty(pMaskTextureProp);

	m_wndPropList.AddProperty(m_pPropBindBone);

	//____
	m_pPropBlendType->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaxParticles->SetNotifyTarget(GetSafeHwnd());
	m_pPropHFPoints->SetNotifyTarget(GetSafeHwnd());
	m_pPropHFRange1->SetNotifyTarget(GetSafeHwnd());
	m_pPropHFRange2->SetNotifyTarget(GetSafeHwnd());
	m_pPropLFPoints->SetNotifyTarget(GetSafeHwnd());
	m_pPropWaveTypeY->SetNotifyTarget(GetSafeHwnd());
	m_pPropLFYParam1->SetNotifyTarget(GetSafeHwnd());
	m_pPropLFYParam2->SetNotifyTarget(GetSafeHwnd());
	m_pPropLFYParam3->SetNotifyTarget(GetSafeHwnd());
	m_pPropWaveTypeZ->SetNotifyTarget(GetSafeHwnd());
	m_pPropLFZParam1->SetNotifyTarget(GetSafeHwnd());
	m_pPropLFZParam2->SetNotifyTarget(GetSafeHwnd());
	m_pPropLFZParam3->SetNotifyTarget(GetSafeHwnd());
	m_pPropUSpeed->SetNotifyTarget(GetSafeHwnd());
	m_pPropVSpeed->SetNotifyTarget(GetSafeHwnd());
	m_pPropEmitOnce->SetNotifyTarget(GetSafeHwnd());
	m_pPropDistort->SetNotifyTarget(GetSafeHwnd());
	m_pPropRandomTile->SetNotifyTarget(GetSafeHwnd());
	m_pPropFrameTick->SetNotifyTarget(GetSafeHwnd());
	m_pPropAddressMode->SetNotifyTarget(GetSafeHwnd());
	m_pPropUVRotate->SetNotifyTarget(GetSafeHwnd());
	m_pPropUVScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropUSpeed->SetNotifyTarget(GetSafeHwnd());
	m_pPropVSpeed->SetNotifyTarget(GetSafeHwnd());
	m_pPropTexRows->SetNotifyTarget(GetSafeHwnd());
	m_pPropTexCols->SetNotifyTarget(GetSafeHwnd());
	m_pPropTexture->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskAddressMode->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskUVRotate->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskUVScale->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskUSpeed->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskVSpeed->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskTexRows->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskTexCols->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskTexture->SetNotifyTarget(GetSafeHwnd());
	m_pPropColorKey->SetNotifyTarget(GetSafeHwnd());
	m_pPropAlphaKey->SetNotifyTarget(GetSafeHwnd());
	m_pPropWidthKey->SetNotifyTarget(GetSafeHwnd());
	m_pPropBindBone->SetNotifyTarget(GetSafeHwnd());

	m_pPropUOgnOff->SetNotifyTarget(GetSafeHwnd());
	m_pPropVOgnOff->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskUOgnOff->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaskVOgnOff->SetNotifyTarget(GetSafeHwnd());

}


void CBeamBar::CreateKeyFrameProp(CBeam::BEA_FRAME* pFrame)
{
	ClearProp();
	KeyFrameUpdate(CREATE_PROP, pFrame);

	CBCGPProp* pStartRange = new CBCGPProp("发射区域范围");
	pStartRange->AddSubItem(m_pPropStartRangeX);
	pStartRange->AddSubItem(m_pPropStartRangeY);
	pStartRange->AddSubItem(m_pPropStartRangeZ);

	CBCGPProp* pEndRange = new CBCGPProp("目标区域范围");
	pEndRange->AddSubItem(m_pPropEndRangeX);
	pEndRange->AddSubItem(m_pPropEndRangeY);
	pEndRange->AddSubItem(m_pPropEndRangeZ);

	CBCGPProp* pEndPos = new CBCGPProp("目标中心位置");
	pEndPos->AddSubItem(m_pPropEndPosX);
	pEndPos->AddSubItem(m_pPropEndPosY);
	pEndPos->AddSubItem(m_pPropEndPosZ);


	m_pPropStartRangeX->SetNotifyTarget(GetSafeHwnd());
	m_pPropStartRangeY->SetNotifyTarget(GetSafeHwnd());
	m_pPropStartRangeZ->SetNotifyTarget(GetSafeHwnd());
	m_pPropEndRangeX->SetNotifyTarget(GetSafeHwnd());
	m_pPropEndRangeY->SetNotifyTarget(GetSafeHwnd());
	m_pPropEndRangeZ->SetNotifyTarget(GetSafeHwnd());
	m_pPropEndPosX->SetNotifyTarget(GetSafeHwnd());
	m_pPropEndPosY->SetNotifyTarget(GetSafeHwnd());
	m_pPropEndPosZ->SetNotifyTarget(GetSafeHwnd());
	m_pPropSizeVar->SetNotifyTarget(GetSafeHwnd());
	m_pPropLife->SetNotifyTarget(GetSafeHwnd());
	m_pPropLifeVar->SetNotifyTarget(GetSafeHwnd());
	m_pPropEmitRate->SetNotifyTarget(GetSafeHwnd());
	m_pPropTick->SetNotifyTarget(GetSafeHwnd());

	m_wndPropList.AddProperty(pStartRange);
	m_wndPropList.AddProperty(pEndRange);
	m_wndPropList.AddProperty(pEndPos);
	m_wndPropList.AddProperty(m_pPropSizeVar);
	m_wndPropList.AddProperty(m_pPropLife);
	m_wndPropList.AddProperty(m_pPropLifeVar);
	m_wndPropList.AddProperty(m_pPropEmitRate);
	m_wndPropList.AddProperty(m_pPropTick);
}

void CBeamBar::SaveCurrentProp(HTREEITEM hOldItem)
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
		CBeam::BEA_FRAME* pFrameItem = (CBeam::BEA_FRAME*)GetItemParam(hOldItem);
		KeyFrameUpdate(PROP_TO_DATA, pFrameItem);
	}
	else if(hOldItem == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
		{
			cvt.ParseInt(OLE2T((_bstr_t)m_wndPropList.GetProperty(i)->GetValue()), (int*)&m_pTarget->m_KeyFrames[i].tick);
		}
	}

	m_pTarget->EndRecordCommand(pCmd);
}

void CBeamBar::UpdateList()
{
	CCvtHelper cvt;
	if(m_wndTree.GetSelectedItem() == m_hDesc)
	{
		DescUpdate(DATA_TO_PROP);
	}
	else if(m_wndTree.GetParentItem(m_wndTree.GetSelectedItem()) == m_hKeyFrame)
	{
		CBeam::BEA_FRAME* pFrameItem = (CBeam::BEA_FRAME*)GetItemParam(m_wndTree.GetSelectedItem());
		KeyFrameUpdate(DATA_TO_PROP, pFrameItem);
	}
	else if(m_wndTree.GetSelectedItem() == m_hKeyFrame)
	{
		for(int i = 0; i < (int)m_pTarget->m_KeyFrames.size(); i++)
			m_wndPropList.GetProperty(i)->SetValue(cvt.IntToString(m_pTarget->m_KeyFrames[i].tick));
	}
	m_wndPropList.Invalidate();
}

void CBeamBar::DescUpdate(DATA_UPDATA_TYPE type)
{

	PropUpdate_blendtype((CBCGPProp**)&m_pPropBlendType, (Ogre::UIBlendMode*)&m_pTarget->m_desc.m_BlendType, type, _T("混合模式"));
	PropUpdate_int((CBCGPProp**)&m_pPropMaxParticles, &m_pTarget->m_desc.m_MaxParticles, type, _T("最大条数"), 1.0f, 32767, 1);

	PropUpdate_int((CBCGPProp**)&m_pPropHFPoints, &m_pTarget->m_desc.m_HighFrequencyPoints, type, _T("高频噪点数"), 0.05f, 32767, 2) ;
	PropUpdate_float((CBCGPProp**)&m_pPropHFRange1, &m_pTarget->m_desc.m_HighFrequencyNoiseRange.x, type, _T("高频范围下限"));
	PropUpdate_float((CBCGPProp**)&m_pPropHFRange2, &m_pTarget->m_desc.m_HighFrequencyNoiseRange.y, type, _T("高频范围上线"));

	PropUpdate_int((CBCGPProp**)&m_pPropLFPoints, &m_pTarget->m_desc.m_LowFrequencyPoints, type, _T("低频噪点数"), 0.05f, 32767, 2);
	PropUpdate_wavetype((CBCGPProp**)&m_pPropWaveTypeY, &m_pTarget->m_desc.m_WaveTypeY, type, _T("Y方向波形"));
	PropUpdate_float((CBCGPProp**)&m_pPropLFYParam1, &m_pTarget->m_desc.m_LFYParam1, type, _T("Y随机上限/范围"));
	PropUpdate_float((CBCGPProp**)&m_pPropLFYParam2, &m_pTarget->m_desc.m_LFYParam2, type, _T("Y随机下线/初相"));
	PropUpdate_float((CBCGPProp**)&m_pPropLFYParam3, &m_pTarget->m_desc.m_LFYParam3, type, _T("Y(无)/周期"));

	PropUpdate_wavetype((CBCGPProp**)&m_pPropWaveTypeZ, &m_pTarget->m_desc.m_WaveTypeZ, type, _T("Z方向波形"));
	PropUpdate_float((CBCGPProp**)&m_pPropLFZParam1, &m_pTarget->m_desc.m_LFZParam1, type, _T("Z随机上限/范围"));
	PropUpdate_float((CBCGPProp**)&m_pPropLFZParam2, &m_pTarget->m_desc.m_LFZParam2, type, _T("Z随机下线/初相"));
	PropUpdate_float((CBCGPProp**)&m_pPropLFZParam3, &m_pTarget->m_desc.m_LFZParam3, type, _T("Z(无)/周期"));


	PropUpdate_bool((CBCGPProp**)&m_pPropEmitOnce, &m_pTarget->m_desc.m_bEmitOnce, type, _T("单次发射"));
	PropUpdate_bool((CBCGPProp**)&m_pPropDistort, &m_pTarget->m_desc.m_bDistort, type, _T("空间扭曲模式"));
	PropUpdate_bool((CBCGPProp**)&m_pPropRandomTile, &m_pTarget->m_desc.m_bRandomTile, type, _T("随机起始帧"));
	PropUpdate_float((CBCGPProp**)&m_pPropFrameTick, &m_pTarget->m_desc.m_FrameTime, type, _T("序列帧间隔"));


	PropUpdate_addressmode((CBCGPProp**)&m_pPropAddressMode, &m_pTarget->m_desc.m_iUVAddressMode, type, _T("寻址模式"));
	PropUpdate_float((CBCGPProp**)&m_pPropUVRotate, &m_pTarget->m_desc.m_UVRotate, type, _T("UV旋转"), 1.0f, 180.0f, -180);
	PropUpdate_float((CBCGPProp**)&m_pPropUVScale, &m_pTarget->m_desc.m_UVScale, type, _T("UV缩放"), 0.01f, 32767, 0.01f);
	PropUpdate_int((CBCGPProp**)&m_pPropTexRows, &m_pTarget->m_desc.m_TexRows, type, _T("贴图行数"), 0.05f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropTexCols, &m_pTarget->m_desc.m_TexCols, type, _T("贴图列数"), 0.05f, 32767, 1);
	PropUpdate_float((CBCGPProp**)&m_pPropUSpeed, &m_pTarget->m_desc.m_USpeed, type, _T("U偏移速度"), 0.1f);
	PropUpdate_float((CBCGPProp**)&m_pPropVSpeed, &m_pTarget->m_desc.m_VSpeed, type, _T("V偏移速度"), 0.1f);
	PropUpdate_cstring((CBCGPProp**)&m_pPropTexture, &m_pTarget->m_strTexture, type, _T("贴图"));
	PropUpdate_float((CBCGPProp**)&m_pPropUOgnOff, &m_pTarget->m_desc.m_UOgnOff, type, _T("U初始偏移"), 0.1f);
	PropUpdate_float((CBCGPProp**)&m_pPropVOgnOff, &m_pTarget->m_desc.m_VOgnOff, type, _T("V初始偏移"), 0.1f);

	PropUpdate_addressmode((CBCGPProp**)&m_pPropMaskAddressMode, &m_pTarget->m_desc.m_iMaskUVAddressMode, type, _T("蒙板寻址模式"));
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVRotate, &m_pTarget->m_desc.m_MaskUVRotate, type, _T("蒙板UV旋转"), 1.0f, 180.0f, -180);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUVScale, &m_pTarget->m_desc.m_MaskUVScale, type, _T("蒙板UV缩放"), 0.01f, 32767, 0.01f);
	PropUpdate_int((CBCGPProp**)&m_pPropMaskTexRows, &m_pTarget->m_desc.m_MaskTexRows, type, _T("蒙板贴图行数"), 0.05f, 32767, 1);
	PropUpdate_int((CBCGPProp**)&m_pPropMaskTexCols, &m_pTarget->m_desc.m_MaskTexCols, type, _T("蒙板贴图列数"), 0.05f, 32767, 1);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUSpeed, &m_pTarget->m_desc.m_MaskUSpeed, type, _T("蒙板U偏移速度"), 0.1f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVSpeed, &m_pTarget->m_desc.m_MaskVSpeed, type, _T("蒙板V偏移速度"), 0.1f);
	PropUpdate_cstring((CBCGPProp**)&m_pPropMaskTexture, &m_pTarget->m_strMaskTexture, type, _T("蒙板贴图"));
	PropUpdate_float((CBCGPProp**)&m_pPropMaskUOgnOff, &m_pTarget->m_desc.m_MaskUOgnOff, type, _T("U初始偏移"), 0.1f);
	PropUpdate_float((CBCGPProp**)&m_pPropMaskVOgnOff, &m_pTarget->m_desc.m_MaskVOgnOff, type, _T("V初始偏移"), 0.1f);

	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("绑定骨骼"));
}

void CBeamBar::KeyFrameUpdate(DATA_UPDATA_TYPE type, CBeam::BEA_FRAME* pFrame)
{
	PropUpdate_float((CBCGPProp**)&m_pPropStartRangeX, &pFrame->startrange.x, type, _T("X"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropStartRangeY, &pFrame->startrange.y, type, _T("Y"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropStartRangeZ, &pFrame->startrange.z, type, _T("Z"), 1.0f, 32767.0f, 0.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropEndRangeX, &pFrame->endrange.x, type, _T("X"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropEndRangeY, &pFrame->endrange.y, type, _T("Y"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropEndRangeZ, &pFrame->endrange.z, type, _T("Z"), 1.0f, 32767.0f, 0.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropEndPosX, &pFrame->endpos.x, type, _T("X"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropEndPosY, &pFrame->endpos.y, type, _T("Y"), 1.0f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropEndPosZ, &pFrame->endpos.z, type, _T("Z"), 1.0f, 32767.0f, 0.0f);

	PropUpdate_float((CBCGPProp**)&m_pPropSizeVar, &pFrame->sizevar, type, _T("宽度误差(%)"));
	PropUpdate_float((CBCGPProp**)&m_pPropLife, &pFrame->life, type, _T("生命长度"),  0.05f, 32767.0f, 0.0f);
	PropUpdate_float((CBCGPProp**)&m_pPropLifeVar, &pFrame->lifevar, type, _T("生命长度误差(%)"));
	PropUpdate_float((CBCGPProp**)&m_pPropEmitRate, &pFrame->emitrate, type, _T("发射率"),  1.0f, 32767.0f, 0.0f);
	PropUpdate_int((CBCGPProp**)&m_pPropTick, &pFrame->tick, type, _T("帧时间"));

}

void CBeamBar::ResetProp()
{
	m_pPropBlendType = NULL;
	m_pPropMaxParticles = NULL;

	m_pPropHFPoints = NULL;
	m_pPropHFRange1 = NULL;
	m_pPropHFRange2 = NULL;

	m_pPropLFPoints = NULL;
	m_pPropWaveTypeY = NULL;
	m_pPropLFYParam1 = NULL;
	m_pPropLFYParam2 = NULL;
	m_pPropLFYParam3 = NULL;

	m_pPropWaveTypeZ = NULL;
	m_pPropLFZParam1 = NULL;
	m_pPropLFZParam2 = NULL;
	m_pPropLFZParam3 = NULL;

	m_pPropUSpeed = NULL;
	m_pPropVSpeed = NULL;

	m_pPropEmitOnce = NULL;
	m_pPropDistort = NULL;
	m_pPropRandomTile = NULL;
	m_pPropFrameTick = NULL;

	m_pPropAddressMode = NULL;
	m_pPropUVRotate = NULL;
	m_pPropUVScale = NULL;
	m_pPropUSpeed = NULL;
	m_pPropVSpeed = NULL;
	m_pPropTexRows = NULL;
	m_pPropTexCols = NULL;
	m_pPropTexture = NULL;

	m_pPropMaskAddressMode = NULL;
	m_pPropMaskUVRotate = NULL;
	m_pPropMaskUVScale = NULL;
	m_pPropMaskUSpeed = NULL;
	m_pPropMaskVSpeed = NULL;
	m_pPropMaskTexRows = NULL;
	m_pPropMaskTexCols = NULL;
	m_pPropMaskTexture = NULL;

	m_pPropColorKey = NULL;
	m_pPropAlphaKey = NULL;
	m_pPropWidthKey = NULL;

	m_pPropBindBone = NULL;


	m_pPropStartRangeX = NULL;
	m_pPropStartRangeY = NULL;
	m_pPropStartRangeZ = NULL;
	m_pPropEndRangeX = NULL;
	m_pPropEndRangeY = NULL;
	m_pPropEndRangeZ = NULL;
	m_pPropEndPosX = NULL;
	m_pPropEndPosY = NULL;
	m_pPropEndPosZ = NULL;
	m_pPropSizeVar = NULL;
	m_pPropLife = NULL;
	m_pPropLifeVar = NULL;
	m_pPropEmitRate = NULL;
	m_pPropTick = NULL;
}

void CBeamBar::ClearProp()
{
	ResetProp();
	m_wndPropList.RemoveAll();
}

void CBeamBar::PropUpdate_wavetype(CBCGPProp** ppProp, Ogre::BEAM_WAVE_TYPE* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		std::vector<CString> types;
		cvt.EnumBeamWaveType(types);
		(*ppProp) = new CNotifiedProp(name, _T(""));
		for(int i = 0; i < (int)types.size(); i++)
		{
			(*ppProp)->AddOption(types[i].GetBuffer());
		}
	}
	else if(type == PROP_TO_DATA)
	{
		CString sel = OLE2T((_bstr_t)(*ppProp)->GetValue());
		*data = cvt.StringToBeamWaveType(sel);
	}
	else if(type == DATA_TO_PROP)
	{
		CString str = cvt.BeamWaveTypeToStringAsPtr(*data);
		(*ppProp)->SetValue((_bstr_t)str.GetBuffer());
	}
}


void CBeamBar::CopyKeyFrame(int from, int to)
{
	if(from < 0 || from > (int)m_pTarget->m_KeyFrames.size() 
		|| to < 0 || to > (int)m_pTarget->m_KeyFrames.size() )
	{
		return;
	}

	long tick = m_pTarget->m_KeyFrames[to].tick;
	m_pTarget->m_KeyFrames[to] = m_pTarget->m_KeyFrames[from];
	m_pTarget->m_KeyFrames[to].tick = tick;
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);

}

#define BEA_LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))
int CBeamBar::AddKeyFrame(UINT tick)
{
	m_wndPropList.EndEditItem(FALSE);
	CBeam::BEA_FRAME frame;
	frame.tick = tick;

	int i = 0;
	for(i = 0; i < (int) m_pTarget->m_KeyFrames.size(); i++)
	{
		if(m_pTarget->m_KeyFrames[i].tick > frame.tick)
			break;
	}

	ASSERT( i > 0 && "Add KeyFrame, Never add at first ");
	if(i == (int) m_pTarget->m_KeyFrames.size())
	{
		CBeam::BEA_FRAME& last = m_pTarget->m_KeyFrames[m_pTarget->m_KeyFrames.size() - 1];
		frame.startrange = last.startrange;
		frame.endrange = last.endrange;
		frame.endpos = last.endpos;

		frame.life = last.life;
		frame.lifevar = last.lifevar;
		frame.sizevar = last.sizevar;
		frame.emitrate = last.emitrate;

		m_pTarget->m_KeyFrames.push_back(frame);
	}
	else
	{
		CBeam::BEA_FRAME& next = m_pTarget->m_KeyFrames[i];
		CBeam::BEA_FRAME& prev = m_pTarget->m_KeyFrames[i - 1];
		float pos = (float)(frame.tick - prev.tick ) / ( next.tick - prev.tick );

		frame.startrange = BEA_LINEAR_LERP(prev.startrange, next.startrange, pos);
		frame.endrange = BEA_LINEAR_LERP(prev.endrange, next.endrange, pos);

		frame.endpos = BEA_LINEAR_LERP(prev.endpos, next.endpos, pos);
		frame.life = BEA_LINEAR_LERP(prev.life, next.life, pos);
		frame.lifevar = BEA_LINEAR_LERP(prev.lifevar, next.lifevar, pos);
		frame.sizevar = BEA_LINEAR_LERP(prev.sizevar, next.sizevar, pos);
		frame.emitrate = BEA_LINEAR_LERP(prev.emitrate, next.emitrate, pos);


		m_pTarget->m_KeyFrames.insert(m_pTarget->m_KeyFrames.begin() + i, frame);
	}
	UpdateTree();
	m_pTarget->UpdateSource();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	return i;

}

void CBeamBar::DeleteKeyFrame(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames.erase(m_pTarget->m_KeyFrames.begin() + index);
		UpdateTree();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

void CBeamBar::ActiveKeyFrame(int index)
{
	if(index >= m_pTarget->m_KeyFrames.size()) return;

	int i = 0;
	HTREEITEM hCurr = m_wndTree.GetChildItem(m_hKeyFrame);
	while(hCurr != NULL)
	{
		CBeam::BEA_FRAME* pFrameItem = (CBeam::BEA_FRAME*)GetItemParam(hCurr);
		if(pFrameItem == &m_pTarget->m_KeyFrames[index])
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

void CBeamBar::SetKeyFramePos(int index, UINT tick)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		m_pTarget->m_KeyFrames[index].tick = tick;
		UpdateList();
		m_pTarget->UpdateSource();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}
}

int CBeamBar::GetKeyFrameCount()
{
	return (int)m_pTarget->m_KeyFrames.size();
}

UINT CBeamBar::GetKeyFrameTick(int index)
{
	if(index >= 0 && index < (int)m_pTarget->m_KeyFrames.size())
	{
		return (UINT)m_pTarget->m_KeyFrames[index].tick;
	}
	return -1;
}

CString CBeamBar::GetName()
{
	return m_pTarget->GetName();
}

void CBeamBar::SetPropValue(CString strFlag, _variant_t var)
{
	PROPNM mPropNM;
	strFlag.MakeUpper();
	if(strFlag == "TEX" && m_pPropTexture != NULL)
	{
		m_pPropTexture->SetValue(var);
		mPropNM.pProp = m_pPropTexture;
	}
	else if(strFlag == "MASKTEX" && m_pPropMaskTexture != NULL)
	{
		m_pPropMaskTexture->SetValue(var);
		mPropNM.pProp = m_pPropMaskTexture;
	}

	OnUpdatePropValue(0, (LPARAM)&mPropNM);
}