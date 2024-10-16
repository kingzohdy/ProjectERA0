
#include "stdafx.h"
#include "UnknownObjectEditBar.h"
#include "NotifyProperty.h"
#include "EffectUnit.h"
#include "EffectManager.h"
#include "ObjectTypeHelper.h"
#include "InteractObject.h"
#include "BindInfo.h"
#include "EffectUnit_UnknownObject.h"

#include "OgreEntityData.h"
#include "OgreEntity.h"
#include "OgreSkeletonAnimData.h"
#include "OgreResourceManager.h"

IMPLEMENT_DYNAMIC(CUnknownObjectBar, CEffectEditBar)

CUnknownObjectBar::CUnknownObjectBar(void): m_pTarget(NULL), m_bLockApp(true)
{
}

CUnknownObjectBar::~CUnknownObjectBar(void)
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CUnknownObjectBar, CEffectEditBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_UPDATE_COMMAND_UI(ID_BUTTON_UNOBJ_VISIBLE, OnUpdateVisible)
	ON_COMMAND(ID_BUTTON_UNOBJ_APPLY, OnApply)
	ON_COMMAND(ID_BUTTON_UNOBJ_REMOVE, OnRemove)
	ON_COMMAND(ID_BUTTON_UNOBJ_CLICK, OnBtnClick)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_COMMAND(ID_BUTTON_UNOBJ_LOCKAPPLY, OnBtnLockApply)	
	ON_UPDATE_COMMAND_UI(ID_BUTTON_UNOBJ_LOCKAPPLY, OnUpdateBtnLockApply)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)
END_MESSAGE_MAP()

// CLightBar message handlers


int CUnknownObjectBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEffectEditBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.Create(this)
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_UNKNOWNOBJ))
	{
		return -1;
	}
	//m_wndPropList.
	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_UNKNOWNPROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);

	CreateProp();

	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());
	return 0;
}

LRESULT CUnknownObjectBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	SaveCurrentProp();
	m_pTarget->UpdateSource();
	if(pnm->pProp == static_cast<CBCGPProp*>(m_pPropBindBone)
		|| pnm->pProp == static_cast<CBCGPProp*>(m_pPropAnimName)
		|| pnm->pProp == static_cast<CBCGPProp*>(m_pPropAnimMode)
		|| pnm->pProp == static_cast<CBCGPProp*>(m_pPropResPath))
	{
		//m_pTarget->RefreshNode();
		CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
	}

	return 0;
}

bool IsUnknownEffectType(const char *path)
{
	if(strstr(path, ".und") != NULL) return true;
	else return false;
}

LRESULT CUnknownObjectBar::OnDropRes(WPARAM w, LPARAM l)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)l;
	if(pdn->hWnd == m_wndPropList.GetSafeHwnd())
	{
		CCvtHelper cvt;

		//if(!IsUnknownEffectType(pdn->strChunk))
		//	return  0;

		if(m_pPropResPath->GetRect().PtInRect(pdn->dropPos))
		{
			m_pPropResPath ->SetValue(pdn->strChunk);
			PROPNM nm;
			nm.pProp = (CNotifiedProp*)m_pPropResPath;
			OnUpdatePropValue(0, (LPARAM)&nm);
		}
	}
	return 0;
}

void CUnknownObjectBar::OnSize(UINT nType, int cx, int cy)
{
	CEffectEditBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndPropList.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CUnknownObjectBar::OnVisible()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();

	m_pTarget->SetVisible(!m_pTarget->IsVisible());
	PostMainFrameMsg(EEM_APPLY);
}

void CUnknownObjectBar::OnUpdateVisible(CCmdUI* pUI)
{
	pUI->SetCheck(m_pTarget->IsVisible() ? 1 : 0);
}

void CUnknownObjectBar::OnApply()
{
	m_wndPropList.EndEditItem();
	SaveCurrentProp();
	//PostMainFrameMsg(EEM_APPLY);
	//m_pTarget->RefreshNode();
	CEffectMgr::GetInstance()->RefreshEffect(m_pTarget);
}



void CUnknownObjectBar::OnRemove()
{
	PostMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM) m_pTarget);
}

void CUnknownObjectBar::OnBtnClick()
{
	m_pTarget->GetInteract()->OnLBtnDown(0.0f, 0.0f, true);
	CEffectMgr::GetInstance()->SetActiveEffect(m_pTarget);
}

void CUnknownObjectBar::OnBtnLockApply()
{
	m_bLockApp = !m_bLockApp;
}

void CUnknownObjectBar::OnUpdateBtnLockApply(CCmdUI* pUI)
{
	pUI->SetCheck(m_bLockApp ? 1 : 0);
}

