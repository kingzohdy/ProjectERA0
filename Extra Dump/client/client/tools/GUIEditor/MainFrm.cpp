// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "CmdMgr.h"

#include "MainFrm.h"
#include <Windows.h>
#include <MMSystem.h>
#include "EnginAgent.h"
#include "UIPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern UIEditManager * g_pUIEditMgr;
extern CCommandMgr * g_pCmdMgr;
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2008, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2008, OnUpdateAppLook)
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_UI_GROUP, &CMainFrame::OnUIGroup)
	ON_MESSAGE(WM_MY_MESSAGE, &CMainFrame::DisplaySelectedUI)
	//ON_NOTIFY(TVN_SELCHANGED, IDT_OBJECT_TREE, &CMainFrame::OnTvnSelchangedObjTree)
	//ON_COMMAND(ID_UIUPDATE, &CMainFrame::OnUiupdate)
	//ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, &CMainFrame::OnUiupdate)
	//ON_COMMAND(ID_FILE_SAVE_AS, &CMainFrame::OnXMLFileSaveAs)
	//ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnXMLFileSave)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnEditRedo)
	
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{

	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2007);
	// TODO: add member initialization code here
	m_FileOpened = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	OnAppLook (m_nAppLook);

	// VISUAL_MANAGER
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW|WS_MAXIMIZE|WS_HSCROLL|WS_VSCROLL,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}


	if (CBCGPToolBar::GetUserImages () == NULL)
	{
		// Load toolbar user images:
		if (!m_UserImages.Load (_T(".\\UserImages.bmp")))
		{
			TRACE(_T("Failed to load user images\n"));
		}
		else
		{
			CBCGPToolBar::SetUserImages (&m_UserImages);
		}
	}

	CBCGPToolBar::EnableQuickCustomization ();


	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

	CList<UINT, UINT>	lstBasicCommands;

	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
	lstBasicCommands.AddTail (ID_FILE_NEW);
	lstBasicCommands.AddTail (ID_FILE_OPEN);
	lstBasicCommands.AddTail (ID_FILE_SAVE);
	lstBasicCommands.AddTail (ID_FILE_PRINT);
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);
	lstBasicCommands.AddTail (ID_EDIT_UNDO);
	lstBasicCommands.AddTail (ID_RECORD_NEXT);
	lstBasicCommands.AddTail (ID_RECORD_LAST);
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2000);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2003);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2005);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_WIN_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_1);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_2);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_3);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2008);

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.SetTransparentColor (RGB (255, 0, 255));
	imagesWorkspace.Load (IDB_WORKSPACE);
	
	if (!m_wndWorkSpace.Create (_T("资源管理器"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);

	if (!m_wndWorkSpace2.Create (_T("属性"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE2,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 2\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace2.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);


	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
	
	if (!CreateToolBox ())
	{
		TRACE0("Failed to create toolbox\n");
		return -1;      // fail to create
	}
	m_pwndUIPanel->SetIcon(imagesWorkspace.ExtractIcon (2), FALSE);

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_pwndUIPanel->EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar(m_pwndUIPanel,AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_wndWorkSpace,AFX_IDW_DOCKBAR_TOP);
	DockControlBar(&m_wndWorkSpace2);
	m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_STANDARD, FALSE, NULL);
	//m_pwndUIPanel->AttachToTabWnd ( &m_wndWorkSpace2, BCGP_DM_STANDARD, FALSE, NULL);
	DockControlBar(&m_wndOutput);
	//DockControlBar(m_pwndUIPanel);


	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));

	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);

	// Enable control bar context menu (list of bars + customize command):
	EnableControlBarMenu (	
		TRUE,				// Enable
		ID_VIEW_CUSTOMIZE, 	// Customize command ID
		_T("Customize..."),	// Customize command text
		ID_VIEW_TOOLBARS);	// Menu items with this ID will be replaced by
							// toolbars menu
	m_wndView.OnInit();	

	g_pUIEditMgr	= new UIEditManager;
	g_pCmdMgr		= new CCommandMgr;

	return 0;
}


BOOL CMainFrame::CreateToolBox()
{
	CToolBoxBarEx* pToolBox = new CToolBoxBarEx;

	if (!pToolBox->Create (_T("控件组"), this, CRect (0, 0, 200, 200),
		TRUE, 
		ID_UI_GROUP,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		0, CBRS_BCGP_FLOAT | CBRS_BCGP_RESIZE))
	{
		TRACE0("Failed to create ToolBox Bar\n");
		return FALSE;		// fail to create
	}
	
	pToolBox->AddToolsPage (_T("Windows Forms"), IDB_TOOLS_GROUP, 16, 
		_T("Pointer\nButton\nLabel\nTextBox\nMainMenu\nCheckBox\nRadioButton\nGroupBox\nPictureBox\nPanel\nDataGrid\nListBox\nCheckedListBox\nComboBox\nListView\nTreeView\nTabControl\nDateTimePicker\nMonthCalendar\nHScrollBar\nVScrollBar\nTimer\nSplitter\nDomainUpDown\nNumericUpDown\nTrackBar\nProgressBar\nRichTextBox"));
	
	pToolBox->SetActivePage (0);

	m_pwndUIPanel = pToolBox;
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = DEFAULT_UI_WIDTH;
	cs.cy = DEFAULT_UI_HEIGHT;
	cs.style |= WS_MAXIMIZE;
	return TRUE;
}


