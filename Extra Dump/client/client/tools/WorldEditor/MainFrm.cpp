// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "MainFrm.h"
#include "EditorManager.h"
#include "CreateMapDlg.h"
#include "TerrainEdit.h"
#include "ControlsHelper.h"
#include "MapLights.h"
#include "EditorMessageQue.h"
#include "EditEvent_ObjectBrush.h"
#include "EditEventHandlerMgr.h"
#include "MapPlayerBrower.h"
#include "Coordinate.h"

#include "TerBeastManager.h"
#include "EditorCamera.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include <iostream>
#include "TerBeastManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace TE;
extern const char *GetCurDir();

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMainFrame::OnFileSaveAs)
	ON_COMMAND(ID_VIEW_RESBROWER, &CMainFrame::OnViewResbrower)
	ON_COMMAND(ID_VIEW_EDITBAR, &CMainFrame::OnViewEditbar)
	ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
	ON_COMMAND(ID_EDITMODE_SELECT, &CMainFrame::OnEditModeSelect)
	ON_COMMAND(ID_EDITMODE_MOVE, &CMainFrame::OnEditModeMove)
	ON_COMMAND(ID_EDITMODE_ROTATE, &CMainFrame::OnEditModeRotate)
	ON_COMMAND(ID_EDITMODE_SCALE, &CMainFrame::OnEditModeScale)
	ON_COMMAND(ID_EDITMODE_ADD, &CMainFrame::OnEditModeAdd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESBROWER, &CMainFrame::OnUpdateViewResbrower)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITBAR, &CMainFrame::OnUpdateViewEditbar)
	ON_COMMAND(ID_EXPORT_MASK_FILE, &CMainFrame::OnExportMaskFile)
	ON_COMMAND(ID_CAL_NAVI_MAP, &CMainFrame::OnCalNaviMap)
	ON_COMMAND(ID_COPY_WALK_TO_FLY, &CMainFrame::OnCopyWalkToFly)
	ON_COMMAND(ID_CAL_STATIC_LIGHT, &CMainFrame::OnCalStaticLight)
	ON_COMMAND(ID_CAL_DIR_LIGHT, &CMainFrame::OnCalDirLight)
	ON_COMMAND(ID_EXPORT_OBJFILE, &CMainFrame::OnExportObjfile)
	ON_COMMAND(ID_PREVIEW_CAMERA_ANIM, &CMainFrame::OnPreviewCameraAnim)
	ON_COMMAND(ID_VIEW_BACKCOLOR, &CMainFrame::OnViewBackcolor)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FINDMODEL, &CMainFrame::OnFindmodel)
	ON_COMMAND(ID_EXPORT_OBJTERRAIN, &CMainFrame::OnExportObjterrain)
	ON_COMMAND(ID_EXPORT_OBJTERRAINANDWATER, &CMainFrame::OnExportObjterrainandwater)
//	ON_COMMAND(ID_BEASTBAKE, &CMainFrame::OnBeastbake)
	ON_COMMAND(ID_SHOW_BEASTBACK, &CMainFrame::OnShowBeastback)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BEASTBACK, &CMainFrame::OnUpdateShowBeastback)
//	ON_COMMAND(ID_BEASTBAKE, &CMainFrame::OnBeastbake)
ON_COMMAND(ID_BEASTBAKE, &CMainFrame::OnBeastbake)
ON_COMMAND(ID_USECAMERARENDERTARGET, &CMainFrame::OnUsecamerarendertarget)
ON_UPDATE_COMMAND_UI(ID_USECAMERARENDERTARGET, &CMainFrame::OnUpdateUsecamerarendertarget)
ON_COMMAND(ID_TERRAIN_USE_LIGHTMAP, &CMainFrame::OnTerrainUseLightmap)
ON_UPDATE_COMMAND_UI(ID_TERRAIN_USE_LIGHTMAP, &CMainFrame::OnUpdateTerrainUseLightmap)
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
	m_pBestManager = 0;
}

