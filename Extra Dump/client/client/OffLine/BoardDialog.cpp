// BoardDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OffLine.h"
#include "BoardDialog.h"
#include "MudNetMgr.h"


// CBoardDialog dialog

IMPLEMENT_DYNAMIC(CBoardDialog, CDialog)

CBoardDialog::CBoardDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBoardDialog::IDD, pParent)
	, m_cstrBroadcastMsg(_T(""))
{

}

CBoardDialog::~CBoardDialog()
{
}

void CBoardDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT2_BROADMSG, m_cstrBroadcastMsg);
}


BEGIN_MESSAGE_MAP(CBoardDialog, CDialog)
	ON_WM_CREATE()
	ON_MESSAGE(WM_NET_NOTIFY,&CBoardDialog::onNetNotify)
END_MESSAGE_MAP()


// CBoardDialog message handlers

int CBoardDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	MudNetMgr::getSingleton().addMessageHwnd(GetSafeHwnd());

	return 0;
}

LRESULT CBoardDialog::onNetNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case eOnline:
		{
			//
			 CSONLINESVR onlineInfo = MudNetMgr::getSingleton().getOnlineInfo((int)lParam);
			 CString msg;
			 if( onlineInfo.Online == 0 )
			 {
				 msg.Format("你的好友[%s]下线了。\n",onlineInfo.RoleName);
			 }else if( onlineInfo.Online == 2 )
			 {
				 msg.Format("你的好友[%s]开始挂机了。\n",onlineInfo.RoleName);
			 }else if( onlineInfo.Online == 1 )
			 {
				 msg.Format("你的好友[%s]在游戏中上线了。\n",onlineInfo.RoleName);
			 }
			 m_cstrBroadcastMsg.Append(msg);
			 UpdateData(FALSE);
		}
		break;
	case eRoleLogin:
		{
			m_cstrBroadcastMsg.Append("角色登陆服务器成功！\n");
			UpdateData(FALSE);
		}
		break;
	}
	return 1L;
}