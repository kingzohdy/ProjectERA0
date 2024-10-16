
#include "stdafx.h"
#include "SkillTemplateEditBar.h"
#include "ObjectEditor.h"
#include "SplitterGridView.h"
#include "EffectManager.h"
#include "ModelMotionInfo.h"
#include "NotifyProperty.h"
#include "EffectUnitEditBar.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CSkillTemplateEdit, CWnd)
CSkillTemplateEdit::CSkillTemplateEdit()
{
	m_bSpawnHitActor = false;
	m_eMonsterAI = Monster_Stand;
	m_pShowModel = NULL;
	m_pShowModelTarget = NULL;
	m_bDelState = false;
}

CSkillTemplateEdit::~CSkillTemplateEdit()
{
	for(int i=0; i<(int)vSkillTemplate.size(); i++)
		delete vSkillTemplate[i];
	vSkillTemplate.clear();
	for(int i=0; i<(int)vSkillsFastKey.size(); i++)
		delete vSkillsFastKey[i];
	vSkillsFastKey.clear();
}

BEGIN_MESSAGE_MAP(CSkillTemplateEdit, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_NOTIFY(TVN_SELCHANGED, ID_SKILLTREE, OnSelectItem)
	ON_NOTIFY(TVN_KEYDOWN, ID_SKILLTREE, OnKeyDownItem)

	ON_MESSAGE(ID_SKILL_FAST_KEY_REVERSE, OnPropChangeFastkey)

	ON_COMMAND(ID_BTN_SKILLTEMPLATE, OnSelectSkillTemplateType)

	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)

	ON_COMMAND(ID_BTN_ST_PLAY, OnPlaySkillTemplate)
	ON_COMMAND(ID_BTN_ST_STOP, OnStopPlaySkillTemplate)
	ON_COMMAND(ID_BTN_ST_SAVE, OnSaveSkillTemplate)
	ON_COMMAND(ID_BTN_ST_DELETE, OnDeleteSkill)
	ON_COMMAND(ID_BTN_ST_OPEN, OnOpenSkillTemplate)
	ON_COMMAND(ID_SAVE_SKILL, OnSaveSkill)
	ON_COMMAND(ID_SAVE_AS_SKILL, OnSaveSkillAs)

	ON_COMMAND(ID_BTN_ADD_AS_TEMP_PARAMS, OnAddAsTempParams)
	ON_COMMAND(ID_BTN_DEL_AS_TEMP_PARAMS, OnDelTempParams)
	ON_UPDATE_COMMAND_UI(ID_BTN_DEL_AS_TEMP_PARAMS, OnUpdateDelTempParams)

	ON_COMMAND(ID_BTN_ST_HITACTOR, OnHitActorOp)
	ON_UPDATE_COMMAND_UI(ID_BTN_ST_HITACTOR, OnUpdateHitActorOp)
	ON_COMMAND(ID_BTN_ST_BEHITACTOR, OnBeHitActorOp)
	ON_COMMAND(ID_ADD_MONSTER, OnAddMonsters)
	ON_COMMAND(ID_CLEAR_MONSTER, OnClearAllMonsters)

	ON_COMMAND(ID_BTN_ST_MONSTERAI, OnMonsterAI)
	ON_COMMAND(ID_MONSTER_AI_STAND, OnMonsterAIStand)
	ON_UPDATE_COMMAND_UI(ID_MONSTER_AI_STAND, OnUpdateMonsterAIStand)
	ON_COMMAND(ID_MONSTER_AI_MOVE, OnMonsterAIMove)
	ON_UPDATE_COMMAND_UI(ID_MONSTER_AI_MOVE, OnUpdateMonsterAIMove)
	ON_COMMAND(ID_MONSTER_AI_ATTACK, OnMonsterAIAttack)
	ON_UPDATE_COMMAND_UI(ID_MONSTER_AI_ATTACK, OnUpdateMonsterAIAttack)
END_MESSAGE_MAP()



