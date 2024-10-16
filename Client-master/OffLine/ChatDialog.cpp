// ChatDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OffLine.h"
#include "ChatDialog.h"
#include "MudNetMgr.h"


// CChatDialog dialog

IMPLEMENT_DYNAMIC(CChatDialog, CDialog)

CChatDialog::CChatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDialog::IDD, pParent)
	, m_cstrSendMsg(_T(""))
	, m_cstrToName(_T(""))
{

}

CChatDialog::~CChatDialog()
{
}

void CChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SENDMSG, m_cstrSendMsg);;
	DDX_Control(pDX, IDC_TREE_PLAYER, m_PlayerTree);
	DDX_Text(pDX, IDC_EDIT_PRIVATE_NAME, m_cstrToName);
	DDX_Control(pDX, IDC_RICHEDIT2_SHOWMSG, m_ShowRichCtrl);
}


BEGIN_MESSAGE_MAP(CChatDialog, CDialog)
	ON_WM_CREATE()
	ON_MESSAGE(WM_NET_NOTIFY,&CChatDialog::onNetNotify)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatDialog::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CChatDialog::OnBnClickedButtonClear)
	ON_COMMAND(ID_PRIVATE, &CChatDialog::OnPrivate)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_PLAYER, &CChatDialog::OnNMRclickTreePlayer)
	ON_BN_CLICKED(IDC_BUTTON_CLEARTONAME, &CChatDialog::OnBnClickedButtonCleartoname)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PLAYER, &CChatDialog::OnNMClickTreePlayer)
END_MESSAGE_MAP()


// CChatDialog message handlers

int CChatDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	MudNetMgr::getSingleton().addMessageHwnd(GetSafeHwnd());

	return 0;
}

LRESULT CChatDialog::onNetNotify(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case eRoleLogin:
		{
			while( m_PlayerTree.GetChildItem(m_RoomParentItem) != NULL )
			{
				m_PlayerTree.DeleteItem(m_PlayerTree.GetChildItem(m_RoomParentItem));
			}
			int nRoomPlayerNum = MudNetMgr::getSingleton().getChatRoomNum();
			HTREEITEM item = NULL;
			for( int i = 0 ; i < nRoomPlayerNum ; i++ )
			{
				CSROOMROLEINFO& info = MudNetMgr::getSingleton().getRoomRoleInfo(i);
				item = m_PlayerTree.InsertItem(info.RoleName,0,0,m_RoomParentItem);
				m_PlayerTree.SetItemData(item,(DWORD_PTR)&info);
			}
		}
		break;
	case eChatMsg:
		{
			CSCHATRES* pRes = (CSCHATRES*)lParam;
			CString msg;
			if( pRes->Type == CHAT_PRIVATE )
			{
				msg.Format("[%s]悄悄地对你说：%s\n",pRes->From,pRes->Content);
				theApp.addRichString(&m_ShowRichCtrl,msg,RGB(218,109,250));
			}else if( pRes->Type == CHAT_FRIEND )
			{
				msg.Format("好友[%s]对你说：%s\n",pRes->From,pRes->Content);
				theApp.addRichString(&m_ShowRichCtrl,msg,RGB(218,109,250));
			}else if( pRes->Type == CHAT_CLAN )
			{
				msg.Format("工会 [%s]：%s\n",pRes->From,pRes->Content);
				theApp.addRichString(&m_ShowRichCtrl,msg,RGB(122,211,43));
			}
			else{
				msg.Format("[%s]：%s\n",pRes->From,pRes->Content);
				theApp.addRichString(&m_ShowRichCtrl,msg,RGB(0,0,0));
			}
			delete pRes;
		}
		break;
	case eOnline:
	case eRelation:
		{					
			while( m_PlayerTree.GetChildItem( m_FriendParentItem ) != NULL )
			{
				m_PlayerTree.DeleteItem( m_PlayerTree.GetChildItem( m_FriendParentItem ) );
			}
			int nFriendNum = MudNetMgr::getSingleton().getFriendNum();
			for( int i = 0 ; i < nFriendNum ; i++ )
			{
				FriendInfo& info = MudNetMgr::getSingleton().getFriendInfo(i);
				HTREEITEM item = NULL;
				if( info.iOnline > 0 )
				{
					item = m_PlayerTree.InsertItem(info.roleName,1,1,m_FriendParentItem);
				}else{
					item = m_PlayerTree.InsertItem(info.roleName,2,2,m_FriendParentItem);
				}
				m_PlayerTree.SetItemData(item,(DWORD_PTR)&info);
			}
		}
		break;
	case eLeaveRoom:
	case eEnterRoom:
		{
			while( m_PlayerTree.GetChildItem(m_RoomParentItem) != NULL )
			{
				m_PlayerTree.DeleteItem(m_PlayerTree.GetChildItem(m_RoomParentItem));
			}
			HTREEITEM item = NULL;
			int nRoomPlayerNum = MudNetMgr::getSingleton().getChatRoomNum();
			for( int i = 0 ; i < nRoomPlayerNum ; i++ )
			{
				CSROOMROLEINFO& info = MudNetMgr::getSingleton().getRoomRoleInfo(i);
				item = m_PlayerTree.InsertItem(info.RoleName,0,0,m_RoomParentItem);
				m_PlayerTree.SetItemData(item,(DWORD_PTR)&info);
			}
			CString msg;
			if( wParam == eLeaveRoom )
			{
				msg.Format("[%s]已离开房间！\n",(char*)lParam);
			}else{
				msg.Format("[%s]进入房间！\n",(char*)lParam);
			}
			theApp.addRichString(&m_ShowRichCtrl,msg,RGB(208,108,179));
			delete (char*)lParam;
			UpdateData(FALSE);
		}
		break;
	default:
		break;
	}
	return 1L;
}