CMainFrame::~CMainFrame()
{
	if (m_pBestManager)
		delete m_pBestManager;
	m_pBestManager = 0;

	delete g_pApp;
	g_pApp = NULL;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*
	CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS (CBCGPVisualManager2003));
	CBCGPToolBar::SetLook2000(true);
	CBCGPToolBar::EnableQuickCustomization();
	*/

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if(!m_InfoPanel.Create("资源场景信息", this, CSize(160, 0), TRUE, 2, WS_CHILD|WS_VISIBLE|CBRS_LEFT))
	{
		TRACE0("Failed to create InfoPanel\n");
		return -1;
	}

	if(!m_EditPanel.Create(_T("场景编辑"), this, CSize (160, 0), TRUE, 1, WS_CHILD|WS_VISIBLE|CBRS_RIGHT))
	{
		TRACE0("Failed to create workspace bar\n");
		return -1;
	}

	m_TexLayerInfoDlg.Create(m_TexLayerInfoDlg.IDD, this);
	m_ModelInfoDlg.Create(m_ModelInfoDlg.IDD, this);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||	!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	if(!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建MenuBar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_InfoPanel.EnableDocking(CBRS_ALIGN_ANY);
	m_EditPanel.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_InfoPanel);
	DockControlBar(&m_EditPanel);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndMenuBar);

	g_pApp = new EditorManager;
	if(!g_pApp->init(AfxGetInstanceHandle(), m_wndView.GetSafeHwnd(), GetResPreviewWnd(), GetRadarViewWnd()))
	//if(!g_pApp->init(AfxGetInstanceHandle(), m_wndView.GetSafeHwnd(), NULL, NULL))
	{
		AfxMessageBox("Init Engine Error", MB_OK);
	}

	m_EditPanel.m_NpcEditDlg.InitNpcResData();

	m_FindDialog.Create(IDD_FIND,this);

	m_pBestManager = new BeastManager();

	BeastManager::getSingleton().Initlize();

	return 0;
}

HWND CMainFrame::GetResPreviewWnd()
{
	return m_InfoPanel.m_ResBrowerDlg.m_ResPreview.GetSafeHwnd();
}

HWND CMainFrame::GetRadarViewWnd()
{
	return NULL;
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
void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

static CString s_FileSavePath;
void CMainFrame::SetCurOpenFilePath(const char *path)
{
	s_FileSavePath = path;

	char buffer[1024];
	sprintf(buffer, "WorldEditor - [%s]", path);
	SetWindowText(buffer);
}

void CMainFrame::SetRenderInfo(float fps, int num_dp, int num_tri)
{
	CString  mapInfo;
	mapInfo.Format("FPS[%.1f]  绘制次数[%d]  三角形面数[%d]", fps, num_dp, num_tri);
	m_wndStatusBar.SetPaneText(0, mapInfo.GetBuffer());
}


void CMainFrame::OnFileNew()
{
	// TODO: Add your command handler code here
	CCreateMapDlg dlg;
	if(dlg.DoModal()) SetCurOpenFilePath("");
}

void CMainFrame::OnFileOpen()
{
	// TODO: Add your command handler code here
	CString path = GetOpenFilePath(TRUE, "maps", "", "map", "Map Files (.map)\0*.map\0\0", FALSE);
	if(!path.IsEmpty())
	{
		BeastManager::getSingleton().Reset();

		g_TerrainEdit.LoadTerrainFile(path.GetBuffer());
		SetCurOpenFilePath(path.GetBuffer());
	}
}

void CMainFrame::OnFileSave()
{
	// TODO: Add your command handler code here
	if(s_FileSavePath.IsEmpty())
	{
		OnFileSaveAs();
	}
	else
	{
		bool isShowStaticLight = BeastManager::getSingleton().IsBeastLightShow();
		if (isShowStaticLight)
		{
			OnShowBeastback ();
		}

		CString path = s_FileSavePath;
		int pos = path.ReverseFind('.');
		assert(path.Right(path.GetLength()-pos) == ".map");
		path = path.Left(pos);

		g_TerrainEdit.SaveTerrainFile(path.GetBuffer());
	}

	std::string infofilename = g_TerrainEdit.m_curSaveFile;
	int posTemp = infofilename.find_last_of('\\');
	if (posTemp != string::npos)
	{
		string strTemp = infofilename.substr(0, posTemp+1);
		BeastManager::msMapDir = strTemp;
	}
}

void CMainFrame::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	bool isShowStaticLight = BeastManager::getSingleton().IsBeastLightShow();
	if (isShowStaticLight)
	{
		OnShowBeastback ();
	}

	CString path = GetOpenFilePath(FALSE, "maps", "", "map", "Map Files(*.map)\0*.map\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		SetCurOpenFilePath(path.GetBuffer());

		int pos = path.ReverseFind('.');
		assert(path.Right(path.GetLength()-pos) == ".map");
		path = path.Left(pos);

		g_TerrainEdit.SaveTerrainFile(path.GetBuffer());
	}
}

