#include "OfflineMainPlayer.h"
#include "MudNetMgr.h"
#include "OfflineManager.h"
#include "RelationManager.h"
#include "ChatRoomManager.h"
#include "FlickerManager.h"
using namespace std;
OfflineMainPlayer::OfflineMainPlayer()
{
	INetworkManager::getSingleton().RegisterMsgHandler(ROLE_LIST,this);
	INetworkManager::getSingleton().RegisterMsgHandler(MUD_ROLE_LOGIN_RES,this);
	INetworkManager::getSingleton().RegisterMsgHandler(MUD_ROLE_ATTR_CHG,this);
	INetworkManager::getSingleton().RegisterMsgHandler(RELATION_OP_TYPE_EMOTION_RES,this);
	INetworkManager::getSingleton().RegisterMsgHandler(MUD_OUT_CHAT_ROOM_RES,this);
	m_SelectRoleindex = 1;
}

int OfflineMainPlayer::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	switch (msgid)
	{
	case ROLE_LIST:
		{
			MudNetMgr::getSingleton().m_nOffline = false;
			m_RoleList = msgbody.RoleList;
			OfflineManager::getSingleton().SendUIEvent("GE_ROLELIST_DIR");
			if ( MudNetMgr::getSingleton().m_bStartfb )
			{
				EnterWorldFromZone();
				MudNetMgr::getSingleton().m_bStartfb=false;
			}
		}
		break;
	case MUD_ROLE_LOGIN_RES:
		{
			m_RoleDetail = msgbody.MudRoleLoginRes.RoleDetail;
			m_OfflineExp= msgbody.MudRoleLoginRes.OfflineExp;
			m_MaxOfflineExp= msgbody.MudRoleLoginRes.MaxOfflineExp;
			ChatRoomManager::getSingleton().setCurrentChatroomindex(-1);//初始状态为不在任何聊天室
			OfflineManager::getSingleton().SendUIEvent("GE_ENTERCHARROOM_LIST1");
			RelationManager::getSingleton().clear();
		}
		break;
	case MUD_ROLE_ATTR_CHG:
		{
			if( msgbody.MudRoleAttrChg.AttrType == MUD_ROLE_OFFLINE_EXP )
			{
				onRoleOffexpChangeRespose(msgbody.MudRoleAttrChg.RoleID,
					msgbody.MudRoleAttrChg.AttrData.RoleOfflineExp.OfflineExp);
			}
		}
		break;
	case MUD_OUT_CHAT_ROOM_RES:
		{
			if( msgbody.MudOutChatRoomRes.Result == 0 )
			{
				OfflineManager::getSingleton().SendUIEvent("SHOW_ROOM_LIST");
			}
		}
		break;
	}
	return 0;
}

void OfflineMainPlayer::onRoleOffexpChangeRespose(tdr_ulonglong RoleID,int offexp)
{
	if ( m_RoleDetail.RoleID == RoleID )
	{
		m_OfflineExp=offexp;
		OfflineManager::getSingleton().SendUIEvent("GE_EXP_UPATE");
	}
}

void OfflineMainPlayer::setEmotion(const char* szEmotion)
{
	strncpy( m_eMotion, szEmotion,strlen( szEmotion ) + 1);
}

const char*		OfflineMainPlayer::getEmotion()
{
	return m_eMotion;
}

const char*		OfflineMainPlayer::getName()
{
	return m_RoleList.RoleList[m_SelectRoleindex-1].RoleName;
}

int OfflineMainPlayer::getLevel()
{
	return m_RoleDetail.Level;
}

int	OfflineMainPlayer::getCareer()
{
	return m_RoleDetail.Career;
}

int	OfflineMainPlayer::getGender()
{
	return m_RoleDetail.Gender;
}

int OfflineMainPlayer::getOfflineExp()
{
	return m_OfflineExp;
}

int OfflineMainPlayer::getMaxOfflineExp()
{
	return m_MaxOfflineExp;
}

int OfflineMainPlayer::getHead()
{
	return m_RoleDetail.Head;
}

int OfflineMainPlayer::getHair()
{
	return m_RoleDetail.Hair;
}

int OfflineMainPlayer::getFace()
{
	return m_RoleDetail.Face;
}
tdr_longlong OfflineMainPlayer::getRoleId()
{
	return m_RoleDetail.RoleID;
}

