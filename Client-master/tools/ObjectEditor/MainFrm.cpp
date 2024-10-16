// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "MainFrm.h"

#include "EditorManager.h"
#include "EffectManager.h"
#include "AvatarShowInfo.h"
#include "EffectUnitEditBar.h"
#include "BindInfo.h"
#include "DigitHelpManager.h"
#include "DBManager.h"
#include "ObjectTypeHelper.h"
#include "ControlsHelper.h"
#include "CameraCtrl.h"
#include "EffectCommand.h"
#include "Coord.h"
#include "InteractObject.h"
#include "RefNodeManager.h"

#include "OgreEntity.h"
#include "OgreResourceManager.h"
#include "OgreGameScene.h"
#include "OgreGameTerrainScene.h"
#include "OgreLines.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()

	ON_WM_DESTROY()
	ON_COMMAND(ID_NEW_FILE, OnNewFile)
	ON_COMMAND(ID_NEW_EFFECT, OnNewEffect)

	ON_MESSAGE(EEM_DELETE_EFFECT, OnEffectDelete)
	ON_MESSAGE(EEM_APPLY, OnApply)
	ON_MESSAGE(EEM_SET_BGCOLOR, OnSetBgcolor)
	ON_MESSAGE(EEM_SAVE, OnSave)
	ON_MESSAGE(EEM_SET_SPEED, OnSetSpeed)
	ON_MESSAGE(EEM_EFFECT_LIST_SELECTED, OnEffectListSelected)
	ON_MESSAGE(EEM_UPDATA_LIST, OnUpdateList)
	ON_MESSAGE(EEM_GOTO_PAGE, OnGotoPage)
	ON_MESSAGE(EEM_NEW_EFFECT, OnAddNewEffect)
	ON_MESSAGE(EEM_ACTOR_LOADED, OnActorLoaded)
	ON_MESSAGE(EEM_DROPRES,	OnDropRes)
	ON_MESSAGE(EEM_REQ_LOAD_ACTOR, OnReqestLoadActor)
	ON_MESSAGE(EEM_REQ_LOAD_AVATAR, OnReqestLoadAvatar)
	ON_MESSAGE(EEM_REQ_LOAD_EFFECT, OnReqestLoadEffect)
	ON_MESSAGE(EEM_CLIENT_CREATED, OnClientCreated)
	ON_MESSAGE(EEM_UPDATE_ANCHOR_ITEM, OnUpdateAnchorItem)
	ON_MESSAGE(EEM_EDITITEM, OnEditItem)
	ON_MESSAGE(EEM_FINISH_EDITITEM, OnFinishEditItem)
	ON_MESSAGE(EEM_UPDATE_SKILLBAR, OnUpdateSkillBar)
	ON_MESSAGE(EEM_AVATAR_UPDATED, OnAvatarUpdated)
	ON_MESSAGE(EEM_REQUEST_KFWND, OnRequestKfWnd)
	ON_MESSAGE(EEM_RELEASE_KFWND, OnReleaseKfWnd)
	ON_MESSAGE(EEM_SELECT_MOTION, OnSelectMotion)
	ON_MESSAGE(EEM_GET_CURR_MOTION_DIR, OnGetCurrMotion)

	ON_MESSAGE(EEN_ADD_EFXFILE_TO_MODELMOTION, OnAddEfxToMotion)

	ON_COMMAND(ID_LOAD_EFFECT, OnLoadEffect)
	ON_COMMAND(ID_SAVE_SCENE, OnSaveAsNPC)
	ON_COMMAND(ID_MNU_EDITVIEW, OnMnuEditview)
	ON_UPDATE_COMMAND_UI(ID_MNU_EDITVIEW, OnUpdateMnuEditview)
	ON_COMMAND(ID_MNU_MODELVIEW, OnMnuModelview)
	ON_UPDATE_COMMAND_UI(ID_MNU_MODELVIEW, OnUpdateMnuModelview)

	ON_COMMAND(ID_MNU_RESVIEW, OnMnuResview)
	ON_UPDATE_COMMAND_UI(ID_MNU_RESVIEW, OnUpdateMnuResview)

	ON_COMMAND(ID_BTN_AABB, OnBtnAABB)
	ON_UPDATE_COMMAND_UI(ID_BTN_AABB, OnUpdateBtnAABB)


	ON_COMMAND(ID_MNU_AVAVIEW, OnMnuAvaview)
	ON_UPDATE_COMMAND_UI(ID_MNU_AVAVIEW, OnUpdateMnuAvaview)
	ON_COMMAND(ID_MNU_ITEMVIEW, OnMnuItemview)
	ON_UPDATE_COMMAND_UI(ID_MNU_ITEMVIEW, OnUpdateMnuItemview)

	ON_COMMAND(ID_BTN_SHOWBOUNDING, OnShowBouding)
	ON_UPDATE_COMMAND_UI(ID_BTN_SHOWBOUNDING, OnUpdateShowBouding)

	ON_COMMAND(ID_BTN_DISTORT, OnSetDistort)
	ON_UPDATE_COMMAND_UI(ID_BTN_DISTORT, OnUpdateSetDistort)

	ON_COMMAND(ID_BTN_RESETVIEW, OnBtnResetView)
	ON_COMMAND(ID_LST_CHANGESPD, OnLstChangeSpeed)
	ON_COMMAND(ID_LST_CHANGECLR, OnLstChangeColor)
	ON_COMMAND(ID_BTN_SETBACKLIGHT, OnBtnSetBackLight)
	ON_UPDATE_COMMAND_UI(ID_MNU_RESVIEW, OnUpdateMnuResview)

	ON_COMMAND_RANGE(ID_MNU_SPD1, ID_MNU_SPD9, OnMnuSpeed)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MNU_SPD1, ID_MNU_SPD9, OnUpdateMnuSpeed)

	ON_COMMAND_RANGE(ID_MNU_UNLIMITEDFRAME, ID_MNU_LOWFRAME, OnMnuFrameLimite)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MNU_UNLIMITEDFRAME, ID_MNU_LOWFRAME, OnUpdateMnuFrameLimite)

	ON_COMMAND(ID_LST_FRAMELIMIT, OnFramelimite)

	ON_WM_CLOSE()

	ON_COMMAND(ID_BTN_UNDO, OnUndo)
	ON_UPDATE_COMMAND_UI(ID_BTN_UNDO, OnUpdateUndo)
	ON_COMMAND(ID_BTN_REDO, OnRedo)
	ON_UPDATE_COMMAND_UI(ID_BTN_REDO, OnUpdateRedo)

	ON_COMMAND(ID_ACC_REDO, OnAccRedo)
	ON_COMMAND(ID_ACC_UNDO, OnAccUndo)

	ON_COMMAND(ID_BTN_LOADBGMESH, OnLoadBGMesh)

	ON_COMMAND(ID_MNU_SAVE_SM, OnMnuSaveSm)
	ON_COMMAND(ID_MNU_OPEN_SM, OnMnuOpenSm)

	ON_COMMAND(ID_BTN_SHOWGROUND, OnBtnShowGrount)
	ON_UPDATE_COMMAND_UI(ID_BTN_SHOWGROUND, OnUpdateBtnShowGround)

	ON_COMMAND(ID_BTN_COODPOS, OnBtnCoodPos)
	ON_UPDATE_COMMAND_UI(ID_BTN_COODPOS, OnUpdateBtnCoodPos)

	ON_COMMAND(ID_BTN_COODROT, OnBtnCoodRot)
	ON_UPDATE_COMMAND_UI(ID_BTN_COODROT, OnUpdateBtnCoodRot)

	ON_COMMAND(ID_BTN_COODSCALE, OnBtnCoodScale)
	ON_UPDATE_COMMAND_UI(ID_BTN_COODSCALE, OnUpdateBtnCoodScale)

	ON_COMMAND(ID_BTN_GLOW, OnBtnGlow)
	ON_UPDATE_COMMAND_UI(ID_BTN_GLOW, OnUpdateBtnGlow)	

	ON_COMMAND(ID_BTN_CHANGEREF, OnBtnChangeRef)

	ON_WM_PAINT()

	ON_COMMAND(ID_BTN_INC_COORD, OnBtnIncCoord)
	ON_COMMAND(ID_BTN_DEC_COORD, OnBtnDecCoord)
	ON_COMMAND(ID_MNU_SRTPROP, OnMnuSrtprop)
	ON_UPDATE_COMMAND_UI(ID_MNU_SRTPROP, OnUpdateMnuSrtprop)
	ON_COMMAND(ID_BACKGROUND_OTHER, OnBackgroundOther)
	ON_COMMAND(ID_BACKGROUND_NONE, OnBackgroundNone)
	ON_COMMAND(ID_BTN_PROJ, OnBtnProj)
	ON_UPDATE_COMMAND_UI(ID_BTN_PROJ, OnUpdateBtnProj)
	ON_COMMAND(ID_BTN_TOPVIEW, OnBtnTopview)
	ON_COMMAND(ID_BTN_ORTHO, OnBtnOrtho)
	ON_UPDATE_COMMAND_UI(ID_BTN_ORTHO, OnUpdateBtnOrtho)
	ON_COMMAND(ID_BTN_LEFTVIEW, OnBtnLeftview)
	ON_COMMAND(ID_BTN_FONTVIEW, OnBtnFontview)
	ON_COMMAND(ID_BTN_FORTYFIVEVIEW, OnFortyFiveview)

	ON_COMMAND(ID_BTN_Edit_MODEL, OnActorEditMode)
	ON_UPDATE_COMMAND_UI(ID_BTN_Edit_MODEL, OnUpdateActorEditMode)

	ON_COMMAND(ID_BTN_HIDEACTOR, OnHideMainActor)
	ON_UPDATE_COMMAND_UI(ID_BTN_HIDEACTOR, OnUpdateHideMainActor)

	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_BACKGROUND_STREET, &CMainFrame::OnBackgroundStreet)
	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_pBKModel = NULL;
	m_StrTitle = "模型文件(*.omod)|*.omod||";
	m_OldPath = "e:\\myworld\\bin";
}