void CMainFrame::OnIdle()
{
	// TODO: Add your message handler code here and/or call default
	if(g_TerrainEdit.m_bOpenTextureInfoDlg)
	{
		g_TerrainEdit.m_bOpenTextureInfoDlg = false;

		POINT ptCursor;
		GetCursorPos(&ptCursor);
		RECT  rect;
		m_TexLayerInfoDlg.ShowWindow(SW_SHOW);
		m_TexLayerInfoDlg.GetWindowRect(&rect);
		m_TexLayerInfoDlg.MoveWindow(ptCursor.x , ptCursor.y , rect.right-rect.left  ,rect.bottom-rect.top);
		m_TexLayerInfoDlg.InitData();
	}
	else if ( g_TerrainEdit.m_bOpenModelInfoDlg )
	{
		g_TerrainEdit.m_bOpenModelInfoDlg = false;

		POINT ptCursor;
		GetCursorPos(&ptCursor);
		RECT  rect;
		m_ModelInfoDlg.GetWindowRect(&rect);
		m_ModelInfoDlg.MoveWindow( ptCursor.x , ptCursor.y , rect.right - rect.left  ,rect.bottom - rect.top  );
		m_ModelInfoDlg.InitData();
		m_ModelInfoDlg.ShowWindow(SW_SHOW);
	}
	/*
	else if( g_TerrainEdit.m_bOpenSoundInfoDlg )
	{
		g_TerrainEdit.m_bOpenSoundInfoDlg = false;
		m_SoundInfo.ShowWindow( SW_SHOW );

		POINT ptCursor;
		GetCursorPos( &ptCursor );

		RECT  rect;
		m_SoundInfo.GetWindowRect ( &rect );
		m_SoundInfo.MoveWindow ( ptCursor.x , ptCursor.y , rect.right - rect.left  ,rect.bottom - rect.top  );
		m_SoundInfo.UpdateInfoData();
	}
	else if ( g_TerrainEdit.m_bOpenAreaInfoDlg )
	{
		g_TerrainEdit.m_bOpenAreaInfoDlg = false;
		m_AreaOptionDlg.ShowWindow( SW_SHOW );

		POINT ptCursor;
		GetCursorPos( &ptCursor );

		RECT  rect;
		m_AreaOptionDlg.GetWindowRect ( &rect );
		m_AreaOptionDlg.MoveWindow ( ptCursor.x , ptCursor.y , rect.right - rect.left  ,rect.bottom - rect.top  );
		//m_AreaOptionDlg.UpdateInfoData();
	}
	else if( g_TerrainEdit.m_bUpdateSoundList )
	{
		g_TerrainEdit.m_bUpdateSoundList = false;
		m_wndWorkSpace.m_SoundEditDlg.SetFocus();
	}

	if(g_TerrainEdit.m_bUpdateAmbientSettingDlg && g_AmbientSettingDlg)
	{
		g_AmbientSettingDlg->UpdateAmbient();
		g_AmbientSettingDlg->UpdateData( true );
		g_TerrainEdit.m_bUpdateAmbientSettingDlg = false;
	}*/

	EditorMsg edmsg;
	while(g_EditorMsgQue.popMessage(edmsg))
	{
		if(edmsg.type == EDMSG_SELECTLIGHT) m_EditPanel.m_LightEditDlg.OnSelectLight(edmsg.param[0]);
		else if(edmsg.type == EDMSG_UPDATELIGHTS) m_EditPanel.m_LightEditDlg.UpdateLightList();
		else if(edmsg.type == EDMSG_SELECTNPC) m_EditPanel.m_NpcEditDlg.OnSelectNPC(edmsg.param[0]);
		else if(edmsg.type == EDMSG_UPDATENPC) m_EditPanel.m_NpcEditDlg.UpdateNpcData();
		else if(edmsg.type == EDMSG_DELETELIGHT) m_EditPanel.m_LightEditDlg.OnDeleteLight(edmsg.param[0]);
		else if(edmsg.type == EDMSG_DELETENPC) m_EditPanel.m_NpcEditDlg.OnDeleteNPC(edmsg.param[0]);
	}

	m_EditPanel.m_AreaEditDlg.UpdateAreaData();
	m_EditPanel.m_WaterEditDlg.UpdateWaterData();
}