// CSkillTemplateEdit message handlers
CBCGPToolbarComboBoxButton itemSkillType(ID_BTN_SKILLTEMPLATE, 0, CBS_DROPDOWNLIST, 160);
int CSkillTemplateEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.Create(this);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SKILLTEMPLATE);
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	//	m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_BTN_ST_OPEN));
	//	m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_BTN_ST_BEHITACTOR));

	//添加MONSTER的AI类型
	HMENU hMenu = ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_MONSTER_AI)), 0);
	CBCGPToolbarMenuButton mMonsterAI(ID_BTN_ST_MONSTERAI, hMenu, 0);
	mMonsterAI.SetMenuOnly(TRUE);
	mMonsterAI.SetImage(m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ST_MONSTERAI))->GetImage());
	m_wndToolBar.ReplaceButton(ID_BTN_ST_MONSTERAI, mMonsterAI);

	//添加怪物操作
	HMENU hMenu2 = ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_MONSTER_OP)), 0);
	CBCGPToolbarMenuButton mMonsterOp(ID_BTN_ST_BEHITACTOR, hMenu2, 0);
	mMonsterOp.SetMenuOnly(TRUE);
	mMonsterOp.SetImage(m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ST_BEHITACTOR))->GetImage());
	m_wndToolBar.ReplaceButton(ID_BTN_ST_BEHITACTOR, mMonsterOp);

	//保存技能
	HMENU hMenu3 = ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_SAVESKILL)), 0);
	CBCGPToolbarMenuButton mSkillSave(ID_BTN_ST_SAVE, hMenu3, 0);
	mSkillSave.SetMenuOnly(TRUE);
	mSkillSave.SetImage(m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ST_SAVE))->GetImage());
	m_wndToolBar.ReplaceButton(ID_BTN_ST_SAVE, mSkillSave);

	CCreateContext context1;
	context1.m_pCurrentDoc = NULL;
	context1.m_pCurrentFrame = NULL;
	context1.m_pLastView = NULL;
	context1.m_pNewDocTemplate = NULL;
	context1.m_pNewViewClass = RUNTIME_CLASS(CSplitterGridView);

	CCreateContext context2;
	context2.m_pCurrentDoc = NULL;
	context2.m_pCurrentFrame = NULL;
	context2.m_pLastView = NULL;
	context2.m_pNewDocTemplate = NULL;
	context2.m_pNewViewClass = RUNTIME_CLASS(CSplitterGridView);

	CRect clientRect;
	GetClientRect(&clientRect);

	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CSplitterGridView), CSize(clientRect.Width()/2, 0), &context1);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSplitterGridView), CSize(clientRect.Width()/2, 0), &context2);
	m_wndSplitter.MoveWindow(CRect(clientRect.left, clientRect.top + 24, clientRect.right, clientRect.bottom));

	m_pGridTarget = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	m_pGridSkill = (CSplitterGridView*)m_wndSplitter.GetPane(0, 1);
	m_wndSplitter.SetColumnInfo(0, 160, 10);

	m_wndPropListSkill.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGridSkill, ID_SKILLTEMPLATE_SKILLPROPLIST);
	m_wndPropListSkill.EnableHeaderCtrl(TRUE, _T("技能信息"), _T("")); //设置标题信息

	//技能名称树
	m_SkillTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS ,
		CRect(0, 0, 100, 100), m_pGridTarget, ID_SKILLTREE);

	m_pGridTarget->SetInnerCtrl(&m_SkillTree);
	m_pGridSkill->SetInnerCtrl(&m_wndPropListSkill);

	m_wndSplitter.ShowWindow(SW_SHOW);
	m_resDropSkill.Register(&m_wndPropListSkill);
	//	m_resDropTarget.Register(&m_wndPropListTarget);
	m_resDropSkill.SetNotifyTarget(GetSafeHwnd());
	m_resDropTarget.SetNotifyTarget(GetSafeHwnd());

	m_strSelectedSkillTemplate = "";
	m_SelectedSkillID = -1;

	return 0;
}

void CSkillTemplateEdit::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

Template* CSkillTemplateEdit::GetCurrentTemplate()
{
	Template* pTemplate = NULL;
	for(int i=0; i<(int)vSkillTemplate.size(); i++)
	{
		if(vSkillTemplate[i]->stTemplateName.Compare(m_strSelectedSkillTemplate) == 0)
			pTemplate = vSkillTemplate[i];
	}
	assert(pTemplate != NULL);
	return pTemplate;
}

void CSkillTemplateEdit::InitializeSkillTemplate(CString strSTName)
{/*
	Template* pTemplate = NULL;
	size_t i;
	for(i=0; i<vSkillTemplate.size(); i++)
	{
		if(vSkillTemplate[i]->stTemplateName.Compare(strSTName) == 0)
		{
			pTemplate = vSkillTemplate[i];
			break;
		}
	}

	SkillTemplateDesc *pdesc = SkillManager::getSingleton().findSkillTemplate(strSTName.GetBuffer());
	if(pdesc == NULL)
	{
		return;
	}

	if(i == vSkillTemplate.size())//此技能模版仍未加载
	{
		pTemplate = new Template;
		pTemplate->InitTemplate(pdesc);
		vSkillTemplate.push_back(pTemplate);
	}
	assert(pTemplate != NULL);
	pTemplate->ResetTemplate(pdesc); //用缺省值重置技能模版的显示值
	pTemplate->UpdateTemplateProps(CREATE_PROP, m_wndPropListSkill);
	pTemplate->SetNotifyWnd(GetSafeHwnd());
	pTemplate->UpdateTemplateProps(DATA_TO_PROP, m_wndPropListSkill);

	AddOption();
	*/
}

void CSkillTemplateEdit::LoadSkillTemplates()
{/*
	size_t k;
	SkillManager &skillmgr = SkillManager::getSingleton();
	for(k=0; k<skillmgr.getNumSkillTemplate(); k++)
	{
		SkillTemplateDesc *psrc = skillmgr.getIthSkillTemplate(k);
		Template *pdest = new Template;
		pdest->InitTemplate(psrc);
		vSkillTemplate.push_back(pdest);
	}
	*/
}

void CSkillTemplateEdit::UpdateSkillTree()
{
	m_SkillTree.DeleteAllItems();
	hRoot = m_SkillTree.InsertItem("所有技能", TVI_ROOT, TVI_LAST);

	//std::vector<int>skills;
	//SkillManager::getSingleton().getAllSkills(skills);


	/*
	GC::GCList< GC::GCString > allSkillName;
	CEnginAgent::GetInstance()->m_GCEditor->ListSkills( allSkillName );

	GC::GCList< GC::GCString >::iterator it = allSkillName.begin();

	for(; it != allSkillName.end(); it++)
	{	
	CString strSkillTemplateName = CEnginAgent::GetInstance()->m_GCEditor->SkillName2TemplateName( const_cast<TCHAR*>((*it).c_str()) );
	if(strSkillTemplateName.Compare(m_strSelectedSkillTemplate) == 0)
	m_SkillTree.InsertItem((*it).c_str() , hRoot, TVI_LAST );
	}*/

	m_SkillTree.Expand( hRoot, TVE_EXPAND );
}

void CSkillTemplateEdit::UpdateSkillTemplateName()
{/*
	itemSkillType.RemoveAllItems();

	LoadSkillTemplates();

	for(size_t i=0; i<SkillManager::getSingleton().getNumSkillTemplate(); i++)
	{
		SkillTemplateDesc *pdesc = SkillManager::getSingleton().getIthSkillTemplate(i);
		itemSkillType.AddItem(pdesc->m_Title.c_str(), i);

		m_SkillTemplateNameExtraId[i] = pdesc->m_Name.c_str();
	}

	CString strChnName = SkillTemplateNameE2C(m_strSelectedSkillTemplate);
	itemSkillType.SelectItem(strChnName.GetBuffer());

	m_wndToolBar.ReplaceButton(ID_BTN_SKILLTEMPLATE, itemSkillType);
*/
}

