
#include "stdafx.h"
#include "SRTEditBar.h"
#include "NotifyProperty.h"
#include "SRTObject.h"
#include "EffectUnitEditBar.h"
#include "EularEditDlg.h"

// CSRTBar

IMPLEMENT_DYNAMIC(CSRTBar, CBCGPDockingControlBar)
CSRTBar::CSRTBar()
{
	m_pSrt = &CSRTObject::ms_DummyObj;
	m_nxoff = 0;
	m_nyoff = 0;
	m_nzoff = 0;
	m_trans = 1.0f;
}

CSRTBar::~CSRTBar()
{
}


BEGIN_MESSAGE_MAP(CSRTBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickBtn)
	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CSRTBar message handlers


int CSRTBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_SRT_PROP);

	m_wndPropList.EnableHeaderCtrl(FALSE);
	CreatePropSRT();

	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());
	return 0;
}

void CSRTBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndPropList.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top , SWP_NOZORDER);

	//m_wndTitle.SetWindowPos(NULL, 0, 0,
	//	frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

LRESULT CSRTBar::OnClickBtn(WPARAM wParam, LPARAM lParam)
{
	if(m_type == SBT_ICEFROZEN) return 0;

	Ogre::Quaternion rot;
	rot.identity();

	CEularEditDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		rot.setEulerAngle(dlg.m_fYaw, dlg.m_fPitch, dlg.m_fRoll);
		m_pSrt->SetRotation(rot);
	}


	return 0;
}

LRESULT CSRTBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	SaveCurrentProp(pnm->pProp->GetName());

	return 0;
}

LRESULT CSRTBar::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	if(pdn->hWnd == m_wndPropList.GetSafeHwnd())
	{
		if(!IsTextureResource(pdn->strChunk))
		{
			return 0;
		}

		if( m_pPropOverlayTex->GetRect().PtInRect(pdn->dropPos))
		{
			m_pPropOverlayTex->SetValue(pdn->strChunk);
			PROPNM nm;
			nm.pProp = (CNotifiedProp*)m_pPropOverlayTex;
			OnUpdatePropValue(0, (LPARAM)&nm);
		}
	}
	return 0;
}

void CSRTBar::CreatePropFloat()
{
	m_wndPropList.RemoveAll();
	m_strFloatName = m_pFlt->GetFloatName();
	m_pFlt->GetFloatRange(&m_min, &m_max, &m_ratio);
	DataUpdateFloat(CREATE_PROP);

	m_pPropFloatValue->SetNotifyTarget(GetSafeHwnd());
	m_wndPropList.AddProperty(m_pPropFloatValue);
	m_pPropResetRot = NULL;

}

void CSRTBar::CreatePropColor()
{
	m_wndPropList.RemoveAll();

	DataUpdateColor(CREATE_PROP);

	m_pPropColor->SetNotifyTarget(GetSafeHwnd());
	m_pPropAlpha->SetNotifyTarget(GetSafeHwnd());	
	m_wndPropList.AddProperty(m_pPropColor);
	m_wndPropList.AddProperty(m_pPropAlpha);

	m_pPropResetRot = NULL;
}

void CSRTBar::CreatePropRemanetShadow()
{
	m_wndPropList.RemoveAll();
	DataUpdateRemanetShadow(CREATE_PROP);

	m_pPropInterval->SetNotifyTarget(GetSafeHwnd());
	m_pPropImages->SetNotifyTarget(GetSafeHwnd());
	m_wndPropList.AddProperty(m_pPropInterval);
	m_wndPropList.AddProperty(m_pPropImages);
}

void CSRTBar::CreatePropOffColor()
{
	m_wndPropList.RemoveAll();
	DataUpdateOffColor(CREATE_PROP);
	m_wndPropList.AddProperty(m_pPropOffColor);
	m_pPropOffColor->SetNotifyTarget(GetSafeHwnd());
}

