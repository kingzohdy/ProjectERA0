// LoadAvatarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "LoadAvatarDlg.h"
#include "ObjectTypeHelper.h"
#include "NotifyProperty.h"
#include "AvatarShowInfo.h"

#include "AvatarConfig.h"
#include "AvatarManager.h"

using namespace Ogre;
// CLoadAvatarDlg dialog

IMPLEMENT_DYNAMIC(CLoadAvatarDlg, CDialog)

CLoadAvatarDlg::CLoadAvatarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadAvatarDlg::IDD, pParent)
{

}

CLoadAvatarDlg::~CLoadAvatarDlg()
{
}

void CLoadAvatarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoadAvatarDlg, CDialog)
END_MESSAGE_MAP()


// CLoadAvatarDlg message handlers
CBCGPProp* CreateProp(CVarInfo* pVarInfo)
{
	CCvtHelper cvt;
	CString varName = cvt.Utf8ToGb((char*)pVarInfo->GetVarDesc().c_str());
	if(pVarInfo->GetVarType() == CVarInfo::ENUMVAR)
	{
		CNotifiedProp* pProp = new CNotifiedProp(varName, _T(""));
		//pProp->SetNotifyTarget(GetSafeHwnd());
		CEnumVarInfo* pEnumVar = (CEnumVarInfo*)pVarInfo;
		for(int i = 0; i < (int)pEnumVar->m_enums.size(); i++)
		{
			CString enumDesc = cvt.Utf8ToGb(pEnumVar->m_enums[i].strDesc.c_str());
			pProp->AddOption(enumDesc.GetBuffer());
		}
		pProp->SetData(pVarInfo->GetVarId());
		pProp->SetValue(cvt.Utf8ToGb(pEnumVar->GetValueDesc(0).c_str()));
		return pProp;
	}
	else
	{
		CNotifiedProp* pProp = new CNotifiedProp(varName, _T(""));
		//pProp->SetNotifyTarget(GetSafeHwnd());
		pProp->SetData(pVarInfo->GetVarId());
		pProp->SetValue(cvt.Utf8ToGb(pVarInfo->GetValueDesc(0).c_str()));
		return pProp;
	}
	return NULL;
}


// CLoadAvatarDlg message handlers

BOOL CLoadAvatarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_wndPropList.Create(WS_VISIBLE, CRect(20, 50, 330, 280), this, ID_AVAMODPROP))
	{
		return -1;
	}

	CCvtHelper cvt;
	for(int i = 0; i < (int) m_pShowInfo->m_pAvatarShow->m_Config.m_VarList.size(); i++)
	{
		CVarInfo *pvarinfo = m_pShowInfo->m_pAvatarShow->m_Config.m_VarList[i];
		if(pvarinfo->m_AffectModelPath)
		{
			CBCGPProp* pProp = CreateProp(m_pShowInfo->m_pAvatarShow->m_Config.m_VarList[i]);
			m_wndPropList.AddProperty(pProp);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}



void CLoadAvatarDlg::OnOK()
{
	ZeroMemory(m_attrib, sizeof(int) * 20);
	CCvtHelper cvt;
	for(int i = 0; i < m_wndPropList.GetPropertyCount(); i++)
	{
		CBCGPProp* pProp = m_wndPropList.GetProperty(i);
		int id = pProp->GetData();
		ASSERT(id >=0 && id < 20);

		CVarInfo* pVar = m_pShowInfo->m_pAvatarShow->m_Config.FindVarById(id);

		int val = pVar->GetValueByDesc(cvt.GbToUtf8((_bstr_t)pProp->GetValue()));
		m_attrib[id] = val;
	}
	m_attrib[0] = 1;
	CDialog::OnOK();
}