int	OfflineMainPlayer::getRoleListNums()
{
	return m_RoleList.RoleNum;
}

const char* OfflineMainPlayer::getRoleName(int i)
{
	if ( m_RoleList.RoleNum > 0 )
	{
		return m_RoleList.RoleList[i-1].RoleName;
	}
	return NULL;
}

int	OfflineMainPlayer::getRoleLevel(int i)
{
	if ( m_RoleList.RoleNum > 0 )
	{
		return m_RoleList.RoleList[i-1].Level;
	}
	return 0;
}

int OfflineMainPlayer::getRoleCareer(int i)
{
	if ( m_RoleList.RoleNum > 0 )
	{
		return m_RoleList.RoleList[i-1].Career;
	}
	return 0;
}

void OfflineMainPlayer::setSelectRoleIndex(int i)
{
	m_SelectRoleindex = i;
}

int OfflineMainPlayer::getSelectRoleIndex()
{
	return m_SelectRoleindex;
}

int OfflineMainPlayer::getOffExplength()
{
	float temp=(float)m_OfflineExp /(float)m_MaxOfflineExp;
	temp=temp*414;
	return (int)temp;
}
void OfflineMainPlayer::requestEnterGameMudWorld()
{
	tagCSPkgBody msgbody;
	msgbody.MudRoleLoginReq.RoleID = m_RoleList.RoleList[m_SelectRoleindex-1].RoleID;
	OfflineSendMessage(MUD_ROLE_LOGIN_REQ,msgbody);
}

void OfflineMainPlayer::requestRoomList()
{
	tagCSPkgBody msgbody;
	msgbody.MudRoleLoginReq.RoleID = m_RoleList.RoleList[m_SelectRoleindex-1].RoleID;
	OfflineSendMessage(MUD_CHAT_ROOM_LIST_REQ,msgbody);
}

void OfflineMainPlayer::requestEmotion(const char* szEmotion)
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_EMOTION_REQ;
	strncpy(msgbody.RelationClt.RelationCltData.EmotionReq.Emotion,szEmotion,strlen( szEmotion ) + 1);
	OfflineSendMessage( RELATION_CLT, msgbody );
}

void OfflineMainPlayer::requestRoleLogout()
{
	tagCSPkgBody msgbody;
	msgbody.MudRoleLogoutReq.RoleID = m_RoleDetail.RoleID;
	OfflineSendMessage( MUD_ROLE_LOGOUT_REQ, msgbody);
}

void OfflineMainPlayer::requestStealexp(tdr_ulonglong RoleID,const char* szName)
{
	tagCSPkgBody msgbody;
	msgbody.MudOfflineExpTakeReq.RoleID=RoleID;
	strncpy(msgbody.MudOfflineExpTakeReq.RoleName,szName,sizeof(msgbody.MudOfflineExpTakeReq.RoleName));
	OfflineSendMessage( MUD_OFFLINE_EXP_TAKE_REQ, msgbody);
}

void OfflineMainPlayer::EnterWorldFromZone()
{
	for (size_t i = 0;i< m_RoleList.RoleNum; i++)
	{
		if ( m_RoleList.RoleList[i].RoleID == MudNetMgr::getSingleton().m_DirectloginRoleID )
		{
			m_SelectRoleindex  = i+1;
			break;
		}
	}
	requestEnterGameMudWorld();
}

void OfflineMainPlayer::setGameState(int i)
{
	m_GameState = i;
	if ( i == LOGIN_STATE ) 
	{
		FlickerManager::getSingleton().changeico( HICON_LOGIN );
	}
	else
	{
		FlickerManager::getSingleton().changeico( HICON_ONLINE );
	}
}

int	OfflineMainPlayer::getGameState()
{
	return m_GameState;
}

bool OfflineMainPlayer::IsSamePlayer(int i)
{
	if (i == m_SelectRoleindex ) 
	{
		return true;
	}
	return false;
}

OfflineMainPlayer::~OfflineMainPlayer()
{

}

//	离开聊天室
void OfflineMainPlayer::requestLeaveChatRoom()
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	SendGameServerMsg( MUD_OUT_CHAT_ROOM_REQ,msgbody );
}