
#include "stdafx.h"
#include "AvatarShowBar.h"
#include "NotifyProperty.h"
#include "AvatarShowInfo.h"
#include "ObjectTypeHelper_Avatar.h"
#include "DBManager.h"
#include "LoadAvatarDlg.h"
#include "EffectManager.h"
#include "AvatarShowInfo.h"
#include "NpcLoadSaveDlg.h"
#include "ObjectEditor.h"
#include "MainFrm.h"
#include "NPCManager.h"

#include "AvatarManager.h"
#include "GameResDBTypes.h"
#include "OgreResDB.h"

using namespace Ogre;

IMPLEMENT_DYNAMIC(CAvatarShowBar, CBCGPDockingControlBar)
CAvatarShowBar::CAvatarShowBar() : m_pAvatarShowInfo(NULL)
{
}

CAvatarShowBar::~CAvatarShowBar()
{
}


BEGIN_MESSAGE_MAP(CAvatarShowBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_COMMAND(ID_BTN_AVA_LOADMOD, OnLoadMod)
	ON_COMMAND(ID_BTN_AVA_SAVEASNPC, OnSaveNPC)
	ON_COMMAND(ID_BTN_AVA_LOADNPC, OnLoadNPC)

	ON_COMMAND(ID_BTN_AVA_ENABLEAVA, OnBtnEnable)
	ON_UPDATE_COMMAND_UI(ID_BTN_AVA_ENABLEAVA, OnUpdateBtnEnable)

	ON_MESSAGE(EEM_DROPITEM, OnDropItem)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
END_MESSAGE_MAP()



// CAvatarShowBar message handlers


int CAvatarShowBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolbar.Create(this) || !m_wndToolbar.LoadToolBar(IDR_TOOLBAR_AVATAR))
	{
		return  -1;
	}

	if(!m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_AVAPROP))
	{
		return -1;
	}
	//LoadPropItem();

	m_dropTarget.Register(&m_wndPropList);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	m_bConfigCreated = false;
	return 0;
}

void CAvatarShowBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndPropList.SetWindowPos(NULL, 0, 24,
		frameRect.Width(), frameRect.Height() - 24 , SWP_NOZORDER);

	m_wndToolbar.SetWindowPos(NULL, 0, 0,
		frameRect.Width(), 24, SWP_NOZORDER);
}

LRESULT CAvatarShowBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	USES_CONVERSION;
	AvatarManager* pAvatarShow = AvatarManager::getSingletonPtr();
	int id = pnm->pProp->GetData();
	CVarInfo* pVar = pAvatarShow->m_Config.FindVarById(id);

	if(pAvatarShow->m_Config.IsNormalVar(id))
	{
		m_pAvatarShowInfo->SetParamValue(pVar->GetVarId(), OLE2T((_bstr_t)pnm->pProp->GetValue()));
		SendMainFrameMsg(EEM_AVATAR_UPDATED);
	}

	return 0;
}
LRESULT CAvatarShowBar::OnDropItem(WPARAM w, LPARAM l)
{
	CItemDropTarget::DROPNOTIFY* pdn = (CItemDropTarget::DROPNOTIFY*)l;
	CBCGPProp* pProp = CItemDropPropListTarget::GetHoveringProp(&m_wndPropList, pdn->dropPos);
	CAvatarCvtHelper cvt;
	if(pProp != NULL)
	{
		int varid = (int) pProp->GetData();
		Ogre::ResTable* pTable =  CDataBase::GetInstance()->GetTable("ItemInfo");
		if(pTable == NULL)
		{
			CString str;
			str.Format("pTable == NULL ！！！！");
			MessageBox(str.GetBuffer());
			return 0;
		}
		ITEMDEF itemInfoSrc;
		ITEMDEF itemInfo;
		itemInfoSrc.ItemID = pdn->itemId;
		if( pTable->FindRecord(&itemInfo, 1, &itemInfoSrc) <= 0)
		{	
			CString str;
			str.Format("找不到物品id: %d", pdn->itemId);
			MessageBox(str.GetBuffer());
			return 0;
		}

		if(cvt.TypeFitSlot(itemInfo.ItemType, varid))
		{
			m_pAvatarShowInfo->SetParamValue(varid, itemInfo.ItemID);
			UpdateList();
			SendMainFrameMsg(EEM_AVATAR_UPDATED);
		}
		else
		{
			CString str;
			str.Format("物品类型 %d 不匹配装备位 %d", itemInfo.ItemType, varid);
			MessageBox(str.GetBuffer());
		}
	}
	else
	{
		CString str;
		str.Format("pProp == NULL ！！！！");
		MessageBox(str.GetBuffer());
	}

	return 0;
}
void CAvatarShowBar::OnLoadMod()
{
	CLoadAvatarDlg dlg;
	dlg.m_pShowInfo = m_pAvatarShowInfo;
	if(IDOK == dlg.DoModal())
	{
		m_pAvatarShowInfo->EnableAvatar(true);
		SendMainFrameMsg(EEM_REQ_LOAD_AVATAR, 0,(LPARAM) dlg.m_attrib);
		//m_pAvatarShowInfo->SetAllParams(dlg.m_attrib, true);
		UpdateList();
		SwitchPropState(TRUE);

	}
}

