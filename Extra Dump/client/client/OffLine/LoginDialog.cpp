// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OffLine.h"
#include "LoginDialog.h"
#include "OffLine.h"


// CLoginDialog dialog

IMPLEMENT_DYNAMIC(CLoginDialog, CDialog)

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
	, m_cstrPassWorld(_T(""))
	, m_UserName(_T(""))
{
	m_bExpand = true;
}

CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVERLIST, m_ServerList);
	DDX_Control(pDX, IDC_WORLDLIST, m_WorldList);
	DDX_Text(pDX, IDC_PASSWORD, m_cstrPassWorld);
	DDX_Control(pDX, IDC_SPLITTER, m_wndSplitter);
	DDX_CBString(pDX, IDC_USERNAME, m_UserName);
	DDX_Control(pDX, IDC_USERNAME, m_UserList);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	ON_BN_CLICKED(IDC_OK, &CLoginDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_UPDATE, &CLoginDialog::OnBnClickedUpdate)
	ON_MESSAGE(WM_NET_NOTIFY,&CLoginDialog::onNetNotify)
	ON_BN_CLICKED(IDC_EXPAND, &CLoginDialog::OnBnClickedExpand)
	ON_CBN_SELCHANGE(IDC_SERVERLIST, &CLoginDialog::OnCbnSelchangeServerlist)
	ON_CBN_SELCHANGE(IDC_USERNAME, &CLoginDialog::OnCbnSelchangeUsername)
END_MESSAGE_MAP()


// CLoginDialog message handlers
void CLoginDialog::OnBnClickedOk()
{
	UpdateData(true);	
	
	int nServerIndex = m_ServerList.GetCurSel();
	int nWorldIndex = m_WorldList.GetCurSel();

	if( nServerIndex == -1 || nWorldIndex == - 1 )
	{
		AfxMessageBox("请选择服务器！");
		return;
	}

	if( m_UserName.IsEmpty() )
	{
		AfxMessageBox("请输入账号！");
		return;
	}
	
	CString serverName;
	m_ServerList.GetLBText(nServerIndex,serverName);
	CString worldName;
	m_WorldList.GetLBText(nWorldIndex,worldName);		


	m_LastUserName = m_UserName.GetBuffer();
	m_LastServerName = serverName.GetBuffer();
	m_LastWorldName = worldName.GetBuffer();

	DirBigWorldInfo& bigWolrd = MudNetMgr::getSingleton().getBidWorldInfo(nServerIndex);
	DirWorldInfo&	worldInfo = bigWolrd.getIthWorld(nWorldIndex);
	MudNetMgr::getSingleton().requestLoginGameServer(worldInfo.mudIp,
													worldInfo.mudPort,
													m_UserName,
													m_cstrPassWorld.GetBuffer(),
													worldInfo.worldid,
													worldInfo.name);
}

void CLoginDialog::OnBnClickedUpdate()
{
	MudNetMgr::getSingleton().requestDirInfo();
}

BOOL CLoginDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	MudNetMgr::getSingleton().addMessageHwnd(GetSafeHwnd());

	std::string lastUserName = theApp.m_LastUserName;

	for (int i=0; i<(int)theApp.m_LocalPlayInfoList.size(); i++)
	{
		std::string name = theApp.m_LocalPlayInfoList[i].playerName;
		std::string server = theApp.m_LocalPlayInfoList[i].lastServer;
		std::string world = theApp.m_LocalPlayInfoList[i].lastworld;
		m_UserList.InsertString(i, name.c_str());

		// 设置默认值
		if (lastUserName == name)
		{ 
			m_UserList.SetCurSel(i);
			m_DefServerName = server;
			m_DefWorldName = world;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CLoginDialog::onNetNotify(WPARAM wParma,LPARAM lParam)
{
	switch( wParma )
	{
	case eUpdateDirList:
		{
			int nBigWorldNum = MudNetMgr::getSingleton().getBigWorldNum();
			m_WorldList.ResetContent();
			m_ServerList.ResetContent();
			for( int i = 0 ; i < nBigWorldNum ; i++)
			{
				DirBigWorldInfo& info = MudNetMgr::getSingleton().getBidWorldInfo(i);
				int index = m_ServerList.AddString( info.getName() );
				if (info.getName() == m_DefServerName)
				{
					m_ServerList.SetCurSel(index);
					OnCbnSelchangeServerlist();
				}				
			}
		}
		break;
	case eError:
		{
			if( lParam == eErrorGetDirError )
			{
				AfxMessageBox("获取服务器列表出错！");
			}else if( lParam == eErrorLoginError )
			{
				ShowWindow(SW_SHOW);
				AfxMessageBox("登陆出错！");
			}else if( lParam == eErrorLoginRepeat )
			{
				ShowWindow(SW_SHOW);
				AfxMessageBox("请不要短时间内连续登陆！");
			}
		}
		break;

	default:
		break;
	}
	return 1l;
}

void CLoginDialog::OnBnClickedExpand()
{
	static bool bGeted = false;
	if (!bGeted)
	{
		bGeted = true;
		GetWindowRect(&m_OriginRect);
	}

	if (m_bExpand)
	{
		CRect rectLogin;
		GetWindowRect(&rectLogin);

		CRect splitterWnd;
		::GetWindowRect(m_wndSplitter.m_hWnd, &splitterWnd);

		rectLogin.bottom = splitterWnd.top;
		GetDlgItem(IDC_EXPAND)->SetWindowText("∨");
		MoveWindow(rectLogin);

		m_bExpand = false;
	}
	else
	{
		GetDlgItem(IDC_EXPAND)->SetWindowText("∧");
		MoveWindow(m_OriginRect);

		m_bExpand = true;
	}
}

void CLoginDialog::OnCbnSelchangeServerlist()
{
	m_WorldList.ResetContent();
	int nIndex = m_ServerList.GetCurSel();
	if( nIndex >= 0 )
	{
		DirBigWorldInfo& info = MudNetMgr::getSingleton().getBidWorldInfo(nIndex);
		for( int i = 0 ; i < info.getNumWorld(); i++ )
		{
			int index = m_WorldList.AddString( info.getIthWorld(i).name );

			if (info.getIthWorld(i).name == m_DefWorldName)
			{
				m_WorldList.SetCurSel(index);
			}
		}
	}
}

void CLoginDialog::OnOK()
{
	OnBnClickedOk();
}

void CLoginDialog::OnCbnSelchangeUsername()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	std::string strUserName = m_UserName.GetBuffer(m_UserName.GetLength());

	for (int i=0; i<(int)theApp.m_LocalPlayInfoList.size(); i++)
	{
		std::string name = theApp.m_LocalPlayInfoList[i].playerName;
		std::string server = theApp.m_LocalPlayInfoList[i].lastServer;
		std::string world = theApp.m_LocalPlayInfoList[i].lastworld;

		// 设置默认值
		if (strUserName == name)
		{ 
			m_ServerList.SelectString(0,server.c_str());
			OnCbnSelchangeServerlist();
			m_WorldList.SelectString(0,world.c_str());
		}
	}
}

void CLoginDialog::OnCancel()
{
	CDialog::OnCancel();
	PostQuitMessage(0);
}