void CUnknownObjectBar::SetEditTarget(CEffectUnit* pEffect)
{
	m_pTarget = pEffect;
	if(GetSafeHwnd())
	{
		CreateProp();
		UpdateList();
		SetWindowText(m_pTarget->GetName().GetBuffer());

	}
}

CEffectUnit* CUnknownObjectBar::GetEditTarget()
{
	return m_pTarget;;
}

static bool FindStringFromArray(std::vector<CString>&strarray, CString &str)
{
	for(size_t k = 0; k<strarray.size(); k++)
	{
		CString strExist = strarray[k];
		strExist.MakeUpper();

		if(str == strExist)
		{
			return true;
		}
	}
	return false;
}

void CUnknownObjectBar::AddAnimNames()
{
	if(m_pTarget == NULL)
	{
		return;
	}

	Ogre::Resource* pRes = m_pTarget->GetSource();
	if(pRes == NULL)
	{
		return;
	}

	std::vector<CString> animNames;
	if(IS_KIND_OF(Ogre::Entity, pRes))
	{
		Ogre::EntityData* pModelSource = static_cast<Ogre::EntityData*>(pRes);

		//添加骨骼动画
		if(pModelSource->m_pMainResource)
		{
			AddAnimFromSkinActor(static_cast<Ogre::ModelData*>(pModelSource->m_pMainResource), animNames);
		}

		//添加Motion
		for(int i = 0; i < (int) pModelSource->m_Motions.size(); i++)
		{
			CString strCurName = pModelSource->m_Motions[i]->m_strName;
			strCurName.MakeUpper();
			if(!FindStringFromArray(animNames, strCurName))
			{
				animNames.push_back(CString((const char *)pModelSource->m_Motions[i]->m_strName));
			}
		}

		for(int i=0; i<(int)pModelSource->m_AddtionAnims.size(); i++)
		{
			CString strCurName = pModelSource->m_AddtionAnims[i]->m_strName;
			strCurName.MakeUpper();
			if(!FindStringFromArray(animNames, strCurName))
			{
				animNames.push_back(CString((const char *)pModelSource->m_AddtionAnims[i]->m_strName));
			}
		}
	}
	else if(IS_KIND_OF(Ogre::ModelData, pRes))
	{
		AddAnimFromSkinActor(static_cast<Ogre::ModelData*>(pRes), animNames);
	}

	for(int i = 0; i < (int) animNames.size(); i++)
	{
		m_pPropAnimName->AddOption(animNames[i].GetBuffer());
	}
}

void CUnknownObjectBar::AddAnimFromSkinActor(Ogre::ModelData* pActorSource, std::vector<CString>& names)
{
	std::vector<Ogre::SequenceDesc>seqarray;
	pActorSource->getAllSequence(seqarray);
	for(size_t iseq=0; iseq<seqarray.size(); iseq++)
	{
		int id = seqarray[iseq].id;
		CString strName;
		strName.Format("%d", id);
		names.push_back(strName);
	}
}

void CUnknownObjectBar::CreateProp()
{
	m_wndPropList.RemoveAll();

	DataUpdate(CREATE_PROP);


	CBCGPProp* pPos = new CBCGPProp(_T("相对位置"));
	pPos->AddSubItem(m_pPropOffsetX);
	pPos->AddSubItem(m_pPropOffsetY);
	pPos->AddSubItem(m_pPropOffsetZ);

	CBCGPProp* pRot = new CBCGPProp(_T("旋转"));
	pRot->AddSubItem(m_pPropYaw);
	pRot->AddSubItem(m_pPropPitch);
	pRot->AddSubItem(m_pPropRoll);

	CBCGPProp* pScale = new CBCGPProp(_T("缩放"));
	pScale->AddSubItem(m_pPropScaleX);
	pScale->AddSubItem(m_pPropScaleY);
	pScale->AddSubItem(m_pPropScaleZ);

	m_pPropOffsetX->SetNotifyTarget(GetSafeHwnd());
	m_pPropOffsetY->SetNotifyTarget(GetSafeHwnd());
	m_pPropOffsetZ->SetNotifyTarget(GetSafeHwnd());

	m_pPropYaw->SetNotifyTarget(GetSafeHwnd());
	m_pPropPitch->SetNotifyTarget(GetSafeHwnd());
	m_pPropRoll->SetNotifyTarget(GetSafeHwnd());

	m_pPropScaleX->SetNotifyTarget(GetSafeHwnd());
	m_pPropScaleY->SetNotifyTarget(GetSafeHwnd());
	m_pPropScaleZ->SetNotifyTarget(GetSafeHwnd());

	m_pPropAnimName->SetNotifyTarget(GetSafeHwnd());
	m_pPropAnimMode->SetNotifyTarget(GetSafeHwnd());


	((CNotifiedProp*)m_pPropBindBone)->SetNotifyTarget(GetSafeHwnd());

	AddAnimNames();

	m_wndPropList.AddProperty(pPos);
	m_wndPropList.AddProperty(pRot);
	m_wndPropList.AddProperty(pScale);
	m_wndPropList.AddProperty(m_pPropResPath);
	m_wndPropList.AddProperty(m_pPropAnimMode);
	m_wndPropList.AddProperty(m_pPropAnimName);
	m_wndPropList.AddProperty(m_pPropBindBone);
	//m_pPropResPath->Enable(FALSE);
}