void CAvatarShowBar::OnSaveNPC()
{
	if(CEffectMgr::GetInstance()->m_avatarShowInfo->IsEnable() && CEffectMgr::GetInstance()->m_Effects.size() != 0)
	{
		if(IDNO == MessageBox("存入NPC表后将不包括所编辑的特效，继续吗?", "提示", MB_YESNO)) return;
	}

	CNpcLoadSaveDlg npcdlg;
	if(IDOK == npcdlg.DoModal())
	{
		NPCDISPLAYINFO npc;
		ZeroMemory(&npc, sizeof(NPCDISPLAYINFO));
		npc.id = npcdlg.m_NpcID;
		assert(npcdlg.m_NpcName.GetLength() < sizeof(npc.name));
		strcpy(npc.name, npcdlg.m_NpcName.GetBuffer());
		strcpy(npc.npcdesc, npcdlg.m_NpcDesc.GetBuffer());
		npc.scale = npcdlg.m_NpcScale;
		npc.alpha = npcdlg.m_NpcXparent;
		npc.height = npcdlg.m_NpcHeight;
		npc.radius = npcdlg.m_NpcRadius;
		if(CEffectMgr::GetInstance()->m_avatarShowInfo->IsEnable())
		{
			int *pattr = &CEffectMgr::GetInstance()->m_avatarShowInfo->m_curAttrib[0];
			npc.race = pattr[0];
			npc.gender = pattr[1];

			int num = sizeof(npc.userdata)/sizeof(npc.userdata[0]);
			if(num > MAX_AVATAR_FIELD-2) num = MAX_AVATAR_FIELD-2;
			for(int i=0; i<num; i++)
			{
				npc.userdata[i] = pattr[i+2];
			}
		}
		else
		{
			assert(CEffectMgr::GetInstance()->m_strSmName.GetLength() < sizeof(npc.model));
			strcpy(npc.model, CEffectMgr::GetInstance()->m_strSmName.GetBuffer());
		}

		CNPCOperator::AddNpc(&npc);
	}
}

void CAvatarShowBar::OnLoadNPC()
{
	CNpcLoadSaveDlg npcdlg;

	if(IDOK != npcdlg.DoModal())
	{
		return;
	}

	NPCDISPLAYINFO npc;
	if(CNPCOperator::GetNpcById(&npc, npcdlg.m_NpcID) == false) return;

	CMainFrame *pmainframe = static_cast<CMainFrame *>(theApp.m_pMainWnd);
	pmainframe->ClearEffectBars();
	CEffectMgr::GetInstance()->ClearEffect();
	CEffectMgr::GetInstance()->DestroyModel();

	const char* modaddr = npc.model;
	if(modaddr[0])
	{
		if(!CEffectMgr::GetInstance()->LoadShowModel(modaddr))
		{
			if(!CEffectMgr::GetInstance()->LoadSkinActor(modaddr))
			{
				MessageBox("打开NPC的模型文件失败，可能是文件类型错误");
			}
		}
	}
	else
	{
		int attrib[32];
		attrib[0] = npc.race;
		attrib[1] = npc.gender;
		memcpy(&attrib[2], npc.userdata, sizeof(npc.userdata));
		if(!CEffectMgr::GetInstance()->LoadModelFromAvatar(attrib))
		{
			MessageBox("打开NPC的Avatar资源失败");
		}
	}
}

void CAvatarShowBar::OnBtnEnable()
{
	//m_pAvatarShowInfo->EnableAvatar( !m_pAvatarShowInfo->IsEnable() );

	//SwitchPropState(m_pAvatarShowInfo->IsEnable() ? TRUE : FALSE);
}

void CAvatarShowBar::OnUpdateBtnEnable(CCmdUI* pUI)
{
	if(m_pAvatarShowInfo) pUI->SetCheck(m_pAvatarShowInfo->IsEnable() ? 1 : 0);
}

