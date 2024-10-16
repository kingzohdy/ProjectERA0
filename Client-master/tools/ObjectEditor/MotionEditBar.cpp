
#include "stdafx.h"
#include "MotionEditBar.h"
#include "SplitterGridView.h"
#include "EffectManager.h"
#include "EffectLayerInfo.h"
#include "LayerInfoDlg.h"
#include "ControlsHelper.h"
#include "MergeMotionDlg.h"
#include "MotionPropertyDlg.h"
#include "SkeletonInfo.h"
#include "ObjectEditor.h"
#include "MainFrm.h"

#include "OgreEntityMotion.h"
#include "OgreEntity.h"
#include "OgreResourceManager.h"

float m_fUnitValue;
int m_iExactInStep;
int m_iStepPixes;
extern OpData gLayersOpObj;


TIME_AIX_LEVEL g_TimeAixLevel[] = 
{
	{ 1.0f/30.0f, 30, 120, 10, false},
	{ 1.0f/30.0f, 30, 150, 10, false },
	{ 1.0f/30.0f, 30, 180, 10, false },
	{ 1.0f/30.0f, 30, 210, 10, true },
	{ 1.0f/30.0f, 30, 240, 10, true },
	{ 1.0f/30.0f, 30, 270, 10, true },
	{ 1.0f/30.0f, 30, 300, 10, true }
};

int g_iTimeAixLevelCount = 7;



// CMontionEditPane

IMPLEMENT_DYNAMIC(CMontionEditPane, CWnd)
CMontionEditPane::CMontionEditPane()
{
	m_iMontionSetLod = -1;
	m_eShowLodState = HIGH_STATE;
	m_CreateLayerByDlg = true;
}

CMontionEditPane::~CMontionEditPane()
{
}


BEGIN_MESSAGE_MAP(CMontionEditPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_COMMAND(ID_BTN_MO_NEWMOTION, OnNewMotion)
	ON_COMMAND(ID_BTN_MO_DELMOTION, OnDelMotion)
	ON_COMMAND(ID_BTN_MO_ADDEFFECT, OnAddEffect)
	ON_COMMAND(ID_BTN_MO_DELEFFECT, OnDelEffect)
	ON_COMMAND(ID_BTN_MO_PLAYMOTION, OnPlayMotion)
	ON_COMMAND(ID_BTN_MO_STOPMOTION, OnStopMotion)
	ON_COMMAND(ID_BTN_MON_SAVEMON, OnSaveMon)
	ON_COMMAND(ID_BTN_MON_LOADMON, OnImportMon)
	ON_COMMAND(ID_BTN_MO_MERGE, OnMergeKeyFrame)
	ON_COMMAND(ID_BTN_MO_APART, OnApartKeyFrame)
	ON_COMMAND(ID_BTN_MO_NEWMOTIONFROMBONE, OnNewMotionFromBone)
	ON_COMMAND(ID_BTN_MO_COPY, OnCopyLayer)
	ON_COMMAND(ID_BTN_MO_RULER_IN, OnRulerIn)
	ON_COMMAND(ID_BTN_MO_RULER_OUT, OnRulerOut)
	ON_COMMAND(ID_BTN_MON_SAVETOLIB, OnSaveMonToLib)
	ON_COMMAND(ID_BTN_MON_SAVEOBJASMON, OnSaveObjAsMotion)
	ON_COMMAND(ID_BTN_MON_SAVESHOWMOD, OnSaveMonAsShowModel)
	ON_COMMAND(ID_MENU_NEWPOST_CAMERASHAKE, OnNewCameraShake)
	ON_COMMAND(ID_MENU_NEWPOST_FROZEN, OnNewFrozen)
	ON_COMMAND(ID_MENU_NEWPOST_TRANSPARENT, OnNewTransparent)

	ON_COMMAND(ID_BTN_MO_MERGEMOTION, OnMergeMotion)
	ON_COMMAND(ID_BTN_MON_SHOWPATH, OnShowPath)
	ON_UPDATE_COMMAND_UI(ID_BTN_MON_SHOWPATH, OnUpdateShowPath)
	ON_COMMAND(ID_BTN_MO_LOOPPLAY, OnLoopPlay)
	ON_UPDATE_COMMAND_UI(ID_BTN_MO_LOOPPLAY, OnUpdateLoopPlay)

	ON_COMMAND_RANGE(ID_BTN_MO_NEW_PATTICLE, ID_BTN_MO_NEW_VIRTUAL, OnNewObjLayer)
	ON_COMMAND(ID_BTN_MO_NEW_EVENT, OnNewEventObjLayer)

	ON_NOTIFY(TAN_SELECT_CHANGE, ID_MONTIONTB, OnTimeLayerSelected)
	ON_NOTIFY(TAN_SIZE_CHANGE, ID_MONTIONTB, OnTimeLayerSizeChange)

	ON_NOTIFY(TAN_POS_CHANGE, ID_MONTIONTB, OnTimeLayerPosChange)
	ON_NOTIFY(TAN_LAYER_ACTIVE, ID_MONTIONTB, OnTimeLayerActive)
	ON_NOTIFY(TAN_KEY_DOWN, ID_MONTIONTB, OnTimeLayerKeyDown)
	ON_NOTIFY(TAN_CLICK_OPTION, ID_MONTIONTB, OnTimeLayerClickOption)


	ON_NOTIFY(TAN_ENDLINE_CHANGE, ID_MONTIONTB, OnTimeAixesEndChange)
	ON_NOTIFY(TAN_CTRL_DRAG, ID_MONTIONTB, OnTimeAixesCtrlDrag)

	ON_NOTIFY(TAN_RED_BOX_CHANGE, ID_MONTIONTB, OnTimeAixesRedBoxChange)

	ON_NOTIFY(LVN_ITEMCHANGED, ID_MOTIONTREE, OnMotionListItemChange)
	ON_NOTIFY(LVN_ITEMACTIVATE, ID_MOTIONTREE, OnMotionListItemActive)

	ON_NOTIFY(TAN_LAYER_POS, ID_MONTIONTB, OnTimeAixLayerPos)	
	ON_NOTIFY(TAN_LAYER_RELATE, ID_MONTIONTB, OnTimeAixLayerRelate)	
	ON_MESSAGE(EEM_DROPRES, OnDropRes)

	ON_NOTIFY(TAN_LOD_CHANGE, ID_MONTIONTB, OnObjLayerLodChange)

	ON_NOTIFY(TAN_LOD_SHOWSTATE, ID_MONTIONTB, OnObjShowByLod)
	ON_WM_SETFOCUS()

	ON_NOTIFY(NM_RCLICK, ID_MOTIONTREE, OnSetMotionLodLevel)

	ON_NOTIFY(TAN_LAYERS_PASTE_OP, ID_MONTIONTB, OnPasteLayers)
	ON_NOTIFY(TAN_LAYERS_COPY_OP, ID_MONTIONTB, OnCopyLayers)
	ON_NOTIFY(TAN_LAYERS_CUT_OP, ID_MONTIONTB, OnCutLayers)

	ON_COMMAND(ID_SET_HIGHLOD, OnSetHighLod)
	ON_COMMAND(ID_SET_MIDDLELOD, OnSetMiddleLod)
	ON_COMMAND(ID_SET_LOWLOD, OnSetLowLod)
	ON_COMMAND(ID_EXIT_LOD, OnExitLodPopMenu)
	ON_COMMAND(ID_NEW_EFFECT, &CMontionEditPane::OnNewEffect)
END_MESSAGE_MAP()

// CMontionEditPane message handlers

int CMontionEditPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.Create(this);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MOTION);
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

	//分割成2个窗口
	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context1);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 1);

	// 在m_pGrid1创建列表框
	m_wndList.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT |LVS_SHOWSELALWAYS, CRect(0, 0, 100, 100), m_pGrid1, ID_MOTIONTREE);
	m_pGrid1->SetInnerCtrl(&m_wndList);
	m_wndSplitter.SetColumnInfo(0, 200, 10);
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_wndList.InsertColumn(0, "动作名称", LVCFMT_LEFT, 120);
	m_wndList.InsertColumn(1, "类型", LVCFMT_LEFT, 80);

	// 在m_pGrid2创建时间交互窗口
	m_wndTimeAixes.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid2, ID_MONTIONTB);
	m_wndTimeAixes.m_iLayerHeight = 20;
	m_pGrid2->SetInnerCtrl(&m_wndTimeAixes);

	m_wndSplitter.ShowWindow(SW_SHOW);
	SetTimeAixLevel(3);

	m_dropTarget.Register(&m_wndTimeAixes);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	//撤掉一些按钮
	m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_BTN_MO_MERGE));
	m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_BTN_MO_APART));
	m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_BTN_MON_SAVETOLIB));
	m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_BTN_MON_SAVEOBJASMON));
	m_wndToolBar.RemoveButton(m_wndToolBar.CommandToIndex(ID_BTN_MON_SAVESHOWMOD));

	// 保存按钮
	int imageid = m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_MON_SAVEMON))->GetImage();
	CBCGPToolbarMenuButton mnuSave(ID_BTN_MON_SAVEMON, GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_MOTIONSAVE)), 0), imageid);
	m_wndToolBar.ReplaceButton(ID_BTN_MON_SAVEMON, mnuSave);

	//后期效果按钮
	imageid = m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_MO_NEW_POST))->GetImage();
	CBCGPToolbarMenuButton mnupost(ID_BTN_MO_NEW_POST, GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_NEW_POST)), 0), imageid);
	m_wndToolBar.ReplaceButton(ID_BTN_MO_NEW_POST, mnupost);

	return 0;
}

void CMontionEditPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

// 新动作
void CMontionEditPane::OnNewMotion()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
	pmo->fTime = 1.0f;
	pmo->name = "新行为";
	pmo->loop = Ogre::ML_ONCE;
	pmo->bIsPublic = false;
	pmo->stMotionLodLevel = (int)Ogre::MOTION_LOD_HIGH;

	if(pInfo->AddMotion(pmo) < 0)
	{
		delete pmo;
	}
	else
	{
		UpdateList();
		PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION_BRO_PRIVATE);
	}
}

// 删除动作
void CMontionEditPane::OnDelMotion()
{
	int index = m_wndList.GetSelectionMark();
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if(index >=0 && index < (int)pInfo->m_motions.size())
	{
		DWORD updateFlag;
		int msgret = AfxMessageBox("同时删除所有特效元素吗?", MB_YESNOCANCEL);
		if(msgret == IDCANCEL)
		{
			return;
		}
		else if(msgret == IDYES)
		{
			pInfo->DeleteMontion(index, true); 
			updateFlag = UE_MOTION_BRO_PRIVATE | UE_EFFECT_LIST;
		}
		else
		{
			pInfo->DeleteMontion(index); 
			updateFlag = UE_MOTION_BRO_PRIVATE;
		}
		UpdateList();
		PostMainFrameMsg(EEM_UPDATA_LIST, updateFlag);
	}
}

