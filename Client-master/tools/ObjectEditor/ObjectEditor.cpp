// ObjectEditor.cpp : ����Ӧ�ó��������Ϊ��
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


// CObjectEditorApp ����

CObjectEditorApp::CObjectEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CObjectEditorApp ����

CObjectEditorApp theApp;


// CObjectEditorApp ��ʼ��

BOOL CObjectEditorApp::InitInstance()
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
	RegDelnode(HKEY_CURRENT_USER, "Software\\MyWorld\\ObjectEditor");
	SetRegistryKey(_T("MyWorld"));
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	//Sleep(10000);
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,	NULL);

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	return TRUE;
}


// CObjectEditorApp ��Ϣ�������




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
void CObjectEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CObjectEditorApp ��Ϣ�������


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