// CMainFrame diagnostics

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


// CMainFrame message handlers




void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */);

	pDlgCust->EnableUserDefinedToolbars ();
	pDlgCust->Create ();
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}

void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007:
	case ID_VIEW_APPLOOK_2007_1:
	case ID_VIEW_APPLOOK_2007_2:
	case ID_VIEW_APPLOOK_2007_3:
		// enable Office 2007 look:
		switch (m_nAppLook)
		{
		case ID_VIEW_APPLOOK_2007:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_2007_1:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_2007_2:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
			break;

		case ID_VIEW_APPLOOK_2007_3:
			CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Aqua);
			break;
		}

		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS 2005 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2008:
		// enable VS 2008 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2008));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;
	}

	CBCGPDockManager* pDockManager = GetDockManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustBarFrames ();
	}

	CBCGPTabbedControlBar::ResetTabs ();

	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);
}

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}
 // RIBBON_APP

void CMainFrame::DoFrame()
{
	static long accumtick = 0;
	static long lasttick = 0;

	long curtick = timeGetTime();
	long dtick = curtick - lasttick;

	accumtick += dtick;
	lasttick = curtick;

	static long count = 0;
	count++;

	CEnginAgent* pEA = CEnginAgent::GetInstance();
	if(pEA->m_bIsInitialized)
	{
		pEA->Update(dtick);
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
	}

	Frame * pFrame = g_pFrameMgr->GetFocusFrame();
	
	if ( NULL !=pFrame && m_FileOpened )
	{
		
		std::string focusName = pFrame->GetName();
		std::string curName = m_wndWorkSpace2.GetCurrentObj()->GetName();
		bool changed = (focusName != curName) 
						|| CEnginAgent::GetInstance()->m_pEditUI->UIObjectChanged()
						|| g_pCmdMgr->m_bUIObjEXE;

		if ( curName.empty() || !changed || !m_wndWorkSpace2.WorkSpaceIsON() )
		{
			//break;
		}
		else
		{
			m_wndWorkSpace2.DisplayProp( pFrame );
			CEnginAgent::GetInstance()->m_pEditUI->SetChanged( false );
			g_pCmdMgr->m_bUIObjEXE = false;
		}
	}
}

void CMainFrame::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	m_wndWorkSpace.TocFileOpen();
	m_FileOpened = true;
}

void CMainFrame::SaveXMLFile(const char * path)
{
	m_wndWorkSpace2.SavePropToXML( path );
}

LRESULT CMainFrame::DisplaySelectedUI(WPARAM w, LPARAM l)
{
	std::map<std::string, UIObject *>::iterator iterLast;
	std::map<std::string, UIObject *>::iterator iterCur;

	iterLast	= g_pFrameMgr->m_ObjectTable.find( this->m_wndWorkSpace.m_strLastObjName.c_str() );
	iterCur		= g_pFrameMgr->m_ObjectTable.find( this->m_wndWorkSpace.m_strCurObjName.c_str() );
	
	if ( iterCur != g_pFrameMgr->m_ObjectTable.end() )
	{
		UIObject * pCurUIObj = iterCur->second;
		m_wndWorkSpace2.WorkSpaceON( true );

		if ( iterLast!=g_pFrameMgr->m_ObjectTable.end() )
		{
			UIObject *pLastUIObj = iterLast->second;	
			g_pUIEditMgr->ShowCurentUI( pLastUIObj, pCurUIObj );
		}

		m_wndWorkSpace2.DisplayProp( pCurUIObj );
	}

	return 0;
}
//void CMainFrame::OnTvnSelchangedObjTree(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
//	// TODO: Add your control notification handler
//	//       code here
//
//
//
//	MessageBox(m_wndWorkSpace.curObjName.c_str());
//	*pResult = 0;
//}
//

//LRESULT CMainFrame::OnUiupdate(WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 在此添加命令处理程序代码
//	m_wndWorkSpace2.UIUpdate();
//	
//	return 1;
//}

//void CMainFrame::OnXMLFileSaveAs()
//{
//	// TODO: 在此添加命令处理程序代码
//	m_wndWorkSpace2.SavePropToXML();
//}
//
//void CMainFrame::OnXMLFileSave()
//{
//	// TODO: 在此添加命令处理程序代码
//	m_wndWorkSpace2.SavePropToXML();
//}

void CMainFrame::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
  	//MessageBox("Ctrl + Z", "CMD", MB_OK);
	//CEditMoveCmd cmd = new CEditMoveCmd()
	g_pCmdMgr->EditUndo();
}

void CMainFrame::OnEditRedo()
{
	// TODO: 在此添加命令处理程序代码
	//MessageBox("Ctrl + R", "CMD", MB_OK);
	g_pCmdMgr->EditRedo();
}

void CMainFrame::OnUIGroup()
{
	int nPage = -1;
	int nTool = -1;
	
	CString UItype;
	nTool = ((CToolBoxBarEx*) m_pwndUIPanel)->GetLastClickedTool (nPage);
	
	switch(nTool)
	{
	case UIT_BUTTON:
		{
			UItype = "Button";
			break;
		}
	}
	MessageBox(UItype);
	//CString str;
	//str.Format (_T("Toolbox item clicked\nPage = %d, item = %d"), nPage, nTool);
	//MessageBox (str);
}