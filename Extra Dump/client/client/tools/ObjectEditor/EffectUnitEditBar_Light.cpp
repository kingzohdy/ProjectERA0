
#include "stdafx.h"
#include "EffectUnitEditBar_Light.h"
#include "NotifyProperty.h"
#include "EffectUnit_Light.h"
#include "EffectManager.h"
#include "ObjectTypeHelper.h"

IMPLEMENT_DYNAMIC(CLightBar, CEffectEditBar)
CLightBar::CLightBar() : m_pTarget(NULL), m_bLockApp(true)
{
}

CLightBar::~CLightBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CLightBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	//ON_COMMAND(ID_BTN_LT_VISIBLE, OnVisible)
	ON_UPDATE_COMMAND_UI(ID_BTN_LT_VISIBLE, OnUpdateVisible)
	ON_COMMAND(ID_BTN_LT_APPLY, OnApply)
	ON_COMMAND(ID_BTN_LT_SAVE, OnSave)
	ON_COMMAND(ID_BTN_LT_REMOVE, OnRemove)
	ON_COMMAND(ID_BTN_LT_CLICK, OnBtnClick)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_COMMAND(ID_BTN_LT_LOCKAPPLY, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_LT_LOCKAPPLY, OnUpdateBtnLockApply)
END_MESSAGE_MAP()

// CLightBar message handlers


int CLightBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this)
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_LIGHT))
	{
		return -1;
	}
	//m_wndPropList.
	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_LIGHTEPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);

	CreateProp();
	return 0;
}

LRESULT CLightBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	SaveCurrentProp();
	m_pTarget->UpdateSource();
	if(pnm->pProp == m_pPropType 
		|| pnm->pProp == m_pPropColor
		/*		|| pnm->pProp == m_pPropOffsetX
		|| pnm->pProp == m_pPropOffsetY
		|| pnm->pProp == m_pPropOffsetZ*/
		)
	{
		//m_pTarget->RefreshNode();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}

	return 0;
}

void CLightBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndPropList.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CLightBar::OnVisible()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();

	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg(EEM_APPLY);
}

void CLightBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->m_bVisible ? 1 : 0);
}

void CLightBar::OnApply()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();
	//PostMainFrameMsg(EEM_APPLY);
	//m_pTarget->RefreshNode();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}

void CLightBar::OnSave()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();
	SendMainFrameMsg( EEM_SAVE, 0, (LPARAM)m_pTarget);

}

void CLightBar::OnRemove()
{
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CLightBar::OnBtnClick()
{
	m_pTarget->GetInteract()->OnLBtnDown(0.0f, 0.0f, true);
	CEffectMgr::GetInstance()->SetActiveEffect(m_pTarget);
}

void CLightBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CLightBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CLightBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CLight*)pEffect;
	if(GetSafeHwnd())
	{
		UpdateList();
		SetWindowText(m_pTarget->m_strName.GetBuffer());
	}
}

CEffectUnit* CLightBar::GetEditTarget()
{
	return m_pTarget;;
}

void CLightBar::CreateProp()
{
	HWND hwnd = GetSafeHwnd();
	m_wndPropList.EndEditItem(TRUE);
	if(hwnd) m_wndPropList.RemoveAll();

	DataUpdate(CREATE_PROP);

	m_pPropType->AddOption(_T("直射光源"));
	m_pPropType->AddOption(_T("点光源"));

	CBCGPProp* pPos = new CBCGPProp(_T("相对位置"));
	pPos->AddSubItem(m_pPropOffsetX);
	pPos->AddSubItem(m_pPropOffsetY);
	pPos->AddSubItem(m_pPropOffsetZ);
	m_wndPropList.AddProperty(pPos);

	((CNotifiedProp*)m_pPropOffsetX)->SetNotifyTarget(GetSafeHwnd());
	((CNotifiedProp*)m_pPropOffsetY)->SetNotifyTarget(GetSafeHwnd());
	((CNotifiedProp*)m_pPropOffsetZ)->SetNotifyTarget(GetSafeHwnd());
	((CNotifiedProp*)m_pPropType)->SetNotifyTarget(GetSafeHwnd());
	m_pPropColor->SetNotifyTarget(GetSafeHwnd());
	((CNotifiedProp*)m_pPropAttenuation)->SetNotifyTarget(GetSafeHwnd());
	((CNotifiedProp*)m_pPropRange)->SetNotifyTarget(GetSafeHwnd());
	((CNotifiedProp*)m_pPropBindBone)->SetNotifyTarget(GetSafeHwnd());

	m_wndPropList.AddProperty(m_pPropType);
	m_wndPropList.AddProperty(m_pPropColor);
	m_wndPropList.AddProperty(m_pPropAttenuation);
	m_wndPropList.AddProperty(m_pPropRange);
	m_wndPropList.AddProperty(m_pPropBindBone);
}

void CLightBar::SaveCurrentProp()
{
	//CCvtHelper cvt;
	//USES_CONVERSION;
	//m_pTarget->m_Type		= cvt.StringToLight(OLE2T((_bstr_t)m_pPropType->GetValue()));
	//m_pTarget->m_Color		= cvt.ColorIntToFloat(m_pPropColor->GetColor());
	//m_pTarget->m_Attenuation= (float)m_pPropAttenuation->GetValue();
	//m_pTarget->m_Range		= (float)m_pPropRange->GetValue();
	//cvt.ParseInt(OLE2T((_bstr_t)m_pPropBindBone->GetValue()), (int*)&m_pTarget->m_bindInfo.boneid);
	DataUpdate(PROP_TO_DATA);
}

void CLightBar::UpdateList()
{
	//CCvtHelper cvt;
	//m_pPropType		->SetValue(cvt.LightToString(m_pTarget->m_Type).GetBuffer());
	//m_pPropColor	->SetColor(cvt.ColorFloatToInt(m_pTarget->m_Color));
	//m_pPropAttenuation->SetValue((float)m_pTarget->m_Attenuation);
	//m_pPropRange	->SetValue((float)m_pTarget->m_Range);
	//m_pPropBindBone	->SetValue(cvt.IntToString(m_pTarget->m_bindInfo.boneid));

	DataUpdate(DATA_TO_PROP);

}

void CLightBar::DataUpdate(DATA_UPDATA_TYPE type)
{

	PropUpdate_float((CBCGPProp**)&m_pPropOffsetX, &m_pTarget->m_bindInfo.offset.x, type, _T("X"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetY, &m_pTarget->m_bindInfo.offset.y, type, _T("Y"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetZ, &m_pTarget->m_bindInfo.offset.z, type, _T("Z"));

	PropUpdate_lighttype((CBCGPProp**)&m_pPropType,		&m_pTarget->m_Type,				type, _T("光源类型"));
	PropUpdate_color((CBCGPColorProp**)&m_pPropColor,			&m_pTarget->m_Color,				type, _T("光源颜色"));
	PropUpdate_float((CBCGPProp**)&m_pPropAttenuation,	&m_pTarget->m_Attenuation,		type, _T("衰减"));
	PropUpdate_float((CBCGPProp**)&m_pPropRange,			&m_pTarget->m_Range,				type, _T("范围"), 1.0f, 32767.0f, 0);
	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone,	&m_pTarget->m_bindInfo.boneid,	type, _T("绑定点"));
}

void CLightBar::PropUpdate_lighttype(CBCGPProp** ppProp, Ogre::LightType* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::LightType)cvt.StringToLight(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else  if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue( (_bstr_t)cvt.LightToString(*data) );
	}
}