void CChatDialog::OnBnClickedButtonSend()
{
	UpdateData(TRUE);
	if( MudNetMgr::getSingleton().getLoginRoleID() == -1 )
	{
		AfxMessageBox("请先登陆游戏！");
	}
	if( m_cstrSendMsg.IsEmpty() )
	{
		return;
	}
	if( !m_cstrToName.IsEmpty() )
	{
		CString msg;
		msg.Format("你对[%s]悄悄地说：%s\n",m_cstrToName.GetBuffer(),m_cstrSendMsg.GetBuffer());
		theApp.addRichString(&m_ShowRichCtrl,msg,RGB(218,109,250));		
	}
	MudNetMgr::getSingleton().requestSendChat(m_cstrToName.GetBuffer(),m_cstrSendMsg.GetBuffer(),m_bIsFriend);
	m_cstrSendMsg.Empty();
	UpdateData(FALSE);
}

BOOL CChatDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SENDMSG);
	pEdit->SetLimitText(CLT_MSG_LEN);

	m_RoomParentItem = m_PlayerTree.InsertItem("房间列表",-1,-1);
	m_PlayerTree.SetIndent(0);
	m_FriendParentItem = m_PlayerTree.InsertItem("好友列表",-1,-1);

	m_ImageList.Create(IDB_BITMAP_ONLINE,16,0,RGB(255,0,255));
	m_PlayerTree.SetImageList(&m_ImageList,TVSIL_NORMAL);

	m_ShowRichCtrl.SetEventMask(ENM_LINK);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CChatDialog::OnBnClickedButtonClear()
{
	m_cstrSendMsg.Empty();
	m_ShowRichCtrl.SetWindowText("");
	UpdateData(FALSE);
}