void CSkillTemplateEdit::AddOptionFromModel(CBCGPProp* pProp, Ogre::Entity* pModel)
{
	pProp->RemoveAllOptions();

	CModelMotionInfo* pMotionInfo = CEffectMgr::GetInstance()->GetModelMotion();
	for(int i = 0; i < (int)pMotionInfo->m_motions.size(); i++)
	{
		pProp->AddOption(pMotionInfo->m_motions[i]->name.c_str());
	}
}

//spPath的格式比如 model.decal 文件夹
void CSkillTemplateEdit::AddOptionFromPath(CBCGPProp* pProp, const char* spPath)
{
	CString strPath;
	CString strTemp;
	if(spPath == NULL)
	{
		strPath = "chunkdata\\effect\\public";
		strTemp = "effect.public.";
	}
	else
	{
		strPath = spPath;
		strTemp = spPath;
		strPath.Replace(".", "\\");
		strPath = "chunkdata\\" + strPath;
	}
	CFileFind finder;
	BOOL bSucc = finder.FindFile(strPath + "\\*.*");
	while(bSucc)
	{
		bSucc = finder.FindNextFile();
		if(finder.GetFileName() == "." || finder.GetFileName() == "..")
			continue;
		CString strName = strTemp + finder.GetFileTitle();
		pProp->AddOption(strName.GetBuffer());
	}
	finder.Close();
}

//此函数中的 motion path 字符串是与技能模版配置文件匹配的
void CSkillTemplateEdit::AddOption()
{
	Template* pTemplate = GetCurrentTemplate();
	std::vector<CNotifiedProp*> vNotifiedProp;

	pTemplate->GetPropByStr("motion", vNotifiedProp);
	for(int i=0; i<(int)vNotifiedProp.size(); i++)
		AddOptionFromModel(vNotifiedProp[i], m_pShowModel);

	vNotifiedProp.clear();
	pTemplate->GetPropByStr("path", vNotifiedProp);
	for(int i=0; i<(int)vNotifiedProp.size(); i++)
		AddOptionFromPath(vNotifiedProp[i]);
}

void CSkillTemplateEdit::OnSelectItem(NMHDR * pNotifyStruct, LRESULT * result)
{/*
	NMTREEVIEW* pNotify = (NMTREEVIEW*)pNotifyStruct;
	HTREEITEM hItem = pNotify->itemNew.hItem;

	if(hRoot == hItem)
	{
		m_SelectedSkillID = -1;
		return;
	}
	CString strText = m_SkillTree.GetItemText(hItem);

	m_SelectedSkillID = atoi(strText.GetBuffer());
	m_strSelectedSkillTemplate = SkillManager::getSingleton().getSkillTemplate(m_SelectedSkillID);

	UpdatePropList();
	UpdateTempParams(m_SelectedSkillID);
	*/
}

void CSkillTemplateEdit::OnKeyDownItem(NMHDR* pNotifyStruct, LRESULT* result)
{
	NMTVKEYDOWN* pNotify = (NMTVKEYDOWN*)pNotifyStruct;
	DWORD virkey = pNotify->wVKey;

	//0----9
	if(virkey >= 48 && virkey <= 57)
	{
		if((GetAsyncKeyState(VK_CONTROL)&0xff00) != 0)
		{
			int nNum = virkey - 48;
			if(m_SelectedSkillID > 0)
			{
				CString strErr;
				CString strNum;
				CString strTemp;

				//如果此快捷键没有被设置，就可用
				size_t i, j;
				for(i=0; i<vSkillsFastKey.size(); i++)
				{
					if(vSkillsFastKey[i]->stNum == nNum)
						break;
				}
				if(i == vSkillsFastKey.size())
				{
					//先检查此技能是否设置快捷键，且以最后一次设置为主

					for(j=0; j<vSkillsFastKey.size(); j++)
					{
						if(vSkillsFastKey[j]->skillid == m_SelectedSkillID &&
							vSkillsFastKey[j]->stSkillTemplateName.compare(m_strSelectedSkillTemplate) == 0)
							break;
					}
					if(j != vSkillsFastKey.size())
					{
						vSkillsFastKey[j]->stNum = nNum;
					}
					else
					{
						SkillsFastKey* pSFK = new SkillsFastKey;
						pSFK->stNum = nNum;
						pSFK->stSkillName = m_SelectedSkillID;
						pSFK->stSkillTemplateName = m_strSelectedSkillTemplate;
						vSkillsFastKey.push_back(pSFK);
					}
					SendMainFrameMsg(ADD_SKILL_FAST_KEY, (WPARAM)&vSkillsFastKey, 0);
				}
				else
				{
					strErr.Format("快捷键 %d 已经被技能 %s 占用", nNum, vSkillsFastKey[i]->stSkillName.c_str());
					MessageBox(strErr);
					return;
				}
			}
		}
	}

	return;
}

LRESULT CSkillTemplateEdit::OnPropChangeFastkey(WPARAM wParam, LPARAM lParam)
{
	SkillsFastKey* pAssis = (SkillsFastKey*)wParam;

	for(int i=0; i<(int)vSkillsFastKey.size(); i++)
	{
		SkillsFastKey* pMain = vSkillsFastKey[i];
		if(pMain->stSkillTemplateName.compare(pAssis->stSkillTemplateName) == 0 &&
			pMain->stSkillName.compare(pAssis->stSkillName) == 0)
			pMain->stNum = pAssis->stNum;
	}
	return (LRESULT)0;
}