CMainFrame::~CMainFrame()
{
	
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ShowWindow(SW_SHOWMAXIMIZED);
	CEnginAgent::CreateInstance();

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this) ||	!m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// 主框架工具条
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	// 创建工具条速度按钮
	CBCGPToolbarMenuButton	mnuSpeed(ID_LST_CHANGESPD, ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_SPEED)), 0), 0);
	mnuSpeed.SetMenuOnly(TRUE);
	mnuSpeed.SetImage(m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_LST_CHANGESPD))->GetImage());


	// 创建工具条限制帧速按钮
	CBCGPToolbarMenuButton	mnuFrameLimite(ID_LST_FRAMELIMIT, ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_FRAMELIMIT)), 0), 0);
	mnuFrameLimite.SetMenuOnly(TRUE);
	mnuFrameLimite.SetImage(m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_LST_FRAMELIMIT))->GetImage());

	//背景颜色按钮
	CBCGPColorMenuButton	clrColor(ID_LST_CHANGECLR, "更多颜色");
	clrColor.EnableOtherButton("更多颜色");

	//环境光按钮
	CBCGPColorMenuButton	clrLight(ID_BTN_SETBACKLIGHT, "更多颜色");
	clrLight.EnableOtherButton("更多颜色");

	//背景模型按钮
	CBCGPToolbarMenuButton mnuBackground(ID_BTN_LOADBGMESH, ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_BACKGROUND)), 0), 0);
	mnuBackground.SetMenuOnly(TRUE);
	mnuBackground.SetImage(m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_LOADBGMESH))->GetImage());

	m_wndToolBar.ReplaceButton(ID_LST_CHANGESPD, mnuSpeed);
	m_wndToolBar.ReplaceButton(ID_LST_CHANGECLR, clrColor);
	m_wndToolBar.ReplaceButton(ID_LST_FRAMELIMIT, mnuFrameLimite);
	m_wndToolBar.ReplaceButton(ID_BTN_SETBACKLIGHT, clrLight);
	m_wndToolBar.ReplaceButton(ID_BTN_LOADBGMESH, mnuBackground);

	if(!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建MenuBar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	if(!m_wndBrowser1.Create(NULL, this, CRect (0, 0, 250,600),
		TRUE, ID_WND_BROWSER1,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		return  -1;
	}
	m_wndBrowser1.SetWindowText(_T("资源浏览器1"));

	if(!m_EffectListBar.Create(NULL, this, CRect (0, 0, 280,800), TRUE, ID_WND_EDIT, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		return -1;
	}
	m_EffectListBar.SetWindowText("特效编辑");

	if(!m_wndAvatar.Create(NULL, this, CRect (0, 0, 250,600),
		TRUE, ID_WND_AVATAR,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		return -1;
	}
	m_wndAvatar.SetWindowText(_T("Avatar 编辑"));

	if(!m_EntityEditDlg.Create(NULL, this, CRect (0, 0, 600,250),TRUE, ID_WND_MODE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		return -1;
	}
	m_EntityEditDlg.SetWindowText("角色编辑");

	m_wndItemViewer.Create(NULL, this, CRect (0, 0, 250,600),
		TRUE, ID_WND_ITEMVIEW,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI);
	m_wndItemViewer.SetWindowText(_T("物品窗口"));

	if(!m_wndKeyFrame.Create(NULL, this, CRect (0, 0, 400,100),
		TRUE, 999,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM| CBRS_FLOAT_MULTI))
	{
		return -1;
	}
	m_wndKeyFrame.SetWindowText("特效元素关键帧");
	m_wndKeyFrame.EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndSrtBar.Create(NULL, this, CRect (0, 0, 280,250),
		TRUE, ID_WND_SRTBAR,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		return -1;
	}
	m_wndSrtBar.SetWindowText("关键帧常规参数");
	m_wndSrtBar.EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndBoundBar.Create(NULL, this, CRect (600, 600, 800,800),
		TRUE, ID_WND_BOUND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM| CBRS_FLOAT_MULTI))
	{
		return -1;
	}
	m_wndBoundBar.SetWindowText("包围盒");
	m_wndBoundBar.EnableDocking(CBRS_ALIGN_ANY);
	/*
	if(!m_wndMotionBrowser.Create(NULL, this, CRect (0, 0, 250,600),TRUE, ID_WND_MOTION_BRO,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		return -1;
	}
	m_wndMotionBrowser.SetWindowText("特效动作信息");
	*/

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_EntityEditDlg.EnableDocking(CBRS_ALIGN_ANY);
	m_EffectListBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndBrowser1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndAvatar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndItemViewer.EnableDocking(CBRS_ALIGN_ANY);
	m_wndBoundBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndMotionBrowser.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_EffectListBar);
	DockControlBar(&m_wndBrowser1);
	DockControlBar(&m_EntityEditDlg);

	DockControlBar(&m_wndKeyFrame);
	DockControlBar(&m_wndAvatar);

	//DockControlBar(&m_wndMotionBrowser);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndMenuBar);

	//绑定特效信息和物品窗口到资源浏览器
	//m_wndMotionBrowser.AttachToTabWnd(&m_wndBrowser1, BCGP_DM_STANDARD, FALSE, NULL);
	m_wndItemViewer.AttachToTabWnd(&m_wndBrowser1, BCGP_DM_STANDARD, FALSE, NULL);

	//m_wndBrowser1.AttachToTabWnd(&m_wndMotionBrowser, BCGP_DM_STANDARD, FALSE, NULL);
	//m_wndItemViewer.AttachToTabWnd(&m_wndMotionBrowser, BCGP_DM_STANDARD, FALSE, NULL);
	m_wndSrtBar.DockToWindow(&m_EffectListBar, CBRS_ALIGN_BOTTOM);

	ShowControlBar(&m_wndKeyFrame, FALSE, FALSE, FALSE);
	ShowControlBar(&m_wndAvatar, FALSE, FALSE, FALSE);
	ShowControlBar(&m_wndBoundBar, FALSE, FALSE, FALSE);

	m_wndView.OnInit();	// 初始化引擎 特效管理 数据
	PostMainFrameMsg(EEM_CLIENT_CREATED);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序
LRESULT CMainFrame::OnClientCreated(WPARAM wParam, LPARAM lParam)
{

	m_wndAvatar.CreateConfig();
	m_wndItemViewer.ReloadItems();
	//m_wndMotionBrowser.OnMainClientCreated();
	//m_wndMotionBrowser.UpdateTree();

//	m_EntityEditDlg.m_wndSkillTemplate.UpdateSkillTemplateName();
//	m_EntityEditDlg.m_wndSkillTemplate.UpdateSkillTree();

	return 0;
}

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

void CMainFrame::DoFrame()
{
	static long accumtick = 0;
	static long lasttick = 0;

	long curtick = timeGetTime();
	long dtick = curtick - lasttick;

	if (m_pBKModel)
	{
		m_pBKModel->update(dtick);
	}
	//if(dtick > 30) dtick = 30;
	accumtick += dtick;
	lasttick = curtick;

	static long count = 0;
	count++;

	CEnginAgent* pEA = CEnginAgent::GetInstance();
	if(pEA->m_bIsInitialized)
	{
		pEA->m_pLineRenderer->reset();

		CEffectMgr::GetInstance()->UpdateEffectPos(dtick);
		ResourceManager::getSingleton().update(dtick);
		m_EntityEditDlg.m_wndMotion.DoFrame( pEA->GetSpeed() * dtick / 1000.0f);

		pEA->Update(dtick);
		CEnginAgent::GetInstance()->RenderBone((Ogre::Model*)CEffectMgr::GetInstance()->m_pModelActor->getMainModel());
		pEA->Draw(dtick);
	}

	if(accumtick > 1000)
	{
		float fps = 1000.0f * count / (float)accumtick;
		accumtick = 0;
		count = 0;
		CString text;

		text.Format("Frame Rate %.1f -%d", fps, dtick);
		m_wndStatusBar.SetPaneText(0, text.GetBuffer());

		UpdateStateText();
	}

// 	Ogre::ResourceManager::getSingleton().garbageCollect();
}

void CMainFrame::UpdateStateText()
{
	CString strShowModel, strMainNode, strAvatar;

	if(CEffectMgr::GetInstance())
	{
		strShowModel = CEffectMgr::GetInstance()->m_strSmName;
		strMainNode = CEffectMgr::GetInstance()->m_strModName;
		strAvatar = CEffectMgr::GetInstance()->m_avatarShowInfo->IsEnable() == true ? "启用" : "未启用";
	}
	if(strShowModel.GetLength() == 0) strShowModel = "新Entity";
	if(strMainNode.GetLength() == 0) strMainNode = "空对象";

	CString str;
	str.Format("Show Model: %s;      主对象: %s;      Avatar: %s;", strShowModel.GetBuffer(), strMainNode.GetBuffer(), strAvatar.GetBuffer());
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	return CBCGPFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(m_EntityEditDlg.OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
		return TRUE;
	if(m_EffectListBar.OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
		return TRUE;
	if(m_wndBrowser1.OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
		return TRUE;
	if(m_wndAvatar.OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
		return TRUE;
	if(m_wndItemViewer.OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
		return TRUE;
//	if(m_wndItemEdit.OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
//		return TRUE;
	//if(m_wndMotionBrowser.OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	for(int i = 0; i < (int)m_EffectBars.size(); i++)
	{
		if(m_EffectBars[i] != NULL && m_EffectBars[i]->GetSafeHwnd() != NULL && m_EffectBars[i]->OnCmdMsg(nID,  nCode, pExtra, pHandlerInfo))
			return TRUE;
	}
	//CBCGPFrameWnd::GetControlBar(

	return CBCGPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnDestroy()
{

	if(m_pBKModel)
	{
		CEnginAgent::GetInstance()->DetachActor(m_pBKModel);
		m_pBKModel->release();
		m_pBKModel = NULL;
	}
	ClearEffectBars();
	CBCGPFrameWnd::OnDestroy();

	m_EffectListBar.DestroyWindow();
	//m_wndItemEdit.DestroyWindow();

	m_EffectListBar.DestroyWindow();
	m_EntityEditDlg.DestroyWindow();

	m_wndBrowser1.DestroyWindow();

	m_wndAvatar.DestroyWindow();
	m_wndItemViewer.DestroyWindow();
	m_wndSrtBar.DestroyWindow();

	m_wndBoundBar.DestroyWindow();
	//m_wndMotionBrowser.DestroyWindow();

	char* a = (char*)malloc(100);
	long an = 0;
	_CrtIsMemoryBlock(a, 100, &an, NULL, NULL);
	sprintf(a, "{%d}Destroy ", an);

	CEffectMgr::GetInstance()->Release();
	CEffectMgr::ReleaseInstance();
	CDigitHelpMgr::ReleaseInstance();
	CEnginAgent::GetInstance()->DestroyScene();
	CEnginAgent::ReleaseInstance();

	CDataBase::GetInstance()->Destroy();
	CDataBase::ReleaseInstance();


	//m_effectMgr.Destroy();
}

void CMainFrame::OnNewFile()
{
	NewFile();
}

void CMainFrame::OnNewEffect()
{
	//NewEffect();
}

LRESULT CMainFrame::OnReqestLoadActor(WPARAM wParam, LPARAM lParam)
{
	if(lParam == 0)
		return 0;
	CString strName = (char*)lParam;
	LoadScene(strName);
	delete (char*)lParam;
	return 0;
}

LRESULT CMainFrame::OnReqestLoadEffect(WPARAM wParam, LPARAM lParam)
{
	//if(lParam == 0)
	//	return 0;
	NEWEFFECT_INFO* pni = (NEWEFFECT_INFO*)lParam;
	CCvtHelper cvt;
	CEffectUnit* pNewEffect = NULL;
	CString strName = pni->path.c_str();
	CString strRealpath = pni->realpath.c_str();

	if(pni->bIsPath == true)
	{
		pNewEffect = CEffectUnit::LoadEffectFromFile(strRealpath);
	}
	else
	{
		pNewEffect = CEffectUnit::LoadEffectFromChunk(pni->pChunk);
		//pNewEffect->SetName(strName);
		//pNewEffect->SetResPath(strRealpath);
	}
	pNewEffect->SetName(strName);
	pNewEffect->SetResPath(strRealpath);

	if(pNewEffect == NULL)
		return 0;

	pNewEffect->GetBindInfo()->boneid = pni->anchor;
	pNewEffect->GetBindInfo()->offset = pni->offset;
	pNewEffect->GetBindInfo()->rotate = pni->rotate;
	pNewEffect->GetBindInfo()->scale = pni->scale;
	pNewEffect->GetBindInfo()->bDynamic = pni->bDynamic;
	pNewEffect->GetBindInfo()->animname = pni->animname.c_str();
	pNewEffect->GetBindInfo()->animmode = pni->animmode;
	pNewEffect->SetVisible(pni->bVisible);
	pNewEffect->SetOwnerEffect(pni->pOwner);

	//忽略掉NEWEFFECT_INFO的bIntegrated参数
	if(pNewEffect->GetType() == EFFECT_UNKNOWN)
		pNewEffect->GetBindInfo()->bIntegrated = false;
	else
		pNewEffect->GetBindInfo()->bIntegrated = true;

	CBCGPDockingControlBar* pBar = pNewEffect->CreateBar(this);

	if(pni->bShowPane)
	{	
		pBar->Create(NULL, this, CRect (0, 0,  250,600),
			TRUE, 0,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);

		//pBar->SetIcon(m_imgIcons.ExtractIcon(1), FALSE);

		pBar->EnableDocking(CBRS_ALIGN_LEFT);
		pBar->AttachToTabWnd(&m_EffectListBar, BCGP_DM_STANDARD, FALSE, NULL);
		ShowControlBar (pBar, TRUE,	FALSE, TRUE);
	}
	static_cast<CEffectEditBar*> (pBar)->SetEditTarget(pNewEffect);
	m_EffectBars.push_back((CEffectEditBar*) pBar);
	//RecalcLayout();
	//}
	CEffectMgr::GetInstance()->AddEffect(pNewEffect);

	//UpdateEffectList();
	//m_EffectListBar.UpdateList(CEffectMgr::GetInstance()->m_Effects);

	return (LRESULT)pNewEffect;
}

LRESULT CMainFrame::OnReqestLoadAvatar(WPARAM wParam, LPARAM lParam)
{
	int* attrib = (int*)lParam;
	ClearEffectBars();

	if(false == CEffectMgr::GetInstance()->LoadModelFromAvatar(attrib))
	{
		MessageBox("打开失败，文件类型错误或者相关资源丢失");
	}

	return 0;
}

LRESULT CMainFrame::OnSave(WPARAM wParam, LPARAM lParam)
{
	CEffectUnit* pEU = (CEffectUnit*)lParam; //CEffectManager::GetInstance()->GetEffect((CEffectUnit*)lParam);

	CString path = GetOpenFilePath(FALSE, "effect", pEU->GetName(), "emo", "Motion Files(*.emo)\0*.emo\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		if(!pEU->SaveEffect(path))
			MessageBox("保存失败，可能是空间不足或者文件只读");
	}
	UpdateEffectList(UE_EFFECT_LIST);
	return  0;
}

LRESULT CMainFrame::OnEffectDelete(WPARAM wParam, LPARAM lParam)
{

	CEffectUnit* pUnit = (CEffectUnit*)lParam;
	std::vector<CEffectEditBar*>::iterator it = m_EffectBars.begin();
	for(; it != m_EffectBars.end(); it++)
	{
		if((*it)->GetEditTarget() == pUnit)
			break;
	}
	if( it == m_EffectBars.end())
		return 0;

	CEffectEditBar* pBar = *it;
	if(pBar->GetSafeHwnd() != NULL)
	{
		if(pBar->IsTabbed())
		{
			pBar->GetParentTabbedBar()->DetachControlBar(pBar, TRUE);
		}

		ShowControlBar(pBar, FALSE, FALSE, FALSE);

#if _BCGCBPRO_VERSION_MAJOR >= 10
		RemoveControlBarFromDockManager(pBar, TRUE, TRUE, FALSE, NULL);
#else
		RemoveControlBarFromDockManager(pBar, TRUE, TRUE, FALSE);
#endif

	}
	//delete pBar; //!!!应该删除这个Bar， 但删除会导致崩溃

	m_EffectBars.erase(it);
	CEffectMgr::GetInstance()->RemoveEffect(pUnit);
	UpdateEffectList(UE_ALL);
	RefreshScene();
	return 0;
}



LRESULT CMainFrame::OnActorLoaded(WPARAM wParam, LPARAM lParam)
{
	RecalcLayout();

	m_EntityEditDlg.m_wndAnim.SetEditTarget(CEffectMgr::GetInstance()->GetSkeleton());
	m_EntityEditDlg.m_wndBone.SetEditTarget(CEffectMgr::GetInstance()->GetSkeleton());
	//m_EntityEditDlg.m_wndAnchor.SetEditTarget(&CEffectMgr::GetInstance()->m_anchorInfo);
	m_EntityEditDlg.m_wndItem.UpdateList();
	m_EntityEditDlg.m_wndItem.UpdateSkins();
	m_EntityEditDlg.m_wndMotion.UpdateList();
//	m_EntityEditDlg.m_wndSkill.UpdatePropList();
//	m_EntityEditDlg.m_wndSkillTemplate.UpdatePropList();
	Ogre::Entity* pModel = CEffectMgr::GetInstance()->m_pModelActor;

	m_wndAvatar.SetEditTarget((CEffectMgr::GetInstance()->m_avatarShowInfo));
	m_wndBoundBar.UpdateList();
	//m_wndMotionBrowser.UpdatePrivateMotion();
	UpdateEffectList(UE_ALL);
	return 0;
}

LRESULT CMainFrame::OnAvatarUpdated(WPARAM wParam, LPARAM lParam)
{
	m_EntityEditDlg.m_wndItem.UpdateSkins();
	return 0;
}

LRESULT CMainFrame::OnApply(WPARAM wParam, LPARAM lParam)
{
	RefreshScene();
	UpdateEffectList(UE_EFFECT_LIST);
	return 0;
}

LRESULT CMainFrame::OnSetBgcolor(WPARAM wParam, LPARAM lParam)
{
	Ogre::ColorQuad* pColor = (Ogre::ColorQuad*)lParam;
	CEnginAgent::GetInstance()->SetBGColor(*pColor);
	return 0;
}

LRESULT CMainFrame::OnSetSpeed(WPARAM wParam, LPARAM lParam)
{
	float fValue = *(float*)lParam;
	CEnginAgent::GetInstance()->SetSpeed(fValue);
	return  0;
}

LRESULT CMainFrame::OnRequestKfWnd(WPARAM wParam, LPARAM lParam)
{
	ShowControlBar(&m_wndKeyFrame, TRUE, FALSE, FALSE);
	m_wndKeyFrame.SetKeyFrameObj((CKeyFrameObject*)lParam);
	return 0;
}

LRESULT CMainFrame::OnReleaseKfWnd(WPARAM wParam, LPARAM lParam)
{
	CKeyFrameObject* pObj = (CKeyFrameObject*)lParam;
	if(m_wndKeyFrame.GetKeyFrameObj() == pObj)
	{
		if(m_wndKeyFrame.GetSafeHwnd())
			ShowControlBar(&m_wndKeyFrame, FALSE, FALSE, FALSE);
		m_wndKeyFrame.SetKeyFrameObj(NULL);
	}
	return 0;
}

LRESULT CMainFrame::OnSelectMotion(WPARAM wParam, LPARAM lParam)
{
	SELECT_MOTION_STRUCT* pSm = (SELECT_MOTION_STRUCT*)lParam;
	m_EntityEditDlg.m_wndMotion.SetSelect(pSm->strName, pSm->bIsPublic, pSm->layer);
	return 0;
}

LRESULT CMainFrame::OnGetCurrMotion(WPARAM wParam, LPARAM lParam)
{
	char* buf = (char*)lParam;
	strcpy(buf, "effect\\public\\");

	//CString strPath = m_wndMotionBrowser.GetSelectedCommonChunkFolder();
	//strncpy(buf, strPath.GetBuffer(), wParam);

	return 0;
}

CEffectEditBar* CMainFrame::GetEffectBarByEffect(CEffectUnit* pEffect)
{
	for(int i = 0; i < (int)m_EffectBars.size(); i++)
	{
		CEffectEditBar* pBar = m_EffectBars[i];
		if(pBar->GetEditTarget() == pEffect)
		{

			if(pBar->GetSafeHwnd() == NULL)
			{
				pBar->Create(NULL, this, CRect (0, 0,  250,600),
					TRUE, 0,
					WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
				pBar->ResetBar();

				if(m_EffectListBar.IsVisible())
					pBar->AttachToTabWnd(&m_EffectListBar, BCGP_DM_STANDARD, FALSE, NULL);
				else
					DockControlBar(pBar);
			}
			pBar->ShowControlBar(TRUE, FALSE, FALSE);
			return pBar;
		}

	}
	return NULL;

}


void CMainFrame::RefreshScene()
{
}

void CMainFrame::NewFile()
{
	CEffectMgr* pEffectMgr = CEffectMgr::GetInstance();
	CEnginAgent* pEa = CEnginAgent::GetInstance();

	//ClearModelPrivateEffectBars();
	//m_EntityEditDlg.m_wndMotion.Clear();

	//pEffectMgr->ClearModelPrivateEffect();
	ClearEffectBars();
	pEffectMgr->m_modelMotion->Clear();
	pEffectMgr->ClearEffect();
	pEffectMgr->CreateDefaultModel();
}

LRESULT CMainFrame::OnAddNewEffect(WPARAM w, LPARAM l)
{

	CEffectUnit* pNewEffect = CEffectUnit::CreateEffect((EFFECT_TYPE)w);
	pNewEffect->Create();

	pNewEffect->GetBindInfo()->boneid = 0;

	pNewEffect->GetBindInfo()->bDynamic = false;
	pNewEffect->GetBindInfo()->animname = "";
	pNewEffect->GetBindInfo()->animmode = Ogre::BAM_SELF;
	pNewEffect->SetVisible(true);
	pNewEffect->SetOwnerEffect(NULL);


	CBCGPDockingControlBar* pBar = pNewEffect->CreateBar(this);

	static_cast<CEffectEditBar*> (pBar)->SetEditTarget(pNewEffect);
	pBar->Create(NULL, this, CRect (0, 0,  250,600),
		TRUE, 0,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	static_cast<CEffectEditBar*> (pBar)->SetEditTarget(pNewEffect);

	//pBar->SetIcon(m_imgIcons.ExtractIcon(1), FALSE);
	pBar->EnableDocking(CBRS_ALIGN_RIGHT);
	pBar->AttachToTabWnd(&m_EffectListBar, BCGP_DM_STANDARD, FALSE, NULL);
	ShowControlBar (pBar, TRUE, FALSE, TRUE); 

	m_EffectBars.push_back((CEffectEditBar*) pBar);

	RecalcLayout();
	CEffectMgr::GetInstance()->AddEffect(pNewEffect);
	UpdateEffectList(UE_ALL);
	RefreshScene();

	return 0;
}

void CMainFrame::SaveScene()
{
}

// 根据资源类型来加载
void CMainFrame::LoadScene(CString strName)
{
	CEffectMgr* pEffectMgr = CEffectMgr::GetInstance();
	CEnginAgent* pEa = CEnginAgent::GetInstance();

	Ogre::Resource* pChu = Ogre::ResourceManager::getSingleton().blockLoad(strName.GetBuffer());
	if(pChu == NULL)
	{
		CString msg;
		msg.Format("无法加载资源: %s", strName.GetBuffer());
		MessageBox(msg);
		return;
	}
	if(IS_KIND_OF(Ogre::EntityData, pChu))
	{
		//ClearEffectBars();
		ClearEffectBars();
		pEffectMgr->ClearEffect();
		pEffectMgr->DestroyModel();


		//m_EffectBars.clear();
		if( false == pEffectMgr->LoadShowModel(strName))
		{
			MessageBox("打开失败，文件类型错误或者相关资源丢失");
		}
	}
	else if(IS_KIND_OF(Ogre::EntityMotionData, pChu))
	{
		CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->m_modelMotion;

		CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();


		pInfo->LoadSingleMotion(static_cast<Ogre::EntityMotionData*>(pChu), pmo, strName);
		pmo->filename = strName;
		pmo->docInfo.Open();

		pmo->bIsPublic = true;

		if(pInfo->AddMotion(pmo) < 0)
		{
			delete pmo;
			assert(0 && "Add motion error");
			return;
		}
		UpdateEffectList(UE_EFFECT_LIST | UE_MOTION);
	}
	else if(IS_KIND_OF(Ogre::ModelData, pChu))
	{
		if( false == pEffectMgr->LoadSkinActor(strName))
		{
			MessageBox("打开失败，文件类型错误或者相关资源丢失");
		}
	}
	else
	{
		MessageBox("不能打开这种文件");
	}

	OGRE_RELEASE(pChu);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	CBCGPFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	CBCGPFrameWnd::OnRButtonDown(nFlags, point);
}

void CMainFrame::OnRButtonUp(UINT nFlags, CPoint point)
{

	CBCGPFrameWnd::OnRButtonUp(nFlags, point);
}

void CMainFrame::OnLoadEffect()
{
}

LRESULT CMainFrame::OnGotoPage(WPARAM wParam, LPARAM lParam)
{
	//int index = (int)wParam;
	//if (index >= 0 && index < (int)m_EffectBars.size()) 
	//{
	//	CEffectEditBar* pBar = m_EffectBars[index];

	//	if(pBar->GetSafeHwnd() == NULL)
	//	{
	//		pBar->Create(NULL, this, CRect (0, 0,  250,600),
	//			TRUE, 0,
	//			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	//		pBar->ResetBar();

	//		if(m_EffectListBar.IsVisible())
	//			pBar->AttachToTabWnd(&m_EffectListBar, BCGP_DM_STANDARD, FALSE, NULL);
	//		else
	//			DockControlBar(pBar);
	//	}
	//	pBar->ShowControlBar(TRUE, FALSE, TRUE);
	//}

	CEffectUnit* pUnit = (CEffectUnit*)lParam;
	if(pUnit != NULL)
	{
		CEffectEditBar* pBar = NULL;
		for(int i = 0; i < (int)m_EffectBars.size(); i++)
		{
			if(m_EffectBars[i]->GetEditTarget() == pUnit)
			{
				pBar = m_EffectBars[i];
				break;
			}
		}
		//m_EffectBars[index];

		if(pBar->GetSafeHwnd() == NULL)
		{
			pBar->Create(NULL, this, CRect (0, 0,  250,600),
				TRUE, 0,
				WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
			pBar->ResetBar();

			if(m_EffectListBar.IsVisible())
				pBar->AttachToTabWnd(&m_EffectListBar, BCGP_DM_STANDARD, FALSE, NULL);
			else
				DockControlBar(pBar);

			//pBar->ShowControlBar(TRUE, TRUE, TRUE);
		}
		pBar->ShowControlBar(TRUE, TRUE, TRUE);

	}

	return 0;
}
void CMainFrame::OnSaveAsNPC()
{
	return;
	/*
	CInputDlg dlg;
	dlg.SetTitle("输出为 Actor");
	dlg.SetDesc("输入保存的名称");
	if(IDOK == dlg.DoModal())
	{
		//CEffectManager::GetInstance()->SaveScene(dlg.m_strText);
	}*/
}

LRESULT CMainFrame::OnEffectListSelected(WPARAM wParam, LPARAM lParam)
{
	bool bVisible = false;
	bool bShowOnly = false;
	int index = (int)wParam;

	if(lParam == UNIT_VIEW_SHOW)
	{
		bVisible = true;
	}
	else if(lParam == UNIT_VIEW_SHOWONLY)
	{
		bVisible = true;
		bShowOnly = true;
	}
	else if(lParam == UNIT_VIEW_HIDE)
	{
		bVisible = false;
		bShowOnly = false;
	}
	//CEffectManager::GetInstance()->SetViewUnit((int)wParam, bVisible, bShowOnly);
	CEffectMgr::GetInstance()->m_Effects[index]->SetVisible(bVisible);
	m_EffectListBar.UpdateList(CEffectMgr::GetInstance()->m_Effects);
	return 0;
}

LRESULT CMainFrame::OnUpdateList(WPARAM wParam, LPARAM lParam)
{
	//m_EffectListBar.UpdateList(CEffectMgr::GetInstance()->m_Effects);
	UpdateEffectList((DWORD)wParam);
	//for(int i = 0; i < (int)m_EffectBars.size(); i++)
	//{
	//	if(m_EffectBars[i] && m_EffectBars[i]->GetSafeHwnd())
	//		m_EffectBars[i]->UpdateList();
	//}
	return 0;
}


void CMainFrame::OnMnuEditview()
{
	ShowControlBar(&m_EffectListBar, !m_EffectListBar.IsVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateMnuEditview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_EffectListBar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnMnuModelview()
{
	ShowControlBar(&m_EntityEditDlg, !m_EntityEditDlg.IsVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateMnuModelview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_EntityEditDlg.IsVisible() ? 1 : 0);
}

void CMainFrame::OnMnuResview()
{
	ShowControlBar(&m_wndBrowser1, !m_wndBrowser1.IsVisible(), FALSE, FALSE);
}
void CMainFrame::OnUpdateMnuResview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndBrowser1.IsVisible() ? 1 : 0);
}

void CMainFrame::OnBtnResetView()	// 重置摄像机
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->Reset();
}

void CMainFrame::OnMnuAvaview()
{
	ShowControlBar(&m_wndAvatar, !m_wndAvatar.IsVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateMnuAvaview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndAvatar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnMnuItemview()
{
	ShowControlBar(&m_wndItemViewer, !m_wndItemViewer.IsVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateMnuItemview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndItemViewer.IsVisible() ? 1 : 0);
}
void CMainFrame::OnLstChangeSpeed()
{
	static float spds[] = { 1.0f/5, 1.0f/4, 1.0f/3, 1.0f/2, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	CBCGPToolbarComboBoxButton* pCmb = 
		(CBCGPToolbarComboBoxButton*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_LST_CHANGESPD));
	int index = pCmb->GetCurSel();
	if(index >= 0)
	{
		CEnginAgent::GetInstance()->SetSpeed(spds[index]);
	}
}

// 背景颜色
void CMainFrame::OnLstChangeColor()
{
	CBCGPColorMenuButton* pBtn = 
		(CBCGPColorMenuButton*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_LST_CHANGECLR));
	COLORREF srcClor = pBtn->GetColor();
	Ogre::ColorQuad mayColor;
	mayColor.r = GetRValue(srcClor);
	mayColor.g = GetGValue(srcClor);
	mayColor.b = GetBValue(srcClor);
	mayColor.a = 255;

	CEnginAgent::GetInstance()->SetBGColor(mayColor);
}

void CMainFrame::OnMnuSpeed(UINT id)
{
	static float spds[] = { 1.0f/5, 1.0f/4, 1.0f/3, 1.0f/2, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	int index = id - ID_MNU_SPD1;
	if(index >= 0)
	{
		CEnginAgent::GetInstance()->SetSpeed(spds[index]);
	}
	CBCGPToolbarMenuButton* pMnuBtn = 
		(CBCGPToolbarMenuButton*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_LST_CHANGESPD));

}
void CMainFrame::OnUpdateMnuSpeed(CCmdUI* pCmd)
{
	static float spds[] = { 1.0f/5, 1.0f/4, 1.0f/3, 1.0f/2, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	float speed = CEnginAgent::GetInstance()->GetSpeed();
	if(pCmd->m_nID - ID_MNU_SPD1 >= 9 || pCmd->m_nID - ID_MNU_SPD1 < 0)
		return;
	for(int i = 0; i < 9; i++)
	{
		if(speed > spds[i] - 0.00001f && speed < spds[i] + 0.00001f 
			&& pCmd->m_nID - ID_MNU_SPD1 == i )
		{
			pCmd->SetRadio(TRUE);
		}

	}
}

LRESULT CMainFrame::OnDropRes(WPARAM w, LPARAM l)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)l;
	m_wndStatusBar.SetPaneText(0, pdn->strPath);

	return  0;
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(AfxMessageBox("你确定要关闭编辑器？", MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	CBCGPFrameWnd::OnClose();
}

void CMainFrame::UpdateEffectList(DWORD mask)
{
	if((mask & UE_EFFECT_LIST) != 0)
	{
		for(int i = 0; i < (int)m_EffectBars.size(); i++)
		{
			CEffectEditBar* pBar = m_EffectBars[i];
			if(pBar->GetSafeHwnd() == NULL)
				continue;
			pBar->UpdateKeyFrameTree();
			pBar->UpdateList();

			CEffectUnit* pUnit = pBar->GetEditTarget();
			if(pUnit != NULL)
			{
				CString str = pUnit->GetName();
				pBar->SetWindowText(pUnit->GetName().GetBuffer());
			}
		}
		m_EffectListBar.UpdateList(CEffectMgr::GetInstance()->m_Effects);
	}//m_EntityEditDlg.m_wndAnchor.UpdateAnchor();

	if((mask & UE_MOTION) != 0)
	{
		m_EntityEditDlg.m_wndMotion.UpdateList();
		m_EntityEditDlg.m_wndMotion.UpdateTimeBarList();
	}

	if((mask & UE_MOTION_BRO_COMMON) != 0)
	{
		//m_wndMotionBrowser.UpdateExistingCommonMotion();
	}

	if ((mask & UE_MOTION_BRO_PRIVATE) != 0)
	{
		//m_wndMotionBrowser.UpdatePrivateMotion();
	}
}

void CMainFrame::OnFramelimite()
{
}

void CMainFrame::OnMnuFrameLimite(UINT id)
{
	if(id == ID_MNU_UNLIMITEDFRAME)
		CEnginAgent::GetInstance()->SetFrameLimit(-1);
	else if(id == ID_MNU_HIGHFRAME)
		CEnginAgent::GetInstance()->SetFrameLimit(15);
	else if(id == ID_MNU_MIDFRAME)
		CEnginAgent::GetInstance()->SetFrameLimit(30);
	else if(id == ID_MNU_LOWFRAME)
		CEnginAgent::GetInstance()->SetFrameLimit(60);

}

void CMainFrame::OnUpdateMnuFrameLimite(CCmdUI* pUI)
{
	CEnginAgent* pEA = CEnginAgent::GetInstance();
	int fl = pEA->GetFrameLimit();
	if(fl > 50 && pUI->m_nID == ID_MNU_LOWFRAME)
		pUI->SetRadio(TRUE);
	else if(fl <= 50 && fl >20 && pUI->m_nID == ID_MNU_MIDFRAME)
		pUI->SetRadio(TRUE);
	else if(fl > 0 && fl <= 20 && pUI->m_nID ==ID_MNU_HIGHFRAME )
		pUI->SetRadio(TRUE);
	else if(fl <= 0 && pUI->m_nID == ID_MNU_UNLIMITEDFRAME)
		pUI->SetRadio(TRUE);
	else
		pUI->SetRadio(FALSE);
}

void CMainFrame::OnBtnSetBackLight()
{
	CBCGPColorMenuButton * pColorBtn = 
		(CBCGPColorMenuButton*)m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_SETBACKLIGHT));
	COLORREF color = pColorBtn->GetColor();
	Ogre::ColourValue clrflt;
	clrflt.r = GetRValue(color) / 255.0f;
	clrflt.g = GetGValue(color) / 255.0f;
	clrflt.b = GetBValue(color) / 255.0f;

	CEnginAgent::GetInstance()->SetAmbientLight(clrflt);
}


LRESULT CMainFrame::OnEditItem(WPARAM wParam, LPARAM lParam)
{
	//m_wndItemEdit.SetEditTarget((unsigned int)lParam);
	//m_wndItemEdit.ShowControlBar(TRUE, TRUE, TRUE);
	return 0;
}

LRESULT CMainFrame::OnFinishEditItem(WPARAM wParam, LPARAM lParam)
{
	m_wndItemViewer.ReloadItems();
	return 0;
}
void CMainFrame::OnMnuOpenSa()
{
	CString path = GetOpenFilePath(TRUE, "", "", "xml", "Show Actor 文件(*.sa)|*.sa|xml描述的 Show Actor 文件|*.xml", FALSE);
	if(path.GetLength() > 0)
	{
		CEffectMgr::GetInstance()->ClearEffect();
		ClearEffectBars();
		CEffectMgr::GetInstance()->LoadModelFromXml(path.GetBuffer());
	}
}

void CMainFrame::OnMnuSaveSa()
{

	std::vector<CEffectUnit*> effects;
	int count = CEffectMgr::GetInstance()->GetNewCreatedEffect(effects);
	if(count != 0)
	{
		for(int i = 0; i < count; i++)
		{
			CString tip;
			tip.Format("%s 未保存，继续操作需要将其保存，继续吗？", effects[i]->GetName().GetBuffer());
			if(IDCANCEL == MessageBox(tip.GetBuffer(), "effect_edit", MB_OKCANCEL))
				return;

			CString path = GetOpenFilePath(FALSE, "", "", "emo", "*.emo", TRUE);
			if(!path.IsEmpty())
			{
				if(!effects[i]->SaveEffect(path))
					MessageBox("失败");
			}
			//UpdateEffectTitle();
			//m_EffectListBar.UpdateList(CEffectMgr::GetInstance()->m_Effects);	
			UpdateEffectList(UE_ALL);
		}
	}

	for(int i = 0; i < (int) CEffectMgr::GetInstance()->m_Effects.size(); i++)
	{
		CEffectMgr::GetInstance()->m_Effects[i]->SaveEffect(CEffectMgr::GetInstance()->m_Effects[i]->GetName());
	}

	CString path = GetOpenFilePath(FALSE, "", "", "xml", "Show Actor 文件(*.sa)|*.sa|xml描述的 Show Actor 文件|*.xml", FALSE);
	if(path.GetLength() > 0)
	{
		CEffectMgr::GetInstance()->SaveShowActor(path);
	}
}

void CMainFrame::ClearEffectBars()
{
	for(int i = 0; i < (int)m_EffectBars.size(); i++)
	{
		CEffectEditBar* pBar = m_EffectBars[i];
		m_EffectBars[i] = NULL;
		if(pBar == NULL)
		{
			continue;
		}
		if(pBar->GetSafeHwnd() != NULL)
		{

			if(pBar->IsTabbed())
			{
				pBar->GetParentTabbedBar()->DetachControlBar(pBar, TRUE);
			}

			ShowControlBar(pBar, FALSE, FALSE, FALSE);
#if _BCGCBPRO_VERSION_MAJOR >= 10
			RemoveControlBarFromDockManager(pBar, TRUE, TRUE, FALSE, NULL);
#else
			RemoveControlBarFromDockManager(pBar, TRUE, TRUE, FALSE);
#endif
			pBar->DestroyWindow();
		}
		delete pBar;
	}
	m_EffectBars.clear();

	CString strInfo;
	strInfo.Format("ClearEffectBars, %d rest\n", m_EffectBars.size());
	TRACE(strInfo.GetBuffer());
}

void CMainFrame::ClearModelPrivateEffectBars()
{
	std::vector<CEffectUnit*> effects;
	CEffectMgr::GetInstance()->GetEffectInPrivateMotion(effects);
	assert(CEffectMgr::GetInstance()->m_Effects.size() == effects.size());
	assert(m_EffectBars.size() == effects.size());

	for(int i = 0; i < (int) effects.size(); i++)
	{
		CEffectUnit* pCurrUnit = effects[i];
		bool bFound = false;
		for(int k = 0; k < (int) m_EffectBars.size(); k++)
		{
			CEffectEditBar* pBar = m_EffectBars[k];
			if(pBar->GetSafeHwnd() == NULL)
			{
				m_EffectBars.erase(m_EffectBars.begin() + k);
				bFound = true;
				break;
			}
			else if(pBar->GetEditTarget() == pCurrUnit)
			{
				m_EffectBars[i] = NULL;
				if(pBar->GetSafeHwnd() != NULL)
				{

					if(pBar->IsTabbed())
					{
						pBar->GetParentTabbedBar()->DetachControlBar(pBar, TRUE);
					}

					ShowControlBar(pBar, FALSE, FALSE, FALSE);
#if _BCGCBPRO_VERSION_MAJOR >= 10
					RemoveControlBarFromDockManager(pBar, TRUE, TRUE, FALSE, NULL);
#else
					RemoveControlBarFromDockManager(pBar, TRUE, TRUE, FALSE);
#endif
					pBar->DestroyWindow();
				}
				delete pBar;
				m_EffectBars.erase(m_EffectBars.begin() + k);
				bFound = true;
				break;
			}
		}

		assert(bFound == true);
	}

	CString strInfo;
	strInfo.Format("ClearModelPrivateEffectBars, %d rest\n", m_EffectBars.size());
	assert(m_EffectBars.size() == 0);
	TRACE(strInfo.GetBuffer());

}

void CMainFrame::OnUndo()
{
	//CEffectMgr::GetInstance()->GetCommandMgr()->Undo();
}

void CMainFrame::OnUpdateUndo(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;

	pCmdUI->Enable(CEffectMgr::GetInstance()->GetCommandMgr()->CanUndo() ? TRUE : FALSE);
}

void CMainFrame::OnRedo()
{
	//CEffectMgr::GetInstance()->GetCommandMgr()->Redo();
}

void CMainFrame::OnUpdateRedo(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;
	pCmdUI->Enable(CEffectMgr::GetInstance()->GetCommandMgr()->CanRedo() ? TRUE : FALSE);
}
void CMainFrame::OnAccRedo()
{
	//CEffectMgr::GetInstance()->GetCommandMgr()->Redo();
}

void CMainFrame::OnAccUndo()
{
	//CEffectMgr::GetInstance()->GetCommandMgr()->Undo();

}



void CMainFrame::OnMnuSaveSm()
{
	CString path = GetOpenFilePath(FALSE, "character", CEffectMgr::GetInstance()->m_strSmName.GetBuffer(), "ent", "Entity Files(*.ent)\0*.ent\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		bool bRet = CEffectMgr::GetInstance()->SaveAsShowModel(path);
		if(bRet)
			MessageBox("保存成功");
		else
			MessageBox("保存失败，可能目标文件已存在而且没有checkout");
	}

	//if(bNeedUpdateList)
	//	UpdateEffectList(0);
}

bool IsEntityDataResource(const char *path)
{
	Ogre::Resource *pres = Ogre::ResourceManager::getSingleton().blockLoad(path);
	bool b = pres!=NULL&&IS_KIND_OF(Ogre::EntityData, pres);

	if(pres) pres->release();
	return b;
}
void CMainFrame::OnMnuOpenSm()
{
	CString path = GetOpenFilePath(TRUE, "character", "", "ent", "Entity Files(*.ent)\0*.ent\0\0", TRUE);
	if(path.GetLength() > 0)
	{
		LoadScene(path);
	}
}

void CMainFrame::OnLoadBGMesh()
{
	//CResLoadSaveDlg resdlg;
	//if(IDOK == resdlg.DoModal())
	//{
	//	CString fileName = resdlg.m_strSelectedChunk;
	//	CEnginAgent::GetInstance()->SetBackNode(fileName.GetBuffer());
	//}
}

LRESULT CMainFrame::OnUpdateAnchorItem(WPARAM wParam, LPARAM lParam)
{
	//m_EntityEditDlg.m_wndAnchor.UpdateCurrentItem();
	m_EntityEditDlg.m_wndBone.UpdateCurrentItem();
	return 0;
}

LRESULT CMainFrame::OnUpdateSkillBar(WPARAM wParam, LPARAM lParam)
{
	//m_EntityEditDlg.m_wndSkill.UpdatePropListData();
	return 0;
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//DWORD tick = ::GetTickCount();
	//((CMainFrame*)theApp.m_pMainWnd)->GraphicProcess((long)tick);
	//Invalidate(FALSE);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_SYSKEYDOWN) 
	{ 
		if(pMsg->wParam == VK_MENU) 
		{ 
			return TRUE; 
		} 
	}
	return CBCGPFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnBtnShowGrount()
{
	bool bShow = !CEnginAgent::GetInstance()->IsShowGround();
	CEnginAgent::GetInstance()->ShowGround(bShow);
}
void CMainFrame::OnUpdateBtnShowGround(CCmdUI* pUI)
{
	pUI->SetCheck(CEnginAgent::GetInstance()->IsShowGround() ? 1 : 0);
}

void CMainFrame::OnBtnIncCoord()
{
	CCoord::ms_fScale *= 1.25f;
}
void CMainFrame::OnBtnDecCoord()
{
	CCoord::ms_fScale *= 0.8f;
}

void CMainFrame::OnShowBouding()
{
	if(CInteract::GetShowState() == IS_HIDE)
		CInteract::SetShowState(IS_ALWAYS_SHOW);
	else
		CInteract::SetShowState(IS_HIDE);
}

void CMainFrame::OnUpdateShowBouding(CCmdUI* pUI)
{
	if(CInteract::GetShowState() == IS_HIDE)
		pUI->SetCheck(0);
	else
		pUI->SetCheck(1);
}

void CMainFrame::OnBtnCoodPos()
{
	CCoord::ms_coordtype = CT_POS;
}
void CMainFrame::OnUpdateBtnCoodPos(CCmdUI* pUI)
{
	if(CCoord::ms_coordtype == CT_POS)
		pUI->SetCheck(1);
	else
		pUI->SetCheck(0);
	pUI->Enable(TRUE);
}

void CMainFrame::OnBtnCoodRot()
{
	CCoord::ms_coordtype = CT_ROTATE;
}
void CMainFrame::OnUpdateBtnCoodRot(CCmdUI* pUI)
{
	if(CCoord::ms_coordtype == CT_ROTATE)
		pUI->SetCheck(1);
	else
		pUI->SetCheck(0);
	pUI->Enable(TRUE);
}

void CMainFrame::OnBtnCoodScale()
{
	CCoord::ms_coordtype = CT_SCALE;
}

void CMainFrame::OnUpdateBtnCoodScale(CCmdUI* pUI)
{
	if(CCoord::ms_coordtype == CT_SCALE)
		pUI->SetCheck(1);
	else
		pUI->SetCheck(0);
	pUI->Enable(TRUE);
}

void CMainFrame::OnBtnChangeRef()
{
	CRefNodeMgr* pRef = CEffectMgr::GetInstance()->GetRefNodeMgr();
	CDammyNodeInfo * pNodeInfo =  pRef->GetNodeInfo(REF_POS_RIGHT);
	if(pNodeInfo != NULL)
	{
		int iNewPosIndex = 0;
		if(pNodeInfo->m_iDefault >= 0 )
		{
			iNewPosIndex = (pNodeInfo->m_iDefault + 1) % CRefNodeMgr::ms_iDefPosCount;
		}

		pNodeInfo->m_pDammyNode->setPosition(CRefNodeMgr::ms_defPos[iNewPosIndex]);
		pNodeInfo->m_iDefault = iNewPosIndex;
	}
}
void CMainFrame::OnMnuSrtprop()
{
	ShowControlBar(&m_wndSrtBar, !m_wndSrtBar.IsVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateMnuSrtprop(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndSrtBar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnBtnAABB()	// AABB包围盒
{
	if(m_wndBoundBar.IsVisible())
	{
		m_wndBoundBar.UpdateList();
	}
	ShowControlBar(&m_wndBoundBar, !m_wndBoundBar.IsVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateBtnAABB(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndBoundBar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnBackgroundOther()
{
	CFileDialog filedlg(TRUE, NULL, NULL, NULL, m_StrTitle,this);
	if (IDOK == filedlg.DoModal())
	{
		SetCurrentDirectory(m_OldPath);
		CString m_filename = filedlg.GetPathName();
		m_filename.MakeLower();
		if (m_filename.Right(4) == "omod")
		{
			ClearBKModel();
			m_pBKModel = loadModelFromFile(m_filename.GetBuffer());
			m_pBKModel->setPosition(WorldPos(0,0,0));
			CEnginAgent::GetInstance()->AttachActor(m_pBKModel);
		}
	}
}

void CMainFrame::OnBackgroundNone()
{
	ClearBKModel();
}


void CMainFrame::OnSetDistort()	// 屏幕扭曲
{
	CEnginAgent* pEngineAgent = CEnginAgent::GetInstance();
	pEngineAgent->SetDistort(!pEngineAgent->GetDistort());
}

void CMainFrame::OnUpdateSetDistort(CCmdUI* pUI)
{
	if(CEnginAgent::GetInstance()->GetDistort() == true)
	{
		pUI->SetCheck(1);
	}
	else
	{
		pUI->SetCheck(0);
	}
}

void CMainFrame::OnBtnProj()	// 透视投影
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->SetProj(true);
}

void CMainFrame::OnUpdateBtnProj(CCmdUI *pCmdUI)
{
	if(CEnginAgent::GetInstance()->GetCameraCtrl() == NULL) return;

	if(CEnginAgent::GetInstance()->GetCameraCtrl()->IsProjMode() == true)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnBtnOrtho()	// 平行投影
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->SetProj(false);
}

void CMainFrame::OnUpdateBtnOrtho(CCmdUI *pCmdUI)
{
	if(CEnginAgent::GetInstance()->GetCameraCtrl() == NULL) return;

	if(CEnginAgent::GetInstance()->GetCameraCtrl()->IsProjMode() == true)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(1);
}

void CMainFrame::OnBtnTopview()	// 顶视
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->TopView();
}

void CMainFrame::OnBtnLeftview()	// 左视
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->LeftView();
}

void CMainFrame::OnBtnFontview()	// 正式
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->FontView();
}

void CMainFrame::OnFortyFiveview()	// 45度角视图
{
	CEnginAgent::GetInstance()->FortyFiveView();
}

void CMainFrame::OnBtnGlow()
{
	CEnginAgent::GetInstance()->EnableGlow(!CEnginAgent::GetInstance()->IsGlowEnable());
}

void CMainFrame::OnUpdateBtnGlow(CCmdUI *pCmdUI)
{
	if(CEnginAgent::GetInstance()->IsGlowEnable())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnActorEditMode()
{
	if(IS_KIND_OF(Ogre::GameTerrainScene, CEnginAgent::GetInstance()->m_pRoom))
		CEnginAgent::GetInstance()->SetEditModel(!CEnginAgent::GetInstance()->IsEditModel());
}

void CMainFrame::OnUpdateActorEditMode(CCmdUI* pCmdUI)
{
	if(CEnginAgent::GetInstance()->m_pRoom == NULL) return;

	if(!IS_KIND_OF(Ogre::GameTerrainScene, CEnginAgent::GetInstance()->m_pRoom))
		pCmdUI->Enable(false);
	else
	{
		bool bIsEditMode = CEnginAgent::GetInstance()->IsEditModel();
		if(bIsEditMode)
			pCmdUI->SetCheck();
		else
			pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnHideMainActor()	// 隐藏主角
{
	bool bShow = CEffectMgr::GetInstance()->GetShowState();
	if(bShow)
		CEffectMgr::GetInstance()->SetMainActorState(false);
	else 
		CEffectMgr::GetInstance()->SetMainActorState(true);
}

void CMainFrame::OnUpdateHideMainActor(CCmdUI* pCmdUI)
{
	if(CEffectMgr::GetInstance() == NULL) return;

	bool bShow = CEffectMgr::GetInstance()->GetShowState();
	if(bShow)
		pCmdUI->SetCheck(0);
	else 
		pCmdUI->SetCheck(1);
}

LRESULT CMainFrame::OnAddEfxToMotion(WPARAM wParam, LPARAM lParam)
{
	m_EntityEditDlg.m_wndMotion.SendMessage(EEN_ADD_EFXFILE_TO_MODELMOTION, wParam, lParam);
	return (LRESULT)0;
}

static CString s_FileSavePath;
void CMainFrame::SetCurOpenFilePath(const char *path)
{
	s_FileSavePath = path;

	char buffer[1024];
	sprintf(buffer, "WorldEditor - [%s]", path);
	SetWindowText(buffer);
}

void CMainFrame::OnBackgroundStreet(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::ClearBKModel()
{
	if (m_pBKModel)
	{
		CEnginAgent::GetInstance()->DetachActor(m_pBKModel);/*AttachActor(m_pBKModel);*/
		m_pBKModel->release();
		m_pBKModel = NULL;
	}
}

//void CMainFrame::BKModelUpdate()
//{
//	if(m_pBKModel == NULL)
//		return;
//	static uint pretime = timeGetTime();
//	uint curtime = timeGetTime();
//	m_pBKModel->update(curtime - pretime, NULL);
//	pretime = curtime;
//}