void CSRTBar::CreatePropSRT()
{
	m_wndPropList.RemoveAll();
	DataUpdateSRT(CREATE_PROP);

	CNotifiedProp* pPos = new CNotifiedProp(_T("相对位置"), 0, TRUE);
	pPos->AddSubItem(m_pPropPosX);
	pPos->AddSubItem(m_pPropPosY);
	pPos->AddSubItem(m_pPropPosZ);
	pPos->SetNotifyTarget(GetSafeHwnd());

	CNotifiedProp* pRot = new CNotifiedProp(_T("旋转"), 0, TRUE);
	pRot->AddSubItem(m_pPropYaw);
	pRot->AddSubItem(m_pPropPitch);
	pRot->AddSubItem(m_pPropRoll);
	//pRot->SetNotifyTarget(GetSafeHwnd());

	//m_pPropResetRot=  new CNotifiedProp("旋转欧拉角", "单击按钮设置", TRUE, GetSafeHwnd());

	CNotifiedProp* pScale = new CNotifiedProp(_T("缩放"), 0, TRUE);
	pScale->AddSubItem(m_pPropScaleX);
	pScale->AddSubItem(m_pPropScaleY);
	pScale->AddSubItem(m_pPropScaleZ);
	pScale->SetNotifyTarget(GetSafeHwnd());

	m_pPropPosX->SetNotifyTarget(GetSafeHwnd());
	m_pPropPosY->SetNotifyTarget(GetSafeHwnd());
	m_pPropPosZ->SetNotifyTarget(GetSafeHwnd());

	m_pPropYaw->SetNotifyTarget(GetSafeHwnd());
	m_pPropPitch->SetNotifyTarget(GetSafeHwnd());
	m_pPropRoll->SetNotifyTarget(GetSafeHwnd());

	m_pPropScaleX->SetNotifyTarget(GetSafeHwnd());
	m_pPropScaleY->SetNotifyTarget(GetSafeHwnd());
	m_pPropScaleZ->SetNotifyTarget(GetSafeHwnd());

	m_pPropTransparent->SetNotifyTarget(GetSafeHwnd());
	m_pPropFade->SetNotifyTarget(GetSafeHwnd());

	m_wndPropList.AddProperty(pPos);
	m_wndPropList.AddProperty(pRot);
	m_wndPropList.AddProperty(pScale);
	m_wndPropList.AddProperty(m_pPropTransparent);
}

void CSRTBar::CreatePropIceFrozen()
{
	m_wndPropList.RemoveAll();
	DataUpdateIceFrozen(CREATE_PROP);

	m_wndPropList.AddProperty(m_pPropOverlayBlendMode);
	m_pPropOverlayBlendMode->SetNotifyTarget(GetSafeHwnd());

	m_wndPropList.AddProperty(m_pPropOverlayTex);
	m_pPropOverlayTex->SetNotifyTarget(GetSafeHwnd());

	m_wndPropList.AddProperty(m_pPropTransparent);
	m_pPropTransparent->SetNotifyTarget(GetSafeHwnd());
}

void CSRTBar::CratePropFocusBlur()
{
	m_wndPropList.RemoveAll();
	DataUpdateFocusBlur(CREATE_PROP);
	m_wndPropList.AddProperty(m_pPropBlur);
	m_pPropBlur->SetNotifyTarget(GetSafeHwnd());
	m_wndPropList.AddProperty(m_pPropXOff);
	m_pPropXOff->SetNotifyTarget(GetSafeHwnd());
	m_wndPropList.AddProperty(m_pPropYOff);
	m_pPropYOff->SetNotifyTarget(GetSafeHwnd());
	m_wndPropList.AddProperty(m_pPropZOff);
	m_pPropZOff->SetNotifyTarget(GetSafeHwnd());
}