void CAvatarShowBar::SwitchPropState(BOOL bEnable)
{
	if(m_bConfigCreated)
	{
		m_pPropMod->Enable(bEnable);
		for(int i = 0; i < m_pPropMod->GetSubItemsCount(); i++)
		{
			m_pPropMod->GetSubItem(i)->Enable(FALSE);
		}
		m_pPropNormal->Enable(bEnable);
		for(int i = 0; i < m_pPropNormal->GetSubItemsCount(); i++)
		{
			m_pPropNormal->GetSubItem(i)->Enable(bEnable);
		}
	}
}

CAvatarShowInfo* CAvatarShowBar::GetEditTarget()
{
	return m_pAvatarShowInfo;
}

void CAvatarShowBar::SetEditTarget(CAvatarShowInfo* pInfo)
{
	m_pAvatarShowInfo = pInfo;
	UpdateList();
	SwitchPropState(m_pAvatarShowInfo->IsEnable() ? TRUE : FALSE);
}

void CAvatarShowBar::UpdateList()
{
	if(m_bConfigCreated == false)
		return ;

	if(m_pAvatarShowInfo->IsEnable() == false)
		return ;

	AvatarManager* pAvatarShow = AvatarManager::getSingletonPtr();

	for(int i = 0; i < m_pPropMod->GetSubItemsCount(); i++)
	{
		CBCGPProp* pProp = m_pPropMod->GetSubItem(i);
		UpdateProp(pProp);
	}

	for(int i = 0; i < m_pPropNormal->GetSubItemsCount(); i++)
	{
		CBCGPProp* pProp = m_pPropNormal->GetSubItem(i);
		UpdateProp(pProp);
	}
}

void CAvatarShowBar::CreateConfig()
{
	AvatarManager* pAvatarShow = AvatarManager::getSingletonPtr();
	if(pAvatarShow == NULL) return;

	m_pPropMod = new CBCGPProp("Avatar模型");
	m_pPropNormal = new CBCGPProp("Avatar部件");

	std::vector<CVarInfo*>&varlist = pAvatarShow->m_Config.m_VarList;
	for(size_t i=0; i<varlist.size(); i++)
	{
		CBCGPProp* pProp = CreateProp(varlist[i]);
		if(varlist[i]->m_AffectModelPath) m_pPropMod->AddSubItem(pProp);
		else m_pPropNormal->AddSubItem(pProp);
	}

	m_wndPropList.AddProperty(m_pPropMod, FALSE);
	m_wndPropList.AddProperty(m_pPropNormal, TRUE);

	m_bConfigCreated = true;
	SwitchPropState(FALSE);
}

CBCGPProp* CAvatarShowBar::CreateProp(CVarInfo* pVarInfo)
{
	CCvtHelper cvt;
	CString varName = cvt.Utf8ToGb((char*)pVarInfo->GetVarDesc().c_str());
	if(pVarInfo->GetVarType() == CVarInfo::ENUMVAR)
	{
		CNotifiedProp* pProp = new CNotifiedProp(varName, _T(""));
		pProp->SetNotifyTarget(GetSafeHwnd());
		CEnumVarInfo* pEnumVar = (CEnumVarInfo*)pVarInfo;
		for(int i = 0; i < (int)pEnumVar->m_enums.size(); i++)
		{
			CString enumDesc = cvt.Utf8ToGb(pEnumVar->m_enums[i].strDesc.c_str());
			pProp->AddOption(enumDesc.GetBuffer());
		}
		pProp->SetData(pVarInfo->GetVarId());
		return pProp;
	}
	else
	{
		CNotifiedProp* pProp = new CNotifiedProp(varName, _T(""));
		pProp->SetNotifyTarget(GetSafeHwnd());
		pProp->SetData(pVarInfo->GetVarId());
		return pProp;
	}
	return NULL;
}

void CAvatarShowBar::UpdateProp(CBCGPProp* pProp)
{
	CCvtHelper cvt;
	AvatarManager* pAvatarShow = AvatarManager::getSingletonPtr();

	int id = (int)pProp->GetData();
	int value = m_pAvatarShowInfo->m_curAttrib[id];

	CVarInfo* pVar = pAvatarShow->m_Config.FindVarById(id);
	ASSERT(pVar);

	CString valueText = cvt.Utf8ToGb(pVar->GetValueDesc(value).c_str());
	pProp->SetValue(_variant_t(valueText.GetBuffer()));
}