// 从骨骼建立
void CMontionEditPane::OnNewMotionFromBone()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0)
		return;

	if(iMotion < (int)pInfo->m_motions.size())
	{
		MessageBox("当前选择的不是一个骨骼");
		return;
	}

	CString strAnimName = m_wndList.GetItemText(iMotion,  0);
	int id = atoi(strAnimName.GetBuffer());
	CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
	pmo->fTime = 1.0f;
	pmo->name = strAnimName.GetBuffer();
	pmo->loop = Ogre::ML_ONCE;
	pmo->bIsPublic = false;
	iMotion = pInfo->AddMotion(pmo);
	if(iMotion < 0)
	{
		delete pmo;
		return;
	}

	if(id >= 0)
	{
		CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_ANIM_ADD, Ogre::PE_BLOOM);
		pobj->m_start = 0;
		pobj->m_end = 10;
		pobj->m_hasStart = true;
		pobj->m_hasEnd = true;
		pobj->m_anim = id;
		pobj->m_priority = 10;
		pobj->m_type = Ogre::MOT_ANIM_ADD;
		pInfo->AddObjLayer(iMotion, pobj);

	}
	UpdateList();
	m_wndList.SetSelectionMark(iMotion);
	m_wndList.SetItem(iMotion, 0, LVIF_STATE, NULL, NULL, LVIS_SELECTED, 0, 0, 0);
	m_wndList.EnsureVisible(iMotion, FALSE);
	pInfo->SetSelect(iMotion, -1);
}


void CMontionEditPane::OnCopyLayer()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->m_modelMotion;
	int iMontion = m_wndList.GetSelectionMark();
	if(iMontion < 0 || iMontion >= (int)pInfo->m_motions.size())
	{
		return;
	}

	CModelMotionInfo::MOTION* pMotion = pInfo->m_motions[iMontion];

	int num[1] = {0};
	bool* selMul = m_wndTimeAixes.GetMultiSelLayer(num);
	assert(num[0] != 0);

	std::vector< std::pair<int,int> > nVector;
	//对选中的层进行偏移处理
	for(int i=0; i<num[0] && i<pMotion->objs.size(); i++)
	{
		if(selMul[i])
		{
			int nExcToLayer = ComputeRelativeLayer(i);
			nVector.push_back(std::pair<int,int>(i, nExcToLayer));
		}
	}
	for(int j=0; j<nVector.size(); j++)
		selMul[nVector[j].first] = false;
	for(int j=0; j<nVector.size(); j++)
		selMul[nVector[j].second] = true;

	for(int i=0; i<num[0]; i++)
	{
		if(i >= pMotion->objs.size())
			break;
		if(selMul[i])
		{
			CObjectLayerInfo* pObj = pMotion->objs[i];
			CObjectLayerInfo* pCopyObj = pObj->CreateClone();
	
			pInfo->AddObjLayer(iMontion, pCopyObj);
		}
	}

	pInfo->ArrangeOrder(pMotion);
	UpdateTimeBarList();
}

// 新动作层
void CMontionEditPane::OnAddEffect()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0)
		return;

	if(iMotion>= (int)pInfo->m_motions.size())
	{
		CString strAnimName = m_wndList.GetItemText(iMotion,  0);
		CString strMsg;
		strMsg.Format("当前选择的动作“%s”是一个骨骼动画", strAnimName.GetBuffer());
		if(IDYES == MessageBox(strMsg.GetBuffer(), "提示", MB_YESNO))
		{
			int id = atoi(strAnimName.GetBuffer());
			CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
			pmo->fTime = 1.0f;
			pmo->name = strAnimName.GetBuffer();
			pmo->loop = Ogre::ML_ONCE;
			pmo->bIsPublic = false;
			iMotion = pInfo->AddMotion(pmo);
			if(iMotion < 0)
			{
				delete pmo;
				return;
			}

			if(id >= 0)
			{
				CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_ANIM_ADD, Ogre::PE_BLOOM);
				pobj->m_start = 0;
				pobj->m_end = 10;
				pobj->m_hasStart = true;
				pobj->m_hasEnd = true;
				pobj->m_anim = id;
				pobj->m_priority = 10;
				pobj->m_type = Ogre::MOT_ANIM_ADD;
				pInfo->AddObjLayer(iMotion, pobj);

			}
			UpdateList();
			m_wndList.SetSelectionMark(iMotion);
			m_wndList.SetItem(iMotion, 0, LVIF_STATE, NULL, NULL, LVIS_SELECTED, 0, 0, 0);
			m_wndList.EnsureVisible(iMotion, FALSE);
			pInfo->SetSelect(iMotion, -1);
		}
		else
		{
			return;
		}
	}


	CLayerInfoDlg dlg;
	if(!m_CreateLayerByDlg || IDOK == dlg.DoModal())
	{
		if(!m_CreateLayerByDlg)
		{
			m_CreateLayerByDlg = true;
			dlg.m_type = m_CreateLayerParam.m_type;
			dlg.m_postEffect = m_CreateLayerParam.m_postEffect;
			dlg.m_fStart = m_CreateLayerParam.m_fStart;
			dlg.m_fEnd = m_CreateLayerParam.m_fEnd;
			dlg.m_bNoStart = m_CreateLayerParam.m_bNoStart;
			dlg.m_bNoEnd = m_CreateLayerParam.m_bNoEnd;
		}
		if(dlg.m_type == Ogre::MOT_ANIM && (dlg.m_iAnim < 0)) return;

		CModelMotionInfo::MOTION* pm = pInfo->m_motions[iMotion];
		CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(dlg.m_type, dlg.m_postEffect);

		pobj->m_start = dlg.m_fStart;
		pobj->m_end = dlg.m_fEnd;
		pobj->m_hasStart = (dlg.m_bNoStart == FALSE) ? true : false;
		pobj->m_hasEnd = (dlg.m_bNoEnd == FALSE) ? true : false;

		if(dlg.m_type == Ogre::MOT_MODEL)
		{
			CEffectUnit* pEffect = NULL;
			if(dlg.m_iSel >= 0)
			{
				pEffect = CEffectMgr::GetInstance()->m_Effects[dlg.m_iSel];
			}
			else
			{//增加新特效

				int iSelMotion = pInfo->m_iSelMontion;
				int iSelObj = pInfo->m_iSelObj;

				//添加特效会刷新面板，导致选择状态丢失，所以保存重设
				SendMainFrameMsg(EEM_NEW_EFFECT, dlg.m_newType, 0);
				int count = CEffectMgr::GetInstance()->m_Effects.size();
				pEffect = CEffectMgr::GetInstance()->m_Effects[count - 1];

				pInfo->SetSelect(iSelMotion, iSelObj);
				m_wndList.SetSelectionMark(iMotion);
			}

			CEffectMgr::GetInstance()->ChangeEffectType(pEffect, true);
			sscanf(dlg.m_strAnchor, "%d", &(pobj->m_anchor) );

			pobj->m_offset = Ogre::Vector3(dlg.m_fx, dlg.m_fy, dlg.m_fz);
			pobj->m_rotate = dlg.m_rotate;
			pobj->m_scale = dlg.m_scale;
			pobj->m_type = Ogre::MOT_MODEL_2;

			Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframePos;
			Ogre::KeyFrameArray<Ogre::Quaternion>::KEYFRAME_T newframeRot;
			Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeScale;
			Ogre::KeyFrameArray<float>::KEYFRAME_T newframeTrans;
			Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeRotAngle;

			newframePos.data = Ogre::Vector3(0.0f, 0.0f, 0.0f);
			newframeRot.data.identity();
			newframeScale.data = Ogre::Vector3(1.0f, 1.0f, 1.0f);
			newframeTrans.data = 1.0f;
			newframeRotAngle.data = Ogre::Vector3(0.0f, 0.0f, 0.0f);

			newframePos.tick = 0;
			pobj->m_posKey.m_Array.push_back(newframePos);

			//newframeRot.tick = 0;
			//pobj->m_rotKey.m_Array.push_back(newframeRot);

			newframeScale.tick = 0;
			pobj->m_scaleKey.m_Array.push_back(newframeScale);

			newframeTrans.tick = 0;
			pobj->m_transparent_keys.m_Array.push_back(newframeTrans);

			newframeRotAngle.tick = 0;
			pobj->m_rotAngleKey.m_Array.push_back(newframeRotAngle);

			pobj->m_pEffect = pEffect;
		}
		else if(dlg.m_type == Ogre::MOT_ANIM)
		{
			if(dlg.m_bOverwrite == FALSE)
				dlg.m_type = Ogre::MOT_ANIM_ADD;

			pobj->m_type = dlg.m_type;
			pobj->m_anim = dlg.m_iAnim;
			pobj->m_priority = dlg.m_iPriority;

		}
		else if(dlg.m_type == Ogre::MOT_POSTEFFECT)
		{
			pobj->m_type = dlg.m_type;
			pobj->m_posteffect = dlg.m_postEffect;
			pobj->m_fs_color = dlg.m_fsColor;

			if(pobj->m_posteffect == Ogre::PE_FSEFFECT)
			{
				Ogre::KeyFrameArray<Ogre::Vector4>::KEYFRAME_T kf;
				kf.tick = 0;
				kf.data = Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
				pobj->m_fs_colorkeys.m_Array.push_back(kf);
				pobj->m_hasStart = false;
				pobj->m_hasEnd = false;
			}
			else if(pobj->m_posteffect == Ogre::PE_TRANSPARENT)
			{
				Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
				kf.tick = 0;
				kf.data = 1.0f;
				pobj->m_transparent_keys.m_Array.push_back(kf);
				pobj->m_hasStart = false;
				pobj->m_hasEnd = false;
			}
			else if(pobj->m_posteffect == Ogre::PE_ZOOM)
			{
				Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
				kf.tick = 0;
				kf.data = 1.0f;
				pobj->m_transparent_keys.m_Array.push_back(kf);
				pobj->m_hasStart = false;
				pobj->m_hasEnd = false;
			}
			else if(pobj->m_posteffect == Ogre::PE_SHAKE)
			{
				Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
				kf.tick = 0;
				kf.data = 0.0f;
				pobj->m_transparent_keys.m_Array.push_back(kf);
				pobj->m_hasStart = pobj->m_hasEnd = false;
			}
			else if(pobj->m_posteffect == Ogre::PE_CUSTOM)
			{
				pobj->m_customPE.clear();
			}

		}

		pInfo->AddObjLayer(iMotion, pobj);

		UpdateTimeBarList(pm);
		m_wndList.SetSelectionMark(iMotion);
	}
}
/*
void CMontionEditPane::OnNewEventObjLayer()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0)
		return;

	if(iMotion>= (int)pInfo->m_motions.size())
	{
		CString strAnimName = m_wndList.GetItemText(iMotion,  0);
		CString strMsg;
		strMsg.Format("当前选择的动作“%s”是一个骨骼动画", strAnimName.GetBuffer());
		if(IDYES == MessageBox(strMsg.GetBuffer(), "提示", MB_YESNO))
		{
			int id = CEffectMgr::GetInstance()->m_pAnimMap->getIDByName(strAnimName.GetBuffer());
			CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
			pmo->fTime = 1.0f;
			pmo->name = strAnimName.GetBuffer();
			pmo->loop = Ogre::ML_ONCE;
			pmo->bIsPublic = false;
			iMotion = pInfo->AddMotion(pmo);
			if(iMotion < 0)
			{
				delete pmo;
				return;
			}

			if(id >= 0)
			{
				CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_ANIM_ADD, Ogre::PE_BLOOM);
				pobj->m_start = 0;
				pobj->m_end = 10;
				pobj->m_hasStart = true;
				pobj->m_hasEnd = true;
				pobj->m_anim = id;
				pobj->m_priority = 10;
				pobj->m_type = Ogre::MOT_ANIM_ADD;
				pInfo->AddObjLayer(iMotion, pobj);

			}
			UpdateList();
			m_wndList.SetSelectionMark(iMotion);
			m_wndList.SetItem(iMotion, 0, LVIF_STATE, NULL, NULL, LVIS_SELECTED, 0, 0, 0);
			m_wndList.EnsureVisible(iMotion, FALSE);
			pInfo->SetSelect(iMotion, -1);
		}
		else
		{
			return;
		}
	}

	int iSelMotion = pInfo->m_iSelMontion;
	int iSelObj = pInfo->m_iSelObj;

	CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_EVENTS, Ogre::PE_BLOOM);
	pobj->m_type = Ogre::MOT_EVENTS;
	pobj->m_start = 0.0f;
	pobj->m_end = 1.0f;
	pobj->m_hasStart = false;
	pobj->m_hasEnd = false;


	pInfo->AddObjLayer(iMotion, pobj);

	UpdateTimeBarList(pInfo->m_motions[iMotion]);
	m_wndTimeAixes.Invalidate(FALSE);
	m_wndList.SetSelectionMark(iMotion);
}*/

