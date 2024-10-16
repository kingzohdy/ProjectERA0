
#include "stdafx.h"
#include "EffectUnit_Sound.h"
#include "EffectUnitEditBar_Sound.h"
#include "EffectManager.h"
#include "NotifyProperty.h"
#include "ObjectTypeHelper.h"
#include "ControlsHelper.h"

IMPLEMENT_DYNAMIC(CSoundBar, CEffectEditBar)
CSoundBar::CSoundBar() : m_bLockApp(true)
{
}

CSoundBar::~CSoundBar()
{
}


BEGIN_MESSAGE_MAP(CSoundBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	//ON_COMMAND(ID_BTN_SND_VISIBLE, OnVisible)
	ON_UPDATE_COMMAND_UI(ID_BTN_SND_VISIBLE, OnUpdateVisible)
	ON_COMMAND(ID_BTN_SND_APPLY, OnApply)
	ON_COMMAND(ID_BTN_SND_SAVE, OnSave)
	ON_COMMAND(ID_BTN_SND_REMOVE, OnRemove)
	ON_COMMAND(ID_BTN_SND_CLICK, OnBtnClick)

	ON_COMMAND(ID_BTN_SND_LOCKAPP, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BTN_SND_LOCKAPP, OnUpdateBtnLockApply)

	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickPropBtn)

END_MESSAGE_MAP()



// CSoundBar message handlers


int CSoundBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this)
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SOUND))
	{
		return -1;
	}
	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_SOUNDPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);

	CreateProp();
	return 0;
}

void CSoundBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndPropList.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CSoundBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->m_bVisible ? 1 : 0);
}

void CSoundBar::OnVisible()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();

	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg(EEM_APPLY);
}

void CSoundBar::OnApply()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	//m_pTarget->RefreshNode();
}

void CSoundBar::OnBtnClick()
{
	m_pTarget->GetInteract()->OnLBtnDown(0.0f, 0.0f, true);
	CEffectMgr::GetInstance()->SetActiveEffect(m_pTarget);
}

void CSoundBar::OnSave()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();
	SendMainFrameMsg( EEM_SAVE, 0, (LPARAM)m_pTarget);
}

void CSoundBar::OnRemove()
{
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CSoundBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CSoundBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}



LRESULT CSoundBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	SaveCurrentProp();
	m_pTarget->UpdateSource();

	if(m_bLockApp && 
		(  pnm->pProp == m_pPropMaxDst
		|| pnm->pProp == m_pPropBindBone
		|| pnm->pProp == m_pPropMinDst
		|| pnm->pProp == m_pPropFileName
		))
	{
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);

	}

	char tmp[1024] = {0};
	::GetCurrentDirectory(1024, tmp);
	//TRACE("%s\n", tmp);

	return 0;
}

LRESULT CSoundBar::OnClickPropBtn(WPARAM w, LPARAM l)
{
	PROPNM* pnm = (PROPNM*)l;
	CCvtHelper cvt;

	if(pnm->pProp == m_pPropFileName)
	{
		CString path = GetOpenFilePath(TRUE, "sound", "", "wav", "*.wav", TRUE);
		if(!path.IsEmpty())
		{
			m_pTarget->m_strSoundFile = path;
			UpdateList();
			m_pTarget->UpdateSource();
		}
	}    
	return 0;
}

void CSoundBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = (CSound*)pEffect;
	if(GetSafeHwnd())
	{
		UpdateList();
		SetWindowText(m_pTarget->m_strName.GetBuffer());
	}

}

CEffectUnit* CSoundBar::GetEditTarget()
{
	return m_pTarget;
}


