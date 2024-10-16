
#include "stdafx.h"
#include "SkillEditBar.h"
#include "SplitterGridView.h"
#include "NotifyProperty.h"
#include "EffectManager.h"
#include "SkillPreview.h"
#include "SelectSkillDlg.h"
#include "ModelMotionInfo.h"
#include "EffectUnitEditBar.h"

#include "OgreEntity.h"
#include "OgreEntityMotion.h"

IMPLEMENT_DYNAMIC(CSkillBar, CWnd)
CSkillBar::CSkillBar()
{

	m_pPropID = NULL;
	m_pPropSkillName = NULL;          
	m_pPropPrepare = NULL;
	m_pPropLaunch = NULL;
	m_pPropBehit = NULL;
	m_pPropFly = NULL;
	m_pPropPrepareTime = NULL;
	m_pPropSrcAnchor = NULL;
	m_pPropDstAnchor = NULL;
	m_pPropSpeed = NULL;  
	m_pPropHitTime = NULL;

}

CSkillBar::~CSkillBar()
{
}


BEGIN_MESSAGE_MAP(CSkillBar, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(ID_BTN_SKL_SKILLTYPE, NM_DBLCLK, TestNotify)
	ON_COMMAND(ID_BTN_SKL_SKILLTYPE, OnSkillTypeCmd)
	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickPropBtn)

	ON_COMMAND(ID_BTN_SKL_PLAY, OnPlay)
	ON_COMMAND(ID_BTN_SKL_SHOWCOORD, OnShowCoord)
	ON_UPDATE_COMMAND_UI(ID_BTN_SKL_SHOWCOORD, OnUpdateShowCoord)

	ON_COMMAND(ID_BTN_SKL_SELFTARGET, OnSelfTarget)
	ON_UPDATE_COMMAND_UI(ID_BTN_SKL_SELFTARGET, OnUpdateSelfTarget)

	ON_COMMAND(ID_BTN_SKL_ENABLETARGET, OnEnableTarget)
	ON_UPDATE_COMMAND_UI(ID_BTN_SKL_ENABLETARGET, OnUpdateEnableTarget)
	ON_COMMAND(ID_BTN_SKL_STOP, OnStop)
	ON_COMMAND(ID_BTN_SKL_SAVE, OnSave)
	ON_COMMAND(ID_BTN_SKL_LOADSKL, OnLoadSkill)



END_MESSAGE_MAP()



// CSkillBar message handlers


int CSkillBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.Create(this);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SKILL);
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

	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context1);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 1);
	m_wndSplitter.SetColumnInfo(0, 300, 10);


	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid1, ID_SKLPROP);
	m_wndPropList.EnableHeaderCtrl(TRUE, _T("技能信息"), _T(""));

	m_wndPropListTarget.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_SKLTARGETPROP);
	m_wndPropListTarget.EnableHeaderCtrl(TRUE, _T("技能目标"), _T(""));
	m_pGrid1->SetInnerCtrl(&m_wndPropList);
	m_pGrid2->SetInnerCtrl(&m_wndPropListTarget);

	m_wndSplitter.ShowWindow(SW_SHOW);
	m_dropTargetTarget.Register(&m_wndPropListTarget);
	m_dropTargetSkill.Register(&m_wndPropList);

	m_dropTargetTarget.SetNotifyTarget(GetSafeHwnd());
	m_dropTargetSkill.SetNotifyTarget(GetSafeHwnd());

	CBCGPToolbarComboBoxButton btnSkillType(ID_BTN_SKL_SKILLTYPE, 0, CBS_DROPDOWNLIST, 130);
	btnSkillType.AddItem("任意技能", SDY_ANY_SKILL);
	btnSkillType.AddItem("一般攻击", SDY_NORMAL_ATTACK);
	btnSkillType.AddItem("吟唱攻击", SDY_SPELL_ATTACK);
	btnSkillType.AddItem("一般攻击（有飞行）", SDY_NORMAL_ATTACK_WITH_FLY);
	btnSkillType.AddItem("吟唱攻击（有飞行）", SDY_SPELL_ATTACK_WITH_FLY);
	btnSkillType.AddItem("一般自身技能", SDY_NORMAL_SELF_STATE);
	btnSkillType.AddItem("吟唱自身技能", SDY_SPELL_SELF_STATE);
	btnSkillType.SelectItem(SDY_ANY_SKILL);
	m_wndToolBar.ReplaceButton(ID_BTN_SKL_SKILLTYPE, btnSkillType);

	m_displayType = SDY_ANY_SKILL;

	return 0;
}

