// WorldEditor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "MainFrm.h"

#include "WorldEditorView.h"
#include "EditorManager.h"

#include "OgrePrerequisites.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWorldEditorApp

BEGIN_MESSAGE_MAP(CWorldEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CWorldEditorApp::OnAppAbout)
END_MESSAGE_MAP()


// CWorldEditorApp 构造

CWorldEditorApp::CWorldEditorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CWorldEditorApp 对象

CWorldEditorApp theApp;


// CWorldEditorApp 初始化

BOOL CWorldEditorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名

	SetRegistryKey(_T("wyylWorldEditor1"));

	CMainFrame *pmainframe = new CMainFrame;
	m_pMainWnd = pmainframe;

	pmainframe->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE, NULL, NULL);
	//pmainframe->SetIcon(LoadIcon(IDR_MAINFRAME),FALSE);//IDR_MAINFRAME

	pmainframe->ShowWindow(SW_SHOW);
	pmainframe->UpdateWindow();

	SetTimer(pmainframe->GetSafeHwnd(), 1, 33, NULL); 

	// 初始化GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken,&gdiplusStartupInput,NULL);

	if(AllocConsole())
	{
		SetConsoleTitle("Debug Window");
		freopen("CONOUT$","w", stdout);
		freopen("CONIN$", "r", stdin);
	}

	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CWorldEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CWorldEditorApp 消息处理程序


int CWorldEditorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	BCGCBProCleanUp();

	// 关闭Gdi+
	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	return __super::ExitInstance();
}

BOOL CWorldEditorApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	//计算FPS
	static unsigned int nframe = 0;
	static unsigned int lasttick = GetTickCount();
	static unsigned int accumtick = 0;

	nframe++;

	unsigned int curtick = GetTickCount();
	unsigned int dtick = curtick - lasttick;
	accumtick += dtick;
	lasttick = curtick;

	if(accumtick >= 1000)
	{
		CString  mapInfo;
		size_t numt = 0,nums = 0;
		if(g_pApp) g_pApp->GetGameDrawInfo( numt,nums);
		float fps = float(nframe) / Ogre::TickToTime(accumtick);

		((CMainFrame *)m_pMainWnd)->SetRenderInfo(fps, numt, nums);

		accumtick = 0;
		nframe = 0;
	}

	((CMainFrame *)m_pMainWnd)->OnIdle();
	if(g_pApp) g_pApp->doFrame();

	return __super::OnIdle(lCount);
}

BOOL CWorldEditorApp::InitApplication()
{
	// TODO: Add your specialized code here and/or call the base class

	return __super::InitApplication();
}