void CSkillTemplateEdit::SetVarToSkill(int skillid, std::vector<VarArgument>&arglist)
{/*
	SkillManager &skillmgr = SkillManager::getSingleton();
	if(!skillmgr.hasSkill(skillid))
	{
		skillmgr.newSkill(skillid);
	}

	for(size_t i=0; i<arglist.size(); i++)
	{
		VarArgument &var = arglist[i];
		if(var.mValueType == VarArgument::VA_INT) skillmgr.setSkillAttribD(skillid, var.Getname(), var.mInt);
		else if(var.mValueType == VarArgument::VA_Number) skillmgr.setSkillAttribD(skillid, var.Getname(), var.mNumber);
		else if(var.mValueType == VarArgument::VA_STRING) skillmgr.setSkillAttribS(skillid, var.Getname(), var.mStr);
		else
		{
			assert(0);
		}
	}*/
}

void CSkillTemplateEdit::GetVarFromSkill(int skillid, std::vector<VarArgument>&arglist)
{
	for(size_t i=0; i<arglist.size(); i++)
	{
		VarArgument &var = arglist[i];

	}
}

LRESULT CSkillTemplateEdit::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	USES_CONVERSION;
	Template* pTemplate = GetCurrentTemplate();
	pTemplate->UpdateTemplateProps(PROP_TO_DATA, m_wndPropListSkill);
	//将当前数据更新到技能里面去
	if(m_SelectedSkillID > 0)
	{
		std::vector<VarArgument> vVarList;
		pTemplate->GetTemplateEachItem(vVarList);
		SetVarToSkill(pTemplate->GetSkillID(), vVarList);
	}
	return 0;
}

void CSkillTemplateEdit::OnSelectSkillTemplateType()
{
	CBCGPToolbarComboBoxButton* pBoxButton = static_cast<CBCGPToolbarComboBoxButton*>
		(m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_SKILLTEMPLATE)));

	int nSel = pBoxButton->GetCurSel();
	if(nSel < 0)
		return;
	int eType = (int)pBoxButton->GetItemData(nSel);


	m_SelectedSkillID = -1;
	m_strSelectedSkillTemplate = (m_SkillTemplateNameExtraId[eType]).c_str();

	UpdateSkillTree();

	ClearSkillProp();
	InitializeSkillTemplate(m_strSelectedSkillTemplate);
	UpdateTempParams(m_SelectedSkillID);
}

void CSkillTemplateEdit::UpdatePropList()
{
	ClearSkillProp();
	InitializeSkillTemplate(m_strSelectedSkillTemplate);

	if(m_SelectedSkillID > 0)
	{//选中了技能
		std::vector<VarArgument> vVarList;
		Template* pTemplate = GetCurrentTemplate();

		GetVarFromSkill(pTemplate->GetSkillID(), vVarList);

		pTemplate->SetTemplateEachItem(vVarList);
		pTemplate->UpdateTemplateProps(DATA_TO_PROP, m_wndPropListSkill);

		pTemplate->EnableSomeItems(false);
	}
}

void CSkillTemplateEdit::ClearSkillProp()
{
	if(m_wndPropListSkill.GetSafeHwnd())
		m_wndPropListSkill.RemoveAll();
}

void CSkillTemplateEdit::PlaySkillByFastKey(int nKey/* =-1 */)
{
	assert(nKey>=0 && nKey<=9);
	size_t i;
	for(i=0; i<vSkillsFastKey.size(); i++)
	{
		if(vSkillsFastKey[i]->stNum == nKey)
			break;
	}
	if(i == vSkillsFastKey.size())
		return;

	int skillid = m_SelectedSkillID;
	m_SelectedSkillID = vSkillsFastKey[i]->skillid;
	OnPlaySkillTemplate();
	m_SelectedSkillID = skillid;
}

void CSkillTemplateEdit::OnPlaySkillTemplate()
{
	if(m_pShowModel == NULL)
	{
		MessageBox("必须先加载一个模型，并且绑定为攻击角色 ！", "加载模型", MB_OK);
		return;
	}
	if( !m_bSpawnHitActor )
	{
		MessageBox("必须先将模型绑定到攻击角色上 ！", "绑定模型", MB_OK);
		return;
	}

	assert(0);
	/*
	if(m_pShowModel->m_Motions.size() == 0 || m_pShowModel->m_Motions.size() == 1)
	{
	CModelMotionInfo* pModelMotionInfo = CEffectMgr::GetInstance()->GetModelMotion();
	for(int i=0; i<(int)pModelMotionInfo->m_motions.size(); i++)
	{
	CEffectMgr::GetInstance()->InjectMotion(i, m_pShowModel);
	}
	}*/

	//SkillManager::getSingleton().castSpell(m_SelectedSkillID);
}

void CSkillTemplateEdit::OnStopPlaySkillTemplate()
{}

void CSkillTemplateEdit::OnSaveSkillTemplate()
{/*
	Template* pTemplate = GetCurrentTemplate();
	int skillid = pTemplate->GetSkillID();

	if(m_SelectedSkillID < 0)
	{
		if(MessageBox("将当前数据添加为新技能？", "", MB_YESNO) == IDNO)
			return;
		//检查id

		bool bIdLegality = SkillManager::getSingleton().hasSkill(skillid);
		if(bIdLegality || (skillid<=0))
		{
			AfxMessageBox("已存在的技能Id或<=0");
			return;
		}

		m_SelectedSkillID = -1;
		std::vector<VarArgument> vVarList;
		pTemplate->GetTemplateEachItem(vVarList);
		SetVarToSkill(skillid, vVarList);

		if(!SkillManager::getSingleton().save("db\\skilldata.lua"))
		{
			MessageBox("保存技能文件失败");
		}
	}
#undef CString
	else
	{
		CString strStr = CString("用新的技能数据覆盖 ") + " 原有的数据？";
		if(MessageBox(strStr, "", MB_YESNO) == IDYES)
		{
			std::vector<VarArgument> vVarList;
			pTemplate->GetTemplateEachItem(vVarList);
			int skillid = m_SelectedSkillID;
			SetVarToSkill(skillid, vVarList);
			SkillManager::getSingleton().save("db\\skilldata.lua");
		}
	}
	UpdateSkillTree();
	*/
}