void CUnknownObjectBar::SaveCurrentProp()
{
	DataUpdate(PROP_TO_DATA);
}

void CUnknownObjectBar::UpdateList()
{
	DataUpdate(DATA_TO_PROP);

}

void CUnknownObjectBar::DataUpdate(DATA_UPDATA_TYPE type)
{
	BIND_INFO* pInfo;
	BIND_INFO tmp;

	if(m_pTarget != NULL)
		pInfo = m_pTarget->GetBindInfo();
	else
		pInfo = &tmp;

	PropUpdate_float((CBCGPProp**)&m_pPropOffsetX, &pInfo->offset.x, type, _T("X"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetY, &pInfo->offset.y, type, _T("Y"));
	PropUpdate_float((CBCGPProp**)&m_pPropOffsetZ, &pInfo->offset.z, type, _T("Z"));

	if(type == PROP_TO_DATA)
	{
		float y,p,r;
		//C3DMath::QuaternionToEular(*data, &y, &p, &r);
		r = (float)(m_pPropRoll->GetValue());
		p = (float)(m_pPropPitch->GetValue());
		y = (float)(m_pPropYaw->GetValue());

		if(r == 0.0f && p == 0.0f && y == 0.0f)
		{
			m_pTarget->GetBindInfo()->rotate.setEulerAngle(y, p, r);
		}
	}
	else
	{
		PropUpdate_eular_yaw  ((CBCGPProp**)&m_pPropYaw, &pInfo->rotate, type, _T("转角"));
		PropUpdate_eular_pitch((CBCGPProp**)&m_pPropPitch, &pInfo->rotate, type, _T("俯仰角"));
		PropUpdate_eular_roll ((CBCGPProp**)&m_pPropRoll, &pInfo->rotate, type, _T("侧滚角"));
	}

	PropUpdate_float((CBCGPProp**)&m_pPropScaleX, &pInfo->scale.x, type, _T("X") , 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropScaleY, &pInfo->scale.y, type, _T("Y") , 0.01f, 32767.0f, 0.01f);
	PropUpdate_float((CBCGPProp**)&m_pPropScaleZ, &pInfo->scale.z, type, _T("Z") , 0.01f, 32767.0f, 0.01f);

	//if(type != PROP_TO_DATA)
	//{
	PropUpdate_cstring((CBCGPProp**)&m_pPropResPath, &static_cast<CUnknownObject*>(m_pTarget)->m_strResPath , type, _T("资源位置"));
	//}

	PropUpdate_bindbone((CBCGPProp**)&m_pPropBindBone,	&pInfo->boneid,	type, _T("绑定点"));

	PropUpdate_animmode((CBCGPProp**)&m_pPropAnimMode, &pInfo->animmode, type, _T("动画播放模式"));
	PropUpdate_cstring((CBCGPProp**)&m_pPropAnimName, &pInfo->animname, type, _T("独立动画名"));
}

void CUnknownObjectBar::PropUpdate_animmode(CBCGPProp** ppProp, Ogre::BIND_ANIM_MODE* data, DATA_UPDATA_TYPE type, CString name)
{
	CCvtHelper cvt;
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		std::vector<CString> animmodes;
		(*ppProp) = new CNotifiedProp(name, _T(""));
		cvt.EnumAnimMode(animmodes);
		for(int i = 0; i < (int) animmodes.size(); i++)
		{
			(*ppProp)->AddOption(animmodes[i].GetBuffer());
		}
	}
	else if(type == PROP_TO_DATA)
	{
		CString str = OLE2T((_bstr_t)(*ppProp)->GetValue());
		(*data) = (Ogre::BIND_ANIM_MODE)cvt.StringToAnimMode(str);
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue((_bstr_t)cvt.AnimModeToStringAsPtr((*data)));
	}
}