// 新层
void CMontionEditPane::OnNewObjLayer(UINT nCmdId)
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0)
		return;

	if(iMotion>= (int)pInfo->m_motions.size())
	{
		CString strAnimName = m_wndList.GetItemText(iMotion,  0);
		CString strMsg;
		strMsg.Format("当前选择的动作“%s”是一个骨骼动画", strAnimName.GetBuffer());
		if(IDYES == MessageBox(strMsg.GetBuffer(), "提示", MB_YESNO))
		{
			int id = atoi(strAnimName.GetBuffer());
			CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
			pmo->fTime = 1.0f;
			pmo->name = strAnimName.GetBuffer();
			pmo->loop = Ogre::ML_ONCE;
			pmo->bIsPublic = false;
			iMotion = pInfo->AddMotion(pmo);
			if(iMotion < 0)
			{
				delete pmo;
				return;
			}

			if(id >= 0)
			{
				CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_ANIM_ADD, Ogre::PE_BLOOM);
				pobj->m_start = 0;
				pobj->m_end = 10;
				pobj->m_hasStart = true;
				pobj->m_hasEnd = true;
				pobj->m_anim = id;
				pobj->m_priority = 10;
				pobj->m_type = Ogre::MOT_ANIM_ADD;
				pInfo->AddObjLayer(iMotion, pobj);

			}
			UpdateList();
			m_wndList.SetSelectionMark(iMotion);
			m_wndList.SetItem(iMotion, 0, LVIF_STATE, NULL, NULL, LVIS_SELECTED, 0, 0, 0);
			m_wndList.EnsureVisible(iMotion, FALSE);
			pInfo->SetSelect(iMotion, -1);
		}
		else
		{
			return;
		}
	}

	int iSelMotion = pInfo->m_iSelMontion;
	if (iSelMotion < 0)	return;
	int iSelObj = pInfo->m_iSelObj;

	EFFECT_TYPE type;
	switch(nCmdId)
	{
	case ID_BTN_MO_NEW_PATTICLE:	type = EFFECT_PEMITTER;		break;
	case ID_BTN_MO_NEW_BILLBOARD:	type = EFFECT_BILLBOARD;	break;
	case ID_BTN_MO_NEW_LIGHT :		type = EFFECT_LIGHT;		break;
	case ID_BTN_MO_NEW_RIBBON:		type = EFFECT_RIBBON;		break;
	case ID_BTN_MO_NEW_SOUND:		type = EFFECT_SOUND;		break;
	case ID_BTN_MO_NEW_DECAL:		type = EFFECT_DECAL;		break;
	case ID_BTN_MO_NEW_BEAM:		type = EFFECT_BEAM;			break;
	case ID_BTN_MO_NEW_GEOM:		type = EFFECT_GEOM;			break;
	case ID_BTN_MO_NEW_VIRTUAL:		type = EFFECT_VIRTUALNODE;	break;
	case ID_BTN_MO_NEW_POST:		type = EFFECT_POSTEFFECT;	break;
	default:
		assert(0 && "Unknown Command ID to create new effect layer");
		return;
	}

	//添加特效会刷新面板，导致选择状态丢失，所以保存重设
	SendMainFrameMsg(EEM_NEW_EFFECT, type, 0);
	int count = CEffectMgr::GetInstance()->m_Effects.size();
	CEffectUnit* pEffect = CEffectMgr::GetInstance()->m_Effects[count - 1];

	pInfo->SetSelect(iSelMotion, iSelObj);
	m_wndList.SetSelectionMark(iMotion);

	CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_MODEL_2, Ogre::PE_BLOOM);
	pobj->m_eEffectType = type;
	CEffectMgr::GetInstance()->ChangeEffectType(pEffect, true);

	pobj->m_anchor = 0;
	pobj->m_offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	pobj->m_rotate.identity();
	pobj->m_scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);
	pobj->m_type = Ogre::MOT_MODEL_2;

	Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframePos;
	//Ogre::KeyFrameArray<Ogre::Quaternion>::KEYFRAME_T newframeRot;
	Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeScale;
	Ogre::KeyFrameArray<float>::KEYFRAME_T newframeTrans;
	Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeRotAngle;

	newframePos.data = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	//newframeRot.data.identity();
	newframeScale.data = Ogre::Vector3(1.0f, 1.0f, 1.0f);
	newframeTrans.data = 1.0f;
	newframeRotAngle.data = Ogre::Vector3(0.0f, 0.0f, 0.0f);

	newframePos.tick = 0;
	pobj->m_posKey.m_Array.push_back(newframePos);

	//newframeRot.tick = 0;
	//pobj->m_rotKey.m_Array.push_back(newframeRot);

	newframeScale.tick = 0;
	pobj->m_scaleKey.m_Array.push_back(newframeScale);

	newframeTrans.tick = 0;
	pobj->m_transparent_keys.m_Array.push_back(newframeTrans);

	newframeRotAngle.tick = 0;
	pobj->m_rotAngleKey.m_Array.push_back(newframeRotAngle);

	pobj->m_pEffect = pEffect;
	pobj->m_start = 0.0f;
	pobj->m_end = 1.0f;
	pobj->m_hasStart = true;
	pobj->m_hasEnd = true;


	pInfo->AddObjLayer(iMotion, pobj);

	UpdateTimeBarList(pInfo->m_motions[iMotion]);
	m_wndTimeAixes.Invalidate(FALSE);
	m_wndList.SetSelectionMark(iMotion);
}

LRESULT CMontionEditPane::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	if(pdn->hWnd == m_wndTimeAixes.GetSafeHwnd())
	{
		CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
		int iMotion = m_wndList.GetSelectionMark();
		if(iMotion < 0 || iMotion >= (int)pInfo->m_motions.size())
			return 0;

		Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(pdn->strChunk);

		if(pChunk == NULL)
			return 0;
		if(CEffectUnit::CanEdit(pChunk))
		{
			NEWEFFECT_INFO ni;
			ni.anchor = 0;
			ni.animmode = Ogre::BAM_SELF;
			ni.animname = "";
			ni.bDynamic = true;
			ni.bIntegrated = true;
			ni.bIsPath = true;
			ni.bShowPane = true;
			ni.offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
			ni.path = pdn->strChunk;
			ni.pChunk = NULL;
			ni.realpath = pdn->strChunk;
			ni.rotate.identity();
			ni.scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);


			int iSelMotion = pInfo->m_iSelMontion;
			int iSelObj = pInfo->m_iSelObj;


			//添加特效会刷新面板，导致选择状态丢失，所以保存重设

			SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0, (LPARAM)&ni);
			int count = CEffectMgr::GetInstance()->m_Effects.size();
			CEffectUnit* pEffect = CEffectMgr::GetInstance()->m_Effects[count - 1];

			pInfo->SetSelect(iSelMotion, iSelObj);
			m_wndList.SetSelectionMark(iMotion);

			CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_MODEL_2, Ogre::PE_BLOOM);
			CEffectMgr::GetInstance()->ChangeEffectType(pEffect, true);

			pobj->m_anchor = 0;
			pobj->m_offset = Ogre::Vector3(0.0f, 0.0f, 0.0f);
			pobj->m_rotate.identity();
			pobj->m_scale = Ogre::Vector3(1.0f, 1.0f, 1.0f);
			pobj->m_type = Ogre::MOT_MODEL_2;

			Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframePos;
			Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeScale;
			Ogre::KeyFrameArray<float>::KEYFRAME_T newframeTrans;
			Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T newframeRotAngle;

			newframePos.data = Ogre::Vector3(0.0f, 0.0f, 0.0f);
			newframeScale.data = Ogre::Vector3(1.0f, 1.0f, 1.0f);
			newframeTrans.data = 1.0f;
			newframeRotAngle.data = Ogre::Vector3(0.0f, 0.0f, 0.0f);

			//pobj->m_posKey.m_bUsed = true;
			//pobj->m_scaleKey.m_bUsed = true;
			//pobj->m_transparent_keys.m_bUsed = true;
			//pobj->m_rotAngleKey.m_bUsed = true;

			newframePos.tick = 0;
			pobj->m_posKey.m_Array.push_back(newframePos);

			newframeScale.tick = 0;
			pobj->m_scaleKey.m_Array.push_back(newframeScale);

			newframeTrans.tick = 0;
			pobj->m_transparent_keys.m_Array.push_back(newframeTrans);

			newframeRotAngle.tick = 0;
			pobj->m_rotAngleKey.m_Array.push_back(newframeRotAngle);

			pobj->m_pEffect = pEffect;
			pobj->m_start = 0.0f;
			pobj->m_end = 1.0f;
			pobj->m_hasStart = true;
			pobj->m_hasEnd = true;


			pInfo->AddObjLayer(iMotion, pobj);

			UpdateTimeBarList(pInfo->m_motions[iMotion]);
			m_wndTimeAixes.Invalidate(FALSE);
			m_wndList.SetSelectionMark(iMotion);

		}
		else if(IS_KIND_OF(Ogre::EntityMotionData, pChunk))
		{
			CModelMotionInfo::MOTION* pMotion = new CModelMotionInfo::MOTION();
			pInfo->LoadSingleMotion((Ogre::EntityMotionData *)pChunk, pMotion);
			pInfo->MergeMotion(pMotion, iMotion);
			delete pMotion;

			UpdateTimeBarList();
		}

		OGRE_RELEASE(pChunk);

	}
	return 0;
}

