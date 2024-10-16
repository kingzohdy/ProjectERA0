#include "ChatRoomManager.h"
#include "MudNetMgr.h"
#include "cs_net.h"
#include "tinyxml.h"
#include "OgreXMLData.h"
#include "ServerTimer.h"
#include "../UILib/ui_gameui.h"
#include <MMSystem.h>
#include "OgreScriptLuaVM.h"
#include "OfflineManager.h"
#include "OfflineMainPlayer.h"
using namespace std;

ChatRoomManager::ChatRoomManager()
{
	INetworkManager::getSingleton().RegisterMsgHandler(MUD_ENTER_CHAT_ROOM_RES,this);
	INetworkManager::getSingleton().RegisterMsgHandler(MUD_CHAT_ROOM_LIST_RES,this);
	INetworkManager::getSingleton().RegisterMsgHandler(MUD_CHAT_ROOM_ENTER_INFO,this);
	INetworkManager::getSingleton().RegisterMsgHandler(MUD_CHAT_ROOM_LEAVE_INFO,this);
	m_ChatRoomPlayerList.clear();
	m_nupdatetime = 0;
	m_bIsConnnectChatroom = true;
}

int	ChatRoomManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	switch (msgid)
	{
	case MUD_CHAT_ROOM_ENTER_INFO:
		{
			PlayerEnterRoom(msgbody.MudChatRoomEnterInfo.RoleInfo);
			string strtemp=msgbody.MudChatRoomEnterInfo.RoleInfo.RoleName;
			strtemp=strtemp+"进入房间!";
			MudNetMgr::getSingleton().m_tiplist.push_back(strtemp);
			OfflineManager::getSingleton().SendUIEvent("GE_RELATION_UPDATE");
		}
		break;
	case MUD_CHAT_ROOM_LEAVE_INFO:
		{
			PlayerLeaveRoom(msgbody.MudChatRoomLeaveInfo.RoleName);
			string strtemp = msgbody.MudChatRoomEnterInfo.RoleInfo.RoleName;
			strtemp		   = strtemp + "离开房间!";
			char pszStr[50];
			sprintf( pszStr, "DeleteChatObject( \"%s\")", msgbody.MudChatRoomEnterInfo.RoleInfo.RoleName );
			OfflineManager::getSingleton().CallString( pszStr );
			MudNetMgr::getSingleton().m_tiplist.push_back( strtemp );
			OfflineManager::getSingleton().SendUIEvent("GE_RELATION_UPDATE");
		}
		break;
	case MUD_CHAT_ROOM_LIST_RES:
		{
			for (int i = 0; i < msgbody.MudChatRoomListRes.RoomNum;i++ )
			{
				m_nRoomPlayerNums[i] = msgbody.MudChatRoomListRes.RoomPlayerNum[i];
			}
			m_nRoomNums = msgbody.MudChatRoomListRes.RoomNum;
			OfflineManager::getSingleton().SendUIEvent("GE_ENTERCHARROOM_LIST2");
		}
		break;
	case  MUD_ENTER_CHAT_ROOM_RES:
		{
			m_bIsConnnectChatroom = true;
			if ( msgbody.MudEnterChatRoomRes.Result == 0)
			{
				initChatRoom( msgbody.MudEnterChatRoomRes );
				OfflineManager::getSingleton().SendUIEvent( "GE_ENTERCHARROOM" );
			}
		}
		break;
	}
	return 1;
}

void ChatRoomManager::PlayerEnterRoom(CSROOMROLEINFO t_ChatRoominfo)
{
	OfflinePlayer temp;
	temp.m_RoleID	=	t_ChatRoominfo.RoleID;
	temp.m_uCareer	=	t_ChatRoominfo.Career;
	temp.m_uGender	=	t_ChatRoominfo.Gender;
	temp.m_uLevel	=	t_ChatRoominfo.Level;
	temp.m_Onlinetype = 2;
	strcpy( temp.m_RoleName, t_ChatRoominfo.RoleName );
	m_ChatRoomPlayerList.push_back(temp);
}