//将数据送给CModelMontionInfo
void CSRTBar::SaveCurrentProp(CString strChangeBy)
{
	if(m_type == SBT_SRT)
	{
		DataUpdateSRT(PROP_TO_DATA);
		if(m_pSrt->IsGeomScale() == true)
		{
			m_scale.y = m_scale.z = m_scale.x;
		}
		m_pSrt->SetPosition(m_pos); 
		m_pSrt->SetRotationAngle(m_rotAngle);
		m_pSrt->SetScale(m_scale);
		m_pSrt->SetTransparent(m_trans);
		m_pSrt->SetOffColor(m_offcolor);
		m_pSrt->SetFade(m_bFade);
	}
	else if(m_type == SBT_FLOAT)
	{
		DataUpdateFloat(PROP_TO_DATA);
		m_pFlt->SetFloat(m_float);
	}
	else if(m_type == SBT_COLOR)
	{
		DataUpdateColor(PROP_TO_DATA);
		m_pClr->SetColor(m_color);
	}
	else if(m_type == SBT_REMANETSHADOW)
	{
		DataUpdateRemanetShadow(PROP_TO_DATA);
		m_pRemanetShadow->SetIntervalTime(m_ShadowInterval);
		m_pRemanetShadow->SetImages(m_Images);
	}
	else if(m_type == SBT_OFFCOLOR)
	{
		DataUpdateOffColor(PROP_TO_DATA);
		m_pOffClr->SetOffColor(m_offcolor);
	}
	else if(m_type == SBT_ICEFROZEN)
	{
		DataUpdateIceFrozen(PROP_TO_DATA);
		m_pIceFrozen->SetIceFrozenBlend(m_OverlayBlendMode, m_OverlayTex,m_trans);
	}
	else if(m_type == SBT_FOCUSBLUR)
	{
		DataUpdateFocusBlur(PROP_TO_DATA);
		m_pFocusBlur->SetFocusBlurParams(1, m_nblur);
		m_pFocusBlur->SetFocusBlurParams(2, m_nxoff);
		m_pFocusBlur->SetFocusBlurParams(3, m_nyoff);
		m_pFocusBlur->SetFocusBlurParams(4, m_nzoff);
	}
}

void CSRTBar::UpdateList()
{	
	m_wndPropList.EndEditItem(TRUE);
	if(m_type == SBT_SRT)
	{
		m_pos = m_pSrt->GetPosition();
		//m_rot = m_pSrt->GetRotation();
		m_rotAngle = m_pSrt->GetRotateAngle();
		m_scale = m_pSrt->GetScale();
		m_trans = m_pSrt->GetTransparent();
		SetWindowText(m_pSrt->GetObjName().GetBuffer());
		DataUpdateSRT(DATA_TO_PROP);

		if(m_pSrt->IsGeomScale() == true)
		{
			m_pPropScaleY->Enable(FALSE);
			m_pPropScaleZ->Enable(FALSE);
		}
		else
		{
			m_pPropScaleY->Enable(TRUE);
			m_pPropScaleZ->Enable(TRUE);
		}
	}
	else if(m_type == SBT_FLOAT)
	{
		m_float = m_pFlt->GetFloat();
		DataUpdateFloat(DATA_TO_PROP);
	}
	else if(m_type == SBT_COLOR)
	{
		m_color = m_pClr->GetColor();
		DataUpdateColor(DATA_TO_PROP);
	}
	else if(m_type == SBT_REMANETSHADOW)
	{
		m_ShadowInterval = m_pRemanetShadow->GetIntervalTime();
		m_Images = m_pRemanetShadow->GetImages();
		DataUpdateRemanetShadow(DATA_TO_PROP);
	}
	else if(m_type == SBT_OFFCOLOR)
	{
		m_offcolor = m_pOffClr->GetOffColor();
		DataUpdateOffColor(DATA_TO_PROP);
	}
	else if(m_type == SBT_ICEFROZEN)
	{
		m_OverlayBlendMode = m_pIceFrozen->GetIceFrozenBlend();
		m_OverlayTex = m_pIceFrozen->GetOverlayTex();
		m_trans = m_pIceFrozen->GetIceFrozenAlpha();
		DataUpdateIceFrozen(DATA_TO_PROP);
	}
	else if(m_type == SBT_FOCUSBLUR)
	{
		m_nblur = m_pFocusBlur->GetFocusBlurParams(1);
		m_nxoff = m_pFocusBlur->GetFocusBlurParams(2);
		m_nyoff = m_pFocusBlur->GetFocusBlurParams(3);
		m_nzoff = m_pFocusBlur->GetFocusBlurParams(4);
		DataUpdateFocusBlur(DATA_TO_PROP);
	}
}