void CMontionEditPane::OnObjLayerLodChange(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	int nLayer = pTAHDR->layer;
	int nLod = (int)pTAHDR->otherParam;

	bool* pbSelLayer = pTAHDR->selLayers;
	int nMaxLayers = pTAHDR->maxLayer;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if(pInfo->m_iSelMontion < 0 || pInfo->m_iSelMontion >= (int)pInfo->m_motions.size())
		return;

	CModelMotionInfo::MOTION* stpMotion = pInfo->m_motions[pInfo->m_iSelMontion];

	std::vector< std::pair<int,int> > nVector;
	//对选中的层进行偏移处理
	for(int i=0; i<nMaxLayers && i<stpMotion->objs.size(); i++)
	{
		if(pbSelLayer[i])
		{
			int nExcToLayer = ComputeRelativeLayer(i);
			nVector.push_back(std::pair<int,int>(i, nExcToLayer));
		}
	}
	for(int j=0; j<nVector.size(); j++)
		pbSelLayer[nVector[j].first] = false;
	for(int j=0; j<nVector.size(); j++)
		pbSelLayer[nVector[j].second] = true;

	//单层修改LOD，现在一般不会用到，都统一用bool*表示
	if(pbSelLayer == NULL && nMaxLayers == 0)
	{
		/*
		CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
		if(pInfo->m_iSelMontion < 0 || pInfo->m_iSelMontion >= (int)pInfo->m_motions.size())
		return;
		CModelMotionInfo::MOTION* stpMotion = pInfo->m_motions[pInfo->m_iSelMontion];
		CObjectLayerInfo* pObjLayer = stpMotion->objs[nLayer];

		if(nLod == (int)Ogre::MOTION_LOD_NOHANDLE && pObjLayer->m_pParent != NULL)
		{
		MessageBox("非顶层对象层不能设置成 \"不处理\" 状态", "警告 ！", MB_OK);
		return;
		}

		CString strCurName = pObjLayer->GetDisplayName();
		/ *与原4的优先级比较* /
		if(nLod == pObjLayer->m_iObjLayerLodLevel)
		return;

		else if(nLod == (int)Ogre::MOTION_LOD_NOHANDLE && pObjLayer->m_pParent == NULL)
		{
		pObjLayer->m_iObjLayerLodLevel = (int)Ogre::MOTION_LOD_NOHANDLE;

		for(UINT i=0; i<stpMotion->objs.size(); i++)
		{
		if(stpMotion->objs[i]->m_pParent == pObjLayer || stpMotion->objs[i] == pObjLayer)
		{
		stpMotion->objs[i]->RepresentAtTime(pInfo->m_fHotTime, pInfo->m_motions[pInfo->m_iSelMontion]->stMotionLodLevel);
		}
		}
		return;
		}

		else if(pObjLayer->m_iObjLayerLodLevel == (int)Ogre::MOTION_LOD_NOHANDLE)
		{
		std::vector<CObjectLayerInfo*> child;
		for(int i=0; i<stpMotion->objs.size(); i++)
		{
		if((stpMotion->objs[i]->m_pParent == pObjLayer) && (stpMotion->objs[i]->m_iObjLayerLodLevel < nLod))
		{
		child.push_back(stpMotion->objs[i]);
		}
		}
		if(child.size() > 0)
		{
		CString strName = child[0]->GetDisplayName();
		CString strOut = "调整不处理层 " + strCurName + " 的LOD会对 " + strName + " 等子层产生影响";
		if(IDYES == MessageBox(strOut, "是否继续执行 ？", MB_YESNO | MB_ICONWARNING))
		{
		for(int i=0; i<child.size(); i++)
		{
		for(int j=0; j<stpMotion->objs.size(); j++)
		{
		if((stpMotion->objs[j]->m_pParent == child[i]) && (stpMotion->objs[j]->m_iObjLayerLodLevel < nLod))
		{
		child.push_back(stpMotion->objs[j]);
		}
		}
		}
		for(int i=0; i<child.size(); i++)
		child[i]->m_iObjLayerLodLevel = nLod;
		}
		else
		return;
		}
		pObjLayer->m_iObjLayerLodLevel = nLod;

		for(UINT i=0; i<stpMotion->objs.size(); i++)
		{
		bool bIsAncestor = false;
		CObjectLayerInfo* pParent = stpMotion->objs[i];
		while(pParent)
		{
		if(pParent == pObjLayer)
		{
		bIsAncestor = true;
		break;
		}
		pParent = pParent->m_pParent;
		}
		if(bIsAncestor)
		stpMotion->objs[i]->RepresentAtTime(pInfo->m_fHotTime, stpMotion->stMotionLodLevel);
		}
		}

		/ *降低自身 Lod ，只对它的父层有影响，不处理层除外* /
		else if(nLod < pObjLayer->m_iObjLayerLodLevel)
		{
		CObjectLayerInfo* pParent = pObjLayer->m_pParent;
		while(pParent)
		{
		CString strParentName;
		if(nLod < pParent->m_iObjLayerLodLevel)
		{
		if(pParent->m_iObjLayerLodLevel == (int)Ogre::MOTION_LOD_NOHANDLE)
		{
		pObjLayer->m_iObjLayerLodLevel = nLod;
		}
		else
		{
		CString temp;
		temp = strCurName + " 层的修改对父层 ";
		strParentName = pParent->GetDisplayName();
		temp += strParentName + " 有影响 ！";

		CString temp2 = "是否强制修改父层 " + strParentName;
		temp2 += " 、及再父层的LOD级别 ？";
		if(IDYES == MessageBox(temp, temp2, MB_YESNO | MB_ICONWARNING))
		{
		while(pParent && (pParent->m_iObjLayerLodLevel != (int)Ogre::MOTION_LOD_NOHANDLE))
		{
		if(pParent->m_iObjLayerLodLevel > nLod)
		pParent->m_iObjLayerLodLevel = nLod;
		pParent = pParent->m_pParent;
		}
		pObjLayer->m_iObjLayerLodLevel = nLod;
		}
		}
		return;
		}
		pParent = pParent->m_pParent;
		}
		pObjLayer->m_iObjLayerLodLevel = nLod;
		}

		/ *提高自身 Lod ，只对它的孩子节点有影响* /
		else if(nLod > pObjLayer->m_iObjLayerLodLevel)
		{
		std::vector<CObjectLayerInfo*> child;
		for(int i=0; i<stpMotion->objs.size(); i++)
		{
		/ *取得当前层的子层中比欲设置的级别(nLod)低的层* /
		if((stpMotion->objs[i]->m_pParent == pObjLayer) && (stpMotion->objs[i]->m_iObjLayerLodLevel < nLod))
		{
		child.push_back(stpMotion->objs[i]);
		}
		}

		if(child.size() > 0)
		{
		CString strName, strOut;
		strName = child[0]->GetDisplayName();
		strName += " 等子层LOD水平比 ";
		strName += strCurName;
		strName += " 低";
		strOut = "强制修改子层到";
		strOut += strCurName + " 的级别 ？";
		if(IDYES == MessageBox(strName, strOut, MB_YESNO | MB_ICONWARNING))
		{
		for(int i=0; i<child.size(); i++)
		{
		for(int j=0; j<stpMotion->objs.size(); j++)
		{
		if((stpMotion->objs[j]->m_pParent == child[i]) && (stpMotion->objs[j]->m_iObjLayerLodLevel < nLod))
		{
		child.push_back(stpMotion->objs[j]);
		}
		}
		}
		for(int i=0; i<child.size(); i++)
		child[i]->m_iObjLayerLodLevel = nLod;
		pObjLayer->m_iObjLayerLodLevel = nLod;
		}
		return;
		}
		pObjLayer->m_iObjLayerLodLevel = nLod;
		}
		*/
	}
	//因特效不需要了“不处理”层，所以多重处理时去掉了此类代码
	else
	{
		for(int i=0; i<nMaxLayers; i++)
		{
			if(i > stpMotion->objs.size())
				break;
			if(pbSelLayer[i])
			{
				CObjectLayerInfo* pObjLayer = stpMotion->objs[i];

				if(nLod == pObjLayer->m_iObjLayerLodLevel)
					break;
				/*降低自身 Lod ，只对它的父层有影响*/
				else if(nLod < pObjLayer->m_iObjLayerLodLevel)
				{
					CObjectLayerInfo* pParent = pObjLayer->getParent();
					while(pParent)
					{
						CString strParentName;
						if(nLod < pParent->m_iObjLayerLodLevel)
						{
							CString temp = "选定层的修改对父层有影响";
							CString temp2 = "是否强制修改父层 LOD 级别 ？";

							if(IDYES == MessageBox(temp2, temp, MB_YESNO | MB_ICONWARNING))
							{
								while(pParent && (pParent->m_iObjLayerLodLevel != (int)Ogre::MOTION_LOD_NOHANDLE))
								{
									if(pParent->m_iObjLayerLodLevel > nLod)
										pParent->m_iObjLayerLodLevel = nLod;
									pParent = pParent->getParent();
								}
								pObjLayer->m_iObjLayerLodLevel = nLod;
							}
							goto TO1;
						}
						pParent = pParent->getParent();
					}
					pObjLayer->m_iObjLayerLodLevel = nLod;
TO1:;
				}
				/*提高自身 Lod ，只对它的孩子节点有影响*/
				else if(nLod > pObjLayer->m_iObjLayerLodLevel)
				{
					std::vector<CObjectLayerInfo*> child;
					for(int i=0; i<stpMotion->objs.size(); i++)
					{
						/*取得当前层的子层中比欲设置的级别(nLod)低的层*/
						if((stpMotion->objs[i]->getParent() == pObjLayer) && (stpMotion->objs[i]->m_iObjLayerLodLevel < nLod))
						{
							child.push_back(stpMotion->objs[i]);
						}
					}

					if(child.size() > 0)
					{
						CString temp = "选定层的修改对子层有影响";
						CString temp2 = "是否强制修改子层的 LOD 级别 ？";

						if(IDYES == MessageBox(temp2,temp, MB_YESNO | MB_ICONWARNING))
						{
							for(int i=0; i<child.size(); i++)
							{
								for(int j=0; j<stpMotion->objs.size(); j++)
								{
									if((stpMotion->objs[j]->getParent() == child[i]) && (stpMotion->objs[j]->m_iObjLayerLodLevel < nLod))
									{
										child.push_back(stpMotion->objs[j]);
									}
								}
							}
							for(int i=0; i<child.size(); i++)
								child[i]->m_iObjLayerLodLevel = nLod;
							pObjLayer->m_iObjLayerLodLevel = nLod;
						}
						break;
					}
					pObjLayer->m_iObjLayerLodLevel = nLod;
				}

			}

		}

		for(int i = 0; i < (int)stpMotion->objs.size(); i++)
		{
			stpMotion->objs[i]->RepresentAtTime(pInfo->m_fHotTime, stpMotion->stMotionLodLevel);
		}
	}

	UpdateTimeBarList();
	*pResult = NULL;
}

void CMontionEditPane::OnObjShowByLod(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pHdr = (TANMHDR*)pHDR;
	m_eShowLodState = (AIXES_LOD_SHOW_MODEL)pHdr->otherParam;
	UpdateTimeBarList();
}

void CMontionEditPane::OnDelEffect()
{
	bool delEff = false;
	int msgret = AfxMessageBox("在删除当前层的同时，删除相应的特效元素吗？", MB_YESNOCANCEL);
	if(msgret == IDCANCEL)
	{
		return;
	}
	if(msgret == IDYES)
	{
		delEff = true;
	}

	//int sel = m_wndTimeBarList.GetSelectIndex();
	int num[1] = {0};
	bool* selMul = m_wndTimeAixes.GetMultiSelLayer(num);
	assert(num[0] != 0);

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion >= (int)pInfo->m_motions.size())
		return;
	CModelMotionInfo::MOTION* pMotion = pInfo->m_motions[iMotion];

	std::vector< std::pair<int,int> > nVector;
	//对选中的层进行偏移处理
	for(int i=0; i<num[0] && i<pMotion->objs.size(); i++)
	{
		if(selMul[i])
		{
			int nExcToLayer = ComputeRelativeLayer(i);
			nVector.push_back(std::pair<int,int>(i, nExcToLayer));
		}
	}
	for(int j=0; j<nVector.size(); j++)
		selMul[nVector[j].first] = false;
	for(int j=0; j<nVector.size(); j++)
		selMul[nVector[j].second] = true;

	std::vector<CEffectUnit*> pUnitToDelete;
	for(int i=(num[0]-1); i>=0; i--)
	{
		if( selMul[i] )
		{
			if(pMotion->objs[i]->m_type == Ogre::MOT_MODEL_2 && delEff)
			{
				pUnitToDelete.push_back(pMotion->objs[i]->m_pEffect);
			}
			pInfo->DeleteObj(iMotion, i);
		}
	}
	if(pUnitToDelete.size() != 0)
	{
		for(size_t i=0; i<pUnitToDelete.size(); i++)
		{
			SendMainFrameMsg(EEM_DELETE_EFFECT, 0,(LPARAM)pUnitToDelete[i]);
			pInfo->SetSelect(iMotion, -1); 
			m_wndList.SetSelectionMark(iMotion);
		}
	}

	UpdateTimeBarList(pMotion);
}