void CSkillBar::TestNotify(NMHDR*, LRESULT*)
{
	MessageBox("Show");
}

void CSkillBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}


LRESULT CSkillBar::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	if(pdn->hWnd == m_wndPropListTarget.GetSafeHwnd())
	{
		if(m_pPropTargetName->GetRect().PtInRect(pdn->dropPos))
		{
			m_pPropTargetName->SetValue(pdn->strChunk);
			CEffectMgr::GetInstance()->GetSkillPreview()->m_strTargetName = pdn->strChunk;
			//CEffectMgr::GetInstance()->GetSkillPreview()->
		}
	}
	else if(pdn->hWnd == m_wndPropList.GetSafeHwnd())
	{
		if(m_pPropFly->GetRect().PtInRect(pdn->dropPos))
		{
			m_pPropFly->SetValue(pdn->strChunk);
			strcpy(CEffectMgr::GetInstance()->GetSkillPreview()->GetSkillInfo()->fly_effect, pdn->strChunk);
		}
	}
	return 0;
}

LRESULT CSkillBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	USES_CONVERSION;

	//TRACE("New Value: %s", OLE2T((_bstr_t)(pnm->pProp)->GetValue()));
	PropUpdate(PROP_TO_DATA);
	CEffectMgr::GetInstance()->GetSkillPreview()->UpdateTarget();
	return 0;
}

LRESULT CSkillBar::OnClickPropBtn(WPARAM w, LPARAM l)
{
	PROPNM* pnm = (PROPNM*)l;
	//CCvtHelper cvt;

	return 0;
}

void CSkillBar::OnSkillTypeCmd()
{
	CBCGPToolbarComboBoxButton* pCmb = static_cast<CBCGPToolbarComboBoxButton*> (m_wndToolBar.GetButton(
		m_wndToolBar.CommandToIndex(ID_BTN_SKL_SKILLTYPE)));

	int sel = pCmb->GetCurSel();
	if(sel < 0)
		return;

	CString str = pCmb->GetItem(sel);
	SKILL_DISPLAY_TYPE type = (SKILL_DISPLAY_TYPE) pCmb->GetItemData(sel);
	m_displayType = type;

	UpdatePropList();
	//MessageBox(str.GetBuffer());
	//TRACE(str.GetBuffer());

}

void CSkillBar::OnPlay()
{
	CEffectMgr::GetInstance()->GetSkillPreview()->PlaySkill();
}

void CSkillBar::OnShowCoord()
{
	CSkillPreview* pSkill = CEffectMgr::GetInstance()->GetSkillPreview();
	pSkill->ShowCoord(!pSkill->IsCoordShowing());
}

void CSkillBar::OnUpdateShowCoord(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;

	CSkillPreview* pSkill = CEffectMgr::GetInstance()->GetSkillPreview();

	pCmdUI->SetCheck(pSkill->IsCoordShowing() == true ? 1 : 0);
}

void CSkillBar::OnSelfTarget()
{
	CSkillPreview* pSkill = CEffectMgr::GetInstance()->GetSkillPreview();
	SKILL_TARGET_TYPE type = pSkill->GetTargetType();

	PropUpdate(PROP_TO_DATA);

	if(type == STT_RESOURCE)
		pSkill->SetTargetType(STT_SELF);
	else if(type == STT_SELF)
		pSkill->SetTargetType(STT_RESOURCE);

	UpdatePropList();
}