void CSoundBar::CreateProp()
{
	DataUpdate(CREATE_PROP);

	CBCGPProp* pOffset = new CBCGPProp("相对位置");
	pOffset->AddSubItem(m_pPropOffsetX);
	pOffset->AddSubItem(m_pPropOffsetY);
	pOffset->AddSubItem(m_pPropOffsetZ);

	m_pPropOffsetX->SetNotifyTarget(GetSafeHwnd());
	m_pPropOffsetY->SetNotifyTarget(GetSafeHwnd());
	m_pPropOffsetZ->SetNotifyTarget(GetSafeHwnd());
	m_pPropMinDst->SetNotifyTarget(GetSafeHwnd());
	m_pPropMaxDst->SetNotifyTarget(GetSafeHwnd());
	m_pPropVolume->SetNotifyTarget(GetSafeHwnd());
	m_pPropVel->SetNotifyTarget(GetSafeHwnd());
	m_pPropLoop->SetNotifyTarget(GetSafeHwnd());
	m_pPropBindBone->SetNotifyTarget(GetSafeHwnd());
	m_pPropPlayMode->SetNotifyTarget(GetSafeHwnd());

	m_pPropVolume->EnableSpinControl(TRUE, 0, 10);

	m_wndPropList.AddProperty(pOffset);
	m_wndPropList.AddProperty(m_pPropFileName);
	m_wndPropList.AddProperty(m_pPropMinDst);
	m_wndPropList.AddProperty(m_pPropMaxDst);
	m_wndPropList.AddProperty(m_pPropVolume);
	m_wndPropList.AddProperty(m_pPropVel);
	m_wndPropList.AddProperty(m_pPropLoop);
	m_wndPropList.AddProperty(m_pPropPlayMode);
	m_wndPropList.AddProperty(m_pPropBindBone);

}

void CSoundBar::SaveCurrentProp()
{
	DataUpdate(PROP_TO_DATA);
}

void CSoundBar::UpdateList()
{
	DataUpdate(DATA_TO_PROP);
}

void CSoundBar::DataUpdate(DATA_UPDATA_TYPE type)
{
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetX,	&m_pTarget->m_bindInfo.offset.x, type, _T("X"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetY,	&m_pTarget->m_bindInfo.offset.y, type, _T("Y"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetZ,	&m_pTarget->m_bindInfo.offset.z, type, _T("Z"));
	PropUpdate_float((CBCGPProp**)&m_pPropMinDst,	&m_pTarget->m_createInfo.mindistance, type, _T("最近作用距离"));
	PropUpdate_float((CBCGPProp**)&m_pPropMaxDst,	&m_pTarget->m_createInfo.maxdistance, type, _T("最远作用距离"));
	PropUpdate_int  ((CBCGPProp**)&m_pPropVolume,	&m_pTarget->m_iVolumeInt, type, _T("音量"));
	PropUpdate_v3   ((CBCGPProp**)&m_pPropVel,		&m_pTarget->m_createInfo.velocity, type, _T("速度"));
	PropUpdate_bool ((CBCGPProp**)&m_pPropLoop,		&m_pTarget->m_createInfo.isloop, type, _T("循环播放"));
	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone, &m_pTarget->m_bindInfo.boneid, type, _T("绑定点"));
	PropUpdate_soundplaymode((CBCGPProp**)&m_pPropPlayMode, &m_pTarget->m_PlayMode, type, _T("播放模式"));

	PropUpdate_file((CBCGPProp**)&m_pPropFileName, &m_pTarget->m_strSoundFile, type, _T("声音文件"));

	m_pTarget->m_createInfo.time0 = 0.0f;
	m_pTarget->m_createInfo.time1 = 0.0f;
	m_pTarget->m_createInfo.fSoundFullRange = 0.0f;
}

void CSoundBar::PropUpdate_soundplaymode(CBCGPProp** ppProp, Ogre::SOUND_PLAY_MODE* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		std::vector<CString> modes;
		(*ppProp) = new CNotifiedProp(name, _T(""));
		cvt.EnumSoundPlayMode(modes);
		for(int i = 0; i < (int) modes.size(); i++)
		{
			(*ppProp)->AddOption(modes[i].GetBuffer());
		}
	}
	else if(type == PROP_TO_DATA)
	{
		(*data) = (Ogre::SOUND_PLAY_MODE)cvt.StringToSoundPlayMode(OLE2T((_bstr_t)(*ppProp)->GetValue()));
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.SoundPlayModeToStringAsPtr((*data)));
	}
}
void CSoundBar::PropUpdate_file(CBCGPProp** ppProp, CString* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""), TRUE, GetSafeHwnd());
	}
	else 
	{
		PropUpdate_cstring(ppProp, data, type, name);
	}
}