void CMontionEditPane::OnSaveMon()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	CString path = GetOpenFilePath(FALSE, "effect", pInfo->m_motions[iMotion]->filename.c_str(), "emo", "Motion Files(*.emo)\0*.emo\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		bool bret = pInfo->SaveSingleMotionToFile(pInfo->m_motions[iMotion], path.GetBuffer());

		if(bret)
		{
			MessageBox("保存成功");
		}
		else
		{
			MessageBox("保存失败");
		}
	}
}

void CMontionEditPane::OnSaveMonToLib()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	CString msg;
	CString strMotionPath = g_strPublicMotionAddr;
	CString strChunkFolder;
	bool	bExistTip = true;

	char* namebuf = new char[512];
	SendMainFrameMsg(EEM_GET_CURR_MOTION_DIR, 512, (LPARAM)namebuf);
	strMotionPath = namebuf;
	strChunkFolder = namebuf;
	delete [] namebuf;

	strMotionPath += pInfo->m_motions[iMotion]->name.c_str();
	strMotionPath += ".emo";

	if(Ogre::ResourceManager::getSingleton().getFileAttrib(strMotionPath.GetBuffer()) == 1)
	{
		msg.Format("“%s” 已经存在，确定覆盖吗？", pInfo->m_motions[iMotion]->name.c_str());
		if(IDOK != MessageBox(msg.GetBuffer(), "提示", MB_OKCANCEL))
		{
			return;
		}
	}

	bool bret = pInfo->SaveSingleMotionToFile(pInfo->m_motions[iMotion], strMotionPath.GetBuffer());
	if(bret)
	{
		msg.Format("保存成功：“%s”", strMotionPath.GetBuffer());
		MessageBox(msg.GetBuffer());

		pInfo->m_motions[iMotion]->bIsPublic = true;
		pInfo->m_motions[iMotion]->filename = strMotionPath.GetBuffer();
		m_wndList.SetItemText(iMotion, 1, "公共特效");
		PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION_BRO_COMMON | UE_MOTION_BRO_PRIVATE);
	}
	else
	{
		MessageBox("保存失败，可能是文件只读或者空间不足");
	}
}

void CMontionEditPane::OnSaveObjAsMotion()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	int nNumLayers[1];
	bool *bSelLayers;
	bSelLayers = m_wndTimeAixes.GetMultiSelLayer( nNumLayers );
	int i;
	for(i=0; i<nNumLayers[0]; i++)
		if( bSelLayers[i] ) break;
	if( i == nNumLayers[0] ) return;

	/*
	int iSelLayer = m_wndTimeAixes.GetSelectedLayer();
	if(iSelLayer < 0 || iSelLayer >= (int)pInfo->m_motions[iMotion]->objs.size())
	return;
	*/

	//	CObjectLayerInfo* pObj = pInfo->m_motions[iMotion]->objs[iSelLayer];	

	CString path = GetOpenFilePath(FALSE, "effect\\material", "", "emo", "Motion Files(*.emo)\0*.emo\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		bool bret = pInfo->SaveMotionObjectAsMotion(pInfo->m_motions[iMotion], -1, path.GetBuffer(), nNumLayers[0], bSelLayers);

		if(bret)
		{
			MessageBox("保存成功");
		}
		else
		{
			MessageBox("保存失败");
		}
	}
}

void CMontionEditPane::OnSaveMonAsShowModel()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	CString strSaveName	= g_strShowModelEffectAddr;
	strSaveName += pInfo->m_motions[iMotion]->name.c_str();

	CString path = GetOpenFilePath(FALSE, "models", strSaveName.GetBuffer(), "ent", "Entity Files(*.ent)\0*.ent\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		bool bret = CEffectMgr::GetInstance()->SaveMotionAsShowModel(path, iMotion);
		if(bret)
		{
			MessageBox("保存成功");
		}
		else
		{
			MessageBox("保存失败");
		}
	}

}

void CMontionEditPane::OnNewCameraShake()
{
	m_CreateLayerByDlg = false;
	m_CreateLayerParam.m_postEffect = Ogre::PE_SHAKE;
	OnAddEffect();
}

void CMontionEditPane::OnNewFrozen()
{
	m_CreateLayerByDlg = false;
	m_CreateLayerParam.m_postEffect = Ogre::PE_FROZEN;
	OnAddEffect();
}

void CMontionEditPane::OnNewTransparent()
{
	m_CreateLayerByDlg = false;
	m_CreateLayerParam.m_postEffect = Ogre::PE_TRANSPARENT;
	OnAddEffect();
}

void CMontionEditPane::OnNewEventObjLayer()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0)
		return;

	if(iMotion>= (int)pInfo->m_motions.size())
	{
		CString strAnimName = m_wndList.GetItemText(iMotion,  0);
		CString strMsg;
		strMsg.Format("当前选择的动作“%s”是一个骨骼动画", strAnimName.GetBuffer());
		if(IDYES == MessageBox(strMsg.GetBuffer(), "提示", MB_YESNO))
		{
			int id = atoi(strAnimName.GetBuffer());
			CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
			pmo->fTime = 1.0f;
			pmo->name = strAnimName.GetBuffer();
			pmo->loop = Ogre::ML_ONCE;
			pmo->bIsPublic = false;
			iMotion = pInfo->AddMotion(pmo);
			if(iMotion < 0)
			{
				delete pmo;
				return;
			}

			if(id >= 0)
			{
				CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_ANIM_ADD, Ogre::PE_BLOOM);
				pobj->m_start = 0;
				pobj->m_end = 10;
				pobj->m_hasStart = true;
				pobj->m_hasEnd = true;
				pobj->m_anim = id;
				pobj->m_priority = 10;
				pobj->m_type = Ogre::MOT_ANIM_ADD;
				pInfo->AddObjLayer(iMotion, pobj);

			}
			UpdateList();
			m_wndList.SetSelectionMark(iMotion);
			m_wndList.SetItem(iMotion, 0, LVIF_STATE, NULL, NULL, LVIS_SELECTED, 0, 0, 0);
			m_wndList.EnsureVisible(iMotion, FALSE);
			pInfo->SetSelect(iMotion, -1);
		}
		else
		{
			return;
		}
	}

	int iSelMotion = pInfo->m_iSelMontion;
	int iSelObj = pInfo->m_iSelObj;

	CObjectLayerInfo* pobj = CObjectLayerInfo::CreateObjectLayer(Ogre::MOT_EVENTS, Ogre::PE_BLOOM);
	pobj->m_type = Ogre::MOT_EVENTS;
	pobj->m_start = 0.0f;
	pobj->m_end = 1.0f;
	pobj->m_hasStart = false;
	pobj->m_hasEnd = false;


	pInfo->AddObjLayer(iMotion, pobj);

	UpdateTimeBarList(pInfo->m_motions[iMotion]);
	m_wndTimeAixes.Invalidate(FALSE);
	m_wndList.SetSelectionMark(iMotion);
}

// 合并动作
void CMontionEditPane::OnMergeMotion()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	CModelMotionInfo::MOTION* pMotion = pInfo->GetSelectedMotion();
	if(pMotion == NULL)
		return;

	CMergeMotionDlg dlg;
	dlg.m_strFromMotion = pMotion->name.c_str();
	if(IDOK == dlg.DoModal())
	{
		bool bDeleteFrom = dlg.m_bNoDelete == FALSE ? true : false;
		pInfo->MergeMotion(pInfo->m_iSelMontion, dlg.m_iToMotion, bDeleteFrom);
		UpdateList();
	}

}

// 载入动作特效
void CMontionEditPane::OnImportMon()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	CString path = GetOpenFilePath(TRUE, "effect", "", "emo", "Motion Files(*.emo)\0*.emo\0\0", TRUE);
	if(!path.IsEmpty())
	{
		CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
		bool bret = pInfo->LoadSingleMotionFromFile(pmo, path.GetBuffer());
		if(bret == false)
		{
			MessageBox("找不到资源或者资源类型错误");
			delete pmo;
			return;
		}
		pmo->bIsPublic = false;
		pmo->filename = "";
		if(pInfo->AddMotion(pmo) < 0)
		{
			delete pmo;
			assert(0 && "Add motion error");
			return;
		}
		//pInfo->m_motions.push_back(pmo);
		UpdateList();
		PostMainFrameMsg(EEM_UPDATA_LIST, UE_EFFECT_LIST);


	}
}

// play
void CMontionEditPane::OnPlayMotion()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 )
		return;
	if(iMotion>= (int)pInfo->m_motions.size())
	{
		CString strAnimName = m_wndList.GetItemText(iMotion, 0);
		int id = atoi(strAnimName.GetBuffer());
		if(id >= 0)
			CEffectMgr::GetInstance()->PlayAnim(id);
	}
	else
	{
		CEffectMgr::GetInstance()->PlayMotion(iMotion);
	}
}

// stop
void CMontionEditPane::OnStopMotion()
{
	CEffectMgr::GetInstance()->StopMotion(0);

}

void CMontionEditPane::OnMergeKeyFrame()
{/*
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	CMergeKfDlg dlg;
	INT_PTR dlgret = dlg.DoModal();
	if(dlgret == IDC_BTN_MG_EFF2MON)
	{
		pInfo->OverwriteMotionFrame(pInfo->m_iSelMontion, pInfo->m_iSelObj);
		UpdateTimeBarList();
	}
	else if(dlgret == IDC_BTN_MG_MON2KF)
	{
		pInfo->OverwriteEffectFrame(pInfo->m_iSelMontion, pInfo->m_iSelObj);
		UpdateTimeBarList();
	}
	else if(dlgret == IDC_BTN_MG_MERGE)
	{
		pInfo->MergeLayerKeyFrame(pInfo->m_iSelMontion, pInfo->m_iSelObj);
		UpdateTimeBarList();
	}

*/
}
void CMontionEditPane::OnApartKeyFrame()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if(true == pInfo->ApartKeyFrame(pInfo->m_iSelMontion, pInfo->m_iSelObj))
		UpdateTimeBarList();
}

// 加大标尺
void CMontionEditPane::OnRulerIn()
{

	//if(m_wndTimeAixes.m_fUnitValue > 0.7f)
	//	m_wndTimeAixes.m_fUnitValue = 0.5f;
	//else if(m_wndTimeAixes.m_fUnitValue > 0.3f)
	//	m_wndTimeAixes.m_fUnitValue = 0.2f;
	//else if(m_wndTimeAixes.m_fUnitValue > 0.15f)
	//	m_wndTimeAixes.m_fUnitValue = 0.1f;
	//else if(m_wndTimeAixes.m_fUnitValue > 0.07f)
	//	m_wndTimeAixes.m_fUnitValue = 0.05f;
	//else if(m_wndTimeAixes.m_fUnitValue > 0.03f)
	//	m_wndTimeAixes.m_fUnitValue = 0.02f;
	//else if(m_wndTimeAixes.m_fUnitValue > 0.015f)
	//	m_wndTimeAixes.m_fUnitValue = 0.01f;

	//UpdateTimeBarList();

	//if(m_wndTimeAixes.m_iStepPixes < 500)
	//{
	//	m_wndTimeAixes.m_iStepPixes += 25;
	//}
	SetTimeAixLevel(m_iCurrAixLevel + 1);
	UpdateTimeBarList();
	m_wndTimeAixes.Invalidate(FALSE);
}

