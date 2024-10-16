// OffLineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OffLine.h"
#include "OffLineDlg.h"
#include "resource.h"
#include "GUILoad.h"
#include "RenderScene.h"
#include "MudNetMgr.h"

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreLog.h>
#include <GameDBData.h>
#include <AvatarManager.h>
#include <AvatarEntity.h>
#include <OgreSceneManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// COffLineDlg �Ի���




COffLineDlg::COffLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COffLineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pRoot = NULL;
	m_pSceneManager = NULL;
	m_pExpRenderUnit = NULL;
	m_pPetRenderUnit = NULL;
}

void COffLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COffLineDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_CHAT, &COffLineDlg::OnBnClickedChat)
	ON_BN_CLICKED(IDC_EXP, &COffLineDlg::OnBnClickedExp)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PET, &COffLineDlg::OnBnClickedPet)
	ON_BN_CLICKED(IDC_BOARD, &COffLineDlg::OnBnClickedBoard)
	ON_MESSAGE(WM_NET_NOTIFY,&COffLineDlg::onNetNotify)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_COMMAND(ID_TRAY_QUIT, &COffLineDlg::OnTrayQuit)
	ON_BN_CLICKED(IDC_STARTGAME, &COffLineDlg::OnBnClickedStartgame)
END_MESSAGE_MAP()


// COffLineDlg ��Ϣ�������

BOOL COffLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COffLineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
	if(SC_MINIMIZE == nID)
	{
		ToTray(TRUE);
		return;
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COffLineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc( this );
		HDC hdc = dc.GetSafeHdc();

		m_BackGround.Blt(hdc, 0, 0);

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR COffLineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int COffLineDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	MudNetMgr::getSingleton().addMessageHwnd(GetSafeHwnd());

	m_LoginDialog.Create(IDD_LOGIN,NULL);
	m_ExpDlg.Create(IDD_EXPDLG, this);
	m_PetDlg.Create(IDD_PET, this);
	m_ChatDlg.Create(IDD_CHAT, this);
	m_BoardDialog.Create(IDD_BOARD, this);

	COffLineApp* pApp = (COffLineApp*)AfxGetApp();

	pApp->m_pRoot->initRenderSystem((HWND)m_ExpDlg.m_expRenderWnd.m_hWnd);

	m_pExpRenderUnit = new RenderUnit(m_ExpDlg.m_expRenderWnd.m_hWnd, true);

	m_pPetRenderUnit = new RenderUnit(m_PetDlg.m_PetRenderWnd.m_hWnd);

	//����Ǵ���Ϸ����������ֱ��������Ϸ��������
	if( strcmp( pApp->m_lpCmdLine,"-direct" ) == 0 )
	{
		MudNetMgr::getSingleton().requestLoginServerDirect();
	}else{
		m_LoginDialog.ShowWindow(SW_SHOW);
		m_LoginDialog.BringWindowToTop();
		m_LoginDialog.SetForegroundWindow();
		m_LoginDialog.SetActiveWindow();
		MudNetMgr::getSingleton().requestDirInfo();
	}

	//��ʼ��ʱ�����еĴ���ȫ�������صġ�
	InitGUI();

	HideAll();

	SetTimer(1, 33, 0);
	SetTimer(2, 30000,0);


	m_pNid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_pNid.hWnd = m_hWnd;
	m_pNid.uID = IDR_MAINFRAME;
	m_pNid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	m_pNid.uCallbackMessage = WM_SHOWTASK;
	m_pNid.hIcon = m_hIcon;
	sprintf_s( m_szTip,"2012�һ�");

	return 0;
}

void COffLineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect clientRect;
	GetClientRect(clientRect);
	ClientToScreen(clientRect);

	m_ExpDlg.MoveWindow(clientRect.left + 15, clientRect.top + 15,
		cx - 30, (int)(cy*0.8f));
	m_ChatDlg.MoveWindow(clientRect.left + 15, clientRect.top + 15,
		cx - 30, (int)(cy*0.8f));
	m_PetDlg.MoveWindow(clientRect.left + 15, clientRect.top + 15,
		cx - 30, (int)(cy*0.8f));
	m_BoardDialog.MoveWindow(clientRect.left + 15, clientRect.top + 15,
		cx - 30, (int)(cy*0.8f));
}

void COffLineDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: Add your message handler code here
	CRect clientRect;
	GetClientRect(clientRect);
	ClientToScreen(clientRect);

	m_ExpDlg.MoveWindow(x + 15, y + 15, 
		clientRect.Width() - 30, int(clientRect.Height()*0.8f), true);
	m_ChatDlg.MoveWindow(x + 15, y + 15, 
		clientRect.Width() - 30, int(clientRect.Height()*0.8f), true);
	m_PetDlg.MoveWindow(x + 15, y + 15, 
		clientRect.Width() - 30, int(clientRect.Height()*0.8f), true);
	m_BoardDialog.MoveWindow(x + 15, y + 15, 
		clientRect.Width() - 30, int(clientRect.Height()*0.8f), true);
}

void COffLineDlg::InitGUI ()
{
	CDC*	pCDC	= GetDC();
	HDC		hdc		= pCDC->GetSafeHdc();

	LOAD_IMAGE(m_BackGround, "resource\\background.bmp");
	// ���ڴ�С
	//CRect clientRect;
	//GetClientRect(&clientRect);
	//clientRect.right = clientRect.left + m_BackGround.m_Size.cx;
	//clientRect.bottom = clientRect.top + m_BackGround.m_Size.cy;
	//MoveWindow(&clientRect, true);

	// 
}

void COffLineDlg::HideAll ()
{
	m_ExpDlg.ShowWindow(0);
	m_ChatDlg.ShowWindow(0);
	m_PetDlg.ShowWindow(0);
}

void COffLineDlg::OnBnClickedChat()
{
	if( MudNetMgr::getSingleton().getLoginRoleID() == -1 )
	{
		AfxMessageBox("���ȵ�½��Ϸ��");
		return;
	}
	m_ChatDlg.ShowWindow(1);
	m_ChatDlg.SetFocus();

	m_ExpDlg.ShowWindow(0);
	m_PetDlg.ShowWindow(0);
}

void COffLineDlg::OnBnClickedExp()
{
	// TODO: Add your control notification handler code here
	m_ExpDlg.ShowWindow(1);
	m_ExpDlg.SetFocus();

	m_ChatDlg.ShowWindow(0);
	m_PetDlg.ShowWindow(0);
}

void COffLineDlg::OnFinalRelease()
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_pExpRenderUnit)
		delete m_pExpRenderUnit;

	if (m_pPetRenderUnit)
		delete m_pPetRenderUnit;

	if (m_pSceneManager)
		delete m_pSceneManager;

	if (m_pRoot)
		delete m_pRoot;

	CDialog::OnFinalRelease();
}

void COffLineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (1 == nIDEvent)
	{
		m_pExpRenderUnit->update(33);
// 		m_pPetRenderUnit->update(33);
		if( m_ExpDlg.m_pEntity != NULL )
		{
			m_ExpDlg.m_pEntity->update(33);
		}		 

		if (Ogre::SceneManager::getSingletonPtr())
			Ogre::SceneManager::getSingleton().doFrame();
	}
	if( 2 == nIDEvent )
	{
		if( MudNetMgr::getSingleton().getLoginRoleID() != -1 )
		{
			MudNetMgr::getSingleton().requestHeartHeat();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void COffLineDlg::OnBnClickedPet()
{
	// TODO: Add your control notification handler code here
	//���ڻ�û������鶫��
	//m_PetDlg.ShowWindow(1);
	//m_PetDlg.SetFocus();

	m_ExpDlg.ShowWindow(0);
	m_ChatDlg.ShowWindow(0);
}

void COffLineDlg::OnBnClickedBoard()
{
	// TODO: Add your control notification handler code here
	m_BoardDialog.ShowWindow(1);
	m_BoardDialog.SetFocus();

	m_PetDlg.ShowWindow(0);
	m_ExpDlg.ShowWindow(0);
	m_ChatDlg.ShowWindow(0);
}

LRESULT COffLineDlg::onNetNotify(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case eRoleList:
		{
			OnBnClickedExp();
		}
		break;
	case eLoginRet:
		{
			switch( lParam )
			{
			case 0:
				{
					ToTray(FALSE);
					m_LoginDialog.ShowWindow(SW_HIDE);
					ShowWindow(SW_SHOW);
				}
				break;
			case 1:
				AfxMessageBox("account or role login fail because repeat!");
				break;
			case 2:
				AfxMessageBox("account or role login fail because server busy");
				break;
			case 3:
				AfxMessageBox("account login fail because client protolcol version not compatible");
				break;
			case 4:
				AfxMessageBox("account or role login fail because system ban the account or role");
				break;
			case 5:
				AfxMessageBox("account login fail because frequency limit");
				break;
			case 6:
				AfxMessageBox("server not available");
				break;
			case 16:
				AfxMessageBox("account or role login fail because other reason");
				break;
			}
		}
		break;
	case eRoleLogin:
		{
			OnBnClickedBoard();
		}
		break;

	case eError:
		{
			if( lParam == eErrorDirectLoginError )
			{
				::MessageBox(NULL,"��������ʧ�ܣ�","����",MB_OK);
				MudNetMgr::getSingleton().requestDirInfo();
				m_LoginDialog.ShowWindow(SW_SHOW);
				ShowWindow(SW_HIDE);
				m_LoginDialog.SetForegroundWindow();
			}else if( lParam == eErrorKickOff )
			{
				::MessageBox(NULL,"�㱻�������ˣ�","����",MB_OK);
				MudNetMgr::getSingleton().requestDirInfo();
				m_LoginDialog.ShowWindow(SW_SHOW);
				ShowWindow(SW_HIDE);
				m_LoginDialog.SetForegroundWindow();
			}
		}

	default:
		break;
	}
	return 1L;
}


void COffLineDlg::ToTray(BOOL bTray)
{
	strcpy_s(m_pNid.szTip, m_szTip);
	if(bTray)
	{
		Shell_NotifyIcon(NIM_ADD, &m_pNid);
		ShowWindow(SW_HIDE); 
	}
	else
	{
		Shell_NotifyIcon(NIM_DELETE, &m_pNid);
		ShowWindow(SW_RESTORE);

		BringWindowToTop();
		SetForegroundWindow();
		SetActiveWindow();
	}
}

LRESULT COffLineDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
	{
		return 1;
	}

	switch(lParam)
	{
	case WM_LBUTTONDBLCLK:
		{
			ToTray(FALSE);
		}
		break;
	case WM_RBUTTONUP:
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_TASK_RBTN);
			CMenu* pSubMenu = menu.GetSubMenu(0);
			CPoint pt;
			GetCursorPos(&pt);
			SetForegroundWindow();
 			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,pt.x,pt.y,this);
			menu.DestroyMenu();
		}
		break;
	}
	return 0;
}

void COffLineDlg::OnTrayQuit()
{
	Shell_NotifyIcon(NIM_DELETE, &m_pNid);
	PostQuitMessage(0);
}

void COffLineDlg::OnBnClickedStartgame()
{
	SHELLEXECUTEINFO info;
	memset(&info,0,sizeof(SHELLEXECUTEINFO));
	info.lpVerb = "runas";
	info.nShow = SW_SHOWNORMAL;
	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.lpFile = "client.exe";
	if( ShellExecuteEx(&info) )
	{
		MudNetMgr::getSingleton().requestLogout();
		PostQuitMessage(0);
	}
}

void COffLineDlg::OnCancel()
{
	if( strlen( MudNetMgr::getSingleton().m_UserName ) >0 )
	{
		MudNetMgr::getSingleton().requestLogout();
	}
	PostQuitMessage(0);
}