void CSkillTemplateEdit::OnSaveSkill()
{
	OnSaveSkillTemplate();
}

void CSkillTemplateEdit::OnSaveSkillAs()
{
	/*
	Template* pTemplate = GetCurrentTemplate();
	std::pair<int, std::string> mIdName = pTemplate->GetSkillIdAndNamd();
	std::vector<int> vSkillIds;
	vSkillIds.push_back(mIdName.first);
	CResLoadSaveShareDlg mResDlg;
	if(IDOK == mResDlg.DoModal())
	{
	std::string stSkillPath = mResDlg.m_strResName;
	CEnginAgent::GetInstance()->m_GCEditor->SaveSomeSkills(vSkillIds, stSkillPath);
	}*/
}

void CSkillTemplateEdit::OnDeleteSkill()
{/*
	if(m_SelectedSkillID < 0)
	{
		MessageBox("请选择你要删除的技能");
		return;
	}
	SkillManager::getSingleton().deleteSkill(m_SelectedSkillID);
	OnSelectSkillTemplateType();
	m_SelectedSkillID = -1;
	*/
}

void CSkillTemplateEdit::OnOpenSkillTemplate()
{/*
 CResLoadSaveShareDlg mResDlg;
 if(IDOK == mResDlg.DoModal())
 {
 std::vector<CString> vSeledFiles = mResDlg.vSeledFile;
 for(int i=0; i<(int)vSeledFiles.size(); i++)
 {
 CEnginAgent::GetInstance()->m_GCEditor->LoadSomeSkills(vSeledFiles[i].GetBuffer());
 }
 UpdateSkillTree();
 }
 */
}

void CSkillTemplateEdit::OnHitActorOp()
{
	if(m_pShowModel == NULL)
	{
		MessageBox("你必须首先加载一个模型 ！", "加载模型", MB_OK);
		return;
	}

	m_bSpawnHitActor = !m_bSpawnHitActor;

	assert(0);
	/*
	if( m_pShowModel && m_bSpawnHitActor)
	{
	CEnginAgent::GetInstance()->m_GCEditor->ReBindPlayerShowMode( m_pShowModel , -1, CEffectMgr::GetInstance()->m_avatarShowInfo.m_curAttrib);
	}
	else if( CEnginAgent::GetInstance()->m_GCEditor && m_pShowModel && !m_bSpawnHitActor)
	{
	CEnginAgent::GetInstance()->m_GCEditor->UnBindPlayerShowModel();
	}*/
}