// 缩小标尺
void CMontionEditPane::OnRulerOut()
{
	//if(m_wndTimeAixes.m_fUnitValue < 0.015f)
	//	m_wndTimeAixes.m_fUnitValue = 0.02f;
	//else if(m_wndTimeAixes.m_fUnitValue < 0.03f)
	//	m_wndTimeAixes.m_fUnitValue = 0.05f;	
	//else if(m_wndTimeAixes.m_fUnitValue < 0.07f)
	//	m_wndTimeAixes.m_fUnitValue = 0.1f;
	//else if(m_wndTimeAixes.m_fUnitValue < 0.15f)
	//	m_wndTimeAixes.m_fUnitValue = 0.2f;
	//else if(m_wndTimeAixes.m_fUnitValue < 0.3f)
	//	m_wndTimeAixes.m_fUnitValue = 0.5f;
	//else if(m_wndTimeAixes.m_fUnitValue < 0.7f)
	//	m_wndTimeAixes.m_fUnitValue = 1.0f;

	//UpdateTimeBarList();

	//if(m_wndTimeAixes.m_iStepPixes > 100)
	//{
	//	m_wndTimeAixes.m_iStepPixes -= 25;
	//}

	SetTimeAixLevel(m_iCurrAixLevel - 1);
	UpdateTimeBarList();
	m_wndTimeAixes.Invalidate(FALSE);
}

// 显示路径
void CMontionEditPane::OnShowPath()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	bool bIsPathShow = pInfo->IsShowPath();
	pInfo->ShowPath(!bIsPathShow);
}

void CMontionEditPane::OnUpdateShowPath(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if (pInfo->IsShowPath() )
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

// loopPlay
void CMontionEditPane::OnLoopPlay()
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	bool bLoopPlay = pInfo->IsLoopPlay();
	pInfo->LoopPlay(!bLoopPlay);
}

void CMontionEditPane::OnUpdateLoopPlay(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if (pInfo->IsLoopPlay() )
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}




void CMontionEditPane::OnTimeLayerSelected(NMHDR* pHDR, LRESULT* pResult)
{
	DWORD start = ::GetTickCount();
	DWORD setseltime, onselltime;
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();

	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	pMainFrame->m_wndSrtBar.UpdateList();

	pTAHDR->layer = ComputeRelativeLayer(pTAHDR->layer);
	if(pTAHDR->layer < 0)
	{
		pInfo->SetSelect(pInfo->m_iSelMontion, -1);
	}
	else
	{
		DWORD setselstart = ::GetTickCount();
		pInfo->SetSelect(pInfo->m_iSelMontion, pTAHDR->layer);
		setseltime = ::GetTickCount() - setselstart;

		CObjectLayerInfo& obj =*(pInfo->m_motions[pInfo->m_iSelMontion]->objs[pInfo->m_iSelObj]);

		DWORD onsellstart = ::GetTickCount();
		obj.OnSelected(pInfo->m_iSelObj, &m_wndTimeAixes);
		onselltime = ::GetTickCount() - setselstart;

		m_wndTimeAixes.SetFocus();
	}
	*pResult = 0;


	DWORD spend = ::GetTickCount() - start;

	//	TRACE("OnTimeLayerSelected 耗费时间 %d 毫秒, SetSelect:%d, OnSelect:%d\n", spend, setseltime,onselltime);

}

void CMontionEditPane::OnTimeAixesRedBoxChange(NMHDR* pHDR, LRESULT* pResult)
{
	DWORD start = ::GetTickCount();
	//TRACE("Layer Red box Change\n");
	CModelMotionInfo* pMotion =  CEffectMgr::GetInstance()->GetModelMotion();
	int box = m_wndTimeAixes.GetSelectedBox();
	float time = m_wndTimeAixes.GetBoxValue(box);

	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	pMainFrame->m_wndSrtBar.UpdateList();

	//TRACE("Red box Change at %d\n", box);
	int layer = m_wndTimeAixes.GetSelectedLayer();
	if(layer >= 0)
	{
		int area = m_wndTimeAixes.GetLayer(layer)->IsFirstBlock(box);
		if(area >= 0)
		{
			CObjectLayerInfo* pObj = pMotion->m_motions[pMotion->m_iSelMontion]->objs[layer];
			pObj->OnSelectFrame(layer, area, &m_wndTimeAixes);
			pMotion->SetSelectedKey(area);
		}
		else
		{
			pMotion->SetSelectedKey(-1);
		}
	}
	else
	{
		pMotion->SetSelectedKey(-1);
	}

	pMotion->SetHotTime(time);

	DWORD spend = ::GetTickCount() - start;

	TRACE("OnTimeAixesRedBoxChange 耗费时间 %d 毫秒\n", spend);

}

int CMontionEditPane::ComputeRelativeLayer(int layer)
{
	CModelMotionInfo* pMotionInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion >= (int)pMotionInfo->m_motions.size())
	{
		return layer;
	}

	if(layer < 0 || layer >= (int)pMotionInfo->m_motions[iMotion]->objs.size())
	{
		return layer;
	}

	int nExcusion = 0;
	for(int i=0; i<layer; i++)
	{
		CObjectLayerInfo* pTempObjLayer = pMotionInfo->m_motions[iMotion]->objs[i];
		if((pTempObjLayer->m_eEffectType == EFFECT_VIRTUALNODE) && (pTempObjLayer->m_bFolded))
		{
			for(int j=i+1; j<pMotionInfo->m_motions[iMotion]->objs.size(); j++)
			{
				CObjectLayerInfo* pParent = pMotionInfo->m_motions[iMotion]->objs[j]->getParent();
				while(pParent)
				{
					if(pParent == pTempObjLayer)
					{
						++nExcusion;
						break;
					}
					pParent = pParent->getParent();
				}
			}
		}
	}
	return (layer+nExcusion);
}

void CMontionEditPane::OnTimeLayerClickOption(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pMotionInfo =  CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();

	if(iMotion < 0 || iMotion >= (int)pMotionInfo->m_motions.size())
	{
		return;
	}

	if(pTAHDR->layer < 0 || pTAHDR->layer >= (int)pMotionInfo->m_motions[iMotion]->objs.size())
	{
		return;
	}

	int nLayer = ComputeRelativeLayer(pTAHDR->layer);
	CObjectLayerInfo*	pObjLayer = pMotionInfo->m_motions[iMotion]->objs[nLayer];
	if(pTAHDR->otherParam == 0) //visible ?
	{
		if(pObjLayer->m_bVisible == false)
		{
			pObjLayer->m_bVisible = true;
			m_wndTimeAixes.SetLayerOption(pTAHDR->layer, 0, 1);
		}
		else
		{
			pObjLayer->m_bVisible = false;
			m_wndTimeAixes.SetLayerOption(pTAHDR->layer, 0, 0);
		}
	}

	else if(pTAHDR->otherParam == 1 && pObjLayer->m_eEffectType == EFFECT_VIRTUALNODE) //folded ?
	{
		if(pObjLayer->m_bFolded == false)
		{
			pObjLayer->m_bFolded = true;
			UpdateTimeBarList();
			m_wndTimeAixes.SetLayerOption(pTAHDR->layer, 1, 1);
		}
		else
		{
			pObjLayer->m_bFolded = false;
			UpdateTimeBarList();
			m_wndTimeAixes.SetLayerOption(pTAHDR->layer, 1, 0);
		}
	}

	pObjLayer->RefreshRepresent(pMotionInfo->m_fHotTime);
	m_wndTimeAixes.Invalidate(FALSE);
}


void CMontionEditPane::OnTimeLayerSizeChange(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	int obj = pTAHDR->layer;


	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	pMainFrame->m_wndSrtBar.UpdateList();

	float len = m_wndTimeAixes.GetLayerLenValue(obj); //m_wndTimeBarList.GetTimeBar(obj)->GetBlock(0)->GetLength();
	float headlen = m_wndTimeAixes.GetLayerStartValue(obj); //m_wndTimeBarList.GetTimeBar(obj)->GetHeadLen();

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	pInfo->m_motions[iMotion]->objs[obj]->OnSizeChange(obj, pTAHDR->area, &m_wndTimeAixes);
	pInfo->m_motions[iMotion]->docInfo.Modify();
	*pResult = 0;
}

void CMontionEditPane::OnTimeLayerPosChange(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	int obj = pTAHDR->layer;
	float len = m_wndTimeAixes.GetLayerLenValue(obj); 
	float headlen = m_wndTimeAixes.GetLayerStartValue(obj); 


	CMainFrame* pMainFrame = static_cast<CMainFrame*>( theApp.m_pMainWnd) ;
	pMainFrame->m_wndSrtBar.UpdateList();

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	CObjectLayerInfo& objinfo = *(pInfo->m_motions[iMotion]->objs[obj]);
	objinfo.OnPosChange(pTAHDR->layer, pTAHDR->area, &m_wndTimeAixes);
	pInfo->m_motions[iMotion]->docInfo.Modify();
	//UpdateTimeBarList(pInfo->m_motions[iMotion]);

	*pResult = 0;
}

void CMontionEditPane::OnTimeAixesCtrlDrag(NMHDR* pHDR, LRESULT* pResult)
{	
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;
	CModelMotionInfo::MOTION* pmo = pInfo->m_motions[iMotion];
	CObjectLayerInfo& objinfo = *(pmo->objs[pTAHDR->layer]);
	int block = (int) pTAHDR->otherParam;
	int iArea = iArea = pTAHDR->area;

	objinfo.OnCtrlDrag(pTAHDR->layer,iArea,  block, &m_wndTimeAixes);
	pInfo->m_motions[iMotion]->docInfo.Modify();
	UpdateTimeBarList();

}

void CMontionEditPane::OnTimeLayerActive(NMHDR* pHDR, LRESULT* pResult)
{

	//TRACE("Layer Active\n");
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;
	CModelMotionInfo::MOTION* pmo = pInfo->m_motions[iMotion];
	CObjectLayerInfo& objinfo = *(pmo->objs[pTAHDR->layer]);
	//int block = m_wndTimeAixes.GetSelectedBox();
	int block = pTAHDR->otherParam; //hover box；
	objinfo.OnActive(pTAHDR->layer, block, &m_wndTimeAixes);
	pInfo->m_motions[iMotion]->docInfo.Modify();

	CEffectMgr::GetInstance()->GetModelMotion()->SetSelect(iMotion, pTAHDR->layer);
	UpdateTimeBarList();

	int box = m_wndTimeAixes.GetSelectedBox();
	float time = m_wndTimeAixes.GetBoxValue(box);
	pInfo->SetHotTime(time);

	int layer = m_wndTimeAixes.GetSelectedLayer();
	if(layer >= 0)
	{
		CObjectLayerInfo* pObj = pInfo->m_motions[pInfo->m_iSelMontion]->objs[layer];
		pInfo->SetCoodPlace(pObj, time);

		int area = m_wndTimeAixes.GetLayer(layer)->IsFirstBlock(box);
		if(area >= 0)
		{
			pInfo->SetSelectedKey(area);
			pObj->OnSelectFrame(layer, area, &m_wndTimeAixes);
		}
		else
		{
			pInfo->SetSelectedKey(-1);
		}
	}
	else
	{
		pInfo->SetSelectedKey(-1);
	}

	*pResult = 0;
}

void CMontionEditPane::OnTimeAixesEndChange(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;
	CModelMotionInfo::MOTION* pmo = pInfo->m_motions[iMotion];

	pmo->fTime = m_wndTimeAixes.GetEndlineValue();
	pmo->docInfo.Modify();
	*pResult = 0;
}

void CMontionEditPane::OnTimeLayerKeyDown(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if(pInfo->m_iSelMontion < 0 || pInfo->m_iSelMontion >= (int) pInfo->m_motions.size())
		return;

	if(pInfo->m_iSelObj < 0 || pInfo->m_iSelObj >= (int) pInfo->m_motions[pInfo->m_iSelMontion]->objs.size())
		return;

	CObjectLayerInfo* pObj = pInfo->m_motions[pInfo->m_iSelMontion]->objs[pInfo->m_iSelObj];

	if(pTAHDR->otherParam == VK_DELETE)
	{
		pObj->OnDeleteFrame(pInfo->m_iSelObj, pInfo->m_iSelKey, &m_wndTimeAixes);
		pInfo->m_motions[pInfo->m_iSelMontion]->docInfo.Modify();
		pInfo->SetSelectedKey(-1);
	}
}