void CSkillBar::OnUpdateSelfTarget(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;

	CSkillPreview* pSkill = CEffectMgr::GetInstance()->GetSkillPreview();
	if(pSkill->IsEnableTarget())
	{
		SKILL_TARGET_TYPE type = pSkill->GetTargetType();
		if(type == STT_RESOURCE)
			pCmdUI->SetCheck(0);
		else if(type == STT_SELF)
			pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CSkillBar::OnEnableTarget()
{
	CSkillPreview* pSkill = CEffectMgr::GetInstance()->GetSkillPreview();
	pSkill->EnableTarget(!pSkill->IsEnableTarget());
}

void CSkillBar::OnUpdateEnableTarget(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;
	CSkillPreview* pSkill = CEffectMgr::GetInstance()->GetSkillPreview();
	pCmdUI->SetCheck(pSkill->IsEnableTarget() ? 1 : 0);
}

void CSkillBar::OnStop()
{
	CEffectMgr::GetInstance()->GetSkillPreview()->StopSkill();
}

void CSkillBar::OnSave()
{
	m_wndPropList.EndEditItem(TRUE);
	PropUpdate(PROP_TO_DATA);
	CSkillPreview* pSkillPreview = CEffectMgr::GetInstance()->GetSkillPreview();
	//pSkillPreview->SaveToDb();

	tagSkillInfo oldskl;
	if(true == pSkillPreview->FindSkill(pSkillPreview->m_skillInfo.id, &oldskl))
	{
		CString msg;
		msg.Format("已经存在ID为 %d 的技能 “%s”，覆盖它吗？", oldskl.id, oldskl.name);
		if(IDNO == MessageBox(msg.GetBuffer(), NULL, MB_YESNO))
		{
			MessageBox("当前技能没有被保存");
			return;
		}

		pSkillPreview->UpdateSkillDB();
	}
	else
	{
		pSkillPreview->AddToSkillDB();
	}
	MessageBox("技能已经保存");

}

void CSkillBar::OnLoadSkill()
{
	CSelectSkillDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		if(true == CEffectMgr::GetInstance()->GetSkillPreview()->LoadSkill(dlg.m_iSelId))
			PropUpdate(DATA_TO_PROP);
		else
			MessageBox("打开数据失败");
	}
}



void CSkillBar::AddMotionsListFromDoc(CBCGPProp* pProp)
{
	pProp->RemoveAllOptions();

	CModelMotionInfo* pMotionInfo = CEffectMgr::GetInstance()->GetModelMotion();
	for(int i = 0; i < (int)pMotionInfo->m_motions.size(); i++)
	{
		pProp->AddOption(pMotionInfo->m_motions[i]->name.c_str());
	}
}

void CSkillBar::AddMotionsListFromModel(CBCGPProp* pProp, Ogre::Entity* pModel)
{
	if(!pModel)
		return;

	pProp->RemoveAllOptions();

	for(size_t i=0; i<pModel->getNumMotion(); i++)
	{
		Ogre::ModelMotion* pMotion = pModel->getIthMotion(i);
		pProp->AddOption(pMotion->m_strName);
	}
}
//
//CNotifiedProp* m_pPropID;      
//CNotifiedProp* m_pPropSkillName;          
//CNotifiedProp* m_pPropPrepare;
//CNotifiedProp* m_pPropLaunch;
//CNotifiedProp* m_pPropBehit;
//CNotifiedProp* m_pPropFly;
//CNotifiedProp* m_pPropPrepareTime;
//CNotifiedProp* m_pPropSrcAnchor;
//CNotifiedProp* m_pPropDstAnchor;
//CNotifiedProp* m_pPropSpeed;  

void CSkillBar::ClearSkillProp()
{
	if(m_wndPropList.GetSafeHwnd())
		m_wndPropList.RemoveAll();
	//SAFE_DELETE(m_pPropID);
	//SAFE_DELETE(m_pPropSkillName);
	//SAFE_DELETE(m_pPropPrepare);
	//SAFE_DELETE(m_pPropLaunch);
	//SAFE_DELETE(m_pPropBehit);
	//SAFE_DELETE(m_pPropFly);
	//SAFE_DELETE(m_pPropPrepareTime);
	//SAFE_DELETE(m_pPropSrcAnchor);
	//SAFE_DELETE(m_pPropDstAnchor);
	//SAFE_DELETE(m_pPropSpeed);
	m_pPropID = NULL;
	m_pPropSkillName = NULL;
	m_pPropPrepare = NULL;
	m_pPropLaunch = NULL;
	m_pPropBehit = NULL;
	m_pPropFly = NULL;
	m_pPropPrepareTime = NULL;
	m_pPropSrcAnchor = NULL;
	m_pPropDstAnchor = NULL;
	m_pPropSpeed = NULL;
	m_pPropHitTime = NULL;

}

void CSkillBar::UpdatePropList()
{
	ClearSkillProp();
	m_wndPropListTarget.RemoveAll();

	PropUpdate(CREATE_PROP);

	if(m_pPropPrepare) AddMotionsListFromDoc(m_pPropPrepare);
	if(m_pPropLaunch) AddMotionsListFromDoc(m_pPropLaunch);
	if(m_pPropBehit) AddMotionsListFromModel(m_pPropBehit, CEffectMgr::GetInstance()->GetSkillPreview()->m_pTargetModel);

	if(m_pPropID)			m_pPropID->			SetNotifyTarget(GetSafeHwnd());
	if(m_pPropSkillName)	m_pPropSkillName->	SetNotifyTarget(GetSafeHwnd());
	if(m_pPropPrepare)		m_pPropPrepare->	SetNotifyTarget(GetSafeHwnd());
	if(m_pPropLaunch)		m_pPropLaunch->		SetNotifyTarget(GetSafeHwnd());
	if(m_pPropBehit)		m_pPropBehit->		SetNotifyTarget(GetSafeHwnd());
	if(m_pPropFly)			m_pPropFly->		SetNotifyTarget(GetSafeHwnd());
	if(m_pPropPrepareTime)	m_pPropPrepareTime->SetNotifyTarget(GetSafeHwnd());
	if(m_pPropSrcAnchor)	m_pPropSrcAnchor->	SetNotifyTarget(GetSafeHwnd());
	if(m_pPropDstAnchor)	m_pPropDstAnchor->	SetNotifyTarget(GetSafeHwnd());
	if(m_pPropSpeed)		m_pPropSpeed->		SetNotifyTarget(GetSafeHwnd());
	if(m_pPropHitTime)		m_pPropHitTime->	SetNotifyTarget(GetSafeHwnd());

	CBCGPProp* pPropInfo = new CBCGPProp("技能信息");
	CBCGPProp* pPropMotions = new CBCGPProp("动作");
	CBCGPProp* pPropFly = new CBCGPProp("飞行");

	if(m_pPropID)			pPropInfo->AddSubItem(m_pPropID);
	if(m_pPropSkillName)	pPropInfo->AddSubItem(m_pPropSkillName);
	if(m_pPropPrepare)		pPropMotions->AddSubItem(m_pPropPrepare);
	if(m_pPropLaunch)		pPropMotions->AddSubItem(m_pPropLaunch);
	if(m_pPropBehit)		pPropMotions->AddSubItem(m_pPropBehit);
	if(m_pPropFly)			pPropFly->AddSubItem(m_pPropFly);
	if(m_pPropPrepareTime)	pPropMotions->AddSubItem(m_pPropPrepareTime);
	if(m_pPropSrcAnchor)	pPropFly->AddSubItem(m_pPropSrcAnchor);
	if(m_pPropDstAnchor)	pPropFly->AddSubItem(m_pPropDstAnchor);
	if(m_pPropSpeed)		pPropFly->AddSubItem(m_pPropSpeed);
	if(m_pPropHitTime)		pPropMotions->AddSubItem(m_pPropHitTime);
	m_wndPropList.AddProperty(pPropInfo);
	m_wndPropList.AddProperty(pPropMotions);
	m_wndPropList.AddProperty(pPropFly);
	//m_pPropTargetType = new CNotifiedProp("目标类型", _T(""));
	//m_pPropTargetType->AddOption(_T("攻击者副本"));
	//m_pPropTargetType->AddOption(_T("其他资源"));

	m_pPropTargetName->SetNotifyTarget(GetSafeHwnd());
	m_pPropTargetPosX->SetNotifyTarget(GetSafeHwnd());
	m_pPropTargetPosY->SetNotifyTarget(GetSafeHwnd());
	m_pPropTargetPosZ->SetNotifyTarget(GetSafeHwnd());
	m_pPropTargetRot->SetNotifyTarget(GetSafeHwnd());

	CBCGPProp* pPos = new CBCGPProp("目标位置");
	pPos->AddSubItem(m_pPropTargetPosX);
	pPos->AddSubItem(m_pPropTargetPosY);
	pPos->AddSubItem(m_pPropTargetPosZ);

	m_wndPropListTarget.AddProperty(m_pPropTargetName);
	m_wndPropListTarget.AddProperty(m_pPropTargetRot);
	m_wndPropListTarget.AddProperty(pPos);

	//	m_wndPropListTarget.SetAlphabeticMode(TRUE);

	UpdatePropListData();


}

void CSkillBar::UpdatePropListData()
{
	PropUpdate(DATA_TO_PROP);
}

void CSkillBar::PropUpdate(DATA_UPDATA_TYPE type)
{
	CSkillPreview* pSkillPreview = CEffectMgr::GetInstance()->GetSkillPreview();
	tagSkillInfo* pSkill = pSkillPreview->GetSkillInfo();

	if(type == PROP_TO_DATA)
	{
		ZeroMemory(pSkill, sizeof(tagSkillInfo));
	}

	bool bHasPre = pSkillPreview->HasPrepare(m_displayType);
	bool bHasLch = pSkillPreview->HasLaunch(m_displayType);
	bool bHasBht = pSkillPreview->HasBehit(m_displayType);
	bool bHasFly = pSkillPreview->HasFly(m_displayType);

	CEffectEditBar::PropUpdate_int	  ((CBCGPProp**)&m_pPropID,			(int*)&pSkill->id,				type, _T("技能ID"));	
	CEffectEditBar::PropUpdate_charptr((CBCGPProp**)&m_pPropSkillName,	pSkill->name,				type, _T("技能名称"));

	if(bHasPre) CEffectEditBar::PropUpdate_charptr((CBCGPProp**)&m_pPropPrepare,	pSkill->prepare_action,		type, _T("准备动作"));
	if(bHasLch) CEffectEditBar::PropUpdate_charptr((CBCGPProp**)&m_pPropLaunch,		pSkill->cast_action,		type, _T("击发动作"));
	if(bHasBht) CEffectEditBar::PropUpdate_charptr((CBCGPProp**)&m_pPropBehit,		pSkill->behit_action,		type, _T("被击动作"));
	if(bHasFly) CEffectEditBar::PropUpdate_charptr((CBCGPProp**)&m_pPropFly,		pSkill->fly_effect,			type, _T("飞行特效"));
	if(bHasPre) CEffectEditBar::PropUpdate_int    ((CBCGPProp**)&m_pPropPrepareTime,&pSkill->prepare_time,		type, _T("准备时间"));

	if(bHasFly) CEffectEditBar::PropUpdate_int    ((CBCGPProp**)&m_pPropSrcAnchor,	&pSkill->fly_start_anchor,	type, _T("飞行起始绑点"));
	if(bHasFly) CEffectEditBar::PropUpdate_int    ((CBCGPProp**)&m_pPropDstAnchor,	&pSkill->fly_end_anchor,	type, _T("飞行终止绑点"));
	if(bHasFly) CEffectEditBar::PropUpdate_int    ((CBCGPProp**)&m_pPropSpeed,		&pSkill->fly_speed,			type, _T("飞行速度(毫米/秒)"));

	CEffectEditBar::PropUpdate_int    ((CBCGPProp**)&m_pPropHitTime,		&pSkill->hit_time,		type, _T("攻击时间"));

	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTargetName, &pSkillPreview->m_strTargetName,type, _T("目标模型"));
	CEffectEditBar::PropUpdate_float  ((CBCGPProp**)&m_pPropTargetRot,	&pSkillPreview->m_fDir,			type, _T("方向"));
	CEffectEditBar::PropUpdate_float  ((CBCGPProp**)&m_pPropTargetPosX, &pSkillPreview->m_vTargetPos.x,	type, _T("X"));	
	CEffectEditBar::PropUpdate_float  ((CBCGPProp**)&m_pPropTargetPosY, &pSkillPreview->m_vTargetPos.y,	type, _T("Y"));
	CEffectEditBar::PropUpdate_float  ((CBCGPProp**)&m_pPropTargetPosZ, &pSkillPreview->m_vTargetPos.z,	type, _T("Z"));
}

BOOL CSkillBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	if(nID == ID_BTN_SKL_SKILLTYPE)
	{
		CString str;
		str.Format("ID_BTN_SKL_SKILLTYPE found, nCode=%d\n", nCode);
		//TRACE(str.GetBuffer());
		if(nCode == 8 || nCode == 9)
		{
			//"OnSkillType 用了一种怪异的方式调用，可能会出问题 ,也许本窗体是CBCG的窗体就没有这个问题
			OnSkillTypeCmd();
			return TRUE;
		}
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
