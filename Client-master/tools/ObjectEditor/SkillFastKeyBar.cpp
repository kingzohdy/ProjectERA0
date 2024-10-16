
#include "stdafx.h"
#include "SkillFastKeyBar.h"
#include "SkillTemplateEditBar.h"
#include "EffectUnitEditBar.h"
#include "NotifyProperty.h"

IMPLEMENT_DYNAMIC(CSkillFastKeyBar, CBCGPDockingControlBar)

CSkillFastKeyBar::CSkillFastKeyBar()
{}

CSkillFastKeyBar::~CSkillFastKeyBar()
{
	for(int i=0; i<(int)vProps.size(); i++)
		delete vProps[i];
	vProps.clear();
}

BEGIN_MESSAGE_MAP(CSkillFastKeyBar, CBCGPDockingControlBar)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CSkillFastKeyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	m_wndSkillFSPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_SKILL_FAST_KEY);
	m_wndSkillFSPropList.EnableHeaderCtrl(FALSE);
	return 0;
}

void CSkillFastKeyBar::ClearAll()
{
	m_wndSkillFSPropList.RemoveAll();
	mGroup.clear();
	for(int i=0; i<(int)vProps.size(); i++)
		delete vProps[i];
	vProps.clear();
}

void CSkillFastKeyBar::UpdatePropsWithData(WPARAM wParam, LPARAM lParam)
{
	ClearAll();

	pvSkillsFastKey = (std::vector<SkillsFastKey*>*)wParam;
	for(int i=0; i<(int)pvSkillsFastKey->size(); i++)
	{
		CBCGPProp* pBCGPProp; //组
		SkillsFastKey* pSFK = (*pvSkillsFastKey)[i];
		std::map<std::string, CBCGPProp*>::iterator it = mGroup.find(pSFK->stSkillTemplateName);
		if(it == mGroup.end())
		{
			pBCGPProp = new CBCGPProp(pSFK->stSkillTemplateName.c_str());
			mGroup[pSFK->stSkillTemplateName] = pBCGPProp;
		}
		else
			pBCGPProp = it->second;
		FastKeyProps* pFKP = new FastKeyProps;
		pFKP->stTemplateName = pSFK->stSkillTemplateName;
		pFKP->stSkillName = pSFK->stSkillName;
		pFKP->stNum = pSFK->stNum;
		vProps.push_back(pFKP);

		CEffectEditBar::PropUpdate_int((CBCGPProp**)(&(pFKP->pProp)), &(pFKP->stNum), CREATE_PROP, pFKP->stSkillName.c_str(), 1, 9 ,0);
		pFKP->pProp->SetNotifyTarget(GetSafeHwnd());
		CEffectEditBar::PropUpdate_int((CBCGPProp**)(&(pFKP->pProp)), &(pFKP->stNum), DATA_TO_PROP, pFKP->stSkillName.c_str(), 1, 9 ,0);

		pBCGPProp->AddSubItem(pFKP->pProp);
	}
	std::map<std::string, CBCGPProp*>::iterator it2 = mGroup.begin();
	for(; it2 != mGroup.end(); it2++)
	{
		m_wndSkillFSPropList.AddProperty(it2->second);
	}
}
#undef CString
LRESULT CSkillFastKeyBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	for(int i=0; i<(int)vProps.size(); i++)
	{
		if(pnm->pProp == vProps[i]->pProp)
		{
			FastKeyProps* pFKP = vProps[i];
			int nIn = -1;
			CEffectEditBar::PropUpdate_int((CBCGPProp**)(&(pFKP->pProp)), &(nIn), PROP_TO_DATA, pFKP->stSkillName.c_str(), 1, 9 ,0);
			if(nIn>9 || nIn<0)
			{
				MessageBox("快捷键值应该在 0--9 之间");
				CEffectEditBar::PropUpdate_int((CBCGPProp**)(&(pFKP->pProp)), &(pFKP->stNum), DATA_TO_PROP, pFKP->stSkillName.c_str(), 1, 9 ,0);
				return (LRESULT)0;
			}
			//检查此键值是否被使用
			int k;
			for(k=0; k<(int)pvSkillsFastKey->size(); k++)
			{
				if(nIn == (*pvSkillsFastKey)[k]->stNum)
					break;
			}
			if(k != (int)pvSkillsFastKey->size())
			{
				CString strErr;
				strErr.Format("键值 %d 已经被 %s : %s 使用", nIn, (*pvSkillsFastKey)[k]->stSkillTemplateName.c_str(), (*pvSkillsFastKey)[k]->stSkillName.c_str());
				CEffectEditBar::PropUpdate_int((CBCGPProp**)(&(pFKP->pProp)), &(pFKP->stNum), DATA_TO_PROP, pFKP->stSkillName.c_str(), 1, 9 ,0);
				MessageBox(strErr);
				return (LRESULT)0;
			}
			pFKP->stNum = nIn;
			CEffectEditBar::PropUpdate_int((CBCGPProp**)(&(pFKP->pProp)), &(pFKP->stNum), DATA_TO_PROP, pFKP->stSkillName.c_str(), 1, 9 ,0);

			m_pLocalSetParams = new SkillsFastKey;
			m_pLocalSetParams->stSkillTemplateName = pFKP->stTemplateName;
			m_pLocalSetParams->stSkillName = pFKP->stSkillName;
			m_pLocalSetParams->stNum = pFKP->stNum;
			SendMainFrameMsg(ID_SKILL_FAST_KEY_REVERSE, (WPARAM)m_pLocalSetParams, 0);
			break;
		}
	}
	return (LRESULT)0;
}

void CSkillFastKeyBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	m_wndSkillFSPropList.SetWindowPos(NULL, 0, 0,
		rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
}