void CMontionEditPane::OnTimeAixLayerPos(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if(pInfo->m_iSelMontion < 0 || pInfo->m_iSelMontion >= (int) pInfo->m_motions.size())
		return;
	int iMotion = m_wndList.GetSelectionMark();
	CModelMotionInfo::MOTION* pMotion = pInfo->m_motions[iMotion];

	int iNewPlace; //保存最后一层的新位置
	if((pTAHDR->selLayers == NULL) && (pTAHDR->maxLayer == 0))
	{//single pos
		int iFromLayer = pTAHDR->layer;
		int iInsertAfter = (int)pTAHDR->otherParam;

		iFromLayer = ComputeRelativeLayer(iFromLayer);
		iInsertAfter = ComputeRelativeLayer(iInsertAfter);

		iNewPlace = pInfo->ChangeLayerOrder(pInfo->m_iSelMontion, iFromLayer, iInsertAfter);
	}
	else
	{//multi pos
		int num = pTAHDR->maxLayer;
		bool *selLayers = pTAHDR->selLayers;
		int iParentLayer = (int)pTAHDR->otherParam;

		std::vector< std::pair<int,int> > nVector;
		//对选中的层进行偏移处理
		for(int i=0; i<num && i<pMotion->objs.size(); i++)
		{
			if(selLayers[i])
			{
				int nExcToLayer = ComputeRelativeLayer(i);
				nVector.push_back(std::pair<int,int>(i, nExcToLayer));
			}
		}
		for(int j=0; j<nVector.size(); j++)
			selLayers[nVector[j].first] = false;
		for(int j=0; j<nVector.size(); j++)
			selLayers[nVector[j].second] = true;

		iParentLayer = ComputeRelativeLayer(iParentLayer);

		iNewPlace = pInfo->ChangeLayerOrder(pInfo->m_iSelMontion, selLayers, num, iParentLayer);
	}

	if(iNewPlace > 0)
	{
		UpdateTimeBarList();
		m_wndTimeAixes.SetSelectedLayer(iNewPlace);
		m_wndTimeAixes.ShiftRedBox(-1);
		pInfo->SetSelect(pInfo->m_iSelMontion, iNewPlace);
	}
}

void CMontionEditPane::OnTimeAixLayerRelate(NMHDR* pHDR, LRESULT* pResult)
{
	TANMHDR* pTAHDR = (TANMHDR*)pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	if(pInfo->m_iSelMontion < 0 || pInfo->m_iSelMontion >= (int) pInfo->m_motions.size())
		return;
	int iMotion = m_wndList.GetSelectionMark();
	CModelMotionInfo::MOTION* pMotion = pInfo->m_motions[iMotion];

	int iNewPlace;
	if((pTAHDR->selLayers == NULL) && (pTAHDR->maxLayer ==0))
	{//single relate
		int iFromLayer = pTAHDR->layer;
		int iParentLayer = (int)pTAHDR->otherParam;

		iFromLayer = ComputeRelativeLayer(iFromLayer);
		iParentLayer = ComputeRelativeLayer(iParentLayer);

		iNewPlace = pInfo->SetParentRelation(pInfo->m_iSelMontion, iFromLayer, iParentLayer);
	}
	else
	{//multi relate
		int num = pTAHDR->maxLayer;
		bool *selLayers = pTAHDR->selLayers;
		int iParentLayer = (int)pTAHDR->otherParam;

		std::vector< std::pair<int,int> > nVector;
		//对选中的层进行偏移处理
		for(int i=0; i<num && i<pMotion->objs.size(); i++)
		{
			if(selLayers[i])
			{
				int nExcToLayer = ComputeRelativeLayer(i);
				nVector.push_back(std::pair<int,int>(i, nExcToLayer));
			}
		}
		for(int j=0; j<nVector.size(); j++)
			selLayers[nVector[j].first] = false;
		for(int j=0; j<nVector.size(); j++)
			selLayers[nVector[j].second] = true;

		iParentLayer = ComputeRelativeLayer(iParentLayer);

		iNewPlace = pInfo->SetParentRelation(pInfo->m_iSelMontion, selLayers, num, iParentLayer);
	}

	if(iNewPlace >= 0)
	{
		UpdateTimeBarList();
		m_wndTimeAixes.SetSelectedLayer(iNewPlace);
		m_wndTimeAixes.ShiftRedBox(-1);
		pInfo->SetSelect(pInfo->m_iSelMontion, iNewPlace);
	}
}

void CMontionEditPane::OnMotionListItemChange(NMHDR* pHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnm = (NMLISTVIEW*) pHDR;
	if(((pnm->uOldState & LVIS_SELECTED) == 0 ) && ((pnm->uNewState & LVIS_SELECTED) != 0 ))
	{
		CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
		int iMotion = pnm->iItem;
		if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		{
			m_wndTimeAixes.ClearLayers();
			pInfo->SetSelect(-1, -1);
			return;
		}
		pInfo->SetSelect(iMotion, -1);
		UpdateTimeBarList(pInfo->m_motions[iMotion]);
	}
	*pResult = 0;
}

void CMontionEditPane::OnMotionListItemActive(NMHDR* pHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnm = (NMLISTVIEW*) pHDR;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	CMotionPropertyDlg dlg;
	dlg.m_fTime = pInfo->m_motions[iMotion]->fTime;
	dlg.m_strMotionName = pInfo->m_motions[iMotion]->name.c_str();
	dlg.m_loop = pInfo->m_motions[iMotion]->loop;
	dlg.m_bPublic = pInfo->m_motions[iMotion]->bIsPublic == true ? TRUE : FALSE;

	//if(pInfo->m_motions[iMotion]->iMainSeq < 0)
	//dlg.m_strAnim = "无";
	//else
	//dlg.m_strAnim.Format("%d", pInfo->m_motions[iMotion]->iMainSeq);

	if(dlg.DoModal() == IDOK)
	{
		CString strOldName = pInfo->m_motions[iMotion]->name.c_str();
		bool bOldPublic = pInfo->m_motions[iMotion]->bIsPublic;
		pInfo->m_motions[iMotion]->fTime = dlg.m_fTime;
		pInfo->m_motions[iMotion]->name = dlg.m_strMotionName.GetBuffer();
		pInfo->m_motions[iMotion]->loop = dlg.m_loop;
		pInfo->m_motions[iMotion]->bIsPublic = dlg.m_bPublic == TRUE ? true : false;
		pInfo->m_motions[iMotion]->docInfo.Modify();
		strOldName.MakeUpper();
		dlg.m_strMotionName.MakeUpper();

		if(strOldName != dlg.m_strMotionName)
		{
			UpdateList();
			PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION_BRO_PRIVATE);
			m_wndList.SetSelectionMark(iMotion);
			//CEffectMgr::GetInstance()->GetModelMotion()->m_iSelMontion = iMotion;
		}
		else
		{
			//m_wndList.SetItemText(iMotion, 0, dlg.m_strMotionName.GetBuffer());
			m_wndList.SetItemText(iMotion, 1, pInfo->m_motions[iMotion]->bIsPublic == true ? "公共特效" : "自身特效" );
			PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION_BRO_PRIVATE);
		}

	}
	*pResult = 0;
}

void CMontionEditPane::SetTimeAixLevel(int level)
{
	if(level >= 0 && level < g_iTimeAixLevelCount)
	{
		m_wndTimeAixes.m_fUnitValue = g_TimeAixLevel[level].m_fUnitValue;
		m_wndTimeAixes.m_iExactInStep = g_TimeAixLevel[level].m_iExactInStep;
		m_wndTimeAixes.m_iStepPixes = g_TimeAixLevel[level].m_iStepPixes;
		m_wndTimeAixes.m_iSeperaterByBox = g_TimeAixLevel[level].m_iSeperate;
		m_wndTimeAixes.m_bShowGrid = g_TimeAixLevel[level].m_bShowGrid;
		m_iCurrAixLevel = level;
	}
}

void CMontionEditPane::Clear()
{
	m_wndList.DeleteAllItems();
	m_wndTimeAixes.ClearLayers();
}

// 更新列表框
void CMontionEditPane::UpdateList()
{
	if(!GetSafeHwnd())
		return;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	pInfo->SetSelect(-1, -1);
	m_wndList.DeleteAllItems();
	m_wndList.m_ItemColors.RemoveAll();

	m_wndTimeAixes.ClearLayers();
	int index = 0;
	for(int i = 0; i < (int)pInfo->m_motions.size(); i++)
	{
		CString str;
		str.Format("%d", i);
		m_wndList.InsertItem(LVIF_TEXT, index, pInfo->m_motions[i]->name.c_str()/* + str*/, 0, 0, 0, 0);
		m_wndList.SetItem(index++, 1, LVIF_TEXT, pInfo->m_motions[i]->bIsPublic ? "公共特效" : "自身特效", 0, 0, 0, 0, 0);
	}

	CSkeleton* pSkeleton = CEffectMgr::GetInstance()->GetSkeleton();
	Ogre::Model *pmodel = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();

	for(int i = 0; i < (int)pSkeleton->m_animOrder.size(); i++)
	{
		Ogre::AnimationData *panimdata = pmodel->getModelData()->getAnimation(pSkeleton->m_animOrder[i].iAnimGroup);
		if(panimdata == NULL) 
			continue;

		int seqid = pSkeleton->m_animOrder[i].iSeqId;
		int seqindex = pSkeleton->m_animOrder[i].iSeqIndex;

		CString strSeqName;
		strSeqName.Format("%d", panimdata->m_Sequences[seqindex].id);
		if(pInfo->GetMotionByName(strSeqName) == NULL)
		{
			m_wndList.InsertItem(LVIF_PARAM | LVIF_TEXT, index,
				strSeqName.GetBuffer(), 0, 0, 0,(LPARAM) &panimdata->m_Sequences[seqindex]);

			m_wndList.SetItem(index, 1, LVIF_TEXT, "骨骼动画", 0, 0, 0, 0, 0);
			m_wndList.SetCellColor(index, 0, RGB(0, 0, 0), RGB(150, 150, 150));
			m_wndList.SetCellColor(index, 1, RGB(0, 0, 0), RGB(150, 150, 150));
			index++;
		}
	}
}

void CMontionEditPane::UpdateTimeBarList()
{
	if(!GetSafeHwnd())
		return;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int iMotion = m_wndList.GetSelectionMark();
	if(iMotion < 0 || iMotion>= (int)pInfo->m_motions.size())
		return;

	int selBox = m_wndTimeAixes.GetSelectedBox();
	//int selLayer = m_wndTimeAixes.GetSelectedLayer();
	int selLayer = pInfo->m_iSelObj;

	UpdateTimeBarList(pInfo->m_motions[iMotion]);

	m_wndTimeAixes.SetSelectedLayer(selLayer);
	m_wndTimeAixes.SetSelectedBox(selBox);
}