void CMainFrame::OnViewResbrower()
{
	// TODO: Add your command handler code here
	if(m_InfoPanel.IsWindowVisible()) 
		m_InfoPanel.ShowWindow(SW_HIDE);
	else 
		m_InfoPanel.ShowWindow(SW_SHOW);

	m_InfoPanel.UpdateWindow();
}

void CMainFrame::OnViewEditbar()
{
	// TODO: Add your command handler code here
	if(m_EditPanel.IsWindowVisible()) m_EditPanel.ShowWindow(SW_HIDE);
	else m_EditPanel.ShowWindow(SW_SHOW);

	m_EditPanel.UpdateWindow();
}

void CMainFrame::OnEditRedo()
{
	// TODO: Add your command handler code here
	g_TerrainEdit.Redo();
}

void CMainFrame::OnEditUndo()
{
	// TODO: Add your command handler code here
	g_TerrainEdit.Undo();
}

void CMainFrame::OnEditModeSelect()
{
	if(g_TerrainEdit.GetCurEditEvent() == EDITEVENT_OBJECTBRUSH)
	{
		ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
		phandler->setEditOP(OBJEDIT_OP_SELECT);
		g_TerrainEdit.GetCoordinate()->setCoordMode(COORD_SELECT);
	}
}

void CMainFrame::OnEditModeMove()
{
	if(g_TerrainEdit.GetCurEditEvent() == EDITEVENT_OBJECTBRUSH)
	{
		ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
		phandler->setEditOP(OBJEDIT_OP_MOVE);
		g_TerrainEdit.GetCoordinate()->setCoordMode(COORD_MOVE);
	}
}

void CMainFrame::OnEditModeRotate()
{
	if(g_TerrainEdit.GetCurEditEvent() == EDITEVENT_OBJECTBRUSH)
	{
		ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
		phandler->setEditOP(OBJEDIT_OP_ROTATE);
		g_TerrainEdit.GetCoordinate()->setCoordMode(COORD_ROTATE);
	}
}

void CMainFrame::OnEditModeScale()
{
	if(g_TerrainEdit.GetCurEditEvent() == EDITEVENT_OBJECTBRUSH)
	{
		ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
		phandler->setEditOP(OBJEDIT_OP_SCALE);
		g_TerrainEdit.GetCoordinate()->setCoordMode(COORD_SCALE);
	}
}

void CMainFrame::OnEditModeAdd()
{
	if(g_TerrainEdit.GetCurEditEvent() == EDITEVENT_OBJECTBRUSH)
	{
		ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
		phandler->setEditOP(OBJEDIT_OP_ADD);
		g_TerrainEdit.GetCoordinate()->setCoordMode(COORD_MOVE);
	}
}

void CMainFrame::OnUpdateViewResbrower(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_InfoPanel.IsWindowVisible()) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateViewEditbar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_EditPanel.IsWindowVisible()) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CMainFrame::OnExportMaskFile()
{
	// TODO: Add your command handler code here
	g_pApp->exportMaskFile();
}

void CMainFrame::OnCalNaviMap()
{
	// TODO: Add your command handler code here
	g_pApp->calNaviMap();
}

void CMainFrame::OnCopyWalkToFly()
{
	g_pApp->copyWalkMaskToFly();
}

void CMainFrame::OnCalStaticLight()
{
	// TODO: Add your command handler code here
	g_LightManager.flushAllLights(false);
}

void CMainFrame::OnCalDirLight()
{
	// TODO: Add your command handler code here
	g_LightManager.flushAllLights(true);
}

void CMainFrame::OnExportObjfile()
{
	// TODO: Add your command handler code here
	CString path = GetOpenFilePath(FALSE, "maps", "", "obj", "OBJ Files(*.obj)\0*.obj\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		g_TerrainEdit.exportToObjFile(path.GetBuffer());
	}
}

void CMainFrame::OnPreviewCameraAnim()
{
	// TODO: Add your command handler code here
	if(g_TerrainEdit.m_EditOption.mCurEditEvent != PLACE_PLYAER)
	{
		AfxMessageBox("需要在预览模式下才能使用相机动画预览");
		return;
	}

	CString path = GetOpenFilePath(TRUE, "maps", "", "omod", "相机动画文件(*.omod)\0*.omod\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		BaseEditHandler *phandler = g_EventProcessFactory.GetEventProcess(g_TerrainEdit.m_EditOption.mCurEditEvent);
		static_cast<MapPlayerBrower *>(phandler)->beginCameraAnim(path.GetBuffer());
	}
}

