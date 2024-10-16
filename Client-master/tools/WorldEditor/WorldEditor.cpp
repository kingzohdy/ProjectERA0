// WorldEditor.cpp : ����Ӧ�ó��������Ϊ��
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


// CWorldEditorApp ����

CWorldEditorApp::CWorldEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CWorldEditorApp ����

CWorldEditorApp theApp;


// CWorldEditorApp ��ʼ��

BOOL CWorldEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��

	SetRegistryKey(_T("wyylWorldEditor1"));

	CMainFrame *pmainframe = new CMainFrame;
	m_pMainWnd = pmainframe;

	pmainframe->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE, NULL, NULL);
	//pmainframe->SetIcon(LoadIcon(IDR_MAINFRAME),FALSE);//IDR_MAINFRAME

	pmainframe->ShowWindow(SW_SHOW);
	pmainframe->UpdateWindow();

	SetTimer(pmainframe->GetSafeHwnd(), 1, 33, NULL); 

	// ��ʼ��GDI+
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CWorldEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CWorldEditorApp ��Ϣ�������


int CWorldEditorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	BCGCBProCleanUp();

	// �ر�Gdi+
	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	return __super::ExitInstance();
}

BOOL CWorldEditorApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	//����FPS
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