void CMontionEditPane::SortObjLayer(CModelMotionInfo::MOTION* pMotion)
{
	std::vector<CObjectLayerInfo*> post;
	std::vector<CObjectLayerInfo*> anim;
	std::vector<CObjectLayerInfo*> effect;
	std::vector<CObjectLayerInfo*> eventlayer;

	for(int i=0; i<pMotion->objs.size(); i++)
	{
		if(pMotion->objs[i]->m_type == Ogre::MOT_POSTEFFECT)
		{
			post.push_back(pMotion->objs[i]);
		}
		else if(pMotion->objs[i]->m_type == Ogre::MOT_ANIM || pMotion->objs[i]->m_type == Ogre::MOT_ANIM_ADD)
		{
			anim.push_back(pMotion->objs[i]);
		}
		else if(pMotion->objs[i]->m_type == Ogre::MOT_MODEL || pMotion->objs[i]->m_type == Ogre::MOT_MODEL_2)
		{
			effect.push_back(pMotion->objs[i]);
		}
		else if(pMotion->objs[i]->m_type == Ogre::MOT_EVENTS)
		{
			eventlayer.push_back(pMotion->objs[i]);
		}
	}
	pMotion->objs.clear();
	pMotion->objs.insert(pMotion->objs.end(), post.begin(), post.end());
	pMotion->objs.insert(pMotion->objs.end(), anim.begin(), anim.end());
	pMotion->objs.insert(pMotion->objs.end(), eventlayer.begin(), eventlayer.end());
	pMotion->objs.insert(pMotion->objs.end(), effect.begin(), effect.end());
}

bool CMontionEditPane::WhetherShowByVN(CObjectLayerInfo* pObjLayer)
{
	CObjectLayerInfo* pParent = pObjLayer->getParent();
	while(pParent)
	{
		if(pParent->m_eEffectType == EFFECT_VIRTUALNODE && pParent->m_bFolded)
		{
			return false;
		}
		pParent = pParent->getParent();
	}
	return true;
}

/*将当前CTimeAxies中的CTimeLayer对应pMotion中所包含的信息*/
void CMontionEditPane::UpdateTimeBarList(CModelMotionInfo::MOTION* pMotion)
{
	SortObjLayer(pMotion);

	m_wndTimeAixes.ClearLayers();

	int nLod = 0;
	switch(m_eShowLodState)
	{
	case HIGH_STATE:
		{
			nLod = 300;
			break;
		}
	case MIDDLE_STATE:
		{
			nLod = 200;
			break;
		}
	case LOW_STATE:
		{
			nLod = 100;
			break;
		}
	default:
		MessageBox("CMontionEditPane::UpdateTimeBarList Error !");
		break;
	}

	m_iMontionSetLod = CEffectMgr::GetInstance()->GetModelMotion()->m_iSelMontion;
	UpdateMontionLod(nLod);

	for(int i = 0; i < (int) pMotion->objs.size(); i++)
	{
		CObjectLayerInfo& objinfo = *(pMotion->objs[i]);
		CString strName = objinfo.GetDisplayName();
		int nLod2 = objinfo.m_iObjLayerLodLevel;
		if(nLod >= objinfo.m_iObjLayerLodLevel && WhetherShowByVN(&objinfo))
		{
			CTimeLayer tl;
			objinfo.CreateTimeLayer(&tl, &m_wndTimeAixes);
			m_wndTimeAixes.AddLayer(tl);

			if(objinfo.m_eEffectType == EFFECT_VIRTUALNODE && objinfo.m_bFolded)
			{
				m_wndTimeAixes.SetLayerOption(m_wndTimeAixes.m_layers.size()-1, 1, 1);
			}
			else if((objinfo.m_eEffectType == EFFECT_VIRTUALNODE) && (objinfo.m_bFolded == false))
			{
				m_wndTimeAixes.SetLayerOption(m_wndTimeAixes.m_layers.size()-1, 1, 0);
			}
		}
	}
	m_wndTimeAixes.SetEndlineByValue(pMotion->fTime);
}                                              

void CMontionEditPane::DoFrame(float dtime)
{
	static float lasttime = 0.0f;
	static float acctime = 0.0f;
	static bool lastIsPlaying = false;
	Ogre::Entity* pModel = CEffectMgr::GetInstance()->GetModelActor();
	acctime += dtime;
	if(acctime >= 0.03)
	{
		if(pModel->getNumMotion() > 0)
		{
			if(pModel->getIthMotion(0)->IsPlaying())
			{
				m_wndTimeAixes.SetPlayLine(pModel->getIthMotion(0)->m_fEscapeTime);
				m_wndTimeAixes.Invalidate(FALSE);
				lastIsPlaying = true;
			}
			else
			{
				m_wndTimeAixes.SetPlayLine(-1.0f);
				if(lastIsPlaying == true)
				{
					//CEffectMgr::GetInstance()->StopAnim(0);
					m_wndTimeAixes.Invalidate(FALSE);
				}
				lastIsPlaying = false;
			}
		}
		else
		{
			m_wndTimeAixes.SetPlayLine(-1.0f);
			if(lastIsPlaying == true)
			{
				//CEffectMgr::GetInstance()->StopAnim(0);
				m_wndTimeAixes.Invalidate(FALSE);
			}
			lastIsPlaying = false;
		}
		acctime = 0.0f;
	}

	lasttime += dtime;
}

void CMontionEditPane::SetSelect(CString strMotionName, bool bIsPublic, int layer)
{
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->m_modelMotion;
	strMotionName.MakeUpper();
	int iTargetMotion = -1;
	int i;
	for(i = 0; i < (int)pInfo->m_motions.size(); i++)
	{
		CString strExistName = pInfo->m_motions[i]->name.c_str();
		strExistName.MakeUpper();

		if(strMotionName == strExistName && bIsPublic == pInfo->m_motions[i]->bIsPublic)
		{
			iTargetMotion = i;
			break;
		}
	}

	if(iTargetMotion != -1)
	{
		for(int k = 0; k < m_wndList.GetItemCount(); k++)
		{
			m_wndList.SetItemState(k, 0, LVIS_SELECTED);
		}
		m_wndList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_wndList.SetSelectionMark(i);
		m_wndList.EnsureVisible(i, FALSE);
	}
	else
	{
		iTargetMotion = pInfo->m_iSelMontion;
	}

	pInfo->SetSelect(iTargetMotion, layer);
	UpdateTimeBarList();

}

void CMontionEditPane::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	//TRACE("Set Focus\n");
	// TODO: Add your message handler code here
}

BOOL CMontionEditPane::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && ((GetAsyncKeyState(VK_CONTROL)&0xff00)!=0))
	{
		int iRedBox = m_wndTimeAixes.m_iSelectedBox;
		if(pMsg->wParam == VK_LEFT)
		{
			if(iRedBox >= 0)
			{
				m_wndTimeAixes.ShiftRedBox(iRedBox - 1);
			}
		}      
		else if(pMsg->wParam == VK_RIGHT)
		{
			m_wndTimeAixes.ShiftRedBox(iRedBox + 1);
		}	
	}
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ADD || pMsg->wParam == VK_OEM_PLUS))
	{
		OnRulerIn();
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_SUBTRACT || pMsg->wParam == VK_OEM_MINUS))
	{
		OnRulerOut();
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CMontionEditPane::OnSetMotionLodLevel(NMHDR* pHDR, LRESULT* pResult)
{

	/*
	LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pHDR;
	CPoint point(pia->ptAction); //point 为 Client 坐标

	UINT uFlags;
	int nItem = m_wndList.HitTest(point, &uFlags);
	ClientToScreen(&point);

	if (uFlags & LVHT_ONITEMLABEL)
	{
	CBCGPPopupMenu* lodMenu = new CBCGPPopupMenu();
	lodMenu->Create(this, point.x+2, point.y+26, ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU4)),0), FALSE, TRUE);

	int id = -1000;
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	assert(pInfo->m_iSelMontion >= 0 && pInfo->m_iSelMontion < pInfo->m_motions.size());
	CModelMotionInfo::MOTION* pstMotion = pInfo->m_motions[pInfo->m_iSelMontion];
	int nLodLevel = pstMotion->stMotionLodLevel;
	switch (nLodLevel)
	{
	case 0:		id = ID_SET_LOWLOD;		break;
	case 100:	id = ID_SET_LOWLOD;		break;
	case 200:	id = ID_SET_MIDDLELOD;	break;
	case 300:	id = ID_SET_HIGHLOD;	break;
	default:	MessageBox("CMontionEditPane::OnSetMotionLodLevel LOD Wrong !");	break;
	}
	lodMenu->SetDefaultItem(id);
	lodMenu->RemoveItem(3); //取消掉"不处理"这项
	lodMenu->RemoveItem(3);
	lodMenu->RemoveItem(3);
	lodMenu->UpdateShadow();
	lodMenu->SetAutoDestroy(true);
	}
	*/


	*pResult = 0;
}

void CMontionEditPane::UpdateMontionLod(int level)
{
	assert(m_iMontionSetLod != -1);
	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	//	pInfo->SetSelect(m_iMontionSetLod, -1);
	pInfo->m_motions[m_iMontionSetLod]->stMotionLodLevel = level;

	/*实时更新显示内容*/
	for(int i = 0; i < (int)pInfo->m_motions[m_iMontionSetLod]->objs.size(); i++)
	{
		pInfo->m_motions[m_iMontionSetLod]->objs[i]->RepresentAtTime(pInfo->m_fHotTime, level);
	}
}

void CMontionEditPane::OnSetHighLod()
{
	UpdateMontionLod((int)Ogre::MOTION_LOD_HIGH);
	m_wndTimeAixes.m_eSelLodState = HIGH_STATE;
}

void CMontionEditPane::OnSetMiddleLod()
{
	UpdateMontionLod((int)Ogre::MOTION_LOD_MIDDLE);
	m_wndTimeAixes.m_eSelLodState = MIDDLE_STATE;
}

void CMontionEditPane::OnSetLowLod()
{
	UpdateMontionLod((int)Ogre::MOTION_LOD_LOW);
	m_wndTimeAixes.m_eSelLodState = LOW_STATE;
}

void CMontionEditPane::OnExitLodPopMenu()
{}

void CMontionEditPane::OnPasteLayers(NMHDR* pHDR, LRESULT* pResult)
{
	CModelMotionInfo* pModelMotion = CEffectMgr::GetInstance()->GetModelMotion();
	int nSelLayerToPaste = pModelMotion->m_iSelObj;
	nSelLayerToPaste = ComputeRelativeLayer(nSelLayerToPaste);
	pModelMotion->PasteLayers();
	pModelMotion->ArrangeOrder(pModelMotion->m_motions[pModelMotion->m_iSelMontion]);
	UpdateTimeBarList(pModelMotion->m_motions[pModelMotion->m_iSelMontion]);
}

void CMontionEditPane::ShareCodes()
{
	CModelMotionInfo* pModelMotion = CEffectMgr::GetInstance()->GetModelMotion();
	//对选中的层进行偏移处理
	std::vector<int> vRelativeTo;
	for(int i=0; i<(int)gLayersOpObj.vLayers.size(); i++)
	{
		int nTo = ComputeRelativeLayer(gLayersOpObj.vLayers[i]);
		vRelativeTo.push_back(nTo);
	}
	gLayersOpObj.vLayers.clear();
	gLayersOpObj.vLayers = vRelativeTo;
}
void CMontionEditPane::OnCopyLayers(NMHDR* pHDR, LRESULT* pResult)
{
	ShareCodes();
}

void CMontionEditPane::OnCutLayers(NMHDR* pHDR, LRESULT* pResult)
{
	ShareCodes();
}

BOOL CMontionEditPane::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndTimeAixes.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) == TRUE)
		return TRUE;

	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMontionEditPane::OnNewEffect()
{
	// TODO: Add your command handler code here
}

void CMontionEditPane::OnMenuNewpostCamerashake()
{
	// TODO: Add your command handler code here
	OnNewObjLayer(ID_BTN_MO_NEW_POST);
}