void CMainFrame::OnViewBackcolor()
{
	// TODO: Add your command handler code here
	CColorDialog dlg;

	if(IDOK == dlg.DoModal())
	{
		COLORREF color = dlg.GetColor();
		unsigned int r = GetRValue(color);
		unsigned int g = GetGValue(color);
		unsigned int b = GetBValue(color);

		g_TerrainEdit.m_pMainRU->setClearColor(RGB(b,g,r)|0xff000000);
	}
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CBCGPFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnFindmodel()
{
	// TODO: Add your command handler code here
	m_FindDialog.ShowWindow(1);
}

void CMainFrame::OnExportObjterrain()
{
	// TODO: Add your command handler code here

	CString path = GetOpenFilePath(FALSE, "maps", "", "obj", "OBJ Files(*.obj)\0*.obj\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		g_TerrainEdit.exportTerrainToObjFile(path.GetBuffer());
	}
}

void CMainFrame::OnExportObjterrainandwater()
{
	// TODO: Add your command handler code here

	CString path = GetOpenFilePath(FALSE, "maps", "", "obj", "OBJ Files(*.obj)\0*.obj\0\0", FALSE);
	if(path.GetLength() > 0)
	{
		g_TerrainEdit.exportTerrainAndWater(path.GetBuffer());
	}
}

void CMainFrame::OnShowBeastback()
{
	// TODO: Add your command handler code here

	bool isShowStaticLight = BeastManager::getSingleton().IsBeastLightShow();

	isShowStaticLight = !isShowStaticLight;

	BeastManager::getSingleton().SetBeastShow(isShowStaticLight);

	if (isShowStaticLight)
	{
		Ogre::Root::getSingleton().setLightMode(2);
	}
	else
	{
		Ogre::Root::getSingleton().setLightMode(0);
	}
}

void CMainFrame::OnUpdateShowBeastback(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (BeastManager::getSingleton().IsBeastLightShow())
	{
		pCmdUI->SetCheck();
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnBeastbake()
{
	// TODO: Add your command handler code here

	BeastManager::getSingleton().Reset ();

	BeastManager::getSingleton().BeginScene();

	std::cout << "AddLightsToBeast" << endl;

	// Lights
	g_LightManager.AddLightsToBeast();

	std::cout << "AddObjectsToBeast" << endl;

	// Objects
	g_TerrainEdit.AddObjectsToBeast();

	// Camera
	BeastManager::getSingleton().SetCamera(
		BeastManager::getSingleton().GetRandName("camera"), &g_Camera);

	BeastManager::getSingleton().EndScene();

	std::string xmlFileNameGI = "bakingGI.xml";
	BeastManager::getSingleton().CreateXML(xmlFileNameGI, true);

	std::cout << "DoBake" <<endl;

	Ogre::SceneManager::getSingleton().setDraw(false);

	if (BeastManager::getSingleton().DoBake())
	{
		Ogre::SceneManager::getSingleton().setDraw(true);

		std::cout << "CollectLightData" <<endl;

		BeastManager::getSingleton().CollectLightData();

		MessageBox("场景烘焙成功,请保存地图！","成功");
	}
	Ogre::SceneManager::getSingleton().setDraw(true);
}

void CMainFrame::OnUsecamerarendertarget()
{
	// TODO: Add your command handler code here

	bool use = BeastManager::getSingleton().IsUseCameraTarget();

	BeastManager::getSingleton().SetUseCameraTarget(!use);
}

void CMainFrame::OnUpdateUsecamerarendertarget(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	bool use = false;
	use = BeastManager::getSingleton().IsUseCameraTarget();

	if (use)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnTerrainUseLightmap()
{
	// TODO: Add your command handler code here
	int mode = BeastManager::getSingleton().GetStaticLightMode();
	mode = 1-mode;
	BeastManager::getSingleton().SetStaticLightMode((BeastManager::StaticLightMode)mode);
}

void CMainFrame::OnUpdateTerrainUseLightmap(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	int mode = BeastManager::getSingleton().GetStaticLightMode();

	if (mode == 1)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}