void CSRTBar::DataUpdateRemanetShadow(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_int((CBCGPProp**)&m_pPropInterval, &m_ShadowInterval, type, "时间间隔", 10, 10000, 1);
	CEffectEditBar::PropUpdate_int((CBCGPProp**)&m_pPropImages, &m_Images, type, "残影采样数量", 10, 10000, 1);
}

void CSRTBar::DataUpdateOffColor(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_color((CBCGPColorProp**)&m_pPropOffColor, &m_offcolor, type, "角色偏色");
}

void CSRTBar::DataUpdateFloat(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropFloatValue, &m_float, type, m_strFloatName, m_ratio, m_max, m_min);
}

void CSRTBar::DataUpdateColor(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_color((CBCGPColorProp**)&m_pPropColor, &m_color, type, "颜色");
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropAlpha, &m_color.a, type, "透明度", 0.05f, 1.0f, 0.0f);
}

void CSRTBar::DataUpdateSRT(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropPosX, &m_pos.x, type, _T("X"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropPosY, &m_pos.y, type, _T("Y"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropPosZ, &m_pos.z, type, _T("Z"));

	//CEffectEditBar::PropUpdate_eular_yaw  ((CBCGPProp**)&m_pPropYaw, &m_rot, type, _T("转角"));
	//CEffectEditBar::PropUpdate_eular_pitch((CBCGPProp**)&m_pPropPitch, &m_rot, type, _T("俯仰角"));
	//CEffectEditBar::PropUpdate_eular_roll ((CBCGPProp**)&m_pPropRoll, &m_rot, type, _T("侧滚角"));
	//CEffectEditBar::PropUpdate_rot((CBCGPProp**)&m_pPropYaw, (CBCGPProp**)&m_pPropPitch,(CBCGPProp**)&m_pPropRoll, &m_rot, type, _T("旋转"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropYaw, &m_rotAngle.x, type, _T("旋转X"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropPitch, &m_rotAngle.y, type, _T("旋转Y"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropRoll, &m_rotAngle.z, type, _T("旋转Z"));

	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropScaleX, &m_scale.x, type, _T("X") , 0.01f, 32767.0f, 0.01f);

	if(m_pSrt->IsGeomScale() == false || type != PROP_TO_DATA)
	{
		CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropScaleY, &m_scale.y, type, _T("Y") , 0.01f, 32767.0f, 0.01f);
		CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropScaleZ, &m_scale.z, type, _T("Z") , 0.01f, 32767.0f, 0.01f);
	}
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropTransparent, &m_trans, type, _T("透明度") , 0.01f, 1.0f, 0.00f);
	CEffectEditBar::PropUpdate_bool((CBCGPProp**)&m_pPropFade, &m_bFade, type, _T("淡出"));
}