void ChatRoomManager::PlayerEnterRoom( const CSMUDROLEDETAIL& roleDetail,const char* name )
{
	OfflinePlayer temp;
	temp.m_RoleID	=	roleDetail.RoleID;
	temp.m_uCareer	=	roleDetail.Career;
	temp.m_uGender	=	roleDetail.Gender;
	temp.m_uLevel	=	roleDetail.Level;
	temp.m_Onlinetype = 2;
	strcpy( temp.m_RoleName, name );
	m_ChatRoomPlayerList.push_back(temp);
}

int	ChatRoomManager::getCurrentRoomrolenums()
{
	return m_ChatRoomPlayerList.size();
}

void ChatRoomManager::initChatRoom(CSMUDENTERCHATROOMRES MudEnterChatRoomRes)
{
	m_ChatRoomPlayerList.clear();
	PlayerEnterRoom( OfflineMainPlayer::getSingleton().getRoleDetail(),OfflineMainPlayer::getSingleton().getName() );
	for (int i = 0; i < MudEnterChatRoomRes.ChatRoomInfo.PlayerNum; i++)
	{
		PlayerEnterRoom( MudEnterChatRoomRes.ChatRoomInfo.ChatRoomRoleInfo[i] );
	}
	m_nCurrenRoomIndex = MudEnterChatRoomRes.ChatRoomInfo.ChatRoomIdx;
}

void ChatRoomManager::PlayerLeaveRoom( const char* szName )
{
	for ( vector<OfflinePlayer>::iterator Chat_bit = m_ChatRoomPlayerList.begin(); Chat_bit != m_ChatRoomPlayerList.end(); Chat_bit++)
	{
		if ( strcmp( Chat_bit->m_RoleName, szName ) == 0 )
		{
			m_ChatRoomPlayerList.erase( Chat_bit );
			break;
		}
	}
}

int ChatRoomManager::getCurrentChatRoomIndex()
{
	return m_nCurrenRoomIndex;
}

void ChatRoomManager::requestLeaveRoom()
{

}

void ChatRoomManager::requestEnterRoom(int index)
{
	tagCSPkgBody msgbody;
	msgbody.MudEnterChatRoomReq.ChatRoomIdx	= index;
	OfflineSendMessage(MUD_ENTER_CHAT_ROOM_REQ,msgbody);
	m_bIsConnnectChatroom = false;
}

OfflinePlayer* ChatRoomManager::getPlayerInfo(int index)
{
	if ( m_ChatRoomPlayerList.size() > 0 && index > 0)
	{
		return &m_ChatRoomPlayerList[index-1];
	}
	return NULL;
}

void	ChatRoomManager::clear()
{
	m_ChatRoomPlayerList.clear();
}

int ChatRoomManager::getChatRoomNums()
{
	return m_nRoomNums;
}

int ChatRoomManager::getRoomPlayerNums(int i)
{
	return m_nRoomPlayerNums[i-1];
}

ChatRoomManager::~ChatRoomManager()
{

}
void	ChatRoomManager::update(unsigned int dtick)
{
	if (OfflineMainPlayer::getSingleton().getGameState() == ROOMLIST_STATE )
	{
		m_nupdatetime = m_nupdatetime + dtick;
		if ( m_nupdatetime > 20000 )
		{
			OfflineMainPlayer::getSingleton().requestRoomList();
			m_nupdatetime = 0;
		}
	}
}

bool ChatRoomManager::IsInChatRoom(std::string szName)
{
	for (vector<OfflinePlayer>::iterator iter = m_ChatRoomPlayerList.begin(); iter != m_ChatRoomPlayerList.end(); iter++)
	{
		if ( strcmp( szName.c_str(), iter->m_RoleName ) == 0)
		{
			return true;
		}
	}
	return false;
}