void CSkillTemplateEdit::OnUpdateHitActorOp(CCmdUI* pCmdUI)
{
	if(m_bSpawnHitActor)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CSkillTemplateEdit::OnBeHitActorOp()
{
	/*
	if(!m_bSpawnHitActor)
	return;

	if(m_bSpawnBeHitActor)
	CEnginAgent::GetInstance()->m_GCEditor->CreateActor(60000,60000,"edit");
	else
	CEnginAgent::GetInstance()->m_GCEditor->DestroyTestMonster();
	*/
}

#undef CString
void CSkillTemplateEdit::OnAddMonsters()
{/*
 if(!m_bSpawnHitActor)	
 {
 AfxMessageBox("你应该先绑定主角 ！");
 return;
 }
 std::vector< std::pair<int, CString> > vNpcIdsAndName;
 CEnginAgent::GetInstance()->GetAllNpcIdsAndName(vNpcIdsAndName);
 std::vector<CString> vTarget;
 for(int i=0; i<(int)vNpcIdsAndName.size(); i++)
 {
 CString strTemp;
 strTemp.Format("%-7d  ", vNpcIdsAndName[i].first);
 strTemp += vNpcIdsAndName[i].second;
 vTarget.push_back(strTemp);
 }
 CShowTargetDlg shownpcids(vTarget, NULL);
 if(IDOK == shownpcids.DoModal())
 {
 std::vector<CString> vSeleds;
 shownpcids.GetResult(vSeleds);
 std::vector<int> vSelIds;
 for(int i=0; i<(int)vSeleds.size(); i++)
 {
 for(int j=0; j<(int)vNpcIdsAndName.size(); j++)
 {
 if(vSeleds[i].Compare(vTarget[j]) == 0)
 {
 vSelIds.push_back(vNpcIdsAndName[j].first);
 break;
 }
 }
 }
 for(int i=0; i<(int)vSelIds.size(); i++)
 {
 CString strName;
 strName.Format("edit%d", i);
 CEnginAgent::GetInstance()->m_GCEditor->CreateActor(strName.GetBuffer(), vSelIds[i]);
 }
 }
 */
}

void CSkillTemplateEdit::OnClearAllMonsters()
{
	//CEnginAgent::GetInstance()->m_GCEditor->DestroyTestMonster();
}

void CSkillTemplateEdit::OnMonsterAI()
{
	//CEnginAgent::GetInstance()->m_GCEditor->SetMonsterAI(m_eMonsterAI);
}

void CSkillTemplateEdit::OnMonsterAIStand()
{
	m_eMonsterAI = Monster_Stand;
	//CEnginAgent::GetInstance()->m_GCEditor->SetMonsterAI(Monster_Stand);
}

void CSkillTemplateEdit::OnUpdateMonsterAIStand(CCmdUI* pCmdUI)
{
	if(m_eMonsterAI == Monster_Stand)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CSkillTemplateEdit::OnMonsterAIMove()
{
	m_eMonsterAI = Monster_Move;
	//CEnginAgent::GetInstance()->m_GCEditor->SetMonsterAI(Monster_Move);
}

void CSkillTemplateEdit::OnUpdateMonsterAIMove(CCmdUI* pCmdUI)
{
	if(m_eMonsterAI == Monster_Move)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CSkillTemplateEdit::OnMonsterAIAttack()
{
	m_eMonsterAI = Monster_Attack;
	//CEnginAgent::GetInstance()->m_GCEditor->SetMonsterAI(Monster_Attack);
}

void CSkillTemplateEdit::OnUpdateMonsterAIAttack(CCmdUI* pCmdUI)
{
	if(m_eMonsterAI == Monster_Attack)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

BOOL CSkillTemplateEdit::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	if(nID == ID_BTN_SKILLTEMPLATE)
	{
		CString str;
		str.Format("ID_WND_SKILLTEMPLATE found, nCode=%d\n", nCode);
		//TRACE(str.GetBuffer());
		if(nCode == 8 || nCode == 9)
		{
			//#pragma warning() 
			OnSelectSkillTemplateType();
			//return TRUE;
		}
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CSkillTemplateEdit::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

//工具栏的高度
const int nSTToolBarHeight = 24;
BOOL CSkillTemplateEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message ==	WM_LBUTTONDOWN || pMsg->message == WM_MOUSEMOVE)
	{
		CRect client;
		GetClientRect(&client);
		ClientToScreen(&client);
		client.InflateRect(0, -nSTToolBarHeight, 0, 0); //工具栏的高度为24

		CPoint pt = pMsg->pt;
		BOOL bIn = client.PtInRect(pt);
		if(bIn && pMsg->message == WM_LBUTTONDOWN)
			LButtonDownAction(pMsg);
		else if(bIn && pMsg->message == WM_MOUSEMOVE)
			MouseMoveOnItem(pMsg);
	}
	else if(pMsg->message == WM_KEYDOWN)
	{
		int nVirKey = (int)pMsg->wParam;
		if(((GetAsyncKeyState(VK_CONTROL)&0xff00)==0) && (nVirKey >= 48 && nVirKey <= 57))
		{
			int nFastKey = nVirKey - 48;
			PlaySkillByFastKey(nFastKey);
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/******************添加临时变量************************/
//text上面会显示“参数1...4之类的值”
//PropList的Head的Text(即右边文字)以4个临时变量为准，如果改变，此text必须相应的改变
void CSkillTemplateEdit::OnAddAsTempParams()
{/*
 GCVector<VarArgument>* vVarList = new GCVector<VarArgument>;
 Template* pTemplate = GetCurrentTemplate();
 pTemplate->GetTemplateEachItem(*vVarList);
 pTemplate->SaveToTempParams(m_strSelectedSkillName, vVarList);
 UpdateTempParams(m_strSelectedSkillName);
 */
}

void CSkillTemplateEdit::OnDelTempParams()
{
	m_bDelState = !m_bDelState;
}

void CSkillTemplateEdit::OnUpdateDelTempParams(CCmdUI* pCmdUI)
{
	if(m_bDelState)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

//点击参数分2中：1、选择当前参数为活动参数。2、删除当前参数项。判断标准是删除项是否按下
const int gEachParams = 60; //每个参数占用的像素数
CRect rectUpdate; //用于重新刷新的区域，以防止残留方框
bool bUpdateRect = false; //刷新项区域的标志
void CSkillTemplateEdit::LButtonDownAction(MSG* pMsg)
{/*
 //检测点击到的参数区域
 CPoint pt = pMsg->pt;

 int nNumTh = ComputeItemOn(pt); //选定第nNumTh参数
 if(nNumTh == -1)
 return;

 Template* pTemplate = GetCurrentTemplate();
 int nParams = pTemplate->GetTempParamsNum(m_strSelectedSkillName);
 if(nNumTh > nParams-1)
 return;
 if(m_bDelState)
 {
 pTemplate->DelTempParamsAtPos(m_strSelectedSkillName, nNumTh);
 UpdateTempParams(m_strSelectedSkillName);

 UpdatePropList();

 if(pTemplate->GetTempParamsNum(m_strSelectedSkillName) == 0)
 m_bDelState = false;
 return;
 }

 GCVector<VarArgument>* pSelParams = pTemplate->GetTempParamsAtPos(m_strSelectedSkillName, nNumTh);
 assert(pSelParams != NULL);
 pTemplate->SetTemplateEachItem(*pSelParams);
 pTemplate->UpdateTemplateProps(DATA_TO_PROP, m_wndPropListSkill);

 g_SkillEdit = CEnginAgent::GetInstance()->m_GCEditor->GetSkillEditObjectSlot(m_strSelectedSkillName.c_str());
 assert(!g_SkillEdit.IsNull());
 g_SkillEdit.Get()->SetVariables(*pSelParams);
 g_SkillEdit.SetNull();
 */
}

//更新当前模版下，技能名称为stBTS的临时参数
void CSkillTemplateEdit::UpdateTempParams(int skillid)
{
	Template* pTemplate = GetCurrentTemplate();
	int nNum = pTemplate->GetTempParamsNum(skillid);
	CString strShowText;
	for(int i=0; i<nNum; i++)
	{
		CString strTemp;
		strTemp.Format("%d", i);
		strTemp = "  参数 " + strTemp + "  ";
		strShowText += strTemp;
	}
	m_wndPropListSkill.EnableHeaderCtrl(TRUE, _T("技能信息"), strShowText);
}

void CSkillTemplateEdit::MouseMoveOnItem(MSG* pMsg)
{
	static int nItem = -1;

	int nNumTh = ComputeItemOn(pMsg->pt);
	if(nNumTh == -1)
		return;
	Template* pTemplate = GetCurrentTemplate();
	int nParams = pTemplate->GetTempParamsNum(m_SelectedSkillID);
	if(nNumTh > nParams-1)
		return;
	if(nItem != nNumTh)
	{
		bUpdateRect = true;
		nItem = nNumTh;
	}
	CRect clientrect;
	GetClientRect(&clientrect);
	ClientToScreen(&clientrect);
	clientrect.InflateRect(0, -nSTToolBarHeight, 0, 0);
	int nHeadHeight = m_wndPropListSkill.GetHeaderHeight();
	int nHeadWidth = m_wndPropListSkill.GetPropertyColumnWidth();
	CRect leftrect;
	m_pGridTarget->GetClientRect(&leftrect);
	CRect brushrect(10+clientrect.left+leftrect.Width()+nHeadWidth+nNumTh*gEachParams, clientrect.top, 
		10+clientrect.left+leftrect.Width()+nHeadWidth+(nNumTh+1)*gEachParams, clientrect.top+nHeadHeight);
	ScreenToClient(&brushrect);
	SpecialEffect(brushrect);
}

//ptt为屏幕坐标点
int CSkillTemplateEdit::ComputeItemOn(CPoint ptt)
{
	//检测点击到的参数区域
	CPoint pt = ptt;
	CRect clientrect;
	GetClientRect(&clientrect);
	ClientToScreen(&clientrect);
	clientrect.InflateRect(0, -nSTToolBarHeight, 0, 0);

	CRect leftrect;
	m_pGridTarget->GetClientRect(&leftrect);

	int nHeadHeight = m_wndPropListSkill.GetHeaderHeight();
	int nHeadWidth = m_wndPropListSkill.GetPropertyColumnWidth();

	int nLeft = clientrect.left + leftrect.Width() + nHeadWidth;
	int nTop = clientrect.top;
	int nRight = clientrect.right;
	int nBottom = clientrect.top + nHeadHeight;
	CRect targetrect(nLeft+10, nTop, nRight, nBottom); //屏幕坐标系，不知为什么这里会偏差10

	if(targetrect.PtInRect(pt) == 0)
		return -1;
	rectUpdate = targetrect;
	int nNumTh = (pt.x - targetrect.left)/gEachParams; //选定第nNumTh参数
	return nNumTh;
}

void CSkillTemplateEdit::SpecialEffect(CRect& rect)
{
	CClientDC dc(this);
	CBrush br;
	if(bUpdateRect)
	{
		ScreenToClient(&rectUpdate);
		InvalidateRect(&rectUpdate);
		bUpdateRect = false;
	}

	br.CreateSolidBrush(RGB(67,86,115));
	dc.FrameRect(&rect, &br);
}
/*****************************************************/

//将属于技能名称stBTS的临时变量vTempParams保存起来，在外面new，由里面统一释放
void Template::SaveToTempParams(int skillid, std::vector<VarArgument>* vTempParams)
{
	int i = 0;
	for(; i<(int)vAllGroupParams.size(); i++)
	{
		if(vAllGroupParams[i]->skillid == skillid)
		{
			vAllGroupParams[i]->vTempParamsList.push_back(vTempParams);
			return;
		}
	}
	if(i == (int)vAllGroupParams.size())
	{
		EachGroupTempParams* pEachGTP = new EachGroupTempParams;
		pEachGTP->skillid = skillid;
		pEachGTP->vTempParamsList.push_back(vTempParams);
		vAllGroupParams.push_back(pEachGTP);
	}
}

//删除属于技能名称stBTS的第nPos位的临时变量
void Template::DelTempParamsAtPos(int skillid, int nPos)
{
	std::vector<VarArgument>* pTemp = GetTempParamsAtPos(skillid, nPos, true);
	if(pTemp != NULL)
		delete pTemp;
}

//得到属于技能名称stBTS的第nPos位的临时变量
std::vector<VarArgument>* Template::GetTempParamsAtPos(int skillid, int nPos, bool bErase)
{
	for(int i=0; i<(int)vAllGroupParams.size(); i++)
	{
		if(vAllGroupParams[i]->skillid == skillid)
		{
			std::vector<VarArgument>* pTemp = vAllGroupParams[i]->vTempParamsList[nPos];
			if(bErase)
				vAllGroupParams[i]->vTempParamsList.erase(vAllGroupParams[i]->vTempParamsList.begin()+nPos);
			return pTemp;
		}
	}
	return NULL;
}

//得到属于技能名称stBTS的所有参数组数目
int Template::GetTempParamsNum(int skillid)
{
	int nNum = 0;
	for(int i=0; i<(int)vAllGroupParams.size(); i++)
	{
		if(vAllGroupParams[i]->skillid == skillid)
		{
			nNum = (int)vAllGroupParams[i]->vTempParamsList.size();
			break;
		}
	}
	return nNum;
}

/*********************************************************/
int Template::GetSkillID()
{
	CString name("skillid");
	if(mSubItem.find(name) != mSubItem.end())
	{
		return mSubItem[CString("skillid")]->stData.stInt;
	}
	else
	{
		assert(0);
		return 0;
	}
}

//用于创建每项对应的pProp指针，即mSubItem的初始化
//void Template::InitTemplate(SkillTemplateDesc *psrc)
//{
//	stTemplateName = psrc->m_Name.c_str();
//	stChineseOfTemplateName = psrc->m_Title.c_str();
//
//	for(size_t i=0; i<psrc->m_Vars.size(); i++)
//	{
//		ItemData* pItemData = new ItemData;
//		SkillVarDesc *pvardesc = psrc->m_Vars[i];
//		pItemData->InitCoreData(pvardesc);
//		mSubItem[CString(pvardesc->varname.c_str())] = pItemData;
//	}
//}

//用一组std::vector<EachItem*>值初始化模版中的数据
//void Template::ResetTemplate(SkillTemplateDesc *psrc)
//{
//	for(size_t i=0; i<psrc->m_Vars.size(); i++)
//	{
//		SkillVarDesc *pvardesc = psrc->m_Vars[i];
//		std::map<CString, ItemData*>::iterator it = mSubItem.begin();
//		for(; it != mSubItem.end(); it++)
//		{
//			if(it->second->stEachItem->varname == pvardesc->varname)
//			{
//				it->second->InitCoreData(pvardesc);
//				break;
//			}
//		}
//	}
//}

//更新技能模版中的pProp显示值
void Template::UpdateTemplateProps(DATA_UPDATA_TYPE eType, CBCGPPropList& PropList)
{
	////用于临时保存已经创建的组的名称，同时根据它来排序
	//std::vector<CString> vTempCreatedGroup;
	//std::map<CString, ItemData*>::iterator it = mSubItem.begin();	
	//for(; it != mSubItem.end(); it++)
	//{
	//	switch(it->second->stData.nType)
	//	{
	//	case CoreData::eInt:
	//		CEffectEditBar::PropUpdate_int((CBCGPProp**)(&(it->second->pProp)), &(it->second->stData.stInt), eType, (it->second->stEachItem->vartitle.c_str()));
	//		break;
	//	case CoreData::eFloat:
	//		CEffectEditBar::PropUpdate_float((CBCGPProp**)(&(it->second->pProp)), &(it->second->stData.stFloat), eType, (it->second->stEachItem->vartitle.c_str()));
	//		break;
	//	case CoreData::eChar:
	//		CEffectEditBar::PropUpdate_charptr((CBCGPProp**)(&(it->second->pProp)), it->second->stData.stChar, eType, (it->second->stEachItem->vartitle.c_str()));
	//		break;
	//	default: AfxMessageBox("void UpdateTemplateProps(DATA_UPDATA_TYPE eType) Wrong !"); break;
	//	}

	//	if(eType == CREATE_PROP)
	//	{
	//		int i = 0;
	//		CString groupname(it->second->stEachItem->groupname.c_str());
	//		for(; i<(int)vTempCreatedGroup.size(); i++)
	//		{
	//			if(vTempCreatedGroup[i].Compare(groupname) == 0)
	//				break;
	//		}
	//		if(i == vTempCreatedGroup.size())
	//		{
	//			mTypeProp[groupname] = new CBCGPProp(groupname);
	//			vTempCreatedGroup.push_back(groupname);
	//		}
	//		std::map<CString, CBCGPProp*>::iterator it1 = mTypeProp.find(groupname);
	//		assert(it1 != mTypeProp.end());
	//		it1->second->AddSubItem(it->second->pProp);
	//	}
	//}
	//if(eType == CREATE_PROP)
	//{
	//	std::map<CString, CBCGPProp*>::iterator it2;
	//	for(int i=(int)vTempCreatedGroup.size()-1; i>=0; i--)
	//	{
	//		it2 = mTypeProp.find(vTempCreatedGroup[i]);
	//		PropList.AddProperty(it2->second);
	//	}
	//}
	//vTempCreatedGroup.clear();
}

//使技能名称和id对应的prop是否可以编辑
void Template::EnableSomeItems(bool bEnable)
{
	CString strEnable1 = "mSkillId";
	CString strEnable2 = "mSkillName";
	std::map<CString, ItemData*>::iterator it = mSubItem.find(strEnable1);
	if(it != mSubItem.end())
		it->second->pProp->Enable(bEnable);
	it = mSubItem.find(strEnable2);
	if(it != mSubItem.end())
		it->second->pProp->Enable(bEnable);
}

//给每项的prop设置一个通告窗口
void Template::SetNotifyWnd(HWND hWnd)
{
	std::map<CString, ItemData*>::iterator it = mSubItem.begin();
	for(; it != mSubItem.end(); it++)
	{
		if(it->second->pProp == NULL)
		{
			AfxMessageBox("pProp还没有创建");
			return;
		}
		CNotifiedProp* pPropT = it->second->pProp;
		pPropT->SetNotifyTarget(hWnd);
	}
}

//用std::vector<VarArgument> vVarList初始化技能此技能模版的每一项
void Template::SetTemplateEachItem(std::vector<VarArgument>& vVarList)
{
	std::map<CString, ItemData*>::iterator it = mSubItem.begin();
	for(; it != mSubItem.end(); it++)
	{
		for(int j=0; j<(int)vVarList.size(); j++)
		{
			if(vVarList[j].Is(TEXT(it->first)))
			{
				it->second->SetCoreData(vVarList[j]);
				break;
			}
		}
	}
}

//用技能模版中的每一项数据填充std::vector<VarArgument>& vVarList
void Template::GetTemplateEachItem(std::vector<VarArgument>& vVarList)
{
	std::map<CString, ItemData*>::iterator it = mSubItem.begin();
	for(; it != mSubItem.end(); it++)
	{
		it->second->GetCoreData(vVarList);
	}
}

//根据strDefault缺省值获取对应的Prop的指针
//用来在此Prop上面增加默认的Option选项
void Template::GetPropByStr(CString strDefault, std::vector<CNotifiedProp*>& vNotifiedProp)
{
	//std::map<CString, ItemData*>::iterator it = mSubItem.begin();
	//for(; it != mSubItem.end(); it++)
	//{
	//	CString defvalue(it->second->stEachItem->defvalue.c_str());
	//	if(defvalue.Compare(strDefault) == 0)
	//		vNotifiedProp.push_back(it->second->pProp);
	//}
}

Template::Template()
{
	stTemplateName = "";
}

Template::Template(CString strSTName)
{
	stTemplateName = strSTName;
}

Template::~Template()
{
	std::map<CString, ItemData*>::const_iterator it = mSubItem.begin();
	for(; it != mSubItem.end(); it++)
	{
		delete it->second;
	}
	mSubItem.clear();
	for(int i=0; i<(int)vAllGroupParams.size(); i++)
	{
		delete vAllGroupParams[i];
	}
	vAllGroupParams.clear();
}