void CSRTBar::DataUpdateIceFrozen(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_blendtype((CBCGPProp**)&m_pPropOverlayBlendMode, &m_OverlayBlendMode, type, _T("叠加混合模式"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropOverlayTex, &m_OverlayTex, type, _T("叠加贴图"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropTransparent, &m_trans, type, _T("透明度") , 0.01f, 1.0f, 0.00f);
}

void CSRTBar::DataUpdateFocusBlur(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropBlur, &m_nblur, type, _T("焦点模糊大小"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropXOff, &m_nxoff, type, _T("焦点模糊X轴偏移"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropYOff, &m_nyoff, type, _T("焦点模糊Y轴偏移"));
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropZOff, &m_nzoff, type, _T("焦点模糊Z轴偏移"));
}

void CSRTBar::ClearTargetObject()
{
	m_pSrt = &CSRTObject::ms_DummyObj;
	m_pFlt = &CFloatObject::ms_DummyObj;
	m_pClr = &CColorObject::ms_DummyObj;
	m_pRemanetShadow = &CRemanetShadowObject::ms_DummyObj;
	m_pOffClr = &COffColorObject::ms_DummyObj;
	m_pIceFrozen = &CIceFrozenObject::ms_DummyObj;
	UpdateList();
}

void CSRTBar::SetRemanetShadowObject(CRemanetShadowObject* pObj)
{
	if(pObj == NULL)
		m_pRemanetShadow = &CRemanetShadowObject::ms_DummyObj;
	else
		m_pRemanetShadow = pObj;
	m_pFlt = &CFloatObject::ms_DummyObj;
	m_pClr = &CColorObject::ms_DummyObj;

	if(m_type != SBT_REMANETSHADOW)
	{
		m_type = SBT_REMANETSHADOW;
		CreatePropRemanetShadow();
	}

	UpdateList();
}

void CSRTBar::SetSRTObject(CSRTObject * pSrtObj)
{
	if(pSrtObj == NULL)
		m_pSrt = &CSRTObject::ms_DummyObj;
	else
		m_pSrt = pSrtObj;

	m_pFlt = &CFloatObject::ms_DummyObj;
	m_pClr = &CColorObject::ms_DummyObj;

	if(m_type != SBT_SRT)
	{
		m_type = SBT_SRT;
		CreatePropSRT();
	}

	UpdateList();
}

void CSRTBar::SetFloatObject(CFloatObject * pObj)
{
	if(pObj == NULL)
		m_pFlt = &CFloatObject::ms_DummyObj;
	else
		m_pFlt = pObj;

	m_pSrt = &CSRTObject::ms_DummyObj;
	m_pClr = &CColorObject::ms_DummyObj;

	if(m_type != SBT_FLOAT)
	{
		m_type = SBT_FLOAT;
		m_strFloatName = pObj->GetFloatName();
		pObj->GetFloatRange(&m_min, &m_max, &m_ratio);
		CreatePropFloat();
	}


	UpdateList();
}


void CSRTBar::SetColorObject(CColorObject * pObj)
{
	if(pObj == NULL)
		m_pClr = &CColorObject::ms_DummyObj;
	else
		m_pClr = pObj;

	m_pSrt = &CSRTObject::ms_DummyObj;
	m_pFlt = &CFloatObject::ms_DummyObj;

	if(m_type != SBT_COLOR)
	{
		m_type = SBT_COLOR;
		CreatePropColor();
	}

	UpdateList();
}

void CSRTBar::SetOffColorObject(COffColorObject* pObj)
{
	if(pObj == NULL)
		m_pOffClr = &COffColorObject::ms_DummyObj;
	else
		m_pOffClr = pObj;
	m_pSrt = &CSRTObject::ms_DummyObj;
	m_pFlt = &CFloatObject::ms_DummyObj;

	if(m_type != SBT_OFFCOLOR)
	{
		m_type = SBT_OFFCOLOR;
		CreatePropOffColor();
	}

	UpdateList();
}

void CSRTBar::SetIceFrozenObject(CIceFrozenObject* pObj)
{
	if(pObj == NULL)
		m_pIceFrozen = &CIceFrozenObject::ms_DummyObj;
	else
		m_pIceFrozen = pObj;
	m_pSrt = &CSRTObject::ms_DummyObj;
	m_pFlt = &CFloatObject::ms_DummyObj;

	if(m_type != SBT_ICEFROZEN)
	{
		m_type = SBT_ICEFROZEN;
		CreatePropIceFrozen();
	}

	UpdateList();
}

void CSRTBar::SetFocusBlurObject(CFocusBlurObject* pObj)
{
	if(pObj == NULL)
		m_pFocusBlur = &CFocusBlurObject::ms_DummyObj;
	else
		m_pFocusBlur = pObj;

	m_pSrt = &CSRTObject::ms_DummyObj;
	m_pFlt = &CFloatObject::ms_DummyObj;

	if(m_type != SBT_FOCUSBLUR)
	{
		m_type = SBT_FOCUSBLUR;
		CratePropFocusBlur();
	}

	UpdateList();
}

void CSRTBar::OnKillFocus(CWnd* pNewWnd)
{
	CBCGPDockingControlBar::OnKillFocus(pNewWnd);
	m_wndPropList.EndEditItem(TRUE);
	// TODO: Add your message handler code here
}
