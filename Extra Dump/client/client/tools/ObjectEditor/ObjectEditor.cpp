// ObjectEditor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "MainFrm.h"
#include "ControlsHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment(lib, "winmm.lib")
#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "GameEngine_d.lib")
#pragma comment(lib, "luadll_d.lib")
#pragma comment(lib, "tolua++_d.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#pragma comment(lib, "GameEngine.lib")
#pragma comment(lib, "luadll.lib")
#pragma comment(lib, "tolua++.lib")
#endif
#pragma comment(lib, "Imm32.lib")

// CObjectEditorApp

BEGIN_MESSAGE_MAP(CObjectEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CObjectEditorApp::OnAppAbout)
END_MESSAGE_MAP()


// CObjectEditorApp 构造

CObjectEditorApp::CObjectEditorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CObjectEditorApp 对象

CObjectEditorApp theApp;


// CObjectEditorApp 初始化

BOOL CObjectEditorApp::InitInstance()
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
	RegDelnode(HKEY_CURRENT_USER, "Software\\MyWorld\\ObjectEditor");
	SetRegistryKey(_T("MyWorld"));
	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	//Sleep(10000);
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,	NULL);

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	return TRUE;
}


// CObjectEditorApp 消息处理程序




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
void CObjectEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CObjectEditorApp 消息处理程序


BOOL CObjectEditorApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class

	//((CMainFrame*)m_pMainWnd)->BKModelUpdate();
	((CMainFrame*)m_pMainWnd)->DoFrame();

	return TRUE;//return __super::OnIdle(lCount);
}

int CObjectEditorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	BCGCBProCleanUp();

	return __super::ExitInstance();
}

LRESULT SendMainFrameMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	return theApp.m_pMainWnd->SendMessage(message, wParam, lParam);
}

LRESULT PostMainFrameMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	return theApp.m_pMainWnd->PostMessage(message, wParam, lParam);
}
