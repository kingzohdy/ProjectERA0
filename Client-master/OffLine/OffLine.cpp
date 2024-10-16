// OffLine.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "OffLine.h"
#include "OffLineDlg.h"
#include "LoginDialog.h"
#include "MudNetMgr.h"
#include "GameDBData.h"

#include <OgreRoot.h>
#include <OgreXMLData.h>
#include "tinyxml.h"

using namespace Ogre;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COffLineApp

BEGIN_MESSAGE_MAP(COffLineApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COffLineApp 构造

COffLineApp::COffLineApp()
	:m_pDBMgr(NULL),
	m_pRoot(NULL),
	m_NetMgr(NULL)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COffLineApp 对象

COffLineApp theApp;


// COffLineApp 初始化

BOOL COffLineApp::InitInstance()
{
	AfxInitRichEdit2();
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

	AfxEnableControlContainer();

	
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	m_pRoot = new Ogre::Root("offline.cfg");
	m_NetMgr = MudNetMgr::Create( &m_pRoot->m_Config );
	m_pDBMgr = new DBDataManager;
	if( !m_pDBMgr->LoadData( m_NetMgr->m_config ) )
	{
		AfxMessageBox("加载数据出错！");
		DeleteFile("mudTmp");
		return FALSE;
	}

	m_NetMgr->startDispatch();

	LoadLocalPlayerInfo("offlinepalyerlist.xml");

	m_OffLineDlg = new COffLineDlg;
	m_OffLineDlg->Create(COffLineDlg::IDD,NULL);
	m_pMainWnd = m_OffLineDlg;
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return TRUE;
}

BOOL COffLineApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::OnIdle(lCount);
}

void COffLineApp::LoadLocalPlayerInfo (std::string fileName)
{
	m_PlayerList.loadFile(fileName);
	m_PlayerListRootNode = m_PlayerList.getRootNode();

	if (m_PlayerListRootNode.isNull())
		return;

	XMLNode lastPlayerNode = m_PlayerListRootNode.getChild("LastPlayer");
	m_LastUserName = lastPlayerNode.attribToString("name");

	XMLNode playerListNode = m_PlayerListRootNode.getChild("Playerlist");
	XMLNode playerNode = playerListNode.getChild("Player");
	while (!playerNode.isNull())
	{
		LocalPlayerInfo playerIno;
		playerIno.playerName = playerNode.attribToString("name");
		playerIno.lastServer = playerNode.attribToString("lastserver");
		playerIno.lastworld = playerNode.attribToString("lastworld");
		m_LocalPlayInfoList.push_back(playerIno);

		playerNode = playerListNode.iterateChild(playerNode);
	}
}

void COffLineApp::SaveLocalPlayerInfo (std::string fileName)
{
	if (m_LastUserName == "")
		return;
	if( m_PlayerListRootNode.isNull() )
	{
		return;
	}

	XMLNode lastPlayerNode = m_PlayerListRootNode.getChild("LastPlayer");
	lastPlayerNode.setAttribStr("name", m_LastUserName.c_str());

	// 重新计算新的Playerlist
	bool bIn = false;
	XMLNode playerListNode = m_PlayerListRootNode.getChild("Playerlist");
	for (int i=0; i<(int)m_LocalPlayInfoList.size(); i++)
	{
		if (m_LastUserName == m_LocalPlayInfoList[i].playerName)
		{
			bIn = true;
			break;
		}
	}

	if (!bIn)
	{
		TiXmlElement *element = new TiXmlElement("Player");
		element->SetAttribute("name", m_LastUserName.c_str());
		element->SetAttribute("lastserver", m_LastServer.c_str());
		element->SetAttribute("lastworld", m_LastWorld.c_str());

		playerListNode.m_pElement->InsertEndChild(*element);
	}

	m_PlayerList.saveFile(fileName);
}

void COffLineApp::addRichString(CRichEditCtrl* pCtrl,CString text,COLORREF color)
{
	if( pCtrl == NULL )
	{
		return;
	}
	HWND hWnd = pCtrl->m_hWnd;
	if (hWnd == NULL || !::IsWindow(hWnd))
		return;

	int nLen = (int)::SendMessage(hWnd, WM_GETTEXTLENGTH, NULL, NULL);
	CHARRANGE cr;
	cr.cpMin = -1;
	cr.cpMax = -1;
	::SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
	CHARFORMAT2 cf;
	memset(&cf,0,sizeof(CHARFORMAT2));
	cf.dwMask = CFM_COLOR | CFM_CHARSET  ;
	cf.bCharSet = GB2312_CHARSET;
	cf.crTextColor = color;
	cf.cbSize = sizeof(CHARFORMAT2);
	::SendMessage(hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	::SendMessage(hWnd, EM_REPLACESEL, (WPARAM) 0, (LPARAM)text.GetBuffer() );

	//滚到最后一行
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_ALL;
	::GetScrollInfo(hWnd, SB_VERT, &ScrollInfo);

	int nTotalLine = (int)::SendMessage(hWnd, EM_GETLINECOUNT, 0, 0);
	int nUpLine = 0 ;
	if(nTotalLine > 0 && ScrollInfo.nMax > 0 && ScrollInfo.nMax / nTotalLine > 0)
		nUpLine = (ScrollInfo.nMax - ScrollInfo.nPos - (ScrollInfo.nPage - 1)) / (ScrollInfo.nMax / nTotalLine);
	if(nUpLine > 0)
		::SendMessage(hWnd, EM_LINESCROLL, 0, nUpLine);
}
int COffLineApp::ExitInstance()
{
	if( m_pDBMgr != NULL )
	{
		delete m_pDBMgr;
		m_pDBMgr = NULL;
	}
	if( m_NetMgr != NULL )
	{
		delete m_NetMgr;
		m_NetMgr = NULL;
	}
	
	return CWinApp::ExitInstance();
}
