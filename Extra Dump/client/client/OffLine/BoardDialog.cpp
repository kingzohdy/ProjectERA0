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
				 msg.Format("��ĺ���[%s]�����ˡ�\n",onlineInfo.RoleName);
			 }else if( onlineInfo.Online == 2 )
			 {
				 msg.Format("��ĺ���[%s]��ʼ�һ��ˡ�\n",onlineInfo.RoleName);
			 }else if( onlineInfo.Online == 1 )
			 {
				 msg.Format("��ĺ���[%s]����Ϸ�������ˡ�\n",onlineInfo.RoleName);
			 }
			 m_cstrBroadcastMsg.Append(msg);
			 UpdateData(FALSE);
		}
		break;
	case eRoleLogin:
		{
			m_cstrBroadcastMsg.Append("��ɫ��½�������ɹ���\n");
			UpdateData(FALSE);
		}
		break;
	}
	return 1L;
}