void CChatDialog::OnPrivate()
{
	HTREEITEM pSelItem = m_PlayerTree.GetSelectedItem();
	if( m_PlayerTree.GetParentItem(pSelItem) == m_RoomParentItem )
	{
		CSROOMROLEINFO* info = (CSROOMROLEINFO*) m_PlayerTree.GetItemData(pSelItem);
		CSMUDROLEDETAIL& detail = MudNetMgr::getSingleton().getRoleDetail();
		if( info->RoleID == detail.RoleID )
		{
			AfxMessageBox("不能和自己私聊！");
			m_cstrToName.Empty();
		}else{
			m_cstrToName = info->RoleName;
		}
	}else if( m_PlayerTree.GetParentItem(pSelItem) == m_FriendParentItem )
	{
		FriendInfo* info = (FriendInfo*) m_PlayerTree.GetItemData(pSelItem);
		if( !info->iOnline )
		{
			AfxMessageBox("不能对离线好友发起私聊");
			m_cstrToName.Empty();
		}else{
			m_cstrToName = m_PlayerTree.GetItemText(pSelItem);
			m_bIsFriend = true;
		}
	}
	UpdateData(FALSE);
}

void CChatDialog::OnNMRclickTreePlayer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	POINT pos ;
	GetCursorPos(&pos);
	m_PlayerTree.ScreenToClient(&pos);

	UINT uFlags;
	HTREEITEM hSelItem = m_PlayerTree.HitTest(pos, &uFlags);
	if( (hSelItem != NULL) && (TVHT_ONITEM & uFlags) )
	{
		m_PlayerTree.Select(hSelItem, TVGN_CARET);
		HTREEITEM pSelItem = m_PlayerTree.GetSelectedItem();
		if( pSelItem != NULL )
		{
			if( m_PlayerTree.GetParentItem(pSelItem) == m_RoomParentItem ||
				m_PlayerTree.GetParentItem(pSelItem) == m_FriendParentItem )
			{
				CMenu menu;
				menu.LoadMenu(IDR_MENU_RBN);
				CMenu* pSub = menu.GetSubMenu(0);
				POINT pt;
				GetCursorPos(&pt);
				pSub->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,pt.x,pt.y,this);
				menu.DestroyMenu();
			}
		}
	}
	*pResult = 0;
}

void CChatDialog::OnBnClickedButtonCleartoname()
{
	m_cstrToName.Empty();
	m_bIsFriend = false;
	UpdateData(FALSE);
}


void CChatDialog::OnNMClickTreePlayer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	POINT pos ;
	GetCursorPos(&pos);
	m_PlayerTree.ScreenToClient(&pos);

	UINT uFlags;
	HTREEITEM hSelItem = m_PlayerTree.HitTest(pos, &uFlags);
	if( (hSelItem != NULL) && (TVHT_ONITEM & uFlags) )
	{
		m_PlayerTree.Select(hSelItem, TVGN_CARET);
		HTREEITEM pSelItem = m_PlayerTree.GetSelectedItem();
		if( m_PlayerTree.GetParentItem(pSelItem) == m_RoomParentItem ||
			m_PlayerTree.GetParentItem(pSelItem) == m_FriendParentItem )
		{
			if( m_PlayerTree.GetParentItem(pSelItem) == m_RoomParentItem )
			{
				CSROOMROLEINFO* info = (CSROOMROLEINFO*) m_PlayerTree.GetItemData(pSelItem);
				CSMUDROLEDETAIL& detail = MudNetMgr::getSingleton().getRoleDetail();
				if( info->RoleID == detail.RoleID )
				{
					AfxMessageBox("不能和自己私聊！");
					m_cstrToName.Empty();
				}else{
					m_cstrToName = info->RoleName;
				}
			}else if( m_PlayerTree.GetParentItem(pSelItem) == m_FriendParentItem )
			{
				FriendInfo* info = (FriendInfo*) m_PlayerTree.GetItemData(pSelItem);
				if( !info->iOnline )
				{
					AfxMessageBox("不能对离线好友发起私聊");
					m_cstrToName.Empty();
				}else{
					m_cstrToName = m_PlayerTree.GetItemText(pSelItem);
					m_bIsFriend = true;
				}
			}
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}


BOOL CChatDialog::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnBnClickedButtonSend();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
